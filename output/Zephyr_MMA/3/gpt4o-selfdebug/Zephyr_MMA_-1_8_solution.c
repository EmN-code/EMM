#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs/nvs.h>
#include <zephyr/kernel.h>

#define STORAGE_PARTITION_LABEL storage
#define STORAGE_PARTITION_OFFSET DT_REG_ADDR(DT_NODELABEL(STORAGE_PARTITION_LABEL))
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 3
#define NVS_FLASH_DEVICE DT_CHOSEN_ZEXTRA_FLASH

void main(void) {
    const struct device *sensor_dev;
    struct sensor_value accel_val[3];  // Array for three axes X, Y, Z
    int ret;

    // Initialize the sensor
    sensor_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!sensor_dev) {
        printk("Failed to get sensor device binding\n");
        return;
    }

    // Initialize NVS
    struct nvs_fs fs = {
        .flash_device = DEVICE_DT_GET(NVS_FLASH_DEVICE),
        .storage_offset = STORAGE_PARTITION_OFFSET,
        .sector_size = NVS_SECTOR_SIZE,
        .sector_count = NVS_SECTOR_COUNT,
    };
    nvs_init(&fs, "storage");

    while (1) {
        // Fetch and get sensor data
        ret = sensor_sample_fetch(sensor_dev);
        if (ret) {
            printk("Failed to fetch sensor sample\n");
        } else {
            sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel_val);
            printk("Accel X: %d, Y: %d, Z: %d\n",
                   sensor_value_to_double(&accel_val[0]),
                   sensor_value_to_double(&accel_val[1]),
                   sensor_value_to_double(&accel_val[2]));

            // Store the readings in NVS
            ret = nvs_write(&fs, 1, accel_val, sizeof(accel_val));
            if (ret < 0) {
                printk("Failed to write to NVS\n");
            }
        }

        k_sleep(K_SECONDS(5));
    }
}