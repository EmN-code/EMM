
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8X5X_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_SECTOR_SIZE 4096
#define NVS_NUM_SECTORS 4

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector sectors[NVS_NUM_SECTORS];

    fs.flash_device = device_get_binding(DT_CHOSEN(zephyr_flash_controller));
    if (!fs.flash_device) {
        LOG_ERR("Flash device not found");
        return;
    }

    fs.offset = FLASH_AREA_OFFSET(storage_partition);
    err = flash_get_sectors(fs.flash_device, &fs.sector_count, sectors);
    if (err) {
        LOG_ERR("Unable to get flash sectors");
        return;
    }

    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_NUM_SECTORS;

    err = nvs_init(&fs, DT_CHOSEN(zephyr_flash_controller));
    if (err) {
        LOG_ERR("NVS init failed");
        return;
    }

    LOG_INF("NVS initialized");
}

void read_mma8x5x(void) {
    const struct device *dev = device_get_binding(MMA8X5X_DEV_NAME);
    struct sensor_value accel[3];
    int err;

    if (!dev) {
        LOG_ERR("Device %s not found", MMA8X5X_DEV_NAME);
        return;
    }

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Failed to fetch sample from %s", MMA8X5X_DEV_NAME);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Failed to get data from %s", MMA8X5X_DEV_NAME);
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the read value persistently in NVS
    uint16_t id = 1;
    err = nvs_write(&fs, id, accel, sizeof(accel));
    if (err < 0) {
        LOG_ERR("NVS write failed: %d", err);
    } else {
        LOG_INF("NVS write successful");
    }
}

void main(void) {
    init_nvs();

    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }
}
