
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/littlefs.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/settings/settings.h>

#define MMA8x5x_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct device *mma8x5x_dev;

static void read_sensor_data(void)
{
    struct sensor_value accel[3];

    if (sensor_sample_fetch(mma8x5x_dev) < 0) {
        printk("Sensor sample update failed\n");
        return;
    }

    if (sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        printk("Cannot read sensor channels\n");
        return;
    }

    printk("Acceleration: X=%f Y=%f Z=%f\n", sensor_value_to_double(&accel[0]),
           sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));

    // Store the reading value persistently using NVS
    struct fs_file_t file;
    fs_file_t_init(&file);

    if (fs_open(&file, "/lfs/accel_data.txt", FS_O_CREATE | FS_O_WRITE) < 0) {
        printk("Failed to open file for writing\n");
        return;
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "X=%f Y=%f Z=%f\n", sensor_value_to_double(&accel[0]),
             sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));

    if (fs_write(&file, buffer, strlen(buffer)) < 0) {
        printk("Failed to write to file\n");
    }

    fs_close(&file);
}

int main(void)
{
    printk("Starting MMA8x5x sensor reading with NVS storage on ESP32\n");

    mma8x5x_dev = device_get_binding(DT_LABEL(MMA8x5x_NODE));
    if (!mma8x5x_dev) {
        printk("Could not get MMA8x5x device\n");
        return -1;
    }

    // Initialize NVS filesystem
    struct fs_mount_t lfs_mnt = {
        .type = FS_LITTLEFS,
        .mnt_point = "/lfs",
        .storage_dev = (void *)NVS_PARTITION_ID,
        .fs_data = NULL,
    };

    if (fs_mount(&lfs_mnt) < 0) {
        printk("Failed to mount LittleFS\n");
        return -1;
    }

    while (1) {
        read_sensor_data();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
