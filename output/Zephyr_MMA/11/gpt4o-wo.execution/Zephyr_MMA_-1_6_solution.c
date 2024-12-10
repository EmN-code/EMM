
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/kernel.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define SLEEP_TIME_MS 5000
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

static struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION_ID),
    .sector_count = DT_REG_SIZE(NVS_PARTITION_ID) / DT_REG_SIZE(NVS_PARTITION_ID),
    .offset = DT_REG_ADDR(NVS_PARTITION_ID),
};

int main(void) {
    int ret;
    uint8_t data = 0;

    // Initialize NVS
    ret = nvs_init(&nvs, DT_LABEL(NVS_PARTITION_ID));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return ret;
    }

    while (1) {
        // Simulate reading from MMA8x5x
        data = (uint8_t)sys_rand32_get();

        // Store the reading value persistently
        ret = nvs_write(&nvs, 1, &data, sizeof(data));
        if (ret < 0) {
            printk("NVS write failed: %d\n", ret);
        } else {
            printk("Data written to NVS: %d\n", data);
        }

        // Sleep for 5 seconds
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
