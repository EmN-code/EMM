
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
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/time.h>

#define MMA8x5x_DEVICE_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(storage)

LOG_MODULE_REGISTER(main);

struct nvs_fs fs;

void read_sensor_and_store(const struct device *dev)
{
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get data (%d)", ret);
        return;
    }

    uint8_t data[6];
    data[0] = (uint8_t)(accel[0].val1 & 0xFF);
    data[1] = (uint8_t)(accel[0].val1 >> 8);
    data[2] = (uint8_t)(accel[1].val1 & 0xFF);
    data[3] = (uint8_t)(accel[1].val1 >> 8);
    data[4] = (uint8_t)(accel[2].val1 & 0xFF);
    data[5] = (uint8_t)(accel[2].val1 >> 8);

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write NVS (%d)", ret);
    } else {
        LOG_INF("Stored data: %d %d %d", accel[0].val1, accel[1].val1, accel[2].val1);
    }
}

int main(void)
{
    const struct device *dev = device_get_binding(MMA8x5x_DEVICE_NAME);
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -ENODEV;
    }

    fs.flash_device = NVS_PARTITION_DEVICE;
    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = FIXED_PARTITION_SIZE(storage);
    fs.sector_count = 1;

    int ret = nvs_init(&fs, NVS_PARTITION_ID);
    if (ret) {
        LOG_ERR("Failed to init NVS (%d)", ret);
        return ret;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
