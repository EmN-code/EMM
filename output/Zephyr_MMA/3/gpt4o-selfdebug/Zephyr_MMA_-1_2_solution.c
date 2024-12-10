#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <inttypes.h>

#define STORAGE_ID 1
#define SLEEP_TIME_MS 5000

static struct nvs_fs fs;

void init_nvs(void) {
    struct flash_pages_info info;

    fs.offset = DT_FLASH_AREA_STORAGE_OFFSET;
    fs.sector_size = DT_FLASH_AREA_STORAGE_SIZE;
    fs.sector_count = 3;

    if (flash_get_page_info_by_offs(device_get_binding(DT_FLASH_DEV_NAME), fs.offset, &info) != 0) {
        printk("Flash page info failed.\n");
        return;
    }

    nvs_init(&fs, DT_FLASH_DEV_NAME);
}

void read_sensor_and_store(void) {
    const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!dev) {
        printk("Could not get MMA8x5x sensor device\n");
        return;
    }

    struct sensor_value val;
    while (1) {
        if (sensor_sample_fetch(dev) < 0) {
            printk("Failed to fetch sensor sample\n");
            return;
        }
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &val);
        printk("Acceleration X: %"PRIi32"\n", val.val1);

        ssize_t bytes_written = nvs_write(&fs, STORAGE_ID, &val.val1, sizeof(val.val1));
        if (bytes_written < 0) {
            printk("Failed to write to NVS\n");
            return;
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}

int main(void) {
    init_nvs();
    read_sensor_and_store();
    return 0;
}