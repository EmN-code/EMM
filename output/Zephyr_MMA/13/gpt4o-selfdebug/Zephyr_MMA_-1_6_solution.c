#include <zephyr/sys/util.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/nvs/nvs.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>

#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c1))
#define MMA8X5X_ADDRESS 0x1D
#define DELAY_TIME K_SECONDS(5)

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

struct nvs_fs fs;

void mma8x5x_init(const struct device *i2c_dev) {
    uint8_t config_data = 0x00;
    int ret = i2c_reg_write_byte(i2c_dev, MMA8X5X_ADDRESS, 0x2A, config_data);
    if (ret) {
        printk("Failed to initialize MMA8x5x sensor\n");
    }
}

int mma8x5x_read(const struct device *i2c_dev) {
    uint8_t sensor_data = 0;
    int ret = i2c_reg_read_byte(i2c_dev, MMA8X5X_ADDRESS, 0x01, &sensor_data);
    if (ret) {
        printk("Failed to read MMA8x5x sensor\n");
    }
    return sensor_data;
}

void store_reading_to_nvs(int reading) {
    struct nvs_fs fs;
    int ret = nvs_init(&fs, DT_CHOSEN(zephyr_flash));
    if (ret) {
        LOG_ERR("Cannot init NVS, error: %d", ret);
        return;
    }
    ret = nvs_write(&fs, 1, &reading, sizeof(reading));
    if (ret < 0) {
        LOG_ERR("Failed to write reading to NVS, error: %d", ret);
    }
}

void main(void) {
    const struct device *i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("I2C: Device driver not found.\n");
        return;
    }

    mma8x5x_init(i2c_dev);

    while (1) {
        int reading = mma8x5x_read(i2c_dev);
        printk("MMA8x5x reading: %d\n", reading);
        store_reading_to_nvs(reading);
        k_sleep(DELAY_TIME);
    }
}