#include <zephyr.h>  // Include the Zephyr OS API header
#include <device.h>  // Include the device API
#include <drivers/sensor.h>  // Include sensor drivers API
#include <storage/flash_map.h>  // Include flash map for NVS
#include <fs/nvs.h>  // Include the NVS file system

#define SLEEP_TIME_SECONDS 5  // Sleep time in seconds
#define SENSOR_DEVICE DT_LABEL(DT_INST(0, nxp_mma8x5x))  // Device Tree label for the sensor

static struct nvs_fs myfs;  // Declare an NVS file system instance

void main(void) {
    struct device *sensor_dev = device_get_binding(SENSOR_DEVICE);  // Get sensor device binding
    if (!sensor_dev) {
        printk("Failed to bind to sensor device!\n");
        return;
    }

    struct nvs_fs nvs;
    nvs.offset = FLASH_AREA_OFFSET(storage);
    int ret = nvs_mount(&nvs);
    if (ret) {
        printk("Failed to mount NVS (%d)!\n", ret);
        return;
    }

    struct sensor_value accel[3];

    while (1) {
        ret = sensor_sample_fetch(sensor_dev);  // Fetch sensor samples
        if (ret) {
            printk("Sensor sample fetch error (%d)\n", ret);
            continue;
        }

        ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);  // Get XYZ acceleration data
        if (ret) {
            printk("Sensor channel get error (%d)\n", ret);
            continue;
        }

        printk("X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n", 
               accel[0].val1, accel[0].val2, 
               accel[1].val1, accel[1].val2, 
               accel[2].val1, accel[2].val2);

        /* Store the X-axis value in NVS */
        ret = nvs_write(&nvs, 1, &accel[0], sizeof(accel[0]));
        if (ret < 0) {
            printk("NVS write error (%d)\n", ret);
        }

        k_sleep(K_SECONDS(SLEEP_TIME_SECONDS));  // Sleep for defined seconds
    }
}