
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
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
#include <zephyr/net/buf.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/linker/sections.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/toolchain.h>
#include <zephyr/rtio/rtio.h>
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

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_NODE DT_NODELABEL(storage)

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_area *fa;

    err = flash_area_open(DT_REG_ADDR(STORAGE_NODE), &fa);
    if (err) {
        LOG_ERR("Failed to open flash area: %d", err);
        return;
    }

    fs.flash_device = (const struct device *)fa->fa_device;
    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 1;

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("Failed to initialize NVS: %d", err);
        return;
    }

    flash_area_close(fa);
}

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int err;

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Failed to fetch sensor sample: %d", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Failed to get sensor channel: %d", err);
        return;
    }

    uint8_t data[3 * sizeof(struct sensor_value)];
    memcpy(data, accel, sizeof(data));

    err = nvs_write(&fs, 1, data, sizeof(data));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS: %d", err);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device not ready");
        return -1;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
