/*
 ip_sniffer.c: capture packet(IPv4/IPv6)
 Copyright © 2017 Yu Zhao <dutzhaoyu@gmail.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pcap.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/icmp6.h>
#include <linux/ipv6.h>
#include <netinet/ip6.h>
#define DEFINE_ICMP 1
#define DEFINE_ICMPv6 58
#define DEFINE_TCP 6
#define DEFINE_UDP 17
#define DEFINE_FRAG 44

void ipv6_to_str(char *, const struct in6_addr *);

void print_ethernet_packet(const u_char *, int);
void print_ethernet_header(const u_char *, int);

void print_ipv4_packet(const u_char *, int);
void print_ipv6_packet(const u_char *, int);
void print_ipv4_header(const u_char *, int);
void print_ipv6_header(const u_char *, int);

void print_tcp_packet(const u_char *, int, int);
void print_udp_packet(const u_char *, int, int);
void print_tcp_header(const u_char *, int, int);
void print_udp_header(const u_char *, int, int);


void print_icmpv4_packet(const u_char *, int, int);
void print_icmpv4_header(const u_char *, int, int);
void print_icmpv6_packet(const u_char *, int, int);
void print_icmpv6_header(const u_char *, int, int);
void print_fragment(const u_char *, int, int);
void print_complete_packet(const u_char *, int);


FILE *fp;
enum { IPv4, IPv6 }IP_TYPE;
int mac_size, ip_size, trans_size;//icmp not belong to trans in theory.
int ipv6_cnt, ipv4_cnt, loss_cnt;

void my_packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    struct ether_header *eth_header;
    eth_header = (struct ether_header *) packet;

    /* if packet is not complete,return */
    if (header->caplen < header->len) {
        loss_cnt++;
       // return;
    }

    if (ntohs(eth_header->ether_type) == ETHERTYPE_IPV6) {
        fprintf(fp, "\n\n------------------------------IPv6------------------------------\n");
        IP_TYPE = IPv6;
        ipv6_cnt++;
        print_ethernet_packet(packet, header->len);
    } else if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        fprintf(fp, "\n\n------------------------------IPv4------------------------------\n");
        IP_TYPE = IPv4;
        ipv4_cnt++;
        print_ethernet_packet(packet, header->len);
    }
    printf("IPv4: %d,   IPv6: %d,   NOT COMPLETE: %d\r", ipv4_cnt, ipv6_cnt, loss_cnt);

    fprintf(fp, "Ethernet Header\n");
    print_complete_packet(packet, mac_size);

    fprintf(fp, "IP Header\n");
    print_complete_packet(packet+mac_size, ip_size);

    fprintf(fp, "TCP/UDP/ICMP Header\n");
    print_complete_packet(packet+mac_size+ip_size, trans_size);

    fprintf(fp, "DATA Packet\n");
    int header_sum = mac_size + ip_size + trans_size;
    print_complete_packet(packet + header_sum, header->len - header_sum);
}

int main(int argc, char const* argv[]) {
    pcap_t *handle;
    char error_buffer[PCAP_ERRBUF_SIZE];
    char *device;
    int snapshot_len = 1028;
    int promiscuous = 0;
    int timeout = 1000;

    mac_size = sizeof(struct ethhdr);
    ipv4_cnt = 0;
    ipv6_cnt = 0;
    loss_cnt = 0;

    device = pcap_lookupdev(error_buffer);
    if (device == NULL) {
        printf("Error finding device: %s\n", error_buffer);
        return 1;
    }
    printf("DEVICE:    %s\n", device);
    if((fp = fopen("sniffer_log.txt", "w")) == NULL) {
        fprintf(stderr, "Can not open sniffer_log.txt\n");
        exit(1);
    }
    handle = pcap_open_live(device, snapshot_len, promiscuous, timeout, error_buffer);
    pcap_loop(handle, 0, my_packet_handler, NULL);
    pcap_close(handle);
    if (fclose(fp) != 0) {
        fprintf(stderr, "Close sniffer_log.txt fail!\n");
        exit(1);
    }
    return 0;
}

void print_ethernet_packet(const u_char *packet, int size) {
    print_ethernet_header(packet, size);
    if (IPv4 == IP_TYPE) {
        print_ipv4_packet(packet, size);
    } else if (IPv6 == IP_TYPE) {
        print_ipv6_packet(packet, size);
    }
}

void print_ethernet_header(const u_char *packet, int size) {
    struct ethhdr *eth = (struct ethhdr *)packet;
    fprintf(fp , "Ethernet Header\n");
    fprintf(fp , "\t|-Destination Address  : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
    fprintf(fp , "\t|-Source Address       : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    fprintf(fp , "\t|-Protocol             : %u\n",(unsigned short)eth->h_proto);
}

void print_ipv4_packet(const u_char *packet, int size) {
    print_ipv4_header(packet, size);
    struct iphdr *iph = (struct iphdr *)(packet  + sizeof(struct ethhdr) );
    int ip4len = iph->ihl*4;
    int offset = sizeof(struct ethhdr) + ip4len;
    switch (iph->protocol) {
       case DEFINE_ICMP:
            print_icmpv4_packet(packet, size, offset); break;
       case DEFINE_TCP:
            print_tcp_packet(packet, size, offset); break;
       case DEFINE_UDP:
            print_udp_packet(packet, size, offset); break;
    }
}

void print_ipv4_header(const u_char *packet, int size) {
    struct sockaddr_in source,dest;
    struct iphdr *iph = (struct iphdr *)(packet  + sizeof(struct ethhdr) );

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    fprintf(fp , "\tIP Header\n");
    fprintf(fp , "\t\t|-IP Version           : %d\n",(unsigned int)iph->version);
    fprintf(fp , "\t\t|-IP Header Length     : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    fprintf(fp , "\t\t|-Type Of Service      : %d\n",(unsigned int)iph->tos);
    fprintf(fp , "\t\t|-IP Total Length      : %d  Bytes(size of Packet)\n",ntohs(iph->tot_len));
    fprintf(fp , "\t\t|-Identification       : %d\n",ntohs(iph->id));
    //fprintf(fp , "   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero);
    //fprintf(fp , "   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment);
    //fprintf(fp , "   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment);
    fprintf(fp , "\t\t|-TTL                  : %d\n",(unsigned int)iph->ttl);
    fprintf(fp , "\t\t|-Protocol             : %d\n",(unsigned int)iph->protocol);
    fprintf(fp , "\t\t|-Checksum             : %d\n",ntohs(iph->check));
    fprintf(fp , "\t\t|-Source IP            : %s\n" , inet_ntoa(source.sin_addr) );
    fprintf(fp , "\t\t|-Destination IP       : %s\n" , inet_ntoa(dest.sin_addr) );
}

void print_ipv6_packet(const u_char *packet, int size) {
    print_ipv6_header(packet, size);
    struct ipv6hdr *iph = (struct ipv6hdr *)(packet + sizeof(struct ethhdr) );
    int offset = sizeof(struct ethhdr) + 40*sizeof(char);
    switch ((unsigned int)iph->nexthdr) {
        case DEFINE_ICMPv6:
            print_icmpv6_packet(packet, size, offset); break;
        case DEFINE_TCP:
            print_tcp_packet(packet, size, offset); break;
        case DEFINE_UDP:
            print_udp_packet(packet, size, offset); break;
        case DEFINE_FRAG:
            print_fragment(packet, size, offset);
            break;
    }
}

void print_ipv6_header(const u_char *packet, int size) {
    unsigned short iphdrlen;
    char *ipv6_addr = (char *)malloc(40*sizeof(char));
    struct ipv6hdr *iph = (struct ipv6hdr *)(packet + sizeof(struct ethhdr) );
    fprintf(fp, "\tIPv6 Header\n");
    fprintf(fp, "\t\t|-IP Version        : %d\n", (unsigned int)iph->version);
    fprintf(fp, "\t\t|-IP Traffic Class  : %d\n", (unsigned int)iph->priority);
    //fprintf(fp, "   |-IP Flow Label     : %d\n", ()iph->);
    fprintf(fp, "\t\t|-IP Payload Length : %d\n", (unsigned int)iph->payload_len);
    fprintf(fp, "\t\t|-IP  Next Header   : %d\n", (unsigned int)iph->nexthdr);
    fprintf(fp, "\t\t|-IP Hop Limit      : %d\n", (unsigned int)iph->hop_limit);
    ipv6_to_str(ipv6_addr, (const struct in6_addr *)&iph->saddr);
    fprintf(fp, "\t\t|-Source IP         : %s\n", ipv6_addr);
    ipv6_to_str(ipv6_addr, (const struct in6_addr *)&iph->daddr);
    fprintf(fp, "\t\t|-Destination IP    : %s\n", ipv6_addr);
    free(ipv6_addr);
    ip_size = 40*sizeof(char);
}

void ipv6_to_str(char * str, const struct in6_addr *addr) {
   sprintf(str, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                 (int)addr->s6_addr[0], (int)addr->s6_addr[1],
                 (int)addr->s6_addr[2], (int)addr->s6_addr[3],
                 (int)addr->s6_addr[4], (int)addr->s6_addr[5],
                 (int)addr->s6_addr[6], (int)addr->s6_addr[7],
                 (int)addr->s6_addr[8], (int)addr->s6_addr[9],
                 (int)addr->s6_addr[10], (int)addr->s6_addr[11],
                 (int)addr->s6_addr[12], (int)addr->s6_addr[13],
                 (int)addr->s6_addr[14], (int)addr->s6_addr[15]);
}

void print_tcp_packet(const u_char *packet, int size, int offset) {
    print_tcp_header(packet, size, offset);
}

void print_tcp_header(const u_char *packet, int size, int offset) {
    struct tcphdr *tcph=(struct tcphdr*)(packet + offset);

    fprintf(fp , "\t\tTCP Header\n");
    fprintf(fp , "\t\t\t|-Source Port         : %u\n",ntohs(tcph->source));
    fprintf(fp , "\t\t\t|-Destination Port    : %u\n",ntohs(tcph->dest));
    fprintf(fp , "\t\t\t|-Sequence Number     : %u\n",ntohl(tcph->seq));
    fprintf(fp , "\t\t\t|-Acknowledge Number  : %u\n",ntohl(tcph->ack_seq));
    fprintf(fp , "\t\t\t|-Header Length       : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
    //fprintf(fp , "   |-CWR Flag : %d\n",(unsigned int)tcph->cwr);
    //fprintf(fp , "   |-ECN Flag : %d\n",(unsigned int)tcph->ece);
    fprintf(fp , "\t\t\t|-Urgent Flag         : %d\n",(unsigned int)tcph->urg);
    fprintf(fp , "\t\t\t|-Acknowledgement Flag: %d\n",(unsigned int)tcph->ack);
    fprintf(fp , "\t\t\t|-Push Flag           : %d\n",(unsigned int)tcph->psh);
    fprintf(fp , "\t\t\t|-Reset Flag          : %d\n",(unsigned int)tcph->rst);
    fprintf(fp , "\t\t\t|-Synchronise Flag    : %d\n",(unsigned int)tcph->syn);
    fprintf(fp , "\t\t\t|-Finish Flag         : %d\n",(unsigned int)tcph->fin);
    fprintf(fp , "\t\t\t|-Window              : %d\n",ntohs(tcph->window));
    fprintf(fp , "\t\t\t|-Checksum            : %d\n",ntohs(tcph->check));
    fprintf(fp , "\t\t\t|-Urgent Pointer      : %d\n",tcph->urg_ptr);

    trans_size = tcph->doff*4;//Data Offset
}

void print_udp_packet(const u_char *packet, int size, int offset) {
    print_udp_header(packet, size, offset);
}

void print_udp_header(const u_char *packet, int size, int offset) {
    struct udphdr *udph = (struct udphdr*)(packet + offset);

    fprintf(fp , "\t\tUDP Header\n");
    fprintf(fp , "\t\t\t|-Source Port      : %d\n" , ntohs(udph->source));
    fprintf(fp , "\t\t\t|-Destination Port : %d\n" , ntohs(udph->dest));
    fprintf(fp , "\t\t\t|-UDP Length       : %d\n" , ntohs(udph->len));
    fprintf(fp , "\t\t\t|-UDP Checksum     : %d\n" , ntohs(udph->check));

    trans_size = sizeof(struct udphdr);
}

void print_icmpv4_packet(const u_char *packet, int size, int offset) {
    print_icmpv4_header(packet, size, offset);
}

void print_icmpv4_header(const u_char *packet, int size, int offset) {
    struct icmphdr *icmph = (struct icmphdr *)(packet + offset);

    fprintf(fp , "\t\tICMP Header\n");
    fprintf(fp , "\t\t\t|-Type : %d",(unsigned int)(icmph->type));

    switch ((unsigned int)(icmph->type)) {
        case ICMP_ECHOREPLY:
            fprintf(fp, "\t\t\tEcho Reply\n"); break;
        case ICMP_DEST_UNREACH:
            fprintf(fp, "\t\t\tDestination Unreachable\n"); break;
        case ICMP_SOURCE_QUENCH:
            fprintf(fp, "\t\t\tSource Quench\n"); break;
        case ICMP_REDIRECT:
            fprintf(fp, "\t\t\tRedirect (change route)\n"); break;
        case ICMP_ECHO:
            fprintf(fp, "\t\t\tEcho Request\n"); break;
        case ICMP_TIME_EXCEEDED:
            fprintf(fp, "\t\t\tTime Exceeded\n"); break;
        case ICMP_PARAMETERPROB:
            fprintf(fp, "\t\t\tParameter Problem\n"); break;
        case ICMP_TIMESTAMP:
            fprintf(fp, "\t\t\tTimestamp Request\n"); break;
        case ICMP_TIMESTAMPREPLY:
            fprintf(fp, "\t\t\tTimestamp Reply\n"); break;
    }

    fprintf(fp , "\t\t\t|-Code : %d\n",(unsigned int)(icmph->code));
    fprintf(fp , "\t\t\t|-Checksum : %d\n",ntohs(icmph->checksum));
    //fprintf(fp , "   |-ID       : %d\n",ntohs(icmph->id));
    //fprintf(fp , "   |-Sequence : %d\n",ntohs(icmph->sequence));

    trans_size = sizeof(struct icmphdr);
}

void print_icmpv6_packet(const u_char *packet, int size, int offset) {
    print_icmpv6_header(packet, size, offset);
}

void print_icmpv6_header(const u_char *packet, int size, int offset) {
    struct icmp6_hdr *icmph = (struct icmp6_hdr *)(packet + offset);
    fprintf(fp ,"\t\tICMPv6 Header\n");
    fprintf(fp ,"\t\t\t|-Type       : %d\n", (unsigned int)(icmph->icmp6_type));
    fprintf(fp, "\t\t\t|-Code       : %d\n", (unsigned int)(icmph->icmp6_code));
    fprintf(fp, "\t\t\t|-Checksum   : %d\n", (unsigned int)(icmph->icmp6_code));

    trans_size = sizeof(struct icmp6_hdr);
}

void print_fragment(const u_char *packet, int size, int offset) {
    struct ip6_frag *frag_hdr = (struct ip6_frag *)(packet + offset);
    fprintf(fp, "\t\tFragment Header\n");
    fprintf(fp, "\t\t\t|-Next Header    : %d\n", (unsigned int)(frag_hdr->ip6f_nxt));
    fprintf(fp, "\t\t\t|-Fragment Offset: %d\n", (unsigned int)(frag_hdr->ip6f_offlg));
    fprintf(fp, "\t\t\t|-Identification : %d\n", (unsigned int)(frag_hdr->ip6f_ident));
    switch ((unsigned int)(frag_hdr->ip6f_nxt)) {
        case DEFINE_TCP:
            print_tcp_packet(packet, size, offset+sizeof(struct ip6_frag));
            break;
        case DEFINE_UDP:
            print_udp_packet(packet, size, offset+sizeof(struct ip6_frag));
            break;
    }
}

void print_complete_packet(const u_char *packet, int size) {
    for(int i = 0 ; i < size ; ++i) {
        if( i!=0 && i%16==0) {
            fprintf(fp , "         ");
            for(int j = i-16 ; j < i ; j++) {
                fprintf(fp , "%c", (packet[j]>=32 && packet[j]<=128) ? (unsigned char)packet[j] : '.');
            }
            fprintf(fp , "\n");
        }
        if(i%16==0) fprintf(fp, "   ");
        fprintf(fp , "%02X ",(unsigned int)packet[i]);
        if( size-1 == i) {
            for(int j = 0;j < 15-i%16; ++j) {
                fprintf(fp , "   ");
            }
            fprintf(fp , "         ");
            for(int j = i-i%16 ; j <= i ; ++j) {
                fprintf(fp , "%c", (packet[j]>=32 && packet[j]<=128) ? (unsigned char)packet[j] : '.');
            }
            fprintf(fp ,  "\n" );
        }
    }
}
