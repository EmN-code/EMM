#include <zephyr/syscalls/nvs.h>
#include <zephyr/sys/time_units.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>

// Device tree overlay should have a proper node for the MMA8652 sensor like this:
// &i2c1 {
//     mma8652@1d {
//         compatible = "nxp,mma8652fc";
//         reg = <0x1d>;
//         label = "MMA8652";
//     };
// };

#define MMA8652_NODE DT_NODELABEL(mma8652)

#if DT_NODE_HAS_STATUS(MMA8652_NODE, okay)
#define MMA8652_LABEL DT_LABEL(MMA8652_NODE)
#else
#error "MMA8652 device not found"
#endif

void main(void) {
    int err;
    struct nvs_fs fs;
    struct device *dev;
    struct sensor_value acc_value;
    
    // Initialize NVS on an empty flash sector
    fs.offset = FLASH_AREA_OFFSET(storage);
    err = nvs_init(&fs, STORAGE_NODE);
    if (err) {
        printk("NVS initialization failed
");
        return;
    }

    dev = device_get_binding(MMA8652_LABEL);
    if (!dev) {
        printk("Failed to find sensor %s
", MMA8652_LABEL);
        return;
    }

    while (1) {
        // Fetch the accelerometer reading
        err = sensor_sample_fetch(dev);
        if (err) {
            printk("Sensor fetch failed
");
            return;
        }

        err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &acc_value);
        if (err) {
            printk("Sensor channel read failed
");
            return;
        }

        // Store the reading persistently
        err = nvs_write(&fs, NVS_ID, &acc_value, sizeof(acc_value));
        if (err < 0) {
            printk("NVS write failed: %d
", err);
        }

        // Delay next read by 5 seconds
        k_sleep(K_SECONDS(5));
    }
}