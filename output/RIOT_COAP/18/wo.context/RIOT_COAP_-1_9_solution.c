#include <stdlib.h>  
#include <string.h> 
#include "net/sock/udp.h"  
#include "net/gcoap.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

const char *hello_cloud = "Hello, COAP Cloud";

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    if (pdu) {
        printf("Response received:\n");
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = CONFIG_GCOAP_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, hello_cloud, strlen(hello_cloud));
    pdu.payload_len = strlen(hello_cloud);

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if(result == 1) {
        printf("Converted IP address successfully\n");
    } else {
        printf("Failed to convert IP address\n");
    }

    ssize_t bytes_sent = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if(bytes_sent <= 0) {
        printf("Error sending CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    return 0;
}