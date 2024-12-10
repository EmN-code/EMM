#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/device.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/kernel.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/util.h>
#include <stdio.h>

#define STORAGE_PARTITION_ID 1

static struct nvs_fs fs;

void init_nvs(void) {
    int rc;
    struct flash_area *fa;

    rc = flash_area_open(STORAGE_PARTITION_ID, &fa);
    if (rc < 0) {
        printf("Failed to open flash area (%d)\n", rc);
        return;
    }

    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size / 8;
    fs.sector_count = 8;

    rc = nvs_mount(&fs);
    if (rc) {
        printf("Flash Init failed (%d)\n", rc);
    } else {
        printf("NVS mounted at offset %lu\n", fs.offset);
    }
}

void read_sensor_value(void) {
    int rc;
    u32_t timestamp = k_uptime_get_32();
    u8_t sensor_data[6]; // Assuming 6 bytes of sensor data
    
    // Simulate reading from the sensor
    for (int i = 0; i < 6; i++) {
        sensor_data[i] = (u8_t)(timestamp >> (i * 4));
    }
    rc = nvs_write(&fs, timestamp, sensor_data, sizeof(sensor_data));
    if (rc < 0) {
        printf("Failed to write data (%d)\n", rc);
    }
}

void main(void) {
    init_nvs();
    while (1) {
        read_sensor_value();
        k_sleep(K_SECONDS(5));
    }
}
