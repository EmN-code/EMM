
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
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/kernel_includes.h>

#define MMA8x5x_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION DT_LABEL(DT_ALIAS(storage_partition))

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector *sectors;
    uint32_t sector_count;

    sectors = flash_get_sectors(DT_REG_ADDR(NVS_PARTITION), &sector_count);
    if (!sectors) {
        LOG_ERR("Failed to get flash sectors");
        return;
    }

    fs.flash_device = DEVICE_DT_GET(NVS_PARTITION);
    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device %s is not ready", fs.flash_device->name);
        return;
    }

    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = sector_count;
    fs.offset = sectors[0].fs_off;

    err = nvs_mount(&fs);
    if (err) {
        LOG_ERR("Failed to mount NVS (err %d)", err);
    }
}

void read_sensor_and_store(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_NODE);
    struct sensor_value accel[3];
    int err;

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor %s is not ready", dev->name);
        return;
    }

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Failed to fetch sample (err %d)", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Failed to get sensor data (err %d)", err);
        return;
    }

    int32_t x = sensor_value_to_double(&accel[0]) * 1000;
    int32_t y = sensor_value_to_double(&accel[1]) * 1000;
    int32_t z = sensor_value_to_double(&accel[2]) * 1000;

    err = nvs_write(&fs, 1, &x, sizeof(x));
    if (err < 0) {
        LOG_ERR("Failed to write x value to NVS (err %d)", err);
    }

    err = nvs_write(&fs, 2, &y, sizeof(y));
    if (err < 0) {
        LOG_ERR("Failed to write y value to NVS (err %d)", err);
    }

    err = nvs_write(&fs, 3, &z, sizeof(z));
    if (err < 0) {
        LOG_ERR("Failed to write z value to NVS (err %d)", err);
    }

    LOG_INF("Stored x: %d, y: %d, z: %d", x, y, z);
}

int main(void) {
    init_nvs();

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
