
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_ALIAS(mma8x5x)
#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID

static struct nvs_fs fs;

void read_sensor_and_store(const struct device *dev) {
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

    uint8_t data[12];
    memcpy(data, &accel[0].val1, 4);
    memcpy(data + 4, &accel[1].val1, 4);
    memcpy(data + 8, &accel[2].val1, 4);

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("MMA8x5x device not ready");
        return -1;
    }

    struct flash_area *fa;
    int ret = flash_area_open(NVS_PARTITION, &fa);
    if (ret) {
        LOG_ERR("Failed to open flash area (%d)", ret);
        return -1;
    }

    fs.flash_device = (struct device *)fa->fa_device;
    fs.offset = fa->fa_off;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
