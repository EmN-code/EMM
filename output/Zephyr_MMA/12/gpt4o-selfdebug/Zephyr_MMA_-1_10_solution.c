#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/nvs.h>

LOG_MODULE_REGISTER(mma8x5x_reader);

// Device tree node identifiers for I2C accelerometer device
#define MMA8X5X_NODE DT_INST(0, fsl_mma8x5x)

static struct device *i2c_dev;
static struct nvs_fs nvs;

#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 3
#define NVS_STORAGE_ID 1
#define ACCEL_DATA_ID 1

// Timer for periodic reading
void read_accel_data(struct k_timer *dummy)
{
    uint8_t data[6]; // assuming 3 axes data (x, y, z) with 2 bytes each
    int ret = i2c_burst_read(i2c_dev, DT_REG_ADDR(MMA8X5X_NODE), 0x01, data, 6);
    if (ret != 0) {
        LOG_ERR("Error reading accelerometer data: %d", ret);
        return;
    }
    
    // Assume data processing here
    LOG_INF("Acceleration X: %d, Y: %d, Z: %d", data[0], data[2], data[4]);

    // Storing data on NVS
    ret = nvs_write(&nvs, ACCEL_DATA_ID, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write acceleration data in NVS: %d", ret);
    }
}

K_TIMER_DEFINE(read_accel_timer, read_accel_data, NULL);

void main(void)
{
    int ret;

    // Initializing NVS
    struct flash_pages_info info;
    nvs.offset = ((flash_area_get_sectors(NVS_STORAGE_ID, &info, 1) == 0) ? info.start_offset : 0);
    nvs.sector_size = NVS_SECTOR_SIZE;
    nvs.sector_count = NVS_SECTOR_COUNT;
    ret = nvs_init(&nvs, DT_PROP(DT_CHOSEN(zephyr_flash), size));
    if (ret) {
        LOG_ERR("Failed to initialize NVS: %d", ret);
        return;
    }

    // Look up for I2C device
    i2c_dev = device_get_binding(DT_LABEL(MMA8X5X_NODE));
    if (!i2c_dev) {
        LOG_ERR("Failed to get I2C device");
        return;
    }

    // Start timer to read accelerometer every 5 seconds
    k_timer_start(&read_accel_timer, K_SECONDS(5), K_SECONDS(5));
}