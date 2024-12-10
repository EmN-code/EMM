
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/random/random.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/drivers/flash.h>
#include <sys/time.h>
#include <time.h>
#include <zephyr/sys/device_mmio.h>

#define MMA8x5x_I2C_ADDR 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1

const struct device *i2c_dev;
struct nvs_fs nvs;

void init_nvs(void) {
    nvs.flash_device = FLASH_AREA_DEVICE(nvs);
    nvs.offset = FIXED_PARTITION_NODE_OFFSET(FIXED_PARTITION_DEVICE(nvs));
    nvs.sector_size = FIXED_PARTITION_NODE_SIZE(FIXED_PARTITION_DEVICE(nvs));
    nvs.sector_count = 3;

    int err = nvs_mount(&nvs);
    if (err) {
        printk("NVS mount failed: %d\n", err);
    } else {
        printk("NVS mounted successfully\n");
    }
}

void read_mma8x5x(void) {
    uint8_t buf[6];
    int16_t x, y, z;

    int ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDR, MMA8x5x_REG_OUT_X_MSB, buf, sizeof(buf));
    if (ret < 0) {
        printk("I2C read failed: %d\n", ret);
        return;
    }

    x = (buf[0] << 8) | buf[1];
    y = (buf[2] << 8) | buf[3];
    z = (buf[4] << 8) | buf[5];

    printk("X: %d, Y: %d, Z: %d\n", x, y, z);

    // Store the read value persistently in NVS
    int32_t value = x; // Store X value as an example
    int err = nvs_write(&nvs, NVS_PARTITION_ID, &value, sizeof(value));
    if (err < 0) {
        printk("NVS write failed: %d\n", err);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void) {
    i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return -1;
    }

    init_nvs();

    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
