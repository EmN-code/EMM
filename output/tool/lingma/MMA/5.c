#include "board.h"
#include "xtimer.h"
#include "mma8x5x.h"

static mma8x5x_t dev;

void init_sensor(void) {
    // Initialize the I2C bus
    i2c_init(I2C_DEV(0), I2C_SPEED_FAST);

    // Initialize the MMA8x5x sensor
    mma8x5x_init(&dev, I2C_DEV(0), MMA8X5X_ADDR0);
}

void read_sensor_data(void) {
    int16_t x, y, z;

    // Read the sensor data
    mma8x5x_read_acceleration(&dev, &x, &y, &z);

    // Print the sensor data
    printf("X: %d, Y: %d, Z: %d\n", x, y, z);
}

#define INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

void timer_callback(void *arg) {
    (void)arg;
    read_sensor_data();
}

void setup_timer(void) {
    // Set up the timer to call the callback every 5 seconds
    xtimer_setup(&timer, INTERVAL, timer_callback, NULL);
}

int main(void) {
    // Initialize the board
    board_init();

    // Initialize the sensor
    init_sensor();

    // Set up the timer
    setup_timer();

    // Main loop
    while (1) {
        xtimer_usleep(1000); // Sleep for 1 ms to avoid busy-waiting
    }

    return 0;
}

