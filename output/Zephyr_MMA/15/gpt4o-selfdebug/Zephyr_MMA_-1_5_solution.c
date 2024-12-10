#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/types.h>
#include <zephyr/data/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs()
{
    int err;

    struct flash_pages_info info;

    /* Define the NVS file system */
    fs.offset = DT_REG_ADDR(DT_NODELABEL(storage));
    err = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL),
                                      fs.offset, &info);
    if (err) {
        LOG_ERR("Failed to initialize flash at offset %zu (err: %d)", fs.offset, err);
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = 3U;

    err = nvs_mount(&fs);
    if (err) {
        LOG_ERR("Flash Init failed (err %d)", err);
    }
}

void read_and_store_sensor_data()
{
    const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));
    struct sensor_value accel[3];
    static uint32_t sensor_data_id;

    if (dev == NULL) {
        LOG_ERR("Could not get MMA8x5x device");
        return;
    }

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Sensor sample fetch error");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Sensor channel get error");
        return;
    }

    LOG_INF("X: %d.%06d; Y: %d.%06d; Z: %d.%06d", accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2, accel[2].val1, accel[2].val2);

    /* Store data in NVS */
    int rc = nvs_write(&fs, sensor_data_id++, accel, sizeof(accel));
    if (rc < 0) {
        LOG_ERR("Failed to write data to NVS");
    }
}

void main(void)
{
    int sleep_time_ms = 5000;
    init_nvs();

    while (1) {
        read_and_store_sensor_data();
        k_msleep(sleep_time_ms);
    }
}