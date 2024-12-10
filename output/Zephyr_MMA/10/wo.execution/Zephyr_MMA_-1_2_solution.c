
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8x5x_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_FLASH_AREA_LABEL(storage)

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_area *fa;

    err = flash_area_open(DT_FLASH_AREA_ID(storage), &fa);
    if (err) {
        printk("Failed to open flash area\n");
        return;
    }

    fs.flash_device = fa->fa_device;
    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 3;

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        printk("Failed to init NVS\n");
        return;
    }

    printk("NVS initialized\n");
}

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int err;

    err = sensor_sample_fetch(dev);
    if (err) {
        printk("Failed to fetch sample\n");
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        printk("Failed to get sensor data\n");
        return;
    }

    printk("Accel: x=%d.%06d, y=%d.%06d, z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the read value persistently in NVS
    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    err = nvs_write(&fs, 1, data, sizeof(data));
    if (err < 0) {
        printk("Failed to write to NVS\n");
    } else {
        printk("Data written to NVS\n");
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_NODE);

    if (!device_is_ready(dev)) {
        printk("Device not ready\n");
        return 0;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
