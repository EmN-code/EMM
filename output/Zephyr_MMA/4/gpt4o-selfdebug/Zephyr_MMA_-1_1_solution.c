#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <storage/flash_map.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs.h>

#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))
#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8X5X_REG_OUT_X_MSB 0x01

#define STORAGE_ID FLASH_AREA_NVS_STORAGE_ID
#define NVS_SECTOR_SIZE 0x1000
#define NVS_SECTOR_COUNT 3

void main(void) {
    const struct device *i2c_dev;
    struct i2c_dt_spec i2c_spec = { .dev = DEVICE_DT_GET(DT_NODELABEL(i2c0)), .addr = MMA8x5x_I2C_ADDRESS };
    struct nvs_fs fs;
    struct nvs_sector sectors[NVS_SECTOR_COUNT];
    uint8_t accel_data[6];
    int16_t accel_values[3];
    int ret;

    i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("I2C: Device binding failed\n");
        return;
    }

    /* Setup NVS */
    for (int i = 0; i < NVS_SECTOR_COUNT; i++) {
        sectors[i].fs_off = i * NVS_SECTOR_SIZE;
        sectors[i].sector_size = NVS_SECTOR_SIZE;
    }
    fs.sector_count = NVS_SECTOR_COUNT;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sectors = sectors;

    ret = nvs_init(&fs, DT_FLASH_AREA_VADDR(STORAGE_ID));
    if (ret) {
        printk("NVS: Initialization failed\n");
        return;
    }

    while (1) {
        /* Read from the accelerometer */
        ret = i2c_reg_read_byte_dt(&i2c_spec, MMA8X5X_REG_OUT_X_MSB, accel_data);
        if (ret) {
            printk("I2C: Read failed\n");
        } else {
            /* Convert raw data to human-readable data */
            for (int i = 0; i < 3; i++) {
                accel_values[i] = (accel_data[2*i] << 8) | (accel_data[2*i + 1]);
                printk("Axis %d: %d\n", i, accel_values[i]);

                /* Store in NVS */
                ret = nvs_write(&fs, i, &accel_values[i], sizeof(accel_values[i]));
                if (ret) {
                    printk("NVS: Write failed\n");
                }
            }
        }
        k_sleep(K_SECONDS(5));
    }
}