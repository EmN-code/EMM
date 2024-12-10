
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 4

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_SECTOR_COUNT,
    .offset = NVS_PARTITION,
};

void read_mma8x5x(const struct device *i2c_dev, uint8_t *data)
{
    uint8_t addr = MMA8x5x_REG_OUT_X_MSB;
    i2c_write_read(i2c_dev, MMA8x5x_I2C_ADDRESS, &addr, 1, data, 6);
}

void store_in_nvs(struct nvs_fs *fs, uint8_t *data)
{
    int rc;
    uint16_t id = 1;

    rc = nvs_write(fs, id, data, 6);
    if (rc < 0) {
        LOG_ERR("NVS write failed: %d", rc);
    } else {
        LOG_INF("NVS write successful");
    }
}

void main(void)
{
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return;
    }

    int rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("NVS mount failed: %d", rc);
        return;
    }

    uint8_t data[6];

    while (1) {
        read_mma8x5x(i2c_dev, data);
        store_in_nvs(&fs, data);
        k_sleep(K_SECONDS(5));
    }
}
