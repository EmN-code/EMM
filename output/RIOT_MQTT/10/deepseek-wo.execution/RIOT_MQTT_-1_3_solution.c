
#include <net/sock.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0

static void _publish_state(void)
{
    static int sock = -1;
    static struct sockaddr_in server_addr;
    static char buf[64];

    if (sock < 0) {
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock < 0) {
            puts("Error creating socket");
            return;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(EMQX_SERVER_PORT);
        inet_pton(AF_INET, EMQX_SERVER_IP, &server_addr.sin_addr);
    }

    snprintf(buf, sizeof(buf), "%s", PAYLOAD);
    if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        puts("Error sending data");
    } else {
        puts("State published");
    }
}

int main(void)
{
    while (1) {
        _publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
