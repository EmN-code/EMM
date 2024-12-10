
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_ALIAS(mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector_param sector[1];

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device not ready");
        return;
    }

    err = flash_get_page_info_by_offs(fs.flash_device, FLASH_AREA_OFFSET(NVS_PARTITION_ID), &sector[0]);
    if (err) {
        LOG_ERR("Unable to get flash page info");
        return;
    }

    fs.sector_size = sector[0].fs_size;
    fs.sector_count = 1;
    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION_ID);

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("NVS init failed");
    }
}

void store_reading(int32_t value) {
    int err;
    err = nvs_write(&fs, 0, &value, sizeof(value));
    if (err < 0) {
        LOG_ERR("NVS write failed");
    }
}

int main(void) {
    const struct device *dev;
    struct sensor_value accel[3];
    int32_t stored_value;

    dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device not ready");
        return -1;
    }

    init_nvs();

    while (1) {
        if (sensor_sample_fetch(dev) < 0) {
            LOG_ERR("Sensor sample fetch failed");
            continue;
        }

        if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
            LOG_ERR("Sensor channel get failed");
            continue;
        }

        int32_t reading = (accel[0].val1 + accel[1].val1 + accel[2].val1) / 3;
        store_reading(reading);

        LOG_INF("Stored reading: %d", reading);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
