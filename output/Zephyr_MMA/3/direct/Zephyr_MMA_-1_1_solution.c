
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/littlefs.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_INST(0, nxp_mma8x5x)
#if DT_NODE_HAS_STATUS(MMA8x5x_NODE, okay)
#define MMA8x5x_LABEL DT_LABEL(MMA8x5x_NODE)
#else
#error "MMA8x5x device not found in the device tree"
#endif

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(storage)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(storage)
#define NVS_PARTITION_SIZE FIXED_PARTITION_SIZE(storage)

#define NVS_FILE_PATH "/lfs/mma8x5x_data.txt"

static struct fs_file_t file;

void save_reading_to_nvs(float value)
{
    int ret;
    struct fs_dirent entry;

    ret = fs_stat(NVS_FILE_PATH, &entry);
    if (ret < 0 && ret != -ENOENT) {
        LOG_ERR("Failed to stat file: %d", ret);
        return;
    }

    if (ret == 0) {
        ret = fs_unlink(NVS_FILE_PATH);
        if (ret < 0) {
            LOG_ERR("Failed to unlink file: %d", ret);
            return;
        }
    }

    fs_file_t_init(&file);
    ret = fs_open(&file, NVS_FILE_PATH, FS_O_CREATE | FS_O_WRITE);
    if (ret < 0) {
        LOG_ERR("Failed to open file: %d", ret);
        return;
    }

    char buffer[16];
    int len = snprintf(buffer, sizeof(buffer), "%.2f\n", value);
    ret = fs_write(&file, buffer, len);
    if (ret < 0) {
        LOG_ERR("Failed to write to file: %d", ret);
    }

    fs_close(&file);
}

void read_mma8x5x(const struct device *dev)
{
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        LOG_ERR("Failed to fetch sample: %d", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret < 0) {
        LOG_ERR("Failed to get sensor data: %d", ret);
        return;
    }

    float x = sensor_value_to_double(&accel[0]);
    float y = sensor_value_to_double(&accel[1]);
    float z = sensor_value_to_double(&accel[2]);

    LOG_INF("MMA8x5x: X=%.2f, Y=%.2f, Z=%.2f", x, y, z);

    save_reading_to_nvs(x);
}

int main(void)
{
    const struct device *dev = device_get_binding(MMA8x5x_LABEL);
    if (dev == NULL) {
        LOG_ERR("Failed to get device binding");
        return -ENODEV;
    }

    struct fs_mount_t lfs_mnt = {
        .type = FS_LITTLEFS,
        .mnt_point = "/lfs",
        .fs_data = NULL,
        .storage_dev = (void *)NVS_PARTITION_DEVICE,
        .storage_dev_id = NVS_PARTITION_ID,
        .mnt_count = 0,
    };

    int ret = fs_mount(&lfs_mnt);
    if (ret < 0) {
        LOG_ERR("Failed to mount LittleFS: %d", ret);
        return ret;
    }

    while (1) {
        read_mma8x5x(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
