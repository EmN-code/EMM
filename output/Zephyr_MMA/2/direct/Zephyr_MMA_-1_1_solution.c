
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/littlefs.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

static struct sensor_value accel_x, accel_y, accel_z;

void read_mma8x5x(const struct device *dev) {
    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &accel_x) < 0) {
        LOG_ERR("Failed to get X channel");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &accel_y) < 0) {
        LOG_ERR("Failed to get Y channel");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &accel_z) < 0) {
        LOG_ERR("Failed to get Z channel");
        return;
    }

    LOG_INF("X: %d.%06d, Y: %d.%06d, Z: %d.%06d",
            accel_x.val1, accel_x.val2,
            accel_y.val1, accel_y.val2,
            accel_z.val1, accel_z.val2);
}

void store_reading_in_nvs(const struct sensor_value *x, const struct sensor_value *y, const struct sensor_value *z) {
    struct fs_file_t file;
    fs_file_t_init(&file);

    if (fs_open(&file, "/lfs/accel_data.txt", FS_O_CREATE | FS_O_WRITE) < 0) {
        LOG_ERR("Failed to open file");
        return;
    }

    char buffer[128];
    int len = snprintf(buffer, sizeof(buffer), "X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
                       x->val1, x->val2, y->val1, y->val2, z->val1, z->val2);

    if (fs_write(&file, buffer, len) < 0) {
        LOG_ERR("Failed to write to file");
    }

    fs_close(&file);
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device not ready");
        return -1;
    }

    struct fs_mount_t lfs_mnt = {
        .type = FS_LITTLEFS,
        .mnt_point = "/lfs",
        .storage_dev = (void *)NVS_PARTITION_ID,
        .fs_data = NULL,
    };

    if (fs_mount(&lfs_mnt) < 0) {
        LOG_ERR("Failed to mount LittleFS");
        return -1;
    }

    while (1) {
        read_mma8x5x(dev);
        store_reading_in_nvs(&accel_x, &accel_y, &accel_z);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
