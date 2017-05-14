/*CLIENT CODE*/
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
#include <sstream>
#include <errno.h>
#include <stdlib.h>
#include <thread>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

/*
struct sockaddr_in {
	short int          sin_family;  // Семейство адресов
	unsigned short int sin_port;    // Номер порта
	struct in_addr     sin_addr;    // IP-адрес
	unsigned char      sin_zero[8]; // "Дополнение" до размера структуры sockaddr
};
*/

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

void CopySlice(const long unsigned int begin, const long unsigned int slice, const string& filename, const string& ipstr, int num) {
    ifstream infile(filename, ios::in | ios::binary);
    if(!infile) {
        cerr << "an error occured while open file to copy" << endl;
        exit(errno);
    }
    infile.seekg(begin);
	/*создание клиентского сокета*/
	int sockfd;
	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { //PF_INET для сокета, протокол TCP/IP
		perror(NULL);
		exit(errno);
	}
	int iSetOption = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(char*)&iSetOption, sizeof(iSetOption));

	/*структура для связывания*/
	struct sockaddr_in servaddr;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET; //AF_INET для адреса
	servaddr.sin_port = htons(availablePorts[num]);

	if (inet_aton(ipstr.c_str(), &servaddr.sin_addr) == 0){
		printf("Invalid IP address\n");
		close(sockfd);
		exit(1);
	}

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) { //преобразование к sockaddr*
		perror(NULL);
		close(sockfd);
		exit(1);
	}

	//отправляем содержимое файла
    //формируем сообщение
    union myProtocol package;

    bzero(package.message.buffer, BUFSIZE);
    package.message.begin = begin;
    
    if (BUFSIZE < slice) {
    	infile.read(package.message.buffer, BUFSIZE);
    } else {
    	infile.read(package.message.buffer, slice);
    }

    union myProtocol received;
    int summaryRead = infile.gcount();
    while (summaryRead <= slice) {
		int n;
		if ( (n = write(sockfd, package.bytes, sizeof(struct message)) < 0)) {
			perror("Can\'t write\n");
			close(sockfd);
			exit(1);
		}
		package.message.begin += infile.gcount();
		if ( (n = read(sockfd, received.bytes, sizeof(struct message))) < 0){
			perror("Can\'t read an answer from server\n");
			close(sockfd);
			exit(1);
		}
		printf("Server answered to you: %s\n", received.message.buffer);
		if (slice - summaryRead < BUFSIZE) {
			bzero(received.message.buffer, slice - summaryRead);
		} else {
			bzero(received.message.buffer, BUFSIZE);
		}
    	summaryRead += infile.gcount();
    }
    //gcount returns amount of bytes
    close(sockfd);
 
    infile.close();
}

int main(int argc, char **argv) {
	if(argc != 4){
		printf("Usage: a.out <IP address> <file to copy> <number of threads>\n");
		exit(1);
	}
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	string ipstr(argv[1]);
    string filename(argv[2]);
	string num(argv[3]);
    istringstream iss(num, istringstream::in);
    int threadNum;
    iss >> threadNum;

	/*создание клиентского сокета*/
	int sockfd;
	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { //PF_INET для сокета, протокол TCP/IP
		perror(NULL);
		exit(errno);
	}
	int iSetOption = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(char*)&iSetOption, sizeof(iSetOption));

	/*структура для связывания*/
	struct sockaddr_in servaddr;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET; //AF_INET для адреса
	servaddr.sin_port = htons(SPECIALPORT);

	if (inet_aton(ipstr.c_str(), &servaddr.sin_addr) == 0){
		printf("Invalid IP address\n");
		close(sockfd);
		exit(1);
	}

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) { //преобразование к sockaddr*
		perror(NULL);
		close(sockfd);
		exit(1);
	}
	//сначала отправляем имя файла
    //формируем сообщение
    union myProtocol package;

    bzero(package.message.buffer, BUFSIZE);
	strncpy(package.message.buffer, filename.c_str(), BUFSIZE);
	package.message.begin = -1;
	int n;
	if ( (n = write(sockfd, package.bytes, sizeof(struct message)) < 0)) {
		perror("Can\'t write\n");
		close(sockfd);
		exit(1);
	}
	union myProtocol received;
	if ( (n = read(sockfd, received.bytes, sizeof(struct message))) < 0){
		perror("Can\'t read an answer from server\n");
		close(sockfd);
		exit(1);
	}
	printf("Server answered to you: %s\n", received.message.buffer);
	bzero(received.message.buffer, BUFSIZE);
    close(sockfd);

    ifstream infile(filename, ios::in | ios::binary);
    if(!infile) {
        cerr << "an error occured while open the input file" << endl;
        exit(errno);
    }
    // get length of file:
    infile.seekg (0, infile.end);
    long unsigned int length = infile.tellg();
    infile.close();
    long unsigned int begin = 0;
    long unsigned int slice = length / threadNum + 1;
    thread *t = new thread[threadNum - 1];

    //launch threads
    int i = 0;
    for ( ; i < threadNum - 1; i++) {
        t[i] = thread(CopySlice, begin, slice, filename, ipstr, i);
        begin+=slice;
    } 
    
    CopySlice(begin, slice, filename, ipstr, i);

    //join threads
    for (int i = 0; i < threadNum - 1; i++) {
        t[i].join();
    } 

    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(t2 - t1).count();

    cout << duration << endl;
	return 0;
}