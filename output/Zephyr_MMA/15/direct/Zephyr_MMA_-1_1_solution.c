
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEV_NAME "MMA8x5x"
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 2

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector_param sector[NVS_SECTOR_COUNT];

    fs.flash_device = NULL;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    err = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL),
                                      FIXED_PARTITION_OFFSET(storage), &sector[0]);
    if (err) {
        LOG_ERR("Failed to get flash sector info (err %d)", err);
        return;
    }

    fs.offset = sector[0].fs_offset;

    err = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (err) {
        LOG_ERR("Failed to init NVS (err %d)", err);
    }
}

void store_reading(int32_t reading) {
    int err;
    err = nvs_write(&fs, 0, &reading, sizeof(reading));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", err);
    } else {
        LOG_INF("Stored reading: %d", reading);
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

    reading = (accel[0].val1 * 1000000 + accel[0].val2) / 1000; // Convert to milli-g
    store_reading(reading);
}

void main(void) {
    const struct device *dev;

    dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (dev == NULL) {
        LOG_ERR("Failed to get device binding");
        return;
    }

    init_nvs();

    while (1) {
        read_sensor(dev);
        k_sleep(K_SECONDS(5));
    }
}
