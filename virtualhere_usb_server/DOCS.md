# Home Assistant App: VirtualHere USB Server

## How to use

1. Install the app.
2. Optionally paste raw VirtualHere `config.ini` content into the `config_ini` option.
3. Start the app.
4. Connect to the server from a VirtualHere client on TCP port `7575`.

## Notes

- The app downloads the official VirtualHere Linux server binary at startup.
- The binary is selected automatically from the container architecture.
- `config.ini` is stored in `/addon_configs/<slug>/config.ini` via the `addon_config` map.
- VirtualHere licensing limits depend on the vendor's license terms.

### Reset USB devices when VirtualHere starts or stops

You can configure the add-on to reset one or more USB devices before VirtualHere starts and/or after VirtualHere stops.

This is useful when a USB device remains locked, unavailable, or does not cleanly return to the system after VirtualHere exits.

The reset implementation does not write to `/sys/bus/usb/drivers/usb/bind` or `/sys/bus/usb/drivers/usb/unbind`. The add-on reads the configured sysfs USB device path, resolves its current `/dev/bus/usb/BBB/DDD` node from `busnum` and `devnum`, then performs the Linux `USBDEVFS_RESET` ioctl using the bundled `/usr/local/bin/usb-reset` helper.

```yaml
options:
  config_ini: ""
  usb_reset_devices:
    - path: "1-1.2"
      vendor: "16d6"
      product: "0008"
      reset_on_start: false
      reset_on_stop: true

    - path: "1-1.3"
      vendor: "1a86"
      product: "55d4"
      reset_on_start: true
      reset_on_stop: false
```

The default reset entry targets the Jablotron JA-100 Flexi USB device shown by `lsusb` as `16d6:0008`. The `path` value is the Linux USB sysfs path under `/sys/bus/usb/devices/`, not the `lsusb` bus/device number. For example, `path: "1-1.2"` means `/sys/bus/usb/devices/1-1.2`.

To confirm or adjust the path, open a host shell and inspect the USB tree:

```sh
lsusb
find /sys/bus/usb/devices -maxdepth 2 -name idVendor -exec sh -c 'for f do d=${f%/*}; printf "%s " "${d##*/}"; cat "$d/idVendor" "$d/idProduct" 2>/dev/null | paste -sd: -; done' sh {} +
```

If a configured vendor or product does not match the live device, the add-on logs a warning and skips that reset entry.
