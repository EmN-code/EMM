
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
#include <zephyr/sys/slist.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/toolchain.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
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

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

#define SENSOR_CHAN_ACCEL_XYZ 0x01

struct nvs_fs fs;

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Device MMA8X5X not ready\n");
        return -1;
    }

    struct flash_sector sectors[1];
    const struct device *flash_dev = DEVICE_DT_GET(NVS_PARTITION);
    if (!device_is_ready(flash_dev)) {
        printk("Flash device not ready\n");
        return -1;
    }

    sectors[0].fs_size = 4096;
    sectors[0].offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 1;
    fs.offset = 0;
    fs.flash_device = flash_dev;

    int ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return -1;
    }

    while (1) {
        struct sensor_value accel[3];
        sensor_sample_fetch(dev);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);

        uint8_t data[12];
        memcpy(data, accel, sizeof(accel));

        ret = nvs_write(&fs, 1, data, sizeof(data));
        if (ret < 0) {
            printk("NVS write failed: %d\n", ret);
        } else {
            printk("NVS write success\n");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
