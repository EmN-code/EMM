
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_NAMESPACE "mma8x5x"
#define NVS_KEY "reading"

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_area *fap;

    err = flash_area_open(NVS_PARTITION, &fap);
    if (err) {
        LOG_ERR("Failed to open flash area %d", NVS_PARTITION);
        return;
    }

    fs.flash_device = fap->fa_device;
    fs.offset = fap->fa_off;
    fs.sector_size = 4096; // Adjust based on your flash sector size
    fs.sector_count = 1;   // Adjust based on your flash size

    err = nvs_init(&fs, NVS_NAMESPACE);
    if (err) {
        LOG_ERR("Failed to init NVS (%d)", err);
        return;
    }
}

void store_reading(int32_t reading) {
    int err;
    err = nvs_write(&fs, NVS_KEY, &reading, sizeof(reading));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS (%d)", err);
    } else {
        LOG_INF("Stored reading: %d", reading);
    }
}

void read_mma8x5x(const struct device *dev) {
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

    reading = (accel[0].val1 << 16) | (accel[1].val1 << 8) | accel[2].val1;
    store_reading(reading);
}

int main(void) {
    const struct device *dev;

    dev = device_get_binding("MMA8x5x");
    if (!dev) {
        LOG_ERR("Failed to get MMA8x5x device");
        return -1;
    }

    init_nvs();

    while (1) {
        read_mma8x5x(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
