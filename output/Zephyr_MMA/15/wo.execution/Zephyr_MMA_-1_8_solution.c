
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

#define MMA8x5x_DEVICE_LABEL DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_LABEL DT_LABEL(DT_ALIAS(storage_partition))

LOG_MODULE_REGISTER(main);

int main(void) {
    const struct device *mma8x5x_dev = device_get_binding(MMA8x5x_DEVICE_LABEL);
    if (!device_is_ready(mma8x5x_dev)) {
        LOG_ERR("MMA8x5x device not ready");
        return -ENODEV;
    }

    struct nvs_fs nvs;
    struct flash_area *fa;
    int rc;

    rc = flash_area_open(DT_FLASH_AREA_ID(storage_partition), &fa);
    if (rc) {
        LOG_ERR("Failed to open flash area");
        return -EIO;
    }

    nvs.flash_device = (const struct device *)fa->fa_device;
    nvs.offset = fa->fa_off;
    nvs.sector_size = DT_PROP(DT_ALIAS(storage_partition), erase_block_size);
    nvs.sector_count = DT_PROP(DT_ALIAS(storage_partition), size) / nvs.sector_size;

    rc = nvs_mount(&nvs);
    if (rc) {
        LOG_ERR("Failed to mount NVS");
        return -EIO;
    }

    while (1) {
        struct sensor_value accel_val;
        rc = sensor_sample_fetch(mma8x5x_dev);
        if (rc) {
            LOG_ERR("Failed to fetch sensor sample");
            continue;
        }

        rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, &accel_val);
        if (rc) {
            LOG_ERR("Failed to get sensor channel");
            continue;
        }

        rc = nvs_write(&nvs, 0, &accel_val, sizeof(accel_val));
        if (rc < 0) {
            LOG_ERR("Failed to write to NVS");
        } else {
            LOG_INF("Accel value stored in NVS");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
