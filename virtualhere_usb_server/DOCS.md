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

```yaml
options:
  config_ini: ""
  usb_reset_devices:
    - path: "1-1.2"
      vendor: "16d6"
      product: "0008"
      reset_on_start: true
      reset_on_stop: true

    - path: "1-1.3"
      vendor: "1a86"
      product: "55d4"
      reset_on_start: true
      reset_on_stop: false