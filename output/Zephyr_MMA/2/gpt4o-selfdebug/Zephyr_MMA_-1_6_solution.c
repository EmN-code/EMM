#include <zephyr.h>
#include <device.h>
#include <drivers/spi.h>
#include <storage/flash_map.h>
#include <drivers/sensor.h>
#include <drivers/i2c.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <sys/util.h>
#include <console/console.h>
#include <sys/reboot.h>

#define SLEEP_TIME_MS   5000
#define I2C_DEV DT_LABEL(DT_ALIAS(i2c_0))

const struct device *i2c_dev;
const struct device *dev_sensor;
struct nvs_fs fs;
int rc;

void setup_nvs(void) {
    struct flash_pages_info info;

    fs.offset = FLASH_AREA_OFFSET(storage);

    rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), fs.offset, &info);
    if (rc) {
        printk("Failed to get page info: %d\n", rc);
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = 3U;

    rc = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (rc) {
        printk("NVS init failed: %d\n", rc);
        return;
    }
}

void read_mma8x5x(void) {
    struct sensor_value accel[3];

    if (sensor_sample_fetch(dev_sensor) < 0) {
        printk("Failed to fetch sensor sample\n");
        return;
    }
    if (sensor_channel_get(dev_sensor, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        printk("Cannot read sensor channels\n");
        return;
    }

    printk("Accel X: %d.%, Accel Y: %d.%, Accel Z: %d.%\n", 
           accel[0].val1, accel[0].val2, 
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    /* Write sensor data to NVS */
    rc = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (rc < 0) {
        printk("Failed to write sensor data to NVS: %d\n", rc);
    }
}

void main(void) {
    printk("Starting application\n");
    setup_nvs();

    i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("I2C: Device not found.\n");
        return;
    }

    dev_sensor = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));
    if (!dev_sensor) {
        printk("Could not get MMA8x5x device\n");
        return;
    }

    while (1) {
        read_mma8x5x();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}