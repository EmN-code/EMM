
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/littlefs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

static struct fs_mount_t lfs_mnt = {
    .type = FS_LITTLEFS,
    .mnt_point = "/lfs",
    .fs_data = NULL,
    .storage_dev = (void *)NVS_PARTITION_ID,
};

static int init_filesystem(void) {
    int ret;

    ret = fs_mount(&lfs_mnt);
    if (ret < 0) {
        LOG_ERR("Error mounting littlefs [%d]", ret);
        return ret;
    }

    return 0;
}

static int read_sensor_data(const struct device *dev, struct sensor_value *val) {
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return ret;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, val);
    if (ret < 0) {
        LOG_ERR("Failed to get data (%d)", ret);
        return ret;
    }

    return 0;
}

static int store_data_to_nvs(struct sensor_value *val) {
    int ret;
    struct fs_file_t file;
    char buffer[32];

    fs_file_t_init(&file);

    ret = fs_open(&file, "/lfs/sensor_data.txt", FS_O_CREATE | FS_O_WRITE);
    if (ret < 0) {
        LOG_ERR("Failed to open file (%d)", ret);
        return ret;
    }

    snprintk(buffer, sizeof(buffer), "X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
             val[0].val1, val[0].val2, val[1].val1, val[1].val2, val[2].val1, val[2].val2);

    ret = fs_write(&file, buffer, strlen(buffer));
    if (ret < 0) {
        LOG_ERR("Failed to write to file (%d)", ret);
    }

    fs_close(&file);
    return ret;
}

int main(void) {
    const struct device *dev;
    struct sensor_value accel_value[3];
    int ret;

    dev = DEVICE_DT_GET(MMA8x5x_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return -ENODEV;
    }

    ret = init_filesystem();
    if (ret < 0) {
        LOG_ERR("Failed to initialize filesystem (%d)", ret);
        return ret;
    }

    while (1) {
        ret = read_sensor_data(dev, accel_value);
        if (ret < 0) {
            LOG_ERR("Failed to read sensor data (%d)", ret);
        } else {
            ret = store_data_to_nvs(accel_value);
            if (ret < 0) {
                LOG_ERR("Failed to store data to NVS (%d)", ret);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
