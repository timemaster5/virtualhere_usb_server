#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/usbdevice_fs.h>

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

    if (sscanf(input, "%d/%d%c", &bus, &dev, &extra) != 2) {
        errno = EINVAL;
        return -1;
    }

    if (bus < 0 || bus > 999 || dev < 0 || dev > 999) {
        errno = EINVAL;
        return -1;
    }

    if (snprintf(output, output_size, "/dev/bus/usb/%03d/%03d", bus, dev) >= (int)output_size) {
        errno = ENAMETOOLONG;
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
    char device_path[128];
    int fd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s /dev/bus/usb/BBB/DDD|BBB/DDD\n", argv[0]);
        return 2;
    }

    if (build_device_path(argv[1], device_path, sizeof(device_path)) != 0) {
        fprintf(stderr, "Invalid USB device '%s': %s\n", argv[1], strerror(errno));
        return 2;
    }

    fd = open(device_path, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "open %s failed: %s\n", device_path, strerror(errno));
        return 1;
    }

    if (ioctl(fd, USBDEVFS_RESET, 0) < 0) {
        fprintf(stderr, "USBDEVFS_RESET %s failed: %s\n", device_path, strerror(errno));
        close(fd);
        return 1;
    }

    close(fd);
    printf("Reset %s OK\n", device_path);
    return 0;
}
