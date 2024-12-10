
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/drivers/flash.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

const struct device *i2c_dev;
struct nvs_fs nvs;

void read_mma8x5x(uint8_t *buf) {
    i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
}

void store_value_in_nvs(uint8_t *buf) {
    struct flash_sector sector;
    flash_get_sector_info_by_id(NVS_PARTITION_ID, 0, &sector);
    nvs.flash_device = (const struct device *)flash_device_get_binding("storage");
    nvs.offset = sector.start;
    nvs.sector_size = sector.size;
    nvs_init(&nvs, "storage");
    nvs_write(&nvs, 0, buf, 6);
}

int main(void) {
    uint8_t buffer[6];

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    while (1) {
        read_mma8x5x(buffer);
        store_value_in_nvs(buffer);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
