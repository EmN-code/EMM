
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/linker/sections.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/device.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION   "storage"
#define MMA8X5X_DEVICE  "mma8x5x"

struct nvs_fs nvs_storage = {
    .sector_size = 4096,
    .sector_count = 2,
    .offset = 0,
};

void read_mma8x5x(void) {
    const struct device *dev = device_get_binding(MMA8X5X_DEVICE);
    if (!dev) {
        printk("Device %s not found\n", MMA8X5X_DEVICE);
        return;
    }

    // Simulate reading data from MMA8x5x
    int16_t data = 0x1234; // Replace with actual sensor reading logic

    // Store the reading value persistently using NVS
    ssize_t ret = nvs_write(&nvs_storage, 0, &data, sizeof(data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", (int)ret);
    } else {
        printk("NVS write success: %d bytes written\n", (int)ret);
    }
}

int main(void) {
    // Initialize NVS
    int ret = nvs_init(&nvs_storage, NVS_PARTITION);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return ret;
    }

    while (1) {
        read_mma8x5x();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
