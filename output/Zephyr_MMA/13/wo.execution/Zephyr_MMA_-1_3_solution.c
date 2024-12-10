
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/util.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/syscalls/device.h>
#include <sys/time.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/net_ip.h>
#include <time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/device.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/buf.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/net/websocket.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/slist.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/drivers/flash.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1

struct nvs_fs nvs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .offset = DT_FLASH_AREA_OFFSET(storage),
};

int read_mma8x5x(uint8_t *buf) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        return -1;
    }
    return i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
}

int main(void) {
    uint8_t data[6];
    int ret;

    ret = nvs_mount(&nvs);
    if (ret) {
        return ret;
    }

    while (1) {
        ret = read_mma8x5x(data);
        if (ret == 0) {
            nvs_write(&nvs, NVS_PARTITION_ID, data, sizeof(data));
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
