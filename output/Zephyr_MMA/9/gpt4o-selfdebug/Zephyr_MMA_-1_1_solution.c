#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/nvs/nvs.h>

#define STORAGE_OFFSET 0x1000  // NVS storage offset in the flash
#define STORAGE_SECTOR_SIZE 0x1000
#define STORAGE_SECTOR_COUNT 3

static struct nvs_fs fs;
static const struct device *sensor_dev;

/* Define a stack size and thread priority for the task */
#define STACK_SIZE 1024
#define PRIORITY 5

void mma8x5x_read_thread(void) {
    struct sensor_value accel[3];
    int rc;
    
    while (1) {
        rc = sensor_sample_fetch(sensor_dev);
        if (rc != 0) {
            printk("Failed to fetch sensor sample (%d)\n", rc);
            k_sleep(K_SECONDS(5));
            continue;
        }

        rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc != 0) {
            printk("Failed to get sensor channel (%d)\n", rc);
            k_sleep(K_SECONDS(5));
            continue;
        }

        printk("Acceleration X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n", 
               accel[0].val1, accel[0].val2, 
               accel[1].val1, accel[1].val2, 
               accel[2].val1, accel[2].val2);

        /* Store X-axis reading in the NVS */
        rc = nvs_write(&fs, 0, &accel[0], sizeof(accel[0]));
        if (rc < 0) {
            printk("Failed to write to NVS (%d)\n", rc);
        } else {
            printk("Stored X-axis reading to NVS\n");
        }

        k_sleep(K_SECONDS(5));  // Wait for 5 seconds before reading again
    }
}

void main(void) {
    int rc;

    struct flash_pages_info info;

    /* Initialize the NVS file system */
    fs.offset = STORAGE_OFFSET;

    rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), fs.offset, &info);
    if (rc) {
        printk("Failed to get page info (%d)\n", rc);
        return;
    }
    
    fs.sector_size = info.size;
    fs.sector_count = STORAGE_SECTOR_COUNT;

    if (nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL)) {
        printk("NVS init failed\n");
        return;
    }

    sensor_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));
    if (!sensor_dev) {
        printk("Sensor device not found\n");
        return;
    }

    /* Create a thread to read from the sensor periodically */
    k_thread_create(0, STACK_SIZE, mma8x5x_read_thread, NULL, NULL, NULL,
            PRIORITY, 0, K_NO_WAIT);
}