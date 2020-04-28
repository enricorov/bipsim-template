/*
	BipSIM template application
	v.0.1

	https://github.com/enricorov/bipsim-template
	
*/

#ifndef __SIM_TEMPLATE_H__
#define __SIM_TEMPLATE_H__


#include "bipui.h"


// sim_template.c
int main_app(int param0);
void show_screen(void *return_screen);
void key_press_screen();
int interactionHandler(void *param);
void refreshScreen();

void begin(app_data_t *app_data);
void end(void);

#endif