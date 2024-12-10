
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_ALIAS(mma8x5x)
#define NVS_PARTITION DT_FLASH_AREA_STORAGE_ID

static struct device *mma8x5x_dev;
static struct fs_mount_t nvs_mnt = {
    .type = FS_NVS,
    .mnt_point = "/nvs",
    .storage_dev = (void *)NVS_PARTITION,
    .fs_data = NULL,
};

void read_and_store_sensor_data(void)
{
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(mma8x5x_dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data (%d)", ret);
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the reading value persistently in NVS
    struct fs_file_t file;
    fs_file_t_init(&file);

    ret = fs_open(&file, "/nvs/accel_data", FS_O_CREATE | FS_O_WRITE);
    if (ret) {
        LOG_ERR("Failed to open NVS file (%d)", ret);
        return;
    }

    ret = fs_write(&file, accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS file (%d)", ret);
    }

    fs_close(&file);
}

int main(void)
{
    int ret;

    mma8x5x_dev = device_get_binding(DT_LABEL(MMA8x5x_NODE));
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -ENODEV;
    }

    ret = fs_mount(&nvs_mnt);
    if (ret) {
        LOG_ERR("Failed to mount NVS (%d)", ret);
        return -EIO;
    }

    while (1) {
        read_and_store_sensor_data();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
