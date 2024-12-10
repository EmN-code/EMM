
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/random/random.h>
#include <zephyr/posix/poll.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/types.h>
#include <zephyr/sys/hooks.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/pm/state.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/socket.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/kernel_includes.h>

#define MMA8x5x_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(nvs_partition)

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *dev, struct nvs_fs *fs) {
    struct sensor_value accel[3];
    int ret;

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device not ready");
        return;
    }

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data (%d)", ret);
        return;
    }

    uint8_t data[6];
    data[0] = (uint8_t)(accel[0].val1 & 0xFF);
    data[1] = (uint8_t)(accel[0].val1 >> 8);
    data[2] = (uint8_t)(accel[1].val1 & 0xFF);
    data[3] = (uint8_t)(accel[1].val1 >> 8);
    data[4] = (uint8_t)(accel[2].val1 & 0xFF);
    data[5] = (uint8_t)(accel[2].val1 >> 8);

    ret = nvs_write(fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_NODE);
    struct nvs_fs fs;
    int ret;

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device not ready");
        return -1;
    }

    fs.flash_device = DEVICE_DT_GET(NVS_PARTITION);
    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device not ready");
        return -1;
    }

    fs.offset = DT_REG_ADDR(NVS_PARTITION);
    fs.sector_size = DT_PROP(NVS_PARTITION, erase_block_size);
    fs.sector_count = DT_PROP(NVS_PARTITION, size) / fs.sector_size;

    ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS (%d)", ret);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
