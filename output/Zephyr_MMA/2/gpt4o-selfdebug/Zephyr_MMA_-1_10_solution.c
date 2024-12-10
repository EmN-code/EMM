#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <settings/settings.h>
#include <sys/printk.h>

#define MMA8x5x_SENSOR_LABEL DT_LABEL(DT_INST(0, freescale_mma8652))
#define READ_INTERVAL K_SECONDS(5)

void main(void) {
    const struct device *sensor;
    struct sensor_value accel[3];

    int ret = settings_subsys_init();
    if (ret) {
        printk("Failed to initialize settings subsystem: %d\n", ret);
        return;
    }

    sensor = device_get_binding(MMA8x5x_SENSOR_LABEL);
    if (!sensor) {
        printk("Could not get %s device\n", MMA8x5x_SENSOR_LABEL);
        return;
    }

    while (1) {
        ret = sensor_sample_fetch(sensor);
        if (ret) {
            printk("Sensor sample update error: %d\n", ret);
        } else {
            sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
            printk("Accel (m/s²): X=%d, Y=%d, Z=%d\n", accel[0].val1, accel[1].val1, accel[2].val1);

            // Store the accelerometer readings in NVS
            settings_save_one("/nv/accel_x", &accel[0], sizeof(accel[0]));
            settings_save_one("/nv/accel_y", &accel[1], sizeof(accel[1]));
            settings_save_one("/nv/accel_z", &accel[2], sizeof(accel[2]));
        }

        k_sleep(READ_INTERVAL);
    }
}