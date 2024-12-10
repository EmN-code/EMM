#include "xtimer.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"

static sock_udp_ep_t server_ep;
static char buffer[CONFIG_GCOAP_PDU_BUF_SIZE];
static size_t buffer_len;

void send_coap_request(void) {
    coap_pkt_t pdu;
    int rc;

    gcoap_req_init(&pdu, buffer, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    buffer_len = gcoap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    buffer_len += strlen(payload);

    while (1) {
        rc = gcoap_req_send(buffer, buffer_len, &server_ep, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc < 0) {
            printf("Failed to send CoAP request. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }
}

int main(void) {
    server_ep.family = AF_INET6;
    server_ep.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&server_ep.addr.ipv6, SERVER_IP);
    server_ep.port = SERVER_PORT;

    send_coap_request();

    return 0;
}