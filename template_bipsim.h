/*
	
	Layers Demo for BipOS custom firmware
    v0.2
	
	by Enrico Rovere - https://github.com/enricorov/bipos-layersdemo
	
*/

#ifndef __LAYERS_DEMO_H__
#define __LAYERS_DEMO_H__
#define COLORS_COUNT 7

#include "bipui.h"


// sim_template.c
int main_app(int param0);

void show_screen(void *return_screen);
void key_press_screen();
int interactionHandler(void *param);
void refreshScreen();

void begin(app_data_t *app_data);
void simpleWindowCallbackFunction(Window_ *window, Way_ way);
void settingsWindowCallbackFunction(Window_ *window, Way_ way);
void changeWindowColourCallbackFunction(Layer_ *layer, short button_id);

void layerMainConstructor(Layer_ *layer);
void layerHelpConstructor(Layer_ *layer);
void layerSettingsConstructor(Layer_ *layer);

#endif