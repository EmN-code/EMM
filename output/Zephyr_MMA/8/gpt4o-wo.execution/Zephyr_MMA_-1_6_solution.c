
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/types.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/math_extras.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID

struct nvs_fs fs;

void read_mma8x5x(const struct device *i2c_dev, int16_t *x_axis) {
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;
    uint8_t data[2];

    struct i2c_msg msgs[2] = {
        {
            .buf = &reg_addr,
            .len = 1,
            .flags = I2C_MSG_WRITE | I2C_MSG_RESTART
        },
        {
            .buf = data,
            .len = 2,
            .flags = I2C_MSG_READ | I2C_MSG_STOP
        }
    };

    if (i2c_transfer(i2c_dev, msgs, 2, MMA8x5x_I2C_ADDRESS) != 0) {
        printk("I2C transfer failed\n");
        return;
    }

    *x_axis = (data[0] << 8) | data[1];
}

void store_reading(int16_t x_axis) {
    ssize_t ret = nvs_write(&fs, 0, &x_axis, sizeof(x_axis));
    if (ret < 0) {
        printk("NVS write failed: %d\n", (int)ret);
    } else {
        printk("Stored reading: %d\n", x_axis);
    }
}

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return -1;
    }

    struct flash_pages_info info;
    if (flash_get_page_info_by_offs(DEVICE_DT_GET(NVS_PARTITION), 0, &info) != 0) {
        printk("Failed to get flash page info\n");
        return -1;
    }

    fs.flash_device = DEVICE_DT_GET(NVS_PARTITION);
    fs.offset = info.start_offset;
    fs.sector_size = info.size;
    fs.sector_count = 1;

    if (nvs_init(&fs, fs.flash_device) != 0) {
        printk("NVS init failed\n");
        return -1;
    }

    while (1) {
        int16_t x_axis;
        read_mma8x5x(i2c_dev, &x_axis);
        store_reading(x_axis);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
