#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t remote = { .port = SERVER_PORT };
    uint8_t buf[128];
    const char *message = "Device state: work";
    size_t len = strlen(message);
    strcpy((char *) buf, message);
    
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    
    while (1) {
        if (sock_udp_send(NULL, buf, len, &remote) < 0) {
            puts("Error sending message");
        }
        else {
            puts("Message sent successfully!");
        }
        xtimer_sleep(INTERVAL);
    }
    return 0;
}
