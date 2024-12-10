#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash.h>
#include <zephyr/fs/nvs.h>

#define STORAGE_NVS_ID 1
#define MMA_SENSOR_NODE DT_ALIAS(mma8x5x)
#define NVS_SECTOR_SIZE FLASH_AREA_SIZE(storage)
#define NVS_STORAGE_OFFSET FLASH_AREA_OFFSET(storage)

void main(void) {
    const struct device *mma_device = device_get_binding(DT_LABEL(MMA_SENSOR_NODE));
    struct nvs_fs fs;
    struct flash_pages_info info;
    sensor_value accel[3];
    int rc;

    if (!mma_device) {
        printk("MMA8x5x device not found\n");
        return;
    }

    fs.offset = NVS_STORAGE_OFFSET;
    rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), NVS_STORAGE_OFFSET, &info);
    if (rc) {
        printk("Flash page info error\n");
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = 3U;
    fs.flash_device = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);

    rc = nvs_init(&fs, DT_LABEL(DT_NODELABEL(storage)));
    if (rc) {
        printk("NVS init error\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(mma_device);
        if (rc) {
            printk("Sensor fetch error: %d\n", rc);
        } else {
            sensor_channel_get(mma_device, SENSOR_CHAN_ACCEL_XYZ, accel);
            printk("Accel X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);
            rc = nvs_write(&fs, STORAGE_NVS_ID, &accel[0], sizeof(accel));
            if (rc < 0) {
                printk("NVS write error: %d\n", rc);
            }
        }
        k_sleep(K_SECONDS(5));
    }
}