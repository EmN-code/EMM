#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/settings/settings.h>

#define I2C_NODE DT_NODELABEL(i2c0)
#define MMA8X5X_DT_SPEC DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)

#if !DT_HAS_COMPAT_STATUS_OKAY(mma8x5x)
#error "No mma8x5x compatible node found in the device tree"
#endif

#define NVS_FLASH_STORAGE_PARTITION storage_partition
#define MMA8X5X_ADDR 0x1D  // Example I2C address

void main(void)
{
    const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);
    const struct device *mma8x5x_dev = device_get_binding(DT_LABEL(MMA8X5X_DT_SPEC));
    struct nvs_fs nvs;
    int ret;

    // Initialize NVS
    nvs.offset = DT_FLASH_AREA_OFFSET(NVS_FLASH_STORAGE_PARTITION);
    ret = nvs_init(&nvs, STORAGE_NODE_ID);
    if (ret) {
        printk("NVS init failed (%d)\n", ret);
        return;
    }

    while (1) {
        uint8_t data[2] = {0};
        ret = i2c_reg_read_byte(i2c_dev, MMA8X5X_ADDR, REG_WHO_AM_I, &data[0]);
        if (ret == 0) {
            printk("MMA8X5x Device ID: 0x%x\n", data[0]);

            // Write to NVS
            ret = nvs_write(&nvs, 1, data, sizeof(data));
            if (ret < 0) {
                printk("NVS write failed (%d)\n", ret);
            }
        } else {
            printk("Read failed: %d\n", ret);
        }

        k_sleep(K_SECONDS(5));
    }
}