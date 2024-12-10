#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/i2c.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(mma8x5x_nvs, LOG_LEVEL_INF);

#define SLEEP_TIME_MS   5000
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 3

static struct nvs_fs fs;

void main(void) {
    struct device *dev;
    struct sensor_value accel[3];
    int ret;

    /* Configure and mount NVS */
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    ret = nvs_init(&fs, DT_LABEL(DT_CHOSEN(zephyr_flash_controller)));
    if (ret) {
        LOG_ERR("NVS initialization failed");
        return;
    }

    /* Get a device structure for the MMA8x5x accelerometer */
    dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652)));
    if (!dev) {
        LOG_ERR("MMA8x5x device not found");
        return;
    }

    while (1) {
        /* Fetch the accelerometer data */
        ret = sensor_sample_fetch(dev);
        if (ret) {
            LOG_ERR("Sensor sample update error");
            return;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret < 0) {
            LOG_ERR("Cannot read sensor channels");
            return;
        }

        LOG_INF("Acceleration X: %d, Y: %d, Z: %d", accel[0].val1, accel[1].val1, accel[2].val1);

        /* Store acceleration in NVS */
        ret = nvs_write(&fs, 1, &accel, sizeof(accel));
        if (ret < 0) {
            LOG_ERR("Failed to write to NVS");
        }

        k_msleep(SLEEP_TIME_MS);
    }
}