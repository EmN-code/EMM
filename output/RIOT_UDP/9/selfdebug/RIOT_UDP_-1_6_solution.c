// Include header files in descending alphabetical order by their first letter
#include "xtimer.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"  // UDP server IP
#define SERVER_PORT 20001          // UDP server port
#define SLEEP_INTERVAL_SEC 5       // Interval between sends in seconds

int main(void)
{
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }

    char *device_state_work = "work";
    char *device_state_not_work = "not work";
    char buffer[128];

    while (1) {
        // Prepare the message
        snprintf(buffer, sizeof(buffer), "Device state: %s", device_state_work);
        
        // Send message to the server
        if (sock_udp_send(&sock, buffer, strlen(buffer), NULL) < 0) {
            puts("Error sending message");
        } else {
            printf("Message '%s' sent to the server\n", buffer);
        }

        // Sleep for specified interval
        xtimer_sleep(SLEEP_INTERVAL_SEC);
    }

    return 0;
}
