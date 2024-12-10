
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_PARTITION DT_FLASH_AREA_LABEL(storage_partition)

static struct nvs_fs fs;

void init_nvs(void)
{
    int err;

    fs.flash_device = device_get_binding(DT_LABEL(STORAGE_PARTITION));
    if (!fs.flash_device) {
        printk("Error: no flash device\n");
        return;
    }

    fs.offset = DT_REG_ADDR(STORAGE_PARTITION);
    fs.sector_size = DT_PROP(STORAGE_PARTITION, erase_block_size);
    fs.sector_count = DT_REG_SIZE(STORAGE_PARTITION) / fs.sector_size;

    err = nvs_init(&fs, DT_LABEL(STORAGE_PARTITION));
    if (err) {
        printk("Error initializing NVS (%d)\n", err);
        return;
    }
}

void read_and_store_sensor_data(const struct device *dev)
{
    struct sensor_value accel[3];
    int err;

    err = sensor_sample_fetch(dev);
    if (err) {
        printk("Sensor sample update error: %d\n", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        printk("Sensor channel get error: %d\n", err);
        return;
    }

    printk("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the sensor data in NVS
    uint16_t id = 1;
    err = nvs_write(&fs, id, accel, sizeof(accel));
    if (err < 0) {
        printk("Error writing to NVS (%d)\n", err);
    } else {
        printk("Data stored in NVS\n");
    }
}

int main(void)
{
    const struct device *dev = device_get_binding(DT_LABEL(MMA8X5X_NODE));
    if (!dev) {
        printk("Error: no device MMA8X5X\n");
        return -1;
    }

    init_nvs();

    while (1) {
        read_and_store_sensor_data(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
