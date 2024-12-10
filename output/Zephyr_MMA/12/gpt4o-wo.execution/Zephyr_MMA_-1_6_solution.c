
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
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
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/device_mmio.h>
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

#define SENSOR_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    struct flash_sector_param sector;
    int rc;

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION);
    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION);
    fs.sector_size = FLASH_SECTOR_SIZE(NVS_PARTITION);
    fs.sector_count = FLASH_SECTOR_COUNT(NVS_PARTITION);

    rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &sector);
    if (rc) {
        LOG_ERR("Failed to get flash page info: %d", rc);
        return;
    }

    rc = nvs_init(&fs, fs.flash_device);
    if (rc) {
        LOG_ERR("Failed to initialize NVS: %d", rc);
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(SENSOR_NODE);
    struct sensor_value accel[3];
    int rc;

    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return -ENODEV;
    }

    init_nvs();

    while (1) {
        rc = sensor_sample_fetch(dev);
        if (rc) {
            LOG_ERR("Failed to fetch sensor sample: %d", rc);
            continue;
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            LOG_ERR("Failed to get sensor channel: %d", rc);
            continue;
        }

        rc = nvs_write(&fs, 1, &accel, sizeof(accel));
        if (rc < 0) {
            LOG_ERR("Failed to write to NVS: %d", rc);
        } else {
            LOG_INF("Written to NVS: %d bytes", rc);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
