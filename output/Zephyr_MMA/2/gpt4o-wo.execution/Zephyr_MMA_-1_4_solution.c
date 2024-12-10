
#include <zephyr/sys/util.h>
#include <zephyr/types.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/device.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/random/random.h>
#include <zephyr/net/buf.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/stats/stats.h>
#include <zephyr/pm/state.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/socket.h>
#include <sys/time.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/linker/sections.h>
#include <zephyr/devicetree.h>
#include <time.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/websocket.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/fs/nvs.h>

#define STACKSIZE 1024
#define PRIORITY 7

void main(void) {
    struct nvs_fs fs;
    int err;
    
    fs.offset = DT_FLASH_AREA_STORAGE_OFFSET;
    fs.sector_size = 4096;
    fs.sector_count = 3;

    err = nvs_mount(&fs);
    if (err) {
        printk("NVS mount error: %d\n", err);
        return;
    }

    struct sensor_value accel_value;

    while (1) {
        k_sleep(K_SECONDS(5));

        // Simulate fetching sensor data
        accel_value.val1 = 1;  // Replace with real sensor reading
        accel_value.val2 = 234;

        // Store sensor data to NVS
        err = nvs_write(&fs, STORAGE_KEY_1, &accel_value, sizeof(accel_value));
        if (err < 0) {
            printk("Failed to write to NVS: %d\n", err);
        } else {
            printk("Data written to NVS: %d.%06d\n", accel_value.val1, accel_value.val2);
        }
    }
}