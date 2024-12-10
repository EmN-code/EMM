// This code demonstrates reading sensor data from an MMA8x5x accelerometer using I2C every 5 seconds. The data is then stored using the Non-Volatile Storage (NVS) on an ESP32 using Zephyr OS.

#include <zephyr.h>
#include <sys/printk.h>
#include <errno.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/i2c.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>

#define SENSOR_NODE DT_NODELABEL(mma8652)
#define I2C_NODE DT_BUS(SENSOR_NODE)

static const struct device *sensor_dev;
static struct nvs_fs my_nvs;

// Thread properties
#define STACKSIZE 1024
#define PRIORITY 7

void read_sensor(struct k_work *work) {
    struct sensor_value accel[3];
    int rc = sensor_sample_fetch(sensor_dev);
    if (rc == 0) {
        rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc == 0) {
            printk("Acceleration X: %d.%06d m/s^2, Y: %d.%06d m/s^2, Z: %d.%06d m/s^2\n", 
                   accel[0].val1, accel[0].val2, accel[1].val1, accel[1].val2, accel[2].val1, accel[2].val2);

            // Store data in NVS
            rc = nvs_write(&my_nvs, 1, &accel, sizeof(accel));
            if (rc < 0) {
                printk("Failed to store data in NVS, error: %d\n", rc);
            }
        }
    }
}

void main(void) {
    printk("Starting accelerometer example\n");
    int rc;

    // Initialize the sensor
    sensor_dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!sensor_dev) {
        printk("Could not get %s device\n", DT_LABEL(SENSOR_NODE));
        return;
    }

    // Initialize NVS
    struct flash_pages_info info;
    const struct device *flash_dev = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);

    if (!device_is_ready(flash_dev)) {
        printk("Flash device %s is not ready\n", flash_dev->name);
        return;
    }

    rc = flash_get_page_info_by_offs(flash_dev, FLASH_AREA_OFFSET(storage), &info);
    if (rc) {
        printk("Unable to get flash area for storage\n");
        return;
    }

    my_nvs.offset = FLASH_AREA_OFFSET(storage);
    my_nvs.sector_size = info.size;
    my_nvs.sector_count = 3U;
    my_nvs.flash_device = flash_dev;

    rc = nvs_init(&my_nvs, my_nvs.flash_device->bbox->sectors[0].fs->base_addr);
    if (rc) {
        printk("Flash Init failed\n");
        return;
    }

    // Schedule a periodic work
    struct k_work_delayable sensor_work;
    k_work_init_delayable(&sensor_work, read_sensor);

    // Start sensor reading every 5 seconds
    while (1) {
        k_work_schedule(&sensor_work, K_SECONDS(5));
        k_sleep(K_SECONDS(5));
    }
}