#include <zephyr/kernel.h>
#include <zephyr/nvs/nvs.h>
#include <device.h>
#include <devicetree.h>

#define STORAGE_PARTITION_LABEL storage_partition
#define STORE_ID 1
#define SENSOR_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8451))

const struct device *sensor_dev;
struct nvs_fs my_fs;

void mma8x5x_read_and_store(void)
{
    int rc;
    struct sensor_value accel[3];

    rc = sensor_sample_fetch(sensor_dev);
    if (rc == 0) {
        rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc == 0) {
            printk("Acceleration (m/s^2): X=%d.%d, Y=%d.%d, Z=%d.%d\n",
                    accel[0].val1, accel[0].val2,
                    accel[1].val1, accel[1].val2,
                    accel[2].val1, accel[2].val2);

            int32_t x = accel[0].val1, y = accel[1].val1, z = accel[2].val1;
            rc = nvs_write(&my_fs, STORE_ID, &x, sizeof(x));
            if (rc < 0) {
                printk("Failed to store X acceleration\n");
            }
            rc = nvs_write(&my_fs, STORE_ID + 1, &y, sizeof(y));
            if (rc < 0) {
                printk("Failed to store Y acceleration\n");
            }
            rc = nvs_write(&my_fs, STORE_ID + 2, &z, sizeof(z));
            if (rc < 0) {
                printk("Failed to store Z acceleration\n");
            }
        }
    }
}

void main(void)
{
    sensor_dev = device_get_binding(SENSOR_DEV_NAME);
    if (!sensor_dev) {
        printk("Could not get %s device\n", SENSOR_DEV_NAME);
        return;
    }

    struct flash_pages_info info;

    my_fs.offset = DT_REG_ADDR(DT_NODELABEL(STORAGE_PARTITION_LABEL));
    flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), my_fs.offset, &info);
    my_fs.sector_size = info.size;
    my_fs.sector_count = 3U;

    if (nvs_init(&my_fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL)) {
        printk("Cannot init NVS\n");
        return;
    }

    while (1) {
        mma8x5x_read_and_store();
        k_sleep(K_SECONDS(5));
    }
}
