// server code for UDP socket programming
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define IP_PROTOCOL 0
#define PORT_NO 15050
#define BUFFER_SIZE 32
#define Key_cipher 'S'
#define sendrecvflag 0
#define nofile "File Not Found!"

// function to clear buffer
void clearBuffer(char* b)
{
	int i;
	for (i = 0; i < BUFFER_SIZE; i++)
		b[i] = '\0';
}

// function to encrypt
char Cipher(char ch)
{
	return ch ^ Key_cipher;
}

// function sending file
int sendFile(FILE* f_p, char* buf, int s)
{
	int i, length;
	if (f_p == NULL) {
		strcpy(buf, nofile);
		length = strlen(nofile);
		buf[length] = EOF;
		for (i = 0; i <= length; i++)
			buf[i] = Cipher(buf[i]);
		return 1;
	}

	char ch, ch2;
	for (i = 0; i < s; i++) {
		ch = fgetc(f_p);
		ch2 = Cipher(ch);
		buf[i] = ch2;
		if (ch == EOF)
			return 1;
	}
	return 0;
}

// driver code
int main()
{
	int sockforword, nBytes;
	struct sockaddr_in addr_con;
	int addrlength = sizeof(addr_con);
	addr_con.sin_family = AF_INET;
	addr_con.sin_port = htons(PORT_NO);
	addr_con.sin_addr.s_addr = INADDR_ANY;
	char network_buffer[BUFFER_SIZE];
	FILE* f_p;

	// socket()
	sockforword = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);

	if (sockforword < 0)
		printf("\nfile descriptor not received!!\n");
	else
		printf("\nfile descriptor %d received\n", sockforword);

	// bind()
	if (bind(sockforword, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0)
		printf("\nSuccessfully binded!\n");
	else
		printf("\nBinding Failed!\n");

	while (1) {
		printf("\nWaiting for file name...\n");

		// receive file name
		clearBuffer(network_buffer);

		nBytes = recvfrom(sockforword, network_buffer,
						BUFFER_SIZE, sendrecvflag,
						(struct sockaddr*)&addr_con, &addrlength);

		f_p = fopen(network_buffer, "r");
		printf("\nFile Name Received: %s\n", network_buffer);
		if (f_p == NULL)
			printf("\nFile open failed!\n");
		else
			printf("\nFile Successfully opened!\n");

		while (1) {

			// process
			if (sendFile(f_p, network_buffer, BUFFER_SIZE)) {
				sendto(sockforword, network_buffer, BUFFER_SIZE,
					sendrecvflag,
					(struct sockaddr*)&addr_con, addrlength);
				break;
			}

			// send
			sendto(sockforword, network_buffer, BUFFER_SIZE,
				sendrecvflag,
				(struct sockaddr*)&addr_con, addrlength);
			clearBuffer(network_buffer);
		}
		if (f_p != NULL)
			fclose(f_p);
	}
	return 0;
}
