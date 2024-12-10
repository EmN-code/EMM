#include <zephyr/sys/reboot.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define STORAGE_NODE_LABEL storage

void mma845x_read_and_store(struct device *dev_nvs, struct device *accel)
{
    struct sensor_value accel_x, accel_y, accel_z;
    int rc, len;
    
    rc = sensor_sample_fetch(accel);
    if (rc) {
        printk("Failed to fetch sample from accelerometer: %d\n", rc);
        return;
    }

    sensor_channel_get(accel, SENSOR_CHAN_ACCEL_X, &accel_x);
    sensor_channel_get(accel, SENSOR_CHAN_ACCEL_Y, &accel_y);
    sensor_channel_get(accel, SENSOR_CHAN_ACCEL_Z, &accel_z);

    printk("Accelerometer Data: X=%d, Y=%d, Z=%d\n", accel_x.val1, accel_y.val1, accel_z.val1);

    struct nvs_fs fs;
    fs.flash_device = FLASH_AREA_DEVICE(STORAGE_NODE_LABEL);
    fs.offset = FLASH_AREA_OFFSET(STORAGE_NODE_LABEL);
    fs.sector_size = sizeof(int);
    fs.sector_count = 3U;

    rc = nvs_mount(&fs);
    if (rc) {
        printk("Flash init failed\n");
        return;
    }

    len = nvs_write(&fs, 1, &accel_x.val1, sizeof(accel_x.val1));
    if (len < 0) {
        printk("Writing to NVS failed: %d\n", rc);
    }
}

void main(void)
{
    struct device *accel = DEVICE_DT_GET_ANY(nxp_mma8451);
    struct device *dev_nvs;

    if (!device_is_ready(accel)) {
        printk("Accelerometer device not found\n");
        return;
    }

    if (!device_is_ready(dev_nvs)) {
        printk("Flash device not ready\n");
        return;
    }

    printk("Zephyr NVS Sample Program\n");

    while (1) {
        mma845x_read_and_store(dev_nvs, accel);
        k_sleep(K_SECONDS(5));
    }
}