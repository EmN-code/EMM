
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION FLASH_AREA_ID(storage)

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector *sectors;
    uint32_t sector_count;

    err = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN(zephyr_flash)),
                                      FLASH_AREA_OFFSET(storage), &sectors[0]);
    if (err) {
        LOG_ERR("Failed to get flash page info (err %d)", err);
        return;
    }

    sector_count = FLASH_AREA_SIZE(storage) / sectors[0].fs_size;

    fs.flash_device = device_get_binding(DT_CHOSEN(zephyr_flash));
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = sector_count;

    err = nvs_init(&fs, DT_CHOSEN(zephyr_flash));
    if (err) {
        LOG_ERR("Failed to init NVS (err %d)", err);
    }
}

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int err;

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Failed to fetch sample (err %d)", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Failed to get sensor data (err %d)", err);
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the sensor data in NVS
    uint16_t id = 1; // Example ID for the sensor data
    err = nvs_write(&fs, id, accel, sizeof(accel));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", err);
    } else {
        LOG_INF("Data written to NVS with ID %d", id);
    }
}

int main(void) {
    const struct device *dev;

    dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!dev) {
        LOG_ERR("Failed to get device binding");
        return -1;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
