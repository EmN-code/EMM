#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/nvs.h>

LOG_MODULE_REGISTER(main);

static const struct device *mma8x5x_dev;
static struct nvs_fs nvs;
#define STORAGE_PARTITION_LABEL "storage"
#define NVS_ID 1

void init_nvs(void) {
    struct flash_pages_info info;
    nvs.offset = DT_FLASH_AREA_OFFSET(storage);
    flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), nvs.offset, &info);
    nvs.sector_size = info.size;
    nvs.sector_count = 3U;
    nvs_mount(&nvs);
}

void read_and_store_acc(void) {
    struct sensor_value accel[3];
    if (sensor_sample_fetch(mma8x5x_dev) < 0) {
        LOG_ERR("Sensor sample update error");
        return;
    }
    sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    LOG_INF("X: %d, Y: %d, Z: %d", accel[0].val1, accel[1].val1, accel[2].val1);
    nvs_write(&nvs, NVS_ID, &accel[0].val1, sizeof(int));
}

void main(void) {
    mma8x5x_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!mma8x5x_dev) {
        LOG_ERR("No device found.");
        return;
    }
    init_nvs();
    while (1) {
        read_and_store_acc();
        k_sleep(K_SECONDS(5));
    }
}