#include <zephyr/storage/flash_map.h>
#include <zephyr/settings/settings.h>
#include <zephyr/nvs/nvs.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define STORAGE_FLASH_LABEL DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL
#define STORAGE_FLASH_PAGE_SIZE DT_PROP(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER, erase_block_size)
#define STORAGE_FLASH_SIZE (STORAGE_FLASH_PAGE_SIZE * 3)

static struct nvs_fs fs;

void init_nvs_storage() {
    int err;
    struct flash_pages_info info;

    fs.offset = DT_FLASH_AREA_STORAGE_OFFSET;
    fs.sector_size = STORAGE_FLASH_PAGE_SIZE;
    fs.sector_count = STORAGE_FLASH_SIZE / fs.sector_size;
    err = nvs_init(&fs, STORAGE_FLASH_LABEL);
    if (err) {
        printk("NVS initialization failed\n");
    } else {
        printk("NVS initialized\n");
    }
}

int main() {
    const struct device *mma8x5x = DEVICE_DT_GET(DT_NODELABEL(mma8x5x));
    if (!device_is_ready(mma8x5x)) {
        printk("MMA8x5x device not found\n");
        return -1;
    }

    init_nvs_storage();

    while (1) {
        // Code to read from the MMA8x5x sensor
        // Assuming the sensor reading is stored in 'sensor_value'
        uint32_t sensor_value = 0; // Placeholder for actual sensor reading logic

        // Store the reading persistently in NVS
        ssize_t written = nvs_write(&fs, 1, &sensor_value, sizeof(sensor_value));
        if (written < 0) {
            printk("Error writing to NVS: %d\n", written);
        } else {
            printk("Sensor value %d written to NVS\n", sensor_value);
        }

        // Sleep for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    return 0;
}