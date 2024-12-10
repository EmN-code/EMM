
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_PARTITION DT_NODELABEL(storage_partition)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(STORAGE_PARTITION)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(STORAGE_PARTITION)

#define SLEEP_TIME_MS 5000

static struct nvs_fs fs;

void init_nvs(void)
{
    int ret;

    fs.flash_device = NVS_PARTITION_DEVICE;
    fs.offset = FIXED_PARTITION_OFFSET(STORAGE_PARTITION);
    fs.sector_size = FIXED_PARTITION_SECTOR_SIZE(STORAGE_PARTITION);
    fs.sector_count = FIXED_PARTITION_SECTOR_COUNT(STORAGE_PARTITION);

    ret = nvs_init(&fs, NVS_PARTITION_ID);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
    } else {
        printk("NVS init successful\n");
    }
}

void read_and_store_sensor_data(const struct device *dev)
{
    struct sensor_value accel[3];
    int ret;
    uint8_t data;

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

    data = (uint8_t)(accel[0].val1); // Simplified storage of x-axis value

    ret = nvs_write(&fs, 1, &data, sizeof(data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful: %d\n", data);
    }
}

int main(void)
{
    const struct device *dev;

    dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("MMA8X5X device not ready\n");
        return 0;
    }

    init_nvs();

    while (1) {
        read_and_store_sensor_data(dev);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
