
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_LABEL DT_LABEL(DT_ALIAS(storage))

struct nvs_fs nvs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .offset = 0,
};

int read_mma8x5x(const struct device *i2c_dev, uint8_t *buf, uint32_t num_bytes) {
    return i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, num_bytes);
}

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return -1;
    }

    int ret = nvs_init(&nvs, NVS_PARTITION_LABEL);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    uint8_t data[6];
    while (1) {
        ret = read_mma8x5x(i2c_dev, data, sizeof(data));
        if (ret < 0) {
            printk("Failed to read MMA8x5x: %d\n", ret);
        } else {
            printk("MMA8x5x data: %02x %02x %02x %02x %02x %02x\n",
                   data[0], data[1], data[2], data[3], data[4], data[5]);

            // Store the read value persistently in NVS
            ret = nvs_write(&nvs, 0, data, sizeof(data));
            if (ret < 0) {
                printk("NVS write failed: %d\n", ret);
            } else {
                printk("Data stored in NVS\n");
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
