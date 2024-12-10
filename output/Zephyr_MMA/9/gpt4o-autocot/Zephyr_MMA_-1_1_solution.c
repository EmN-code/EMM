
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID

static struct device *mma8x5x_dev;
static struct nvs_fs nvs = {
    .sector_size = DT_FLASH_AREA_STORAGE_SIZE,
    .sector_count = DT_FLASH_AREA_STORAGE_NUM,
    .offset = DT_FLASH_AREA_STORAGE_OFFSET,
};

void read_and_store_sensor_data(void)
{
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(mma8x5x_dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data (%d)", ret);
        return;
    }

    uint32_t data = (accel[0].val1 << 16) | (accel[1].val1 << 8) | accel[2].val1;

    ret = nvs_write(&nvs, 0, &data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Stored sensor data: %u", data);
    }
}

int main(void)
{
    int ret;

    mma8x5x_dev = device_get_binding(DT_LABEL(MMA8x5x_NODE));
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -ENODEV;
    }

    ret = nvs_init(&nvs, DT_LABEL(NVS_PARTITION));
    if (ret) {
        LOG_ERR("NVS init failed (%d)", ret);
        return -EIO;
    }

    while (1) {
        read_and_store_sensor_data();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
