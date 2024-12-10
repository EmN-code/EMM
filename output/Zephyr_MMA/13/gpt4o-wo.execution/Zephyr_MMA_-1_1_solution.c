
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
#define NVS_SECTOR_COUNT 4
#define NVS_SECTOR_SIZE 4096

static struct nvs_fs nvs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_SECTOR_COUNT,
    .offset = FLASH_AREA_OFFSET(storage),
};

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

void store_reading_in_nvs(struct sensor_value *accel) {
    int rc;
    struct nvs_fs *fs = &nvs;

    rc = nvs_init(fs, "storage");
    if (rc) {
        LOG_ERR("NVS init failed: %d", rc);
        return;
    }

    uint8_t data[12];
    memcpy(data, &accel->val1, sizeof(int32_t));
    memcpy(data + 4, &accel->val2, sizeof(int32_t));
    memcpy(data + 8, &accel->val3, sizeof(int32_t));

    rc = nvs_write(fs, 1, data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("NVS write failed: %d", rc);
    } else {
        LOG_INF("NVS write success");
    }
}

int main(void) {
    const struct device *mma8x5x_dev = device_get_binding("MMA8x5x");
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    struct sensor_value accel;

    while (1) {
        read_mma8x5x(mma8x5x_dev, &accel);
        store_reading_in_nvs(&accel);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
