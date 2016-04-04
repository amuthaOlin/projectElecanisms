#include "spacecomms.h"

uint8_t CONS1_NUMACTS = 7;
uint8_t CONS1_STATES[] = {
    1, // red_button:1;
    2, // toggle1:1;
    2, // toggle2:1;
    8, // joystick:4;
    1, // wormhole1:1;
    1, // wormhole2:1;
    2  // hotsystem:1; 
};

uint8_t CONS2_NUMACTS = 13;
uint8_t CONS2_STATES[] = {
    1, // red_button:1;
    1, // green_button1:1;
    1, // green_button2:1;
    1, // green_button3:1;
    1, // green_button4:1;
    1, // wormhole1:1;
    1, // wormhole2:1;
    2, // tri_state:2;
    4, // slider:2;
    2, // hotsystem1:1;
    2, // hotsystem2:1;
    2, // hotsystem3:1;
    2  // hotsystem4:1;
};

uint8_t CONS3_NUMACTS = 8;
uint8_t CONS3_STATES[] = {
    1, // red_button:1;
    1, // triangle1:1;
    1, // triangle2:1;
    1, // triangle3:1;
    2, // toggle1:1;
    2, // toggle2:1;
    1, // wormhole1:1;
    1  // wormhole2:1;
};