#include <stdlib.h>  // Standard library for dynamic memory allocation, program utilities, etc.
#include <event/thread.h>  // RIOT OS specific library for event threading 
#include <thread.h> // For threading
#include <stdio.h> // Standard I/O operations
#include <MQTTClient.h> // MQTT client header for interacting with MQTT broker

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "device/state"
#define MQTT_CLIENT_THREAD_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char mqtt_client_stack[MQTT_CLIENT_THREAD_STACK_SIZE];
static mqtt_context_t mqtt_ctx;

void configure_mqtt(mqtt_context_t *mqtt_ctx) {
    memset(mqtt_ctx->client_id, 0, sizeof(mqtt_ctx->client_id));
    snprintf(mqtt_ctx->client_id, sizeof(mqtt_ctx->client_id), "esp32_client");
    mqtt_ctx->timeout = 5;
}

void *mqtt_client_thread(void *arg) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    while (1) {
        int rc;
        // Initialize the network
        NetworkInit(&network);

        // Attempt to connect to the network
        while((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
        }
        printf("Network connection successful.\n");

        // Initialize MQTT client
        MQTTClientInit(&client, &network, 1000, mqtt_ctx.buffer, sizeof(mqtt_ctx.buffer), mqtt_ctx.readbuf, sizeof(mqtt_ctx.readbuf));
        options.keepAliveInterval = 20;
        options.cleansession = 1;

        // Try to connect to the MQTT broker
        while ((rc = MQTTConnect(&client, &options)) != 0) {
            printf("MQTT connect failed, retrying...\n");
            xtimer_sleep(1);
        }
        printf("MQTT connection successful.\n");

        // Create the message to publish
        MQTTMessage message;
        message.qos = 1;
        message.retained = 0;
        message.payload = (void *)"work or not work";
        message.payloadlen = strlen((const char *)message.payload);

        // Publish message every 5 seconds
        while (1) {
            printf("Publishing message to topic %s\n", MQTT_PUBLISH_TOPIC);
            rc = MQTTPublish(&client, MQTT_PUBLISH_TOPIC, &message);
            if (rc != 0) {
                printf("Error publishing message, disconnecting...\n");
                break;
            }
            xtimer_sleep(5);
        }

        printf("Disconnecting and attempting reconnect...\n");
        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }
    return NULL;
}

int main(void) {
    configure_mqtt(&mqtt_ctx);
    thread_create(mqtt_client_stack, sizeof(mqtt_client_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_client_thread, NULL, "mqtt_client_thread");
    return 0;
}