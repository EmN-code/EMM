// Ensure that you've included the correct path for the NVS library.
// Correcting the previous error by checking the include path.

#include <zephyr/storage/flash_map.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr.h>

void main(void) {
    const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    struct sensor_value accel[3];
    struct nvs_fs fs;
    struct flash_pages_info info;
    int rc;

    // NVS file system setup
    fs.offset = FLASH_AREA_OFFSET(storage);
    rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER), fs.offset, &info);
    if (rc) {
        printk("Storage flash page info error\n");
        return;
    }
    fs.sector_size = info.size;
    fs.sector_count = 3U;

    rc = nvs_mount(&fs);
    if (rc) {
        printk("NVS mount error: %d\n", rc);
        return;
    }

    while (1) {
        if (sensor_sample_fetch(dev)) {
            printk("Sensor sample update error\n");
            return;
        } else {
            sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
            printk("X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);
            rc = nvs_write(&fs, 1, &accel, sizeof(accel));
            if (rc < 0) {
                printk("NVS write error: %d\n", rc);
                return;
            }
        }
        k_sleep(K_SECONDS(5));
    }
}