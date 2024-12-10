
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/wifi.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/slist.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/device.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/fdtable.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1

static const struct device *i2c_dev;
static struct nvs_fs fs;

int read_mma8x5x(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buf[6];
    int ret;

    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
    if (ret < 0) {
        printk("I2C read failed: %d\n", ret);
        return ret;
    }

    *x = (buf[0] << 8) | buf[1];
    *y = (buf[2] << 8) | buf[3];
    *z = (buf[4] << 8) | buf[5];

    return 0;
}

int store_nvs(int16_t x, int16_t y, int16_t z) {
    int ret;
    uint8_t data[6];

    data[0] = (x >> 8) & 0xFF;
    data[1] = x & 0xFF;
    data[2] = (y >> 8) & 0xFF;
    data[3] = y & 0xFF;
    data[4] = (z >> 8) & 0xFF;
    data[5] = z & 0xFF;

    ret = nvs_write(&fs, NVS_PARTITION_ID, data, sizeof(data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
        return ret;
    }

    return 0;
}

int main(void) {
    int16_t x, y, z;
    int ret;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    fs.flash_device = device_get_binding("FLASH_CTRL");
    if (!fs.flash_device) {
        printk("Flash device not found\n");
        return -1;
    }

    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = FIXED_PARTITION_SIZE(storage);
    fs.sector_count = 1;

    ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(&x, &y, &z);
        if (ret == 0) {
            printk("MMA8x5x: x=%d, y=%d, z=%d\n", x, y, z);
            store_nvs(x, y, z);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
