
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/wifi.h>
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
#include <zephyr/net/websocket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/net_if.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_NODE DT_NODELABEL(storage)

#define NVS_SECTOR_SIZE DT_REG_SIZE(STORAGE_NODE)
#define NVS_SECTOR_COUNT 4
#define NVS_OFFSET DT_REG_ADDR(STORAGE_NODE)

struct nvs_fs nvs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_SECTOR_COUNT,
    .offset = NVS_OFFSET,
};

void read_sensor_data(const struct device *dev, struct sensor_value *val) {
    sensor_sample_fetch(dev);
    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, val);
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Sensor device not ready\n");
        return -1;
    }

    int ret = nvs_init(&nvs, DT_REG_ADDR(STORAGE_NODE));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    struct sensor_value val[3];
    while (1) {
        read_sensor_data(dev, val);
        nvs_write(&nvs, 1, val, sizeof(val));
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
