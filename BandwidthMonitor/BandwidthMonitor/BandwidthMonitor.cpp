// BandwidthMonitor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <pcap.h>
//#include <netinet/in.h>
#include <winsock.h>
#include <time.h>

char *iptos(u_long in);
char* ip6tos(struct sockaddr *sockaddr, char *address, int addrlen);
void ifprint(pcap_if_t* d);

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

static int sizeCap = 0;
static time_t start;
static time_t end;
static double timeDif;
int main(int argc, char** argv){
	pcap_if_t* alldevs, *d;
	pcap_t* fp, *adhandle;
	int i = 0;
	char errbuf[PCAP_ERRBUF_SIZE];

	

	i = pcap_findalldevs(&alldevs, errbuf);

	d = alldevs;

	if (d->next == NULL){
		printf("No local interfaces found");
	}
	ifprint(d);
	//pcap_freealldevs(alldevs);

	adhandle = pcap_open_live(d->name, 65536, 1, 1000, errbuf);
	if (adhandle == NULL){
		printf("Unable to open adapter");
		return -1;
	}

	time(&start);
	time(&end);
	timeDif = difftime(end, start);

	pcap_loop(adhandle, 0, packet_handler, NULL);
}

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data){
	timeDif = difftime(end, start);
	if (timeDif <= 10){
		time(&end);
		sizeCap += header->len; //Convert byte to KB - Loses a few KB probably
	}
	else {
		time(&start);
		time(&end);
		
		printf("%d KB\n", sizeCap / 1024);
		sizeCap = 0;
	}
	
	
}


	/* Print all the available information on the given interface */
	void ifprint(pcap_if_t *d)
	{
		pcap_addr_t *a;
		char ip6str[128];

		/* Name */
		printf("%s\n", d->name);

		/* Description */
		if (d->description)
			printf("\tDescription: %s\n", d->description);

		/* Loopback Address*/
		printf("\tLoopback: %s\n", (d->flags & PCAP_IF_LOOPBACK) ? "yes" : "no");

		/* IP addresses */
		for (a = d->addresses; a; a = a->next) {
			printf("\tAddress Family: #%d\n", a->addr->sa_family);
			
			switch (a->addr->sa_family)
			{
			case AF_INET:
				printf("\tAddress Family Name: AF_INET\n");
				if (a->addr)
					printf("\tAddress: %s\n", iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
				if (a->netmask)
					printf("\tNetmask: %s\n", iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr));
				if (a->broadaddr)
					printf("\tBroadcast Address: %s\n", iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr));
				if (a->dstaddr)
					printf("\tDestination Address: %s\n", iptos(((struct sockaddr_in *)a->dstaddr)->sin_addr.s_addr));
				break;

			/*case AF_INET6:
				printf("\tAddress Family Name: AF_INET6\n");
				if (a->addr)
					printf("\tAddress: %s\n", ip6tos(a->addr, ip6str, sizeof(ip6str)));
				break;
				*/
			default:
				printf("\tAddress Family Name: Unknown\n");
				break;
			}
		}
		printf("\n");
	}





/* From tcptraceroute, convert a numeric IP address to a string */
#define IPTOSBUFFERS    12
char *iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3 * 4 + 3 + 1];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	_snprintf_s(output[which], sizeof(output[which]), sizeof(output[which]), "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}
/*
char* ip6tos(struct sockaddr *sockaddr, char *address, int addrlen)
{
	socklen_t sockaddrlen;

#ifdef WIN32
	sockaddrlen = sizeof(struct sockaddr_in6);
#else
	sockaddrlen = sizeof(struct sockaddr_storage);
#endif


	if (getnameinfo(sockaddr,
		sockaddrlen,
		address,
		addrlen,
		NULL,
		0,
		NI_NUMERICHOST) != 0) address = NULL;

	return address;
}

*/