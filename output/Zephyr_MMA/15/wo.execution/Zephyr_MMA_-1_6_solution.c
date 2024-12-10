
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

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector *sectors;
    uint8_t num_sectors;

    err = flash_get_sectors(NVS_PARTITION_ID, &num_sectors, &sectors);
    if (err) {
        LOG_ERR("Failed to get flash sectors: %d", err);
        return;
    }

    fs.flash_device = DEVICE_DT_GET(DT_MTD_FROM_FIXED_PARTITION(storage));
    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device not ready");
        return;
    }

    fs.offset = sectors[0].fs_offset;
    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = num_sectors;

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("Failed to init NVS: %d", err);
    }
}

void read_sensor_and_store(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_NODE);
    struct sensor_value accel[3];
    int err;

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device not ready");
        return;
    }

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Failed to fetch sensor sample: %d", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Failed to get sensor channel: %d", err);
        return;
    }

    uint8_t data[3];
    data[0] = (uint8_t)sensor_value_to_double(&accel[0]);
    data[1] = (uint8_t)sensor_value_to_double(&accel[1]);
    data[2] = (uint8_t)sensor_value_to_double(&accel[2]);

    err = nvs_write(&fs, 1, data, sizeof(data));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS: %d", err);
    } else {
        LOG_INF("Stored sensor data in NVS");
    }
}

int main(void) {
    init_nvs();

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
