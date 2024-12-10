#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>

#define STORAGE_ID 1

void main(void) {
    const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));
    struct sensor_value accel[3];
    struct nvs_fs fs;
    struct flash_pages_info info;

    if (!device_is_ready(dev)) {
        printk("Device not ready.\n");
        return;
    }

    fs.offset = FLASH_AREA_OFFSET(storage);
    flash_get_page_info_by_offs(device_get_binding(FLASH_AREA_LABEL(storage)),
                                fs.offset, &info);

    fs.sector_size = info.size;
    fs.sector_count = 3U;

    nvs_init(&fs, FLASH_AREA_ID(storage));

    while (1) {
        sensor_sample_fetch(dev);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);

        printk("Accel (m/s²): X=%f, Y=%f, Z=%f\n",
               sensor_value_to_double(&accel[0]),
               sensor_value_to_double(&accel[1]),
               sensor_value_to_double(&accel[2]));

        int rc = nvs_write(&fs, STORAGE_ID, accel, sizeof(accel));
        if (rc < 0) {
            printk("Cannot write to flash, error %d\n", rc);
        }

        k_sleep(K_SECONDS(5));
    }
}