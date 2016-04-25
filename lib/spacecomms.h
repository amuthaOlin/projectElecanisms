#ifndef _SPACECOMMS_H
#define _SPACECOMMS_H

#include <stdint.h>

extern uint8_t CONS_NUMACTS[];
extern uint8_t CONS_STATES[][15];
extern uint8_t CONS_HASREST[][15];
extern uint8_t CONS_GROUP[][15];

typedef struct LCD_NAMES1 {
  unsigned packet:8;
  unsigned index1:8;
  unsigned index2:8;
  unsigned index3:8;
  unsigned index4:8;
  unsigned index5:8;
  unsigned index6:8;
  unsigned long :8;
} LCD_NAMES1;

typedef struct LCD_NAMES2 {
  unsigned packet:8;
  unsigned theme:8;
  unsigned mods:8;
  unsigned argument1:8;
  unsigned argument2:8;
  unsigned long :24;
} LCD_NAMES2;


typedef struct CONS1_STATE {
  unsigned red_button:1;        // 0
  unsigned toggle1:1;           // 1
  unsigned toggle2:1;           // 2
  unsigned green_button:1;      // 3
  unsigned joystick:4;          // 4
  unsigned hotsystem:1;         // 5
  unsigned wordwheel_inside:4;  // 6
  unsigned wordwheel_outside:4; // 7
  unsigned asteroid:1;          // 8
  unsigned wormhole1:1;         // 9
  unsigned wormhole2:1;         // 10
  unsigned long :13;
} CONS1_STATE;

typedef struct CONS2_STATE {
  unsigned red_button:1;        // 0
  unsigned green_button1:1;     // 1
  unsigned green_button2:1;     // 2
  unsigned green_button3:1;     // 3
  unsigned green_button4:1;     // 4
  unsigned tri_state:2;         // 5
  unsigned slider:2;            // 6
  unsigned hotsystem1:1;        // 7
  unsigned hotsystem2:1;        // 8
  unsigned hotsystem3:1;        // 9
  unsigned hotsystem4:1;        // 10
  unsigned dial:3;              // 11
  unsigned asteroid:1;          // 12
  unsigned wormhole1:1;         // 13
  unsigned wormhole2:1;         // 14
  unsigned long :13;
} CONS2_STATE;

typedef struct CONS3_STATE {
  unsigned red_button:1;        // 0
  unsigned triangle1:1;         // 1
  unsigned triangle2:1;         // 2
  unsigned triangle3:1;         // 3
  unsigned toggle1:1;           // 4
  unsigned toggle2:1;           // 5
  unsigned clutch:2;            // 6
  unsigned arming:2;            // 7
  unsigned arming_button:1;     // 8
  unsigned dial:3;              // 9
  unsigned asteroid:1;          // 10
  unsigned wormhole1:1;         // 11
  unsigned wormhole2:1;         // 12
  unsigned long :15;
} CONS3_STATE;



#endif