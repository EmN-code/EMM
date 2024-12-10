
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
#include <sys/time.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define SENSOR_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_LABEL(DT_ALIAS(storage_partition))

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    struct flash_sector *sectors;
    int i;

    fs.flash_device = device_get_binding(NVS_PARTITION);
    if (!fs.flash_device) {
        LOG_ERR("Failed to get flash device binding");
        return;
    }

    fs.offset = 0;
    sectors = k_malloc(sizeof(struct flash_sector) * 2);
    if (!sectors) {
        LOG_ERR("Failed to allocate memory for sectors");
        return;
    }

    sectors[0].fs_size = 0x1000;
    sectors[0].fs_offset = 0;
    sectors[1].fs_size = 0x1000;
    sectors[1].fs_offset = 0x1000;

    fs.sector_size = 0x1000;
    fs.sector_count = 2;

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS: %d", ret);
    }
}

void read_sensor_and_store(void) {
    const struct device *dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!dev) {
        LOG_ERR("Failed to get device binding");
        return;
    }

    struct sensor_value accel[3];
    int ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sensor sample: %d", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor channel: %d", ret);
        return;
    }

    uint8_t data[3 * sizeof(struct sensor_value)];
    memcpy(data, accel, sizeof(data));

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    } else {
        LOG_INF("Successfully wrote to NVS: %d bytes", ret);
    }
}

int main(void) {
    init_nvs();

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
