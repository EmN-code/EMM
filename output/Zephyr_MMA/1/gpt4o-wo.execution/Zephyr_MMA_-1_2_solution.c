#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/stats/stats.h>
#include <zephyr/pm/state.h>
#include <zephyr/linker/sections.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/util.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/socket_poll.h>

#define SAMPLE_INTERVAL 5000
#define SENSOR_CHANNEL 0

void simulate_sensor_read(int *sensor_value) {
    // Simulate reading from a sensor
    *sensor_value = 42;  // Example fixed value
}

void store_sensor_value_in_nvs(int sensor_value) {
    // Simulate storing the sensor value in NVS
    printk("Storing value %d in NVS\n", sensor_value);
}

int main() {
    int sensor_value;

    while (1) {
        simulate_sensor_read(&sensor_value);
        printk("Read sensor value: %d\n", sensor_value);
        store_sensor_value_in_nvs(sensor_value);

        k_sleep(SAMPLE_INTERVAL);
    }

    return 0;
}