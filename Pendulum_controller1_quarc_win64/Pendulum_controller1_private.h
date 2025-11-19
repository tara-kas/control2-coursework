/*
 * Pendulum_controller1_private.h
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

#ifndef Pendulum_controller1_private_h_
#define Pendulum_controller1_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "zero_crossing_types.h"
#include "Pendulum_controller1_types.h"

/* A global buffer for storing error messages (defined in quanser_common library) */
EXTERN char _rt_error_message[512];
extern real_T rt_modd_snf(real_T u0, real_T u1);

/* private model entry point functions */
extern void Pendulum_controller1_derivatives(void);

#endif                                 /* Pendulum_controller1_private_h_ */
