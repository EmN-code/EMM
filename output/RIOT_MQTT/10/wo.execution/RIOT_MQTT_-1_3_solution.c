// import headers in descending alphabetical order
#include "thread.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "net/gcoap.h"
#include "event/thread.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "MQTTClient.h"

// Macros for the MQTT Broker IP and Port
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

// MQTT topic and message
#define MQTT_TOPIC "device/state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"

// Thread stack configuration
#define STACKSIZE 2048
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];

// Function to send state to the MQTT Broker
void *mqtt_sender(void *arg) {
    (void)arg;  // suppress unused warning
    int sock;
    struct sockaddr_in dest;
    char *message;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("unable to create socket");
        return NULL;
    }

    // Configure broker address
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &dest.sin_addr);

    // Simulation loop to send work/not work state
    while (1) {
        // Alternate between sending work / not work message
        static int state = 0;
        message = state ? MQTT_MESSAGE_WORK : MQTT_MESSAGE_NOT_WORK;
        state = !state;

        // Send message
        printf("Sending message: %s\n", message);
        if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
            perror("sendto");
        }

        // Sleep for 5 seconds
        sleep(5);
    }
}

int main(void) {
    // Create a thread for the MQTT sender
    thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0,
                  mqtt_sender, NULL, "mqtt_sender");

    return 0;
}