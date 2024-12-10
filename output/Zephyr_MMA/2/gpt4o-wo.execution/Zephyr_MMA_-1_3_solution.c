// Required headers
#include <zephyr/sys/util.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/net_if.h>
#include <zephyr/types.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/websocket.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/random/random.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/fs/nvs.h>


// Main function definition
int main(void) {
    // Initialize logging
    printk("Application started\n");

    // Initialize non-volatile storage
    static struct nvs_fs fs;
    struct flash_pages_info info;
    fs.offset = DT_FLASH_AREA_NVS_OFFSET;
    flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), fs.offset, &info);
    fs.sector_size = info.size;
    fs.sector_count = 3U;

    int rc = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (rc) {
        printk("NVS initialization failed\n");
        return rc;
    }

    // Find the accelerometer device
    const struct device *dev = device_get_binding("MMA8x5x");
    if (!dev) {
        printk("Failed to find sensor device\n");
        return -1;
    }

    while (1) {
        // Fetch sensor sample
        struct sensor_value accel[3];
        rc = sensor_sample_fetch(dev);
        if (rc) {
            printk("Sensor sampling failed\n");
            continue;
        }

        // Read accelerometer data
        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            printk("Failed to get sensor channel data\n");
            continue;
        }

        // Print and store data
        printk("Accelerometer: X=%d, Y=%d, Z=%d\n",
               sensor_value_to_double(&accel[0]),
               sensor_value_to_double(&accel[1]),
               sensor_value_to_double(&accel[2]));

        // Write the sensor value to non-volatile storage
        ssize_t bytes_written = nvs_write(&fs, STORAGE_KEY_1, &accel, sizeof(accel));
        if (bytes_written <= 0) {
            printk("Error writing to NVS\n");
        }

        // Sleep for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    return 0;
}