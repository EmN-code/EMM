
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>

#define MMA8x5x_I2C_ADDR 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID
#define NVS_SECTOR_SIZE DT_FLASH_AREA_STORAGE_SIZE

struct i2c_msg {
    uint8_t *buf;
    uint32_t len;
    uint16_t flags;
};

#define I2C_MSG_WRITE 0
#define I2C_MSG_READ 1
#define I2C_MSG_RESTART 2

const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c_0));
struct nvs_fs fs;

void read_mma8x5x(uint8_t *data) {
    struct i2c_msg msgs[2];
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;

    msgs[0].buf = &reg_addr;
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE;

    msgs[1].buf = data;
    msgs[1].len = 1;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_RESTART;

    i2c_transfer(i2c_dev, msgs, 2, MMA8x5x_I2C_ADDR);
}

void store_value_in_nvs(uint8_t value) {
    int ret;
    uint8_t buffer[1];
    buffer[0] = value;

    ret = nvs_write(&fs, 0, buffer, sizeof(buffer));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("Value stored in NVS: %d\n", value);
    }
}

int main(void) {
    uint8_t data;
    int ret;

    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return 0;
    }

    fs.flash_device = DEVICE_DT_GET(DT_NODELABEL(storage));
    fs.offset = DT_REG_ADDR(DT_NODELABEL(storage));
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = 1;

    ret = nvs_init(&fs, NULL);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return 0;
    }

    while (1) {
        read_mma8x5x(&data);
        printk("MMA8x5x value: %d\n", data);
        store_value_in_nvs(data);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
