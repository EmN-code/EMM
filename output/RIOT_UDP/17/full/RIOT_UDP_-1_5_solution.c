#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    char addr_str[40];
    int res;

    // Convert and validate the IP address
    while (1) {
        res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
        if (res != 1) {
            printf("Error: unable to convert server IP\n");
            xtimer_sleep(1);
            continue;
        }
        break;
    }
    
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        // Send the message
        res = sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote);
        
        // Check if the message was sent successfully
        if (res < 0) {
            printf("Error sending data over UDP\n");
        } else {
            printf("Message sent: %s\n", MESSAGE);
        }

        // Wait for the interval to send the next message
        xtimer_usleep(INTERVAL);
    }

    // Normally we would use sock_udp_close, but this will never be reached in our case
    return 0;
}
