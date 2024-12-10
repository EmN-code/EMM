#include <string.h>  // Required for memcpy and other standard functions
#include <stdio.h>    // Required for printf
#include "net/ipv6/addr.h"  // Required for inet_pton
#include "net/iphc.h"  // Required for COAP methods and options
#include "os.h"  // Required for thread creating and kernel_pid_t
#include "shell.h"  // Required for shell command handling
#include "gcoap.h"   // Required for CoAP functions

// Constants
#define SERVER_IP        "47.102.103.1"
#define SERVER_PORT      5683
#define URI_PATH         "/coap/test"
#define COAP_PAYLOAD     "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];  // Buffer for CoAP PDU

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)remote;  // Unused parameter
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    size_t len;
    coap_pkt_t pdu;

    // Convert IP address
    if (!inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6)) {
        puts("Error: Unable to parse destination address");
        return 1;
    }

    // Initialize CoAP request
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_put_option(&pdu, COAP_OPT_CONTENT_FORMAT, COAP_CT_TEXT_PLAIN);
    memcpy(pdu.payload, COAP_PAYLOAD, sizeof(COAP_PAYLOAD));
    len += sizeof(COAP_PAYLOAD);

    // Send the request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending request\n");
        return 1;
    }

    return 0;
}