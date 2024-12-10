
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8x5x_NODE DT_ALIAS(mma8x5x)
#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID

#if !DT_NODE_HAS_STATUS(MMA8x5x_NODE, okay)
#error "MMA8x5x device is not enabled in the device tree"
#endif

#if !DT_NODE_HAS_STATUS(DT_NODELABEL(storage_partition), okay)
#error "NVS partition is not enabled in the device tree"
#endif

static struct nvs_fs fs;

void read_mma8x5x(const struct device *dev)
{
    struct sensor_value accel[3];
    int ret;

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

    printk("X: %d.%06d m/s^2, Y: %d.%06d m/s^2, Z: %d.%06d m/s^2\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the reading value persistently in NVS
    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));
    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void)
{
    const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8x5x_NODE);
    if (!device_is_ready(mma8x5x_dev)) {
        printk("MMA8x5x device is not ready\n");
        return -1;
    }

    // Initialize NVS
    fs.flash_device = FLASH_AREA_ID(NVS_PARTITION);
    fs.offset = 0;
    fs.sector_size = 4096; // Adjust according to your NVS partition size
    fs.id_size = 2;

    int ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return -1;
    }

    while (1) {
        read_mma8x5x(mma8x5x_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
