#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define COAP_SERVER_IP       "47.102.103.1"
#define COAP_SERVER_PORT     CONFIG_GCOAP_PORT
#define COAP_URI_PATH        "/coap/test"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    char *msg = "Hello, COAP Cloud";

    printf("Starting CoAP POST request to server at %s\n", COAP_SERVER_IP);

    // Initialize remote server details
    remote_family = AF_INET;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, COAP_SERVER_IP);
    remote.port = COAP_SERVER_PORT;

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    // Append payload to CoAP packet
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD) < 0) {
        printf("Error finishing CoAP options\n");
        return 1;
    }
    memcpy(pdu.payload, msg, strlen(msg));
    pdu.payload_len = strlen(msg);

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}