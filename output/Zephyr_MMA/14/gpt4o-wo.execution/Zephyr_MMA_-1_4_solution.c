
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
#define NVS_PARTITION_ID 0
#define SAMPLE_INTERVAL K_SECONDS(5)

struct nvs_fs fs;

void read_mma8x5x(void) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return;
    }

    uint8_t data[6];
    if (i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, 0x01, data, 6) != 0) {
        printk("Failed to read from MMA8x5x\n");
        return;
    }

    int16_t x = (data[0] << 8) | data[1];
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];

    printk("MMA8x5x: x=%d, y=%d, z=%d\n", x, y, z);

    // Store the reading value persistently using NVS
    nvs_write(&fs, NVS_PARTITION_ID, &x, sizeof(x));
    nvs_write(&fs, NVS_PARTITION_ID + 1, &y, sizeof(y));
    nvs_write(&fs, NVS_PARTITION_ID + 2, &z, sizeof(z));
}

int main(void) {
    // Initialize NVS
    fs.flash_device = FIXED_PARTITION_DEVICE(storage);
    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = 4096;
    fs.sector_count = 8;

    if (nvs_mount(&fs) != 0) {
        printk("Failed to mount NVS\n");
        return -1;
    }

    while (1) {
        read_mma8x5x();
        k_sleep(SAMPLE_INTERVAL);
    }

    return 0;
}
