#include <arpa/inet.h>
#include <dht.h>
#include <dht_params.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <msg.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>

void *accelerometer_thread(void *arg) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        // Assuming the correct I2C address and device settings
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return NULL;
    }
    
    mma8x5x_data_t data;
    while (1) {
        // Read the data from the accelerometer
        mma8x5x_read(&dev, &data);
        printf("Accel - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Thread stack allocate
    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t accel_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, accelerometer_thread, NULL, "accel_thread");
    if (accel_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating accelerometer thread\n");
        return 1;
    }
    
    // Establishing network connection using MQTTClient
    Network network;
    int rc;
    MQTTClient client;

    NetworkInit(&network);

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if(rc != 0){
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to network successfully\n");
            break;
        }
    }

    MQTTClientInit(&client, &network, 1000, stack, THREAD_STACKSIZE_MAIN, stack, THREAD_STACKSIZE_MAIN);

    while (1) {
        rc = MQTTConnect(&client, &data);
        if (rc != 0) {
            printf("MQTT connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to MQTT successfully\n");
            break;
        }
    }

    return 0;
}