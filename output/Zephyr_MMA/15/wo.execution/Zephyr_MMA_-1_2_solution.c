
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION   DT_FLASH_AREA_STORAGE_ID

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_area *fa;

    fs.offset = FLASH_AREA_OFFSET(storage);
    err = flash_area_open(NVS_PARTITION, &fa);
    if (err) {
        printk("Error opening flash area\n");
        return;
    }

    fs.sector_size = fa->fa_size;
    fs.sector_count = 1;

    err = nvs_mount(&fs);
    if (err) {
        printk("Error mounting NVS\n");
        return;
    }
}

void store_value(uint16_t id, uint32_t val) {
    int err;

    err = nvs_write(&fs, id, &val, sizeof(val));
    if (err < 0) {
        printk("Error writing to NVS\n");
    } else {
        printk("Value stored successfully\n");
    }
}

int main(void) {
    const struct device *dev;
    struct sensor_value accel[3];
    int ret;

    dev = DEVICE_DT_GET_ANY(nxp_mma8x5x);
    if (!dev) {
        printk("No MMA8x5x device found\n");
        return 0;
    }

    if (!device_is_ready(dev)) {
        printk("MMA8x5x device not ready\n");
        return 0;
    }

    init_nvs();

    while (1) {
        ret = sensor_sample_fetch(dev);
        if (ret) {
            printk("Sensor sample fetch failed\n");
            continue;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret) {
            printk("Sensor channel get failed\n");
            continue;
        }

        printk("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
               accel[0].val1, accel[0].val2,
               accel[1].val1, accel[1].val2,
               accel[2].val1, accel[2].val2);

        store_value(1, accel[0].val1);
        store_value(2, accel[1].val1);
        store_value(3, accel[2].val1);

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
