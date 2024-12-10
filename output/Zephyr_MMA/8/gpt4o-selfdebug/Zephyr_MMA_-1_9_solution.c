#include <zephyr/sys/reboot.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash_map_backend/flash_map_backend.h>
#include <zephyr/storage/nvs/nvs.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>

#define STORAGE_ID 1
#define STORAGE_FLASH_PARTITION "storage"

#define BUFFER_SIZE 64
static struct nvs_fs fs;
static struct device *flash_dev;

void sensor_read(void)
{
    // Mock reading from MMA8x5x sensor
    int reading = 123;  // Example reading
    printk("Sensor reading: %d\n", reading);

    // Writing to NVS
    off_t offset = 0;
    nvs_write(&fs, offset, &reading, sizeof(reading));

    printk("Writing sensor data to NVS done\n");
}

int main(void)
{
    printk("Starting Zephyr application\n");

    flash_dev = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER);
    if (!flash_dev) {
        printk("Error getting flash device\n");
        return -1;
    }

    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.dev_name = flash_dev->name;

    int rc = nvs_init(&fs, flash_dev->name);
    if (rc) {
        printk("Error initializing NVS \n");
        return -1;
    }

    while (1) {
        sensor_read();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}