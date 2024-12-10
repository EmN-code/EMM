#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/storage/nvs.h>

#define MMA8X5X_I2C_ADDR 0x1C
#define STORAGE_PARTITION_LABEL "storage"
#define SENSOR_DATA_ID 1

void mma8x5x_read_and_store(const struct device *i2c_dev, struct nvs_fs *nvs)
{
    uint8_t data;
    int ret = i2c_reg_read_byte(i2c_dev, MMA8X5X_I2C_ADDR, 0x01, &data);
    if (ret) {
        printk("Failed to read sensor data (err %d)\n", ret);
        return;
    }

    ret = nvs_write(nvs, SENSOR_DATA_ID, &data, sizeof(data));
    if (ret < 0) {
        printk("Failed to store data in NVS (err %d)\n", ret);
    } else {
        printk("Stored sensor data: %d\n", data);
    }
}

void main(void)
{
    struct nvs_fs nvs;
    const struct device *i2c_dev;
    struct flash_sector fs;
    uint32_t size;
    int ret;

    printk("ESP32 MMA8x5x NVS Example\n");

    i2c_dev = device_get_binding(DT_LABEL(DT_NODELABEL(i2c0)));
    if (!i2c_dev) {
        printk("I2C device not found\n");
    }

    nvs.offset = FLASH_AREA_OFFSET(storage);
    ret = flash_get_page_info_by_offs(flash_area_get_device(FLASH_AREA_ID(storage)), nvs.offset, &fs);
    if (ret) {
        printk("Failed to get page info (err %d)\n", ret);
        return;
    }
    size = fs.fs_size;
    nvs.sector_size = fs.fs_sectors;
    nvs.sector_count = size / nvs.sector_size;

    ret = nvs_init(&nvs, STORAGE_PARTITION_LABEL);
    if (ret) {
        printk("NVS initialization failed (err %d)\n", ret);
        return;
    }

    while (1) {
        mma8x5x_read_and_store(i2c_dev, &nvs);
        k_sleep(K_SECONDS(5));
    }
}