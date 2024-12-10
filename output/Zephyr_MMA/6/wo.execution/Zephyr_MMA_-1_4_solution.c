
#include <zephyr/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/net_ip.h>
#include <time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/random/random.h>
#include <zephyr/linker/sections.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>

#define FLASH_DEVICE_LABEL DT_LABEL(DT_ALIAS(flash_primary))
#define NVS_PARTITION_ID DT_FIXED_PARTITION_ID(DT_ALIAS(storage))

struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION_ID),
    .sector_count = DT_FIXED_PARTITION_SIZE(NVS_PARTITION_ID) / DT_REG_SIZE(NVS_PARTITION_ID),
    .offset = DT_REG_ADDR(NVS_PARTITION_ID),
};

void read_sensor_and_store() {
    // Simulate reading the MMA8x5x sensor
    uint32_t sensor_value = 12345; // Replace with actual sensor reading logic

    // Store the value in NVS
    int err = nvs_write(&nvs, 1, &sensor_value, sizeof(sensor_value));
    if (err < 0) {
        printk("NVS write failed: %d\n", err);
    } else {
        printk("Sensor value stored: %u\n", sensor_value);
    }
}

int main(void) {
    // Initialize NVS
    int err = nvs_init(&nvs, FLASH_DEVICE_LABEL);
    if (err) {
        printk("NVS init failed: %d\n", err);
        return err;
    }

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
