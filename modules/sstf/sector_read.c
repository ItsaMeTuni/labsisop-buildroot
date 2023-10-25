/*
 * Simple disc I/O generator
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define BUFFER_LENGTH 512
#define DISK_SZ 1073741824
#define FORKS 120

int main()
{
	int ret, fd, pid, i;
	unsigned int pos;
	char buf[BUFFER_LENGTH];
	

	printf("Starting sector read example...\n");

	printf("Cleaning disk cache...\n");
	system("echo 3 > /proc/sys/vm/drop_caches"); // limpa buffers e caches de disco

	printf("Configuring scheduling queues...\n");
	system("echo 2 > /sys/block/sda/queue/nomerges");
	system("echo 4 > /sys/block/sda/queue/max_sectors_kb"); 
	system("echo 0 > /sys/block/sda/queue/read_ahead_kb");

    struct timeval t1, t2;
    double elapsedTime;
	int is_parent = 0;

	gettimeofday(&t1, NULL);

	printf("Forking processes to put stress on disk scheduler...\n");
	for (int i = 0; i < FORKS; i++) {
		is_parent = fork();
		// fork();
		if (!is_parent) {
			break;
		};
	}

	srand(getpid());

	fd = open("/dev/sda", O_RDWR);
	if (fd < 0) {
		perror("Failed to open the device...");
		return errno;
	}

	for (i = 0; i < 500; i++) {
		pos = (rand() % (DISK_SZ >> 9));
		/* Set position */
		lseek(fd, pos * 512, SEEK_SET);
		/* Peform read. */
		read(fd, buf, 100);
	}
	close(fd);

	while (wait(NULL) > 0);

	if (is_parent) {
		gettimeofday(&t2, NULL);
		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
		printf("%f ms.\n", elapsedTime);
	}

	return 0;
}