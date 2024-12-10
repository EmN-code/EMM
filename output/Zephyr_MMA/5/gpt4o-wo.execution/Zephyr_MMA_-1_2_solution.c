#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/stats/stats.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/linker/sections.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/poll.h>
#include <zephyr/net/sockets.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/coap.h>
#include <zephyr/misc/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/io.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>

#define MMA8x5x_SENSOR_LABEL "MMA8x5x"

void main(void) {
    const struct flash_area *fa;
    int rc;

    /* Open the flash area */
    rc = flash_area_open(DT_FLASH_AREA_STORAGE_ID, &fa);
    if (rc != 0) {
        printk("Flash area failed to open.");
        return;
    }

    while (1) {
        /* Simulated reading from the sensor */
        uint8_t sensor_value = 123; // replace with actual sensor read

        /* Writing the sensor value to NVS */
        rc = flash_area_write(fa, 0, &sensor_value, sizeof(sensor_value));
        if (rc != 0) {
            printk("Failed to write sensor value to NVS.");
            flash_area_close(fa);
            return;
        }

        
        printk("Sensor value written to NVS: %d\n", sensor_value);
        k_sleep(K_SECONDS(5));
    }

    /* Close the flash area */
    flash_area_close(fa);
}