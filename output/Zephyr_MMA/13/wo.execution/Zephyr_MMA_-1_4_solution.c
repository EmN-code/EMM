
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/time.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/buf.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/stats/stats.h>
#include <zephyr/linker/sections.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/random/random.h>
#include <time.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_LABEL DT_LABEL(DT_ALIAS(storage))
#define NVS_SECTOR_SIZE 4096
#define NVS_NUM_SECTORS 4

static struct nvs_fs nvs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_NUM_SECTORS,
    .flash_device = NVS_PARTITION_LABEL,
};

static const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));

int read_mma8x5x(uint8_t *buf) {
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return -ENODEV;
    }

    return i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
}

int main(void) {
    uint8_t accel_data[6];
    int ret;

    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return -ENODEV;
    }

    ret = nvs_mount(&nvs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return ret;
    }

    while (1) {
        ret = read_mma8x5x(accel_data);
        if (ret) {
            printk("Failed to read MMA8x5x: %d\n", ret);
        } else {
            printk("MMA8x5x data: %02x %02x %02x %02x %02x %02x\n",
                   accel_data[0], accel_data[1], accel_data[2],
                   accel_data[3], accel_data[4], accel_data[5]);

            ret = nvs_write(&nvs, 0, accel_data, sizeof(accel_data));
            if (ret < 0) {
                printk("NVS write failed: %d\n", ret);
            } else {
                printk("NVS write successful\n");
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
