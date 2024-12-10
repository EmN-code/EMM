
#include "esp_system.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO           (GPIO_NUM_22)
#define I2C_MASTER_SDA_IO           (GPIO_NUM_21)
#define I2C_MASTER_NUM              (I2C_NUM_0)
#define I2C_MASTER_FREQ_HZ          (100000)
#define MMA8X5X_I2C_ADDRESS         (0x1D)

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mma8x5x_data_t;

typedef struct {
    i2c_port_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
    uint8_t offset[3];
} mma8x5x_params_t;

typedef struct {
    mma8x5x_params_t params;
} mma8x5x_t;

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params);
void mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data);

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    dev->params = *params;
    return 0;
}

void mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data) {
    uint8_t buffer[6];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev->params.addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x01, true); // Assuming 0x01 is the register to start reading from
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev->params.addr << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, buffer, 6, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    data->x = (buffer[0] << 8) | buffer[1];
    data->y = (buffer[2] << 8) | buffer[3];
    data->z = (buffer[4] << 8) | buffer[5];
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_MASTER_NUM,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = 50,
        .range = 2,
        .offset = {0, 0, 0}
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        while (1) {
            // Handle initialization error
        }
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        // Process data
        vTaskDelay(5000 / portTICK_PERIOD_MS); // Delay for 5 seconds
    }

    return 0;
}
