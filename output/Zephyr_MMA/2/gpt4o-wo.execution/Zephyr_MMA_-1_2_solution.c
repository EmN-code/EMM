#include <zephyr/sys/util.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/net_if.h>
#include <zephyr/types.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/random/random.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/net_mgmt.h>

#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/stats/stats.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/pm/state.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/sys/mem_stats.h>

#include <zephyr/syscalls/device.h>

static struct flash_area *fa;

int get_storage_area() {
    int rc = flash_area_open(DT_FIXED_PARTITION_ID(storage_partition), &fa);
    if (rc < 0) {
        printk("Failed to open flash area (%d)\n", rc);
    }
    return rc;
}

int my_main() {
    struct device *dev;
    struct sensor_value accel[3];
    
    printk("Starting the application\n");

    if (get_storage_area() < 0) {
        return -1;
    }

    dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_kinetis_i2c_40066000)));
    if (dev == NULL) {
        printk("Failed to get I2C device\n");
        return -1;
    }

    while (1) {
        k_sleep(K_SECONDS(5));

        if (sensor_sample_fetch(dev) < 0) {
            printk("Sensor fetch failed\n");
            continue;
        }

        if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
           printk("Failed to get sensor values\n");
           continue;
        }

        double x = sensor_value_to_double(&accel[0]);
        double y = sensor_value_to_double(&accel[1]);
        double z = sensor_value_to_double(&accel[2]);

        printk("Acceleration (x, y, z): (%f, %f, %f)\n", x, y, z);

        int err = nvs_write(nvs_get_storage(), NVS_FLASH_AREA_OFFSET,
                    (void *)&accel, sizeof(accel));
        if (err < 0) {
            printk("NVS write error (%d)\n", err);
        }
    }

    flash_area_close(fa);
    return 0;
}

