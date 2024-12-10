
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define NVS_PARTITION_LABEL "storage"
#define SAMPLE_INTERVAL_SECONDS 5

static struct nvs_fs nvs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .flash_device = FIXED_PARTITION_DEVICE(storage),
    .offset = FIXED_PARTITION_OFFSET(storage),
};

void read_mma8x5x(const struct device *dev, struct sensor_value *accel) {
    if (sensor_sample_fetch(dev) < 0) {
        printk("Failed to fetch sample from MMA8x5x\n");
        return;
    }
    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
}

void store_value_in_nvs(struct sensor_value *accel) {
    int ret;
    uint8_t buffer[sizeof(struct sensor_value) * 3];
    memcpy(buffer, accel, sizeof(struct sensor_value) * 3);

    ret = nvs_write(&nvs, 0, buffer, sizeof(buffer));
    if (ret < 0) {
        printk("Failed to write to NVS: %d\n", ret);
    } else {
        printk("Value stored in NVS\n");
    }
}

int main(void) {
    const struct device *mma8x5x_dev = device_get_binding("MMA8x5x");
    if (!mma8x5x_dev) {
        printk("MMA8x5x device not found\n");
        return -1;
    }

    int ret = nvs_mount(&nvs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return -1;
    }

    struct sensor_value accel[3];

    while (1) {
        read_mma8x5x(mma8x5x_dev, accel);
        store_value_in_nvs(accel);
        k_sleep(K_SECONDS(SAMPLE_INTERVAL_SECONDS));
    }

    return 0;
}
