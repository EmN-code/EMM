
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION FIXED_PARTITION_ID(DT_COMPAT_fixed_partitions_LABEL_4_PARENT_PARENT)

#define NVS_STORAGE_NAME "mma8x5x_data"
#define NVS_STORAGE_SIZE 32

static struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = NVS_PARTITION;
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    int ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
    }
}

void store_value(int32_t value) {
    int ret = nvs_write(&fs, 0, &value, sizeof(value));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("MMA8X5X device not ready\n");
        return -1;
    }

    init_nvs();

    while (1) {
        struct sensor_value accel[3];
        int ret = sensor_sample_fetch(dev);
        if (ret) {
            printk("Sensor fetch failed: %d\n", ret);
            continue;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret) {
            printk("Sensor channel get failed: %d\n", ret);
            continue;
        }

        int32_t value = accel[0].val1; // Store the x-axis value
        store_value(value);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
