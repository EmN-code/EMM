
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8X5X_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_FLASH_DEVICE FIXED_PARTITION_DEVICE(storage)
#define NVS_FLASH_OFFSET FIXED_PARTITION_OFFSET(storage)
#define NVS_FLASH_SIZE FIXED_PARTITION_SIZE(storage)

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = NVS_FLASH_DEVICE;
    fs.offset = NVS_FLASH_OFFSET;
    fs.sector_size = 4096;
    fs.sector_count = NVS_FLASH_SIZE / fs.sector_size;

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
    } else {
        LOG_INF("NVS mounted successfully");
    }
}

void read_and_store_sensor_data(const struct device *dev) {
    struct sensor_value accel[3];
    int ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample: %d", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data: %d", ret);
        return;
    }

    uint8_t data[12];
    memcpy(data, &accel[0].val1, 4);
    memcpy(data + 4, &accel[1].val1, 4);
    memcpy(data + 8, &accel[2].val1, 4);

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("Data stored successfully");
    }
}

int main(void) {
    const struct device *dev = device_get_binding(MMA8X5X_DEV_NAME);
    if (!dev) {
        LOG_ERR("Could not get MMA8X5X device");
        return -1;
    }

    init_nvs();

    while (1) {
        read_and_store_sensor_data(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
