
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8x5x_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    struct flash_sector_param sector[1];

    sector[0].fs_size = 4096; // Example size, adjust as needed
    sector[0].offset = FIXED_PARTITION_OFFSET(storage_partition);

    fs.sector_size = 4096; // Example size, adjust as needed
    fs.sector_count = 1;
    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);

    int ret = nvs_init(&fs, "storage");
    if (ret) {
        printk("NVS init failed: %d\n", ret);
    } else {
        printk("NVS init successful\n");
    }
}

void store_reading(int32_t reading) {
    int ret = nvs_write(&fs, 1, &reading, sizeof(reading));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful\n");
    }
}

void read_sensor(const struct device *dev) {
    struct sensor_value accel[3];

    if (sensor_sample_fetch(dev) < 0) {
        printk("Sensor sample update failed\n");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        printk("Failed to get sensor data\n");
        return;
    }

    int32_t reading = (accel[0].val1 * 1000000 + accel[0].val2) / 1000; // Example conversion
    printk("Acceleration: %d\n", reading);

    store_reading(reading);
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_NODE);

    if (!device_is_ready(dev)) {
        printk("Device not ready\n");
        return 0;
    }

    init_nvs();

    while (1) {
        read_sensor(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
