#include "xtimer.h"
#include "net/gnrc/udp.h"
#include "net/af.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5 * US_PER_SEC)
#define DEVICE_STATE "work"

int main(void) {
    kernel_pid_t ifs[GNRC_NETIF_NUMOF];
    size_t if_count = gnrc_netif_get(ifs);
    if (if_count == 0) {
        puts("No network interface found\n");
        return 1;
    }

    while (1) {
        /* Create the destination address struct */
        ipv6_addr_t server_addr;
        if (ipv6_addr_from_str(&server_addr, SERVER_ADDR) == NULL) {
            puts("Error: unable to parse server address\n");
            return 1;
        }

        gnrc_pktsnip_t *payload, *udp, *msg;
        payload = gnrc_pktbuf_add(NULL, DEVICE_STATE, sizeof(DEVICE_STATE), GNRC_NETTYPE_UNDEF);
        udp = gnrc_udp_hdr_build(payload, SERVER_PORT, SERVER_PORT);
        msg = gnrc_ipv6_hdr_build(udp, NULL, &server_addr);

        if (gnrc_netapi_send(ifs[0], msg) < 1) {
            puts("Error: unable to send message\n");
            gnrc_pktbuf_release(msg);
            return 1;
        }

        printf("Device state sent: %s\n", DEVICE_STATE);
        xtimer_sleep(5);
    }

    return 0;
}