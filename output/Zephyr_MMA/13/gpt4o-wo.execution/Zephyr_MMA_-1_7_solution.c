
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define SENSOR_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(nvs_storage)

static struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION),
    .sector_count = DT_PROP(NVS_PARTITION, nvs_num_sectors),
};

void read_sensor_and_store(const struct device *dev)
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

    uint8_t data[6];
    data[0] = (uint8_t)(accel[0].val1 >> 8);
    data[1] = (uint8_t)(accel[0].val1 & 0xFF);
    data[2] = (uint8_t)(accel[1].val1 >> 8);
    data[3] = (uint8_t)(accel[1].val1 & 0xFF);
    data[4] = (uint8_t)(accel[2].val1 >> 8);
    data[5] = (uint8_t)(accel[2].val1 & 0xFF);

    ret = nvs_write(&nvs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void)
{
    const struct device *dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -ENODEV;
    }

    int ret = nvs_init(&nvs, DT_REG_ADDR(NVS_PARTITION));
    if (ret) {
        LOG_ERR("Failed to init NVS (%d)", ret);
        return ret;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
