
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_NAMESPACE "mma8x5x_data"

LOG_MODULE_REGISTER(main);

void read_mma8x5x(const struct device *dev) {
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

    LOG_INF("X: %d.%06d m/s^2, Y: %d.%06d m/s^2, Z: %d.%06d m/s^2",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the read value persistently in NVS
    struct nvs_fs fs;
    fs.sector_size = DT_REG_SIZE(NVS_PARTITION);
    fs.sector_count = 1;
    fs.offset = DT_REG_ADDR(NVS_PARTITION);

    ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS (%d)", ret);
        return;
    }

    ret = nvs_write(&fs, NVS_NAMESPACE, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS successfully");
    }

    nvs_unmount(&fs);
}

void main(void) {
    const struct device *dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return;
    }

    while (1) {
        read_mma8x5x(dev);
        k_sleep(K_SECONDS(5));
    }
}
