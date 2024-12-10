
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDR 0x1D
#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 4

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector sectors[NVS_SECTOR_COUNT];

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION);
    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    for (int i = 0; i < NVS_SECTOR_COUNT; i++) {
        sectors[i].fs_sector_size = NVS_SECTOR_SIZE;
        sectors[i].fs_sector_start = fs.offset + i * NVS_SECTOR_SIZE;
    }

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("NVS init failed: %d", err);
    } else {
        LOG_INF("NVS initialized");
    }
}

void read_mma8x5x(const struct device *dev) {
    struct sensor_value accel[3];
    int err;

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Sensor fetch failed: %d", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Sensor channel get failed: %d", err);
        return;
    }

    LOG_INF("Accel X: %d.%06d m/s^2", accel[0].val1, accel[0].val2);
    LOG_INF("Accel Y: %d.%06d m/s^2", accel[1].val1, accel[1].val2);
    LOG_INF("Accel Z: %d.%06d m/s^2", accel[2].val1, accel[2].val2);

    // Store the reading value persistently in NVS
    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));
    err = nvs_write(&fs, 1, data, sizeof(data));
    if (err < 0) {
        LOG_ERR("NVS write failed: %d", err);
    } else {
        LOG_INF("NVS write successful");
    }
}

int main(void) {
    const struct device *dev;

    dev = device_get_binding("MMA8x5x");
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    init_nvs();

    while (1) {
        read_mma8x5x(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
