#ifndef CALENDAR_H

#define CALENDAR_H

#define FILE_NAME "caldata"
#define MIN_CAPACITY 256
#define INCREASE_RATIO 2


#include <stdlib.h>

typedef struct {
	int room;
	float begin, end;
} Meeting_t;


typedef struct {
	Meeting_t **meet;
	size_t meetingsCount;
	size_t capacity;
} Calendar_t;



Calendar_t* CreateCal(size_t capacity);
Meeting_t* CreateMeet(int room, float begin, float end);
void Print(Calendar_t *myStruct);
int DestroyCal(Calendar_t *cal);
Meeting_t *search(Calendar_t *cal, float begin);
int Removal(Calendar_t *cal,float begin);
int Insert(Calendar_t *cal ,Meeting_t *meet);
int DoesCalExist(Calendar_t **cal);
void StoreCal(Calendar_t *cal);
#endif