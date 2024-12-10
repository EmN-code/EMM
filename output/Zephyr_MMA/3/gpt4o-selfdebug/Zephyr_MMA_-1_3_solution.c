#include <zephyr/types.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_mgmt.h>

#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>

#define SENSOR_NODE DT_ALIAS(mma8x5x)
#define STORAGE_NODE DT_NODELABEL(storage)

static struct nvs_fs fs;

void main(void) {
    const struct device *sensor = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!sensor) {
        printk("Failed to get sensor device binding\n");
        return;
    }

    /* NVS initialization */
    struct flash_pages_info info;
    fs.offset = FLASH_AREA_OFFSET(storage);
    flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER), fs.offset, &info);
    fs.sector_size = info.size;
    fs.sector_count = 3U;
    nvs_init(&fs, DT_LABEL(STORAGE_NODE));

    struct sensor_value accel[3];
    while (1) {
        if (sensor_sample_fetch(sensor) < 0) {
            printk("Sensor sample update error\n");
            continue;
        }

        sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
        printk("Accel (m/s²): X=%d, Y=%d, Z=%d\n", accel[0].val1, accel[1].val1, accel[2].val1);

        /* Store to NVS */
        uint16_t id = 1;  // Example id
        nvs_write(&fs, id, accel, sizeof(accel));

        k_sleep(K_SECONDS(5));
    }
}