
#include <architecture.h>
#include <stddef.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <mma8x5x.h>
#include <netinet/in.h>
#include <mutex.h>
#include <evtimer_mbox.h>
#include <freertos/task.h>
#include <net/emcute.h>
#include <dht.h>
#include <esp_wifi.h>
#include <thread.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <unistd.h>
#include <freertos/FreeRTOS.h>
#include <net/sock/udp.h>
#include <assert.h>
#include <stdatomic.h>
#include <string.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <event/thread.h>
#include <MQTTClient.h>
#include <mbox.h>
#include <nvs_flash.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <periph/rtc.h>
#include <timex.h>
#include <nvs.h>
#include <stdio.h>
#include <periph/i2c.h>
#include <stdbool.h>
#include <shell.h>
#include <esp_system.h>
#include <msg.h>
#include <ztimer.h>
#include <event/periodic_callback.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <paho_mqtt.h>
#include <net/sock/util.h>

#define MMA8X5X_I2C_ADDRESS (0x1D)
#define MMA8X5X_RATE_50HZ (0x06)
#define MMA8X5X_RANGE_2G (0x00)

static mma8x5x_t dev;
static mma8x5x_data_t data;

static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res;

    // Initialize the MMA8x5x accelerometer
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed with error code %d\n", res);
        return 1;
    }

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the accelerometer data
        printf("Accelerometer data: x = %d, y = %d, z = %d\n", data.x, data.y, data.z);

        // Wait for 5 seconds
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    return 0;
}
