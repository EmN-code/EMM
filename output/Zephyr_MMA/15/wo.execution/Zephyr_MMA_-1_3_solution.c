
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <time.h>

#define MMA8x5x_I2C_ADDR 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID

static struct nvs_fs storage = {
    .sector_size = DT_FLASH_AREA_STORAGE_SECTOR_SIZE,
    .sector_count = DT_FLASH_AREA_STORAGE_SECTOR_COUNT,
    .offset = DT_FLASH_AREA_STORAGE_OFFSET,
};

int read_mma8x5x(const struct device *i2c_dev, uint8_t *buf) {
    return i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDR, MMA8x5x_REG_OUT_X_MSB, buf, 6);
}

int store_value_in_nvs(uint16_t id, const void *data, size_t len) {
    return nvs_write(&storage, id, data, len);
}

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    uint8_t data[6];
    int ret;

    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return -1;
    }

    ret = flash_area_open(NVS_PARTITION, &storage.flash_device);
    if (ret) {
        printk("Failed to open flash area\n");
        return -1;
    }

    ret = nvs_mount(&storage);
    if (ret) {
        printk("Failed to mount NVS\n");
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(i2c_dev, data);
        if (ret < 0) {
            printk("Failed to read MMA8x5x\n");
        } else {
            printk("MMA8x5x data: %02x %02x %02x %02x %02x %02x\n",
                   data[0], data[1], data[2], data[3], data[4], data[5]);
            store_value_in_nvs(1, data, sizeof(data));
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
