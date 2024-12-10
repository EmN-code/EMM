
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_LABEL DT_LABEL(DT_ALIAS(storage_partition))

static struct nvs_fs fs;

void read_mma8x5x(uint8_t *buf) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C: Device not found\n");
        return;
    }

    if (i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6) < 0) {
        printk("I2C: Read failed\n");
    }
}

void store_value_in_nvs(uint8_t *buf) {
    struct flash_sector sector;
    int rc;

    if (flash_get_sector_info_by_id(FLASH_AREA_ID(storage_partition), 0, &sector) != 0) {
        printk("Failed to get sector info\n");
        return;
    }

    fs.offset = sector.start;
    fs.sector_size = sector.size;
    fs.sector_count = 1;

    rc = nvs_init(&fs, NVS_PARTITION_LABEL);
    if (rc) {
        printk("NVS init failed: %d\n", rc);
        return;
    }

    rc = nvs_write(&fs, 1, buf, 6);
    if (rc < 0) {
        printk("NVS write failed: %d\n", rc);
    } else {
        printk("NVS write success: %d bytes\n", rc);
    }
}

int main(void) {
    uint8_t buffer[6];

    while (1) {
        read_mma8x5x(buffer);
        store_value_in_nvs(buffer);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
