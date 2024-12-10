
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/time.h>

#define MMA8X5X_I2C_ADDRESS DT_REG_ADDR(DT_NODELABEL(mma8x5x))
#define MMA8X5X_I2C_BUS DT_BUS_LABEL(DT_NODELABEL(mma8x5x))
#define NVS_FLASH_DEVICE DT_LABEL(DT_NODELABEL(storage))
#define NVS_SECTOR_COUNT DT_PROP(DT_NODELABEL(storage), nvs_num_sectors)
#define NVS_SECTOR_SIZE DT_PROP(DT_NODELABEL(storage), nvs_sector_size)

LOG_MODULE_REGISTER(main);

static struct nvs_fs nvs = {
    .flash_device = NVS_FLASH_DEVICE,
    .sector_count = NVS_SECTOR_COUNT,
    .sector_size = NVS_SECTOR_SIZE,
    .offset = 0,
};

int main(void) {
    const struct device *i2c_dev = device_get_binding(MMA8X5X_I2C_BUS);
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    int ret = nvs_init(&nvs, NVS_FLASH_DEVICE);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    uint8_t accel_value;
    while (1) {
        ret = i2c_reg_read_byte(i2c_dev, MMA8X5X_I2C_ADDRESS, 0x00, &accel_value);
        if (ret) {
            LOG_ERR("I2C read failed: %d", ret);
        } else {
            LOG_INF("Accelerometer value: %d", accel_value);
            ret = nvs_write(&nvs, 0, &accel_value, sizeof(accel_value));
            if (ret < 0) {
                LOG_ERR("NVS write failed: %d", ret);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
