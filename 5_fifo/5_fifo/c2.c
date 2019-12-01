#include "common.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <stdlib.h>
#define TO_SERVER_FILE "./from_client"
#define FROM_SERVER_FILE "./to_client_"
#define BUFF_SIZE 32

int fd, cmd, pid;
char buff[BUFF_SIZE];
char filename[BUFF_SIZE];

int shmId;
int semId;
char buffer[BUFFER_SIZE];
int nwritten;

pthread_t threads[MAX_PLAYERS];

int my_sum;
int dealer_sum;

int check = 0;
int check2 = 0;

int finalcheck = 0;
void send_msg()
{
	int fd2;
	char filename[BUFF_SIZE];

	sprintf(filename, "%s%d", TO_SERVER_FILE, getpid());
	if ((fd2 = open(filename, O_WRONLY)) == -1) {
		printf("[SYSTEM] in send_msg() open error!!\n");
		return NULL;
	}

	int choice;
	while (1) {
		sleep(1);
		if (finalcheck) { break; }
		if (check2 == 1) {
			printf("\n");
			printf("1. Hit\n");
			printf("2. Stand\n");
			printf("Please choose 1 or 2: ");
			fflush(stdout);

			scanf("%d", &choice);
			if (choice == 1)
			{
				strcpy(buffer, HIT);
				write(fd2, buffer, BUFFER_SIZE);

				printf("Sending: %s\n", buffer);
				/* HIT send */

				check2 = 2;
				sleep(1);
			}
			else if (choice == 2)
			{
				strcpy(buffer, STAND);
				write(fd2, buffer, BUFFER_SIZE);

				printf("Sending: %s\n", buffer);
				check2 = 0;
				check = 0;
				finalcheck = 1;
				break;
			}
			else
				printf("Unrecognized choice. Choose again.\n");
		}
	}
}


void recv_msg(void* x)
{
	check = 1;
	int my_hand_values[20], dealer_hand_values[20];
	int my_hand_suits[20], dealer_hand_suits[20];
	int nmy = 0, ndealer = 0;
	/* server side - send first cardset */
	char filename[BUFF_SIZE];
	int fdd;

	sprintf(filename, "%s%d", FROM_SERVER_FILE, getpid());
	if ((fdd = open(filename, O_RDONLY)) == -1) {
		printf("[SYSTEM] in recv_msg() open error!!\n");
		return NULL;
	}
	while (1) {
		if (check == 1) {
			read(fdd, buffer, BUFF_SIZE);

			my_hand_values[0] = get_value_id(buffer[0]);
			my_hand_suits[0] = get_suit_id(buffer[1]);
			my_hand_values[1] = get_value_id(buffer[2]);
			my_hand_suits[1] = get_suit_id(buffer[3]);
			dealer_hand_values[0] = get_value_id(buffer[4]);
			dealer_hand_suits[0] = get_suit_id(buffer[5]);
			nmy = 2;
			ndealer = 1;

			int choice;
			my_sum = calc_sum(my_hand_values, nmy);

			printf("\n");
			printf("My Hand: ");
			display_state(my_hand_values, my_hand_suits, nmy);
			printf("Dealer Hand: ");
			display_state(dealer_hand_values, dealer_hand_suits, ndealer);
			/* 내 덱에 합계가 21이 넘어갈 경우 lose */
			if (my_sum > 21)
			{
				printf("\nI'm busted! I lose!\n");
				return;
			}

			check = 0;
			check2 = 1;
		}
		int n = 4;
		for (n; n > 0; n--) {
			printf("\n");
			sleep(1);
		}

		//HIT 일때
		if (check2 == 2) {
			read(fdd, buffer, BUFF_SIZE);
			printf("I received: %c%c\n", buffer[0], buffer[1]);

			my_hand_values[nmy] = get_value_id(buffer[0]);
			my_hand_suits[nmy] = get_suit_id(buffer[1]);
			++nmy;
			printf("\n");
			printf("My Hand: ");
			display_state(my_hand_values, my_hand_suits, nmy);
			printf("Dealer Hand: ");
			display_state(dealer_hand_values, dealer_hand_suits, ndealer);
			my_sum = calc_sum(my_hand_values, nmy);
			if (my_sum > 21)
			{
				printf("\nI'm busted! I lose!\n");
				return;
			}
			check2 = 1;
		}
		n = 4;
		for (n; n > 0; n--) {
			printf("\n");
			sleep(1);
		}
		//STAND 일때
		if (finalcheck == 1) {
			finalcheck = 0;
			unsigned i;

			/* dealer card recv*/
			read(fdd, buffer, BUFF_SIZE);

			for (i = 0; i < strlen(buffer); i += 2)
			{
				dealer_hand_values[ndealer] = get_value_id(buffer[i]);
				dealer_hand_suits[ndealer] = get_suit_id(buffer[i + 1]);
				++ndealer;
			}

			printf("\n");
			printf("My Hand: ");
			display_state(my_hand_values, my_hand_suits, nmy);
			printf("Dealer Hand: ");
			display_state(dealer_hand_values, dealer_hand_suits, ndealer);

			my_sum = calc_sum(my_hand_values, nmy);
			dealer_sum = calc_sum(dealer_hand_values, ndealer);

			printf("my sum = %d\n", my_sum);
			printf("dealer sum = %d\n", dealer_sum);

			if (dealer_sum > 21)
				printf("\nDealer busted! I win!\n");
			else if (my_sum == dealer_sum)
				printf("\nMe and the dealer have the same score. It's a push!\n");
			else if (my_sum < dealer_sum)
				printf("\nDealer has a higher score. I lose!\n");
			else
				printf("\nI have a higher score. I win!\n");
			return;
		}
	}
}


void play_game(void* id)
{
	pthread_create(&threads[1], NULL, recv_msg);
	pthread_create(&threads[2], NULL, send_msg);
	int tid;
	for (tid = 1; tid < 2; ++tid)
		pthread_join(threads[tid], NULL);
}

int main()
{
	int id;
	char msg[BUFF_SIZE];

	if ((fd = open(TO_SERVER_FILE, O_WRONLY)) == -1) {
		printf("[SYSTEM] in main() open error!!\n");
		exit(1);
	}

	pid = getpid();
	printf("[CLIENT] client(%d) enter\n", pid);

	sprintf(msg, "3 %d", pid);
	write(fd, msg, strlen(msg));

	pthread_create(&threads[0], NULL, play_game, (void*)pid);
	pthread_join(threads[0], NULL);

	printf("\n\n============Game End============\n\n");
}