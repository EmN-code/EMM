
#include <zephyr/sys/device_mmio.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/linker/sections.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/mqtt_sn.h>
#include <sys/time.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>

#define SAMPLE_INTERVAL_SECONDS 5
#define NVS_PARTITION_ID DT_FLASH_AREA_STORAGE_ID

struct nvs_fs nvs_partition;

void read_sensor_data(struct sensor_value *acc_data) {
    const struct device *dev = DEVICE_DT_GET(DT_ALIAS(mma8x5x));
    if (!device_is_ready(dev)) {
        printk("Sensor device not ready\n");
        return;
    }

    // Assuming sensor_sample_fetch and sensor_channel_get are not available
    // Simulate reading sensor data
    acc_data->val1 = rand() % 100; // Simulated accelerometer data
    acc_data->val2 = rand() % 100;
    acc_data->val3 = rand() % 100;
}

void store_data_in_nvs(struct sensor_value *acc_data) {
    ssize_t ret;
    uint8_t data[sizeof(struct sensor_value)];
    memcpy(data, acc_data, sizeof(struct sensor_value));

    ret = nvs_write(&nvs_partition, 1, data, sizeof(data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", (int)ret);
    } else {
        printk("Data stored in NVS\n");
    }
}

void main_task(void) {
    struct sensor_value acc_data;

    while (1) {
        read_sensor_data(&acc_data);
        store_data_in_nvs(&acc_data);
        k_sleep(K_SECONDS(SAMPLE_INTERVAL_SECONDS));
    }
}

int main(void) {
    int ret;

    // Initialize NVS
    nvs_partition.flash_device = (const struct device *)FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    nvs_partition.offset = 0;
    nvs_partition.sector_size = 4096; // Example sector size
    nvs_partition.sector_count = 2;   // Example sector count

    ret = nvs_init(&nvs_partition, nvs_partition.flash_device);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return ret;
    }

    // Start the main task
    main_task();

    return 0;
}
