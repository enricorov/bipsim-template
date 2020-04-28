/*
	BipSIM template application
	v.0.1

	https://github.com/enricorov/bipsim-template
	
*/

#include "template_bipsim.h"

#ifdef __SIMULATION__
	#include <stdio.h>
	#include <string.h>
#endif
//	screen menu structure - each screen has its own
struct regmenu_ screen_data = {
	55,				  //	curr_screen - main screen number, value 0-255, for custom windows it is better to take from 50 and above
	1,				  //	swipe_scr - auxiliary screen number (usually 1)
	0,				  //	overlay - 0
	interactionHandler,  //          - pointer to the handler of interaction (touch, swipe, long press)
	key_press_screen, //	        - handler of short button press
	refreshScreen,	//	        - timer callback function and..
	0,				  //	            ..the variable passed to it
	show_screen,	  //	        - function writing to video buffer and..
	0,				  //              ..the variable passed to it
	0				  //	        - handler of long button press
};

#ifdef __SIMULATION__
int main_app(int param0)
	{
#else
int main(int param0, char **argv)
{ //	here the variable argv is not defined
#endif
	show_screen((void *)param0);
}

// CALLBACK FUNCTIONS - functions associated to objects i.e. buttons or layers

// CONSTRUCTORS - Defining elements, put the messy intializations here

// Utility functions

void show_screen(void *param0)
{
	#ifdef __SIMULATION__
		app_data_t *app_data = get_app_data_ptr();
		app_data_t **app_data_p = &app_data;
	#else
		app_data_t **app_data_p = get_ptr_temp_buf_2(); //	pointer to a pointer to screen data
		app_data_t *app_data;							//	pointer to screen data
	#endif

	Elf_proc_ *proc;

	// check the source at the procedure launch
	if ((param0 == *app_data_p) && get_var_menu_overlay())
	{ // return from the overlay screen (incoming call, notification, alarm, target, etc.)

		app_data = *app_data_p; //	the data pointer must be saved for the deletion
								//	release memory function reg_menu
		*app_data_p = NULL;		//	reset the pointer to pass it to the function reg_menu

		// 	create a new screen when the pointer temp_buf_2 is equal to 0 and the memory is not released
		reg_menu(&screen_data, 0); // 	menu_overlay=0

		*app_data_p = app_data;
	}
	else
	{ // if the function is started for the first time i.e. from the menu

		// create a screen (register in the system)
		reg_menu(&screen_data, 0);

		// allocate the necessary memory and place the data in it (the memory by the pointer stored at temp_buf_2 is released automatically by the function reg_menu of another screen)
		*app_data_p = (app_data_t *)pvPortMalloc(sizeof(app_data_t));
		app_data = *app_data_p; //	data pointer

		// clear the memory for data
		_memclr(app_data, sizeof(app_data_t));

		//	param0 value contains a pointer to the data of the running process structure Elf_proc_
		proc = param0;

		// remember the address of the pointer to the function you need to return to after finishing this screen
		if (param0 && proc->ret_f) //	if the return pointer is passed, then return to it
			app_data->ret_f = proc->elf_finish;
		else //	if not, to the watchface
			app_data->ret_f = show_watchface;

		#ifdef __SIMULATION__
			set_app_data_ptr(app_data);
		#endif

	}


}

void begin(app_data_t *app_data)
{

	// Do stuff

}

	void end(void)
{
	app_data_t *app_data = getAppData();

	destroyViewport(app_data);
	show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
}

void key_press_screen()
{
	app_data_t *app_data = getAppData();
	
};

void refreshScreen()
{ // triggered by set_update_period

	app_data_t *app_data = getAppData();

	refreshLayer(getActiveLayer(app_data), 1);
	vibrate(2, 50, 150);

}

int interactionHandler(void *param)
{
	app_data_t *app_data = getAppData();	
	struct gesture_ *gest = param;
	int result = 0;

	switch (gest->gesture)
	{
	case GESTURE_CLICK:
	{
		processTap(getActiveLayer(app_data), gest->touch_pos_x, gest->touch_pos_y);

		break;
	};
	case GESTURE_SWIPE_UP:
	{
		// implement your handler here	
		break;
	};
	case GESTURE_SWIPE_DOWN:
	{ 	

		refreshLayer(getActiveLayer(app_data), 1); // manual refresh
		set_update_period(0, 0);				   // removing scheduled refresh
		break;
	};
	case GESTURE_SWIPE_RIGHT:
	{	
		// implement your handler here
		break;
	};
	case GESTURE_SWIPE_LEFT:
	{
		// implement your handler here
		break;
	};

	default:
	{ // something went wrong ...

		break;
	};
	}
	//	}

	return result;
};