#include <zephyr/logging/log.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <storage/flash_area.h>
#include <string.h>

#define SENSOR_NODE DT_ALIAS(mma8x5x)
#define I2C_NODE DT_BUS(SENSOR_NODE)
#define STORAGE_PARTITION_FLASH_ID FIXED_PARTITION_ID(storage_partition)

LOG_MODULE_REGISTER(mma8x5x_nvs, LOG_LEVEL_DBG);

static int read_mma8x5x(const struct device *dev, int16_t *val)
{
    struct sensor_value accel[3];
    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        LOG_ERR("Failed to fetch sample: %d", ret);
        return ret;
    }
    
    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor channel: %d", ret);
        return ret;
    }
    *val = accel[0].val1;
    return 0;
}

void main(void)
{
    const struct device *dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return;
    }

    const struct flash_area *fa;
    int rc = flash_area_open(STORAGE_PARTITION_FLASH_ID, &fa);
    if (rc != 0) {
        LOG_ERR("Failed to open flash area");
        return;
    }

    int16_t accel_val;
    while (1) {
        rc = read_mma8x5x(dev, &accel_val);
        if (rc == 0) {
            LOG_INF("Acceleration: %d", accel_val);
            flash_area_erase(fa, 0, FLASH_AREA_SIZE);
            flash_area_write(fa, 0, &accel_val, sizeof(accel_val));
        } else {
            LOG_ERR("Failed to read acceleration");
        }
        k_sleep(K_SECONDS(5));
    }
    
    flash_area_close(fa);
}