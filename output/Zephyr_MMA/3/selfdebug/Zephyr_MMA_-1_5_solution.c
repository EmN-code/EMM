
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8x5x_NODE DT_N_INST_0_nxp_mma8x5x
#define MMA8x5x_DEV_NAME DT_LABEL(MMA8x5x_NODE)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION_ID)

void read_sensor_and_store(const struct device *dev)
{
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

    printk("Accel: x=%d.%06d, y=%d.%06d, z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the read value persistently in NVS
    struct nvs_fs fs;
    fs.sector_size = FLASH_AREA_OFFSET(storage);
    fs.sector_count = FLASH_AREA_SIZE(storage) / fs.sector_size;
    fs.offset = 0;

    ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return;
    }

    uint32_t data = (accel[0].val1 << 16) | (accel[1].val1 << 8) | accel[2].val1;
    ret = nvs_write(&fs, 1, &data, sizeof(data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void)
{
    const struct device *dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!dev) {
        printk("Could not get MMA8x5x device\n");
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
