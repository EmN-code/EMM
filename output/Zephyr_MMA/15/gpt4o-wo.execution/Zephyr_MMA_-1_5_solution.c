
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/sys/math_extras.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION_LABEL "storage"

struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_LABEL);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_LABEL);
    fs.sector_size = FIXED_PARTITION_SIZE(NVS_PARTITION_LABEL);
    fs.sector_count = 3;

    int ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
    } else {
        printk("NVS mounted successfully\n");
    }
}

void store_reading(uint16_t id, const void *data, size_t len) {
    ssize_t ret = nvs_write(&fs, id, data, len);
    if (ret < 0) {
        printk("NVS write failed: %zd\n", ret);
    } else {
        printk("NVS write successful: %zd bytes written\n", ret);
    }
}

int main(void) {
    printk("Starting MMA8x5x reading and NVS storage application\n");

    init_nvs();

    while (1) {
        // Simulate reading from MMA8x5x sensor
        uint32_t reading = sys_rand32_get(); // Random value for simulation
        printk("Simulated reading: %u\n", reading);

        // Store the reading in NVS
        store_reading(1, &reading, sizeof(reading));

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
