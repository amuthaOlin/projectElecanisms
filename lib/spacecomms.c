#include "spacecomms.h"

uint8_t CONS_NUMACTS[] = {11, 15, 13};
uint8_t CONS_STATES[][15] = {
    {
        2, // red_button:1
        2, // toggle1:1
        2, // toggle2:1
        2, // green_button:1
        9, // joystick:4
        2, // hotsystem:1
        8, // wordwheel_inside:4
        8, // wordwheel_outside:4
        2, // asteroid:4
        2, // wormhole1:1
        2  // wormhole2:1
    }, // not all states are "commandable" but every state must be captured here
    {
        2, // red_button:1
        2, // green_button1:1
        2, // green_button2:1
        2, // green_button3:1
        2, // green_button4:1
        3, // tri_state:2
        5, // slider:2
        2, // hotsystem1:1
        2, // hotsystem2:1
        2, // hotsystem3:1
        2, // hotsystem4:1
        4, // dial:3
        2, // asteroid:1
        2, // wormhole1:1
        2, // wormhole2:1
    },
    {
        2, // red_button:1
        2, // triangle1:1
        2, // triangle2:1
        2, // triangle3:1
        2, // toggle1:1
        2, // toggle2:1
        5, // clutch:2
        4, // arming:2
        2, // arming_button:1
        5, // dial:3
        2, // asteroid:1
        2, // wormhole1:1
        2, // wormhole2:1
    }
};
// 97 commands
uint8_t CONS_HASREST[][15] = {// has a "rest state" that cannot be commanded
    {
        1, // if it has rest it's just a momentary button, we can use things like "push"
        0, // if it doesn't then it's a "set"
        0,
        0,
        1,
        0,
        0,
        0,
        1,
        1,
        1
    },
    {
        1,
        0,
        0,
        0,
        0,
        1,
        1,
        0,
        0,
        0,
        0,
        1,
        1,
        1,
        1
    },
    {
        1,
        1,
        1,
        1,
        0,
        0,
        1,
        0,
        0,
        1,
        1,
        1,
        1
    }
};

uint8_t CONS_GROUP[][15] = {// has a "rest state" that cannot be commanded
    {
        4,
        0,
        0,
        1,
        2,
        3,
        5,
        5,
        6,
        7,
        7
    },
    {
        2,
        3,
        3,
        3,
        3,
        4,
        1,
        0,
        0,
        0,
        0,
        5,
        6,
        7,
        7
    },
    {
        2,
        3,
        3,
        3,
        4,
        4,
        0,
        5,
        5,
        1,
        6,
        7,
        7
    }
};

uint8_t CONS_GROUPNUM[][15] = {// has a "rest state" that cannot be commanded
    {
        0,
        1,
        2,
        0,
        0,
        0,
        1,
        2,
        0,
        1,
        2
    },
    {
        0,
        1,
        2,
        3,
        4,
        0,
        0,
        1,
        2,
        3,
        4,
        0,
        0,
        1,
        2
    },
    {
        0,
        1,
        2,
        3,
        1,
        2,
        0,
        1,
        2,
        0,
        0,
        1,
        2
    }
};