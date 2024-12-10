#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <drivers/i2c.h>
#include <drivers/nvs.h>
#include <sys/printk.h>
#include <storage/flash_map.h>

#define SLEEP_TIME_MS   5000
#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))
#define MMA8x5x_ADDR 0x1D

void main(void) {
    const struct device *i2c_dev;
    const struct device *sensor_dev;
    struct sensor_value accel[3];
    const struct device *flash_dev;
    struct nvs_fs nvs;
    int rc;
    
    i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("I2C: Device driver not found.\n");
        return;
    }
    
    sensor_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_fxos8700))); 
    if (!sensor_dev) {
        printk("Sensor: Device driver not found.\n");
        return;
    }

    flash_dev = device_get_binding(FLASH_AREA_LABEL(storage));
    if (!flash_dev) {
        printk("Flash: Device driver not found.\n");
        return;
    }
    
    nvs.flash_device = flash_dev;
    nvs.offset = FLASH_AREA_OFFSET(storage);
    nvs.sector_size = 4096;
    nvs.sector_count = 3;

    rc = nvs_init(&nvs, flash_dev);
    if (rc) {
        printk("NVS init failed\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(sensor_dev);
        if (rc == 0) {
            rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
            if (rc == 0) {
                printk("Accel X:%d, Y:%d, Z:%d\n", accel[0].val1, accel[1].val1, accel[2].val1);

                /* Store the reading in NVS */
                rc = nvs_write(&nvs, 1, &accel, sizeof(accel));
                if (rc < 0) {
                    printk("NVS write error\n");
                } else {
                    printk("Data stored in NVS\n");
                }
            }
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}