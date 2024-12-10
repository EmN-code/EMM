
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    struct flash_sector_param sector[1];

    sector[0].fs_size = 4096;
    sector[0].offset = DT_REG_ADDR(NVS_PARTITION);

    fs.sector_size = 4096;
    fs.sector_count = 1;
    fs.offset = DT_REG_ADDR(NVS_PARTITION);

    int ret = nvs_init(&fs, DT_LABEL(NVS_PARTITION));
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
    }
}

void read_sensor_and_store(const struct device *dev) {
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

    uint32_t data = (uint32_t)(accel[0].val1 << 16 | accel[1].val1 << 8 | accel[2].val1);
    ret = nvs_write(&fs, 1, &data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("Stored value: %u", data);
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("MMA8X5X device not ready");
        return -1;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
