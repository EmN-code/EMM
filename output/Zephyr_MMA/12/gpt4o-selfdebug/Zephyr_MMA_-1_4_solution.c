#include <zephyr/types.h>
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <drivers/gpio.h>
#include <fs/nvs.h>
#include <random/rand32.h>

#define ACCEL_SENSOR DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define STORAGE_ID 1
#define STORAGE_KEY 0
#define DELAY K_SECONDS(5)

void main(void) {
    const struct device *dev = device_get_binding(ACCEL_SENSOR);
    if (!dev) {
        printk("No device found\n");
        return;
    }

    struct sensor_value accel[3];
    struct nvs_fs fs;
    struct flash_pages_info info;
    int rc;

    /* NVS setup */
    fs.offset = FLASH_AREA_OFFSET(storage);
    rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), fs.offset, &info);
    if (rc) {
        printk("Storage device setup failed\n");
        return;
    }
    fs.sector_size = info.size;
    fs.sector_count = 3U;
    rc = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (rc) {
        printk("NVS initialization failed: %d\n", rc);
        return;
    }

    /* Main loop */
    while (1) {
        sensor_sample_fetch(dev);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        printk("Accel X: %f, Y: %f, Z: %f\n", 
               sensor_value_to_double(&accel[0]), 
               sensor_value_to_double(&accel[1]),
               sensor_value_to_double(&accel[2]));

        /* Store value */
        rc = nvs_write(&fs, STORAGE_KEY, &accel, sizeof(accel));
        if (rc) {
            printk("NVS write error: %d\n", rc);
        }

        k_sleep(DELAY);
    }
}