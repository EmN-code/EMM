#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs.h>

#define MMA8X5X_I2C_DEV DT_NODELABEL(i2c0)
#define NVS_SECTOR_SIZE FLASH_AREA_SIZE(storage)
#define NVS_SECTOR_COUNT 3
#define NVS_STORAGE_OFFSET FLASH_AREA_OFFSET(storage)

static const struct i2c_dt_spec mma8x5x = I2C_DT_SPEC_GET(MMA8X5X_I2C_DEV);
static struct nvs_fs fs;
static int32_t sensor_data = 0;

void main(void) {
    int rc;
    struct nvs_fs fs_config = {
        .sector_size = NVS_SECTOR_SIZE,
        .sector_count = NVS_SECTOR_COUNT,
        .offset = NVS_STORAGE_OFFSET,
        .flash_dev_name = DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL,
    };
    struct sensor_value val;

    if (!device_is_ready(mma8x5x.bus)) {
        printk("I2C bus not ready\n");
        return;
    }

    rc = nvs_init(&fs, &fs_config);
    if (rc) {
        printk("NVS Initialization failed\n");
        return;
    }

    while (1) {
        rc = i2c_read(mma8x5x.bus, &val, sizeof(val), mma8x5x.addr);
        if (rc != 0) {
            printk("Failed to read sensor data\n");
        } else {
            sensor_data = val.val1;

            rc = nvs_write(&fs, 1, &sensor_data, sizeof(sensor_data));
            if (rc < 0) {
                printk("Failed to write to NVS\n");
            } else {
                printk("Stored sensor data: %d\n", sensor_data);
            }
        }
        k_sleep(K_SECONDS(5));
    }
}