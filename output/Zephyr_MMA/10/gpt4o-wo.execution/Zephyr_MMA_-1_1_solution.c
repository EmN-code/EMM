
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_LABEL DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION DT_LABEL(DT_ALIAS(storage_partition))

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

    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Stored sensor data: %d, %d, %d", data[0], data[1], data[2]);
    }
}

int main(void) {
    const struct device *mma8x5x_dev = device_get_binding(MMA8x5x_LABEL);
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    struct flash_area *fa;
    int ret = flash_area_open(DT_FLASH_AREA_ID(storage_partition), &fa);
    if (ret) {
        LOG_ERR("Failed to open flash area (%d)", ret);
        return -1;
    }

    fs.flash_device = fa->fa_device;
    fs.offset = fa->fa_off;
    fs.sector_size = 4096; // Adjust based on your flash sector size
    fs.sector_count = 1;   // Adjust based on your flash size

    ret = nvs_init(&fs, NVS_PARTITION);
    if (ret) {
        LOG_ERR("Failed to init NVS (%d)", ret);
        return -1;
    }

    while (1) {
        read_sensor_and_store(mma8x5x_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
