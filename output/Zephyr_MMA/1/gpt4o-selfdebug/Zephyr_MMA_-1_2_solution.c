#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs.h>

LOG_MODULE_REGISTER(mma8x5x_reader, LOG_LEVEL_INF);

#define MMA8X5X_NODE DT_INST(0, nxp_mma8652fc)

#if !DT_NODE_HAS_STATUS(MMA8X5X_NODE, okay)
#error "Unsupported board: mma8652fc devicetree label is not defined"
#endif

static struct nvs_fs fs;
static struct device *mma8x5x_dev;

void init_nvs(void) {
    struct flash_pages_info info;
    const struct device *flash_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_flash_controller));

    if (!device_is_ready(flash_dev)) {
        LOG_ERR("Flash device %s is not ready", flash_dev->name);
        return;
    }

    fs.offset = FLASH_AREA_OFFSET(storage);
    int rc = flash_get_page_info_by_offs(flash_dev, fs.offset, &info);
    if (rc) {
        LOG_ERR("Can't get page info: %d", rc);
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = 3U;  // change this as per your requirement

    rc = nvs_init(&fs, flash_dev->name);
    if (rc) {
        LOG_ERR("Flash Init failed: %d", rc);
        return;
    }
}

void read_and_store_sensor_data(void) {
    struct sensor_value accel[3];
    static uint16_t data_id = 0;
    int rc;

    while (1) {
        k_sleep(K_SECONDS(5));

        rc = sensor_sample_fetch(mma8x5x_dev);
        if (rc < 0) {
            LOG_ERR("Failed to fetch sensor sample: %d", rc);
            continue;
        }

        rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc < 0) {
            LOG_ERR("Failed to get sensor data: %d", rc);
            continue;
        }

        LOG_INF("Accel X: %d.%06d, Y: %d.%06d, Z: %d.%06d",
                accel[0].val1, accel[0].val2,
                accel[1].val1, accel[1].val2,
                accel[2].val1, accel[2].val2);

        rc = nvs_write(&fs, data_id, &accel, sizeof(accel));
        if (rc < 0) {
            LOG_ERR("Failed to write data to NVS: %d", rc);
        } else {
            data_id++;
        }
    }
}

void main(void) {
    mma8x5x_dev = device_get_binding(DT_LABEL(MMA8X5X_NODE));
    if (!mma8x5x_dev) {
        LOG_ERR("Failed to get device binding for MMA8x5x");
        return;
    }

    init_nvs();
    read_and_store_sensor_data();
}