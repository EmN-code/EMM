
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    struct flash_sector_param sector[1];

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION_ID);
    sector[0].fs_size = 4096; // Example size, adjust as needed
    sector[0].fs_sector_size = 4096; // Example size, adjust as needed
    fs.sector_count = 1;
    fs.sector_size = sector[0].fs_sector_size;

    int ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
    } else {
        LOG_INF("NVS init successful");
    }
}

void store_reading(int16_t reading) {
    int ret = nvs_write(&fs, 0, &reading, sizeof(reading));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("NVS write successful");
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device MMA8X5X not ready");
        return -ENODEV;
    }

    init_nvs();

    while (1) {
        struct sensor_value accel[3];
        int ret = sensor_sample_fetch(dev);
        if (ret) {
            LOG_ERR("Sensor sample fetch failed: %d", ret);
            continue;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret) {
            LOG_ERR("Sensor channel get failed: %d", ret);
            continue;
        }

        int16_t reading = (int16_t)(accel[0].val1); // Example: using x-axis value
        store_reading(reading);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
