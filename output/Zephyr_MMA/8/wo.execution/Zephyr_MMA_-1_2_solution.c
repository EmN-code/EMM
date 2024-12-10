
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

struct nvs_fs fs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .offset = FIXED_PARTITION_OFFSET(storage),
};

void read_mma8x5x(int *value) {
    // Simulated function to read from MMA8x5x sensor
    *value = 42; // Placeholder value
}

void store_value_in_nvs(int value) {
    struct nvs_fs *fs_ptr = &fs;
    int rc;

    rc = nvs_init(fs_ptr, NVS_PARTITION_ID);
    if (rc) {
        printk("NVS init failed: %d\n", rc);
        return;
    }

    rc = nvs_write(fs_ptr, 0, &value, sizeof(value));
    if (rc < 0) {
        printk("NVS write failed: %d\n", rc);
    } else {
        printk("Value stored: %d\n", value);
    }
}

int main(void) {
    int sensor_value;

    while (1) {
        read_mma8x5x(&sensor_value);
        store_value_in_nvs(sensor_value);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
