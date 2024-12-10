#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <sock/udp.h>
#include <net/inet.h>  
#include <arpa/inet.h>  // Include inet_ntop for IP address conversion

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUF_SIZE 128
#define SEND_INTERVAL (5U * US_PER_SEC)  // sending interval 5 seconds

static const char* states[] = {"work", "not work"};

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    char addr_str[INET_ADDRSTRLEN];
    uint8_t buf[BUF_SIZE];
    int state_index = 0;  // start with "work"

    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("Could not convert IP address");
        return 1;
    }

    while (1) {
        snprintf((char *)buf, BUF_SIZE, "Device state: %s", states[state_index]);
        ssize_t res = sock_udp_send(NULL, buf, strlen((char *)buf), &server);

        if (res >= 0) {
            printf("Sent message: %s\n", buf);
        } else {
            printf("Error sending message\n");
            xtimer_sleep(1);
            continue;  // retry sending
        }

        state_index = (state_index + 1) % 2;  // alternate states
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}