
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

static int read_mma8x5x(const struct device *dev)
{
    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return -EIO;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &accel_x) < 0 ||
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &accel_y) < 0 ||
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &accel_z) < 0) {
        LOG_ERR("Failed to get sensor data");
        return -EIO;
    }

    LOG_INF("Acceleration: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel_x.val1, accel_x.val2,
            accel_y.val1, accel_y.val2,
            accel_z.val1, accel_z.val2);

    return 0;
}

static int store_to_nvs(const struct sensor_value *x, const struct sensor_value *y, const struct sensor_value *z)
{
    struct fs_file_t file;
    struct fs_mount_t mount = {
        .type = FS_LITTLEFS,
        .mnt_point = "/nvs",
        .storage_dev = (void *)NVS_PARTITION_ID,
    };

    fs_file_t_init(&file);

    if (fs_mount(&mount) < 0) {
        LOG_ERR("Failed to mount NVS partition");
        return -EIO;
    }

    if (fs_open(&file, "/nvs/accel.txt", FS_O_CREATE | FS_O_WRITE) < 0) {
        LOG_ERR("Failed to open file");
        fs_unmount(&mount);
        return -EIO;
    }

    char buf[64];
    int len = snprintf(buf, sizeof(buf), "X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
                       x->val1, x->val2, y->val1, y->val2, z->val1, z->val2);

    if (fs_write(&file, buf, len) < 0) {
        LOG_ERR("Failed to write to file");
        fs_close(&file);
        fs_unmount(&mount);
        return -EIO;
    }

    fs_close(&file);
    fs_unmount(&mount);

    return 0;
}

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);

    if (!device_is_ready(dev)) {
        LOG_ERR("Device MMA8X5X is not ready");
        return -ENODEV;
    }

    while (1) {
        if (read_mma8x5x(dev) == 0) {
            store_to_nvs(&accel_x, &accel_y, &accel_z);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
