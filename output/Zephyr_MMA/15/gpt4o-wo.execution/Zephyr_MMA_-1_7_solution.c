
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION DT_FLASH_AREA_LABEL(storage)

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector *sectors;
    uint32_t sector_count;

    sectors = flash_area_get_sectors(DT_FLASH_AREA_ID(storage), &sector_count);
    if (!sectors) {
        LOG_ERR("Failed to get flash sectors");
        return;
    }

    fs.flash_device = (const struct device *)device_get_binding(DT_LABEL(NVS_PARTITION));
    if (!fs.flash_device) {
        LOG_ERR("Failed to get flash device");
        return;
    }

    fs.offset = sectors[0].fs_offset;
    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = sector_count;

    err = nvs_mount(&fs);
    if (err) {
        LOG_ERR("Failed to mount NVS (err %d)", err);
    }
}

void read_mma8x5x(void) {
    const struct device *dev = device_get_binding(DT_LABEL(MMA8X5X_NODE));
    if (!dev) {
        LOG_ERR("Failed to get MMA8X5X device");
        return;
    }

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

    printk("Accel: X=%d.%06d Y=%d.%06d Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the reading value persistently in NVS
    uint16_t id = 1;
    err = nvs_write(&fs, id, &accel, sizeof(accel));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", err);
    } else {
        LOG_INF("NVS write successful");
    }
}

int main(void) {
    init_nvs();

    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
