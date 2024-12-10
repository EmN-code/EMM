
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/rtio/rtio.h>
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
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
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
#include <zephyr/sys/mutex.h>
#include <sys/time.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/kernel.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 0

void read_mma8x5x(uint8_t *value) {
    struct i2c_msg msgs[2];
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;

    msgs[0].buf = &reg_addr;
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE;

    msgs[1].buf = value;
    msgs[1].len = 1;
    msgs[1].flags = I2C_MSG_READ;

    i2c_transfer(DEVICE_DT_GET(DT_NODELABEL(i2c_0)), msgs, 2, MMA8x5x_I2C_ADDRESS);
}

void store_value_in_nvs(uint8_t value) {
    struct nvs_fs fs;
    fs.flash_device = DEVICE_DT_GET(DT_NODELABEL(storage));
    fs.offset = DT_REG_ADDR(DT_NODELABEL(storage));
    fs.sector_size = DT_REG_SIZE(DT_NODELABEL(storage));
    fs.sector_count = 1;

    nvs_init(&fs, NULL);
    nvs_write(&fs, NVS_PARTITION_ID, &value, sizeof(value));
}

int main(void) {
    uint8_t mma8x5x_value;

    while (1) {
        read_mma8x5x(&mma8x5x_value);
        store_value_in_nvs(mma8x5x_value);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
