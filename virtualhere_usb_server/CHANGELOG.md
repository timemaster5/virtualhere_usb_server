# Changelog

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
