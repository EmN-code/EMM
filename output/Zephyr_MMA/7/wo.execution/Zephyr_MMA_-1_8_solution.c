
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

#define MMA8x5x_I2C_ADDR 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(storage)
#define NVS_PARTITION_SIZE FIXED_PARTITION_SIZE(storage)

LOG_MODULE_REGISTER(main);

void read_mma8x5x(const struct device *i2c_dev, uint8_t *value) {
    if (i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDR, MMA8x5x_REG_OUT_X_MSB, value) != 0) {
        LOG_ERR("Failed to read from MMA8x5x sensor");
    }
}

void store_value_in_nvs(struct nvs_fs *fs, uint16_t id, uint8_t value) {
    if (nvs_write(fs, id, &value, sizeof(value)) != 0) {
        LOG_ERR("Failed to write to NVS");
    }
}

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    struct nvs_fs fs;
    fs.flash_device = DEVICE_DT_GET(NVS_PARTITION_ID);
    fs.offset = NVS_PARTITION_OFFSET;
    fs.sector_size = NVS_PARTITION_SIZE;
    fs.sector_count = 1;

    if (nvs_init(&fs, NVS_PARTITION_ID) != 0) {
        LOG_ERR("Failed to initialize NVS");
        return -1;
    }

    uint8_t value;
    while (1) {
        read_mma8x5x(i2c_dev, &value);
        store_value_in_nvs(&fs, 1, value);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
