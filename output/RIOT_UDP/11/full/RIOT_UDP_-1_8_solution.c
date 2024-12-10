#include <pthread.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL 5

void *send_udp_message(void *arg) {
    (void)arg;  // Unused parameter

    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    while (1) {
        const char *message = "work or not work";
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);

        if (res < 0) {
            printf("Sending message failed\n");
        }
        else {
            printf("Message sent: %s\n", message);
        }

        sleep(MESSAGE_INTERVAL);
    }

    return NULL;
}

int main(void) {
    pthread_t udp_thread;

    if (pthread_create(&udp_thread, NULL, send_udp_message, NULL) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    pthread_join(udp_thread, NULL);

    return 0;
}
