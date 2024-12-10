
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
#define NVS_PARTITION FLASH_AREA_ID(storage)

static struct sensor_value accel_x, accel_y, accel_z;

static int read_mma8x5x(const struct device *dev) {
    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return -EIO;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &accel_x) < 0) {
        LOG_ERR("Failed to get X channel");
        return -EIO;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &accel_y) < 0) {
        LOG_ERR("Failed to get Y channel");
        return -EIO;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &accel_z) < 0) {
        LOG_ERR("Failed to get Z channel");
        return -EIO;
    }

    LOG_INF("X: %d.%06d m/s^2, Y: %d.%06d m/s^2, Z: %d.%06d m/s^2",
            accel_x.val1, accel_x.val2, accel_y.val1, accel_y.val2, accel_z.val1, accel_z.val2);

    return 0;
}

static int store_reading_to_nvs(void) {
    struct fs_file_t file;
    char buffer[64];
    int ret;

    fs_file_t_init(&file);

    ret = fs_open(&file, "/lfs/accel_data.txt", FS_O_CREATE | FS_O_WRITE);
    if (ret) {
        LOG_ERR("Failed to open file: %d", ret);
        return ret;
    }

    snprintf(buffer, sizeof(buffer), "X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
             accel_x.val1, accel_x.val2, accel_y.val1, accel_y.val2, accel_z.val1, accel_z.val2);

    ret = fs_write(&file, buffer, strlen(buffer));
    if (ret < 0) {
        LOG_ERR("Failed to write to file: %d", ret);
    }

    fs_close(&file);
    return ret;
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return -ENODEV;
    }

    int ret = settings_subsys_init();
    if (ret) {
        LOG_ERR("Settings subsystem init failed: %d", ret);
        return ret;
    }

    ret = fs_mount(&littlefs_mount);
    if (ret) {
        LOG_ERR("Failed to mount littlefs: %d", ret);
        return ret;
    }

    while (1) {
        if (read_mma8x5x(dev) == 0) {
            store_reading_to_nvs();
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
