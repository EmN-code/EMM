
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_I2C_BUS DT_LABEL(DT_NODELABEL(i2c0))

#define NVS_PARTITION_ID FIXED_PARTITION_ID(DT_COMPAT_fixed_partitions_LABEL_1_PARENT_PARENT_ORD)

static struct nvs_fs fs;

void init_nvs(void) {
    int ret;
    struct flash_sector_param info;

    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_ID);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = info.fs_size;
    fs.sector_count = 1;

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
    }
}

void read_mma8x5x(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Sensor sample fetch failed: %d", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Sensor channel get failed: %d", ret);
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the values in NVS
    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));
    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    }
}

int main(void) {
    const struct device *dev;

    dev = device_get_binding(MMA8x5x_I2C_BUS);
    if (!dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    init_nvs();

    while (1) {
        read_mma8x5x(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
