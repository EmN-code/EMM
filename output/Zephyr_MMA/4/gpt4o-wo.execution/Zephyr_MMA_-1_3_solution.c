#include <zephyr/types.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/random/random.h>
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

#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <time.h>
#include <sys/time.h>

#define STORAGE_LABEL "storage"
#define ACCELEROMETER_LABEL "mma8451"

static struct nvs_fs fs;
static const struct device *i2c_dev;

void setup_nvs(void) {
    int ret;
    
    fs.sector_size = 4096;
    fs.sector_count = 3;
    fs.offset = FLASH_AREA_OFFSET(storage);

    ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed\n");
    }
}

void read_mma8x5x_and_store(void) {
    uint8_t sensor_data[6]; // Assuming the sensor has 6 bytes of data
    int ret;
    
    // Here you would call the function to read from I2C
    // ret = i2c_burst_read(i2c_dev, MMA8451_I2C_ADDRESS, MMA8451_REG_OUT_X_MSB, sensor_data, sizeof(sensor_data));

    if (ret) {
        printk("Failed to read sensor data\n");
        return;
    }

    ret = nvs_write(&fs, 1, sensor_data, sizeof(sensor_data));
    if (ret < 0) {
        printk("Failed to write sensor data to NVS\n");
    } else {
        printk("Sensor data written to NVS\n");
    }
}

void main(void) {
    setup_nvs();

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        printk("I2C: Device driver not found.\n");
        return;
    }

    while (1) {
        read_mma8x5x_and_store();
        k_sleep(K_SECONDS(5));
    }
}
