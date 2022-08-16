/*
  Jonas da Silva
  119289334
  jdasilva
*/
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
  return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

static void free_info(void *ptr) {
  char *info = (char *)ptr;
  free(info);
}

/* This test checks calendar initialization and print calendar */
static int test1() {
  int days = 7;
  Calendar *calendar;

  if(init_calendar(NULL, days, comp_minutes, NULL, &calendar) == SUCCESS){
    return FAILURE;
  }
  else if(init_calendar("Spr", 0, comp_minutes, NULL, &calendar) == SUCCESS){
    return FAILURE;
  }
  else if(init_calendar("Spr", days, comp_minutes, NULL, NULL) == SUCCESS){
    return FAILURE;
  }
  else if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) 
            == SUCCESS) {
    if(print_calendar(NULL, stdout, 1) == SUCCESS){
      return FAILURE;
    }
    if(print_calendar(calendar, NULL, 1) == SUCCESS){
      return FAILURE;
    }
    if (print_calendar(calendar, stdout, 1) == SUCCESS) {
      return destroy_calendar(calendar);
    }
  }
  return FAILURE;
}

/*checks add event*/
static int test2() {
  int days = 7;
  Calendar *calendar;
  Event *current_node;

  /*inserts events*/
  init_calendar("Spr", days, comp_minutes, NULL, &calendar);
  if(add_event(calendar, "breakfast", 100, 30, NULL, 1) == FAILURE){
    return FAILURE;
  }
  else if(add_event(calendar, "breakfast", 100, 30, NULL, 1) == SUCCESS){
    return FAILURE;
  }
  else if(add_event(calendar, "lunch", 200, 40, NULL, 1) == FAILURE){
    return FAILURE;
  }
  else if(add_event(calendar, "dinner", 300, 20, NULL, 1) == FAILURE){
    return FAILURE;
  }
  else if(add_event(calendar, "day2", 1000, 100, NULL, 2) == FAILURE){
    return FAILURE;
  }
  else if(add_event(calendar, "day3", 2300, 15, NULL, 3) == FAILURE){
    return FAILURE;
  }
  else if(add_event(calendar, "day4", 2300, 15, NULL, 4) == FAILURE){
    return FAILURE;
  }
  else if(add_event(calendar, "day7", 2300, 15, NULL, 7) == FAILURE){
    return FAILURE;
  }
  else if(add_event(NULL, "invalid1", 1000, 15, NULL, 5) == SUCCESS){
    return FAILURE;
  }
  else if(add_event(calendar, NULL, 1000, 15, NULL, 5) == SUCCESS){
    return FAILURE;
  }
  else if(add_event(calendar, "invalid2", -1, 15, NULL, 5) == SUCCESS){
    return FAILURE;
  }
  else if(add_event(calendar, "invalid2", 2401, 15, NULL, 5) == SUCCESS){
    return FAILURE;
  }
  else if(add_event(calendar, "invalid2", 1000, 15, NULL, 8) == SUCCESS){
    return FAILURE;
  }
  else if(add_event(calendar, "invalid2", 1000, 15, NULL, 0) == SUCCESS){
    return FAILURE;
  }
  else if(add_event(calendar, "invalid2", 1000, 0, NULL, 5) == SUCCESS){
    return FAILURE;
  }
  /*checks if events were inserted correctly*/
  else if(print_calendar(calendar, stdout, 1) == SUCCESS){
    current_node = calendar->events[0];
    if(strcmp(current_node->name, "dinner") != 0){
      return FAILURE;
    }
    current_node = current_node->next;
    if(strcmp(current_node->name, "breakfast") != 0){
      return FAILURE;
    }
    current_node = current_node->next;
    if(strcmp(current_node->name, "lunch") != 0){
      return FAILURE;
    }
    current_node = current_node->next;
    if(current_node != NULL){
      return FAILURE;
    }
    current_node = calendar->events[1];
    if(strcmp(current_node->name, "day2") != 0){
      return FAILURE;
    }
    current_node = calendar->events[2];
    if(strcmp(current_node->name, "day3") != 0){
      return FAILURE;
    }
    current_node = calendar->events[3];
    if(strcmp(current_node->name, "day4") != 0){
      return FAILURE;
    }
    current_node = calendar->events[4];
    if(current_node != NULL){
      return FAILURE;
    }
    current_node = calendar->events[5];
    if(current_node != NULL){
      return FAILURE;
    }
    current_node = calendar->events[6];
    if(strcmp(current_node->name, "day7") != 0){
      return FAILURE;
    }
    if(calendar->total_events != 7){
      return FAILURE;
    }
    return destroy_calendar(calendar);
      
  }
  return FAILURE;
}

/*tests find event and find in day*/
static int test3(){
  int days = 7;
  Calendar *calendar;
  Event *current_node, *event;

  init_calendar("Spr", days, comp_minutes, NULL, &calendar);
  add_event(calendar, "breakfast", 100, 30, NULL, 1);
  add_event(calendar, "lunch", 200, 40, NULL, 1);
  add_event(calendar, "dinner", 300, 20, NULL, 1);
  add_event(calendar, "day2", 1000, 100, NULL, 2);
  add_event(calendar, "day3", 2300, 15, NULL, 3);
  add_event(calendar, "day4", 2300, 15, NULL, 4);
  add_event(calendar, "day7", 2300, 15, NULL, 7);

  /*tests if find event was successful and if the event returned is equal
    to the respective node in calendar*/
  current_node = calendar->events[0];
  if (find_event(calendar, "dinner", &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  if (find_event_in_day(calendar, "dinner", 1, &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  current_node = current_node->next;
  if (find_event(calendar, "breakfast", &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  if (find_event_in_day(calendar, "breakfast", 1, &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  current_node = current_node->next;
  if (find_event(calendar, "lunch", &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  if (find_event_in_day(calendar, "lunch", 1, &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  current_node = calendar->events[1];
  if (find_event(calendar, "day2", &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  if (find_event_in_day(calendar, "day2", 2, &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  current_node = calendar->events[2];
  if (find_event(calendar, "day3", &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  if (find_event_in_day(calendar, "day3", 3, &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  current_node = calendar->events[3];
  if (find_event(calendar, "day4", &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  current_node = calendar->events[6];
  if (find_event(calendar, "day7", &event) == SUCCESS){
    if(current_node != event){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  /*tests if finding non existing event is unsuccessful and if event
    is not modified*/
  if(find_event(calendar, "non existing event", &event) == SUCCESS){
    return FAILURE;
  }
  if(current_node != event){
    return FAILURE;
  }
  if(find_event_in_day(calendar, "non existing event", 1, &event) == SUCCESS){
    return FAILURE;
  }
  if(current_node != event){
    return FAILURE;
  }
  return destroy_calendar(calendar);
}

/*test remove event*/
static int test4(){
  int days = 7;
  Calendar *calendar;
  Event *current_node;

  init_calendar("Spr", days, comp_minutes, NULL, &calendar);
  add_event(calendar, "breakfast", 100, 30, NULL, 1);
  add_event(calendar, "lunch", 200, 40, NULL, 1);
  add_event(calendar, "dinner", 300, 20, NULL, 1);
  add_event(calendar, "day2", 1000, 100, NULL, 2);
  add_event(calendar, "day3", 2300, 15, NULL, 3);
  add_event(calendar, "day4", 2300, 15, NULL, 4);
  add_event(calendar, "day7", 2300, 15, NULL, 7);

  /*tests invalid parameters*/
  if(remove_event(NULL, "dinner") == SUCCESS || remove_event(calendar, NULL)
     == SUCCESS || remove_event(calendar, "non existent event") == SUCCESS){
    return FAILURE;
  }

  /*tests removals for all day 1 events and if header is null after
    all have been removed*/
  current_node = calendar->events[0];
  current_node = current_node->next;
  if(remove_event(calendar, "dinner") == SUCCESS){
    if(calendar->events[0] != current_node){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  add_event(calendar, "dinner", 300, 20, NULL, 1);
  if(remove_event(calendar, "breakfast") == SUCCESS){
    current_node = calendar->events[0];
    current_node = current_node->next;
    if(strcmp(current_node->name, "lunch") != 0){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  add_event(calendar, "breakfast", 100, 30, NULL, 1);
  if(remove_event(calendar, "lunch") == SUCCESS){
    current_node = calendar->events[0];
    current_node = current_node->next;
    if(strcmp(current_node->name, "breakfast") != 0){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  remove_event(calendar, "breakfast");
  remove_event(calendar, "dinner");
  if(calendar->events[0] != NULL){
    return FAILURE;
  }

  /*tests removals for days 2-7*/
  if(remove_event(calendar, "day2") == SUCCESS){
    if(calendar->events[1] != NULL){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  if(remove_event(calendar, "day3") == SUCCESS){
    if(calendar->events[2] != NULL){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  if(remove_event(calendar, "day4") == SUCCESS){
    if(calendar->events[3] != NULL){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }
  if(remove_event(calendar, "day7") == SUCCESS){
    if(calendar->events[6] != NULL){
      return FAILURE;
    }
  }
  else{
    return FAILURE;
  }

  return destroy_calendar(calendar);
}


static int test5(){
  char info[7] = "cereal", *info_ptr;
  int days = 7;
  Calendar *calendar;

  if(init_calendar("Spr", days, comp_minutes, free_info, &calendar) == FAILURE){
    return FAILURE;
  }
  /*allocates memory to info pointer and copies info string into it*/
  info_ptr = malloc(strlen(info) + 1);
  if(!info_ptr){
    return FAILURE;
  }
  strcpy(info_ptr, info);
  add_event(calendar, "breakfast", 100, 30, info_ptr, 1);
  add_event(calendar, "lunch", 200, 40, NULL, 1);
  add_event(calendar, "dinner", 300, 20, NULL, 1);
  add_event(calendar, "day2", 1000, 100, NULL, 2);
  add_event(calendar, "day3", 2300, 15, NULL, 3);
  add_event(calendar, "day4", 2300, 15, NULL, 4);
  add_event(calendar, "day7", 2300, 15, NULL, 7);
  /*test if info is correct*/
  if(strcmp(get_event_info(calendar, "breakfast"), "cereal") != 0){
    return FAILURE;
  }

  return destroy_calendar(calendar);
}

/*tests clear calendar and clear day*/
static int test6(){
  int days = 7, i;
  Calendar *calendar;

  init_calendar("Spr", days, comp_minutes, NULL, &calendar);
  add_event(calendar, "breakfast", 100, 30, NULL, 1);
  add_event(calendar, "lunch", 200, 40, NULL, 1);
  add_event(calendar, "dinner", 300, 20, NULL, 1);
  add_event(calendar, "day2", 1000, 100, NULL, 2);
  add_event(calendar, "day3", 2300, 15, NULL, 3);
  add_event(calendar, "day4", 2300, 15, NULL, 4);
  add_event(calendar, "day7", 2300, 15, NULL, 7);

  /*tests clear day*/
  if(clear_day(NULL, 1) == SUCCESS){
    return FAILURE;
  }
  if(clear_day(calendar, 0) == SUCCESS){
    return FAILURE;
  }
  if(clear_day(calendar, 8) == SUCCESS){
    return FAILURE;
  }
  if(clear_day(calendar, 1) == FAILURE){
    return FAILURE;
  }
  if(calendar->events[0] != NULL){
    return FAILURE;
  }
  if(calendar->total_events != 4){
    return FAILURE;
  }
  /*tests clear calendar*/
  if(clear_calendar(NULL) == SUCCESS){
    return FAILURE;
  }
  if(clear_calendar(calendar) == FAILURE){
    return FAILURE;
  }
  for(i = 0; i < calendar->days; i++){
    if(calendar->events[i] != NULL){
      return FAILURE;
    }
  }
  if(calendar->total_events != 0){
    return FAILURE;
  }

  return destroy_calendar(calendar);
}


int main() {
  int result = SUCCESS;

  /***** Starting memory checking *****/
  start_memory_check();
  /***** Starting memory checking *****/

  if (test1() == FAILURE) result = FAILURE;
  if (test2() == FAILURE) result = FAILURE;
  if (test3() == FAILURE) result = FAILURE;
  if (test4() == FAILURE) result = FAILURE;
  if (test5() == FAILURE) result = FAILURE;
  if (test6() == FAILURE) result = FAILURE;

  /****** Gathering memory checking info *****/
  stop_memory_check();
  /****** Gathering memory checking info *****/
   
  if (result == FAILURE) {
    printf("\n\n-------TEST FAILED---------\n\n");
    exit(EXIT_FAILURE);
  }

  printf("\n\n-------TEST SUCCESSFUL-------\n\n");
  return EXIT_SUCCESS;
}
