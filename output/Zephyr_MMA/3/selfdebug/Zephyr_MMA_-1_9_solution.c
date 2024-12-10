
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(NVS_PARTITION)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION)

static struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION),
    .sector_count = 1,
    .offset = 0,
};

void read_mma8x5x(const struct device *dev)
{
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data (%d)", ret);
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the read value in NVS
    uint8_t data[3];
    data[0] = accel[0].val1;
    data[1] = accel[1].val1;
    data[2] = accel[2].val1;

    ret = nvs_write(&nvs, 0, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void)
{
    const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8x5x_NODE);
    if (!device_is_ready(mma8x5x_dev)) {
        LOG_ERR("MMA8x5x device not ready");
        return -ENODEV;
    }

    int ret = nvs_init(&nvs, NVS_PARTITION_DEVICE);
    if (ret) {
        LOG_ERR("Failed to init NVS (%d)", ret);
        return ret;
    }

    while (1) {
        read_mma8x5x(mma8x5x_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
