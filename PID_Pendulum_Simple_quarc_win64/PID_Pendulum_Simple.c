/*
 * PID_Pendulum_Simple.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "PID_Pendulum_Simple".
 *
 * Model version              : 14.0
 * Simulink Coder version : 25.1 (R2025a) 21-Nov-2024
 * C source code generated on : Sun Nov 30 15:46:22 2025
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "PID_Pendulum_Simple.h"
#include "rtwtypes.h"
#include "PID_Pendulum_Simple_private.h"
#include <math.h>
#include <string.h>
#include "rt_nonfinite.h"
#include <float.h>
#include "PID_Pendulum_Simple_dt.h"

/* Named constants for MATLAB Function: '<Root>/controller' */
#define PID_Pendulum_Simple_CALL_EVENT (-1)

const real_T PID_Pendulum_Simple_RGND = 0.0;/* real_T ground */

/* Block signals (default storage) */
B_PID_Pendulum_Simple_T PID_Pendulum_Simple_B;

/* Continuous states */
X_PID_Pendulum_Simple_T PID_Pendulum_Simple_X;

/* Disabled State Vector */
XDis_PID_Pendulum_Simple_T PID_Pendulum_Simple_XDis;

/* Block states (default storage) */
DW_PID_Pendulum_Simple_T PID_Pendulum_Simple_DW;

/* Real-time model */
static RT_MODEL_PID_Pendulum_Simple_T PID_Pendulum_Simple_M_;
RT_MODEL_PID_Pendulum_Simple_T *const PID_Pendulum_Simple_M =
  &PID_Pendulum_Simple_M_;

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = (ODE4_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 2;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  PID_Pendulum_Simple_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  PID_Pendulum_Simple_output();
  PID_Pendulum_Simple_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  PID_Pendulum_Simple_output();
  PID_Pendulum_Simple_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  PID_Pendulum_Simple_output();
  PID_Pendulum_Simple_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

real_T rt_modd_snf(real_T u0, real_T u1)
{
  real_T y;
  y = u0;
  if (u1 == 0.0) {
    if (u0 == 0.0) {
      y = u1;
    }
  } else if (rtIsNaN(u0) || rtIsNaN(u1) || rtIsInf(u0)) {
    y = (rtNaN);
  } else if (u0 == 0.0) {
    y = 0.0 / u1;
  } else if (rtIsInf(u1)) {
    if ((u1 < 0.0) != (u0 < 0.0)) {
      y = u1;
    }
  } else {
    boolean_T yEq;
    y = fmod(u0, u1);
    yEq = (y == 0.0);
    if ((!yEq) && (u1 > floor(u1))) {
      real_T q;
      q = fabs(u0 / u1);
      yEq = !(fabs(q - floor(q + 0.5)) > DBL_EPSILON * q);
    }

    if (yEq) {
      y = u1 * 0.0;
    } else if ((u0 < 0.0) != (u1 < 0.0)) {
      y += u1;
    }
  }

  return y;
}

/* Model output function */
void PID_Pendulum_Simple_output(void)
{
  /* local block i/o variables */
  real_T rtb_motorcurrent;
  real_T rtb_HILReadTimebase_o5;
  real_T rtb_HILReadTimebase_o6;
  boolean_T rtb_HILReadTimebase_o4;
  real_T rtb_ForveCCW;
  real_T rtb_encoder;
  if (rtmIsMajorTimeStep(PID_Pendulum_Simple_M)) {
    /* set solver stop time */
    if (!(PID_Pendulum_Simple_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&PID_Pendulum_Simple_M->solverInfo,
                            ((PID_Pendulum_Simple_M->Timing.clockTickH0 + 1) *
        PID_Pendulum_Simple_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&PID_Pendulum_Simple_M->solverInfo,
                            ((PID_Pendulum_Simple_M->Timing.clockTick0 + 1) *
        PID_Pendulum_Simple_M->Timing.stepSize0 +
        PID_Pendulum_Simple_M->Timing.clockTickH0 *
        PID_Pendulum_Simple_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(PID_Pendulum_Simple_M)) {
    PID_Pendulum_Simple_M->Timing.t[0] = rtsiGetT
      (&PID_Pendulum_Simple_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(PID_Pendulum_Simple_M)) {
    /* S-Function (hil_read_timebase_block): '<S7>/HIL Read Timebase' */

    /* S-Function Block: PID_Pendulum_Simple/Qube With Pendulum/Qube-Servo 3 - IO (QAL)/HIL Read Timebase (hil_read_timebase_block) */
    {
      t_error result;
      result = hil_task_read(PID_Pendulum_Simple_DW.HILReadTimebase_Task, 1,
        &PID_Pendulum_Simple_DW.HILReadTimebase_AnalogBuffer,
        &PID_Pendulum_Simple_DW.HILReadTimebase_EncoderBuffer[0],
        &PID_Pendulum_Simple_DW.HILReadTimebase_DigitalBuffer,
        &PID_Pendulum_Simple_DW.HILReadTimebase_OtherBuffer[0]
        );
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
      } else {
        rtb_motorcurrent = PID_Pendulum_Simple_DW.HILReadTimebase_AnalogBuffer;
        rtb_encoder = PID_Pendulum_Simple_DW.HILReadTimebase_EncoderBuffer[0];
        rtb_ForveCCW = PID_Pendulum_Simple_DW.HILReadTimebase_EncoderBuffer[1];
        rtb_HILReadTimebase_o4 =
          PID_Pendulum_Simple_DW.HILReadTimebase_DigitalBuffer;
        rtb_HILReadTimebase_o5 =
          PID_Pendulum_Simple_DW.HILReadTimebase_OtherBuffer[0];
        rtb_HILReadTimebase_o6 =
          PID_Pendulum_Simple_DW.HILReadTimebase_OtherBuffer[1];
      }
    }

    /* Bias: '<S6>/Bias' incorporates:
     *  Constant: '<S6>/Constant1'
     *  Gain: '<S6>/Pendulum:  counts to rad'
     *  Math: '<S6>/Math Function'
     */
    PID_Pendulum_Simple_B.Bias = rt_modd_snf
      (PID_Pendulum_Simple_P.Pendulumcountstorad_Gain * rtb_ForveCCW,
       PID_Pendulum_Simple_P.Constant1_Value) + PID_Pendulum_Simple_P.Bias_Bias;

    /* Abs: '<Root>/|alpha|' */
    rtb_ForveCCW = fabs(PID_Pendulum_Simple_B.Bias);

    /* MATLAB Function: '<Root>/controller' */
    PID_Pendulum_Simple_DW.sfEvent = PID_Pendulum_Simple_CALL_EVENT;
    PID_Pendulum_Simple_DW.integral_sum += (0.0 - PID_Pendulum_Simple_B.Bias) *
      0.001;
    PID_Pendulum_Simple_B.u = ((0.0 - PID_Pendulum_Simple_B.Bias) -
      PID_Pendulum_Simple_DW.prev_err) / 0.001 * 0.21 + ((0.0 -
      PID_Pendulum_Simple_B.Bias) * 40.0 + 0.0 *
      PID_Pendulum_Simple_DW.integral_sum);
    PID_Pendulum_Simple_DW.prev_err = 0.0 - PID_Pendulum_Simple_B.Bias;

    /* MultiPortSwitch: '<Root>/Enable Balance Control Switch' incorporates:
     *  Constant: '<S1>/Constant'
     *  RelationalOperator: '<S1>/Compare'
     *  Saturate: '<Root>/Saturation'
     */
    if (!(rtb_ForveCCW <= PID_Pendulum_Simple_P.EnableBalanceControl_const)) {
      /* MultiPortSwitch: '<Root>/Enable Balance Control Switch' incorporates:
       *  Constant: '<Root>/Constant'
       */
      PID_Pendulum_Simple_B.EnableBalanceControlSwitch =
        PID_Pendulum_Simple_P.Constant_Value;
    } else if (PID_Pendulum_Simple_B.u >
               PID_Pendulum_Simple_P.Saturation_UpperSat) {
      /* Saturate: '<Root>/Saturation' incorporates:
       *  MultiPortSwitch: '<Root>/Enable Balance Control Switch'
       */
      PID_Pendulum_Simple_B.EnableBalanceControlSwitch =
        PID_Pendulum_Simple_P.Saturation_UpperSat;
    } else if (PID_Pendulum_Simple_B.u <
               PID_Pendulum_Simple_P.Saturation_LowerSat) {
      /* Saturate: '<Root>/Saturation' incorporates:
       *  MultiPortSwitch: '<Root>/Enable Balance Control Switch'
       */
      PID_Pendulum_Simple_B.EnableBalanceControlSwitch =
        PID_Pendulum_Simple_P.Saturation_LowerSat;
    } else {
      /* MultiPortSwitch: '<Root>/Enable Balance Control Switch' incorporates:
       *  Saturate: '<Root>/Saturation'
       */
      PID_Pendulum_Simple_B.EnableBalanceControlSwitch = PID_Pendulum_Simple_B.u;
    }

    /* End of MultiPortSwitch: '<Root>/Enable Balance Control Switch' */

    /* Gain: '<S2>/For +ve CCW' */
    rtb_ForveCCW = PID_Pendulum_Simple_P.ForveCCW_Gain *
      PID_Pendulum_Simple_B.EnableBalanceControlSwitch;

    /* S-Function (hil_write_block): '<S7>/HIL Write' */

    /* S-Function Block: PID_Pendulum_Simple/Qube With Pendulum/Qube-Servo 3 - IO (QAL)/HIL Write (hil_write_block) */
    {
      t_error result;
      result = hil_write(PID_Pendulum_Simple_DW.HILInitialize_Card,
                         &PID_Pendulum_Simple_P.HILWrite_analog_channels, 1U,
                         NULL, 0U,
                         NULL, 0U,
                         &PID_Pendulum_Simple_P.HILWrite_other_channels, 1U,
                         &rtb_ForveCCW,
                         NULL,
                         NULL,
                         ((const real_T*) &PID_Pendulum_Simple_RGND)
                         );
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
      }
    }

    /* Gain: '<S6>/Arm: counts to rad' */
    PID_Pendulum_Simple_B.Armcountstorad =
      PID_Pendulum_Simple_P.Armcountstorad_Gain * rtb_encoder;

    /* Gain: '<S4>/Gain' */
    PID_Pendulum_Simple_B.Gain = PID_Pendulum_Simple_P.Gain_Gain *
      PID_Pendulum_Simple_B.Armcountstorad;

    /* Gain: '<S3>/Gain' */
    PID_Pendulum_Simple_B.Gain_g = PID_Pendulum_Simple_P.Gain_Gain_j *
      PID_Pendulum_Simple_B.Bias;
  }
}

/* Model update function */
void PID_Pendulum_Simple_update(void)
{
  if (rtmIsMajorTimeStep(PID_Pendulum_Simple_M)) {
    rt_ertODEUpdateContinuousStates(&PID_Pendulum_Simple_M->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++PID_Pendulum_Simple_M->Timing.clockTick0)) {
    ++PID_Pendulum_Simple_M->Timing.clockTickH0;
  }

  PID_Pendulum_Simple_M->Timing.t[0] = rtsiGetSolverStopTime
    (&PID_Pendulum_Simple_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.001s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++PID_Pendulum_Simple_M->Timing.clockTick1)) {
      ++PID_Pendulum_Simple_M->Timing.clockTickH1;
    }

    PID_Pendulum_Simple_M->Timing.t[1] =
      PID_Pendulum_Simple_M->Timing.clockTick1 *
      PID_Pendulum_Simple_M->Timing.stepSize1 +
      PID_Pendulum_Simple_M->Timing.clockTickH1 *
      PID_Pendulum_Simple_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void PID_Pendulum_Simple_derivatives(void)
{
  XDot_PID_Pendulum_Simple_T *_rtXdot;
  _rtXdot = ((XDot_PID_Pendulum_Simple_T *) PID_Pendulum_Simple_M->derivs);

  /* Derivatives for TransferFcn: '<S8>/alpha_dot' */
  _rtXdot->alpha_dot_CSTATE = PID_Pendulum_Simple_P.alpha_dot_A *
    PID_Pendulum_Simple_X.alpha_dot_CSTATE;
  _rtXdot->alpha_dot_CSTATE += PID_Pendulum_Simple_B.Bias;

  /* Derivatives for TransferFcn: '<S8>/theta_dot' */
  _rtXdot->theta_dot_CSTATE = PID_Pendulum_Simple_P.theta_dot_A *
    PID_Pendulum_Simple_X.theta_dot_CSTATE;
  _rtXdot->theta_dot_CSTATE += PID_Pendulum_Simple_B.Armcountstorad;
}

/* Model initialize function */
void PID_Pendulum_Simple_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<S7>/HIL Initialize' */

  /* S-Function Block: PID_Pendulum_Simple/Qube With Pendulum/Qube-Servo 3 - IO (QAL)/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("qube_servo3_usb", "0@tcpip://localhost:18923",
                      &PID_Pendulum_Simple_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options
      (PID_Pendulum_Simple_DW.HILInitialize_Card,
       "deadband_compensation=0.3;pwm_en=0;enc0_velocity=3.0;enc1_velocity=3.0;min_diode_compensation=0.3;max_diode_compensation=1.5",
       125);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(PID_Pendulum_Simple_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
      return;
    }

    if ((PID_Pendulum_Simple_P.HILInitialize_AIPStart && !is_switching) ||
        (PID_Pendulum_Simple_P.HILInitialize_AIPEnter && is_switching)) {
      result = hil_set_analog_input_ranges
        (PID_Pendulum_Simple_DW.HILInitialize_Card,
         &PID_Pendulum_Simple_P.HILInitialize_AIChannels, 1U,
         &PID_Pendulum_Simple_P.HILInitialize_AILow,
         &PID_Pendulum_Simple_P.HILInitialize_AIHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        return;
      }
    }

    if ((PID_Pendulum_Simple_P.HILInitialize_AOPStart && !is_switching) ||
        (PID_Pendulum_Simple_P.HILInitialize_AOPEnter && is_switching)) {
      result = hil_set_analog_output_ranges
        (PID_Pendulum_Simple_DW.HILInitialize_Card,
         &PID_Pendulum_Simple_P.HILInitialize_AOChannels, 1U,
         &PID_Pendulum_Simple_P.HILInitialize_AOLow,
         &PID_Pendulum_Simple_P.HILInitialize_AOHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        return;
      }
    }

    if ((PID_Pendulum_Simple_P.HILInitialize_AOStart && !is_switching) ||
        (PID_Pendulum_Simple_P.HILInitialize_AOEnter && is_switching)) {
      result = hil_write_analog(PID_Pendulum_Simple_DW.HILInitialize_Card,
        &PID_Pendulum_Simple_P.HILInitialize_AOChannels, 1U,
        &PID_Pendulum_Simple_P.HILInitialize_AOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        return;
      }
    }

    if (PID_Pendulum_Simple_P.HILInitialize_AOReset) {
      result = hil_watchdog_set_analog_expiration_state
        (PID_Pendulum_Simple_DW.HILInitialize_Card,
         &PID_Pendulum_Simple_P.HILInitialize_AOChannels, 1U,
         &PID_Pendulum_Simple_P.HILInitialize_AOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        return;
      }
    }

    result = hil_set_digital_directions
      (PID_Pendulum_Simple_DW.HILInitialize_Card, NULL, 0U,
       &PID_Pendulum_Simple_P.HILInitialize_DOChannels, 1U);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
      return;
    }

    if ((PID_Pendulum_Simple_P.HILInitialize_DOStart && !is_switching) ||
        (PID_Pendulum_Simple_P.HILInitialize_DOEnter && is_switching)) {
      result = hil_write_digital(PID_Pendulum_Simple_DW.HILInitialize_Card,
        &PID_Pendulum_Simple_P.HILInitialize_DOChannels, 1U, (t_boolean *)
        &PID_Pendulum_Simple_P.HILInitialize_DOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        return;
      }
    }

    if (PID_Pendulum_Simple_P.HILInitialize_DOReset) {
      result = hil_watchdog_set_digital_expiration_state
        (PID_Pendulum_Simple_DW.HILInitialize_Card,
         &PID_Pendulum_Simple_P.HILInitialize_DOChannels, 1U, (const
          t_digital_state *) &PID_Pendulum_Simple_P.HILInitialize_DOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        return;
      }
    }

    if ((PID_Pendulum_Simple_P.HILInitialize_EIPStart && !is_switching) ||
        (PID_Pendulum_Simple_P.HILInitialize_EIPEnter && is_switching)) {
      PID_Pendulum_Simple_DW.HILInitialize_QuadratureModes[0] =
        PID_Pendulum_Simple_P.HILInitialize_EIQuadrature;
      PID_Pendulum_Simple_DW.HILInitialize_QuadratureModes[1] =
        PID_Pendulum_Simple_P.HILInitialize_EIQuadrature;
      result = hil_set_encoder_quadrature_mode
        (PID_Pendulum_Simple_DW.HILInitialize_Card,
         PID_Pendulum_Simple_P.HILInitialize_EIChannels, 2U,
         (t_encoder_quadrature_mode *)
         &PID_Pendulum_Simple_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        return;
      }
    }

    if ((PID_Pendulum_Simple_P.HILInitialize_EIStart && !is_switching) ||
        (PID_Pendulum_Simple_P.HILInitialize_EIEnter && is_switching)) {
      PID_Pendulum_Simple_DW.HILInitialize_InitialEICounts[0] =
        PID_Pendulum_Simple_P.HILInitialize_EIInitial;
      PID_Pendulum_Simple_DW.HILInitialize_InitialEICounts[1] =
        PID_Pendulum_Simple_P.HILInitialize_EIInitial;
      result = hil_set_encoder_counts(PID_Pendulum_Simple_DW.HILInitialize_Card,
        PID_Pendulum_Simple_P.HILInitialize_EIChannels, 2U,
        &PID_Pendulum_Simple_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        return;
      }
    }

    if ((PID_Pendulum_Simple_P.HILInitialize_OOStart && !is_switching) ||
        (PID_Pendulum_Simple_P.HILInitialize_OOEnter && is_switching)) {
      result = hil_write_other(PID_Pendulum_Simple_DW.HILInitialize_Card,
        PID_Pendulum_Simple_P.HILInitialize_OOChannels, 3U,
        PID_Pendulum_Simple_P.HILInitialize_OOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        return;
      }
    }

    if (PID_Pendulum_Simple_P.HILInitialize_OOReset) {
      result = hil_watchdog_set_other_expiration_state
        (PID_Pendulum_Simple_DW.HILInitialize_Card,
         PID_Pendulum_Simple_P.HILInitialize_OOChannels, 3U,
         PID_Pendulum_Simple_P.HILInitialize_OOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_timebase_block): '<S7>/HIL Read Timebase' */

  /* S-Function Block: PID_Pendulum_Simple/Qube With Pendulum/Qube-Servo 3 - IO (QAL)/HIL Read Timebase (hil_read_timebase_block) */
  {
    t_error result;
    result = hil_task_create_reader(PID_Pendulum_Simple_DW.HILInitialize_Card,
      PID_Pendulum_Simple_P.HILReadTimebase_SamplesInBuffer,
      &PID_Pendulum_Simple_P.HILReadTimebase_AnalogChannels, 1U,
      PID_Pendulum_Simple_P.HILReadTimebase_EncoderChannels, 2U,
      &PID_Pendulum_Simple_P.HILReadTimebase_DigitalChannels, 1U,
      PID_Pendulum_Simple_P.HILReadTimebase_OtherChannels, 2U,
      &PID_Pendulum_Simple_DW.HILReadTimebase_Task);
    if (result >= 0) {
      result = hil_task_set_buffer_overflow_mode
        (PID_Pendulum_Simple_DW.HILReadTimebase_Task, (t_buffer_overflow_mode)
         (PID_Pendulum_Simple_P.HILReadTimebase_OverflowMode - 1));
    }

    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
    }
  }

  /* InitializeConditions for TransferFcn: '<S8>/alpha_dot' */
  PID_Pendulum_Simple_X.alpha_dot_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S8>/theta_dot' */
  PID_Pendulum_Simple_X.theta_dot_CSTATE = 0.0;

  /* SystemInitialize for MATLAB Function: '<Root>/controller' */
  PID_Pendulum_Simple_DW.integral_sum = 0.0;
  PID_Pendulum_Simple_DW.integral_sum_not_empty = true;
  PID_Pendulum_Simple_DW.prev_err = 0.0;
  PID_Pendulum_Simple_DW.prev_err_not_empty = true;
  PID_Pendulum_Simple_DW.sfEvent = PID_Pendulum_Simple_CALL_EVENT;
}

/* Model terminate function */
void PID_Pendulum_Simple_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<S7>/HIL Initialize' */

  /* S-Function Block: PID_Pendulum_Simple/Qube With Pendulum/Qube-Servo 3 - IO (QAL)/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_digital_outputs = 0;
    t_uint32 num_final_other_outputs = 0;
    hil_task_stop_all(PID_Pendulum_Simple_DW.HILInitialize_Card);
    hil_monitor_stop_all(PID_Pendulum_Simple_DW.HILInitialize_Card);
    is_switching = false;
    if ((PID_Pendulum_Simple_P.HILInitialize_AOTerminate && !is_switching) ||
        (PID_Pendulum_Simple_P.HILInitialize_AOExit && is_switching)) {
      num_final_analog_outputs = 1U;
    } else {
      num_final_analog_outputs = 0;
    }

    if ((PID_Pendulum_Simple_P.HILInitialize_DOTerminate && !is_switching) ||
        (PID_Pendulum_Simple_P.HILInitialize_DOExit && is_switching)) {
      num_final_digital_outputs = 1U;
    } else {
      num_final_digital_outputs = 0;
    }

    if ((PID_Pendulum_Simple_P.HILInitialize_OOTerminate && !is_switching) ||
        (PID_Pendulum_Simple_P.HILInitialize_OOExit && is_switching)) {
      num_final_other_outputs = 3U;
    } else {
      num_final_other_outputs = 0;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_digital_outputs > 0
        || num_final_other_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(PID_Pendulum_Simple_DW.HILInitialize_Card
                         , &PID_Pendulum_Simple_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , NULL, 0
                         , &PID_Pendulum_Simple_P.HILInitialize_DOChannels,
                         num_final_digital_outputs
                         , PID_Pendulum_Simple_P.HILInitialize_OOChannels,
                         num_final_other_outputs
                         , &PID_Pendulum_Simple_P.HILInitialize_AOFinal
                         , NULL
                         , (t_boolean *)
                         &PID_Pendulum_Simple_P.HILInitialize_DOFinal
                         , PID_Pendulum_Simple_P.HILInitialize_OOFinal
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog
            (PID_Pendulum_Simple_DW.HILInitialize_Card,
             &PID_Pendulum_Simple_P.HILInitialize_AOChannels,
             num_final_analog_outputs,
             &PID_Pendulum_Simple_P.HILInitialize_AOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_digital_outputs > 0) {
          local_result = hil_write_digital
            (PID_Pendulum_Simple_DW.HILInitialize_Card,
             &PID_Pendulum_Simple_P.HILInitialize_DOChannels,
             num_final_digital_outputs, (t_boolean *)
             &PID_Pendulum_Simple_P.HILInitialize_DOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_other_outputs > 0) {
          local_result = hil_write_other
            (PID_Pendulum_Simple_DW.HILInitialize_Card,
             PID_Pendulum_Simple_P.HILInitialize_OOChannels,
             num_final_other_outputs,
             PID_Pendulum_Simple_P.HILInitialize_OOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(PID_Pendulum_Simple_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(PID_Pendulum_Simple_DW.HILInitialize_Card);
    hil_monitor_delete_all(PID_Pendulum_Simple_DW.HILInitialize_Card);
    hil_close(PID_Pendulum_Simple_DW.HILInitialize_Card);
    PID_Pendulum_Simple_DW.HILInitialize_Card = NULL;
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  PID_Pendulum_Simple_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  PID_Pendulum_Simple_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  PID_Pendulum_Simple_initialize();
}

void MdlTerminate(void)
{
  PID_Pendulum_Simple_terminate();
}

/* Registration function */
RT_MODEL_PID_Pendulum_Simple_T *PID_Pendulum_Simple(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)PID_Pendulum_Simple_M, 0,
                sizeof(RT_MODEL_PID_Pendulum_Simple_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&PID_Pendulum_Simple_M->solverInfo,
                          &PID_Pendulum_Simple_M->Timing.simTimeStep);
    rtsiSetTPtr(&PID_Pendulum_Simple_M->solverInfo, &rtmGetTPtr
                (PID_Pendulum_Simple_M));
    rtsiSetStepSizePtr(&PID_Pendulum_Simple_M->solverInfo,
                       &PID_Pendulum_Simple_M->Timing.stepSize0);
    rtsiSetdXPtr(&PID_Pendulum_Simple_M->solverInfo,
                 &PID_Pendulum_Simple_M->derivs);
    rtsiSetContStatesPtr(&PID_Pendulum_Simple_M->solverInfo, (real_T **)
                         &PID_Pendulum_Simple_M->contStates);
    rtsiSetNumContStatesPtr(&PID_Pendulum_Simple_M->solverInfo,
      &PID_Pendulum_Simple_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&PID_Pendulum_Simple_M->solverInfo,
      &PID_Pendulum_Simple_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&PID_Pendulum_Simple_M->solverInfo,
      &PID_Pendulum_Simple_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&PID_Pendulum_Simple_M->solverInfo,
      &PID_Pendulum_Simple_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&PID_Pendulum_Simple_M->solverInfo, (boolean_T**)
      &PID_Pendulum_Simple_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&PID_Pendulum_Simple_M->solverInfo,
                          (&rtmGetErrorStatus(PID_Pendulum_Simple_M)));
    rtsiSetRTModelPtr(&PID_Pendulum_Simple_M->solverInfo, PID_Pendulum_Simple_M);
  }

  rtsiSetSimTimeStep(&PID_Pendulum_Simple_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&PID_Pendulum_Simple_M->solverInfo, false);
  rtsiSetIsContModeFrozen(&PID_Pendulum_Simple_M->solverInfo, false);
  PID_Pendulum_Simple_M->intgData.y = PID_Pendulum_Simple_M->odeY;
  PID_Pendulum_Simple_M->intgData.f[0] = PID_Pendulum_Simple_M->odeF[0];
  PID_Pendulum_Simple_M->intgData.f[1] = PID_Pendulum_Simple_M->odeF[1];
  PID_Pendulum_Simple_M->intgData.f[2] = PID_Pendulum_Simple_M->odeF[2];
  PID_Pendulum_Simple_M->intgData.f[3] = PID_Pendulum_Simple_M->odeF[3];
  PID_Pendulum_Simple_M->contStates = ((real_T *) &PID_Pendulum_Simple_X);
  PID_Pendulum_Simple_M->contStateDisabled = ((boolean_T *)
    &PID_Pendulum_Simple_XDis);
  PID_Pendulum_Simple_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&PID_Pendulum_Simple_M->solverInfo, (void *)
                    &PID_Pendulum_Simple_M->intgData);
  rtsiSetSolverName(&PID_Pendulum_Simple_M->solverInfo,"ode4");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = PID_Pendulum_Simple_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    PID_Pendulum_Simple_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    PID_Pendulum_Simple_M->Timing.sampleTimes =
      (&PID_Pendulum_Simple_M->Timing.sampleTimesArray[0]);
    PID_Pendulum_Simple_M->Timing.offsetTimes =
      (&PID_Pendulum_Simple_M->Timing.offsetTimesArray[0]);

    /* task periods */
    PID_Pendulum_Simple_M->Timing.sampleTimes[0] = (0.0);
    PID_Pendulum_Simple_M->Timing.sampleTimes[1] = (0.001);

    /* task offsets */
    PID_Pendulum_Simple_M->Timing.offsetTimes[0] = (0.0);
    PID_Pendulum_Simple_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(PID_Pendulum_Simple_M, &PID_Pendulum_Simple_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = PID_Pendulum_Simple_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    PID_Pendulum_Simple_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(PID_Pendulum_Simple_M, -1);
  PID_Pendulum_Simple_M->Timing.stepSize0 = 0.001;
  PID_Pendulum_Simple_M->Timing.stepSize1 = 0.001;

  /* External mode info */
  PID_Pendulum_Simple_M->Sizes.checksums[0] = (1008379644U);
  PID_Pendulum_Simple_M->Sizes.checksums[1] = (2542497477U);
  PID_Pendulum_Simple_M->Sizes.checksums[2] = (608010934U);
  PID_Pendulum_Simple_M->Sizes.checksums[3] = (784681161U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[4];
    PID_Pendulum_Simple_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    systemRan[1] = &rtAlwaysEnabled;
    systemRan[2] = &rtAlwaysEnabled;
    systemRan[3] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(PID_Pendulum_Simple_M->extModeInfo,
      &PID_Pendulum_Simple_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(PID_Pendulum_Simple_M->extModeInfo,
                        PID_Pendulum_Simple_M->Sizes.checksums);
    rteiSetTPtr(PID_Pendulum_Simple_M->extModeInfo, rtmGetTPtr
                (PID_Pendulum_Simple_M));
  }

  PID_Pendulum_Simple_M->solverInfoPtr = (&PID_Pendulum_Simple_M->solverInfo);
  PID_Pendulum_Simple_M->Timing.stepSize = (0.001);
  rtsiSetFixedStepSize(&PID_Pendulum_Simple_M->solverInfo, 0.001);
  rtsiSetSolverMode(&PID_Pendulum_Simple_M->solverInfo,
                    SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  PID_Pendulum_Simple_M->blockIO = ((void *) &PID_Pendulum_Simple_B);
  (void) memset(((void *) &PID_Pendulum_Simple_B), 0,
                sizeof(B_PID_Pendulum_Simple_T));

  /* parameters */
  PID_Pendulum_Simple_M->defaultParam = ((real_T *)&PID_Pendulum_Simple_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &PID_Pendulum_Simple_X;
    PID_Pendulum_Simple_M->contStates = (x);
    (void) memset((void *)&PID_Pendulum_Simple_X, 0,
                  sizeof(X_PID_Pendulum_Simple_T));
  }

  /* disabled states */
  {
    boolean_T *xdis = (boolean_T *) &PID_Pendulum_Simple_XDis;
    PID_Pendulum_Simple_M->contStateDisabled = (xdis);
    (void) memset((void *)&PID_Pendulum_Simple_XDis, 0,
                  sizeof(XDis_PID_Pendulum_Simple_T));
  }

  /* states (dwork) */
  PID_Pendulum_Simple_M->dwork = ((void *) &PID_Pendulum_Simple_DW);
  (void) memset((void *)&PID_Pendulum_Simple_DW, 0,
                sizeof(DW_PID_Pendulum_Simple_T));

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    PID_Pendulum_Simple_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 22;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  PID_Pendulum_Simple_M->Sizes.numContStates = (2);/* Number of continuous states */
  PID_Pendulum_Simple_M->Sizes.numPeriodicContStates = (0);
                                      /* Number of periodic continuous states */
  PID_Pendulum_Simple_M->Sizes.numY = (0);/* Number of model outputs */
  PID_Pendulum_Simple_M->Sizes.numU = (0);/* Number of model inputs */
  PID_Pendulum_Simple_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  PID_Pendulum_Simple_M->Sizes.numSampTimes = (2);/* Number of sample times */
  PID_Pendulum_Simple_M->Sizes.numBlocks = (27);/* Number of blocks */
  PID_Pendulum_Simple_M->Sizes.numBlockIO = (6);/* Number of block outputs */
  PID_Pendulum_Simple_M->Sizes.numBlockPrms = (106);/* Sum of parameter "widths" */
  return PID_Pendulum_Simple_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
