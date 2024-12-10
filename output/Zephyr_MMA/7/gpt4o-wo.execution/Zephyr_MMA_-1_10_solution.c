
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/types.h>
#include <zephyr/net/wifi.h>
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
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/net/net_if.h>
#include <zephyr/kernel.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/net_if.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/device.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = FLASH_AREA_DEVICE(storage_partition);
    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);
    fs.sector_size = 4096; // Example sector size, adjust as needed
    fs.sector_count = 2;   // Example sector count, adjust as needed

    int err = nvs_mount(&fs);
    if (err) {
        LOG_ERR("NVS mount failed: %d", err);
    } else {
        LOG_INF("NVS mounted successfully");
    }
}

void read_sensor_and_store(void) {
    const struct device *dev = device_get_binding("MMA8x5x");
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return;
    }

    struct sensor_value accel[3];
    int err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Sensor fetch failed: %d", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Sensor channel get failed: %d", err);
        return;
    }

    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    ssize_t written = nvs_write(&fs, NVS_PARTITION_ID, data, sizeof(data));
    if (written < 0) {
        LOG_ERR("NVS write failed: %d", (int)written);
    } else {
        LOG_INF("NVS write successful: %d bytes", (int)written);
    }
}

int main(void) {
    init_nvs();

    while (1) {
        read_sensor_and_store();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
