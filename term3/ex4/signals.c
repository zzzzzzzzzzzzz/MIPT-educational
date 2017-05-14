#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int* getbits(int file_descriptor) {
	size_t size;
	int* bits = (int *)calloc(8*sizeof(int), 1);
	char byte = 0;
	int byte_number = 0;
	size_t filesize = 0;
	printf("here getbits\n");
	while((size = read(file_descriptor, &byte, 1)) != 0) {
		if (size < 0) {
			printf("Can\'t read from file");
			_exit(errno);
		}
		int mask = 1;
		int i;
		for (i = 0; i < 8; i++) {
			if ((byte & mask) == 0) {
				bits[byte_number*8 + i] = 0;
			} else {
				bits[byte_number*8 + i] = 1;
			}
			mask *= 2; 
		}
		byte_number++;
		bits = (int*)realloc(bits, (byte_number+1)*sizeof(int)*8);
		memset(&bits[byte_number * 8], 0, 8*sizeof(int));
	}
	bits[byte_number * 8] = 2;
	return bits;
}

int sender(int file_descriptor, pid_t proc_id) {
	sigset_t sigset_listen_to_printer;
	int any_errors = 0;
	sigaddset(&sigset_listen_to_printer, SIGUSR1);
	sigaddset(&sigset_listen_to_printer, SIGTERM);
	int response;
	/*getting bits of file*/
	size_t sum_size = 0;
	int *bits;
	bits = getbits(file_descriptor);
	int i = 0;
	while(1) {
		int signal;
		response = sigwait(&sigset_listen_to_printer, &signal);
		if (response < 0) {
			printf("Failed to sigwait() for the printer");
			goto go_out;
		}
		switch(signal) {
			case SIGTERM:
				/*then exiting with error*/
				any_errors = 1;
				goto go_out;
				break;
			case SIGUSR1:
				/*then sending next bit to printer*/
				if (bits[i] != 2) {
					response = kill(proc_id, bits[i] ? SIGUSR2 : SIGUSR1);
					if (response < 0) {
						printf("Failed to kill() the printer with SIGUSR1/SIGUSR2\n");
						any_errors = 1;
						goto go_out;
					}
					i++;
				} else {
					response = kill(proc_id, SIGTERM);
					if (response < 0) {
						printf("Failed to kill() the printer with SIGTERM\n");
						any_errors = 1;
						goto go_out;
					}
				}
				break;
			default:
				printf("Got signal %d", signal);
				goto go_out;
		}
	}
go_out:
	free(bits);
	close(file_descriptor);
	return any_errors;
}

int printer(int write_to, pid_t child_pid) {
	sigset_t sigset_listen_to_sender;
	int any_errors = 0;
	sigaddset(&sigset_listen_to_sender, SIGUSR1);
	sigaddset(&sigset_listen_to_sender, SIGUSR2);
	sigaddset(&sigset_listen_to_sender, SIGTERM);
	int response = 0;
	char byte = 0;
	int byte_number = 0;
	int bit_mask = 1;
	while(1) {
		int signal;
		response = kill(child_pid, SIGUSR1); //ready to get the next bit
		if (response < 0) {
			printf("Failed to kill() to the sender");
			any_errors = 1;
			goto go_out;
		}
		response = sigwait(&sigset_listen_to_sender, &signal);
		if (response < 0) {
			printf("Failed to sigwait() for the printer");
			any_errors = 1;
			goto go_out;
		}
		switch(signal) {
			case SIGTERM:
				/*then exiting*/
				return any_errors;
				break;
			case SIGUSR1:
				byte |= (0);
				break;
			case SIGUSR2:
				byte |= (bit_mask);
				break;
			default:
				printf("Got signal %d", signal);
				goto go_out;
		}
		bit_mask *= 2;
		if (bit_mask == 256) {
			if (write(write_to, &byte, 1) < 0) {
				printf("Failed to write\n");
				any_errors = 1;
				goto go_out;
			}
			byte = 0;
			bit_mask = 1;
		}
	}
go_out:
	response = kill(child_pid, SIGTERM);
	if (response < 0) {
		printf("Failed to kill() to the sender");
		any_errors = 1;
		goto go_out;
	}	
	return any_errors;
}

int main(int argc, char** argv) {

	if (argc != 2) {
		printf("Note, this program takes only one argument, dude");
		_exit(errno);
	}
	const char* pathname = argv[1];
	int fd;
	if ((fd = open(pathname, O_RDONLY)) < 0) {
		printf("Failed to open() file '%s': %m", pathname);
		_exit(errno);
	}
	signal(SIGCHLD, SIG_IGN);

	/*
	 * Block our IPC signals before forking to avoid race conditions during initialization.
	 * The signal masks are preserved across fork().
	 */

	sigset_t sigset_ipc;
	sigaddset(&sigset_ipc, SIGUSR1);
	sigaddset(&sigset_ipc, SIGUSR2);
	sigaddset(&sigset_ipc, SIGTERM);
	sigprocmask(SIG_BLOCK, &sigset_ipc, NULL);

	pid_t child_pid = fork();
	if (child_pid == 0) {
		return sender(fd, getppid());
	} else if (child_pid < 0) {
		printf("Fork failed\n");
		_exit(errno);
	} else {
		close(fd);
		return printer(STDOUT_FILENO, child_pid);
	}
	return 0;
}