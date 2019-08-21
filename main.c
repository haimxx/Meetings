#include <stdio.h>
#include <stdlib.h>

#include "Calendar.h"

int main() {
	Meeting_t *meet;
	Calendar_t *cal;
	int err, room;
	size_t choose, capacity;
	float begin, end;

	puts(" #####     #    #       ####### #     # ######     #    ######  ");
 	puts("#     #   # #   #       #       ##    # #     #   # #   #     # ");
 	puts("#        #   #  #       #       # #   # #     #  #   #  #     # ");
 	puts("#       #     # #       ####### #  #  # #     # #     # ######  ");
 	puts("#       ####### #       #       #   # # #     # ####### #   #   ");
 	puts("#     # #     # #       #       #    ## #     # #     # #    #  ");
 	puts(" #####  #     # ####### ####### #     # ######  #     # #     #");
 	puts("");

	err = DoesCalExist(&cal);
	if (err == -1){
		puts("Can't create calendar!");
		return -1;
	} else if (err == 0){
		puts ("Please enter initial number of meetings:");
		scanf("%lu",&capacity);
		if (capacity < 1) { 
			puts("Wrong input!");
			return -2;
		}

		cal = CreateCal(capacity);
		if (NULL == cal) {
			puts ("Not enough space!");
			return -3;
		}
	} 

	while (1) {
		puts("*********************************************");
		puts("*         Welcome to Calendar© app!         *");
		puts("*********************************************");
		puts("*     Please enter your desired action:     *");
		puts("*                                           *");
		puts("* 1 - Create a new meeting                  *");
		puts("* 2 - Delete a meeting                      *");
		puts("* 3 - Search for a meeting by starting time *");
		puts("* 4 - Print calendar                        *");
		puts("* 5 - Exit                                  *");
		puts("*********************************************");
		scanf("%lu",&choose);
	
		switch (choose) {
			case 1:
				puts("Please enter begining time:");
				scanf ("%5f", &begin);
				puts("Please enter ending time:");
				scanf ("%5f", &end);
				puts("Please enter room number:");
				scanf("%d", &room);
				meet = CreateMeet(room, begin, end);
				if (Insert(cal,meet) == -2) {
					puts("Time is already taken for another meeting!");
					break;
				}
			break;

			case 2:
				puts("Please enter begin time of meeting for removal:");
				scanf("%f", &begin);
				if (0 == Removal(cal,begin)){
					puts("Meeting deleted!");
				} else {
					puts("Meeting wasn't Found!");
				}
			break;

			case 3:
				puts("Enter begining time:");
				scanf("%f",&begin);
				meet = search(cal, begin);
				if (NULL == meet) {
					puts("*********************************");
					puts("There is no meeting in that time!");
					puts("*********************************");
				} else {
					puts("**************************************************************");
					printf("Room:%d \t Begining:%2.2f \t Ending:%2.2f \t \n", meet->room,meet->begin,meet->end);
					puts("**************************************************************");
				}
			break;

			case 4:
				Print(cal);
			break;

			case 5:
				StoreCal(cal);
				err = DestroyCal(cal);
				if (-1 == err) {
					puts ("Error clearing memory!");
					return -4;
				} else {
					puts("GOOD BYE!");
					return 0;
				}
			default:
				puts("You have typed a wrong number!");
				puts("");
			break;
		}
	}
}