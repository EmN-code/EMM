#include <zephyr/types.h>
#include <zephyr/sys/nvs.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define STORAGE_NAME "nvs"
#define MMA_SENSOR_NODE DT_NODELABEL(mma_sensor)
#define SENSOR_READ_INTERVAL K_SECONDS(5)
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 3

void read_sensor(const struct device *sensor_dev) {
    struct sensor_value sensor_value;

    if (sensor_sample_fetch(sensor_dev) < 0) {
        printk("Failed to fetch sensor data\n");
        return;
    }

    if (sensor_channel_get(sensor_dev, SENSOR_CHAN_ALL, &sensor_value) < 0) {
        printk("Failed to get sensor channel\n");
        return;
    }

    printk("Sensor value: %d.%06d\n", sensor_value.val1, sensor_value.val2);
}

int main() {
    const struct device *sensor_dev = device_get_binding(
        DT_LABEL(MMA_SENSOR_NODE)
    );

    if (!device_is_ready(sensor_dev)) {
        printk("Sensor device not found\n");
        return -1;
    }

    struct nvs_fs nvs;
    memset(&nvs, 0, sizeof(nvs));
    nvs.sector_size = NVS_SECTOR_SIZE;
    nvs.sector_count = NVS_SECTOR_COUNT;

    int rc = nvs_mount(&nvs);
    if (rc) {
        printk("NVS mount failed: %d\n", rc);
        return rc;
    }

    while (1) {
        read_sensor(sensor_dev);

        // Here you would save data to NVS if needed

        k_sleep(SENSOR_READ_INTERVAL);
    }

    return 0;
}
