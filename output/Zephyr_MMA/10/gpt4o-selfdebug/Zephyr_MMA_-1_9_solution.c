#include <zephyr/storage/flash_map.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>

#define MMA8x5x_ADDR  0x1D  // Replace with correct I2C address for the sensor
#define NVS_DATA_ID   1     // NVS data ID

static const struct device *i2c_dev;
static const struct device *sensor_dev;

void main(void) {
    struct sensor_value accel[3];
    struct flash_area *fa;
    int rc;

    printk("Initializing I2C and sensor\n");
    i2c_dev = device_get_binding(DT_LABEL(DT_NODELABEL(i2c0)));
    sensor_dev = device_get_binding(DT_LABEL(DT_NODELABEL(mma8x5x)));

    if (!i2c_dev || !sensor_dev) {
        printk("Failed to get I2C or sensor device\n");
        return;
    }

    printk("I2C and sensor devices are initialized\n");

    k_sleep(K_SECONDS(5));

    while (1) {
        rc = sensor_sample_fetch(sensor_dev);
        if (rc == 0) {
            rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
            printk("Accelerometer: X=%d, Y=%d, Z=%d\n", accel[0].val1, accel[1].val1, accel[2].val1);
        }

        if (rc != 0) {
            printk("Failed to fetch sensor data\n");
            continue;
        }

        rc = flash_area_open(NVS_DATA_ID, &fa);
        if (rc == 0) {
            flash_area_write(fa, 0, &accel, sizeof(accel));
            flash_area_close(fa);
            printk("Saved values to NVS\n");
        } else {
            printk("Failed to open NVS area\n");
        }

        k_sleep(K_SECONDS(5));
    }
}