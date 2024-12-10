
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/types.h>
#include <zephyr/net/wifi.h>
#include <zephyr/random/random.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sys/math_extras.h>
#include <time.h>
#include <sys/time.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

void read_mma8x5x(const struct device *i2c_dev, uint8_t *buf) {
    i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
}

void store_value_in_nvs(struct nvs_fs *fs, uint32_t addr, uint8_t *data, uint32_t len) {
    nvs_write(fs, addr, data, len);
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    struct nvs_fs fs;
    fs.flash_device = (struct device *)flash_device_get_binding("storage");
    if (!fs.flash_device) {
        printk("Flash device not found\n");
        return -1;
    }

    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = FIXED_PARTITION_SIZE(NVS_PARTITION_ID);
    fs.sector_count = 1;

    int ret = nvs_init(&fs, "storage");
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    uint8_t buf[6];
    while (1) {
        read_mma8x5x(i2c_dev, buf);
        store_value_in_nvs(&fs, 0, buf, sizeof(buf));
        printk("Stored value in NVS\n");
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
