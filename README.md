# VirtualHere Local Apps Repository

This repository contains a Home Assistant app for running the VirtualHere USB Server.

Copy this whole repository folder into `/addons/local/` on Home Assistant OS / Supervised.
The resulting structure should be:

/addons/local/virtualhere_apps_repo/
  repository.yaml
  virtualhere_usb_server/
    config.yaml
    Dockerfile
    DOCS.md
    CHANGELOG.md
    rootfs/

Then in Home Assistant open **Settings -> Apps -> App Store** and refresh.
