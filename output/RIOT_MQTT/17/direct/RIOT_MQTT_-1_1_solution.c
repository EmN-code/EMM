#include "thread.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_ADDR  "47.102.103.1"
#define SERVER_PORT  1883
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

#define BUFFER_SIZE 64

static char message_buffer[BUFFER_SIZE];

static void *mqtt_client(void *arg) {
    (void)arg;

    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);

    while (1) {
        // Prepare the message
        const char *state = "work or not work";
        snprintf(message_buffer, BUFFER_SIZE, "PUB: %s", state);

        // Send the message
        size_t len = strlen(message_buffer);
        if (sock_udp_send(NULL, message_buffer, len, &server) < 0) {
            puts("Error sending message.");
        } else {
            puts("Message sent successfully.");
        }

        // Wait for the next period
        xtimer_usleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("MQTT Client is starting...");

    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_client, NULL, "mqtt_client");

    return 0;
}