
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8X5X_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_NAMESPACE "mma8x5x_data"

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector_param sectors[1];

    sectors[0].fs_size = 4096;
    sectors[0].offset = FLASH_AREA_OFFSET(storage);

    fs.sector_size = 4096;
    fs.sector_count = 1;
    fs.offset = FLASH_AREA_OFFSET(storage);

    err = nvs_init(&fs, DT_LABEL(DT_INST(0, soc_flash_0)));
    if (err) {
        LOG_ERR("NVS init failed: %d", err);
    }
}

void store_reading(int16_t reading) {
    int err;
    static uint16_t id = 0;

    err = nvs_write(&fs, id++, &reading, sizeof(reading));
    if (err < 0) {
        LOG_ERR("NVS write failed: %d", err);
    }
}

void read_mma8x5x(const struct device *dev) {
    struct sensor_value accel[3];
    int16_t reading;

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Failed to get sensor data");
        return;
    }

    reading = (int16_t)(accel[0].val1 + accel[1].val1 + accel[2].val1);
    LOG_INF("MMA8x5x reading: %d", reading);

    store_reading(reading);
}

void main(void) {
    const struct device *dev;

    dev = device_get_binding(MMA8X5X_DEV_NAME);
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return;
    }

    init_nvs();

    while (1) {
        read_mma8x5x(dev);
        k_sleep(K_SECONDS(5));
    }
}
