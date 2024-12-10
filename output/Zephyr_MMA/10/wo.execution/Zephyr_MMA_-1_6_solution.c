
#include <zephyr/kernel.h>
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
#include <zephyr/net/tls_credentials.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/random/random.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_if.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/iterable_sections.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION_LABEL "storage"

void main(void) {
    struct nvs_fs fs;
    uint8_t data = 0;
    int ret;

    // Initialize NVS
    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_LABEL);
    if (!fs.flash_device) {
        return;
    }

    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_LABEL);
    ret = nvs_init(&fs, NVS_PARTITION_LABEL);
    if (ret) {
        return;
    }

    while (1) {
        // Simulate reading from MMA8x5x
        data = (uint8_t)sys_rand32_get();

        // Store the read value persistently in NVS
        ret = nvs_write(&fs, 1, &data, sizeof(data));
        if (ret < 0) {
            return;
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}
