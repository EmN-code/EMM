
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 4

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector sectors[NVS_SECTOR_COUNT];

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION);
    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    for (int i = 0; i < NVS_SECTOR_COUNT; i++) {
        sectors[i].fs_sector_size = NVS_SECTOR_SIZE;
        sectors[i].fs_sector_addr = fs.offset + i * NVS_SECTOR_SIZE;
    }

    err = nvs_mount(&fs);
    if (err) {
        LOG_ERR("NVS mount failed: %d", err);
    }
}

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];
    int ret;

    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, sizeof(data));
    if (ret) {
        LOG_ERR("Failed to read MMA8x5x: %d", ret);
        return;
    }

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];
}

void store_data_in_nvs(int16_t x, int16_t y, int16_t z) {
    int err;
    uint16_t id = 1;
    struct {
        int16_t x;
        int16_t y;
        int16_t z;
    } data = {x, y, z};

    err = nvs_write(&fs, id, &data, sizeof(data));
    if (err < 0) {
        LOG_ERR("NVS write failed: %d", err);
    } else {
        LOG_INF("Data stored in NVS");
    }
}

void main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return;
    }

    init_nvs();

    while (1) {
        int16_t x, y, z;
        read_mma8x5x(i2c_dev, &x, &y, &z);
        LOG_INF("MMA8x5x: x=%d, y=%d, z=%d", x, y, z);
        store_data_in_nvs(x, y, z);
        k_sleep(K_SECONDS(5));
    }
}
