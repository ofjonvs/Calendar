/* 
   Jonas da Silva
   119289334
   jdasilva
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "calendar.h"

/* initializes a Calendar structure based on the parameters provided*/
int init_calendar(const char *name, int days,
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar){

  Calendar *new_calendar;
  if(calendar == NULL || name == NULL || days < 1){
    return FAILURE;
  }

  /*allocates calendar memory*/
  new_calendar = malloc(sizeof(Calendar));
  if(!new_calendar){
    return FAILURE;
  }

  /*allocates name memory*/
  new_calendar->name = malloc(strlen(name) + 1);
  if(new_calendar->name == NULL){
    return FAILURE;
  }
  strcpy(new_calendar->name, name);

  /*allocates event memory*/
  new_calendar->events = calloc(days, (sizeof(Event*)));
  if(new_calendar->events == NULL){
    return FAILURE;
  }

  new_calendar->days = days;
  new_calendar->total_events = 0;
  new_calendar->comp_func = comp_func;
  new_calendar->free_info_func = free_info_func;
  *calendar = new_calendar;
  if(*calendar == NULL){
    return FAILURE;
  }
  return SUCCESS;
}

/* prints, to the designated output stream, the calendar’s name, days,
   and total number of events if print all is true*/
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all){
  Event *current_node;
  int i;
  if(calendar == NULL || output_stream == NULL){
    return FAILURE;
  }

  /*prints calendar name, days and total events if print_all is true*/
  if(print_all){
    printf("Calendar's Name: \"%s\"\nDays: %d\nTotal Events: %d\n\n",
	   calendar->name, calendar->days, calendar->total_events);
  }
  printf("**** Events ****\n");
  /*prints nothing if there are no elements*/
  if(calendar->total_events == 0){
    return SUCCESS;
  }

  for(i = 0; i < calendar->days; i++){
    current_node = calendar->events[i];
    printf("Day %d\n", i + 1);

    /*iterates through events in the day and prints data for each one*/
    while(current_node){
      printf("Event's name: \"%s\", Start_time: %d, Duration: %d\n", 
	     current_node->name, current_node->start_time, 
	     current_node->duration_minutes);
      current_node = current_node->next;
    }
  }
  return SUCCESS;
}

/*adds the specified event to the list associated with the day parameter
  in increasing order*/
int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day){
  Event *current_node, *event, *e, *prev_node;
  int i;
  if(calendar == NULL || name == NULL || start_time <= 0 || 
    start_time >= 2400 || duration_minutes <= 0 || day < 1 ||
    day > calendar->days){
    return FAILURE;
  }

  /*checks if event is already in calendar*/
  for(i = 0; i < calendar->days; i++){
    e = calendar->events[i];
    while(e){
      if(strcmp(name, e->name) == 0){
	return FAILURE;
      }
      e = e->next;
    }
  }

  /*allocates memory to new event and its name*/
  event = malloc(sizeof(Event));
  event->name = malloc(strlen(name) + 1);
  if(!event || !(event->name)){
    return FAILURE;
  }

  /*initializes event*/
  strcpy(event->name, name);
  event->start_time = start_time;
  event->duration_minutes = duration_minutes;
  event->info = info; 
  event->next = NULL;

  /*if header is null*/
  if(!calendar->events[day - 1]){
    calendar->events[day - 1] = event;
    calendar->total_events++;
    return SUCCESS;
  }
  current_node = calendar->events[day-1];

  /*if event is to be added at header*/
  if(calendar->comp_func(event, current_node) < 0){
    event->next = current_node;
    calendar->events[day - 1] = event;
    calendar->total_events++;
    return SUCCESS;
  }

  /*iterates through day events*/
  while(current_node){
    /*finds where to insert event and does so*/
    if(calendar->comp_func(event, current_node) < 0){
      event->next = prev_node->next;
      prev_node->next = event;
      calendar->total_events++;
      return SUCCESS;
    }
    prev_node = current_node;
    current_node = current_node->next;
  }
   
  /*if reaches end then event is inserted to tail node*/
  event->next = prev_node->next;
  prev_node->next = event;
  calendar->total_events++;
  return SUCCESS;
}

/*returns a pointer via the out parameter event to the event
  with the specified name*/
int find_event(Calendar *calendar, const char *name, Event **event){
  Event *e;
  int i;
  if(calendar == NULL || name == NULL){
    return FAILURE;
  }
  /*iterates through days an calls find event in day*/
  for(i = 0; i < calendar->days; i++){
    if(find_event_in_day(calendar, name, i + 1, &e) == SUCCESS){
      *event = e;
      return SUCCESS;
    }
  }
  return FAILURE;

}

/*returns a pointer via the out parameter event to the event 
  with the specified name in the specified day*/
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event){
  Event *e;
  if(calendar == NULL || name == NULL || day < 1 || day > calendar->days 
     || event == NULL){
    return FAILURE;
  }

  e = calendar->events[day - 1];
  /*iterates through linked list and returns success and returns success and
    pointer to event if found*/
  while(e){
    if(strcmp(e->name, name) == 0){
      *event = e;
      return SUCCESS;
    }
    e = e->next;
  }
  return FAILURE;

}

/*removes the specified event from the calendar returning any memory 
  allocated for the event*/
int remove_event(Calendar *calendar, const char *name){
  Event *current_node, *prev_node;
  int i;
  if(calendar == NULL || name == NULL){
    return FAILURE;
  }

  /*iterates through days*/
  for(i = 0; i < calendar->days; i++){
    /*if no events, move to next day*/
    if(calendar->events[i] == NULL){
      continue;
    }
    /*if target event is head remove head*/
    current_node = calendar->events[i];
    if(strcmp(current_node->name, name) == 0){
      if(current_node->info && calendar->free_info_func){
	calendar->free_info_func(current_node->info);
      }
      calendar->events[i] = current_node->next;
      free(current_node->name);
      current_node->name = NULL;
      free(current_node);
      current_node = NULL;
      calendar->total_events--;
      return SUCCESS;
    }

    /*advances node and sets previous to head before iterating through
      events. When event is found it is removed*/
    prev_node = current_node;
    current_node = current_node->next;
    while(current_node){
      if(strcmp(current_node->name, name) == 0){
	if(current_node->info && calendar->free_info_func){
	  calendar->free_info_func(current_node->info);
	}
	prev_node->next = current_node->next;
	free(current_node->name);
	current_node->name = NULL;
	free(current_node);
	current_node = NULL;
	calendar->total_events--;
	return SUCCESS;
      }
      prev_node = current_node;
      current_node = current_node->next;
    }
  }  
  /*if event is not found return FAILURE*/
  return FAILURE;

}

/*returns the info pointer associated with the specified event*/
void *get_event_info(Calendar *calendar, const char *name){
  Event *e;
  int i;

  /*iterates through days*/
  for(i = 0; i < calendar->days; i ++){
    e = calendar->events[i];
    /*iterates through events. if event is found, info is returned*/
    while(e){
      if(strcmp(e->name, name) == 0){
	return e->info;
      }
      e = e->next;
    }
  }
  /*if name is not found return NULL*/
  return NULL;
}

/*remove all the event lists associated with the calendar and 
  set them to empty lists*/
int clear_calendar(Calendar *calendar){
  int i;
  if(calendar == NULL){
    return FAILURE;
  }

  /*iterates through days and clears each*/
  for(i = 0; i < calendar->days; i++){
    clear_day(calendar, i + 1);
  }
  calendar->total_events = 0;
  return SUCCESS;
}

/*removes all the events for the specified day setting the event list to 
  empty*/
int clear_day(Calendar *calendar, int day){
  Event *current_node, *prev_node;
  if(calendar == NULL || day < 1 || day > calendar->days){
    return FAILURE;
  }

  /*iterates through linked list*/
  current_node = calendar->events[day - 1];
  while(current_node){
    /*advances header node*/
    calendar->events[day - 1] = current_node->next;
    prev_node = current_node; 

    /*frees fields from node and decrements total events*/    
    if(current_node->info && calendar->free_info_func){
      calendar->free_info_func(current_node->info);
    }
    free(prev_node->name);      
    current_node->name = NULL;      
    free(prev_node);
    prev_node = NULL;
    calendar->total_events--;
    current_node = calendar->events[day - 1];
  }
  return SUCCESS;
}

/*returns memory that was dynamically-allocated for the calendar*/
int destroy_calendar(Calendar *calendar){
  if(calendar == NULL){
    return FAILURE;
  }

  /*frees all memory and handles dangling pointers*/
  clear_calendar(calendar);
  free(calendar->name);
  calendar->name = NULL;
  free(calendar->events);
  calendar->events = NULL;
  free(calendar);
  return SUCCESS;
}
