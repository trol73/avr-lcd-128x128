#include "server.h"
#include "display.h"

#define PORT_NUMBER	3310

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>

#include <SDL.h>

static int sockfd;
static int newsockfd;

uint32_t write_after_sync_cnt = 0;

bool server_start() {
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("ERROR opening socket\n");
		return false;
	}
	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags < 0) {
		printf("ERROR wrong flags received\n");
		return false;
	}
	if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
		printf("ERROR fcntl error\n");
		return false;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT_NUMBER);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("ERROR on binding\n");
		exit(-1);
		return false;
	}
	printf("listen...\n");
	listen(sockfd, 1);
	printf("listen!\n");

	newsockfd = -1;
	return true;
}


int server_read_next_byte() {
	if (newsockfd < 0) {
		struct sockaddr_in cli_addr;
		socklen_t clilen;
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) {
			return -1;
		}
		int on = 1;
		setsockopt(newsockfd, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
		printf("Connection started\n");
		simulator_to_front();
	} else {
		int error;
		socklen_t len = sizeof(error);
		int retval = getsockopt(newsockfd, SOL_SOCKET, SO_ERROR, &error, &len);
		if (retval != 0) {
			 // there was a problem getting the error code
			 fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
			 newsockfd = -1;
		}
		if (error != 0) {
			 // socket has a non zero error status
			 fprintf(stderr, "socket error: %s\n", strerror(error));
			 newsockfd = -1;
		}
	}
	int val = 0;
	int n = read(newsockfd, &val, 1);

	if (n != 1) {
		if (errno == ENETUNREACH || errno == ENETRESET || errno == ECONNABORTED || errno == ECONNRESET || errno == ENOTCONN ||
				  errno == ETIMEDOUT || errno == ECONNREFUSED) {
			printf("Connection refused\n");
			shutdown(newsockfd, 2);
			newsockfd = -1;
		}

		return -1;
	}
//	printf(":read %i -> %x\n", n, val);
	return val;
}

void server_write_byte(int val) {
//	printf(":write %x\n", val);
	int n = write(newsockfd, &val, 1);
	if (n != 1) {
		printf("ERROR writing to socket\n");
	}
	if (++write_after_sync_cnt >= 256) {
		printf("WARNING 256 buffer overflow!\n");
	}
}


void server_stop() {
	close(newsockfd);
	close(sockfd);
}