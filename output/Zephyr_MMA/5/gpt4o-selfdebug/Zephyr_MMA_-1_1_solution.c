#include <zephyr/types.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>
#include <drivers/sensor/mma8x5x.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define I2C_DEV DT_LABEL(DT_INST(0, nxp_mma8x5x)) // Assume MMA8x5x is found on I2C address 0.
#define SLEEP_TIME_MS 5000
#define STORAGE_ID 1
#define STORAGE_NAME "storage"

static struct nvs_fs fs;

void configure_nvs(void) {
    int rc;
    struct flash_pages_info info;
    
    /* Configure the file system storage location */
    fs.sector_size = FLASH_SECTOR_SIZE;
    fs.sector_count = 3U;
    fs.offset = FLASH_AREA_OFFSET;

    rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("Flash Init failed: %d", rc);
    }
}

void store_reading(int32_t reading) {
    int rc;
    rc = nvs_write(&fs, STORAGE_ID, &reading, sizeof(reading));
    if (rc < 0) {
        LOG_ERR("Failed to write data to NVS: %d", rc);
    } else {
        LOG_INF("Reading %d stored in NVS", reading);
    }
}

void main(void) {
    const struct device *i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        LOG_ERR("Failed to get I2C device");
        return;
    }
    
    configure_nvs();

    while (1) {
        struct sensor_value accel;
        int ret = sensor_sample_fetch(i2c_dev);

        if (ret == 0) {
            ret = sensor_channel_get(i2c_dev, SENSOR_CHAN_ACCEL_XYZ, &accel);
            if (ret == 0) {
                int32_t reading = accel.val1;  // Assuming only x-axis is read
                LOG_INF("Accelerometer reading (x): %d", reading);
                store_reading(reading);
            } else {
                LOG_ERR("Failed to get sensor reading");
            }
        } else {
            LOG_ERR("Failed to fetch sensor sample");
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}