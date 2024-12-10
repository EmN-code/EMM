
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_ALIAS(mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

static struct nvs_fs fs;

void store_reading(int16_t reading) {
    int rc;
    static uint8_t data[2];

    data[0] = (reading >> 8) & 0xFF;
    data[1] = reading & 0xFF;

    rc = nvs_write(&fs, 0, data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", rc);
    } else {
        LOG_INF("Stored reading: %d", reading);
    }
}

int16_t read_reading(void) {
    int rc;
    static uint8_t data[2];

    rc = nvs_read(&fs, 0, data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("Failed to read from NVS (err %d)", rc);
        return -1;
    } else {
        int16_t reading = (data[0] << 8) | data[1];
        LOG_INF("Read reading: %d", reading);
        return reading;
    }
}

void main(void) {
    const struct device *dev;
    struct sensor_value accel[3];
    int16_t reading;
    int rc;

    dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return;
    }

    struct flash_sector_param sector;
    rc = flash_get_page_info_by_id(NVS_PARTITION_ID, 0, &sector);
    if (rc) {
        LOG_ERR("Failed to get flash sector info (err %d)", rc);
        return;
    }

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    fs.offset = sector.start;
    fs.sector_size = sector.size;
    fs.sector_count = 1;

    rc = nvs_init(&fs, fs.flash_device);
    if (rc) {
        LOG_ERR("Failed to init NVS (err %d)", rc);
        return;
    }

    while (1) {
        if (sensor_sample_fetch(dev) < 0) {
            LOG_ERR("Failed to fetch sample");
        } else {
            if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
                LOG_ERR("Failed to get sensor data");
            } else {
                reading = accel[0].val1; // Assuming we are interested in the X-axis reading
                store_reading(reading);
            }
        }

        k_sleep(K_SECONDS(5));
    }
}
