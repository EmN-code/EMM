
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/wifi.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/stats/stats.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/net_if.h>
#include <zephyr/types.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION   FIXED_PARTITION_ID(storage_partition)

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .offset = NVS_PARTITION,
};

void read_sensor_and_store(const struct device *dev)
{
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data (%d)", ret);
        return;
    }

    ret = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Stored sensor data in NVS");
    }
}

int main(void)
{
    const struct device *dev;
    int ret;

    dev = device_get_binding("MMA8X5X");
    if (!dev) {
        LOG_ERR("Could not get MMA8X5X device");
        return -ENODEV;
    }

    ret = nvs_init(&fs, "storage");
    if (ret) {
        LOG_ERR("Failed to init NVS (%d)", ret);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
