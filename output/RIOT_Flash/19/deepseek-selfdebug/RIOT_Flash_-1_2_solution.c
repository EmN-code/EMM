
#include "nvs.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "log.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define TEMP_SENSOR_I2C_DEV I2C_DEV(0)
#define TEMP_SENSOR_ADDR 0x1D
#define TEMP_SENSOR_RATE MMA8X5X_RATE_50HZ
#define TEMP_SENSOR_RANGE MMA8X5X_RANGE_2G

static nvs_handle_t nvs_handle;
static mma8x5x_t temp_sensor;

void read_temperature(void) {
    mma8x5x_data_t data;
    mma8x5x_read(&temp_sensor, &data);
    int temperature = (data.x + data.y + data.z) / 3; // Simplified temperature calculation

    int state = (temperature > 25) ? 1 : 0; // Assuming 25 as the threshold

    nvs_set_u8(nvs_handle, NVS_KEY, state);
    nvs_commit(nvs_handle);

    LOG_INFO("Temperature: %d, State: %d\n", temperature, state);
}

int main(void) {
    int err;

    // Initialize NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != NVS_OK) {
        LOG_ERROR("NVS open failed: %d\n", err);
        return 1;
    }

    // Initialize I2C
    i2c_init(TEMP_SENSOR_I2C_DEV);

    // Initialize temperature sensor
    mma8x5x_params_t params = {
        .i2c = TEMP_SENSOR_I2C_DEV,
        .addr = TEMP_SENSOR_ADDR,
        .rate = TEMP_SENSOR_RATE,
        .range = TEMP_SENSOR_RANGE
    };
    err = mma8x5x_init(&temp_sensor, &params);
    if (err != MMA8X5X_OK) {
        LOG_ERROR("Temperature sensor initialization failed: %d\n", err);
        return 1;
    }

    // Periodically read temperature and update NVS
    while (1) {
        read_temperature();
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}
