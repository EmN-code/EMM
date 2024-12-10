
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/net_ip.h>
#include <time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/wifi.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/random/random.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_if.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/mutex.h>
#include <sys/time.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/toolchain.h>
#include <zephyr/devicetree.h>

#define MMA8X5X_I2C_ADDRESS DT_REG_ADDR(DT_NODELABEL(mma8x5x))
#define MMA8X5X_I2C_BUS DT_BUS(DT_NODELABEL(mma8x5x))

#define NVS_PARTITION_ID DT_REG_ADDR(DT_NODELABEL(nvs))
#define NVS_SECTOR_SIZE DT_PROP(DT_NODELABEL(nvs), sector_size)
#define NVS_SECTOR_COUNT DT_PROP(DT_NODELABEL(nvs), nvs_sector_count)

LOG_MODULE_REGISTER(main);

struct nvs_fs nvs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_SECTOR_COUNT,
    .offset = NVS_PARTITION_ID,
};

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(MMA8X5X_I2C_BUS);
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    int ret = nvs_init(&nvs, "nvs");
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    uint8_t accel_value;
    while (1) {
        ret = i2c_reg_read_byte(i2c_dev, MMA8X5X_I2C_ADDRESS, 0x00, &accel_value);
        if (ret) {
            LOG_ERR("Failed to read I2C register: %d", ret);
        } else {
            LOG_INF("Accelerometer value: %d", accel_value);
            ret = nvs_write(&nvs, 0, &accel_value, sizeof(accel_value));
            if (ret < 0) {
                LOG_ERR("NVS write failed: %d", ret);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
