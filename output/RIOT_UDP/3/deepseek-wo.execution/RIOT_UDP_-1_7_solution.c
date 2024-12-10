
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
#include <esp_system.h>
#include <mbox.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <evtimer_msg.h>
#include <stddef.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <dht_params.h>
#include <xtimer.h>
#include <net/sock.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * US_PER_SEC)

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[10];

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        puts("Failed to create socket");
        return 1;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        puts("Invalid address/ Address not supported");
        close(sock);
        return 1;
    }

    // Main loop to send device state every 5 seconds
    while (1) {
        // Prepare the message
        snprintf(buffer, sizeof(buffer), "%s", DEVICE_STATE);

        // Send the message
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            puts("Failed to send message");
        } else {
            puts("Message sent");
        }

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    // Close the socket
    close(sock);

    return 0;
}
