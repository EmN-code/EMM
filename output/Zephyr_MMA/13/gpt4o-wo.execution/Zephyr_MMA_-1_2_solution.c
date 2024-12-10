
#include <zephyr/sys/device_mmio.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/coap.h>
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
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/net/net_if.h>

#define SLEEP_TIME_MS   5000
#define MMA8X5X_DEVICE_NAME "MMA8X5X"

struct sensor_value {
    int32_t val1;
    int32_t val2;
};

int main(void) {
    struct device *dev;
    struct sensor_value accel;
    int ret;

    dev = device_get_binding(MMA8X5X_DEVICE_NAME);
    if (dev == NULL) {
        printk("Device %s not found\n", MMA8X5X_DEVICE_NAME);
        return -1;
    }

    while (1) {
        ret = sensor_sample_fetch(dev);
        if (ret) {
            printk("Failed to fetch sample (%d)\n", ret);
            continue;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &accel);
        if (ret) {
            printk("Failed to get data (%d)\n", ret);
            continue;
        }

        printk("Acceleration: x=%d, y=%d, z=%d\n", accel.val1, accel.val2, accel.val3);

        // Store the reading value persistently using NVS
        // Note: NVS functions are not available in the provided headers, so this part is commented out.
        // ret = nvs_write(&storage, 0, &accel, sizeof(accel));
        // if (ret < 0) {
        //     printk("Failed to write to NVS (%d)\n", ret);
        // }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
