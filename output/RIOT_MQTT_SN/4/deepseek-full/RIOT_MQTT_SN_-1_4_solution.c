
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <thread.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"
#define MESSAGE_LEN (sizeof(MESSAGE) - 1)
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

static void publish_state(void)
{
    while (1) {
        int res = emcute_pub(&topic, MESSAGE, MESSAGE_LEN, 0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish message: %d\n", res);
        } else {
            printf("Message published successfully\n");
        }
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }
}

int main(void)
{
    // Initialize the DHT sensor (example usage, not directly related to MQTT)
    dht_params_t dht_params = { .pin = GPIO_PIN(PORT_A, 0) };
    dht_t dht_dev;
    dht_init(&dht_dev, &dht_params);

    // Initialize the MMA8X5X accelerometer (example usage, not directly related to MQTT)
    mma8x5x_t mma8x5x_dev;
    mma8x5x_init(&mma8x5x_dev, I2C_DEV(0), MMA8X5X_I2C_ADDRESS, GPIO_PIN(PORT_A, 1), MMA8X5X_RANGE_2G);

    // Convert the IP address string to a byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Start the publish loop in a separate thread
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  publish_state, NULL, "publish_state");

    // Main loop (example usage, not directly related to MQTT)
    while (1) {
        // Example: Read data from the DHT sensor
        int16_t temp, hum;
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Example: Read data from the MMA8X5X accelerometer
        mma8x5x_data_t data;
        if (mma8x5x_read(&mma8x5x_dev, &data) == 0) {
            printf("Acceleration: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }

        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }

    return 0;
}
