#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "ui.h"
#define NUM_LINES 20

static char *arr[NUM_LINES];
static int last_msg = 0;
static pthread_mutex_t log_mutex;

void print_stars_dots(int start_x, int y, int count) {
	move(y, start_x);
	for(int i = 0; i < count / 10; i++) {
		move(y, start_x);
		start_x++;
		addch('*');
	}
	addch(count % 10 + 48);
}

void init() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	for(int i = 0; i < NUM_LINES; i++) {
		arr[i] = malloc(2 * sizeof(char));
		strcpy(arr[i], "");
	}
	
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	pthread_mutex_init(&log_mutex, NULL);
}

void game_end(int zombies) {
	 endwin();
	 pthread_mutex_destroy(&log_mutex);
	 printf("High score: %d\n", zombies * 100);
	 exit(0);
}

int get_input() {
	return getch();
}

void do_print_msg(char *msg, int color) {
	pthread_mutex_lock(&log_mutex);
	free(arr[last_msg]);
	arr[last_msg] = malloc((strlen(msg) + 1) * sizeof(char));
	strcpy(arr[last_msg], msg);
	last_msg = (last_msg + 1) % NUM_LINES;
	
	for(int i = 0; i < NUM_LINES; i++) {
		move(10 + i, 0);
		clrtoeol();
		if (color > 0 && NUM_LINES == i + 1) {
			attron(COLOR_PAIR(color));
		}
		
		printw(arr[(last_msg + i) % NUM_LINES]);
		
		if (color > 0 && NUM_LINES == i + 1) {
			attroff(COLOR_PAIR(color));
		}
	}
	pthread_mutex_unlock(&log_mutex);
	refresh();
	
}

void print_msg(char *msg) {
	do_print_msg(msg, 0);
}

void print_fail(char *msg) {
	do_print_msg(msg, 1);
}

void print_succ(char *msg) {
	do_print_msg(msg, 2);
}


void print_gold(int gold) {
	move(8, 0);
	clrtoeol();
	printw("Gold: %d", gold);
	refresh();
}

void print_soldiers(int soldiers) {
	int pos;
	move(6, 0);
	clrtoeol();
	printw("Soldiers:");
	print_stars_dots(10, 6, soldiers);
	refresh();
}

void print_zombies(int distance, int zombies) {
	int pos;
	for(int i = 0; i <= 5; i++) {
		move(i, 0);
		clrtoeol(); 
	}
	move(5 - distance, 0);
	clrtoeol(); 
	printw("Zombies:");
	pos = 10;
	print_stars_dots(10, 5 - distance, zombies);
	refresh();
}

void print_health(int health) {
	move(7, 0);
	clrtoeol(); 
	printw("Health:");
	attron(COLOR_PAIR(1));
	print_stars_dots(10, 7, health);
	attroff(COLOR_PAIR(1));
	refresh();
}





