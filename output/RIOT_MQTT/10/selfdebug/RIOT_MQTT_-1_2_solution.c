#include <xtimer.h>  // Include for timer functionality
#include <stdio.h>   // Include for standard input/output
#include <string.h>  // Include for string operations
#include <net/sock.h> // Include for networking operations
#include <net/sock/udp.h> // Include for UDP sock operations
#include <net/mqtt.h>  // Include for MQTT operations

#define SERVER_IP "47.102.103.1" // MQTT Broker IP address
#define SERVER_PORT 1883 // MQTT Broker port
#define PUB_INTERVAL (5U * US_PER_SEC) // Publishing interval of 5 seconds

static mqtt_client_t mqtt_client;
static mqtt_connect_params_t connect_params;
static char mqtt_client_id[64];
static mqtt_publish_params_t publish_params;
static xtimer_t timer;

void publish_state(void)
{
    char *state = "work"; // State to be published
    publish_params.topic = "state_topic";
    publish_params.payload = state;
    publish_params.payload_len = strlen(state);
    publish_params.qos = MQTT_QOS_0;
    
    if (mqtt_publish(&mqtt_client, &publish_params) != MQTT_OK)
    {
        printf("Failed to publish message\n");
    }
    else
    {
        printf("Message published successfully\n");
    }
}

void* mqtt_thread_handler(void *arg)
{
    (void)arg;
    while (1)
    {
        publish_state();
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void)
{
    // Initialize network interface for ESP32
    gnrc_netif_init();

    // MQTT client setup
    snprintf(mqtt_client_id, sizeof(mqtt_client_id), "mqtt_client_%d", random_uint32());
    connect_params.client_id = mqtt_client_id;
    connect_params.keep_alive = 1000;

    if (mqtt_init(&mqtt_client, SERVER_IP, SERVER_PORT, &connect_params) != MQTT_OK)
    {
        printf("Failed to initialize MQTT client\n");
        return 1;
    }

    printf("MQTT client initialized successfully\n");

    // Start MQTT communication thread
    thread_create(mqtt_thread_handler, NULL, THREAD_PRIORITY, THREAD_STACK_SIZE_DEFAULT, "mqtt_thread");

    // Start the main thread loop
    while (1)
    {
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }

    return 0;
}