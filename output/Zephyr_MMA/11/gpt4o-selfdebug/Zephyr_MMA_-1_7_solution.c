#include <zephyr.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <logging/log.h>
#include <zephyr/fs/nvs.h>

LOG_MODULE_REGISTER(mma8x5x_sample);

#define SENSOR_READ_INTERVAL_SECONDS 5
#define NVS_SECTOR_SIZE 4096 /* Make sure this matches your NVS configuration. */
#define NVS_SECTOR_COUNT 3 /* Number of sectors */
#define NVS_FLASH_DEVICE DT_CHOSEN(zephyr_flash_controller)

static struct nvs_fs fs;
static const uint16_t data_id = 1;

void mma8x5x_sample_thread(void) {
    const struct device *sensor_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!sensor_dev) {
        LOG_ERR("Could not find MMA8x5x device!");
        return;
    }

    struct nvs_fs fs;
    struct flash_pages_info info;
    memset(&fs, 0, sizeof(fs));

    fs.offset = DT_REG_ADDR(NVS_FLASH_DEVICE);

    int rc = flash_get_page_info_by_offs(device_get_binding(NVS_FLASH_DEVICE), fs.offset, &info);
    if (rc) {
        LOG_ERR("Couldn't get page info");
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = NVS_SECTOR_COUNT;

    rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("Flash Init failed");
    }

    while (1) {
        struct sensor_value accel[3];
        if (sensor_sample_fetch(sensor_dev) < 0) {
            LOG_ERR("Sample fetch error");
            continue;
        }
        rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc < 0) {
            LOG_ERR("Get channel error");
            continue;
        }

        int32_t x = accel[0].val1;
        int32_t y = accel[1].val1;
        int32_t z = accel[2].val1;

        LOG_INF("Accel X: %d, Y: %d, Z: %d", x, y, z);

        /* Write sensor value to NVS */
        rc = nvs_write(&fs, data_id, &x, sizeof(x));
        if (rc < 0) {
            LOG_ERR("Failed to write to NVS");
        }

        k_sleep(K_SECONDS(SENSOR_READ_INTERVAL_SECONDS));
    }
}

K_THREAD_DEFINE(mma8x5x_thread_id, 1024, mma8x5x_sample_thread, NULL, NULL, NULL, 7, 0, 0);