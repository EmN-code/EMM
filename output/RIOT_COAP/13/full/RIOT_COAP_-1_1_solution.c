#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);

    int rc;
    while (1) {
        if ((rc = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH)) < 0) {
            printf("gcoap_req_init() failed\n");
            xtimer_sleep(1);
            continue;
        }

        coap_hdr_set_type(pdu.hdr, COAP_TYPE_NON);
        memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
        gcoap_finish_payload(&pdu, strlen(PAYLOAD));

        rc = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc > 0) {
            printf("CoAP message sent successfully\n");
            break;  // Successfully sent, exit the retry loop.
        } else {
            printf("CoAP message failed to send, retrying...\n");
            xtimer_sleep(1);
        }
    }

    return 0;
}