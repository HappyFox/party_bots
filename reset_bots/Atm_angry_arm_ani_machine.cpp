#include "Atm_angry_arm_ani_machine.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_angry_arm_ani_machine& Atm_angry_arm_ani_machine::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                             ON_ENTER  ON_LOOP          ON_EXIT     EVT_START  EVT_STOP  EVT_PAUSE_DONE  EVT_ARM_WAVE_DONE  EVT_SERVO_DONE  ELSE */
    /*           IDLE */           ENT_IDLE,      -1,              -1, ANGRY_ARM_UP,       -1,             -1,                -1,             -1,   -1,
    /*   ANGRY_ARM_UP */   ENT_ANGRY_ARM_UP,      -1,              -1,           -1,     IDLE,             -1,       ANGRY_PAUSE, ANGRY_ARM_DOWN,   -1,
    /* ANGRY_ARM_DOWN */ ENT_ANGRY_ARM_DOWN,      -1,              -1,           -1,     IDLE,             -1,       ANGRY_PAUSE,   ANGRY_ARM_UP,   -1,
    /*    ANGRY_PAUSE */    ENT_ANGRY_PAUSE,      -1, EXT_ANGRY_PAUSE,           -1,     IDLE,   ANGRY_ARM_UP,                -1,             -1,   -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int Atm_angry_arm_ani_machine::event( int id ) {
  switch ( id ) {
    case EVT_START:
      return 0;
    case EVT_STOP:
      return 0;
    case EVT_PAUSE_DONE:
      return 0;
    case EVT_ARM_WAVE_DONE:
      return 0;
    case EVT_SERVO_DONE:
        if (this->leftServo.isMoving()) {
            return 0;
        }
        if (this->rightServo.isMoving()) {
            return 0;
        }
        return 1;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_angry_arm_ani_machine::action( int id ) {
  switch ( id ) {
    case ENT_IDLE:
        this->leftServo.startEaseTo(ARM_MID_POINT, ARM_SPEED);
        this->rightServo.startEaseTo(ARM_MID_POINT, ARM_SPEED);
        this->eyes[0] = CRGB::Green;
        this->eyes[1] = CRGB::Green;
        FastLED.show();
        return;
    case ENT_ANGRY_ARM_UP:
        this->leftServo.startEaseTo(ARM_UP, ARM_SPEED);
        this->rightServo.startEaseTo(ARM_DOWN, ARM_SPEED);
        this->eyes[0] = CRGB::Purple;
        this->eyes[1] = CRGB::Yellow;
        FastLED.show();
        return;
    case ENT_ANGRY_ARM_DOWN:
        this->leftServo.startEaseTo(ARM_DOWN, ARM_SPEED);
        this->rightServo.startEaseTo(ARM_UP, ARM_SPEED);
        this->eyes[0] = CRGB::Yellow;
        this->eyes[1] = CRGB::Purple;
        FastLED.show();
        return;
    case ENT_ANGRY_PAUSE:
        this->leftServo.startEaseTo(ARM_MID_POINT, ARM_SPEED);
        this->rightServo.startEaseTo(ARM_MID_POINT, ARM_SPEED);
        return;
    case EXT_ANGRY_PAUSE:
        return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_angry_arm_ani_machine& Atm_angry_arm_ani_machine::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_angry_arm_ani_machine::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_angry_arm_ani_machine& Atm_angry_arm_ani_machine::start() {
  trigger( EVT_START );
  return *this;
}

Atm_angry_arm_ani_machine& Atm_angry_arm_ani_machine::stop() {
  trigger( EVT_STOP );
  return *this;
}

Atm_angry_arm_ani_machine& Atm_angry_arm_ani_machine::pause_done() {
  trigger( EVT_PAUSE_DONE );
  return *this;
}

Atm_angry_arm_ani_machine& Atm_angry_arm_ani_machine::arm_wave_done() {
  trigger( EVT_ARM_WAVE_DONE );
  return *this;
}

Atm_angry_arm_ani_machine& Atm_angry_arm_ani_machine::servo_done() {
  trigger( EVT_SERVO_DONE );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_angry_arm_ani_machine& Atm_angry_arm_ani_machine::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "ANGRY_ARM_ANI_MACHINE\0EVT_START\0EVT_STOP\0EVT_PAUSE_DONE\0EVT_ARM_WAVE_DONE\0EVT_SERVO_DONE\0ELSE\0IDLE\0ANGRY_ARM_UP\0ANGRY_ARM_DOWN\0ANGRY_PAUSE" );
  return *this;
}
