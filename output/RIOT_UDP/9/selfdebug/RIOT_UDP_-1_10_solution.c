#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void){
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };
    uint8_t buf[128];
    strcpy((char *)buf, "work");
    
    inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6);

    while (1) {
        ssize_t res = sock_udp_send(NULL, buf, sizeof(buf), &remote);
        if (res < 0) {
            puts("Error sending message");
        }
        xtimer_sleep(INTERVAL);
    }
    return 0;
}