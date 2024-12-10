
#include <zephyr/kernel.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket_poll.h>
#include <time.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/device.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION   FIXED_PARTITION_ID(storage_partition)
#define NVS_ID          0x1234

struct nvs_fs fs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .offset = FIXED_PARTITION_OFFSET(storage_partition),
};

void read_sensor_and_store(void)
{
    // Simulate reading from MMA8x5x sensor
    uint32_t sensor_value = sys_rand32_get();

    // Store the reading value persistently using NVS
    int ret = nvs_write(&fs, NVS_ID, &sensor_value, sizeof(sensor_value));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("Sensor value %u stored in NVS\n", sensor_value);
    }
}

int main(void)
{
    // Initialize NVS file system
    int ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return ret;
    }

    while (1) {
        read_sensor_and_store();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
