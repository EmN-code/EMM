
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/net_ip.h>
#include <time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/wifi.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/init.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/random/random.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_if.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/toolchain.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mutex.h>
#include <sys/time.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define NVS_PARTITION_ID 0
#define NVS_ADDRESS 0x0

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs nvs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .offset = NVS_ADDRESS,
};

int read_mma8x5x(const struct device *dev, uint8_t *data, uint32_t len) {
    return i2c_burst_read(dev, MMA8x5x_I2C_ADDRESS, 0x00, data, len);
}

int write_nvs(struct nvs_fs *fs, uint16_t id, const void *data, size_t len) {
    return nvs_write(fs, id, data, len);
}

int main(void) {
    uint8_t mma8x5x_data[6];
    int ret;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    ret = nvs_init(&nvs, "storage");
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(i2c_dev, mma8x5x_data, sizeof(mma8x5x_data));
        if (ret) {
            LOG_ERR("Failed to read MMA8x5x: %d", ret);
        } else {
            LOG_INF("MMA8x5x data: %02x %02x %02x %02x %02x %02x",
                    mma8x5x_data[0], mma8x5x_data[1], mma8x5x_data[2],
                    mma8x5x_data[3], mma8x5x_data[4], mma8x5x_data[5]);

            ret = write_nvs(&nvs, NVS_PARTITION_ID, mma8x5x_data, sizeof(mma8x5x_data));
            if (ret) {
                LOG_ERR("Failed to write NVS: %d", ret);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
