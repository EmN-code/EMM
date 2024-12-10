#include <zephyr/types.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/buf.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/pm/state.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/init.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sys/util.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel.h>

#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>

#include <zephyr/storage/flash_map.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>

#define STORAGE_NODE DT_NODELABEL(storage)
#define STORAGE_PARTITION_LABEL storage

void mma8x5x_read_stub(uint8_t *accel_data) {
    // Stub function for reading from MMA8x5x sensor
    // In a real-world application, this function would interact with the device
    accel_data[0] = 0x01; // Placeholder value representing X-axis acceleration data
}

int main(void) {
    printk("Starting MMA8x5x accelerometer data reading and NVS storage application...\n");

    struct nvs_fs fs;
    struct flash_area *fa;
    uint8_t accel_data[1];

    int rc = flash_area_open(FLASH_AREA_ID(storage), &fa);
    if (rc) {
        printk("Failed to open flash area\n");
        return rc;
    }

    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 3U;

    rc = nvs_init(&fs, STORAGE_NODE);
    if (rc) {
        printk("NVS initialization failed\n");
        return rc;
    }

    while (1) {
        mma8x5x_read_stub(accel_data);

        rc = nvs_write(&fs, 1, accel_data, sizeof(accel_data));
        if (rc < 0) {
            printk("Failed to write accel data to NVS\n");
        }

        printk("Accel data "%2X" written to NVS\n", accel_data[0]);
        k_sleep(K_SECONDS(5));
    }
    return 0;
}
