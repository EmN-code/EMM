
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/wifi.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/random/random.h>
#include <zephyr/linker/sections.h>
#include <time.h>
#include <sys/time.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs fs;

int read_mma8x5x(int16_t *x)
{
    uint8_t data[2];
    int ret;

    ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, &data[0]);
    if (ret) {
        LOG_ERR("Failed to read MMA8x5x X MSB");
        return ret;
    }

    ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB + 1, &data[1]);
    if (ret) {
        LOG_ERR("Failed to read MMA8x5x X LSB");
        return ret;
    }

    *x = (data[0] << 8) | data[1];
    return 0;
}

int main(void)
{
    int16_t x_value;
    int ret;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    struct flash_sector sector = {
        .fs_size = 4096,
        .fs_sector_count = 2,
    };

    fs.flash_device = FLASH_AREA_DEVICE(storage);
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = sector.fs_size;
    fs.sector_count = sector.fs_sector_count;

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("NVS init failed");
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(&x_value);
        if (ret) {
            LOG_ERR("Failed to read MMA8x5x");
        } else {
            LOG_INF("MMA8x5x X value: %d", x_value);
            ret = nvs_write(&fs, 0, &x_value, sizeof(x_value));
            if (ret < 0) {
                LOG_ERR("NVS write failed");
            } else {
                LOG_INF("NVS write successful");
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
