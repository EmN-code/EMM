// Description: The following code initializes and configures a Zephyr-based application on an ESP32 that reads data from an MMA8x5x accelerometer every 5 seconds and stores it persistently using NVS.

// Header files included in descending order by their first letter.
#include <zephyr/kernel.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/device.h>
#include <drivers/sensor.h>

// Macro for NVS storage size
#define STORAGE_SIZE 512

// Timer delay for 5 seconds
#define TIMER_DELAY K_SECONDS(5)

// NVS file system
static struct nvs_fs fs;

// Logger macros
#define LOG_MODULE_NAME app
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

// Timer work handler function
void read_and_store_sensor_data(struct k_work *work) {
    struct sensor_value accel[3];
    const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652)));

    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return;
    }

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Sensor sample update error");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Cannot read MMA8x5x channels");
        return;
    }

    // Print and store readings using NVS
    LOG_INF("Accel X: %d.%06d", accel[0].val1, accel[0].val2);
    LOG_INF("Accel Y: %d.%06d", accel[1].val1, accel[1].val2);
    LOG_INF("Accel Z: %d.%06d", accel[2].val1, accel[2].val2);

    int rc = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS (error: %d)", rc);
    }
}

// Timer work item and queue
K_WORK_DEFINE(sensor_work, read_and_store_sensor_data);
k_timer work_timer;

void timer_expiry_function(struct k_timer *dummy) {
    k_work_submit(&sensor_work);
}

K_TIMER_DEFINE(work_timer, timer_expiry_function, NULL);

// Main function initializing NVS and setting up the timer
int main(void) {
    struct flash_pages_info info;

    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = (STORAGE_SIZE / fs.sector_size);

    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device not ready");
        return -1;
    }

    int rc = nvs_init(&fs, DT_LABEL(DT_CHOSEN(zephyr_flash)))
    if (rc) {
        LOG_ERR("Failed to initialize NVS (error: %d)", rc);
        return rc;
    }

    // Start the timer
    k_timer_start(&work_timer, TIMER_DELAY, TIMER_DELAY);
    return 0;
}