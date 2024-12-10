#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_params.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "time_units.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 64
#define SEND_INTERVAL_SEC 5

void *udp_sender(void *arg) {
    (void)arg;  // Unused parameter

    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int counter = 0;
    char *state;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        return NULL;
    }

    // Set up server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        // Alternate state between "work" and "not work"
        if (counter % 2 == 0) {
            state = "work";
        } else {
            state = "not work";
        }

        // Copy state into buffer
        strncpy(buffer, state, BUFFER_SIZE - 1);
        buffer[BUFFER_SIZE - 1] = '\0';  // Ensure null-termination

        // Send the state to the server
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("sendto failed");
        } else {
            printf("Sent: %s\n", buffer);
        }

        // Increment counter for next state
        counter++;

        // Sleep for the defined interval
        sleep(SEND_INTERVAL_SEC);
    }

    // Close the socket
    close(sock);
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, udp_sender, NULL, "udp_sender");

    // Keep the main thread alive to avoid program termination
    while (1) {
        sleep(60);
    }

    return 0;
}