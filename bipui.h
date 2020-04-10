#pragma once

/////////////////////////////////////////////////////////////
//
//  BIPUI GRAPHICS FRAMEWORK v.0.0
//	for BipOS
//  (C) Enrico Rovere 2020      <enricorov@gmail.com>
//
/////////////////////////////////////////////////////////////
/****
v.0.0
-   unreleased

*****/


#ifndef __BIPUI_H__
#define __BIPUI_H__

#ifdef __SIMULATION__
    #include <simulator.h>
#else
    #include "libbip_EN.h"
#endif


// Debug defines, enable to see debug prints to screen



#ifndef __SIMULATION__

#define MAX_NUM_BUTTONS 8
#define MAX_SIZE_BUTTON_LABEL 20
#define MAX_SIZE_TEXT_BOX 120

#define MAX_NUM_LAYERS 2

#define DEFAULT_BORDER_THICKNESS 4 // minimum reasonable distance of button edge to screen

#define DEFAULT_TEXT_HEIGHT 25


typedef struct Point_
{

    short x,
        y;
} Point_;

typedef enum Caffeine_t
{

    WEAK,  // 0 - screen backlight will go off
    STRONG // 1 - backlight always on

} Caffeine_t;

typedef enum Style_t
{

    BUTTON_STYLE_DEFAULT_SQUARED,
    BUTTON_STYLE_SQUARED_NOBORDER,
    BUTTON_STYLE_ROUNDED_NOBORDER

} Style_t;

typedef enum Way_
{

    UP,
    DOWN,
    LEFT,
    RIGHT

} Way_;

typedef struct ButtonParams_
{
    Style_t style;

} ButtonParams_;

typedef struct Button_
{

    Point_ topLeft,
        bottomRight;

    char label[MAX_SIZE_BUTTON_LABEL];

    short border, // color of button features
        filling,
        text;

    void (*callbackFunction)();

    ButtonParams_ params; // style, state..

} Button_;

typedef struct TextBox_
{

    Point_ topLeft,
        bottomRight;

    char body[MAX_SIZE_TEXT_BOX];

    short colour,
        background;

} TextBox_;

typedef struct LayerParams_
{

    //char overlay; // 1: something was drawn on top of the layer and it should be refreshed
    short refreshDelay;

} LayerParams_;

typedef struct Layer_
{

    Button_ buttonArray[MAX_NUM_BUTTONS]; // all buttons
    unsigned short index;                 // current valid button, init=0

    short backgroundColour; // background for the current Layer
    short visible;          // is the layer visible?
    TextBox_ textBox;       // textbox for general usage

    LayerParams_ params; // holding state of the layer
    void (*callbackFunction)();
} Layer_;

typedef struct Window_
{

    Layer_ layerArray[MAX_NUM_LAYERS];
    short index;

} Window_;
typedef struct Viewport_
{

    Layer_ *active; // layer currently drawn
    Layer_ *up;     // pointers to layers on all sides
    Layer_ *down;
    Layer_ *left;
    Layer_ *right;

} Viewport_;

typedef struct app_data_t
{
    void *ret_f; //	the address of the return function

    Viewport_ vp;
} app_data_t;
#endif

// CONSTANTS ----------------------

//#ifdef __cplusplus
        const static Point_ BIPUI_TOP_LEFT_POINT = {

        DEFAULT_BORDER_THICKNESS,
        DEFAULT_BORDER_THICKNESS
        };

    const static Point_ BIPUI_TOP_RIGHT_POINT = {

        DEFAULT_BORDER_THICKNESS,
        VIDEO_X - DEFAULT_BORDER_THICKNESS
        };

    const static Point_ BIPUI_BOTTOM_LEFT_POINT = {

        DEFAULT_BORDER_THICKNESS,
        VIDEO_X - DEFAULT_BORDER_THICKNESS
        };

    const static Point_ BIPUI_BOTTOM_RIGHT_POINT = {

        VIDEO_Y - DEFAULT_BORDER_THICKNESS,
        VIDEO_X - DEFAULT_BORDER_THICKNESS
        };

    const static TextBox_ DEFAULT_TEXTBOX = {

        {10, 10},
        {VIDEO_Y - 5, VIDEO_X - 5},

        "TEXTBOX SAMPLE",

        COLOR_SH_RED,
        COLOR_SH_BLACK
    };

    const static ButtonParams_ DEFAULT_BUTTON_PARAMETERS = {

        BUTTON_STYLE_DEFAULT_SQUARED

    };

    const static Button_ DEFAULT_BUTTON_INSTANCE = {

        {50, 50},
        {100, 100},

        "UNDEFINED",
        COLOR_SH_WHITE,
        COLOR_SH_PURPLE,
        COLOR_SH_WHITE,

        0,

        {0}
    };
    
/* #else
    const static Point_ BIPUI_TOP_LEFT_POINT = {

        .x = DEFAULT_BORDER_THICKNESS,
        .y = DEFAULT_BORDER_THICKNESS};

    const static Point_ BIPUI_TOP_RIGHT_POINT = {

        .x = DEFAULT_BORDER_THICKNESS,
        .y = VIDEO_X - DEFAULT_BORDER_THICKNESS};

    const static Point_ BIPUI_BOTTOM_LEFT_POINT = {

        .x = DEFAULT_BORDER_THICKNESS,
        .y = VIDEO_X - DEFAULT_BORDER_THICKNESS};

    const static Point_ BIPUI_BOTTOM_RIGHT_POINT = {

        .x = VIDEO_Y - DEFAULT_BORDER_THICKNESS,
        .y = VIDEO_X - DEFAULT_BORDER_THICKNESS};

    const static TextBox_ DEFAULT_TEXTBOX = {

        .topLeft = {10, 10},
        .bottomRight = {VIDEO_Y - 5, VIDEO_X - 5},

        .body = "TEXTBOX SAMPLE",

        .colour = COLOR_SH_RED,
        .background = COLOR_SH_BLACK

    };

    const static ButtonParams_ DEFAULT_BUTTON_PARAMETERS = {

        .style = BUTTON_STYLE_DEFAULT_SQUARED

    };

    const static Button_ DEFAULT_BUTTON_INSTANCE = {

        .topLeft = {50, 50},
        .bottomRight = {100, 100},

        .label = "UNDEFINED",
        .border = COLOR_SH_WHITE,
        .filling = COLOR_SH_PURPLE,
        .text = COLOR_SH_WHITE,

        .callbackFunction = 0,

        .params = {0}

    };
#endif */
// PROTOTYPES --------------------------

void initButton(Button_ *button, Point_ topLeft, Point_ bottomRight, // initialize button with these parameters
                char *label, short border, short filling, short text, void *callbackFunction);
void spawnButton(Button_ *button, Layer_ *layer);       // adds button to layer and draws it - note: graphics are shown only after calling refresh_screen_lines()
void drawButton(Button_ *button);                       // draws a button
short addButtonToLayer(Button_ *button, Layer_ *layer); // adds button to layer without drawing it

long getLongColour(short colour); // returns long from short versions
void caffeine(Caffeine_t coffee); // set display backlight
void setLayerBackground(Layer_ *layer, short colour);
Layer_ *createLayer(void);
void destroyLayer(Layer_ *layer);
Layer_ *getActiveLayer(app_data_t *app_data);                           // returns layer currently in use
Layer_ *getTopLayer(app_data_t *app_data);                              // returns topmost layer
void processTap(Layer_ *layer, int x, int y);                           // iterates over a layer for the button corresponding to a tap
Button_ moveInDirectionButton(Button_ *button, Way_ dir, short offset); // given a button, it changes its parameters to move it.
Button_ mirrorInDirectionButton(Button_ *button, Way_ dir);             // mirrors a button with respect to one of the four axes
short findHighestOpaqueLayer(Window_ *window);                          // returns the highest indexed layer with bg != COLOR_SH_MASK
Window_ *getCurrentWindow(app_data_t *app_data);
short getCurrentLayerIndex(Window_ *window);
void initializeLayer(Layer_ *layer);                // setting layer params to default
void setLayerTextBox(Layer_ *layer, TextBox_ tbox); // setting text box for a given layer
short addLayerToWindow(Layer_ *layer, Window_ *window);
void spawnLayer(Layer_ *layer, Window_ *window);
void refreshWindow(Window_ *window);
void drawTextBox(TextBox_ *textbox);
void refreshLayer(Layer_ *layer, short repaint);
void initializeWindow(Window_ *window);
void initializeViewport(Viewport_ *wp);
Viewport_ *createViewport(void);
Viewport_ *getCurrentViewport(app_data_t *app_data);
void setViewportLayer(Viewport_ *vp, Layer_ *layer, Way_ dir);
void setActiveLayerViewport(Viewport_ *vp, Layer_ *layer);
void destroyViewport(Viewport_ *vp); // destroy the viewport
void blank_screen(void);
void printErrorText(char *error);
char getActiveOverlayValue(Layer_ *layer);
void resetActiveOverlayValue(Layer_ *layer);
void setActiveOverlayValue(Layer_ *layer);

#endif