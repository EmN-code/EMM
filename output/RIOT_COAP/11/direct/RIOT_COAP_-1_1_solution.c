#include "net/sock/udp.h"
#include "net/coap.h"
#include "msg.h"
#include "xtimer.h"
#include <stdio.h>
#include <string.h>

#define COAP_PORT 5683
#define COAP_BUF_SIZE 128
#define SERVER_ADDR "47.102.103.1"
#define URI_PATH "/coap/test"

void send_coap_post_request(void) {
    coap_pkt_t pdu;
    uint8_t buf[COAP_BUF_SIZE];

    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = COAP_PORT
    };

    /* Parse server address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse IPv6 address");
        return;
    }

    /* Initialize UDP socket */
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    /* Initialize CoAP PDU */
    coap_init_message(&pdu, COAP_TYPE_CON, COAP_METHOD_POST, coap_get_mid());
    coap_set_header_uri_path(&pdu, URI_PATH);
    char payload[] = "Hello, COAP Cloud";
    coap_set_payload(&pdu, (uint8_t *)payload, strlen(payload));

    size_t bytes = coap_serialize_message(&pdu, buf);

    /* Send packet */
    sock_udp_send(&sock, buf, bytes, &remote);

    puts("CoAP POST request sent!");

    /* Close socket */
    sock_udp_close(&sock);
}

int main(void) {
    /* Allow system to initialize */
    xtimer_sleep(1);
    puts("Starting CoAP client...");
    while (1) {
        send_coap_post_request();
        xtimer_sleep(5);
    }
    return 0;
}