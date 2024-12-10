
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_ALIAS(mma8x5x)
#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID

static struct nvs_fs fs;

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int rc;

    if (!device_is_ready(dev)) {
        LOG_ERR("MMA8x5x device not ready");
        return;
    }

    rc = sensor_sample_fetch(dev);
    if (rc) {
        LOG_ERR("Failed to fetch sample (%d)", rc);
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        LOG_ERR("Failed to get sensor data (%d)", rc);
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the reading value persistently in NVS
    uint8_t data[12];
    memcpy(data, &accel, sizeof(accel));

    rc = nvs_write(&fs, 1, data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS (%d)", rc);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_NODE);
    int rc;

    // Initialize NVS
    fs.sector_size = DT_PROP(NVS_PARTITION, erase_block_size);
    fs.sector_count = DT_PROP(NVS_PARTITION, size) / fs.sector_size;
    fs.offset = DT_REG_ADDR(NVS_PARTITION);

    rc = nvs_init(&fs, DT_LABEL(NVS_PARTITION));
    if (rc) {
        LOG_ERR("Failed to initialize NVS (%d)", rc);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
