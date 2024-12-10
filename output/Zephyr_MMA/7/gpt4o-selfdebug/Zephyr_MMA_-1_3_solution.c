

// Add the required Zephyr header files from the top-level to the specific header needed.
#include <zephyr/kernel.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs/nvs.h>
#include <zephyr/drivers/i2c.h>

// Define the I2C device node and create a unique label for MMA8x5x
#define MMA8x5x_NODE DT_ALIAS(i2c_mma8x5x)

// Error checking for the I2C node existence
#if !DT_NODE_HAS_STATUS(MMA8x5x_NODE, okay)
#error "Unsupported board: mma8x5x devicetree alias is not defined"
#endif

// Retrieve device binding for I2C
static const struct device *i2c_dev = DEVICE_DT_GET(DT_BUS(MMA8x5x_NODE));

// Error check device readiness
if (!device_is_ready(i2c_dev)) {
    printk("I2C device not found\n");
    return;
}

// Flash storage setup for NVS
struct nvs_fs fs;
struct flash_sector sectors[CONFIG_NVS_SECTOR_COUNT];
fs.offset = FLASH_AREA_OFFSET(storage);
fs.sector_size = FLASH_AREA_SIZE(storage) >> (CONFIG_NVS_SECTOR_COUNT - 1);
fs.sector_count = CONFIG_NVS_SECTOR_COUNT;

// Initialize flash map with sectors
int rc = flash_area_open(FLASH_AREA_ID(storage), &fs.flash_device);
if (rc) {
    printk("Can't open flash area\n");
    return;
}

rc = flash_get_page_info(&fs.flash_device, 0, sectors);
if (rc) {
    printk("Can't get page info\n");
    return;
}

fs.sectors = sectors;

// Initialize the NVS file system
rc = nvs_init(&fs, FLASH_AREA_ID(storage));
if (rc) {
    printk("Failed to initialize NVS\n");
    return;
}

// Main application loop
int main(void) {
    while (1) {
        // Simulate sensor read (replace with actual I2C read routine)
        uint8_t sensor_data[6]; // assuming 6 bytes for the MMA8x5x accelerometer data
        printk("Reading MMA8x5x sensor\n");
        rc = i2c_burst_read(i2c_dev, DT_REG_ADDR(MMA8x5x_NODE), 0x00, sensor_data, sizeof(sensor_data));
        if (rc) {
            printk("Failed to read sensor data\n");
        } else {
            // Print read sensor data
            printk("Sensor Data: ");
            for (int i = 0; i < sizeof(sensor_data); i++) {
                printk("%02x ", sensor_data[i]);
            }
            printk("\n");

            // Store sensor data into NVS
            rc = nvs_write(&fs, NVS_ID_EXTRA + 1, sensor_data, sizeof(sensor_data));
            if (rc < 0) {
                printk("Failed to store sensor data\n");
            }
        }
        // Sleep for 5 seconds
        k_sleep(K_SECONDS(5));
    }
    return 0;
}

