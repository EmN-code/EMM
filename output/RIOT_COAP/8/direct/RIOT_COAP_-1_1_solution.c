#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <net/gnrc/coap.h>
#include <net/gnrc/ipv6/netif.h>
#include <net/gnrc/netif.h>
#include <net/sock/udp.h>

int main(void) {
    ipv6_addr_t addr;
    uint8_t buf[128];
    const char *message = "Hello, COAP Cloud";
    sock_udp_ep_t remote = { .family = AF_INET6, .port = 5683 };

    /* Convert the IP address from string to binary */
    ipv6_addr_from_str(&addr, "47.102.103.1");

    /* Configure the remote endpoint parameters */
    remote.netif = SOCK_ADDR_ANY_NETIF;
    memcpy(&remote.addr.ipv6, &addr, sizeof(addr));

    /* Construct the CoAP request */
    gnrc_coap_req_init(buf, sizeof(buf), GNRC_COAP_METHOD_POST, "/coap/test");
    gnrc_coap_req_add_token(buf, sizeof(buf), "token", 5);
    gnrc_coap_add_payload(buf, sizeof(buf), (uint8_t *)message, strlen(message));

    /* Send the CoAP Message */
    if (sock_udp_send(NULL, buf, sizeof(buf), &remote) < 0) {
        puts("Error sending CoAP request");
    }
    else {
        puts("CoAP POST request sent successfully.");
    }

    return 0;
}