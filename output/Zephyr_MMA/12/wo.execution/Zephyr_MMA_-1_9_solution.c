
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDR 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs fs;

void read_mma8x5x(void)
{
    uint8_t accel_data[6];
    struct i2c_msg msgs[2];

    msgs[0].buf = &MMA8x5x_REG_OUT_X_MSB;
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE;

    msgs[1].buf = accel_data;
    msgs[1].len = 6;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

    if (i2c_transfer(i2c_dev, msgs, 2, MMA8x5x_I2C_ADDR) != 0) {
        LOG_ERR("I2C transfer failed");
        return;
    }

    int16_t x = (accel_data[0] << 8) | accel_data[1];
    int16_t y = (accel_data[2] << 8) | accel_data[3];
    int16_t z = (accel_data[4] << 8) | accel_data[5];

    LOG_INF("X: %d, Y: %d, Z: %d", x, y, z);

    // Store the read value persistently in NVS
    if (nvs_write(&fs, 0, &x, sizeof(x)) != 0) {
        LOG_ERR("NVS write failed");
    }
}

int main(void)
{
    i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c_0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    fs.flash_device = DEVICE_DT_GET(DT_NODELABEL(storage));
    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device not ready");
        return -1;
    }

    fs.offset = DT_REG_ADDR(DT_NODELABEL(storage));
    fs.sector_size = DT_REG_SIZE(DT_NODELABEL(storage));
    fs.sector_count = 1;

    if (nvs_init(&fs, NULL) != 0) {
        LOG_ERR("NVS init failed");
        return -1;
    }

    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
