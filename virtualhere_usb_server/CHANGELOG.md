# Changelog

## 0.4.1

- VirtualHere is stopped with SIGTERM again: it ignores SIGINT, which only added 5 s.
  SIGKILL after 10 s stays as the fallback.
- `USBDEVFS_CONNECT` answering `EBUSY` counts as "driver already bound"; whether the
  device is back is decided by the script's check of `/sys` afterwards, as before.
- Log lines name the device by `vendor:product` when no `path` is set.
- Tested with a client using a Jablotron JA-100 while the app stops: VirtualHere stopped in
  about 1 s, the HID interface got its driver back, and the Jablotron 100 integration
  reconnected by itself within 30 s.

## 0.4.0

- Fixed: on stop, the device now really goes back to its kernel drivers. VirtualHere
  detaches them; the helper reattaches each interface with `USBDEVFS_CONNECT` inside
  `USBDEVFS_IOCTL` (the usbfs equivalent of a sysfs `bind`, which an add-on cannot write).
  Before, `USBDEVFS_DISCONNECT`/`CONNECT` were called directly on the device, which the
  kernel rejects, so a HID device such as the Jablotron JA-100 was left with no `hidraw`
  node. A reset now runs only if an interface is still without a driver.
- The device is found by `vendor` and `product`; `path` is optional and only a hint.
- Defaults: `reset_on_stop: true`, `powercycle_on_stop: false` (port power switching is
  rarely available, and never on a virtual machine's USB controller), no `path`.
- `usb-reset` takes `--connect` (attach drivers only) or `--reset` (reset, then attach).
- VirtualHere is stopped with SIGINT (its CTRL-C), which lets it hand its devices back;
  SIGTERM after 5 s and SIGKILL after 10 s are fallbacks. Before, it got SIGTERM and was
  usually killed after 10 s.
- Shorter stop: s6 grace periods 25 s for services and 3 s for leftover processes (were
  60 s each, which could run into Home Assistant's own stop timeout); app timeout 40 s.
- The log states how long stopping VirtualHere and the whole shutdown took.

## 0.3.7

- Improved the bundled `usb-reset` helper to try a stronger USB cleanup sequence: `USBDEVFS_DISCONNECT`, `USBDEVFS_RESET`, then `USBDEVFS_CONNECT`.
- Keeps the plain USB reset path as a fallback when disconnecting the device from kernel drivers is not supported.
- No configuration changes are required; existing `reset_on_start` and `reset_on_stop` options use the improved helper automatically.

## 0.3.6

- Extended the Home Assistant add-on shutdown timeout to give stop-time USB reset and port power-cycle actions time to complete.
- Increased s6 service shutdown grace periods so the service script is not killed before cleanup actions run.

## 0.3.5

- Added USB port power cycling on add-on stop using bundled `uhubctl`.
- Added `powercycle_on_stop` and `powercycle_delay` options to each `usb_reset_devices` entry.
- Enabled port power cycling by default for the Jablotron JA-100 Flexi target, using vendor `16d6`, product `0008`, and path `1-1.2`.
- Updated shutdown handling so VirtualHere is stopped before running USB reset and power-cycle actions.
- Kept the existing configurable USB reset behavior for multiple devices on add-on start and/or stop.

## 0.3.0

- Added a bundled `usb-reset` helper that resets configured USB devices through the Linux `USBDEVFS_RESET` ioctl on `/dev/bus/usb`.
- Kept configurable `usb_reset_devices` support for resetting one or more devices on add-on start and/or stop.
- Preserved the default Jablotron JA-100 Flexi reset target, using vendor `16d6` and product `0008`.
- Updated reset handling to resolve each configured sysfs USB path to the current `/dev/bus/usb/BBB/DDD` node before resetting.
- Avoids relying on writable `/sys/bus/usb/drivers/usb/bind` and `unbind` for reset behavior.

## 0.2.0

- Reworked into a repository layout based on `home-assistant/apps-example`.
- Uses a Home Assistant base image with an s6 service.
- Downloads the official VirtualHere server binary at startup based on runtime architecture.
