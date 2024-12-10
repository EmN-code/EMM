
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_INST(0, nxp_mma8x5x)
#if DT_NODE_HAS_STATUS(MMA8X5X_NODE, okay)
#define MMA8X5X_DEV_NAME DT_LABEL(MMA8X5X_NODE)
#else
#error "MMA8x5x device is not defined in the device tree"
#endif

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    int ret;
    struct flash_sector *sectors;
    uint32_t sector_count;

    sectors = flash_get_sectors(NVS_PARTITION_DEVICE, &sector_count);
    if (!sectors) {
        printk("Failed to get flash sectors\n");
        return;
    }

    fs.flash_device = NVS_PARTITION_DEVICE;
    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);
    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = sector_count;

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return;
    }
}

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        printk("Sensor sample fetch failed: %d\n", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Sensor channel get failed: %d\n", ret);
        return;
    }

    printk("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the read value persistently in NVS
    uint8_t data[3];
    data[0] = accel[0].val1;
    data[1] = accel[1].val1;
    data[2] = accel[2].val1;

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void) {
    const struct device *dev;

    dev = device_get_binding(MMA8X5X_DEV_NAME);
    if (!dev) {
        printk("Failed to get MMA8x5x device\n");
        return -1;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
