
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/kernel.h>
#include <time.h>

#define MMA8X5X_DEVICE_NAME "MMA8X5X"
#define NVS_PARTITION_LABEL "storage"
#define NVS_ID 1

struct nvs_fs fs;

void read_mma8x5x(void) {
    const struct device *dev = device_get_binding(MMA8X5X_DEVICE_NAME);
    if (!dev) {
        printk("Error: could not get device binding for MMA8X5X\n");
        return;
    }

    struct sensor_value accel[3];
    if (sensor_sample_fetch(dev) < 0) {
        printk("Error: could not fetch sample from MMA8X5X\n");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        printk("Error: could not get sensor channel from MMA8X5X\n");
        return;
    }

    printk("MMA8X5X: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the reading value persistently using NVS
    ssize_t ret = nvs_write(&fs, NVS_ID, &accel, sizeof(accel));
    if (ret < 0) {
        printk("Error: could not write to NVS, error code %d\n", (int)ret);
    } else {
        printk("NVS write successful, %d bytes written\n", (int)ret);
    }
}

int main(void) {
    // Initialize NVS
    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_LABEL);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_LABEL);
    fs.sector_size = 4096; // Example sector size, adjust as needed
    fs.sector_count = 8;   // Example sector count, adjust as needed

    if (nvs_mount(&fs) < 0) {
        printk("Error: could not mount NVS\n");
        return -1;
    }

    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
