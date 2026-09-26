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

### Give USB devices back to Home Assistant when VirtualHere stops

While VirtualHere runs it detaches the kernel drivers from the devices it serves, so an
integration that uses the device directly (for example Jablotron 100 through `hidraw`)
cannot reach it. On stop, for each entry in `usb_reset_devices` with `reset_on_stop: true`,
the app:

1. stops VirtualHere (TERM, then KILL after 10 s),
2. reattaches the kernel drivers to every interface of the device (`usb-reset --connect`,
   the usbfs equivalent of a sysfs `bind`; the app sees `/sys` read-only),
3. resets the device only if an interface is still without a driver, then reattaches again.

Integrations that had the device open reconnect on their own (Jablotron 100 within about
30 s); reload an integration that does not.

```yaml
options:
  usb_reset_devices:
    - vendor: "16d6"        # Jablotron JA-100 Flexi, as shown by lsusb (16d6:0008)
      product: "0008"
      reset_on_stop: true
```

The device is found by `vendor` and `product`. `path` (the sysfs name under
`/sys/bus/usb/devices/`, for example `1-1` or `1-1.2`) is optional: it is used when it
exists and matches, and is needed only when two identical devices are connected.

`reset_on_start: true` resets the device before VirtualHere starts.

`powercycle_on_stop: true` additionally power-cycles the device's hub port with the bundled
`uhubctl`. It needs a hub with per-port power switching and a device behind a hub (a path
with a dot, such as `1-1.2`); a virtual machine's USB controller has none. Off by default.
