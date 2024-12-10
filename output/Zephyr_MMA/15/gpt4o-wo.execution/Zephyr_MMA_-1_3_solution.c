
#include <zephyr/sys/device_mmio.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/poll.h>
#include <time.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/util.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <sys/time.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>

#define MMA8X5X_I2C_ADDRESS 0x1D
#define NVS_PARTITION_LABEL "storage"

static const struct device *mma8x5x_dev;
static struct nvs_fs nvs;

void init_mma8x5x(void) {
    mma8x5x_dev = device_get_binding("MMA8X5X");
    if (!mma8x5x_dev) {
        printk("Error: Could not get MMA8X5X device\n");
        return;
    }
    if (!device_is_ready(mma8x5x_dev)) {
        printk("Error: MMA8X5X device is not ready\n");
        return;
    }
}

void init_nvs(void) {
    int ret;

    nvs.sector_size = 4096;
    nvs.sector_count = 2;
    nvs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_LABEL);

    ret = nvs_mount(&nvs);
    if (ret) {
        printk("Error: NVS mount failed: %d\n", ret);
        return;
    }
}

void read_mma8x5x(void) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(mma8x5x_dev);
    if (ret) {
        printk("Error: MMA8X5X sample fetch failed: %d\n", ret);
        return;
    }

    ret = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Error: MMA8X5X channel get failed: %d\n", ret);
        return;
    }

    printk("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the reading value persistently in NVS
    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));
    ret = nvs_write(&nvs, 1, data, sizeof(data));
    if (ret < 0) {
        printk("Error: NVS write failed: %d\n", ret);
    }
}

int main(void) {
    init_mma8x5x();
    init_nvs();

    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
