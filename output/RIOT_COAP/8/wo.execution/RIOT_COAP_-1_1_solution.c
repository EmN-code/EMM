#include <stdio.h>
#include <string.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;
    sock_udp_ep_t remote;

    /* Server address and properties */
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    remote.addr.ipv4[0] = 47;
    remote.addr.ipv4[1] = 102;
    remote.addr.ipv4[2] = 103;
    remote.addr.ipv4[3] = 1;

    /* Initialize CoAP packet */
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (result < 0) {
        printf("Error: unable to initialize request\n");
        return 1;
    }

    /* Set the payload */
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    pdu.payload_len = 17;

    /* Send CoAP request */
    result = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        printf("Error: unable to send request\n");
        return 1;
    }

    printf("CoAP request sent to %s at port %d\n", SERVER_IP, SERVER_PORT);
    return 0;
}