
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEVICE_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 4

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void read_and_store_sensor_data(const struct device *dev)
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

    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

void main(void)
{
    const struct device *dev = device_get_binding(MMA8x5x_DEVICE_NAME);
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return;
    }

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    int ret = nvs_init(&fs, "storage");
    if (ret) {
        LOG_ERR("Failed to init NVS (%d)", ret);
        return;
    }

    while (1) {
        read_and_store_sensor_data(dev);
        k_sleep(K_SECONDS(5));
    }
}
