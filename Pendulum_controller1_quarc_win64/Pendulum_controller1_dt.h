/*
 * Pendulum_controller1_dt.h
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Pendulum_controller1".
 *
 * Model version              : 12.43
 * Simulink Coder version : 25.1 (R2025a) 21-Nov-2024
 * C source code generated on : Sun Nov  9 21:58:24 2025
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ext_types.h"

/* data type size table */
static uint_T rtDataTypeSizes[] = {
  sizeof(real_T),
  sizeof(real32_T),
  sizeof(int8_T),
  sizeof(uint8_T),
  sizeof(int16_T),
  sizeof(uint16_T),
  sizeof(int32_T),
  sizeof(uint32_T),
  sizeof(boolean_T),
  sizeof(fcn_call_T),
  sizeof(int_T),
  sizeof(pointer_T),
  sizeof(action_T),
  2*sizeof(uint32_T),
  sizeof(int32_T),
  sizeof(t_card),
  sizeof(t_task),
  sizeof(t_boolean),
  sizeof(uint_T),
  sizeof(char_T),
  sizeof(uchar_T),
  sizeof(time_T)
};

/* data type name table */
static const char_T * rtDataTypeNames[] = {
  "real_T",
  "real32_T",
  "int8_T",
  "uint8_T",
  "int16_T",
  "uint16_T",
  "int32_T",
  "uint32_T",
  "boolean_T",
  "fcn_call_T",
  "int_T",
  "pointer_T",
  "action_T",
  "timer_uint32_pair_T",
  "physical_connection",
  "t_card",
  "t_task",
  "t_boolean",
  "uint_T",
  "char_T",
  "uchar_T",
  "time_T"
};

/* data type transitions for block I/O structure */
static DataTypeTransition rtBTransitions[] = {
  { (char_T *)(&Pendulum_controller1_B.Bias), 0, 0, 8 }
  ,

  { (char_T *)(&Pendulum_controller1_DW.HILInitialize_FilterFrequency[0]), 0, 0,
    7 },

  { (char_T *)(&Pendulum_controller1_DW.HILInitialize_Card), 15, 0, 1 },

  { (char_T *)(&Pendulum_controller1_DW.HILReadTimebase_Task), 16, 0, 1 },

  { (char_T *)(&Pendulum_controller1_DW.HILWrite_PWORK), 11, 0, 6 },

  { (char_T *)(&Pendulum_controller1_DW.HILInitialize_ClockModes), 6, 0, 9 },

  { (char_T *)(&Pendulum_controller1_DW.doneDoubleBufferReInit), 8, 0, 3 },

  { (char_T *)(&Pendulum_controller1_DW.HILReadTimebase_DigitalBuffer), 17, 0, 1
  }
};

/* data type transition table for block I/O structure */
static DataTypeTransitionTable rtBTransTable = {
  8U,
  rtBTransitions
};

/* data type transitions for Parameters structure */
static DataTypeTransition rtPTransitions[] = {
  { (char_T *)(&Pendulum_controller1_P.EnableBalanceControl_const), 0, 0, 1 },

  { (char_T *)(&Pendulum_controller1_P.HILWrite_analog_channels), 7, 0, 2 },

  { (char_T *)(&Pendulum_controller1_P.Constant_Value), 0, 0, 41 },

  { (char_T *)(&Pendulum_controller1_P.HILInitialize_CKChannels), 6, 0, 8 },

  { (char_T *)(&Pendulum_controller1_P.HILInitialize_AIChannels), 7, 0, 16 },

  { (char_T *)(&Pendulum_controller1_P.HILInitialize_Active), 8, 0, 37 },

  { (char_T *)(&Pendulum_controller1_P.HILReadTimebase_OverflowMode), 3, 0, 1 }
};

/* data type transition table for Parameters structure */
static DataTypeTransitionTable rtPTransTable = {
  7U,
  rtPTransitions
};

/* [EOF] Pendulum_controller1_dt.h */
