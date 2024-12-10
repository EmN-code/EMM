
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8x5x_DEVICE_NODE DT_INST(0, nxp_mma8x5x)
#define MMA8x5x_LABEL DT_LABEL(MMA8x5x_DEVICE_NODE)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(storage_partition)

static struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION_ID),
    .sector_count = DT_REG_SIZE(NVS_PARTITION_ID) / DT_REG_SIZE(NVS_PARTITION_ID),
    .flash_device = (const struct device *)NVS_PARTITION_DEVICE,
    .offset = DT_REG_ADDR(NVS_PARTITION_ID),
};

void read_and_store_sensor_data(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        printk("Sensor fetch failed: %d\n", ret);
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
    ret = nvs_write(&nvs, 0, accel, sizeof(accel));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void) {
    const struct device *dev;
    int ret;

    dev = device_get_binding(MMA8x5x_LABEL);
    if (dev == NULL) {
        printk("Could not get MMA8x5x device\n");
        return -1;
    }

    ret = nvs_init(&nvs, NVS_PARTITION_DEVICE);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    while (1) {
        read_and_store_sensor_data(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
