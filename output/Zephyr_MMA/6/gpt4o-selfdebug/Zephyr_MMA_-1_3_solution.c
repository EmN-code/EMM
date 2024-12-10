#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <sys/printk.h>

#define SENSOR_NODE DT_INST(0, nxp_mma8652)
#define STORAGE_PARTITION storage_partition

#if !DT_NODE_HAS_STATUS(SENSOR_NODE, okay)
#error "No sensor node found in the device tree"
#endif

#define NVS_SECTOR_SIZE DT_PROP(DT_NODELABEL(STORAGE_PARTITION), size)
#define NVS_SECTOR_COUNT 3
#define NVS_STORAGE_OFFSET FLASH_AREA_OFFSET(STORAGE_PARTITION)

static const struct device *sensor_dev;
static struct nvs_fs nvs;

void main(void) {
    int rc;
    struct sensor_value accel[3];

    printk("Starting MMA8x5x and NVS example...\n");

    sensor_dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!sensor_dev) {
        printk("No device found; failed to get MMA8x5x device binding\n");
        return;
    }

    nvs.sector_size = NVS_SECTOR_SIZE;
    nvs.sector_count = NVS_SECTOR_COUNT;
    nvs.offset = NVS_STORAGE_OFFSET;

    rc = nvs_init(&nvs, DT_LABEL(DT_NODELABEL(STORAGE_PARTITION)));
    if (rc) {
        printk("NVS init failed\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(sensor_dev);
        if (rc == 0) {
            rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        }

        if (rc) {
            printk("Failed to fetch sensor data\n");
        } else {
            printk("Accelerometer (x, y, z): (%d.%06d, %d.%06d, %d.%06d)\n",
                   accel[0].val1, accel[0].val2,
                   accel[1].val1, accel[1].val2,
                   accel[2].val1, accel[2].val2);

            rc = nvs_write(&nvs, 1, &accel, sizeof(accel));
            if (rc < 0) {
                printk("Error writing to NVS\n");
            }
        }

        k_sleep(K_SECONDS(5));
    }
}