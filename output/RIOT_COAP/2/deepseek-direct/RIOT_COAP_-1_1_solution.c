
#include <stdio.h>
#include <string.h>
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/coap.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pkt.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/coap/option.h"
#include "net/gnrc/coap/pkt.h"
#include "net/gnrc/coap/server.h"
#include "net/gnrc/coap/client.h"
#include "net/gnrc/coap/coap.h"
#include "net/gnrc/coap/coap_client.h"
#include "net/gnrc/coap/coap_server.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_option.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h"
#include "net/gnrc/coap/coap_pkt.h"
#include "net/gnrc/coap/coap_block.h"
#include "net/gnrc/coap/coap_observe.h"
#include "net/gnrc/coap/coap_resource.h"
#include "net/gnrc/coap/coap_endpoint.h"
#include "net/gnrc/coap/coap_msg.h"
#include "net/gnrc/coap/coap_hdr.h"
#include "net/gnrc/coap/coap_opt.h