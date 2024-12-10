
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_SECTOR_SIZE 4096

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs fs;

int read_mma8x5x(uint8_t *data)
{
    struct i2c_msg msgs[2];
    uint8_t addr = MMA8x5x_I2C_ADDRESS;
    uint8_t reg = MMA8x5x_REG_OUT_X_MSB;

    msgs[0].buf = &reg;
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE;

    msgs[1].buf = data;
    msgs[1].len = 6;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

    return i2c_transfer(i2c_dev, msgs, 2, addr);
}

int init_nvs(void)
{
    int rc;
    struct flash_sector sector[1];

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device %s is not ready", fs.flash_device->name);
        return -ENODEV;
    }

    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = 1;

    rc = flash_get_page_info_by_offs(fs.flash_device, 0, &sector[0]);
    if (rc) {
        LOG_ERR("Unable to get flash sector info (err %d)", rc);
        return rc;
    }

    fs.offset = sector[0].fs_offset;

    rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("NVS mount failed (err %d)", rc);
        return rc;
    }

    return 0;
}

int store_data_to_nvs(uint8_t *data)
{
    int rc;
    size_t len = 6;

    rc = nvs_write(&fs, 0, data, len);
    if (rc < 0) {
        LOG_ERR("NVS write failed (err %d)", rc);
        return rc;
    }

    return 0;
}

int main(void)
{
    uint8_t data[6];
    int rc;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -ENODEV;
    }

    rc = init_nvs();
    if (rc) {
        LOG_ERR("NVS initialization failed (err %d)", rc);
        return rc;
    }

    while (1) {
        rc = read_mma8x5x(data);
        if (rc) {
            LOG_ERR("Failed to read MMA8x5x (err %d)", rc);
        } else {
            LOG_INF("MMA8x5x data: %02x %02x %02x %02x %02x %02x",
                    data[0], data[1], data[2], data[3], data[4], data[5]);

            rc = store_data_to_nvs(data);
            if (rc) {
                LOG_ERR("Failed to store data to NVS (err %d)", rc);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
