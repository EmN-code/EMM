
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION FLASH_AREA_ID(storage)

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *dev, struct nvs_fs *fs) {
    struct sensor_value accel[3];
    int rc;

    if (!device_is_ready(dev)) {
        LOG_ERR("MMA8x5x device not ready");
        return;
    }

    rc = sensor_sample_fetch(dev);
    if (rc) {
        LOG_ERR("Failed to fetch sample (%d)", rc);
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        LOG_ERR("Failed to get sensor data (%d)", rc);
        return;
    }

    uint32_t data = (uint32_t)(accel[0].val1);
    rc = nvs_write(fs, 0, &data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS (%d)", rc);
    } else {
        LOG_INF("Stored value: %d", data);
    }
}

int main(void) {
    const struct device *mma8x5x_dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    struct nvs_fs fs;
    fs.sector_size = DT_REG_SIZE(NVS_PARTITION);
    fs.sector_count = 3;
    fs.offset = DT_REG_ADDR(NVS_PARTITION);

    int rc = nvs_init(&fs, DT_LABEL(NVS_PARTITION));
    if (rc) {
        LOG_ERR("Failed to init NVS (%d)", rc);
        return -1;
    }

    while (1) {
        read_sensor_and_store(mma8x5x_dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
