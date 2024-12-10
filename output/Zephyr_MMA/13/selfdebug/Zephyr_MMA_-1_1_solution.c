
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_NAMESPACE "mma8x5x"
#define NVS_KEY "accel_data"

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

static int read_mma8x5x(const struct device *i2c_dev, uint8_t *data)
{
    return i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, 6);
}

static int store_data_to_nvs(uint8_t *data, size_t len)
{
    struct nvs_fs *fs_ptr = &fs;
    int rc;

    rc = nvs_write(fs_ptr, NVS_KEY, data, len);
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", rc);
        return rc;
    }

    LOG_INF("Data stored in NVS");
    return 0;
}

static int init_nvs(void)
{
    int rc;
    struct flash_area *fa;

    rc = flash_area_open(NVS_PARTITION, &fa);
    if (rc) {
        LOG_ERR("Failed to open flash area (err %d)", rc);
        return rc;
    }

    fs.flash_device = fa->fa_device;
    fs.offset = fa->fa_off;
    fs.sector_size = 4096; // Adjust based on your flash sector size
    fs.sector_count = 1;   // Adjust based on your flash size

    rc = nvs_init(&fs, fs.flash_device);
    if (rc) {
        LOG_ERR("Failed to initialize NVS (err %d)", rc);
        return rc;
    }

    LOG_INF("NVS initialized");
    return 0;
}

int main(void)
{
    const struct device *i2c_dev;
    uint8_t accel_data[6];
    int rc;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -ENODEV;
    }

    rc = init_nvs();
    if (rc) {
        LOG_ERR("NVS initialization failed");
        return rc;
    }

    while (1) {
        rc = read_mma8x5x(i2c_dev, accel_data);
        if (rc) {
            LOG_ERR("Failed to read MMA8x5x (err %d)", rc);
        } else {
            LOG_INF("MMA8x5x data read: %02x %02x %02x %02x %02x %02x",
                    accel_data[0], accel_data[1], accel_data[2],
                    accel_data[3], accel_data[4], accel_data[5]);

            rc = store_data_to_nvs(accel_data, sizeof(accel_data));
            if (rc) {
                LOG_ERR("Failed to store data in NVS");
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
