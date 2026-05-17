ARG BUILD_FROM=ghcr.io/home-assistant/base:3.23
FROM ${BUILD_FROM}

RUN apk add --no-cache bash curl ca-certificates \
    && apk add --no-cache --virtual .build-deps gcc musl-dev linux-headers

COPY usb-reset.c /usr/src/usb-reset.c
RUN gcc -O2 -Wall -Wextra -o /usr/local/bin/usb-reset /usr/src/usb-reset.c \
    && apk del .build-deps \
    && rm -f /usr/src/usb-reset.c

COPY rootfs /

LABEL \
  io.hass.type="addon" \
  org.opencontainers.image.title="Home Assistant App: VirtualHere USB Server" \
  org.opencontainers.image.description="Share USB devices over the network using the VirtualHere Linux server." \
  org.opencontainers.image.source="https://www.virtualhere.com/usb_server_software"
