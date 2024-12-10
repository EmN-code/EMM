
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 0

struct nvs_fs storage;

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buf[6];
    i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
    *x = (buf[0] << 8) | buf[1];
    *y = (buf[2] << 8) | buf[3];
    *z = (buf[4] << 8) | buf[5];
}

void store_value_in_nvs(int16_t value) {
    nvs_write(&storage, 1, &value, sizeof(value));
}

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return 0;
    }

    int16_t x, y, z;

    // Initialize NVS
    storage.flash_device = FLASH_AREA_DEVICE(nvs);
    storage.offset = 0;
    storage.sector_size = 4096;
    storage.sector_count = 2;

    int ret = nvs_mount(&storage);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return 0;
    }

    while (1) {
        read_mma8x5x(i2c_dev, &x, &y, &z);
        printk("MMA8x5x: X=%d, Y=%d, Z=%d\n", x, y, z);
        store_value_in_nvs(x);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
