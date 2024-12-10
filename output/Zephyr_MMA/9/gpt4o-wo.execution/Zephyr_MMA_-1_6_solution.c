
#include <zephyr/sys/mutex.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/init.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/wifi.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <time.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/devicetree.h>
#include <zephyr/linker/sections.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/toolchain.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/net/net_if.h>
#include <zephyr/types.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

#define SLEEP_TIME_MS 5000

struct nvs_fs fs;

void read_sensor(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        printk("Sensor fetch failed: %d\n", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Sensor get failed: %d\n", ret);
        return;
    }

    printk("X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the reading value persistently in NVS
    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));
    nvs_write(&fs, 1, data, sizeof(data));
}

int main(void) {
    const struct device *dev;
    int ret;

    dev = device_get_binding(DT_LABEL(MMA8X5X_NODE));
    if (!dev) {
        printk("Could not get MMA8X5X device\n");
        return -1;
    }

    if (!device_is_ready(dev)) {
        printk("MMA8X5X device is not ready\n");
        return -1;
    }

    fs.flash_device = DEVICE_DT_GET(NVS_PARTITION);
    if (!device_is_ready(fs.flash_device)) {
        printk("Flash device for NVS is not ready\n");
        return -1;
    }

    fs.offset = DT_REG_ADDR(NVS_PARTITION);
    fs.sector_size = DT_REG_SIZE(NVS_PARTITION);
    fs.sector_count = 1;

    ret = nvs_init(&fs, DT_LABEL(NVS_PARTITION));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    while (1) {
        read_sensor(dev);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
