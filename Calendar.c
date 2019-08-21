#include <stdlib.h>
#include <stdio.h>

#include "Calendar.h"


int DoesCalExist(Calendar_t **cal)
{
	FILE *fp;
	size_t i;
	int room;
	float begin,end;
	size_t meetingsCount;
	size_t capacity;
	Meeting_t *meet;

	if (!(fp = fopen(FILE_NAME,"r"))) {
		return 0;
	}

	fscanf(fp, "%lu",&meetingsCount);
	capacity = (meetingsCount > MIN_CAPACITY) ? meetingsCount : MIN_CAPACITY;
	
	*cal = CreateCal(capacity);
	if(NULL == *cal){
		return -1;
	}

	for(i=0; i<meetingsCount; ++i){
		fscanf(fp, "%d%f%f",&room, &begin, &end);
		meet = CreateMeet(room, begin, end);
		if (NULL == meet){
			DestroyCal(*cal);
			return -1;
		}
		
		Insert(*cal,meet);
	}
	fclose(fp);
	return 1;
}


void StoreCal(Calendar_t *cal){
	FILE *fp;
	size_t i;

	fp = fopen(FILE_NAME,"w");

	fprintf(fp, "%lu\n", cal->meetingsCount);
	for (i=0; i<cal->meetingsCount; ++i){
		fprintf(fp, "%d %2.2f %2.2f\n",cal->meet[i]->room, cal->meet[i]->begin, cal->meet[i]->end);
	}
	fclose(fp);
}


Calendar_t* CreateCal(size_t capacity){
	Calendar_t *cal;

	if (capacity < 1){
		return NULL;
	}
	
	if (capacity < MIN_CAPACITY) {
		capacity = MIN_CAPACITY;
	}

	cal = malloc(sizeof(Calendar_t));
	if (NULL != cal) {
		cal->meet = malloc(sizeof(Meeting_t*)*capacity);
		if (NULL != cal->meet ) {
			cal->meetingsCount = 0;
			cal->capacity = capacity;
		}
		else {
			free(cal);
			return NULL;
		}
	}
	return cal;
} 


Meeting_t* CreateMeet(int room, float begin, float end){
	Meeting_t *meet;
	
	if (begin >= end || begin < 0 || begin >= 24 || end < 0 || end >= 24) {
		return NULL;
	}

	meet = malloc(sizeof(Meeting_t));
	if (NULL == meet) {return NULL;}
	meet->room = room;
	meet->begin = begin;
	meet->end = end;
	return meet;
}


int Insert(Calendar_t *cal ,Meeting_t *meet){
	size_t i,j;
	Meeting_t **tmp;

	if (NULL == meet || NULL == cal) {
		return -1;
	}

	/*first element*/
	if (0 == cal->meetingsCount) {
		cal->meet[0] = meet;
		cal->meetingsCount++;
		return 0;
	}

	/*Space check*/
	if (cal->meetingsCount == cal->capacity) {
		tmp = realloc(cal->meet, sizeof(Meeting_t)*INCREASE_RATIO*cal->capacity);
		if (NULL == tmp) {
			return -1;
		}

	cal->meet = tmp;
	cal->capacity *= INCREASE_RATIO;
	}

	/* Is the meeting should be the first element */
	if (cal->meet[0]->begin >= meet->end){
		/* Move one place rightside */
		for (i=cal->meetingsCount; i>0; --i){
			cal->meet[i] = cal->meet[i-1];
		}
		cal->meet[0] = meet;
		cal->meetingsCount++;	
		return 0;

	/* Is the meeting should be the last element */
	} else if (cal->meet[cal->meetingsCount-1]->end <= meet->begin){  	
		cal->meet[cal->meetingsCount] = meet;
		cal->meetingsCount++;	
		return 0;
	}

	/* Meeting is in the middle */
	for (i=0; i<cal->meetingsCount-1; ++i){
		if ((cal->meet[i]->end <= meet->begin) && (cal->meet[i+1]->begin >= meet->end)) {
			/* Move all the corresponding meetings one place right */
			for (j=cal->meetingsCount; j>i+1; --j) {
				cal->meet[j] = cal->meet[j-1];
			}
			cal->meet[i+1] = meet;
			cal->meetingsCount++;
			return 0;
		}
	}
	
	/* Meeting does not fit */
	return -2;
}


int DestroyCal(Calendar_t *cal) {
	size_t i;

	if (NULL == cal) {
		return -1;
	} 

	if (NULL != cal->meet) {
		for (i=0; i<cal->meetingsCount; ++i) {
			free(cal->meet[i]);
		}
	}

	free (cal->meet);
	free (cal);
	return 0;
}


void Print(Calendar_t *cal) {
	size_t i;
	
	if (NULL == cal || NULL == cal->meet) {
		return;
	}

	if (0 == cal->meetingsCount) {
		puts("The calendar is empty!");
		return;
	}
	
	puts("_________________________________");
	puts("| # | Room | Begining | Ending |");
	puts("_________________________________");
	for (i=0; i<cal->meetingsCount; ++i) {
		printf("|  %ld  |  %d  |  %2.2f  |  %2.2f  |\n", i+1,cal->meet[i]->room,cal->meet[i]->begin,cal->meet[i]->end);
	}
	puts("_________________________________");
}


int Removal(Calendar_t *cal,float begin){
	size_t i,j;

	for (i=0; i<cal->meetingsCount; ++i){
		if (cal->meet[i]->begin == begin) {
				free(cal->meet[i]);
			for (j=i; j<cal->meetingsCount; ++j){
				cal->meet[j] = cal->meet[j+1];
			}
			cal->meetingsCount--;
			return 0;
		}
	}
	/* Wasn't found */
	return 1;
}


/* Inner Function */

Meeting_t *search(Calendar_t *cal, float begin) {
	size_t i;
	int index = -1;

	for (i=0; i<cal->meetingsCount; ++i) {
		if (cal->meet[i]->begin == begin) {
			index = i;
			break;
		} else if (cal->meet[i]->begin > begin) {
			break;
		}
	}
	
	if (index != -1) {
		return cal->meet[index];
	} else {
		return NULL;
	}
}