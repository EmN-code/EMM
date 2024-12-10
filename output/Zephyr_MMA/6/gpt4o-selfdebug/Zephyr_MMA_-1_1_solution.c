#include <zephyr.h>
#include <sys/printk.h>
#include <nvs/nvs.h>
#include <drivers/sensor.h>

#define SENSOR_THREAD_STACK_SIZE 1024
#define SENSOR_THREAD_PRIORITY 5
#define SENSOR_READ_INTERVAL K_SECONDS(5)
#define NVS_STORAGE_ID 1

struct nvs_fs fs;
static struct sensor_value acceleration[3];

void mma8x5x_read_and_store(void) {
    const struct device *sensor = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!sensor) {
        printk("Sensor device not found\n");
        return;
    }

    if (sensor_sample_fetch(sensor) < 0 ||
        sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, acceleration) < 0) {
        printk("Failed to fetch sensor data\n");
        return;
    }
    
    printk("X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
           acceleration[0].val1, acceleration[0].val2,
           acceleration[1].val1, acceleration[1].val2,
           acceleration[2].val1, acceleration[2].val2);

    int rc = nvs_write(&fs, NVS_STORAGE_ID, &acceleration,
                       sizeof(acceleration));
    if (rc < 0) {
        printk("Failed to write data to NVS\n");
    } else {
        printk("Data stored in NVS successfully\n");
    }
}

void sensor_thread(void) {
    while (1) {
        mma8x5x_read_and_store();
        k_sleep(SENSOR_READ_INTERVAL);
    }
}

void main(void) {
    struct flash_pages_info info;
    fs.offset = FLASH_AREA_OFFSET(storage);

    if (flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER),
                                    fs.offset, &info)) {
        printk("Unable to get flash page info\n");
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = 3U;

    if (nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER)) {
        printk("NVS initialization failed\n");
        return;
    }

    k_thread_stack_define(sensor_thread_stack_area, SENSOR_THREAD_STACK_SIZE);
    static struct k_thread sensor_thread_data;
    k_thread_create(&sensor_thread_data, sensor_thread_stack_area,
                    K_THREAD_STACK_SIZEOF(sensor_thread_stack_area),
                    (k_thread_entry_t)sensor_thread, NULL, NULL, NULL,
                    SENSOR_THREAD_PRIORITY, 0, K_NO_WAIT);
}
