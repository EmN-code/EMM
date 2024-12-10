#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/settings/settings.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define SENSOR_NODE DT_NODELABEL(mma8x5x)

#if !DT_NODE_HAS_STATUS(SENSOR_NODE, okay)
#error "Unsupported board: sensor devicetree alias is not defined"
#endif

#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 3
static const struct flash_area *fa;

static struct nvs_fs fs;
static struct sensor_value sensor_val;

void main(void) {
    int ret;

    /* Flash area for NVS */
    ret = flash_area_open(DT_FLASH_AREA_STORAGE_ID, &fa);
    if (ret) {
        LOG_ERR("Flash area open failed");
        return;
    }

    fs.offset = fa->fa_offset;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed");
        return;
    }

    const struct device *sensor = DEVICE_DT_GET(SENSOR_NODE);

    if (!device_is_ready(sensor)) {
        LOG_ERR("Sensor device not found");
        return;
    }

    while (true) {
        ret = sensor_sample_fetch(sensor);
        if (ret) {
            LOG_ERR("Failed to fetch sensor sample");
        } else {
            ret = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &sensor_val);
            if (ret) {
                LOG_ERR("Failed to get sensor channel");
            }
            LOG_INF("Accel X: %d.%06d, Y: %d.%06d, Z: %d.%06d",
                    sensor_val.val1, sensor_val.val2, sensor_val.val1,
                    sensor_val.val2, sensor_val.val1, sensor_val.val2);

            /* Store the reading in NVS */
            ret = nvs_write(&fs, 1, &sensor_val, sizeof(sensor_val));
            if (ret < 0) {
                LOG_ERR("Failed to store sensor value in NVS");
            }
        }

        k_sleep(K_SECONDS(5));
    }
}