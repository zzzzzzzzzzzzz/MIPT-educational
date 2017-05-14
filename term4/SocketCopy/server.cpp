/*SERVER CODE*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <errno.h>
#include <stdlib.h>
#include <thread>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define BUFSIZE 2048
#define SPECIALPORT 52000
int availablePorts[8] = {53000, 53001, 53002, 53003, 53004, 53005, 53006, 53007};

struct message {
	char buffer[BUFSIZE];
	int begin;
};

union myProtocol {
	struct message message;
	char bytes[sizeof(struct message)];
};

string output;

void handleFunc() {
	int sockfd, newsockfd;
	int clilen;
	int n;
	struct sockaddr_in servaddr, cliaddr;

	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror(NULL);
		exit(1);
	}
	int iSetOption = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(char*)&iSetOption, sizeof(iSetOption));

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family= AF_INET;
	servaddr.sin_port= htons(SPECIALPORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //готов соединяться с клиентами через любой интерфейс

	if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) { //биндим порт, который слушаем, 
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	if(listen(sockfd, 5) < 0) { //второй параметр - размер очереди запросов
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	while (1) {
		clilen = sizeof(cliaddr);
		if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, (socklen_t *) &clilen)) < 0){
			perror(NULL);
			close(sockfd);
			exit(1);
		}
		cout << "Accepted new connection" << endl;
		union myProtocol received;
		union myProtocol answer;
		bzero(received.message.buffer, BUFSIZE);
		bzero(answer.message.buffer, BUFSIZE);
		if ((n = read(newsockfd, received.bytes, sizeof(struct message)) > 0)) {
			output = "distination/";
			output += received.message.buffer;
			bzero(received.message.buffer, BUFSIZE);
			//отправляем ответ клиенту
			bzero(answer.message.buffer, BUFSIZE);
			string answ("Accepted filename ");
			answ += received.message.buffer;
			strncpy(answer.message.buffer, answ.c_str(), BUFSIZE);
			answer.message.begin = -1;
			if (write(newsockfd, answer.bytes, sizeof(struct message)) < 0) {
				perror(NULL);
				close(sockfd);
				close(newsockfd);
				exit(1);
			}
		}
		close(newsockfd);
	}
	close(sockfd);
}

void acceptConnecton(int num) {

	int sockfd, newsockfd;
	int clilen;
	int n;
	struct sockaddr_in servaddr, cliaddr;

	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror(NULL);
		exit(1);
	}
	int iSetOption = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family= AF_INET;
	servaddr.sin_port= htons(availablePorts[num]);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //готов соединяться с клиентами через любой интерфейс

	if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) { //биндим порт, который слушаем, 
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	if(listen(sockfd, 5) < 0) { //второй параметр - размер очереди запросов
		perror(NULL);
		close(sockfd);
		exit(1);
	}


	union myProtocol received;
	union myProtocol answer;
	bzero(received.message.buffer, BUFSIZE);
	bzero(answer.message.buffer, BUFSIZE);
	while(1) {
		clilen = sizeof(cliaddr);
		if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, (socklen_t *) &clilen)) < 0) {
			perror(NULL);
			close(sockfd);
			exit(1);
		}
		cout << "Accepted new connection" << endl;
		ofstream outfile(output, ios::out | ios::binary);
    	if(!outfile) {
        	cerr << "an error occured while open output " + output << endl;
        	exit(errno);
    	}	

    	bzero(received.message.buffer, BUFSIZE);
		while ((n = read(newsockfd, received.bytes, sizeof(struct message)) > 0)) {
			outfile.seekp(received.message.begin);
    		outfile.write(received.message.buffer, strlen(received.message.buffer));

    		//отправляем ответ клиенту
    		bzero(answer.message.buffer, BUFSIZE);
    		string answ("Part delivered");
			strncpy(answer.message.buffer, answ.c_str(), BUFSIZE);
			answer.message.begin = -1;
			if ((n = write(newsockfd, answer.bytes, sizeof(struct message))) < 0) {
				perror(NULL);
				close(sockfd);
				close(newsockfd);
				exit(1);
			}
		}

		if (n < 0) {
			perror(NULL);
			close(sockfd);
			close(newsockfd);
			exit(1);
		}
		outfile.close();
		close(newsockfd);
	}
	close(sockfd);
}

int main(int argc, char **argv) {

	if(argc != 2){
		printf("Usage: ./server <number of threads>\n");
		exit(1);
	}

	string num(argv[1]);
    istringstream iss(num, istringstream::in);
    int threadNum;
    iss >> threadNum;

	thread *t = new thread[threadNum - 1];

	thread handler = thread(handleFunc);

    //launch threads

    int i = 0;
    for ( ; i < threadNum - 1; i++) {
        t[i] = thread(acceptConnecton, i);
    } 
    
    acceptConnecton(i);

    handler.join();
    //join threads
    for (int i = 0; i < threadNum - 1; i++) {
        t[i].join();
    } 

	return 0;
}

