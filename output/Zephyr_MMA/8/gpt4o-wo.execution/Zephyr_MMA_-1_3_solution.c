
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/sys/types.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/pm/state.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/linker/sections.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>
#include <time.h>
#include <sys/time.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define NVS_PARTITION_LABEL "nvs"
#define SENSOR_ID 1

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs fs;

int init_nvs(void) {
    struct flash_sector sectors[1];

    sectors[0].fs_size = 4096;
    sectors[0].fs_off = 0;
    sectors[0].fs_addr = FLASH_AREA_OFFSET(nvs);

    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = 1;
    fs.offset = sectors[0].fs_off;

    return nvs_init(&fs, NVS_PARTITION_LABEL);
}

int read_mma8x5x(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buffer[6];
    struct i2c_msg msgs[2];

    msgs[0].buf = buffer;
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE;

    msgs[1].buf = buffer;
    msgs[1].len = 6;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_RESTART;

    buffer[0] = 0x01; // Register to start reading from

    if (i2c_transfer(i2c_dev, msgs, 2, MMA8x5x_I2C_ADDRESS) != 0) {
        return -1;
    }

    *x = (buffer[0] << 8) | buffer[1];
    *y = (buffer[2] << 8) | buffer[3];
    *z = (buffer[4] << 8) | buffer[5];

    return 0;
}

int store_reading(int16_t x, int16_t y, int16_t z) {
    struct sensor_value data[3];

    data[0].val1 = x;
    data[1].val1 = y;
    data[2].val1 = z;

    return nvs_write(&fs, SENSOR_ID, data, sizeof(data));
}

int main(void) {
    int16_t x, y, z;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    if (init_nvs() != 0) {
        LOG_ERR("NVS initialization failed");
        return -1;
    }

    while (1) {
        if (read_mma8x5x(&x, &y, &z) == 0) {
            LOG_INF("MMA8x5x: x=%d, y=%d, z=%d", x, y, z);
            if (store_reading(x, y, z) != sizeof(struct sensor_value) * 3) {
                LOG_ERR("Failed to store reading");
            }
        } else {
            LOG_ERR("Failed to read MMA8x5x");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
