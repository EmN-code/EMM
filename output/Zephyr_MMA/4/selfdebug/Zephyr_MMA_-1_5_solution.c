
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8x5x_I2C_ADDR DT_REG_ADDR(DT_NODELABEL(mma8x5x))
#define NVS_PARTITION DT_FLASH_AREA_LABEL_BY_NAME(storage_partition)

#define SLEEP_TIME_MS 5000

const struct device *i2c_dev;
const struct device *nvs_dev;

void read_sensor_and_store(void)
{
    struct sensor_value accel[3];
    int ret;

    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(mma8x5x));
    if (!device_is_ready(dev)) {
        printk("Sensor device not ready\n");
        return;
    }

    ret = sensor_sample_fetch(dev);
    if (ret) {
        printk("Sensor sample fetch failed: %d\n", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Sensor channel get failed: %d\n", ret);
        return;
    }

    printk("Accel: X=%f Y=%f Z=%f\n", sensor_value_to_double(&accel[0]),
           sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));

    // Store the read value in NVS
    struct nvs_fs fs;
    fs.flash_device = nvs_dev;
    fs.offset = DT_REG_ADDR(NVS_PARTITION);
    fs.sector_size = DT_PROP(NVS_PARTITION, erase_block_size);
    fs.sector_count = DT_PROP(NVS_PARTITION, size) / fs.sector_size;

    ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return;
    }

    uint16_t id = 1; // Example ID for the stored value
    ret = nvs_write(&fs, id, &accel, sizeof(accel));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void)
{
    i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return -1;
    }

    nvs_dev = DEVICE_DT_GET(NVS_PARTITION);
    if (!device_is_ready(nvs_dev)) {
        printk("NVS device not ready\n");
        return -1;
    }

    while (1) {
        read_sensor_and_store();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
