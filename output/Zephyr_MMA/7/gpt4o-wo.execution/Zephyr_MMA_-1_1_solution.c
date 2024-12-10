
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
    int rc;

    // Read sensor data
    rc = sensor_sample_fetch(dev);
    if (rc) {
        LOG_ERR("Failed to fetch sample (%d)", rc);
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        LOG_ERR("Failed to get channel (%d)", rc);
        return;
    }

    // Store the reading value persistently in NVS
    uint8_t data[12];
    memcpy(data, &accel[0].val1, sizeof(data));

    rc = nvs_write(&fs, 1, data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS (%d)", rc);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void) {
    const struct device *dev;
    int rc;

    // Initialize NVS
    fs.flash_device = FLASH_AREA_ID(NVS_PARTITION);
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    rc = nvs_init(&fs, "storage");
    if (rc) {
        LOG_ERR("Failed to init NVS (%d)", rc);
        return -1;
    }

    // Get the MMA8x5x device
    dev = DEVICE_DT_GET(MMA8x5x_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("MMA8x5x device not ready");
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
