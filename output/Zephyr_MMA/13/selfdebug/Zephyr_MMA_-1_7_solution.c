
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_NODE DT_NODELABEL(nvs)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(NVS_NODE)

static struct nvs_fs fs;

void init_nvs(void) {
    int err;

    fs.flash_device = device_get_binding(DT_LABEL(NVS_NODE));
    if (!fs.flash_device) {
        LOG_ERR("Flash device %s not found", DT_LABEL(NVS_NODE));
        return;
    }

    fs.offset = FIXED_PARTITION_OFFSET(NVS_NODE);
    fs.sector_size = DT_PROP(NVS_NODE, erase_block_size);
    fs.sector_count = FIXED_PARTITION_SIZE(NVS_NODE) / fs.sector_size;

    err = nvs_init(&fs, DT_LABEL(NVS_NODE));
    if (err) {
        LOG_ERR("NVS init failed: %d", err);
    }
}

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int err;

    err = sensor_sample_fetch(dev);
    if (err) {
        LOG_ERR("Sensor sample fetch failed: %d", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        LOG_ERR("Sensor channel get failed: %d", err);
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the values in NVS
    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));
    err = nvs_write(&fs, 1, data, sizeof(data));
    if (err < 0) {
        LOG_ERR("NVS write failed: %d", err);
    }
}

int main(void) {
    const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(mma8x5x_dev)) {
        LOG_ERR("MMA8X5X device not ready");
        return -ENODEV;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(mma8x5x_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
