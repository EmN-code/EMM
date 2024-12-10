
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8x5x_DEVICE_NODE DT_NODELABEL(i2c0)
#define MMA8x5x_LABEL DT_LABEL(MMA8x5x_DEVICE_NODE)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

static struct nvs_fs fs;

void init_nvs(void) {
    int ret;
    struct flash_pages_info info;

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION_ID);
    ret = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
    if (ret) {
        printk("Failed to get flash page info (%d)\n", ret);
        return;
    }
    fs.sector_size = info.size;
    fs.sector_count = 3; // Adjust based on your needs

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        printk("Failed to init NVS (%d)\n", ret);
        return;
    }
}

void read_mma8x5x(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        printk("Failed to fetch sample (%d)\n", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Failed to get sensor data (%d)\n", ret);
        return;
    }

    printk("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the read value persistently in NVS
    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        printk("Failed to write to NVS (%d)\n", ret);
    } else {
        printk("Data written to NVS\n");
    }
}

int main(void) {
    const struct device *dev;

    dev = device_get_binding(MMA8x5x_LABEL);
    if (dev == NULL) {
        printk("Could not get MMA8x5x device\n");
        return -1;
    }

    init_nvs();

    while (1) {
        read_mma8x5x(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
