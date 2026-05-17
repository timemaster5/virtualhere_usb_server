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

### Reset and power cycle USB devices when VirtualHere starts or stops

You can configure the add-on to reset one or more USB devices before VirtualHere starts and/or after VirtualHere stops.

This is useful when a USB device remains locked, unavailable, or does not cleanly return to the system after VirtualHere exits.

The reset implementation reads the configured sysfs USB device path, resolves its current `/dev/bus/usb/BBB/DDD` node from `busnum` and `devnum`, then performs the Linux `USBDEVFS_RESET` ioctl using the bundled `/usr/local/bin/usb-reset` helper.

On add-on stop, each device can also power cycle its parent hub port using bundled `uhubctl`. The add-on derives the parent hub and port from the configured sysfs path. For example, `1-1.2` becomes hub `1-1`, port `2`. Port power cycling only works when the parent USB hub supports per-port power switching.

```yaml
options:
  config_ini: ""
  usb_reset_devices:
    - path: "1-1.2"
      vendor: "16d6"
      product: "0008"
      reset_on_start: false
      reset_on_stop: true
      powercycle_on_stop: true
      powercycle_delay: 5

    - path: "1-1.3"
      vendor: "1a86"
      product: "55d4"
      reset_on_start: true
      reset_on_stop: false
      powercycle_on_stop: false
      powercycle_delay: 5
```

The default reset entry targets the Jablotron JA-100 Flexi USB device shown by `lsusb` as `16d6:0008`. The `path` value is the Linux USB sysfs path under `/sys/bus/usb/devices/`, not the `lsusb` bus/device number. For example, `path: "1-1.2"` means `/sys/bus/usb/devices/1-1.2`.

To confirm or adjust the path, open a host shell and inspect the USB tree:

```sh
lsusb
find /sys/bus/usb/devices -maxdepth 2 -name idVendor -exec sh -c 'for f do d=${f%/*}; printf "%s " "${d##*/}"; cat "$d/idVendor" "$d/idProduct" 2>/dev/null | paste -sd: -; done' sh {} +
```

If a configured vendor or product does not match the live device, the add-on logs a warning and skips that reset or power-cycle entry.

To check whether port power control is available for the parent hub, start the add-on and inspect the logs from `uhubctl`. Some hubs report switching support but do not physically remove VBUS power from the port; in that case the USB reset still runs, but the power cycle may not recover a deeply wedged device.
