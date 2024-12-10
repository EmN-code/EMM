#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/af.h>
#include <net/ipv4/addr.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_POST_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t result;

    /* Initialize the remote endpoint data */
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    /* Convert the IP address from text to binary form and store in remote.addr.ipv4 */
    result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        printf("Error: Invalid IP address format or unsupported address family.\n");
        return -1;
    }

    /* Initialize the CoAP request packet */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_POST_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    size_t len = gcoap_finish(&pdu, strlen(COAP_MESSAGE), COAP_FORMAT_TEXT);

    /* Send CoAP request */
    if (gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: Unable to send CoAP request.\n");
        return -1;
    }

    printf("CoAP request sent successfully.\n");
    return 0;
}