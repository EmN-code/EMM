
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs fs;

int read_mma8x5x(int16_t *x)
{
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;
    uint8_t data[2];

    if (i2c_write_read(i2c_dev, MMA8x5x_I2C_ADDRESS, &reg_addr, 1, data, 2) != 0) {
        LOG_ERR("Failed to read from MMA8x5x");
        return -EIO;
    }

    *x = (data[0] << 8) | data[1];
    return 0;
}

int init_nvs(void)
{
    struct flash_sector sectors[1];

    sectors[0].fs_size = 4096;
    sectors[0].fs_off = 0;
    sectors[0].flash_dev = device_get_binding(DT_CHOSEN(zephyr_flash_controller));

    fs.sector_size = 4096;
    fs.sector_count = 1;
    fs.offset = DT_REG_ADDR(NVS_PARTITION);

    if (nvs_init(&fs, sectors, 1) != 0) {
        LOG_ERR("Failed to initialize NVS");
        return -EIO;
    }

    return 0;
}

int store_reading(int16_t x)
{
    uint8_t data[2];

    data[0] = (x >> 8) & 0xFF;
    data[1] = x & 0xFF;

    if (nvs_write(&fs, 0, data, 2) != 2) {
        LOG_ERR("Failed to write to NVS");
        return -EIO;
    }

    return 0;
}

int main(void)
{
    int16_t x;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -ENODEV;
    }

    if (init_nvs() != 0) {
        return -EIO;
    }

    while (1) {
        if (read_mma8x5x(&x) == 0) {
            LOG_INF("MMA8x5x X: %d", x);
            store_reading(x);
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
