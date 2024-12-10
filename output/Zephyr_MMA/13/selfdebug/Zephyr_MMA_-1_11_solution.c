
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define MMA8X5X_DEV_NAME DT_LABEL(MMA8X5X_NODE)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION FIXED_PARTITION_DEVICE(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    int ret;

    fs.flash_device = NVS_PARTITION;
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    ret = nvs_init(&fs, "storage");
    if (ret) {
        printk("NVS init failed: %d\n", ret);
    }
}

void store_value(int32_t value) {
    int ret;
    ret = nvs_write(&fs, 0, &value, sizeof(value));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    }
}

int32_t read_value(void) {
    int32_t value;
    int ret;
    ret = nvs_read(&fs, 0, &value, sizeof(value));
    if (ret < 0) {
        printk("NVS read failed: %d\n", ret);
        return -1;
    }
    return value;
}

int main(void) {
    const struct device *dev;
    struct sensor_value accel[3];
    int32_t stored_value;

    dev = device_get_binding(MMA8X5X_DEV_NAME);
    if (!dev) {
        printk("Could not get MMA8X5X device\n");
        return -1;
    }

    init_nvs();

    while (1) {
        if (sensor_sample_fetch(dev) < 0) {
            printk("Sensor sample update failed\n");
            continue;
        }

        if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
            printk("Could not get sensor data\n");
            continue;
        }

        printk("Accel: X=%f Y=%f Z=%f\n", sensor_value_to_double(&accel[0]),
               sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));

        stored_value = (int32_t)(sensor_value_to_double(&accel[0]) * 1000);
        store_value(stored_value);

        stored_value = read_value();
        printk("Stored value: %d\n", stored_value);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
