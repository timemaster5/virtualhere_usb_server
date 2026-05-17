# Changelog

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
