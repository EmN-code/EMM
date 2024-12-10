#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include
#include "socket_network.h" // Hypothetical header for network operations
#include "mqtt_protocol.h"  // Hypothetical header for MQTT protocols

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define RECONNECT_INTERVAL 5

void* publish_state(void *arg) {
    int sockfd;
    struct sockaddr_in broker_addr;
    char work_state[] = "work";
    char not_work_state[] = "not work";
    int state = 1; // Flag to alternate states

    while (1) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Socket creation failed");
            continue;
        }

        memset(&broker_addr, 0, sizeof(broker_addr));
        broker_addr.sin_family = AF_INET;
        broker_addr.sin_port = htons(BROKER_PORT);
        broker_addr.sin_addr.s_addr = inet_addr(BROKER_IP);

        if (connect(sockfd, (struct sockaddr*)&broker_addr, sizeof(broker_addr)) < 0) {
            perror("Connection to broker failed");
            close(sockfd);
            sleep(RECONNECT_INTERVAL);
            continue;
        }

        const char *state_message = state ? work_state : not_work_state;

        if (send(sockfd, state_message, strlen(state_message), 0) < 0) {
            perror("MQTT Publish failed");
        }

        close(sockfd);

        state = !state; // Toggle state

        sleep(5); // Wait for 5 seconds before sending another message
    }

    return NULL;
}

int main() {
    pthread_t mqtt_thread;
    if (pthread_create(&mqtt_thread, NULL, publish_state, NULL) != 0) {
        fprintf(stderr, "Failed to create MQTT publish thread\n");
        return EXIT_FAILURE;
    }

    if (pthread_join(mqtt_thread, NULL) != 0) {
        fprintf(stderr, "Failed to join MQTT publish thread\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}