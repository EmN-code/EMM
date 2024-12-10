
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
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/net_if.h>
#include <zephyr/types.h>
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
#define NVS_NODE DT_NODELABEL(storage)

#define NVS_SECTOR_SIZE DT_REG_SIZE(NVS_NODE)
#define NVS_SECTOR_COUNT 4
#define NVS_OFFSET 0

#define SENSOR_READ_INTERVAL K_SECONDS(5)

struct nvs_fs nvs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_SECTOR_COUNT,
    .offset = NVS_OFFSET,
};

void read_sensor_and_store(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Sensor device not ready\n");
        return;
    }

    struct sensor_value accel[3];
    if (sensor_sample_fetch(dev) < 0) {
        printk("Failed to fetch sensor sample\n");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        printk("Failed to get sensor channel\n");
        return;
    }

    int16_t data[3];
    for (int i = 0; i < 3; i++) {
        data[i] = accel[i].val1;
    }

    ssize_t ret = nvs_write(&nvs, 0, data, sizeof(data));
    if (ret < 0) {
        printk("Failed to write to NVS: %d\n", (int)ret);
    } else {
        printk("Data written to NVS\n");
    }
}

int main(void) {
    if (nvs_init(&nvs, NVS_NODE) < 0) {
        printk("Failed to initialize NVS\n");
        return -1;
    }

    while (1) {
        read_sensor_and_store();
        k_sleep(SENSOR_READ_INTERVAL);
    }

    return 0;
}
