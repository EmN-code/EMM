
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <time.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
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
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define SLEEP_TIME_MS 5000

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    uint8_t buffer[6];
    int16_t x, y, z;

    while (1) {
        // Read accelerometer data
        i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buffer, 6);

        x = (buffer[0] << 8) | buffer[1];
        y = (buffer[2] << 8) | buffer[3];
        z = (buffer[4] << 8) | buffer[5];

        printk("X: %d, Y: %d, Z: %d\n", x, y, z);

        // Store the read value persistently in NVS
        struct nvs_fs fs;
        fs.sector_size = 4096;
        fs.sector_count = 2;
        fs.offset = 0;

        nvs_init(&fs, "nvs");
        nvs_write(&fs, 1, &x, sizeof(x));
        nvs_write(&fs, 2, &y, sizeof(y));
        nvs_write(&fs, 3, &z, sizeof(z));

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
