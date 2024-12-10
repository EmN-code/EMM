#include <stdio.h>
#include <string.h>
#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void send_coap_post(void)
{
    uint8_t buf[128];
    coap_pkt_t pkt;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR);

    /* Initialize CoAP packet */
    coap_pkt_init(&pkt, buf, sizeof(buf), 0);
    coap_hdr_set_type(pkt.hdr, COAP_TYPE_CON);
    coap_hdr_set_code(pkt.hdr, COAP_METHOD_POST);
    coap_hdr_set_id(pkt.hdr, coap_get_id());

    /* Set URI path */
    size_t len = coap_opt_put_string(buf + pkt.payload_len, 0, COAP_OPT_URI_PATH, URI_PATH);
    pkt.payload_len += len;

    /* Set payload */
    memcpy(buf + pkt.payload_len, PAYLOAD, strlen(PAYLOAD));
    pkt.payload_len += strlen(PAYLOAD);

    /* Send CoAP request */
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);
    sock_udp_send(&sock, buf, pkt.payload_len, &remote);
    sock_udp_close(&sock);

    printf("CoAP POST request sent to %s:%d\n", SERVER_ADDR, SERVER_PORT);
}

int main(void)
{
    while (1) {
        send_coap_post();
        xtimer_sleep(10);  // Send request every 10 seconds
    }
    return 0;
}