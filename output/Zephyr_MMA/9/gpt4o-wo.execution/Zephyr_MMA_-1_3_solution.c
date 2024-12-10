
#include <zephyr/sys/device_mmio.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/coap.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/util.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>

#define MMA8x5x_DEVICE_NAME "MMA8x5x"
#define NVS_PARTITION_ID 1

static struct device *mma8x5x_dev;
static struct nvs_fs nvs;

void read_sensor_and_store(void)
{
    struct sensor_value accel[3];
    int ret;

    // Fetch sensor data
    ret = sensor_sample_fetch(mma8x5x_dev);
    if (ret) {
        printk("Sensor fetch failed: %d\n", ret);
        return;
    }

    // Get sensor data
    ret = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Sensor get failed: %d\n", ret);
        return;
    }

    // Store data in NVS
    ret = nvs_write(&nvs, NVS_PARTITION_ID, &accel, sizeof(accel));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("Data stored successfully\n");
    }
}

int main(void)
{
    int ret;

    // Initialize NVS
    nvs.flash_device = FIXED_PARTITION_DEVICE(DT_N_S_soc_S_flash_controller_3ff42000_S_flash_0_S_partitions_S_partition_250000);
    nvs.offset = FIXED_PARTITION_OFFSET(DT_N_S_soc_S_flash_controller_3ff42000_S_flash_0_S_partitions_S_partition_250000);
    nvs.sector_size = 4096;
    nvs.sector_count = 8;

    ret = nvs_init(&nvs, nvs.flash_device);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return ret;
    }

    // Get MMA8x5x device
    mma8x5x_dev = device_get_binding(MMA8x5x_DEVICE_NAME);
    if (!mma8x5x_dev) {
        printk("Failed to get MMA8x5x device\n");
        return -ENODEV;
    }

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
