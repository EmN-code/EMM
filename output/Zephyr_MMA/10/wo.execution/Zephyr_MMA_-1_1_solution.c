
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDR 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_PARTITION_SIZE 0x1000

static struct nvs_fs fs;

void read_mma8x5x(const struct device *i2c_dev, uint8_t *data) {
    int ret;
    ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDR, MMA8x5x_REG_OUT_X_MSB, data);
    if (ret < 0) {
        LOG_ERR("Failed to read from MMA8x5x: %d", ret);
    } else {
        LOG_INF("MMA8x5x read value: %x", *data);
    }
}

void store_value_in_nvs(uint8_t value) {
    int ret;
    struct nvs_fs *nvs = &fs;
    ret = nvs_write(nvs, 1, &value, sizeof(value));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    } else {
        LOG_INF("Value stored in NVS: %x", value);
    }
}

int main(void) {
    int ret;
    uint8_t mma8x5x_data;
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));

    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    struct flash_area *fa;
    ret = flash_area_open(NVS_PARTITION, &fa);
    if (ret < 0) {
        LOG_ERR("Failed to open flash area: %d", ret);
        return -1;
    }

    fs.flash_device = (struct device *)fa->fa_device;
    fs.offset = fa->fa_off;
    fs.sector_size = NVS_PARTITION_SIZE;
    fs.sector_count = 1;

    ret = nvs_init(&fs, fs.flash_device);
    if (ret < 0) {
        LOG_ERR("Failed to initialize NVS: %d", ret);
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev, &mma8x5x_data);
        store_value_in_nvs(mma8x5x_data);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
