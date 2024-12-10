
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_N_INST_0_nxp_mma8x5x
#define NVS_PARTITION DT_NODELABEL(storage)

static struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION),
    .sector_count = DT_PROP(NVS_PARTITION, nvs_num_sectors),
    .offset = DT_REG_ADDR(NVS_PARTITION),
};

void read_mma8x5x(struct sensor_value *accel)
{
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("MMA8X5X device not ready");
        return;
    }

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
}

void store_nvs(struct sensor_value *accel)
{
    int ret;
    uint8_t data[sizeof(struct sensor_value) * 3];
    memcpy(data, accel, sizeof(struct sensor_value) * 3);

    ret = nvs_write(&nvs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("NVS write success");
    }
}

int main(void)
{
    struct sensor_value accel[3];
    int ret;

    ret = nvs_init(&nvs, DT_REG_ADDR(NVS_PARTITION));
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return ret;
    }

    while (1) {
        read_mma8x5x(accel);
        store_nvs(accel);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
