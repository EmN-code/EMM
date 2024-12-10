#include <zephyr/types.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>

#define MMA8x5x_ADDR 0x1D     // Replace with actual I2C address of the sensor
#define DATA_REGISTER 0x00   // Replace with actual data register

static const struct device *i2c_dev;
static struct nvs_fs nvs;

void read_sensor_and_store()
{
    // Read data from sensor
    uint8_t sensor_data = 0;
    i2c_reg_read_byte(i2c_dev, MMA8x5x_ADDR, DATA_REGISTER, &sensor_data);

    // Store data in NVS
    nvs_write(&nvs, 1, &sensor_data, sizeof(sensor_data));
}

int main()
{
    // Check if the device is ready
    i2c_dev = device_get_binding("I2C_0");
    if (!device_is_ready(i2c_dev)) {
        return -1;
    }

    // Initialize the file system
    int ret = nvs_init(&nvs, "NVS_PARTITION");
    if (ret) {
        return ret;
    }

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}