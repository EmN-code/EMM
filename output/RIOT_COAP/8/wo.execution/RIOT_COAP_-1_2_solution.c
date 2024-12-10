#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"

#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

// Specify the CoAP server IP and port
char *SERVER_IP = "47.102.103.1";
uint16_t SERVER_PORT = 5683;

void _resp_handler(unsigned req_state, coap_pkt_t* pdu) {
    // Handle the response (this part is just a placeholder)
}

int main(void) {
    // Prepare the endpoint for sending data
    sock_udp_ep_t remote = { .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr);

    // Buffer to hold the CoAP message
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Send the CoAP request
    gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}