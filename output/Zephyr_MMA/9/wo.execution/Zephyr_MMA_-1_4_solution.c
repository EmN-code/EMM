
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/storage/flash_map.h>
#include <time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/net/wifi.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/random/random.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mutex.h>
#include <sys/time.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/iterable_sections.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_LABEL "storage_partition"

int read_mma8x5x(uint8_t *buf) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    if (!device_is_ready(i2c_dev)) {
        printk("I2C device is not ready\n");
        return -1;
    }

    return i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
}

int store_value_in_nvs(uint8_t *buf) {
    const struct device *flash_dev = device_get_binding(NVS_PARTITION_LABEL);
    if (!flash_dev) {
        printk("Flash device not found\n");
        return -1;
    }

    struct nvs_fs fs;
    fs.flash_device = flash_dev;
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    if (nvs_mount(&fs) != 0) {
        printk("NVS mount failed\n");
        return -1;
    }

    int id = 1;
    if (nvs_write(&fs, id, buf, 6) != 6) {
        printk("NVS write failed\n");
        return -1;
    }

    return 0;
}

int main(void) {
    uint8_t buffer[6];

    while (1) {
        if (read_mma8x5x(buffer) == 0) {
            printk("MMA8x5x read successful\n");
            if (store_value_in_nvs(buffer) == 0) {
                printk("Value stored in NVS\n");
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
