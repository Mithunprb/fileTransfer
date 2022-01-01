// client code for UDP socket programming
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_PROTOCOL 0
#define IP_ADDRESS "127.0.0.1" // localhost
#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define cipherKey 'S'
#define sendrecvflag 0

// function to clear bufferfer
void clearBuf(char* b)
{
	int i;
	for (i = 0; i < NET_BUF_SIZE; i++)
		b[i] = '\0';
}

// function for decryption
char Cipher(char channel)
{
	return channel ^ cipherKey;
}

// function to receive file
int recvFile(char* buffer, int s)
{
	int i;
	char channel;
	for (i = 0; i < s; i++) {
		channel = buffer[i];
		channel = Cipher(channel);
		if (channel == EOF)
			return 1;
		else
			printf("%c", channel);
	}
	return 0;
}

// driver code
int main()
{
	int sockforword, nBytes;
	struct sockaddr_in addr_con;
	int addrlen = sizeof(addr_con);
	addr_con.sin_family = AF_INET;
	addr_con.sin_port = htons(PORT_NO);
	addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	char net_buffer[NET_BUF_SIZE];
	FILE* fp;

	// socket()
	sockforword = socket(AF_INET, SOCK_DGRAM,
					IP_PROTOCOL);

	if (sockforword < 0)
		printf("\nfile descriptor not received!!\n");
	else
		printf("\nfile descriptor %d received\n", sockforword);

	while (1) {
		printf("\nPlease enter file name to receive:\n");
		scanf("%s", net_buffer);
		sendto(sockforword, net_buffer, NET_BUF_SIZE,
			sendrecvflag, (struct sockaddr*)&addr_con,
			addrlen);

		printf("\n---------Data Received---------\n");

		while (1) {
			// receive
			clearBuf(net_buffer);
			nBytes = recvfrom(sockforword, net_buffer, NET_BUF_SIZE,
							sendrecvflag, (struct sockaddr*)&addr_con,
							&addrlen);

			// process
			if (recvFile(net_buffer, NET_BUF_SIZE)) {
				break;
			}
		}
		printf("\n-------------------------------\n");
	}
	return 0;
}
