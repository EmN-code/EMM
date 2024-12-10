
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <esp_system.h>

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
} mma8x5x_t;

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params);
void mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data);

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params) {
    dev->params = *params;
    return 0;
}

void mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data) {
    // Simulated data for demonstration purposes
    data->x = 100;
    data->y = 200;
    data->z = 300;
}

void accelerometer_task(void *pvParameters) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mma8x5x_init(&dev, &params);

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

int main(void) {
    i2c_init(I2C_DEV(0));
    xTaskCreate(accelerometer_task, "accelerometer_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    vTaskStartScheduler();
    return 0;
}
