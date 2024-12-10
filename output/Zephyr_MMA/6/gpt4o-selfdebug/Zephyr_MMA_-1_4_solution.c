#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs/nvs.h>

#define STORAGE_ID 1
#define STORAGE_MAGIC 0x534F72
#define STORAGE_MAX_SIZE 64

void main(void) {
    struct k_timer timer;
    k_timer_init(&timer, NULL, NULL);

    const struct device *dev = device_get_binding("MMA8X5X");
    if (!dev) {
        printk("Failed to find sensor\n");
        return;
    }

    struct nvs_fs fs;
    struct flash_pages_info info;
    fs.sector_size = DT_FLASH_ERASE_BLOCK_SIZE;
    fs.sector_count = 3U;
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.storage_size = STORAGE_MAX_SIZE;
    nvs_init(&fs, "storage");

    int id = 1;
    while (1) {
        struct sensor_value val;

        if (sensor_sample_fetch(dev) < 0) {
            printk("Sensor fetch failed\n");
            continue;
        }

        if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &val) < 0) {
            printk("Cannot get sensor channel\n");
            continue;
        }

        int ret = nvs_write(&fs, id++, &val, sizeof(val));
        if (ret >= 0) {
            printk("Stored value: %d\n", val.val1);
        } else {
            printk("Failed to store value\n");
        }

        k_sleep(K_SECONDS(5));
    }
}