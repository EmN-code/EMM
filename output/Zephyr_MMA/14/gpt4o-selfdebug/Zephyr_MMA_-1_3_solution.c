#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define STORAGE_PARTITION FLASH_AREA_ID(storage)
#define SENSOR_SAMPLE_INTERVAL K_SECONDS(5)

void mma8x5x_read_and_store(const struct device *dev, struct nvs_fs *nvs) {
    struct sensor_value accel[3];
    int rc = sensor_sample_fetch(dev);
    
    if (rc == 0) {
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        printk("Acceleration X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
                accel[0].val1, accel[0].val2,
                accel[1].val1, accel[1].val2,
                accel[2].val1, accel[2].val2);

        // Storing X acceleration value in NVS
        rc = nvs_write(nvs, 1, &accel[0].val1, sizeof(accel[0].val1));
        if (rc < 0) {
            printk("Error writing to NVS\n");
        }
    } else {
        printk("Error fetching sample\n");
    }
}

void main(void) {
    const struct device *mma8x5x_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652)));
    if (!mma8x5x_dev) {
        printk("No device found\n");
        return;
    }

    struct nvs_fs nvs;
    struct flash_area *fa;
 
    if (flash_area_open(STORAGE_PARTITION, &fa) < 0) {
        printk("Error opening flash area\n");
        return;
    }
    nvs.offset = fa->fa_off;
    nvs.sector_size = fa->fa_size;
    nvs.sector_count = 3;
    if (nvs_mount(&nvs) < 0) {
        printk("Error mounting NVS\n");
        return;
    }

    while (1) {
        mma8x5x_read_and_store(mma8x5x_dev, &nvs);
        k_sleep(SENSOR_SAMPLE_INTERVAL);
    }
}