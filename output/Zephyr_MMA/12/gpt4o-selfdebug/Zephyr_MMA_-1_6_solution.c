#include <zephyr/types.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <zephyr.h>
#include <sys/printk.h>
#include <nvs/nvs.h>

#define STORAGE_PARTITION_LABEL        storage
#define STORAGE_PARTITION_SIZE         (DT_REG_SIZE(DT_CHOSEN(zephyr_qspi)))
#define NVS_SECTOR_SIZE                DT_PROP(DT_CHOSEN(zephyr_flash), erase_block_size)
#define NVS_SECTOR_COUNT               (STORAGE_PARTITION_SIZE / NVS_SECTOR_SIZE)

void main(void)
{
    const struct device *sensor;
    struct sensor_value accel_x;
    int ret;

    sensor = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma865x)));
    if (!sensor) {
        printk("No device found.\n");
        return;
    }

    struct nvs_fs fs;
    struct flash_sector sector;
    uint8_t buf[NVS_SECTOR_SIZE];

    /* Setup NVS */
    fs.offset = FLASH_AREA_OFFSET(STORAGE_PARTITION_LABEL);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    fs.flash_device = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);

    ret = nvs_init(&fs, buf);
    if (ret) {
        printk("NVS Init failed: %d\n", ret);
        return;
    }

    while (1) {
        ret = sensor_sample_fetch(sensor);
        if (ret) {
            printk("Sensor sample update error\n");
            continue;
        }

        ret = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &accel_x);
        if (ret) {
            printk("Cannot read sensor channel\n");
            continue;
        }

        /* Store value in NVS */
        ret = nvs_write(&fs, 1, &accel_x.val1, sizeof(accel_x.val1));
        if (ret >= 0) {
            printk("X-axis acc: %d stored successfully\n", accel_x.val1);
        } else {
            printk("Error writing to NVS: %d\n", ret);
        }

        k_sleep(K_SECONDS(5));
    }
}