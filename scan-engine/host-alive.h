/*
  host-alive contains functions which will make use of ICMP and various other 
  methods to probe whether a host is alive or down. This is used prior to initiating
  a full port scan so we can skip wasting time on dead/non-existent hosts
*/

// TODO: heres what we can do:
// non root-users can use a TCP connect to port 80 or 443 to see if host is alive
// root users have more options. ICMP echo requests, ACK to port 80 for example,
// SYN to port 443, an ICMP timestamp request, udp, and more. I will focus on ICMP and TCP
// then come back to these other options later after more scanning methods are added.

#pragma once

#define ICMP_ECHOREPLY 0	
#define ICMP_DEST_UNREACH	3	
#define ICMP_ECHO 8

// ICMP UNREACHABLE CODES
#define ICMP_NET_UNREACH	0
#define ICMP_HOST_UNREACH	1
#define ICMP_PROT_UNREACH	2
#define ICMP_PORT_UNREACH	3
#define ICMP_FRAG_NEEDED	4
#define ICMP_SR_FAILED		5
#define ICMP_NET_UNKNOWN	6
#define ICMP_HOST_UNKNOWN	7
#define ICMP_HOST_ISOLATED 8

#include "./scan-engine.h"

// https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol
typedef struct {
  u8 type;
  u8 code;
  u16 checksum;
  u16 id;
  u16 sequence;
} __attribute__((packed)) s_icmp_hdr;

// should I have one function that calls other functions, or just have separate functions?
// i will (for now at least) just make an ICMP ping request function to test. But consider the above...
boolean icmp4_ping_request(scan_info_t* s);

// https://www.rfc-editor.org/rfc/rfc1071 ICMP checksum here
u16 icmp_checksum(u16* data, size_t size);
