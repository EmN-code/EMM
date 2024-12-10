#include "mutex.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/af.h"
#include "esp_system.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

static kernel_pid_t sender_thread_pid;

void *sender_thread(void *arg) {
    (void)arg; // Unused parameter
    int sock;
    struct sockaddr_in server_addr;
    char *message = "work";

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket");
        return NULL;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return NULL;
    }

    while (1) {
        if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("sendto");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    close(sock);
    return NULL;
}

int main(void) {
    puts("RIOT UDP client example");

    char sender_thread_stack[THREAD_STACKSIZE_MAIN];
    sender_thread_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                                      THREAD_PRIORITY_MAIN - 1, 0, sender_thread,
                                      NULL, "sender thread");

    return 0;
}