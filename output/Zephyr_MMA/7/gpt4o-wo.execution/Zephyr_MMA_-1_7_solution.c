
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/types.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/stats/stats.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/net/net_if.h>
#include <zephyr/kernel.h>
#include <time.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <sys/time.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_PARTITION DT_NODELABEL(storage_partition)

LOG_MODULE_REGISTER(main);

void read_sensor(const struct device *dev, struct sensor_value *val)
{
    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, val) < 0) {
        LOG_ERR("Failed to get sample");
        return;
    }
}

int main(void)
{
    const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(mma8x5x_dev)) {
        LOG_ERR("MMA8X5X device not ready");
        return -ENODEV;
    }

    struct nvs_fs fs;
    fs.flash_device = DEVICE_DT_GET(STORAGE_PARTITION);
    fs.offset = DT_REG_ADDR(STORAGE_PARTITION);
    fs.sector_size = DT_REG_SIZE(STORAGE_PARTITION);
    fs.sector_count = 1;

    if (nvs_mount(&fs) < 0) {
        LOG_ERR("Failed to mount NVS");
        return -EIO;
    }

    struct sensor_value accel_val;
    uint16_t id = 1;

    while (1) {
        read_sensor(mma8x5x_dev, &accel_val);
        int32_t accel_x = accel_val.val1;

        if (nvs_write(&fs, id, &accel_x, sizeof(accel_x)) < 0) {
            LOG_ERR("Failed to write to NVS");
        } else {
            LOG_INF("Written to NVS: %d", accel_x);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
