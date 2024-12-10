
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(storage)

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int ret;

    fs.flash_device = NVS_PARTITION_DEVICE;
    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
    }
}

void store_sensor_data(int16_t data) {
    int ret;
    ret = nvs_write(&fs, 0, &data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("Data stored: %d", data);
    }
}

void read_sensor_data(void) {
    const struct device *dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!dev) {
        LOG_ERR("Failed to get device binding");
        return;
    }

    struct sensor_value accel[3];
    int ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Sensor sample fetch failed: %d", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Sensor channel get failed: %d", ret);
        return;
    }

    int16_t data = (int16_t)(accel[0].val1);
    store_sensor_data(data);
}

void main(void) {
    init_nvs();

    while (1) {
        read_sensor_data();
        k_sleep(K_SECONDS(5));
    }
}
