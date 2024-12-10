
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDR 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION DT_N_INST_0_nvs_partition
#define NVS_PARTITION_ID DT_N_INST_0_nvs_partition_BUS_NAME

LOG_MODULE_REGISTER(main);

void read_mma8x5x(const struct device *i2c_dev, uint8_t *value) {
    if (i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDR, MMA8x5x_REG_OUT_X_MSB, value) != 0) {
        LOG_ERR("Failed to read from MMA8x5x sensor");
    }
}

void store_value_in_nvs(struct nvs_fs *fs, uint16_t id, uint8_t value) {
    if (nvs_write(fs, id, &value, sizeof(value)) != sizeof(value)) {
        LOG_ERR("Failed to write to NVS");
    }
}

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    struct nvs_fs fs;
    fs.flash_device = (void *)DEVICE_DT_GET(NVS_PARTITION_ID);
    fs.offset = DT_REG_ADDR(NVS_PARTITION);
    fs.sector_size = DT_PROP(NVS_PARTITION, sector_size);
    fs.sector_count = DT_PROP(NVS_PARTITION, nvs_sector_count);

    if (nvs_init(&fs, DT_LABEL(NVS_PARTITION)) != 0) {
        LOG_ERR("Failed to initialize NVS");
        return -1;
    }

    uint8_t value;
    while (1) {
        read_mma8x5x(i2c_dev, &value);
        store_value_in_nvs(&fs, 0, value);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
