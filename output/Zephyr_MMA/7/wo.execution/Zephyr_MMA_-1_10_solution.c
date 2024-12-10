
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID

LOG_MODULE_REGISTER(main);

struct nvs_fs fs = {
    .sector_size = DT_FLASH_AREA_STORAGE_SIZE,
    .sector_count = DT_FLASH_AREA_STORAGE_NUM,
    .offset = DT_FLASH_AREA_STORAGE_OFFSET,
};

int read_mma8x5x(const struct device *i2c_dev, uint8_t *data)
{
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data);
}

int main(void)
{
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    if (nvs_init(&fs, DT_FLASH_AREA_STORAGE_LABEL) != 0) {
        LOG_ERR("NVS init failed");
        return -1;
    }

    while (1) {
        uint8_t data;
        if (read_mma8x5x(i2c_dev, &data) == 0) {
            LOG_INF("MMA8x5x read: %d", data);
            nvs_write(&fs, 0, &data, sizeof(data));
        } else {
            LOG_ERR("Failed to read from MMA8x5x");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
