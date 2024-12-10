
#include <zephyr/sys/device_mmio.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/poll.h>
#include <time.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <sys/time.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>

#define MMA8X5X_I2C_ADDRESS 0x1D
#define NVS_PARTITION_LABEL "storage"
#define NVS_NAMESPACE "mma8x5x"

static struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_LABEL);
    fs.offset = FIXED_PARTITION_NODE_OFFSET(NVS_PARTITION_LABEL);
    fs.sector_size = FIXED_PARTITION_NODE_SIZE(NVS_PARTITION_LABEL);
    fs.id_size = 2;

    int ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
    }
}

void read_mma8x5x(void) {
    const struct device *dev = device_get_binding("I2C_0");
    if (!dev) {
        printk("I2C device not found\n");
        return;
    }

    uint8_t data[6];
    int ret = i2c_burst_read(dev, MMA8X5X_I2C_ADDRESS, 0x01, data, sizeof(data));
    if (ret < 0) {
        printk("I2C read failed: %d\n", ret);
        return;
    }

    int16_t x = (data[0] << 8) | data[1];
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];

    printk("MMA8X5X: x=%d, y=%d, z=%d\n", x, y, z);

    uint16_t value = (x & 0xFFFF) | ((y & 0xFFFF) << 16);
    ret = nvs_write(&fs, 1, &value, sizeof(value));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    }
}

void main(void) {
    init_nvs();

    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }
}
