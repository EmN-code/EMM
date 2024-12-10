
#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/wifi.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/net/websocket.h>
#include <zephyr/random/random.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/coap.h>
#include <zephyr/sys/printk.h>
#include <time.h>
#include <sys/time.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION_ID 0
#define NVS_STORAGE_ID 0

struct nvs_fs nvs = {
    .sector_size = CONFIG_NVS_SECTOR_SIZE,
    .sector_count = CONFIG_NVS_SECTOR_COUNT,
    .offset = NVS_PARTITION_ID,
};

int read_mma8x5x(uint8_t *value) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    if (i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, value) != 0) {
        printk("Failed to read from MMA8x5x\n");
        return -1;
    }

    return 0;
}

int main(void) {
    uint8_t value;
    int ret;

    ret = nvs_init(&nvs, NVS_PARTITION_ID);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(&value);
        if (ret == 0) {
            printk("MMA8x5x value: %d\n", value);
            ret = nvs_write(&nvs, NVS_STORAGE_ID, &value, sizeof(value));
            if (ret < 0) {
                printk("NVS write failed: %d\n", ret);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
