#include "spacecomms.h"

uint8_t CONS1_NUMACTS = 7;
uint8_t CONS1_STATES[] = {
    2, // red_button:1;
    2, // toggle1:1;
    2, // toggle2:1;
    9, // joystick:4;
    2, // wormhole1:1;
    2, // wormhole2:1;
    2  // hotsystem:1; 
}; // not all states are "commandable" but every state must be captured here

uint8_t CONS2_NUMACTS = 13;
uint8_t CONS2_STATES[] = {
    2, // red_button:1;
    2, // green_button1:1;
    2, // green_button2:1;
    2, // green_button3:1;
    2, // green_button4:1;
    2, // wormhole1:1;
    2, // wormhole2:1;
    2, // tri_state:2;
    4, // slider:2;
    2, // hotsystem1:1;
    2, // hotsystem2:1;
    2, // hotsystem3:1;
    2  // hotsystem4:1;
};

uint8_t CONS3_NUMACTS = 8;
uint8_t CONS3_STATES[] = {
    2, // red_button:1;
    2, // triangle1:1;
    2, // triangle2:1;
    2, // triangle3:1;
    2, // toggle1:1;
    2, // toggle2:1;
    2, // wormhole1:1;
    2  // wormhole2:1;
};

uint8_t CONS1_HASREST[] = { // has a "rest state" that cannot be commanded
    1,
    0,
    0,
    1,
    1,
    1,
    0
};


uint8_t CONS2_HASREST[] = { // has a "rest state" that cannot be commanded
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    0
};


uint8_t CONS1_HASREST[] = { // has a "rest state" that cannot be commanded
    1,
    1,
    1,
    1,
    0,
    0,
    1,
    1
}