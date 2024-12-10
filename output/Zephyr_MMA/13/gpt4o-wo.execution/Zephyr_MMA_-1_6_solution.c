
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/flash.h>

LOG_MODULE_REGISTER(main);

#define STORAGE_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define STORAGE_PARTITION FIXED_PARTITION_DEVICE(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    int ret;
    struct flash_sector *sectors;
    uint32_t sector_count;

    sectors = flash_area_get_sectors(STORAGE_PARTITION_ID, &sector_count);
    if (!sectors) {
        LOG_ERR("Failed to get flash sectors");
        return;
    }

    fs.flash_device = STORAGE_PARTITION;
    fs.offset = sectors[0].fs_sector_start;
    fs.sector_size = sectors[0].fs_sector_size;
    fs.sector_count = sector_count;

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
    }
}

void read_sensor_and_store(void) {
    const struct device *dev = device_get_binding("MMA8x5x");
    if (!dev) {
        LOG_ERR("Failed to get MMA8x5x device");
        return;
    }

    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sensor sample: %d", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor channel: %d", ret);
        return;
    }

    uint8_t data[6];
    data[0] = (uint8_t)(accel[0].val1 >> 8);
    data[1] = (uint8_t)(accel[0].val1 & 0xFF);
    data[2] = (uint8_t)(accel[1].val1 >> 8);
    data[3] = (uint8_t)(accel[1].val1 & 0xFF);
    data[4] = (uint8_t)(accel[2].val1 >> 8);
    data[5] = (uint8_t)(accel[2].val1 & 0xFF);

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    } else {
        LOG_INF("Data written to NVS: %d bytes", ret);
    }
}

int main(void) {
    init_nvs();

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
