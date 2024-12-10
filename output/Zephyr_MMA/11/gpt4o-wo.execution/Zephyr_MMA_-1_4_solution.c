
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
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
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/mem_stats.h>
#include <sys/time.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION   "storage"

struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = FLASH_AREA_DEVICE(storage);
    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = FIXED_PARTITION_SIZE(storage);
    fs.sector_count = fs.sector_size / fs.sector_size;

    int ret = nvs_init(&fs, NVS_PARTITION);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
    } else {
        printk("NVS init successful\n");
    }
}

void read_mma8x5x(void) {
    // Simulate reading from MMA8x5x sensor
    int16_t acc_x = sys_rand32_get() % 100;
    int16_t acc_y = sys_rand32_get() % 100;
    int16_t acc_z = sys_rand32_get() % 100;

    printk("MMA8x5x readings: X=%d, Y=%d, Z=%d\n", acc_x, acc_y, acc_z);

    // Store the readings in NVS
    uint16_t id = 1;
    size_t len = sizeof(acc_x) + sizeof(acc_y) + sizeof(acc_z);
    uint8_t data[len];
    memcpy(data, &acc_x, sizeof(acc_x));
    memcpy(data + sizeof(acc_x), &acc_y, sizeof(acc_y));
    memcpy(data + sizeof(acc_x) + sizeof(acc_y), &acc_z, sizeof(acc_z));

    ssize_t written = nvs_write(&fs, id, data, len);
    if (written < 0) {
        printk("NVS write failed: %zd\n", written);
    } else {
        printk("NVS write successful: %zd bytes written\n", written);
    }
}

int main(void) {
    printk("Starting MMA8x5x sensor reading and NVS storage example\n");

    init_nvs();

    while (1) {
        read_mma8x5x();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
