
#include <zephyr/sys/device_mmio.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/logging/log.h>

#define SENSOR_NODE DT_N_S_soc_S_i2c_3ff53000_ORD
#define STORAGE_PARTITION DT_N_NODELABEL_storage_partition

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = FLASH_AREA_ID(STORAGE_PARTITION);
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    int err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("NVS init failed: %d", err);
    } else {
        LOG_INF("NVS init successful");
    }
}

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Sensor fetch failed");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Sensor channel get failed");
        return;
    }

    uint8_t data[6];
    data[0] = (uint8_t)(accel[0].val1 >> 8);
    data[1] = (uint8_t)(accel[0].val1 & 0xFF);
    data[2] = (uint8_t)(accel[1].val1 >> 8);
    data[3] = (uint8_t)(accel[1].val1 & 0xFF);
    data[4] = (uint8_t)(accel[2].val1 >> 8);
    data[5] = (uint8_t)(accel[2].val1 & 0xFF);

    ssize_t written = nvs_write(&fs, 1, data, sizeof(data));
    if (written < 0) {
        LOG_ERR("NVS write failed: %d", (int)written);
    } else {
        LOG_INF("NVS write successful: %d bytes", (int)written);
    }
}

int main(void) {
    const struct device *dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
