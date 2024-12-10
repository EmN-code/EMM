
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/littlefs.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>

#define MMA8x5x_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(storage_partition)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(storage_partition)
#define NVS_PARTITION_SIZE FIXED_PARTITION_SIZE(storage_partition)

LOG_MODULE_REGISTER(main);

static struct fs_mount_t nvs_mnt = {
    .type = FS_LITTLEFS,
    .mnt_point = "/nvs",
    .fs_data = NULL,
};

static int init_nvs(void) {
    int rc;
    struct flash_pages_info info;

    rc = flash_get_page_info_by_offs(NVS_PARTITION_DEVICE, NVS_PARTITION_OFFSET, &info);
    if (rc) {
        LOG_ERR("Failed to get page info (%d)", rc);
        return rc;
    }

    nvs_mnt.storage_dev = NVS_PARTITION_DEVICE;
    nvs_mnt.storage_dev_id = NVS_PARTITION_ID;
    nvs_mnt.storage_area_offset = NVS_PARTITION_OFFSET;
    nvs_mnt.storage_area_size = NVS_PARTITION_SIZE;

    rc = fs_mount(&nvs_mnt);
    if (rc) {
        LOG_ERR("Failed to mount NVS partition (%d)", rc);
        return rc;
    }

    return 0;
}

static int store_reading(int16_t x, int16_t y, int16_t z) {
    int rc;
    struct fs_file_t file;
    char buffer[16];

    fs_file_t_init(&file);
    rc = fs_open(&file, "/nvs/accel_data", FS_O_CREATE | FS_O_WRITE);
    if (rc) {
        LOG_ERR("Failed to open file (%d)", rc);
        return rc;
    }

    snprintf(buffer, sizeof(buffer), "%d,%d,%d\n", x, y, z);
    rc = fs_write(&file, buffer, strlen(buffer));
    if (rc < 0) {
        LOG_ERR("Failed to write to file (%d)", rc);
    }

    fs_close(&file);
    return rc;
}

int main(void) {
    const struct device *dev;
    struct sensor_value accel[3];
    int rc;

    dev = DEVICE_DT_GET(MMA8x5x_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return -ENODEV;
    }

    rc = init_nvs();
    if (rc) {
        LOG_ERR("Failed to initialize NVS (%d)", rc);
        return rc;
    }

    while (1) {
        rc = sensor_sample_fetch(dev);
        if (rc) {
            LOG_ERR("Failed to fetch sample (%d)", rc);
            continue;
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            LOG_ERR("Failed to get sensor data (%d)", rc);
            continue;
        }

        LOG_INF("Accelerometer: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
                accel[0].val1, accel[0].val2,
                accel[1].val1, accel[1].val2,
                accel[2].val1, accel[2].val2);

        rc = store_reading(accel[0].val1, accel[1].val1, accel[2].val1);
        if (rc) {
            LOG_ERR("Failed to store reading (%d)", rc);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
