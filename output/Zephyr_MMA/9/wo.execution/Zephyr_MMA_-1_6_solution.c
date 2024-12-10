
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
#include <zephyr/net/net_mgmt.h>
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
#define NVS_PARTITION_LABEL DT_NODELABEL(storage_partition)

static struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION_LABEL),
    .sector_count = DT_REG_SIZE(NVS_PARTITION_LABEL) / DT_REG_SIZE(NVS_PARTITION_LABEL),
    .offset = DT_REG_ADDRESS(NVS_PARTITION_LABEL),
};

int main(void) {
    int ret;
    uint8_t data[6];
    struct device *i2c_dev = device_get_binding("I2C_0");

    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    ret = nvs_init(&nvs, DT_LABEL(NVS_PARTITION_LABEL));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    while (1) {
        ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, sizeof(data));
        if (ret) {
            printk("I2C read failed: %d\n", ret);
        } else {
            int16_t x = (data[0] << 8) | data[1];
            int16_t y = (data[2] << 8) | data[3];
            int16_t z = (data[4] << 8) | data[5];

            printk("X: %d, Y: %d, Z: %d\n", x, y, z);

            ret = nvs_write(&nvs, 0, &x, sizeof(x));
            if (ret < 0) {
                printk("NVS write failed: %d\n", ret);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
