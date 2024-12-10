
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector *sectors;
    uint32_t sector_count;

    sectors = flash_get_sectors(NVS_PARTITION_ID, &sector_count);
    if (!sectors) {
        printk("Failed to get flash sectors\n");
        return;
    }

    fs.flash_device = sectors[0].fs_dev_id;
    fs.offset = sectors[0].fs_off;
    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = sector_count;

    err = nvs_mount(&fs);
    if (err) {
        printk("NVS mount failed: %d\n", err);
    }
}

int read_mma8x5x(uint8_t *buf) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    return i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
}

void store_value(uint8_t *buf) {
    int err;
    uint16_t id = 1;

    err = nvs_write(&fs, id, buf, 6);
    if (err < 0) {
        printk("NVS write failed: %d\n", err);
    } else {
        printk("Value stored successfully\n");
    }
}

int main(void) {
    uint8_t buf[6];

    init_nvs();

    while (1) {
        if (read_mma8x5x(buf) == 0) {
            store_value(buf);
        } else {
            printk("Failed to read MMA8x5x\n");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
