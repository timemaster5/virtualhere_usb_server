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
