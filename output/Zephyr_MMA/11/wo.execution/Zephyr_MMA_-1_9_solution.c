
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/posix/unistd.h>
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
#include <zephyr/net/wifi.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/init.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/random/random.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_if.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/toolchain.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mutex.h>
#include <sys/time.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/device.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/kernel.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 0

struct nvs_fs fs = {
    .sector_size = DT_N_INST_0_nvs_partition_P_sector_size,
    .sector_count = DT_N_INST_0_nvs_partition_P_nvs_sector_count,
    .offset = DT_N_INST_0_nvs_partition_REG_IDX_0_VAL_ADDRESS,
};

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_N_INST_0_nvs_partition);
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return -1;
    }

    int ret = nvs_init(&fs, "nvs");
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    uint8_t value;
    while (1) {
        ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, &value);
        if (ret) {
            printk("I2C read failed: %d\n", ret);
        } else {
            printk("MMA8x5x X-axis value: %d\n", value);
            ret = nvs_write(&fs, NVS_PARTITION_ID, &value, sizeof(value));
            if (ret < 0) {
                printk("NVS write failed: %d\n", ret);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
