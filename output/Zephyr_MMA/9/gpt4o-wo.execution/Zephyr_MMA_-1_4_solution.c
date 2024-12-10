
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/linker/sections.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define SENSOR_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

#define NVS_FLASH_DEVICE DT_MTD_FROM_FIXED_PARTITION(NVS_PARTITION)
#define NVS_PARTITION_OFFSET DT_REG_ADDR(NVS_PARTITION)
#define NVS_PARTITION_SIZE DT_REG_SIZE(NVS_PARTITION)

#define SENSOR_READING_ID 0x1234

struct nvs_fs nvs = {
    .flash_device = NVS_FLASH_DEVICE,
    .offset = NVS_PARTITION_OFFSET,
    .sector_size = NVS_PARTITION_SIZE,
};

void read_sensor(const struct device *dev, struct sensor_value *val) {
    if (sensor_sample_fetch(dev) < 0) {
        printk("Sensor fetch failed\n");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, val) < 0) {
        printk("Failed to get sensor data\n");
        return;
    }
}

void store_reading(struct nvs_fs *fs, uint16_t id, const struct sensor_value *val) {
    ssize_t ret = nvs_write(fs, id, val, sizeof(struct sensor_value));
    if (ret < 0) {
        printk("NVS write failed: %d\n", (int)ret);
    } else {
        printk("NVS write success: %d bytes\n", (int)ret);
    }
}

int main(void) {
    const struct device *sensor_dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!sensor_dev) {
        printk("Could not get MMA8x5x device\n");
        return -1;
    }

    int ret = nvs_init(&nvs, DT_LABEL(NVS_PARTITION));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    struct sensor_value val;

    while (1) {
        read_sensor(sensor_dev, &val);
        store_reading(&nvs, SENSOR_READING_ID, &val);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
