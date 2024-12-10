#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(main);

#define SLEEP_TIME K_MSEC(5000)
#define SENSOR_NAME DT_LABEL(DT_INST(0, nxp_mma8451))

static struct nvs_fs fs;

void main(void) {
    const struct device *dev = device_get_binding(SENSOR_NAME);

    if (dev == NULL) {
        LOG_ERR("No device found");
        return;
    }

    struct sensor_value accel[3];
    int rc;

    /* Initialize NVS volume */
    struct flash_pages_info info;
    rc = flash_get_page_info(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER), 0, &info);
    if (rc) {
        LOG_ERR("Flash I/O error");
        return;
    }

    fs.offset = DT_CHOSEN_ZEPHYR_SCRATCH_CONTROLLER_OFFSET;
    fs.sector_size = info.size;
    fs.sector_count = 3U;
    fs.__bus = NULL;

    rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("Flash Init failed");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(dev);
        if (rc) {
            LOG_ERR("Data fetch error");
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            LOG_ERR("Accelerometer read error");
        }

        LOG_INF("Accel X: %d.%06d, Y: %d.%06d, Z: %d.%06d", 
                accel[0].val1, accel[0].val2, accel[1].val1, accel[1].val2, accel[2].val1, accel[2].val2);

        rc = nvs_write(&fs, 1, &accel[0], sizeof(accel));

        if (rc < 0) {
            LOG_ERR("NVS write error: %d", rc);
        }

        k_sleep(SLEEP_TIME);
    }
}