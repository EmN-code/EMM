#include <stdio.h>
#include <string.h>
#include "xtimer.h"  // For delay functionality
#include "thread.h"
#include "mqtt.h"
#include "mqtt_transport_net.h"
#include "net/sock/udp.h" // UDP functionalities

#define BROKER_IP                 "47.102.103.1"
#define MQTT_PORT                 1883
#define BUFFER_SIZE               128
#define PUBLISH_INTERVAL_MS       5000

static char mqtt_client_id[] = "esp32-mqtt-client";
static char topic[] = "state/status";
static sock_udp_ep_t broker_endpoint;
static char mqtt_send_buf[BUFFER_SIZE];
static char mqtt_read_buf[BUFFER_SIZE];

static void *publish_thread(void *arg) {
    (void)arg;
    mqtt_transport_net_t transport;
    mqtt_client_t client;
    mqtt_initialize_transport_net(&transport, &broker_endpoint);
    mqtt_init(&client, &mqtt_send_buf, BUFFER_SIZE, &mqtt_read_buf, BUFFER_SIZE, mqtt_transport_net_send, mqtt_transport_net_recv, NULL, &transport);
    
    printf("Connecting to MQTT broker at %s:%d...\n", BROKER_IP, MQTT_PORT);

    if (mqtt_connect(&client, mqtt_client_id, NULL, NULL, 0, 0, 0) < 0) {
        printf("Failed to connect to the broker\n");
        return NULL;
    }

    printf("Connected to MQTT broker!\n");

    char *message;
    int status = 1;

    while (1) {
        message = status ? "work" : "not work";
        printf("Publishing message: %s\n", message);

        if (mqtt_publish(&client, topic, message, strlen(message), MQTT_PUBLISH_QOS_0) < 0) {
            printf("Failed to publish message\n");
        }
        
        status ^= 1;  // Toggle work state
        xtimer_sleep(PUBLISH_INTERVAL_MS / 1000);
    }
    
    // Clean up
    mqtt_disconnect(&client);
    return NULL;
}

int main(void) {
    kernel_pid_t pid;

    // Set up broker endpoint
    broker_endpoint.family = AF_INET;
    broker_endpoint.netif = SOCK_ADDR_ANY_NETIF;
    broker_endpoint.port = MQTT_PORT;
    inet_pton(AF_INET, BROKER_IP, &broker_endpoint.addr.ipv4);

    // Start the publishing thread
    char publish_stack[THREAD_STACKSIZE_DEFAULT];
    pid = thread_create(publish_stack, sizeof(publish_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish_thread");

    thread_join(pid);

    return 0;
}
