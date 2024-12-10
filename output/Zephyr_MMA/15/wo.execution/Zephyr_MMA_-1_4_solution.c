
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/time.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/linker/sections.h>
#include <zephyr/pm/state.h>
#include <zephyr/kernel_includes.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

const struct device *i2c_dev;
struct nvs_fs nvs;

void read_mma8x5x(int16_t *x) {
    uint8_t buffer[2];
    i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buffer, 2);
    *x = (buffer[0] << 8) | buffer[1];
}

int main(void) {
    int16_t x_value;
    int ret;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    struct flash_area fa;
    ret = flash_area_open(NVS_PARTITION_ID, &fa);
    if (ret) {
        printk("Failed to open flash area\n");
        return -1;
    }

    nvs.offset = fa.fa_off;
    nvs.sector_size = 4096; // Assuming sector size is 4096 bytes
    nvs.sector_count = 2;   // Assuming 2 sectors

    ret = nvs_mount(&nvs);
    if (ret) {
        printk("Failed to mount NVS\n");
        return -1;
    }

    while (1) {
        read_mma8x5x(&x_value);
        printk("Read value: %d\n", x_value);

        ret = nvs_write(&nvs, 1, &x_value, sizeof(x_value));
        if (ret < 0) {
            printk("Failed to write to NVS\n");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
