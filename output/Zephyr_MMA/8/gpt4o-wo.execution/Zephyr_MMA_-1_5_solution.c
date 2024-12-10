
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

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

int store_reading(int16_t x)
{
    uint8_t data[2];
    data[0] = (x >> 8) & 0xFF;
    data[1] = x & 0xFF;

    if (nvs_write(&fs, 1, data, sizeof(data)) != sizeof(data)) {
        LOG_ERR("Failed to store reading in NVS");
        return -EIO;
    }

    return 0;
}

int main(void)
{
    int16_t x;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -ENODEV;
    }

    struct flash_sector_param sector;
    if (flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN(zephyr_flash)),
                                    FIXED_PARTITION_OFFSET(storage_partition),
                                    &sector) != 0) {
        LOG_ERR("Failed to get flash sector info");
        return -EIO;
    }

    fs.flash_device = device_get_binding(DT_CHOSEN(zephyr_flash));
    if (!fs.flash_device) {
        LOG_ERR("Flash device not found");
        return -ENODEV;
    }

    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);
    fs.sector_size = sector.fs_size;
    fs.sector_count = 1;

    if (nvs_init(&fs, fs.flash_device) != 0) {
        LOG_ERR("Failed to initialize NVS");
        return -EIO;
    }

    while (1) {
        if (read_mma8x5x(&x) == 0) {
            LOG_INF("MMA8x5x reading: %d", x);
            if (store_reading(x) != 0) {
                LOG_ERR("Failed to store reading");
            }
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
