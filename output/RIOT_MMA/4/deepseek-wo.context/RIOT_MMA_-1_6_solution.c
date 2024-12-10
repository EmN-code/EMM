
#include <stdlib.h>
#include <esp_wifi.h>
#include <kernel_defines.h>
#include <sys/socket.h>
#include <MQTTClient.h>
#include <nvs_flash.h>
#include <esp_system.h>

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define I2C_DEV(x) (x)

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mma8x5x_data_t;

typedef struct {
    int i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
} mma8x5x_params_t;

typedef struct {
    mma8x5x_params_t params;
} mma8x5x_t;

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params);
void mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data);

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = 50, // Assuming MMA8X5X_RATE_50HZ is 50
        .range = 2  // Assuming MMA8X5X_RANGE_2G is 2
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        // Handle initialization error
        return -1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        // Process data as needed

        // Sleep for 5 seconds
        sleep(5);
    }

    return 0;
}
