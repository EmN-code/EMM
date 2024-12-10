
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/net_ip.h>
#include <time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/wifi.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/init.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/random/random.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_if.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/toolchain.h>
#include <zephyr/sys/mutex.h>
#include <sys/time.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>

#define MMA8x5x_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_NODE DT_NODELABEL(storage)

#define MMA8x5x_I2C_ADDR DT_PROP(MMA8x5x_NODE, reg)
#define STORAGE_FLASH_ADDRESS DT_REG_ADDR(STORAGE_NODE)
#define STORAGE_FLASH_SIZE DT_REG_SIZE(STORAGE_NODE)

#define SLEEP_TIME_MS 5000

int main(void) {
    uint8_t buffer[2];
    int ret;

    // Initialize the I2C bus for MMA8x5x
    const struct device *i2c_dev = DEVICE_DT_GET(DT_BUS(MMA8x5x_NODE));
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return -1;
    }

    // Initialize the flash storage
    const struct device *flash_dev = DEVICE_DT_GET(STORAGE_NODE);
    if (!device_is_ready(flash_dev)) {
        printk("Flash device not ready\n");
        return -1;
    }

    while (1) {
        // Read data from MMA8x5x
        ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDR, 0x00, buffer, sizeof(buffer));
        if (ret < 0) {
            printk("Failed to read from MMA8x5x\n");
        } else {
            printk("MMA8x5x data: %x %x\n", buffer[0], buffer[1]);

            // Write data to flash storage
            ret = flash_write(flash_dev, STORAGE_FLASH_ADDRESS, buffer, sizeof(buffer));
            if (ret < 0) {
                printk("Failed to write to flash\n");
            } else {
                printk("Data written to flash\n");
            }
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
