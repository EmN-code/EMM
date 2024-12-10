#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs.h>

#define I2C_DEV_LABEL DT_LABEL(DT_ALIAS(i2c0))
#define MMA8X5X_ADDRESS 0x1D
#define NVS_SECTOR_SIZE 0x1000
#define NVS_SECTOR_COUNT 3
#define NVS_FLASH_DEVICE FIXED_PARTITION_DEVICE(storage_partition)
#define SLEEP_TIME_SECONDS 5

static struct nvs_fs fs;

void mma8x5x_read(struct device *dev)
{
    struct sensor_value val;
    int err;

    err = sensor_sample_fetch(dev);
    if (err) {
        printk("Failed to fetch the sample (%d)\n", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &val);
    if (err) {
        printk("Failed to get the sample (%d)\n", err);
        return;
    }

    printk("Accel X: %d, Y: %d, Z: %d\n", val.val1, val.val2, val.val3);
    
    /* Store value in NVS */
    ssize_t bytes_written = nvs_write(&fs, 1, &val, sizeof(val));
    if (bytes_written > 0) {
        printk("Stored value successfully\n");
    } else {
        printk("Failed to store value (%d)\n", bytes_written);
    }
}

void main(void)
{
    struct device *mma8x5x_dev;
    struct flash_pages_info info;
    int err;

    /* Setting up NVS */
    fs.flash_device = NVS_FLASH_DEVICE;
    err = flash_get_page_info_by_offs(device_get_binding(NVS_FLASH_DEVICE),
                                      FLASH_AREA_OFFSET(storage), &info);
    if (err) {
        printk("Unable to get page info\n");
        return;
    }
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    fs.offset = FLASH_AREA_OFFSET(storage);

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        printk("Cannot init NVS (%d)\n", err);
        return;
    }

    /* Setting up sensor */
    mma8x5x_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!mma8x5x_dev) {
        printk("Cannot find MMA8x5x device!\n");
        return;
    }

    while (1) {
        mma8x5x_read(mma8x5x_dev);
        k_sleep(K_SECONDS(SLEEP_TIME_SECONDS));
    }
}