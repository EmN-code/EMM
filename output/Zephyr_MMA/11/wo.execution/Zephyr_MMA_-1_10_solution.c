
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION DT_LABEL(DT_INST(0, zephyr_nvs))
#define NVS_SECTOR_SIZE DT_PROP(DT_INST(0, zephyr_nvs), sector_size)
#define NVS_SECTOR_COUNT DT_PROP(DT_INST(0, zephyr_nvs), nvs_sector_count)

LOG_MODULE_REGISTER(main);

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    struct nvs_fs fs;
    fs.flash_device = NVS_PARTITION;
    fs.offset = DT_REG_ADDR(DT_INST(0, zephyr_nvs));
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    int ret = nvs_init(&fs, NVS_PARTITION);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    uint8_t accel_value;
    uint16_t nvs_id = 0;

    while (1) {
        ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, &accel_value);
        if (ret) {
            LOG_ERR("I2C read failed: %d", ret);
        } else {
            LOG_INF("Accelerometer value: %d", accel_value);

            ret = nvs_write(&fs, nvs_id, &accel_value, sizeof(accel_value));
            if (ret < 0) {
                LOG_ERR("NVS write failed: %d", ret);
            } else {
                LOG_INF("NVS write successful");
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
