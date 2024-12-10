
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8X5X_NODE DT_INST(0, nxp_mma8x5x)
#define FLASH_DEVICE_NODE DT_CHOSEN(zephyr_flash_controller)
#define NVS_PARTITION DT_LABEL(DT_CHOSEN(zephyr_flash_log))

#if DT_NODE_HAS_STATUS(MMA8X5X_NODE, okay)
#define MMA8X5X_DEV DEVICE_DT_GET(MMA8X5X_NODE)
#else
#error "No device found for MMA8X5X"
#endif

#if DT_NODE_HAS_STATUS(FLASH_DEVICE_NODE, okay)
#define FLASH_DEVICE DEVICE_DT_GET(FLASH_DEVICE_NODE)
#else
#error "No device found for flash"
#endif

#define NVS_SECTOR_COUNT 4
#define NVS_SECTOR_SIZE DT_REG_SIZE(NVS_PARTITION) / NVS_SECTOR_COUNT

static struct nvs_fs fs;

void init_nvs(void)
{
    int err;
    struct flash_sector sectors[NVS_SECTOR_COUNT];

    fs.flash_device = NVS_PARTITION;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    err = flash_get_sectors(FLASH_DEVICE, &fs.sector_count, sectors);
    if (err) {
        printk("Failed to get flash sectors: %d\n", err);
        return;
    }

    fs.offset = sectors[0].start;

    err = nvs_init(&fs, NVS_PARTITION);
    if (err) {
        printk("Failed to init NVS: %d\n", err);
        return;
    }
}

void read_sensor_and_store(void)
{
    const struct device *dev = MMA8X5X_DEV;
    struct sensor_value accel[3];
    int err;
    uint8_t data[3];

    if (!device_is_ready(dev)) {
        printk("MMA8X5X device not ready\n");
        return;
    }

    err = sensor_sample_fetch(dev);
    if (err) {
        printk("Failed to fetch sample: %d\n", err);
        return;
    }

    err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (err) {
        printk("Failed to get sensor data: %d\n", err);
        return;
    }

    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    err = nvs_write(&fs, 1, data, sizeof(data));
    if (err < 0) {
        printk("Failed to write to NVS: %d\n", err);
    } else {
        printk("Data written to NVS\n");
    }
}

int main(void)
{
    init_nvs();

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
