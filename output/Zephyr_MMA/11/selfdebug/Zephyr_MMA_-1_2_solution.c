
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
    int rc;

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

    uint32_t data = (uint32_t)(accel[0].val1); // Store the x-axis value as an example
    rc = nvs_write(fs, 0, &data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS (%d)", rc);
    } else {
        LOG_INF("Stored value: %d", data);
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
    fs.flash_device = NULL;
    fs.offset = FLASH_AREA_OFFSET(storage_partition);
    fs.sector_size = 4096; // Example sector size, adjust as needed
    fs.sector_count = 2;   // Example sector count, adjust as needed

    int rc = nvs_init(&fs, DT_LABEL(DT_INST(0, zephyr_nvs)));
    if (rc) {
        LOG_ERR("Failed to init NVS (%d)", rc);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
