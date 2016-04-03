#ifndef _SPACECOMMS_H
#define _SPACECOMMS_H

#include <stdint.h>

typedef struct SPACK_DIR {
  unsigned packet:2;
  unsigned actaddr:5;
  unsigned actact:6;
  unsigned long :19;
} SPACK_DIR;

typedef struct CONS1_STATE {
  unsigned red_button:1;
  unsigned hotsystem1:1;
  unsigned hotsystem2:1;
  unsigned toggle_sw1:1;
  unsigned toggle_sw2:1;
  unsigned toggle_sw3:1;
  unsigned toggle_sw4:1;
  unsigned wormhole1:1;
  unsigned wormhole2:1; 
  unsigned long :24;
} CONS1_STATE;

typedef struct CONS2_STATE {
  unsigned red_button:1;
  unsigned hotsystem:1;
  unsigned green_button1:1;
  unsigned green_button2:1;
  unsigned green_button3:1;
  unsigned green_button4:1;
  unsigned pin_11:1;
  unsigned wormhole1:1;
  unsigned wormhole2:1;
  unsigned tri_state:2;
  unsigned slider:2;
  unsigned long :23;
} CONS2_STATE;

typedef struct CONS3_STATE {
  unsigned red_button:1;
  unsigned hotsystem1:1;
  unsigned hotsystem2:1;
  unsigned triangle1:1;
  unsigned triangle2:1;
  unsigned triangle3:1;
  unsigned pin_11:1;
  unsigned wormhole1:1;
  unsigned wormhole2:1;
  unsigned long :23;
} CONS3_STATE;

#endif