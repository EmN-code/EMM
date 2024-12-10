#include <zephyr/sys/util.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/stats/stats.h>
#include <zephyr/pm/state.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/wifi.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi_mgmt.h>

#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <string.h>

#define MMA8X5X_I2C_ADDR 0x1D
#define STORAGE_KEY 0x0001

static const struct device *i2c_dev;

void read_and_store_mma8x5x_data(void) {
    uint8_t buffer[6]; // assuming MMA8x5x provides 6 bytes of data
    uint32_t sensor_value;

    if (i2c_burst_read(i2c_dev, MMA8X5X_I2C_ADDR, 0x00, buffer, sizeof(buffer)) == 0) {
        // Process buffer data here to a value
        sensor_value = buffer[0]; // hypothetical processing

        printk("Sensor value: %d\n", sensor_value);

        // Store sensor value in NVS (simulated by logging here)
        printk("Storing sensor value %d in NVS.\n", sensor_value);
        // nvs_write(nvs_fs, STORAGE_KEY, &sensor_value, sizeof(sensor_value));
    }
}

void main(void) {
    printk("Starting MMA8x5x sensor reading example\n");

    i2c_dev = device_get_binding("I2C_0"); // Use bindings based on your board and config
    if (!i2c_dev) {
        printk("Failed to get I2C device\n");
        return;
    }

    while (1) {
        read_and_store_mma8x5x_data();
        k_sleep(K_SECONDS(5));
    }
}