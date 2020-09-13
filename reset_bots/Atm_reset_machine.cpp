#include "Atm_reset_machine.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_reset_machine& Atm_reset_machine::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                           ON_ENTER  ON_LOOP  ON_EXIT  EVT_INIT_PULL  EVT_INIT_RESET  EVT_SERVO_DONE  ELSE */
    /*          IDLE */          ENT_IDLE,      -1,      -1,  PERFORM_PULL,  PERFORM_RESET,             -1,   -1,
    /*   RETURN_HOME */   ENT_RETURN_HOME,      -1,      -1,  PERFORM_PULL,  PERFORM_RESET,           IDLE,   -1,
    /* PERFORM_RESET */ ENT_PERFORM_RESET,      -1,      -1,  PERFORM_PULL,             -1,    RETURN_HOME,   -1,
    /*  PERFORM_PULL */  ENT_PERFORM_PULL,      -1,      -1,            -1,  PERFORM_RESET,    RETURN_HOME,   -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int Atm_reset_machine::event( int id ) {
  switch ( id ) {
    case EVT_INIT_PULL:
      return 0;
    case EVT_INIT_RESET:
      return 0;
    case EVT_SERVO_DONE:
        if (this->resetServo.isMoving()) {
            return 0;
        }
        return 1;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void Atm_reset_machine::action( int id ) {
  switch ( id ) {
    case ENT_IDLE:
      return;
    case ENT_RETURN_HOME:
      this->resetServo.startEaseTo(RESET_MID_POINT);
      return;
    case ENT_PERFORM_RESET:
      this->resetServo.startEaseTo(RESET_RESET_POINT);
      return;
    case ENT_PERFORM_PULL:
      this->resetServo.startEaseTo(RESET_PULL_POINT);
      return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_reset_machine& Atm_reset_machine::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_reset_machine::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_reset_machine& Atm_reset_machine::init_pull() {
  trigger( EVT_INIT_PULL );
  return *this;
}

Atm_reset_machine& Atm_reset_machine::init_reset() {
  trigger( EVT_INIT_RESET );
  return *this;
}

Atm_reset_machine& Atm_reset_machine::servo_done() {
  trigger( EVT_SERVO_DONE );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_reset_machine& Atm_reset_machine::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "RESET_MACHINE\0EVT_INIT_PULL\0EVT_INIT_RESET\0EVT_SERVO_DONE\0ELSE\0IDLE\0RETURN_HOME\0PERFORM_RESET\0PERFORM_PULL" );
  return *this;
}
