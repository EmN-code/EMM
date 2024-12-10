
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/drivers/flash.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1
#define NVS_PARTITION_LABEL FIXED_PARTITION_ID(storage)

static struct nvs_fs fs;

void init_nvs(void) {
    struct flash_sector *sectors;
    int sector_count;

    sectors = (struct flash_sector *)flash_area_get_sectors(NVS_PARTITION_ID, &sector_count);
    if (sectors == NULL) {
        printk("Failed to get flash sectors\n");
        return;
    }

    fs.flash_device = NVS_PARTITION_LABEL;
    fs.offset = sectors[0].fs_offset;
    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = sector_count;

    int ret = nvs_init(&fs, NVS_PARTITION_LABEL);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
    } else {
        printk("NVS init successful\n");
    }
}

int read_mma8x5x(const struct device *i2c_dev, uint8_t *value) {
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, value);
}

void store_value_in_nvs(uint8_t value) {
    int ret = nvs_write(&fs, 1, &value, sizeof(value));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful: %d\n", ret);
    }
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    init_nvs();

    while (1) {
        uint8_t value;
        int ret = read_mma8x5x(i2c_dev, &value);
        if (ret) {
            printk("Failed to read MMA8x5x: %d\n", ret);
        } else {
            printk("MMA8x5x read value: %d\n", value);
            store_value_in_nvs(value);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
