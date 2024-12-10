#include "udp.h"
#include "esp_system.h"
#include "net/sock/util.h"
#include "mutex.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define WAIT_TIME_US ((5U) * (1000U * 1000U))

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Resolve the server address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Could not resolve server address\n");
        return 1;
    }

    // Create the UDP socket
    while (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        vTaskDelay(pdMS_TO_TICKS(WAIT_TIME_US));
    }

    // Continuous loop to send data every 5 seconds
    while (1) {
        // Send the message
        ssize_t res = sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote);

        if (res < 0) {
            printf("Error: Failed to send message\n");
        } else {
            printf("Message sent successfully\n");
        }

        // Delay for 5 seconds
        ets_delay_us(WAIT_TIME_US);
    }

    // Close the socket (not reached in this endless loop)
    // sock_udp_close(&sock);

    return 0;
}
