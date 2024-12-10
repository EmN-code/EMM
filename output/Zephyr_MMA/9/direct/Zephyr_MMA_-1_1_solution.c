
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
    struct flash_sector_param sectors[1];

    sectors[0].fs_size = 4096;
    sectors[0].fs_addr = FLASH_AREA_OFFSET(storage_partition);

    fs.sector_size = 4096;
    fs.sector_count = 1;
    fs.offset = FLASH_AREA_OFFSET(storage_partition);

    int ret = nvs_init(&fs, "storage");
    if (ret) {
        printk("NVS init failed: %d\n", ret);
    }
}

void store_reading(int16_t reading) {
    int ret = nvs_write(&fs, 1, &reading, sizeof(reading));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    }
}

void read_sensor(const struct device *dev) {
    struct sensor_value accel[3];

    if (sensor_sample_fetch(dev)) {
        printk("Sensor sample fetch failed\n");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel)) {
        printk("Sensor channel get failed\n");
        return;
    }

    int16_t reading = (int16_t)(accel[0].val1);
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
