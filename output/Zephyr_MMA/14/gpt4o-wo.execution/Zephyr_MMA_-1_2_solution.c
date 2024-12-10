
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/coap.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/mqtt_sn.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION   FIXED_PARTITION_ID(storage)

struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = NVS_PARTITION;
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    int ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
    } else {
        printk("NVS mounted successfully\n");
    }
}

void read_sensor_and_store(void) {
    // Assuming MMA8x5x sensor is initialized and available
    // This is a placeholder for actual sensor reading logic
    int sensor_value = 0; // Placeholder for sensor reading

    ssize_t ret = nvs_write(&fs, 1, &sensor_value, sizeof(sensor_value));
    if (ret < 0) {
        printk("NVS write failed: %zd\n", ret);
    } else {
        printk("Sensor value %d stored successfully\n", sensor_value);
    }
}

int main(void) {
    init_nvs();

    while (1) {
        read_sensor_and_store();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
