
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8X5X_I2C_ADDRESS 0x1D
#define NVS_PARTITION_LABEL storage
#define SLEEP_TIME_MS 5000

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_LABEL);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_LABEL);
    fs.sector_size = 4096; // Example sector size, adjust as needed
    fs.sector_count = 2; // Example sector count, adjust as needed

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
    } else {
        LOG_INF("NVS mounted successfully");
    }
}

void read_mma8x5x(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Sensor fetch failed: %d", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Sensor channel get failed: %d", ret);
        return;
    }

    LOG_INF("Accel X: %d.%06d m/s^2", accel[0].val1, accel[0].val2);
    LOG_INF("Accel Y: %d.%06d m/s^2", accel[1].val1, accel[1].val2);
    LOG_INF("Accel Z: %d.%06d m/s^2", accel[2].val1, accel[2].val2);

    // Store the read value persistently in NVS
    ret = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("NVS write successful");
    }
}

int main(void) {
    const struct device *dev;

    dev = device_get_binding("MMA8X5X");
    if (!dev) {
        LOG_ERR("Could not get MMA8X5X device");
        return -1;
    }

    init_nvs();

    while (1) {
        read_mma8x5x(dev);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
