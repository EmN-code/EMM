#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 1883
#define MSG_INTERVAL (5U * 1000000U)  // 5 seconds

int main(void) {
    sock_udp_ep_t server_endpoint = { .family = AF_INET,
                                       .port = SERVER_PORT };
    uint8_t buf[128];
    size_t msg_len;
    char *state_msg = "work or not work";

    // Initialize address
    int res = sock_udp_str2ep(&server_endpoint, SERVER_IP);
    if (res < 0) {
        printf("Error: unable to resolve server address\n");
        return 1;
    }

    // Create UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create socket\n");
        return 1;
    }

    while (1) {
        // Prepare the message
        msg_len = snprintf((char *)buf, sizeof(buf), "%s", state_msg);

        // Send the message
        if (sock_udp_send(&sock, buf, msg_len, &server_endpoint) < 0) {
            printf("Error: unable to send message\n");
            return 1;
        } else {
            printf("Message sent successfully\n");
        }

        // Wait a specified interval
        xtimer_usleep(MSG_INTERVAL);
    }

    // Close socket
    sock_udp_close(&sock);
    return 0;
}