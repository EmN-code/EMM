
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/math_extras.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION_ID 0

void read_sensor_and_store(void) {
    // Simulate reading from MMA8x5x sensor
    int32_t sensor_value = sys_rand32_get();

    // Simulate storing the value in NVS
    printk("Storing sensor value: %d\n", sensor_value);
}

int main(void) {
    printk("Starting MMA8x5x sensor reading and NVS storage application\n");

    while (1) {
        read_sensor_and_store();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
