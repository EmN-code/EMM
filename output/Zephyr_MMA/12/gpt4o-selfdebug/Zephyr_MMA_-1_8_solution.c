#include <zephyr/types.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/nvs.h>
#include <stdint.h>
#include <device.h>
#include <devicetree.h>
#include <stdio.h>

// Main function to execute sensor read and NVS store
int main(void) {
    const struct device *sensor_dev;
    struct sensor_value accel[3];
    struct nvs_fs nvs;
    struct flash_area *fa;
    int err;

    // Initialize the flash area
    err = flash_area_open(FLASH_AREA_ID(storage), &fa);
    if (err) {
        printf("Failed to open flash area\n");
        return err;
    }

    // Initialize NVS
    nvs.offset = fa->fa_off;
    nvs.sector_size = CONFIG_FLASH_ERASE_SIZE;
    nvs.sector_count = 3U;
    err = nvs_init(&nvs, "storage");
    if (err) {
        printf("NVS initialization failed\n");
        return err;
    }

    // Fetch the sensor device
    sensor_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!sensor_dev) {
        printf("No device found\n");
        return -1;
    }

    while (1) {
        // Sample accelerometer data
        sensor_sample_fetch(sensor_dev);
        sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);

        printf("Accel X: %d, Y: %d, Z: %d\n",
            accel[0].val1, accel[1].val1, accel[2].val1);

        // Write data to NVS
        err = nvs_write(&nvs, 1, &accel, sizeof(accel));
        if (err < 0) {
            printf("Failed to write acceleration data to NVS\n");
        } else {
            printf("Acceleration data stored\n");
        }

        k_sleep(K_SECONDS(5));
    }
    return 0;
}