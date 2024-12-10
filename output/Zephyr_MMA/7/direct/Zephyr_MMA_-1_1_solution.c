
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_ALIAS(mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector_param sectors[1];

    sectors[0].fs_size = 4096; // Example size, adjust as needed
    sectors[0].fs_addr = FLASH_AREA_OFFSET(storage_partition);

    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = 1;
    fs.offset = sectors[0].fs_addr;

    err = nvs_init(&fs, "storage");
    if (err) {
        printk("NVS init failed: %d\n", err);
    } else {
        printk("NVS init successful\n");
    }
}

void store_reading(int32_t reading) {
    int err;
    err = nvs_write(&fs, 1, &reading, sizeof(reading));
    if (err < 0) {
        printk("NVS write failed: %d\n", err);
    } else {
        printk("NVS write successful\n");
    }
}

void read_sensor(const struct device *dev) {
    struct sensor_value accel[3];
    int32_t reading;

    if (sensor_sample_fetch(dev) < 0) {
        printk("Sensor sample update failed\n");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        printk("Failed to get sensor data\n");
        return;
    }

    reading = (accel[0].val1 << 16) | (accel[1].val1 << 8) | accel[2].val1;
    printk("Acceleration: X=%d, Y=%d, Z=%d\n", accel[0].val1, accel[1].val1, accel[2].val1);

    store_reading(reading);
}

int main(void) {
    const struct device *dev;

    dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Sensor device not ready\n");
        return 0;
    }

    init_nvs();

    while (1) {
        read_sensor(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
