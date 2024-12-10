
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/math_extras.h>

#define SENSOR_ID 1
#define NVS_PARTITION_ID 2

struct nvs_fs nvs_partition = {
    .sector_size = 4096,
    .sector_count = 2,
    .offset = 0,
};

void read_sensor(int *value) {
    // Simulate reading the MMA8x5x sensor
    *value = rand() % 100;
}

void store_nvs(int value) {
    nvs_write(&nvs_partition, SENSOR_ID, &value, sizeof(value));
}

int main(void) {
    int sensor_value;

    // Initialize NVS
    nvs_init(&nvs_partition, NVS_PARTITION_ID);

    while (1) {
        // Read sensor value
        read_sensor(&sensor_value);

        // Store the sensor value in NVS
        store_nvs(sensor_value);

        // Print the sensor value
        printk("Sensor value: %d\n", sensor_value);

        // Sleep for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
