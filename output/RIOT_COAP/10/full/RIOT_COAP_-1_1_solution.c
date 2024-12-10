#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP   "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH    "/coap/test"
#define REQUEST_MESSAGE  "Hello, COAP Cloud"

static ssize_t _content_handler(coap_pkt_t *pdu);
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

/* Response Handler */
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (pdu) {
        printf("Response received: Code %1u.%02u\n",
                coap_get_code_class(pdu),
                coap_get_code_detail(pdu));
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    /* CoAP Packet buffer */
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t bytes;
    
    /* Prepare the packet */
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    
    /* Add payload */
    memcpy(pdu.payload, REQUEST_MESSAGE, strlen(REQUEST_MESSAGE));
    
    /* Set the payload length */
    bytes = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (bytes < 0) {
        printf("Error while adding payload: %d\n", (int)bytes);
        return 1;
    }
    
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_SERVER_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP) == NULL) {
        printf("Failed converting server address\n");
        return 1;
    }
    
    /* Send CoAP request within a retry loop */
    while(1) {
        bytes = gcoap_req_send(buf, bytes, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (bytes <= 0) {
            printf("Failed to send request: %d\n", (int)bytes);
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Request sent successfully.\n");
            break;
        }
    }
    
    return 0;
}