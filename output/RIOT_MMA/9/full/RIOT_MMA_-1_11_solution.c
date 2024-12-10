#include <net/sock/util.h>
#include <stdatomic.h>
#include <MQTTClient.h>
#include <net/gcoap.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <ztimer.h>
#include <msg.h>
#include <dht_params.h>
#include <esp_system.h>
#include <timex.h>
#include <dht.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <esp_wifi.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <sys/socket.h>
#include <periph/i2c.h>
#include <net/sock/udp.h>
#include <periph_conf.h>
#include <architecture.h>
#include <mutex.h>
#include <net/emcute.h>
#include <stdlib.h>
#include <xtimer.h>
#include <mma8x5x_params.h>
#include <thread.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <event/thread.h>

// Define the device descriptor and data structure
static mma8x5x_t dev;
mma8x5x_data_t data;

// Configuration parameters
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *sensor_thread(void *arg) {
    // Initialize sensor
    while (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Sensor initialization failed, retrying...\n");
        sleep(1);
    }
    printf("Sensor initialized successfully.\n");

    // Periodically read sensor data
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Read accelerometer data: X: %d mg, Y: %d mg, Z: %d mg\n", 
               data.x_axis, data.y_axis, data.z_axis);
        sleep(5);
    }

    return NULL;
}

int main(void) {
    // Create a thread to handle sensor reading
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0,
                  sensor_thread, NULL, "sensor_thread");

    while (1) {
        // Main loop can handle other tasks or just sleep
        sleep(60);
    }

    return 0;
}