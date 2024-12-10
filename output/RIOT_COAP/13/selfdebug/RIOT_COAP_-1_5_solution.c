#include <net/sock/udp.h>
#include <arpa/inet.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 0x02
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };

int main(void)
{
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}