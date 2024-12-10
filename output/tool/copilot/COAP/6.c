#include <stdio.h>
#include <string.h>
#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void send_coap_post(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP);

    uint8_t buf[128];
    coap_pkt_t pkt;
    coap_pkt_init(&pkt, buf, sizeof(buf), 0);
    coap_hdr_set_type(pkt.hdr, COAP_TYPE_CON);
    coap_hdr_set_code(pkt.hdr, COAP_METHOD_POST);
    coap_hdr_set_id(pkt.hdr, coap_get_id());
    coap_opt_add_string(&pkt, COAP_OPT_URI_PATH, URI_PATH, '/');
    coap_payload_put_bytes(&pkt, PAYLOAD, strlen(PAYLOAD));

    ssize_t len = coap_build(&pkt, buf, sizeof(buf));
    if (len < 0) {
        printf("Error building CoAP packet\n");
        return;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return;
    }

    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        printf("Error sending CoAP packet\n");
    } else {
        printf("CoAP POST request sent\n");
    }

    sock_udp_close(&sock);
}

int main(void)
{
    printf("RIOT CoAP POST example\n");

    while (1) {
        send_coap_post();
        xtimer_sleep(10); // Send request every 10 seconds
    }

    return 0;
}