
#include <zephyr/sys/fdtable.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/net_ip.h>
#include <time.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/net/wifi.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/random/random.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_if.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/mutex.h>
#include <sys/time.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION_ID 1
#define NVS_NAMESPACE "storage"
#define NVS_KEY "accel_data"

int main(void) {
    uint8_t accel_data;
    struct nvs_fs fs;
    struct flash_area *fa;

    // Initialize NVS
    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_ID);
    fs.offset = FIXED_PARTITION_NODE_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = FIXED_PARTITION_NODE_SIZE(NVS_PARTITION_ID);
    fs.sector_count = fs.sector_size / sizeof(struct nvs_fs);

    if (nvs_init(&fs, NVS_NAMESPACE) != 0) {
        printk("NVS init failed\n");
        return -1;
    }

    // Initialize I2C
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    while (1) {
        // Read accelerometer data
        if (i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, &accel_data) != 0) {
            printk("Failed to read accelerometer data\n");
        } else {
            printk("Accelerometer data: %x\n", accel_data);

            // Store data in NVS
            if (nvs_write(&fs, NVS_KEY, &accel_data, sizeof(accel_data)) != sizeof(accel_data)) {
                printk("Failed to write data to NVS\n");
            }
        }

        // Sleep for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
