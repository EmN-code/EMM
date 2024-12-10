#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/nvs/nvs.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define NVS_STORAGE_ID 1

static struct nvs_fs nvs_fs_config;
static struct device *accel_dev;

void read_sensor_and_store_nvs(void) {
    struct sensor_value accel_value;
    int ret;
    
    ret = sensor_sample_fetch(accel_dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(accel_dev, SENSOR_CHAN_ACCEL_XYZ, &accel_value);
    if (ret) {
        LOG_ERR("Failed to get sensor channel (%d)", ret);
        return;
    }

    ret = nvs_write(&nvs_fs_config, NVS_STORAGE_ID, &accel_value, sizeof(accel_value));
    if (ret < 0) {
        LOG_ERR("NVS write error (%d)", ret);
    } else {
        LOG_INF("Stored value: %d", accel_value.val1);
    }
}

void init_nvs(void) {
    struct flash_sector fs;
    int ret;
    
    nvs_fs_config.offset = FLASH_AREA_OFFSET(storage);
    ret = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL),
                                      nvs_fs_config.offset, &fs);
    if (ret) {
        LOG_ERR("Failed to get flash page (%d)", ret);
        return;
    }

    nvs_fs_config.sector_size = fs.size;
    nvs_fs_config.sector_count = 3U;

    ret = nvs_init(&nvs_fs_config, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
    }
}

void main(void) {
    accel_dev = device_get_binding("MMA8X5X");
    if (!accel_dev) {
        LOG_ERR("Failed to find sensor device");
        return;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store_nvs();
        k_sleep(K_SECONDS(5));
    }
}