#include <ztimer.h>
#include <stdio.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <msg.h>
#include <dht_params.h>
#include <timex.h>
#include <dht.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
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

#define I2C_DEV      (I2C_DEV(0))
#define I2C_ADDR     (0x1d) // Example address, replace this with actual
#define RATE         MMA8X5X_RATE_50HZ
#define RANGE        MMA8X5X_RANGE_2G
#define STACK_SIZE   (THREAD_STACKSIZE_MAIN)
#define MQTT_BROKER_IP "192.168.1.1" // Example IP
#define MQTT_BROKER_PORT 1883

static char stack[STACK_SIZE];
mma8x5x_t dev;
MQTTClient client;

void *mqtt_thread(void *arg) {
    (void)arg;
    Network network;
    int rc;
  
    printf("Connecting to network...\n");
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("Network connected\n");
        break;
    }

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "example_client_id";

    while (1) {
        rc = MQTTConnect(&client, &options);
        if (rc != 0) {
            printf("MQTT connect failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("MQTT connected\n");
        break;
    }

    return NULL;
}

void *sensor_thread(void *arg) {
    (void)arg;
    mma8x5x_data_t data;

    printf("Initializing sensor...\n");
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = I2C_ADDR,
        .rate = RATE,
        .range = RANGE
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        perror("Failed to initialize sensor\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        float ax = (float)data.x_axis * 1000.0 / 1024;
        float ay = (float)data.y_axis * 1000.0 / 1024;
        float az = (float)data.z_axis * 1000.0 / 1024;
        printf("Acceleration [mg]: x=%.2f y=%.2f z=%.2f\n", ax, ay, az);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  mqtt_thread, NULL, "mqtt_thread");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  sensor_thread, NULL, "sensor_thread");
    
    return 0;
}