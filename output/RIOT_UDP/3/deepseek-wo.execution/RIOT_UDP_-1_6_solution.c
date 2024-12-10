
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time_units.h>
#include <mutex.h>
#include <msg.h>
#include <timex.h>
#include <ztimer.h>
#include <mbox.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/sock.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <dht_params.h>
#include <dht.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MSG_QUEUE_SIZE 10

static msg_t _main_msg_queue[MSG_QUEUE_SIZE];

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char state[] = "work"; // or "not work"

    // Initialize the message queue
    msg_init_queue(_main_msg_queue, MSG_QUEUE_SIZE);

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        puts("Failed to create socket");
        return 1;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Send the device state every 5 seconds
    while (1) {
        ssize_t bytes_sent = sendto(sock, state, sizeof(state), 0,
                                    (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (bytes_sent < 0) {
            puts("Failed to send data");
        } else {
            printf("Sent device state: %s\n", state);
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    // Close the socket (this part is unreachable in the current loop)
    close(sock);

    return 0;
}
