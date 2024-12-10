
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

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_NODE DT_NODELABEL(storage)

static struct nvs_fs nvs_storage = {
	.sector_size = DT_REG_SIZE(NVS_NODE),
	.sector_count = DT_REG_SIZE(NVS_NODE) / DT_REG_SIZE(NVS_NODE),
	.offset = DT_REG_ADDR(NVS_NODE),
};

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Device not ready\n");
        return -1;
    }

    int ret = nvs_init(&nvs_storage, DT_REG_ADDR(NVS_NODE));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    while (1) {
        // Simulate reading MMA8x5x sensor
        uint32_t sensor_value = sys_rand32_get();

        // Store the reading value persistently
        ret = nvs_write(&nvs_storage, 1, &sensor_value, sizeof(sensor_value));
        if (ret < 0) {
            printk("NVS write failed: %d\n", ret);
        } else {
            printk("Sensor value stored: %u\n", sensor_value);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
