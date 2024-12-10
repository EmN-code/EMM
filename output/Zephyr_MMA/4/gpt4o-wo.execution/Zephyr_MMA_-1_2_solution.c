#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/buf.h>
#include <zephyr/pm/state.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/linker/sections.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/device.h>
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/sys/socket.h>

#define MMA8X5X_I2C_ADDR 0x1D

static const struct device *i2c_dev;

void main(void) {
    uint8_t sensor_data[6];
    int ret;

    i2c_dev = device_get_binding(DT_LABEL(DT_NODELABEL(i2c1)));
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return;
    }

    struct nvs_fs fs = {
        .sector_size = FLASH_AREA_SIZE(storage),
        .sector_count = 3,
        .offset = FLASH_AREA_OFFSET(storage),
    };

    ret = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (ret) {
        printk("NVS Init failed\n");
        return;
    }

    while (1) {
        ret = i2c_reg_read_byte(i2c_dev, MMA8X5X_I2C_ADDR, 0x00, &sensor_data[0]);
        if (!ret) {
            printk("Read sensor: %d\n", sensor_data[0]);
            nvs_write(&fs, 1, &sensor_data, sizeof(sensor_data));
        } else {
            printk("Failed to read sensor data\n");
        }

        k_sleep(K_SECONDS(5));
    }
}