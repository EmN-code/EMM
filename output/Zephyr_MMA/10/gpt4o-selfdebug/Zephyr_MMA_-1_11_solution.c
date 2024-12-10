#include <zephyr/nvs/nvs.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define STORAGE_NVS_PARTITION "storage"
#define STORAGE_SENSOR_KEY 1

void mma8x5x_read_and_store(struct k_work *work);

// Global structure for NVS
static struct nvs_fs fs;
static struct k_work_delayable sensor_work;

void mma8x5x_read_and_store(struct k_work *work) {
    const struct device *mma_device = device_get_binding("MMA8x5x");
    if (!mma_device) {
        printk("Failed to get MMA8x5x device binding\n");
        return;
    }

    struct sensor_value accel_x, accel_y, accel_z;
    if (sensor_sample_fetch(mma_device) < 0) {
        printk("Failed to fetch sample from MMA8x5x\n");
        return;
    }

    sensor_channel_get(mma_device, SENSOR_CHAN_ACCEL_X, &accel_x);
    sensor_channel_get(mma_device, SENSOR_CHAN_ACCEL_Y, &accel_y);
    sensor_channel_get(mma_device, SENSOR_CHAN_ACCEL_Z, &accel_z);

    printk("Acceleration X: %.2f, Y: %.2f, Z: %.2f\n",
           sensor_value_to_double(&accel_x),
           sensor_value_to_double(&accel_y),
           sensor_value_to_double(&accel_z));

    int rc = nvs_write(&fs, STORAGE_SENSOR_KEY, &accel_x, sizeof(accel_x));
    if (rc < 0) {
        printk("Failed to store sensor reading in NVS\n");
    }

    k_work_reschedule(&sensor_work, K_SECONDS(5));
}

void main(void) {
    int rc;
    struct flash_pages_info info;

    const struct device *flash_device = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH);
    if (!flash_device) {
        printk("Flash device not found\n");
        return;
    }

    rc = flash_get_page_info_by_offs(flash_device, FLASH_AREA_OFFSET(storage), &info);
    if (rc) {
        printk("Cannot get page info\n");
        return;
    }

    fs.offset = info.start_offset;
    fs.sector_size = info.size;

    // Initialize NVS
    if (nvs_init(&fs, STORAGE_NVS_PARTITION, &info)) {
        printk("NVS initialization failed\n");
        return;
    }

    k_work_init_delayable(&sensor_work, mma8x5x_read_and_store);
    k_work_reschedule(&sensor_work, K_NO_WAIT);
}
