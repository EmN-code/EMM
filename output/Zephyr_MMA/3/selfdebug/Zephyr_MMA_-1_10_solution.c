
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(NVS_PARTITION)

static struct nvs_fs fs;

void read_sensor_and_store(const struct device *dev)
{
    struct sensor_value accel[3];
    int rc;

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Failed to get sensor data");
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the read value in NVS
    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    rc = nvs_write(&fs, 1, data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS: %d", rc);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("MMA8X5X device not ready");
        return -ENODEV;
    }

    // Initialize NVS
    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device for NVS is not ready");
        return -ENODEV;
    }

    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = 4096; // Adjust according to your flash sector size
    fs.id_size = 2;

    int rc = nvs_init(&fs, "storage");
    if (rc) {
        LOG_ERR("NVS init failed: %d", rc);
        return rc;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
