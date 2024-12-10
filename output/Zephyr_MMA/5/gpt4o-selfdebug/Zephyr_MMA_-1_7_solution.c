#include <zephyr.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/settings/settings.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define STORAGE_NODE DT_NODELABEL(storage)
#define NVS_SECTOR_SIZE DT_PROP(STORAGE_NODE, size) / 4
#define NVS_SECTOR_COUNT 3
#define SLEEP_TIME_MS 5000

struct flash_sector nvs_sectors[NVS_SECTOR_COUNT];
static struct nvs_fs fs;

void main(void) {
    const struct device *sensor = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!sensor) {
        LOG_ERR("Failed to find sensor");
        return;
    }

    /* Set up NVS */
    int rc = flash_area_open(DT_FIXED_PARTITION_ID(DT_NODELABEL(storage)), &fs.area);
    if (rc) {
        LOG_ERR("Failed to open flash area");
        return;
    }

    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    rc = nvs_init(&fs, DT_FLASH_AREA_STORAGE_OFFSET);
    if (rc) {
        LOG_ERR("Failed to initialize NVS");
        return;
    }

    struct sensor_value accel[3];

    while (1) {
        rc = sensor_sample_fetch(sensor);
        if (rc) {
            LOG_ERR("Sensor sample update error");
            continue;
        }

        rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            LOG_ERR("Sensor channel get error");
            continue;
        }

        LOG_INF("Accel X: %d.%06d; Y: %d.%06d; Z: %d.%06d", accel[0].val1, accel[0].val2, accel[1].val1, accel[1].val2, accel[2].val1, accel[2].val2);

        uint32_t accel_data[3] = {accel[0].val1, accel[1].val1, accel[2].val1};
        rc = nvs_write(&fs, 1, &accel_data, sizeof(accel_data));
        if (rc < 0) {
            LOG_ERR("Failed to write data to NVS");
            continue;
        }

        k_msleep(SLEEP_TIME_MS);
    }
}