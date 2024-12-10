#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs/nvs.h>

#define STORAGE_PARTITION_ID  FIXED_PARTITION_ID(storage)
#define NVS_SECTOR_SIZE       0x4000
#define NVS_SECTOR_COUNT      3
#define READ_INTERVAL         K_SECONDS(5)

static struct nvs_fs fs;
static struct device *flash_dev;

static void init_nvs(void) {
    struct flash_pages_info info;

    flash_dev = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER);
    if (!device) {
        printk("Flash device not found!");
        return;
    }

    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    fs.flash_device = flash_dev;

    int err = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER);
    if (err) {
        printk("NVS init failed: %d", err);
        return;
    }
}

static void write_mma_value(uint16_t value) {
    int err = nvs_write(&fs, 1, &value, sizeof(value));
    if (err < 0) {
        printk("Failed to write to NVS: %d", err);
    }
}

void main(void) {
    printk("Starting MMA8x5x reader\n");

    init_nvs();

    while (1) {
        uint16_t sensor_value = 123; // Simulated value

        printk("Read sensor value: %d\n", sensor_value);

        write_mma_value(sensor_value);

        k_sleep(READ_INTERVAL);
    }
}

/* Ensure the CMake file links the NVS and FLASH components */
# Add these lines in your CMakeLists.txt
# find_package(Zephyr REQUIRED)
# project(my_project)
# target_link_libraries(app PRIVATE zephyr_interface)