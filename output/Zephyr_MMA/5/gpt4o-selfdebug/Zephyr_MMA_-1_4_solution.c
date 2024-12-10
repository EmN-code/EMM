#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs.h>

#define STORAGE_PARTITION_LABEL storage
#define SENSOR_INTERVAL K_SECONDS(5)

static struct nvs_fs fs;
static const struct device *mma8x5x;

void main(void) {
    int rc;
    struct sensor_value val;
    struct flash_sector sectors[CONFIG_FLASH_SECTOR_INFO_MAX];
    uint8_t read_buffer[64];
    off_t offset = 0;

    printk("Starting ESP32 MMA8x5x Sensor Reader\n");

    fs.offset = FLASH_AREA_OFFSET(STORAGE_PARTITION_LABEL);
    rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), 
                                     fs.offset, &sectors[0]);
    if (rc) {
        printk("Failed to get page info\n");
        return;
    }

    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = FLASH_AREA_SIZE(STORAGE_PARTITION_LABEL) / fs.sector_size;
    if (nvs_init(&fs, STORAGE_PARTITION_LABEL)) {
        printk("NVS init failed\n");
        return;
    }

    mma8x5x = DEVICE_DT_GET_ANY(mediatek_00_mma8x5x);
    if (!device_is_ready(mma8x5x)) {
        printk("Sensor not found\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(mma8x5x);
        if (rc) {
            printk("Failed to fetch sample\n");
            continue;
        }
        rc = sensor_channel_get(mma8x5x, SENSOR_CHAN_ACCEL_XYZ, &val);
        if (rc) {
            printk("Failed to get sensor data\n");
            continue;
        }
        printk("Sensor X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n", val.val1, val.val2);

        rc = nvs_write(&fs, offset, &val, sizeof(val));
        if (rc < 0) {
            printk("Failed to write sensor data to NVS\n");
        } else {
            printk("Sensor data saved\n");
        }

        offset += sizeof(val);
        k_sleep(SENSOR_INTERVAL);
    }
}