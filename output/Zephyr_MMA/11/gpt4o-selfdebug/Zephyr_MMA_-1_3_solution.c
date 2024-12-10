#include <zephyr/sys/mutex.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mem_stats.h>
#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs.h>

#define SLEEP_TIME_MS   5000
#define NVS_ID          1

static const struct device *flash_dev;
struct nvs_fs nvs;

void mma8x5x_read_and_store(void) {
    int rc;
    int sensor_value = 42; // Placeholder for the actual sensor read function

    rc = nvs_write(&nvs, NVS_ID, &sensor_value, sizeof(sensor_value));
    if (rc < 0) {
        printf("Failed to store sensor value: %d\n", rc);
    } else {
        printf("Sensor value %d stored.\n", sensor_value);
    }
}

int main(void) {
    printk("ESP32 MMA8x5x and NVS sample\n");

    flash_dev = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (!device_is_ready(flash_dev)) {
        printf("Flash device %s is not ready\n", flash_dev->name);
        return -1;
    }

    nvs.flash_device = flash_dev;
    nvs.offset = DT_FLASH_AREA_STORAGE_OFFSET;
    nvs.sector_size = DT_FLASH_AREA_STORAGE_SIZE;
    nvs.sector_count = 3;

    int rc = nvs_init(&nvs, DT_LABEL(DT_INST(0, nrf52_flash_controller)));
    if (rc) {
        printf("Flash Init failed: %d\n", rc);
        return -2;
    }

    while (1) {
        mma8x5x_read_and_store();
        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}