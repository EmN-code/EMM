
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

// Define the NVS storage
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION    FIXED_PARTITION_DEVICE(storage_partition)

// Define the MMA8x5x sensor instance
#define MMA8x5x DT_N_INST_0_nxp_mma8x5x

// Function to initialize NVS
int init_nvs(struct nvs_fs *fs) {
    fs->flash_device = NVS_PARTITION;
    fs->offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    fs->sector_size = FIXED_PARTITION_SECTOR_SIZE(NVS_PARTITION_ID);
    fs->sector_count = FIXED_PARTITION_SECTOR_COUNT(NVS_PARTITION_ID);

    return nvs_init(fs, NVS_PARTITION_ID);
}

int main(void) {
    // Initialize NVS
    struct nvs_fs fs;
    if (init_nvs(&fs) != 0) {
        printk("NVS initialization failed\n");
        return -1;
    }

    // Get the MMA8x5x sensor device
    const struct device *dev = DEVICE_DT_GET(MMA8x5x);
    if (!device_is_ready(dev)) {
        printk("MMA8x5x sensor is not ready\n");
        return -1;
    }

    // Main loop
    while (1) {
        // Read the sensor
        struct sensor_value accel[3];
        if (sensor_sample_fetch(dev) != 0) {
            printk("Failed to fetch sample from MMA8x5x\n");
            continue;
        }
        if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) != 0) {
            printk("Failed to get data from MMA8x5x\n");
            continue;
        }

        // Print the sensor values
        printk("Acceleration: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
               accel[0].val1, accel[0].val2,
               accel[1].val1, accel[1].val2,
               accel[2].val1, accel[2].val2);

        // Store the sensor values in NVS
        int32_t data = (accel[0].val1 << 16) | (accel[0].val2 & 0xFFFF);
        if (nvs_write(&fs, 0, &data, sizeof(data)) != sizeof(data)) {
            printk("Failed to write data to NVS\n");
        }

        // Wait for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
