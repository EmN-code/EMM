
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8X5X_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_SECTOR_SIZE 4096

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector_param sector[1];

    fs.flash_device = NULL;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = 1;

    err = flash_map_get_device(NVS_PARTITION_ID, &fs.flash_device);
    if (err) {
        LOG_ERR("Failed to get flash device: %d", err);
        return;
    }

    sector[0].fs_sector_id = 0;
    sector[0].fs_sector_size = NVS_SECTOR_SIZE;

    err = nvs_init(&fs, sector);
    if (err) {
        LOG_ERR("Failed to init NVS: %d", err);
        return;
    }

    LOG_INF("NVS initialized successfully");
}

void store_reading(int32_t reading) {
    int err;
    uint16_t id = 1;

    err = nvs_write(&fs, id, &reading, sizeof(reading));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS: %d", err);
    } else {
        LOG_INF("Reading stored successfully: %d", reading);
    }
}

void read_sensor(const struct device *dev) {
    struct sensor_value accel[3];
    int32_t reading;

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Failed to get sensor data");
        return;
    }

    reading = (accel[0].val1 * 1000000 + accel[0].val2) / 1000;
    store_reading(reading);
}

void main(void) {
    const struct device *dev;

    dev = device_get_binding(MMA8X5X_DEV_NAME);
    if (!dev) {
        LOG_ERR("Failed to get device binding");
        return;
    }

    init_nvs();

    while (1) {
        read_sensor(dev);
        k_sleep(K_SECONDS(5));
    }
}
