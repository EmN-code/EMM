#include "mutex.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include <stdio.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    const char *EMQX_URI_PATH = "/coap/test";
    const char *PAYLOAD = "Hello, COAP Cloud";
    const char *EMQX_SERVER_IP = "47.102.103.1";
    sock_udp_ep_t remote = { .port = 5683 };

    // Convert the IP address from text to binary form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error converting IP address\n");
        return -1;
    }

    // Initialize CoAP request
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Copy the payload data
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}
