#pragma once

#include <Automaton.h>

#include <FastLED.h>
#include <ServoEasing.h>

#include "servos.h"


class Atm_angry_arm_ani_machine: public Machine {


 public:
  enum { IDLE, ANGRY_ARM_UP, ANGRY_ARM_DOWN, ANGRY_PAUSE }; // STATES
  enum { EVT_START, EVT_STOP, EVT_PAUSE_DONE, EVT_ARM_WAVE_DONE, EVT_SERVO_DONE, ELSE }; // EVENTS
  Atm_angry_arm_ani_machine( ServoEasing &leftServo, ServoEasing &rightServo, CRGB (&eyes)[2] )
        : leftServo(leftServo)
        , rightServo(rightServo)
        , eyes(eyes) {}
  Atm_angry_arm_ani_machine& begin( void );
  Atm_angry_arm_ani_machine& trace( Stream & stream );
  Atm_angry_arm_ani_machine& trigger( int event );
  int state( void );
  Atm_angry_arm_ani_machine& start( void );
  Atm_angry_arm_ani_machine& stop( void );
  Atm_angry_arm_ani_machine& pause_done( void );
  Atm_angry_arm_ani_machine& arm_wave_done( void );
  Atm_angry_arm_ani_machine& servo_done( void );

 private:
  enum { ENT_IDLE, ENT_ANGRY_ARM_UP, ENT_ANGRY_ARM_DOWN, ENT_ANGRY_PAUSE, EXT_ANGRY_PAUSE }; // ACTIONS
  int event( int id );
  void action( int id );
    ServoEasing &leftServo;
    ServoEasing &rightServo;
    CRGB (&eyes)[2];

};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_angry_arm_ani_machine">
    <states>
      <IDLE index="0" on_enter="ENT_IDLE">
      </IDLE>
      <ANGRY_ARM_UP index="1" on_enter="ENT_ANGRY_ARM_UP">
        <EVT_ARM_WAVE_DONE>ANGRY_PAUSE</EVT_ARM_WAVE_DONE>
        <EVT_SERVO_DONE>ANGRY_ARM_DOWN</EVT_SERVO_DONE>
        <EVT_STOP>IDLE</EVT_STOP>
      </ANGRY_ARM_UP>
      <ANGRY_ARM_DOWN index="2" on_enter="ENT_ANGRY_ARM_DOWN">
        <EVT_ARM_WAVE_DONE>ANGRY_PAUSE</EVT_ARM_WAVE_DONE>
        <EVT_SERVO_DONE>ANGRY_ARM_UP</EVT_SERVO_DONE>
        <EVT_STOP>IDLE</EVT_STOP>
      </ANGRY_ARM_DOWN>
      <ANGRY_PAUSE index="3" on_enter="ENT_ANGRY_PAUSE" on_exit="EXT_ANGRY_PAUSE">
        <EVT_PAUSE_DONE>ANGRY_ARM_UP</EVT_PAUSE_DONE>
        <EVT_STOP>IDLE</EVT_STOP>
      </ANGRY_PAUSE>
    </states>
    <events>
      <EVT_PAUSE_DONE index="0" access="MIXED"/>
      <EVT_ARM_WAVE_DONE index="1" access="MIXED"/>
      <EVT_SERVO_DONE index="2" access="MIXED"/>
      <EVT_STOP index="3" access="MIXED"/>
    </events>
    <connectors>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
