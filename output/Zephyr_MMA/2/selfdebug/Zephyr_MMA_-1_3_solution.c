
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *dev, struct nvs_fs *fs)
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

    uint32_t value = (uint32_t)(accel[0].val1); // Store the x-axis value as an example

    ret = nvs_write(fs, 0, &value, sizeof(value));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Stored value: %d", value);
    }
}

int main(void)
{
    const struct device *dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    struct nvs_fs fs;
    fs.sector_size = DT_SIZE_GET(NVS_PARTITION_ID);
    fs.sector_count = 1;
    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS (%d)", ret);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
