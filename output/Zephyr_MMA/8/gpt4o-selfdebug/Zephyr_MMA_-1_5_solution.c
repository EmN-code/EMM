#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/linker/sections.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define STORAGE_OFFSET 0x0000
#define STORAGE_SIZE   0x1000
#define NVS_FLASH_DEVICE DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL
#define NVS_DATA_ID      1

LOG_MODULE_REGISTER(main);

static struct nvs_fs nvs;
struct flash_pages_info info;

int main() {
    struct device *sensor = device_get_binding(DT_LABEL(DT_NODELABEL(mma8652fc)));
    struct sensor_value accel[3];
    if (!sensor) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    nvs.offset = STORAGE_OFFSET;
    nvs.sector_size = STORAGE_SIZE;
    nvs.sector_count = 3;
    if (nvs_mount(&nvs)) {
        LOG_ERR("Flash Init failed");
        return -1;
    }

    while (1) {
        if (sensor_sample_fetch(sensor)) {
            LOG_ERR("Sensor sample fetch error");
            continue;
        }
        if (sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel)) {
            LOG_ERR("Sensor channel get error");
            continue;
        } else {
            LOG_INF("Acceleration: X: %d, Y: %d, Z: %d", accel[0].val1, accel[1].val1, accel[2].val1);
        }

        if (nvs_write(&nvs, NVS_DATA_ID, &accel, sizeof(accel)) < 0) {
            LOG_ERR("NVS write error");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
