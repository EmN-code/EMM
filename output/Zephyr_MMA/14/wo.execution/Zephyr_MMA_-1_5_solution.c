
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/syscalls/i2c.h>
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
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/random/random.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_if.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mutex.h>
#include <sys/time.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>

#define MMA8x5x_I2C_ADDR 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION DT_NODELABEL(nvs)

void read_mma8x5x(uint8_t *buf) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return;
    }

    struct i2c_msg msgs[2];
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;

    msgs[0].buf = &reg_addr;
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE;

    msgs[1].buf = buf;
    msgs[1].len = 6;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_RESTART;

    i2c_transfer(i2c_dev, msgs, 2, MMA8x5x_I2C_ADDR);
}

void store_to_nvs(uint8_t *data, size_t len) {
    const struct device *flash_dev = DEVICE_DT_GET(NVS_PARTITION);
    if (!device_is_ready(flash_dev)) {
        printk("Flash device not ready\n");
        return;
    }

    struct nvs_fs fs;
    fs.flash_device = flash_dev;
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    nvs_mount(&fs);
    nvs_write(&fs, 0, data, len);
}

int main(void) {
    uint8_t data[6];

    while (1) {
        read_mma8x5x(data);
        store_to_nvs(data, sizeof(data));
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
