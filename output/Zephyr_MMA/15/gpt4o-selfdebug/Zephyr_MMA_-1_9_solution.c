#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/settings/settings.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_I2C_ADDR 0x1D
#define SENSOR_REG 0x00

static const struct device *i2c_dev;
static const struct device *nvs_dev;

void read_and_store_sensor_data(void)
{
    uint8_t sensor_data;
    int ret;

    ret = i2c_reg_read_byte(i2c_dev, MMA8X5X_I2C_ADDR, SENSOR_REG, &sensor_data);
    if (ret != 0) {
        LOG_ERR("Failed to read sensor data");
        return;
    }

    /* Write sensor_data to NVS */
    ret = flash_area_write(nvs_dev, 0, &sensor_data, sizeof(sensor_data));
    if (ret != 0) {
        LOG_ERR("Failed to write sensor data to NVS");
    } else {
        LOG_INF("Sensor data stored to NVS successfully");
    }
}

void main(void)
{
    i2c_dev = device_get_binding(DT_LABEL(DT_NODELABEL(i2c0)));
    if (!i2c_dev) {
        LOG_ERR("I2C: Device not found");
        return;
    }

    nvs_dev = device_get_binding(DT_LABEL(DT_NODELABEL(storage_partition)));
    if (!nvs_dev) {
        LOG_ERR("NVS: Device not found");
        return;
    }

    while (1) {
        read_and_store_sensor_data();
        k_sleep(K_SECONDS(5));
    }
}
