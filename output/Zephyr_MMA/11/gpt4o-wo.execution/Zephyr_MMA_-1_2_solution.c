
#include <zephyr/sys/device_mmio.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/linker/sections.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>

#define SLEEP_TIME_MS   5000
#define MMA8X5X_NODE DT_ALIAS(mma8x5x)
#define FLASH_AREA_STORAGE_ID DT_FLASH_AREA_STORAGE_ID

static struct nvs_fs fs;

void init_nvs(void) {
    struct flash_area *fa;
    int rc;

    rc = flash_area_open(FLASH_AREA_STORAGE_ID, &fa);
    if (rc) {
        printk("Failed to open flash area\n");
        return;
    }

    fs.flash_device = (const struct device *)fa->fa_dev;
    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;

    rc = nvs_init(&fs, fs.flash_device);
    if (rc) {
        printk("Failed to init NVS\n");
        return;
    }
}

void read_mma8x5x(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Device MMA8X5X is not ready\n");
        return;
    }

    struct sensor_value accel[3];
    int rc;

    rc = sensor_sample_fetch(dev);
    if (rc) {
        printk("Failed to fetch sample from MMA8X5X\n");
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        printk("Failed to get channel data from MMA8X5X\n");
        return;
    }

    printk("Accel: x=%d.%06d, y=%d.%06d, z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the reading value persistently
    uint16_t id = 1;
    rc = nvs_write(&fs, id, &accel, sizeof(accel));
    if (rc < 0) {
        printk("Failed to write to NVS\n");
    } else {
        printk("Data written to NVS\n");
    }
}

int main(void) {
    init_nvs();

    while (1) {
        read_mma8x5x();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
