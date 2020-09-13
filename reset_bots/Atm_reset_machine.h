#pragma once

#include <Automaton.h>

#include <ServoEasing.h>


#include "servos.h"


class Atm_reset_machine: public Machine {

 public:
  enum { IDLE, RETURN_HOME, PERFORM_RESET, PERFORM_PULL }; // STATES
  enum { EVT_INIT_PULL, EVT_INIT_RESET, EVT_SERVO_DONE, ELSE }; // EVENTS
  Atm_reset_machine( ServoEasing &ResetServo) : resetServo(ResetServo) {};
  Atm_reset_machine& begin( void );
  Atm_reset_machine& trace( Stream & stream );
  Atm_reset_machine& trigger( int event );
  int state( void );
  Atm_reset_machine& init_pull( void );
  Atm_reset_machine& init_reset( void );
  Atm_reset_machine& servo_done( void );

 private:
  enum { ENT_IDLE, ENT_RETURN_HOME, ENT_PERFORM_RESET, ENT_PERFORM_PULL }; // ACTIONS
  int event( int id );
  void action( int id );
  ServoEasing &resetServo;

};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_reset_machine">
    <states>
      <IDLE index="0" on_enter="ENT_IDLE">
        <EVT_INIT_PULL>PERFORM_PULL</EVT_INIT_PULL>
        <EVT_INIT_RESET>PERFORM_RESET</EVT_INIT_RESET>
      </IDLE>
      <RETURN_HOME index="1" on_enter="ENT_RETURN_HOME">
        <EVT_INIT_PULL>PERFORM_PULL</EVT_INIT_PULL>
        <EVT_INIT_RESET>PERFORM_RESET</EVT_INIT_RESET>
        <EVT_SERVO_DONE>IDLE</EVT_SERVO_DONE>
      </RETURN_HOME>
      <PERFORM_RESET index="2" on_enter="ENT_PERFORM_RESET">
        <EVT_INIT_PULL>PERFORM_PULL</EVT_INIT_PULL>
        <EVT_SERVO_DONE>RETURN_HOME</EVT_SERVO_DONE>
      </PERFORM_RESET>
      <PERFORM_PULL index="3" on_enter="ENT_PERFORM_PULL">
        <EVT_INIT_RESET>PERFORM_RESET</EVT_INIT_RESET>
        <EVT_SERVO_DONE>RETURN_HOME</EVT_SERVO_DONE>
      </PERFORM_PULL>
    </states>
    <events>
      <EVT_INIT_PULL index="0" access="MIXED"/>
      <EVT_INIT_RESET index="1" access="MIXED"/>
      <EVT_SERVO_DONE index="2" access="MIXED"/>
    </events>
    <connectors>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
