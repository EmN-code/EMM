
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_SECTOR_COUNT 4

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void read_mma8x5x(const struct device *i2c_dev, uint8_t *data)
{
    i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data);
}

void init_nvs(void)
{
    int rc;
    struct flash_sector sectors[NVS_SECTOR_COUNT];

    rc = flash_area_get_sectors(NVS_PARTITION, &NVS_SECTOR_COUNT, sectors);
    if (rc) {
        LOG_ERR("Failed to get flash sectors: %d", rc);
        return;
    }

    fs.flash_device = (struct device *)device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (!fs.flash_device) {
        LOG_ERR("Flash device not found");
        return;
    }

    fs.offset = sectors[0].fs_off; // Corrected from fs_offset to fs_off
    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = NVS_SECTOR_COUNT;

    rc = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (rc) {
        LOG_ERR("NVS init failed: %d", rc);
    }
}

void store_data_to_nvs(uint8_t data)
{
    int rc;
    static uint16_t id = 0;

    rc = nvs_write(&fs, id++, &data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("NVS write failed: %d", rc);
    }
}

void main(void)
{
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return;
    }

    init_nvs();

    uint8_t data;

    while (1) {
        read_mma8x5x(i2c_dev, &data);
        store_data_to_nvs(data);
        k_sleep(K_SECONDS(5));
    }
}
