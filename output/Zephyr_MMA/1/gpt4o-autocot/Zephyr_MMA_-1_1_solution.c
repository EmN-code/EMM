
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define NVS_PARTITION_ID 1
#define NVS_STORAGE_ID 1

static struct nvs_fs fs;

void read_mma8x5x(const struct device *dev, struct sensor_value *accel) {
    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Failed to get sensor data");
        return;
    }
}

void store_reading(struct sensor_value *accel) {
    int rc;
    uint8_t data[sizeof(struct sensor_value) * 3];
    memcpy(data, accel, sizeof(struct sensor_value) * 3);

    rc = nvs_write(&fs, NVS_STORAGE_ID, data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS: %d", rc);
    } else {
        LOG_INF("Data stored successfully");
    }
}

int main(void) {
    const struct device *mma8x5x_dev = device_get_binding("MMA8x5x");
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    if (!device_is_ready(mma8x5x_dev)) {
        LOG_ERR("MMA8x5x device is not ready");
        return -1;
    }

    struct sensor_value accel[3];

    fs.sector_size = 4096;
    fs.sector_count = 2;
    fs.offset = FLASH_AREA_OFFSET(storage);

    int rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("Failed to mount NVS: %d", rc);
        return -1;
    }

    while (1) {
        read_mma8x5x(mma8x5x_dev, accel);
        store_reading(accel);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
