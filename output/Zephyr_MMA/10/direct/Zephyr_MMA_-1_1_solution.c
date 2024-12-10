
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_ALIAS(mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector_param sectors[1];

    sectors[0].fs_size = 4096;
    sectors[0].fs_addr = FLASH_AREA_OFFSET(storage);

    fs.sector_size = 4096;
    fs.sector_count = 1;
    fs.offset = FLASH_AREA_OFFSET(storage);

    err = nvs_init(&fs, DT_LABEL(DT_INST(0, zephyr_nvs)));
    if (err) {
        LOG_ERR("NVS init failed: %d", err);
        return;
    }

    LOG_INF("NVS initialized");
}

void store_reading(int32_t reading) {
    int err;
    err = nvs_write(&fs, 1, &reading, sizeof(reading));
    if (err < 0) {
        LOG_ERR("NVS write failed: %d", err);
    } else {
        LOG_INF("Reading stored: %d", reading);
    }
}

void read_sensor(const struct device *dev) {
    struct sensor_value accel[3];
    int32_t reading;

    if (sensor_sample_fetch(dev)) {
        LOG_ERR("Sensor sample fetch failed");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel)) {
        LOG_ERR("Sensor channel get failed");
        return;
    }

    reading = (accel[0].val1 * 1000000 + accel[0].val2) / 1000;
    store_reading(reading);
}

int main(void) {
    const struct device *dev;

    dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return -ENODEV;
    }

    init_nvs();

    while (1) {
        read_sensor(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
