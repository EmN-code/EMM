#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>

#define SLEEP_TIME_MS 5000
#define SENSOR_NODE DT_LABEL(DT_INST(0, nxp_mma8451))

const struct device *mma8x5x_dev;
struct nvs_fs nvs;

void main(void) {
    int err;
    struct sensor_value accel_x, accel_y, accel_z;
    struct flash_area const *fa;
    
    printk("Starting MMA8x5x example\n");

    mma8x5x_dev = device_get_binding(SENSOR_NODE);
    if (!mma8x5x_dev) {
        printk("Failed to get device binding\n");
        return;
    }

    err = flash_area_open(FLASH_AREA_ID(storage), &fa);
    if (err < 0) {
        printk("Flash area open error\n");
        return;
    }

    nvs.offset = fa->fa_size / 2;
    err = nvs_init(&nvs, flash_area_dev(fa));
    if (err) {
        printk("NVS initialization failed\n");
        return;
    }

    while (1) {
        err = sensor_sample_fetch(mma8x5x_dev);
        if (err) {
            printk("Failed to fetch sample\n");
            continue;
        }

        err = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_X, &accel_x);
        err += sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_Y, &accel_y);
        err += sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_Z, &accel_z);

        if (err) {
            printk("Failed to get sensor data\n");
            continue;
        }

        printk("Accel X: %d.%06d; Y: %d.%06d; Z: %d.%06d\n",
               accel_x.val1, accel_x.val2,
               accel_y.val1, accel_y.val2,
               accel_z.val1, accel_z.val2);

        /* Save values to NVS */
        nvs_write(&nvs, 1, &accel_x, sizeof(accel_x));
        nvs_write(&nvs, 2, &accel_y, sizeof(accel_y));
        nvs_write(&nvs, 3, &accel_z, sizeof(accel_z));

        k_msleep(SLEEP_TIME_MS);
    }
}