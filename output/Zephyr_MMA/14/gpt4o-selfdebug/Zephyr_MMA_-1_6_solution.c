// Corrects missing header file problem and adds capabilities to read from MMA8x5x and store in NVS.

#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h> // corrected header inclusion
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(main);

#define STORAGE_PARTITION DT_LABEL(DT_FIXED_PARTITION_LABEL(storage))
#define NVS_SECTOR_SIZE   (FLASH_AREA_SIZE(storage))
#define NVS_SECTOR_COUNT  3

static struct nvs_fs nvs;

void main(void) {
    struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));
    if (!dev) {
        LOG_ERR("Failed to get device binding");
        return;
    }

    // Setup NVS
    struct flash_pages_info info;
    nvs.storage.flash_device = device_get_binding(STORAGE_PARTITION);
    nvs.storage.offset = FLASH_AREA_OFFSET(storage);
    flash_get_page_info_by_offs(nvs.storage.flash_device, nvs.storage.offset, &info);
    nvs.sector_size = info.size;
    nvs.sector_count = NVS_SECTOR_COUNT;
    nvs_init(&nvs, STORAGE_PARTITION);

    struct sensor_value val;
    while (1) {
        sensor_sample_fetch(dev);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &val);
        LOG_INF("Acceleration X: %d.%06d", val.val1, val.val2);

        // Store value in NVS
        int rc = nvs_write(&nvs, 1, &val, sizeof(val));
        if (rc >= 0) {
            LOG_INF("Stored value to NVS");
        } else {
            LOG_ERR("Error storing in NVS");
        }

        k_sleep(K_SECONDS(5));
    }
}