
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/ipv4/addr.h"
#include "net/ipv4/uip.h"
#include "net/ipv4/uip-ds6.h"
#include "net/ipv4/uip-debug.h"
#include "net/ipv4/uip-fw.h"
#include "net/ipv4/uip-split.h"
#include "net/ipv4/uip-udp-packet.h"
#include "net/ipv4/uip-tcp-packet.h"
#include "net/ipv4/uip-icmp.h"
#include "net/ipv4/uip-arp.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip-nd6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-udp6.h"
#include "net/ipv4/uip-tcp6.h"
#include "net/ipv4/uip-fw6.h"
#include "net/ipv4/uip-split6.h"
#include "net/ipv4/uip-udp-packet6.h"
#include "net/ipv4/uip-tcp-packet6.h"
#include "net/ipv4/uip-icmp6.h"
#include "net/ipv4/uip-arp6.h"
#include "net/ipv4/uip