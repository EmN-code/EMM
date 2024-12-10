#include <stdio.h>
#include "tim.h"
#include "sock_udp.h"
#include "net/gcoap.h"

#define SERVER_IP      "47.102.103.1"
#define SERVER_PORT    CONFIG_GCOAP_PORT
#define URI_PATH       "/coap/test"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static ssize_t _send_coap_request(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6);
    
    /* Initialize CoAP packet */
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Failed to initialize CoAP request\n");
        return -1;
    }

    /* Add payload "Hello, CoAP Cloud" */
    const char *message = "Hello, CoAP Cloud";
    memcpy(pdu.payload, message, strlen(message));
    len += strlen(message);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    
    return gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void) {
    /* Perform necessary initializations */
    if (_send_coap_request() < 0) {
        printf("Failed to send CoAP POST request\n");
        return 1;
    }

    printf("CoAP POST request sent successfully\n");
    return 0;
}
