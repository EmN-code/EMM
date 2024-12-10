
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_SECTOR_COUNT 4

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector sectors[NVS_SECTOR_COUNT];

    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_ID);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = FIXED_PARTITION_SIZE(NVS_PARTITION_ID);
    fs.sector_count = NVS_SECTOR_COUNT;

    err = flash_get_sectors(fs.flash_device, fs.sector_count, sectors);
    if (err) {
        LOG_ERR("Failed to get flash sectors: %d", err);
        return;
    }

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("Failed to init NVS: %d", err);
        return;
    }
}

void read_mma8x5x(const struct device *dev) {
    struct sensor_value accel[3];
    int err;

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Failed to fetch sample: %d", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Failed to get sensor data: %d", err);
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the read value persistently in NVS
    uint8_t data[3];
    data[0] = accel[0].val1;
    data[1] = accel[1].val1;
    data[2] = accel[2].val1;

    err = nvs_write(&fs, 1, data, sizeof(data));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS: %d", err);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void) {
    const struct device *dev = device_get_binding("MMA8x5x");
    if (!dev) {
        LOG_ERR("Device MMA8x5x not found");
        return -1;
    }

    init_nvs();

    while (1) {
        read_mma8x5x(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
