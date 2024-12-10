#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <stdio.h>

#define SERVER_PORT (20001)
#define SEND_INTERVAL (5U)
#define SERVER_ADDR "47.102.103.1"

int main(void) {
    // Initialize address and socket
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);
    
    // Prepare the device state messages
    const char *work_message = "Device is working";
    const char *not_work_message = "Device is not working";
    bool is_working = true;
    
    while (1) {
        // Send the appropriate message based on the device state
        const char *message = is_working ? work_message : not_work_message;
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &server);
        if (res < 0) {
            printf("Error sending message: %d\n", res);
        } else {
            printf("Message sent: %s\n", message);
        }

        // Toggle device state (for demonstration purposes)
        is_working = !is_working;

        // Wait for the next interval
        xtimer_sleep(SEND_INTERVAL);
    }
    return 0;
}