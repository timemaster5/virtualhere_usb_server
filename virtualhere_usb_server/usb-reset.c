#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/usbdevice_fs.h>

/*
 * usb-reset [--connect|--reset] /dev/bus/usb/BBB/DDD|BBB/DDD
 *
 * --connect  attach kernel drivers to every interface that has none
 * --reset    USBDEVFS_RESET, then --connect (the default)
 *
 * USBDEVFS_CONNECT is only accepted inside USBDEVFS_IOCTL, per interface; it is the
 * usbfs equivalent of writing the interface to /sys/bus/usb/drivers/<driver>/bind,
 * which an add-on cannot do because it sees /sys read-only.
 */

#define MAX_INTERFACES 32

static int build_device_path(const char *input, char *output, size_t output_size) {
    const char *prefix = "/dev/bus/usb/";
    int bus = -1;
    int dev = -1;
    char extra = '\0';

    if (strncmp(input, prefix, strlen(prefix)) == 0) {
        if (snprintf(output, output_size, "%s", input) >= (int)output_size) {
            errno = ENAMETOOLONG;
            return -1;
        }
        return 0;
    }

    if (sscanf(input, "%d/%d%c", &bus, &dev, &extra) != 2 || bus < 0 || bus > 999 || dev < 0 || dev > 999) {
        errno = EINVAL;
        return -1;
    }

    if (snprintf(output, output_size, "/dev/bus/usb/%03d/%03d", bus, dev) >= (int)output_size) {
        errno = ENAMETOOLONG;
        return -1;
    }

    return 0;
}

/* Returns the number of interfaces found, or -1 if the first one could not be reached. */
static int connect_interfaces(int fd, const char *device_path) {
    int found = 0;

    for (int ifno = 0; ifno < MAX_INTERFACES; ifno++) {
        struct usbdevfs_ioctl command = {.ifno = ifno, .ioctl_code = USBDEVFS_CONNECT, .data = NULL};

        if (ioctl(fd, USBDEVFS_IOCTL, &command) < 0) {
            if (errno == EINVAL) {
                continue; /* no interface with this number in the active configuration */
            }
            fprintf(stderr, "USBDEVFS_CONNECT %s interface %d failed: %s\n", device_path, ifno, strerror(errno));
            continue;
        }

        found++;
        printf("Kernel drivers attached to %s interface %d\n", device_path, ifno);
    }

    if (found == 0) {
        fprintf(stderr, "No interface of %s accepted USBDEVFS_CONNECT\n", device_path);
        return -1;
    }

    return found;
}

int main(int argc, char **argv) {
    char device_path[128];
    const char *target;
    int reset = 1;
    int fd;
    int status = 0;

    if (argc == 3 && strcmp(argv[1], "--connect") == 0) {
        reset = 0;
        target = argv[2];
    } else if (argc == 3 && strcmp(argv[1], "--reset") == 0) {
        target = argv[2];
    } else if (argc == 2) {
        target = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [--connect|--reset] /dev/bus/usb/BBB/DDD|BBB/DDD\n", argv[0]);
        return 2;
    }

    if (build_device_path(target, device_path, sizeof(device_path)) != 0) {
        fprintf(stderr, "Invalid USB device '%s': %s\n", target, strerror(errno));
        return 2;
    }

    fd = open(device_path, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open %s failed: %s\n", device_path, strerror(errno));
        return 1;
    }

    if (reset) {
        if (ioctl(fd, USBDEVFS_RESET, 0) < 0) {
            fprintf(stderr, "USBDEVFS_RESET %s failed: %s\n", device_path, strerror(errno));
            status = 1;
        } else {
            printf("Reset %s OK\n", device_path);
            sleep(1);
        }
    }

    if (connect_interfaces(fd, device_path) < 0) {
        status = 1;
    }

    close(fd);
    return status;
}
