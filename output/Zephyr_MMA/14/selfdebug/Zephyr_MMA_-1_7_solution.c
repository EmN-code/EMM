
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define FLASH_NODE DT_NODELABEL(flash0)

#define NVS_PARTITION DT_LABEL(DT_CHOSEN(zephyr_flash_controller))
#define NVS_SECTOR_SIZE DT_PROP(DT_CHOSEN(zephyr_flash_controller), erase_block_size)
#define NVS_SECTOR_COUNT 4

struct nvs_fs fs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_SECTOR_COUNT,
    .offset = 0,
};

void read_sensor_and_store(const struct device *dev)
{
    struct sensor_value accel[3];
    int rc;

    rc = sensor_sample_fetch(dev);
    if (rc) {
        printk("Sensor fetch failed (err %d)\n", rc);
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        printk("Sensor get failed (err %d)\n", rc);
        return;
    }

    printk("Accel: x=%d.%06d, y=%d.%06d, z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    uint8_t data[sizeof(accel)];
    memcpy(data, accel, sizeof(accel));

    rc = nvs_write(&fs, 1, data, sizeof(data));
    if (rc < 0) {
        printk("NVS write failed (err %d)\n", rc);
    } else {
        printk("NVS write success\n");
    }
}

int main(void)
{
    const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(mma8x5x_dev)) {
        printk("MMA8X5X device not found\n");
        return -ENODEV;
    }

    const struct device *flash_dev = DEVICE_DT_GET(FLASH_NODE);
    if (!device_is_ready(flash_dev)) {
        printk("Flash device not found\n");
        return -ENODEV;
    }

    fs.flash_device = flash_dev;
    fs.offset = FLASH_AREA_OFFSET(storage);

    int rc = nvs_init(&fs, NVS_PARTITION);
    if (rc) {
        printk("NVS init failed (err %d)\n", rc);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(mma8x5x_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
