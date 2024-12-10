#include "xtimer.h"
#include "saul_reg.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV         I2C_DEV(0)
#define ADDR            (0x1D) // Assuming default I2C address for MMA8653
#define TIMER_INTERVAL  (5U * US_PER_SEC)

static mma8x5x_t dev;

void read_sensor(void)
{
    mma8x5x_data_t data;
    mma8x5x_read(&dev, &data);
    printf("Acceleration: X=%dmg, Y=%dmg, Z=%dmg\n", data.x_axis, data.y_axis, data.z_axis);
}

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }

    while (1) {
        read_sensor();
        xtimer_sleep(TIMER_INTERVAL);
    }

    return 0;
}