
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8X5X_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_NAMESPACE "mma8x5x_data"
#define NVS_KEY "reading"

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

    err = nvs_init(&fs, DT_LABEL(DT_INST(0, zephyr_flash_controller)));
    if (err) {
        LOG_ERR("NVS init failed: %d", err);
    }
}

void store_reading(int32_t reading) {
    int err;

    err = nvs_write(&fs, NVS_KEY, &reading, sizeof(reading));
    if (err < 0) {
        LOG_ERR("NVS write failed: %d", err);
    } else {
        LOG_INF("Reading stored: %d", reading);
    }
}

void read_sensor(const struct device *dev) {
    struct sensor_value accel[3];
    int32_t reading;

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Sensor sample fetch failed");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Sensor channel get failed");
        return;
    }

    reading = (accel[0].val1 * 1000000 + accel[0].val2) / 1000;
    store_reading(reading);
}

int main(void) {
    const struct device *dev;

    dev = device_get_binding(MMA8X5X_DEV_NAME);
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    init_nvs();

    while (1) {
        read_sensor(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}