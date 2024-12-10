#include <zephyr/storage/flash_map_defs.h>
#include <zephyr/sys/nvs.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define MMA8X5X_DEVICE DT_LABEL(DT_INST(0, nxp_fxos8700))
#define STORAGE_ID 1
#define STORAGE_KEY 1

void main(void) {
    const struct device *mma8x5x_dev = device_get_binding(MMA8X5X_DEVICE);
    if (!mma8x5x_dev) {
        printk("Failed to get device binding for MMA8X5X sensor\n");
        return;
    }
    
    static struct nvs_fs fs;
    fs.offset = DT_REG_ADDR(DT_NODELABEL(nvs_storage));
    
    struct flash_pages_info info;
    if (flash_get_page_info_by_offs(device_get_binding(DT_FLASH_CONTROLLER), fs.offset, &info) != 0) {
        printk("Failed to get page info\n");
        return;
    }
    fs.sector_size = info.size;
    fs.sector_count = DT_REG_SIZE(DT_NODELABEL(nvs_storage)) / fs.sector_size;
    
    int rc = nvs_init(&fs, DT_FLASH_DEV_NAME);
    if (rc) {
        printk("Flash Init failed: %d\n", rc);
        return;
    }

    while (1) {
        uint8_t sensor_value;
        int ret = sensor_sample_fetch(mma8x5x_dev);
        if (ret != 0) {
            printk("Sensor fetch error\n");
            continue;
        }
        
        struct sensor_value val;
        sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_X, &val);
        sensor_value = val.val1; // Storing only whole part for simplicity

        rc = nvs_write(&fs, STORAGE_KEY, &sensor_value, sizeof(sensor_value));
        if (rc < 0) {
            printk("Unable to write data to NVS: %d\n", rc);
            continue;
        }

        printk("Stored sensor value: %d\n", sensor_value);
        k_sleep(K_SECONDS(5));
    }
}