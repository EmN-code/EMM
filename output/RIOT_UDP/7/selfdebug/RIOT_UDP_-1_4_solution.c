#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <inet.h>

#define MESSAGE_INTERVAL (5U) // Send a message every 5 seconds
#define SERVER_PORT (20001)

int main(void) {
    sock_udp_ep_t server = {.family = AF_INET, .port = SERVER_PORT};
    if (!inet_pton(AF_INET, "47.102.103.1", &server.addr)) {
        printf("Error: Failed to convert IP address\n");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        return -1;
    }

    const char *message_work = "work";
    const char *message_not_work = "not work";
    bool device_state = true; // Example state
    while (1) {
        const char *message = device_state ? message_work : message_not_work;
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            printf("Error: Unable to send message\n");
        }
        device_state = !device_state; // toggle state for demonstration
        xtimer_sleep(MESSAGE_INTERVAL);
    }
    return 0;
}