
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFF_LEN 512
#define DELAY_MS (5 * 1000000U)  // 5 seconds in microseconds

static void udp_send_state(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    char buf[BUFF_LEN];
    while (1) {
        strcpy(buf, "work");  // Assuming the device is working
        printf("Sending: %s\n", buf);
        sock_udp_send(&sock, buf, strlen(buf), NULL);

        xtimer_usleep(DELAY_MS);  // Sleep for 5 seconds
    }

    sock_udp_close(&sock);
}

int main(void)
{
    puts("UDP client starting...");
    udp_send_state();
    return 0;
}
