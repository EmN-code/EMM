
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>
#include <time.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_LABEL DT_LABEL(DT_NODELABEL(nvs))

#define SLEEP_TIME_MS 5000

const struct device *i2c_dev;
struct nvs_fs nvs;

void read_mma8x5x(uint8_t *buf) {
    int ret;
    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
    if (ret < 0) {
        printk("I2C read failed: %d\n", ret);
    }
}

void store_to_nvs(uint8_t *buf) {
    int ret;
    ret = nvs_write(&nvs, 0, buf, 6);
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    }
}

int main(void) {
    uint8_t data[6];
    int ret;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    nvs.flash_device = NVS_PARTITION_LABEL;
    nvs.offset = 0;
    nvs.sector_size = 4096;
    nvs.sector_count = 2;

    ret = nvs_mount(&nvs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return -1;
    }

    while (1) {
        read_mma8x5x(data);
        store_to_nvs(data);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
