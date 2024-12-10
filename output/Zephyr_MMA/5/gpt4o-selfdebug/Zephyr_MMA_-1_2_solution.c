#include <zephyr/types.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/nvs/nvs.h>

#define SLEEP_TIME_MS   5000

static struct nvs_fs fs;
static struct device *mma8x5x_dev;

void mma8x5x_init(void) {
    mma8x5x_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!mma8x5x_dev) {
        printk("Could not get MMA8x5x device binding\n");
        return;
    }
}

void nvs_init(void) {
    struct flash_sector sector;
    int rc;

    fs.offset = DT_FLASH_AREA_OFFSET(storage);
    rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), fs.offset, &sector);
    if (rc) {
        printk("Unable to get page info\n");
        return;
    }

    fs.sector_size = sector.size;
    fs.sector_count = (DT_FLASH_AREA_SIZE(storage) / sector.size);

    rc = nvs_init(&fs, DT_LABEL(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER));
    if (rc) {
        printk("NVS initialization failed\n");
        return;
    }
}

static void read_sensor_and_store(void) {
    struct sensor_value accel[3];
    int rc = sensor_sample_fetch(mma8x5x_dev);
    if (rc) {
        printk("Failed to fetch sample (%d)\n", rc);
        return;
    }

    rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        printk("Failed to get sensor data (%d)\n", rc);
        return;
    }

    rc = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (rc < 0) {
        printk("Failed to write data to NVS (%d)\n", rc);
    } else {
        printk("Data written to NVS\n");
    }
}

void main(void) {
    printk("MMA8x5x NVS Sample\n");
    nvs_init();
    mma8x5x_init();

    while (1) {
        read_sensor_and_store();
        k_msleep(SLEEP_TIME_MS);
    }
}
