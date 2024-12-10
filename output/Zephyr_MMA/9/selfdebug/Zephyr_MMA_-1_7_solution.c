
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <storage/nvs.h>
#include <sys/printk.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_STORAGE_NAME "mma8x5x_data"

void read_mma8x5x(const struct device *i2c_dev, uint8_t *data) {
    struct i2c_msg msgs[2];

    msgs[0].buf = &MMA8x5x_REG_OUT_X_MSB;
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_RESTART;

    msgs[1].buf = data;
    msgs[1].len = 6;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

    i2c_transfer(i2c_dev, msgs, 2, MMA8x5x_I2C_ADDRESS);
}

void store_value_nvs(struct nvs_fs *fs, uint8_t *data) {
    int rc;
    uint32_t id = 1;

    rc = nvs_write(fs, id, data, 6);
    if (rc < 0) {
        printk("NVS write failed: %d\n", rc);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void) {
    const struct device *i2c_dev;
    struct nvs_fs fs;
    uint8_t data[6];

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    fs.flash_device = FLASH_AREA_ID(storage_partition);
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.id_size = 1;

    int rc = nvs_mount(&fs);
    if (rc) {
        printk("NVS mount failed: %d\n", rc);
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev, data);
        store_value_nvs(&fs, data);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
