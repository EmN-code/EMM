
#include <zephyr/sys/device_mmio.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/iterable_sections.h>
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
#include <zephyr/net/net_mgmt.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/net/net_if.h>

#define MMA8X5X_NODE DT_ALIAS(mma8x5x)
#define FLASH_AREA_STORAGE_NODE DT_FLASH_AREA_STORAGE_ID

struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = DEVICE_DT_GET(DT_MTD_FROM_IDX(FLASH_AREA_STORAGE_NODE));
    fs.offset = DT_REG_ADDR(FLASH_AREA_STORAGE_NODE);
    fs.sector_size = DT_REG_SIZE(FLASH_AREA_STORAGE_NODE);
    fs.sector_count = 1;

    int ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
    }
}

void read_mma8x5x(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("MMA8X5X device not ready\n");
        return;
    }

    struct sensor_value accel[3];
    sensor_sample_fetch(dev);
    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);

    int16_t data[3];
    for (int i = 0; i < 3; i++) {
        data[i] = accel[i].val1;
    }

    ssize_t ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", (int)ret);
    } else {
        printk("NVS write success: %d bytes\n", (int)ret);
    }
}

int main(void) {
    init_nvs();

    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
