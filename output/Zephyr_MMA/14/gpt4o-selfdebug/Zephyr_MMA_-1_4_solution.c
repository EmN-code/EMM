#include <zephyr/storage/flash_map.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

/* Check if the MMA8652 is enabled in the device tree */
#if DT_NODE_HAS_STATUS(DT_INST(0, nxp_mma8652), okay)
#define MMA8652_NODE DT_INST(0, nxp_mma8652)
#else
#error "Unsupported board: nxp, mma8652 device not defined"
#define MMA8652_NODE DT_INVALID_NODE
#endif

/* Importing NVS header for storage handling */
#include <zephyr/nvs/nvs.h>

/* Define the stacking for the stack_space thread attribute */
#define STACK_SIZE 1024
#define THREAD_PRIORITY 7

struct nvs_fs fs;

void mma8652_read_and_store(struct k_work *work)
{
    const struct device *sensor = DEVICE_DT_GET(MMA8652_NODE);
    if (!device_is_ready(sensor)) {
        LOG_ERR("Device %s is not ready", sensor->name);
        return;
    }

    struct sensor_value val;
    if (sensor_sample_fetch(sensor) < 0) {
        LOG_ERR("Failed to fetch sample from MMA8652 sensor");
        return;
    }

    if (sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &val) < 0) {
        LOG_ERR("Failed to get sensor data");
        return;
    }

    LOG_INF("Acceleration (m/s^2): X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            val.val1, val.val2,
            val.val1, val.val2,
            val.val1, val.val2);

    /* Writing value to NVS */
    int ret;
    ret = nvs_write(&fs, 1, &val, sizeof(val));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS");
    }
}

void main(void)
{
    int err;
    LOG_INF("MMA8652 Sensor Reading");

    /* Initialize the NVS */
    struct flash_area *fa;

    err = flash_area_open(FLASH_AREA_ID(storage), &fa);
    if (err) {
        LOG_ERR("Flash area open failed");
        return;
    }

    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 2;

    k_work_init_delayable(&mma8652_work, mma8652_read_and_store);
    k_work_schedule(&mma8652_work, K_SECONDS(5));
}

K_THREAD_STACK_DEFINE(stack_space, STACK_SIZE);
k_tid_t my_tid = k_thread_create(&my_thread_data, stack_space, STACK_SIZE,
                    mma8652_read_and_store, NULL, NULL, NULL,
                    THREAD_PRIORITY, 0, K_SECONDS(5));