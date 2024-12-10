#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>

#define SERVER_IP           "47.102.103.1"
#define SERVER_PORT         (5683)
#define URI_PATH            "/coap/test"
#define PAYLOAD             "Hello, COAP Cloud"
#define COAP_METHOD         COAP_METHOD_POST

static ssize_t _response_handler(coap_pkt_t* pdu, uint8_t* buf, size_t len)
{
    (void)pdu;
    (void)buf;
    (void)len;
    printf("Received response\n");
    return 0;
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Try to convert the IP address string to byte representation
    while (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("IP address conversion failed, retrying...\n");
        xtimer_sleep(1);
    }

    // Initialize CoAP request
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD, URI_PATH);
    coap_hdr_set_type(&pdu, COAP_TYPE_CON);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    gcoap_opt_finish(&pdu, len, COAP_FORMAT_TEXT);

    // Send CoAP request
    while (gcoap_req_send(&buf[0], len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Retrying to send CoAP request...\n");
        xtimer_sleep(1);
    }
    printf("CoAP request sent successfully\n");

    return 0;
}