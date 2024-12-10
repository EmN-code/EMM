
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

#if !DT_NODE_HAS_STATUS(MMA8X5X_NODE, okay)
#error "MMA8X5X device not found in the device tree"
#endif

#if !DT_NODE_HAS_STATUS(NVS_PARTITION, okay)
#error "NVS partition not found in the device tree"
#endif

static struct nvs_fs fs;

void read_sensor_and_store(const struct device *dev)
{
    struct sensor_value accel[3];
    int rc;

    if (sensor_sample_fetch(dev) < 0) {
        printk("Sensor sample update failed\n");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        printk("Failed to get sensor data\n");
        return;
    }

    printk("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the sensor data in NVS
    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    rc = nvs_write(&fs, 1, data, sizeof(data));
    if (rc < 0) {
        printk("NVS write failed: %d\n", rc);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("MMA8X5X device not ready\n");
        return -ENODEV;
    }

    // Initialize NVS
    fs.flash_device = DEVICE_DT_GET(NVS_PARTITION);
    if (!device_is_ready(fs.flash_device)) {
        printk("Flash device for NVS is not ready\n");
        return -ENODEV;
    }

    fs.offset = DT_REG_ADDR(NVS_PARTITION);
    fs.sector_size = DT_REG_SIZE(NVS_PARTITION);
    fs.id_size = 1;

    int rc = nvs_init(&fs, DT_LABEL(NVS_PARTITION));
    if (rc) {
        printk("NVS init failed: %d\n", rc);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
