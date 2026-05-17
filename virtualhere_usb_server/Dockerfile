ARG BUILD_FROM=ghcr.io/home-assistant/base:3.23
FROM ${BUILD_FROM}

RUN apk add --no-cache bash curl ca-certificates libusb \
    && apk add --no-cache --virtual .build-deps gcc musl-dev linux-headers make git pkgconf libusb-dev

COPY usb-reset.c /usr/src/usb-reset.c
RUN gcc -O2 -Wall -Wextra -o /usr/local/bin/usb-reset /usr/src/usb-reset.c

RUN git clone --depth 1 https://github.com/mvp/uhubctl.git /usr/src/uhubctl \
    && make -C /usr/src/uhubctl \
    && install -m 0755 /usr/src/uhubctl/uhubctl /usr/local/bin/uhubctl \
    && apk del .build-deps \
    && rm -rf /usr/src/usb-reset.c /usr/src/uhubctl

COPY rootfs /

LABEL \
  io.hass.type="addon" \
  org.opencontainers.image.title="Home Assistant App: VirtualHere USB Server" \
  org.opencontainers.image.description="Share USB devices over the network using the VirtualHere Linux server." \
  org.opencontainers.image.source="https://www.virtualhere.com/usb_server_software"
