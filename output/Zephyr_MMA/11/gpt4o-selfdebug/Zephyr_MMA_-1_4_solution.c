#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/settings/settings.h>
#include <zephyr/storage/nvs/nvs.h>
#include <zephyr/kernel.h>

#define STORAGE_MAGIC 0x55AA
#define SENSOR_READ_INTERVAL K_SECONDS(5)
#define NVS_ID 1

void main(void) {
    const struct device *mma = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!mma) {
        printk("Failed to get device binding\n");
        return;
    }

    struct nvs_fs fs;
    struct flash_pages_info info;
    fs.offset = FLASH_AREA_OFFSET(storage);
    int rc = flash_get_page_info_by_offs(device_get_binding(FLASH_CTRL_NAME), fs.offset, &info);
    if (rc) {
        printk("Failed to get page layout\n");
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = FLASH_AREA_SIZE(storage) / fs.sector_size;

    rc = nvs_init(&fs, FLASH_CTRL_NAME);
    if (rc) {
        printk("NVS initialization failed\n");
        return;
    }

    while (1) {
        struct sensor_value accel;
        sensor_sample_fetch_chan(mma, SENSOR_CHAN_ACCEL_XYZ);
        sensor_channel_get(mma, SENSOR_CHAN_ACCEL_X, &accel);
        printk("Acceleration: %d\n", accel.val1);

        rc = nvs_write(&fs, NVS_ID, &accel, sizeof(accel));
        if (rc < 0) {
            printk("Failed to write Acceleration value\n");
        }

        k_sleep(SENSOR_READ_INTERVAL);
    }
}