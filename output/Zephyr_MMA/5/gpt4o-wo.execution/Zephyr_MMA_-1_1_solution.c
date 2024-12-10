// The goal of this code is to read data from an MMA8x5x sensor every 5 seconds 
// and store the read data persistently using Non-Volatile Storage (NVS) on an ESP32.

#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>

LOG_MODULE_REGISTER(mma8x5x_nvs, LOG_LEVEL_INF);

#define MMA8X5X_LABEL "ACCEL"
#define NVS_FLASH_AREA_ID 1

void mma8x5x_read_and_store(const struct device *sensor, const struct device *nvs_dev) {
    struct sensor_value accel[3];
    int ret;

    // Fetch the sensor data
    ret = sensor_sample_fetch(sensor);
    if (ret) {
        LOG_ERR("Failed to fetch sensor sample (err: %d)", ret);
        return;
    }

    // Get the sensor data
    ret = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor channel (err: %d)", ret);
        return;
    }

    LOG_INF("Accel X: %d.%06d, Y: %d.%06d, Z: %d.%06d", 
             accel[0].val1, accel[0].val2,
             accel[1].val1, accel[1].val2,
             accel[2].val1, accel[2].val2);

    // Use NVS to store the sensor data
    ret = flash_area_write(nvs_dev, 0, &accel, sizeof(accel));
    if (ret) {
        LOG_ERR("Failed to write data to NVS (err: %d)", ret);
    } else {
        LOG_INF("Sensor data written to NVS");
    }
}

int main(void) {
    const struct device *sensor, *nvs_dev;
    struct flash_area *fa;
    int err;

    LOG_INF("MMA8x5x sensor reading and storing to NVS started");

    // Get sensor device binding
    sensor = device_get_binding(MMA8X5X_LABEL);
    if (!sensor) {
        LOG_ERR("Cannot get MMA8x5x device");
        return -1;
    }

    // Open NVS flash area
    err = flash_area_open(NVS_FLASH_AREA_ID, &fa);
    if (err) {
        LOG_ERR("Cannot open NVS flash area");
        return err;
    }

    nvs_dev = device_get_binding(FLASH_AREA_LABEL_NVS);
    if (!nvs_dev) {
        LOG_ERR("Cannot get NVS device");
        return -1;
    }

    // Infinite loop to read and store sensor data every 5 seconds
    while (1) {
        mma8x5x_read_and_store(sensor, nvs_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}