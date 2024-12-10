
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_ALIAS(mma8x5x)
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

    err = nvs_init(&fs, "storage");
    if (err) {
        LOG_ERR("NVS init failed: %d", err);
    }
}

void store_reading(int32_t reading) {
    int err;
    err = nvs_write(&fs, 1, &reading, sizeof(reading));
    if (err < 0) {
        LOG_ERR("NVS write failed: %d", err);
    }
}

int main(void) {
    const struct device *dev;
    struct sensor_value accel[3];
    int32_t reading;

    dev = DEVICE_DT_GET(MMA8x5x_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return 0;
    }

    init_nvs();

    while (1) {
        if (sensor_sample_fetch(dev) < 0) {
            LOG_ERR("Failed to fetch sample");
        } else {
            sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
            reading = (accel[0].val1 << 16) | (accel[1].val1 << 8) | accel[2].val1;
            store_reading(reading);
            LOG_INF("Stored reading: %d", reading);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
