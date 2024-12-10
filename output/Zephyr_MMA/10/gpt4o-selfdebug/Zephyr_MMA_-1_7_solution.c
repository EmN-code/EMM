#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash.h>
#include <zephyr/storage/nvs.h>

#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 3
#define NVS_OFFSET DT_FLASH_AREA_STORAGE_OFFSET

void read_mma8x5x_and_store(const struct device *sensor, struct nvs_fs *fs)
{
    struct sensor_value value;
    if (sensor_sample_fetch(sensor) < 0) {
        printk("Failed to fetch sample\n");
        return;
    }
    if (sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &value) < 0) {
        printk("Failed to get sensor channel\n");
        return;
    }
    printk("Accelerometer reading: x=%d, y=%d, z=%d\n",
           value.val1, value.val1, value.val1);

    int rc = nvs_write(fs, 1, &value, sizeof(value));
    if (rc < 0) {
        printk("Failed to write to NVS\n");
    }
}

void main(void)
{
    const struct device *sensor = DEVICE_DT_GET_ANY(nxpmotion_mma8x5x);
    if (sensor == NULL || !device_is_ready(sensor)) {
        printk("Could not get MMA8X5X device\n");
        return;
    }

    struct nvs_fs fs;
    struct flash_pages_info info;

    fs.offset = NVS_OFFSET;

    if (flash_get_page_info_by_idx(device_get_binding(DT_FLASH_AREA_STORAGE_LABEL),
                                   0, &info) != 0) {
        printk("Failed to get page info\n");
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = NVS_SECTOR_COUNT;

    if (nvs_init(&fs, DT_FLASH_AREA_STORAGE_LABEL) != 0) {
        printk("Cannot initialize NVS\n");
        return;
    }

    while (1) {
        read_mma8x5x_and_store(sensor, &fs);
        k_sleep(K_SECONDS(5));
    }
}