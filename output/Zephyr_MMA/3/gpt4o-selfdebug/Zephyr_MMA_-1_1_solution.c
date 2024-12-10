#include <zephyr.h>
#include <sys/reboot.h>
#include <logging/log.h>
#include <drivers/sensor.h>
#include <drivers/i2c.h>
#include <device.h>
#include <nvs/nvs.h>

LOG_MODULE_REGISTER(main);

#define SLEEP_TIME_MS 5000
#define MMA8X5X_I2C_ADDR 0x1C // Example address
#define STORAGE_ID 0x100 /** NVS storage ID **/
#define PARTITION_NAME storage /** Name of the storage partition **/

static struct nvs_fs fs;
static const struct device *i2c_dev;

void mma8_read_and_store() {
    uint8_t data[6]; // Assuming MMA8x5x returns 6 bytes of data
    int err;

    // Read the sensor data over I2C
    err = i2c_burst_read(i2c_dev, MMA8x5X_I2C_ADDR, 0x00, data, sizeof(data));
    if (err) {
        LOG_ERR("Failed to read sensor: %d", err);
        return;
    }

    LOG_INF("Sensor data: %02x %02x %02x %02x %02x %02x", data[0], data[1], data[2], data[3], data[4], data[5]);

    // Write to NVS
    err = nvs_write(&fs, STORAGE_ID, data, sizeof(data));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS: %d", err);
    } else {
        LOG_INF("Successfully stored reading to NVS");
    }
}

void main(void) {
    int err;
    struct flash_sector fs_sectors[10];
    uint32_t storage_size = 0;

    // Initialize I2C device
    i2c_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_kinetis_mma8x5x)));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return;
    }

    // Initialize NVS
    fs.offset = FLASH_AREA_OFFSET(storage_partition);
    err = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), fs.offset, fs_sectors);
    if (err) {
        LOG_ERR("Unable to get flash page info");
        return;
    }

    fs.sector_size = fs_sectors->size;
    fs.sector_count = (storage_size / fs.sector_size);

    err = nvs_init(&fs, PARTITION_NAME);
    if (err) {
        LOG_ERR("NVS init failed");
        return;
    }

    while (1) {
        mma8_read_and_store();
        k_msleep(SLEEP_TIME_MS);
    }
}
