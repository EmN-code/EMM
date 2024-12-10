
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/wifi.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/slist.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/device.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/fdtable.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 0

struct nvs_fs fs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .offset = DT_REG_ADDR(DT_NODELABEL(storage)),
};

int read_mma8x5x(const struct device *i2c_dev, uint8_t *buf) {
    return i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
}

int store_reading(struct nvs_fs *fs, uint16_t id, const void *data, size_t len) {
    return nvs_write(fs, id, data, len);
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    if (nvs_mount(&fs) != 0) {
        printk("NVS mount failed\n");
        return -1;
    }

    uint8_t buffer[6];
    while (1) {
        if (read_mma8x5x(i2c_dev, buffer) == 0) {
            store_reading(&fs, NVS_PARTITION_ID, buffer, sizeof(buffer));
            printk("MMA8x5x data read and stored\n");
        } else {
            printk("Failed to read MMA8x5x data\n");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
