
#include <stddef.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <mutex.h>
#include <dht.h>
#include <thread.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <time_units.h>
#include <MQTTClient.h>
#include <sys/socket.h>
#include <mbox.h>
#include <nvs_flash.h>
#include <periph/rtc.h>
#include <timex.h>
#include <stdio.h>
#include <periph/i2c.h>
#include <shell.h>
#include <esp_system.h>
#include <periph/gpio.h>

#define MMA8X5X_I2C_ADDRESS 0x1C
#define MMA8X5X_RATE_50HZ 0x06
#define MMA8X5X_RANGE_2G 0x00

typedef struct {
    i2c_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
} mma8x5x_params_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mma8x5x_data_t;

typedef struct {
    mma8x5x_params_t params;
    mutex_t lock;
} mma8x5x_t;

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params) {
    mutex_init(&dev->lock);
    dev->params = *params;
    return 0;
}

void mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data) {
    // Dummy implementation for reading data
    data->x = 100;
    data->y = 200;
    data->z = 300;
}

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_accelerometer(void) {
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        sleep(5);
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mma8x5x_init(&dev, &params);

    read_accelerometer();

    return 0;
}
