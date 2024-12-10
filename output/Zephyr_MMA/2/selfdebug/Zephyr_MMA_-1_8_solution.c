
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(main);

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    int ret;
    struct flash_pages_info info;

    fs.flash_device = NVS_PARTITION_ID;
    fs.offset = 0;
    ret = flash_get_page_info_by_offs(device_get_binding(fs.flash_device), fs.offset, &info);
    if (ret) {
        LOG_ERR("Unable to get page info: %d", ret);
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = 3; // Adjust based on your needs

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
    }
}

void main(void) {
    const struct device *dev;
    struct sensor_value accel[3];
    int ret;
    uint16_t id = 1; // NVS ID for storing the sensor value

    init_nvs();

    dev = device_get_binding("MMA8x5x");
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return;
    }

    while (1) {
        ret = sensor_sample_fetch(dev);
        if (ret) {
            LOG_ERR("Sensor sample fetch failed: %d", ret);
            k_sleep(K_MSEC(SLEEP_TIME_MS));
            continue;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret) {
            LOG_ERR("Sensor channel get failed: %d", ret);
            k_sleep(K_MSEC(SLEEP_TIME_MS));
            continue;
        }

        LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
                accel[0].val1, accel[0].val2,
                accel[1].val1, accel[1].val2,
                accel[2].val1, accel[2].val2);

        // Store the X-axis value in NVS
        ret = nvs_write(&fs, id, &accel[0].val1, sizeof(accel[0].val1));
        if (ret < 0) {
            LOG_ERR("NVS write failed: %d", ret);
        } else {
            LOG_INF("NVS write successful");
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}
