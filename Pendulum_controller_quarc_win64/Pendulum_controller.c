/*
 * Pendulum_controller.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Pendulum_controller".
 *
 * Model version              : 13.10
 * Simulink Coder version : 25.1 (R2025a) 21-Nov-2024
 * C source code generated on : Thu Nov  6 18:18:35 2025
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Pendulum_controller.h"
#include "rtwtypes.h"
#include "Pendulum_controller_private.h"
#include <math.h>
#include <string.h>
#include "rt_nonfinite.h"
#include <float.h>
#include "Pendulum_controller_dt.h"

/* Named constants for MATLAB Function: '<Root>/MATLAB Function' */
#define Pendulum_controller_CALL_EVENT (-1)

const real_T Pendulum_controller_RGND = 0.0;/* real_T ground */

/* Block signals (default storage) */
B_Pendulum_controller_T Pendulum_controller_B;

/* Continuous states */
X_Pendulum_controller_T Pendulum_controller_X;

/* Disabled State Vector */
XDis_Pendulum_controller_T Pendulum_controller_XDis;

/* Block states (default storage) */
DW_Pendulum_controller_T Pendulum_controller_DW;

/* Real-time model */
static RT_MODEL_Pendulum_controller_T Pendulum_controller_M_;
RT_MODEL_Pendulum_controller_T *const Pendulum_controller_M =
  &Pendulum_controller_M_;

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
  Pendulum_controller_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  Pendulum_controller_output();
  Pendulum_controller_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  Pendulum_controller_output();
  Pendulum_controller_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  Pendulum_controller_output();
  Pendulum_controller_derivatives();

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
void Pendulum_controller_output(void)
{
  /* local block i/o variables */
  real_T rtb_motorcurrent;
  real_T rtb_HILReadTimebase_o5;
  real_T rtb_HILReadTimebase_o6;
  boolean_T rtb_HILReadTimebase_o4;
  real_T rtb_ForveCCW;
  real_T rtb_encoder;
  if (rtmIsMajorTimeStep(Pendulum_controller_M)) {
    /* set solver stop time */
    if (!(Pendulum_controller_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&Pendulum_controller_M->solverInfo,
                            ((Pendulum_controller_M->Timing.clockTickH0 + 1) *
        Pendulum_controller_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&Pendulum_controller_M->solverInfo,
                            ((Pendulum_controller_M->Timing.clockTick0 + 1) *
        Pendulum_controller_M->Timing.stepSize0 +
        Pendulum_controller_M->Timing.clockTickH0 *
        Pendulum_controller_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(Pendulum_controller_M)) {
    Pendulum_controller_M->Timing.t[0] = rtsiGetT
      (&Pendulum_controller_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(Pendulum_controller_M)) {
    /* S-Function (hil_read_timebase_block): '<S7>/HIL Read Timebase' */

    /* S-Function Block: Pendulum_controller/Qube With Pendulum/Qube-Servo 3 - IO (QAL)/HIL Read Timebase (hil_read_timebase_block) */
    {
      t_error result;
      result = hil_task_read(Pendulum_controller_DW.HILReadTimebase_Task, 1,
        &Pendulum_controller_DW.HILReadTimebase_AnalogBuffer,
        &Pendulum_controller_DW.HILReadTimebase_EncoderBuffer[0],
        &Pendulum_controller_DW.HILReadTimebase_DigitalBuffer,
        &Pendulum_controller_DW.HILReadTimebase_OtherBuffer[0]
        );
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
      } else {
        rtb_motorcurrent = Pendulum_controller_DW.HILReadTimebase_AnalogBuffer;
        rtb_encoder = Pendulum_controller_DW.HILReadTimebase_EncoderBuffer[0];
        rtb_ForveCCW = Pendulum_controller_DW.HILReadTimebase_EncoderBuffer[1];
        rtb_HILReadTimebase_o4 =
          Pendulum_controller_DW.HILReadTimebase_DigitalBuffer;
        rtb_HILReadTimebase_o5 =
          Pendulum_controller_DW.HILReadTimebase_OtherBuffer[0];
        rtb_HILReadTimebase_o6 =
          Pendulum_controller_DW.HILReadTimebase_OtherBuffer[1];
      }
    }

    /* Bias: '<S6>/Bias' incorporates:
     *  Constant: '<S6>/Constant1'
     *  Gain: '<S6>/Pendulum:  counts to rad'
     *  Math: '<S6>/Math Function'
     */
    Pendulum_controller_B.Bias = rt_modd_snf
      (Pendulum_controller_P.Pendulumcountstorad_Gain * rtb_ForveCCW,
       Pendulum_controller_P.Constant1_Value) + Pendulum_controller_P.Bias_Bias;

    /* Abs: '<Root>/|alpha|' */
    rtb_ForveCCW = fabs(Pendulum_controller_B.Bias);

    /* Gain: '<S6>/Arm: counts to rad' */
    Pendulum_controller_B.Armcountstorad =
      Pendulum_controller_P.Armcountstorad_Gain * rtb_encoder;

    /* MATLAB Function: '<Root>/MATLAB Function' */
    Pendulum_controller_DW.sfEvent = Pendulum_controller_CALL_EVENT;
    Pendulum_controller_DW.Ei += (0.0 - Pendulum_controller_B.Bias) * 0.0 *
      0.002;
    rtb_encoder = ((0.0 - Pendulum_controller_B.Bias) -
                   Pendulum_controller_DW.prev_error) * 0.0 / 0.002;
    Pendulum_controller_DW.prev_error = 0.0 - Pendulum_controller_B.Bias;
    Pendulum_controller_B.u = ((0.0 - Pendulum_controller_B.Bias) +
      Pendulum_controller_DW.Ei) + rtb_encoder;

    /* MultiPortSwitch: '<Root>/Enable Balance Control Switch' incorporates:
     *  Constant: '<S1>/Constant'
     *  RelationalOperator: '<S1>/Compare'
     *  Saturate: '<Root>/Saturation'
     */
    if (!(rtb_ForveCCW <= Pendulum_controller_P.EnableBalanceControl_const)) {
      /* MultiPortSwitch: '<Root>/Enable Balance Control Switch' incorporates:
       *  Constant: '<Root>/Constant'
       */
      Pendulum_controller_B.EnableBalanceControlSwitch =
        Pendulum_controller_P.Constant_Value;
    } else if (Pendulum_controller_B.u >
               Pendulum_controller_P.Saturation_UpperSat) {
      /* Saturate: '<Root>/Saturation' incorporates:
       *  MultiPortSwitch: '<Root>/Enable Balance Control Switch'
       */
      Pendulum_controller_B.EnableBalanceControlSwitch =
        Pendulum_controller_P.Saturation_UpperSat;
    } else if (Pendulum_controller_B.u <
               Pendulum_controller_P.Saturation_LowerSat) {
      /* Saturate: '<Root>/Saturation' incorporates:
       *  MultiPortSwitch: '<Root>/Enable Balance Control Switch'
       */
      Pendulum_controller_B.EnableBalanceControlSwitch =
        Pendulum_controller_P.Saturation_LowerSat;
    } else {
      /* MultiPortSwitch: '<Root>/Enable Balance Control Switch' incorporates:
       *  Saturate: '<Root>/Saturation'
       */
      Pendulum_controller_B.EnableBalanceControlSwitch = Pendulum_controller_B.u;
    }

    /* End of MultiPortSwitch: '<Root>/Enable Balance Control Switch' */

    /* Gain: '<S3>/For +ve CCW' */
    rtb_ForveCCW = Pendulum_controller_P.ForveCCW_Gain *
      Pendulum_controller_B.EnableBalanceControlSwitch;

    /* S-Function (hil_write_block): '<S7>/HIL Write' */

    /* S-Function Block: Pendulum_controller/Qube With Pendulum/Qube-Servo 3 - IO (QAL)/HIL Write (hil_write_block) */
    {
      t_error result;
      result = hil_write(Pendulum_controller_DW.HILInitialize_Card,
                         &Pendulum_controller_P.HILWrite_analog_channels, 1U,
                         NULL, 0U,
                         NULL, 0U,
                         &Pendulum_controller_P.HILWrite_other_channels, 1U,
                         &rtb_ForveCCW,
                         NULL,
                         NULL,
                         ((const real_T*) &Pendulum_controller_RGND)
                         );
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
      }
    }

    /* Gain: '<S5>/Gain' */
    Pendulum_controller_B.Gain = Pendulum_controller_P.Gain_Gain *
      Pendulum_controller_B.Armcountstorad;

    /* Gain: '<S4>/Gain' */
    Pendulum_controller_B.Gain_g = Pendulum_controller_P.Gain_Gain_j *
      Pendulum_controller_B.Bias;
  }
}

/* Model update function */
void Pendulum_controller_update(void)
{
  if (rtmIsMajorTimeStep(Pendulum_controller_M)) {
    rt_ertODEUpdateContinuousStates(&Pendulum_controller_M->solverInfo);
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
  if (!(++Pendulum_controller_M->Timing.clockTick0)) {
    ++Pendulum_controller_M->Timing.clockTickH0;
  }

  Pendulum_controller_M->Timing.t[0] = rtsiGetSolverStopTime
    (&Pendulum_controller_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.002s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++Pendulum_controller_M->Timing.clockTick1)) {
      ++Pendulum_controller_M->Timing.clockTickH1;
    }

    Pendulum_controller_M->Timing.t[1] =
      Pendulum_controller_M->Timing.clockTick1 *
      Pendulum_controller_M->Timing.stepSize1 +
      Pendulum_controller_M->Timing.clockTickH1 *
      Pendulum_controller_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void Pendulum_controller_derivatives(void)
{
  XDot_Pendulum_controller_T *_rtXdot;
  _rtXdot = ((XDot_Pendulum_controller_T *) Pendulum_controller_M->derivs);

  /* Derivatives for TransferFcn: '<S8>/alpha_dot' */
  _rtXdot->alpha_dot_CSTATE = Pendulum_controller_P.alpha_dot_A *
    Pendulum_controller_X.alpha_dot_CSTATE;
  _rtXdot->alpha_dot_CSTATE += Pendulum_controller_B.Bias;

  /* Derivatives for TransferFcn: '<S8>/theta_dot' */
  _rtXdot->theta_dot_CSTATE = Pendulum_controller_P.theta_dot_A *
    Pendulum_controller_X.theta_dot_CSTATE;
  _rtXdot->theta_dot_CSTATE += Pendulum_controller_B.Armcountstorad;
}

/* Model initialize function */
void Pendulum_controller_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<S7>/HIL Initialize' */

  /* S-Function Block: Pendulum_controller/Qube With Pendulum/Qube-Servo 3 - IO (QAL)/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("qube_servo3_usb", "0@tcpip://localhost:18923",
                      &Pendulum_controller_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options
      (Pendulum_controller_DW.HILInitialize_Card,
       "deadband_compensation=0.3;pwm_en=0;enc0_velocity=3.0;enc1_velocity=3.0;min_diode_compensation=0.3;max_diode_compensation=1.5",
       125);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(Pendulum_controller_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
      return;
    }

    if ((Pendulum_controller_P.HILInitialize_AIPStart && !is_switching) ||
        (Pendulum_controller_P.HILInitialize_AIPEnter && is_switching)) {
      result = hil_set_analog_input_ranges
        (Pendulum_controller_DW.HILInitialize_Card,
         &Pendulum_controller_P.HILInitialize_AIChannels, 1U,
         &Pendulum_controller_P.HILInitialize_AILow,
         &Pendulum_controller_P.HILInitialize_AIHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        return;
      }
    }

    if ((Pendulum_controller_P.HILInitialize_AOPStart && !is_switching) ||
        (Pendulum_controller_P.HILInitialize_AOPEnter && is_switching)) {
      result = hil_set_analog_output_ranges
        (Pendulum_controller_DW.HILInitialize_Card,
         &Pendulum_controller_P.HILInitialize_AOChannels, 1U,
         &Pendulum_controller_P.HILInitialize_AOLow,
         &Pendulum_controller_P.HILInitialize_AOHigh);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        return;
      }
    }

    if ((Pendulum_controller_P.HILInitialize_AOStart && !is_switching) ||
        (Pendulum_controller_P.HILInitialize_AOEnter && is_switching)) {
      result = hil_write_analog(Pendulum_controller_DW.HILInitialize_Card,
        &Pendulum_controller_P.HILInitialize_AOChannels, 1U,
        &Pendulum_controller_P.HILInitialize_AOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        return;
      }
    }

    if (Pendulum_controller_P.HILInitialize_AOReset) {
      result = hil_watchdog_set_analog_expiration_state
        (Pendulum_controller_DW.HILInitialize_Card,
         &Pendulum_controller_P.HILInitialize_AOChannels, 1U,
         &Pendulum_controller_P.HILInitialize_AOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        return;
      }
    }

    result = hil_set_digital_directions
      (Pendulum_controller_DW.HILInitialize_Card, NULL, 0U,
       &Pendulum_controller_P.HILInitialize_DOChannels, 1U);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
      return;
    }

    if ((Pendulum_controller_P.HILInitialize_DOStart && !is_switching) ||
        (Pendulum_controller_P.HILInitialize_DOEnter && is_switching)) {
      result = hil_write_digital(Pendulum_controller_DW.HILInitialize_Card,
        &Pendulum_controller_P.HILInitialize_DOChannels, 1U, (t_boolean *)
        &Pendulum_controller_P.HILInitialize_DOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        return;
      }
    }

    if (Pendulum_controller_P.HILInitialize_DOReset) {
      result = hil_watchdog_set_digital_expiration_state
        (Pendulum_controller_DW.HILInitialize_Card,
         &Pendulum_controller_P.HILInitialize_DOChannels, 1U, (const
          t_digital_state *) &Pendulum_controller_P.HILInitialize_DOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        return;
      }
    }

    if ((Pendulum_controller_P.HILInitialize_EIPStart && !is_switching) ||
        (Pendulum_controller_P.HILInitialize_EIPEnter && is_switching)) {
      Pendulum_controller_DW.HILInitialize_QuadratureModes[0] =
        Pendulum_controller_P.HILInitialize_EIQuadrature;
      Pendulum_controller_DW.HILInitialize_QuadratureModes[1] =
        Pendulum_controller_P.HILInitialize_EIQuadrature;
      result = hil_set_encoder_quadrature_mode
        (Pendulum_controller_DW.HILInitialize_Card,
         Pendulum_controller_P.HILInitialize_EIChannels, 2U,
         (t_encoder_quadrature_mode *)
         &Pendulum_controller_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        return;
      }
    }

    if ((Pendulum_controller_P.HILInitialize_EIStart && !is_switching) ||
        (Pendulum_controller_P.HILInitialize_EIEnter && is_switching)) {
      Pendulum_controller_DW.HILInitialize_InitialEICounts[0] =
        Pendulum_controller_P.HILInitialize_EIInitial;
      Pendulum_controller_DW.HILInitialize_InitialEICounts[1] =
        Pendulum_controller_P.HILInitialize_EIInitial;
      result = hil_set_encoder_counts(Pendulum_controller_DW.HILInitialize_Card,
        Pendulum_controller_P.HILInitialize_EIChannels, 2U,
        &Pendulum_controller_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        return;
      }
    }

    if ((Pendulum_controller_P.HILInitialize_OOStart && !is_switching) ||
        (Pendulum_controller_P.HILInitialize_OOEnter && is_switching)) {
      result = hil_write_other(Pendulum_controller_DW.HILInitialize_Card,
        Pendulum_controller_P.HILInitialize_OOChannels, 3U,
        Pendulum_controller_P.HILInitialize_OOInitial);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        return;
      }
    }

    if (Pendulum_controller_P.HILInitialize_OOReset) {
      result = hil_watchdog_set_other_expiration_state
        (Pendulum_controller_DW.HILInitialize_Card,
         Pendulum_controller_P.HILInitialize_OOChannels, 3U,
         Pendulum_controller_P.HILInitialize_OOWatchdog);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_timebase_block): '<S7>/HIL Read Timebase' */

  /* S-Function Block: Pendulum_controller/Qube With Pendulum/Qube-Servo 3 - IO (QAL)/HIL Read Timebase (hil_read_timebase_block) */
  {
    t_error result;
    result = hil_task_create_reader(Pendulum_controller_DW.HILInitialize_Card,
      Pendulum_controller_P.HILReadTimebase_SamplesInBuffer,
      &Pendulum_controller_P.HILReadTimebase_AnalogChannels, 1U,
      Pendulum_controller_P.HILReadTimebase_EncoderChannels, 2U,
      &Pendulum_controller_P.HILReadTimebase_DigitalChannels, 1U,
      Pendulum_controller_P.HILReadTimebase_OtherChannels, 2U,
      &Pendulum_controller_DW.HILReadTimebase_Task);
    if (result >= 0) {
      result = hil_task_set_buffer_overflow_mode
        (Pendulum_controller_DW.HILReadTimebase_Task, (t_buffer_overflow_mode)
         (Pendulum_controller_P.HILReadTimebase_OverflowMode - 1));
    }

    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
    }
  }

  /* InitializeConditions for TransferFcn: '<S8>/alpha_dot' */
  Pendulum_controller_X.alpha_dot_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S8>/theta_dot' */
  Pendulum_controller_X.theta_dot_CSTATE = 0.0;

  /* SystemInitialize for MATLAB Function: '<Root>/MATLAB Function' */
  Pendulum_controller_DW.Ei = 0.0;
  Pendulum_controller_DW.Ei_not_empty = true;
  Pendulum_controller_DW.prev_error = 0.0;
  Pendulum_controller_DW.prev_error_not_empty = true;
  Pendulum_controller_DW.sfEvent = Pendulum_controller_CALL_EVENT;
}

/* Model terminate function */
void Pendulum_controller_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<S7>/HIL Initialize' */

  /* S-Function Block: Pendulum_controller/Qube With Pendulum/Qube-Servo 3 - IO (QAL)/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_digital_outputs = 0;
    t_uint32 num_final_other_outputs = 0;
    hil_task_stop_all(Pendulum_controller_DW.HILInitialize_Card);
    hil_monitor_stop_all(Pendulum_controller_DW.HILInitialize_Card);
    is_switching = false;
    if ((Pendulum_controller_P.HILInitialize_AOTerminate && !is_switching) ||
        (Pendulum_controller_P.HILInitialize_AOExit && is_switching)) {
      num_final_analog_outputs = 1U;
    } else {
      num_final_analog_outputs = 0;
    }

    if ((Pendulum_controller_P.HILInitialize_DOTerminate && !is_switching) ||
        (Pendulum_controller_P.HILInitialize_DOExit && is_switching)) {
      num_final_digital_outputs = 1U;
    } else {
      num_final_digital_outputs = 0;
    }

    if ((Pendulum_controller_P.HILInitialize_OOTerminate && !is_switching) ||
        (Pendulum_controller_P.HILInitialize_OOExit && is_switching)) {
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
      result = hil_write(Pendulum_controller_DW.HILInitialize_Card
                         , &Pendulum_controller_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , NULL, 0
                         , &Pendulum_controller_P.HILInitialize_DOChannels,
                         num_final_digital_outputs
                         , Pendulum_controller_P.HILInitialize_OOChannels,
                         num_final_other_outputs
                         , &Pendulum_controller_P.HILInitialize_AOFinal
                         , NULL
                         , (t_boolean *)
                         &Pendulum_controller_P.HILInitialize_DOFinal
                         , Pendulum_controller_P.HILInitialize_OOFinal
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog
            (Pendulum_controller_DW.HILInitialize_Card,
             &Pendulum_controller_P.HILInitialize_AOChannels,
             num_final_analog_outputs,
             &Pendulum_controller_P.HILInitialize_AOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_digital_outputs > 0) {
          local_result = hil_write_digital
            (Pendulum_controller_DW.HILInitialize_Card,
             &Pendulum_controller_P.HILInitialize_DOChannels,
             num_final_digital_outputs, (t_boolean *)
             &Pendulum_controller_P.HILInitialize_DOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_other_outputs > 0) {
          local_result = hil_write_other
            (Pendulum_controller_DW.HILInitialize_Card,
             Pendulum_controller_P.HILInitialize_OOChannels,
             num_final_other_outputs,
             Pendulum_controller_P.HILInitialize_OOFinal);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(Pendulum_controller_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(Pendulum_controller_DW.HILInitialize_Card);
    hil_monitor_delete_all(Pendulum_controller_DW.HILInitialize_Card);
    hil_close(Pendulum_controller_DW.HILInitialize_Card);
    Pendulum_controller_DW.HILInitialize_Card = NULL;
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
  Pendulum_controller_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  Pendulum_controller_update();
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
  Pendulum_controller_initialize();
}

void MdlTerminate(void)
{
  Pendulum_controller_terminate();
}

/* Registration function */
RT_MODEL_Pendulum_controller_T *Pendulum_controller(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)Pendulum_controller_M, 0,
                sizeof(RT_MODEL_Pendulum_controller_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Pendulum_controller_M->solverInfo,
                          &Pendulum_controller_M->Timing.simTimeStep);
    rtsiSetTPtr(&Pendulum_controller_M->solverInfo, &rtmGetTPtr
                (Pendulum_controller_M));
    rtsiSetStepSizePtr(&Pendulum_controller_M->solverInfo,
                       &Pendulum_controller_M->Timing.stepSize0);
    rtsiSetdXPtr(&Pendulum_controller_M->solverInfo,
                 &Pendulum_controller_M->derivs);
    rtsiSetContStatesPtr(&Pendulum_controller_M->solverInfo, (real_T **)
                         &Pendulum_controller_M->contStates);
    rtsiSetNumContStatesPtr(&Pendulum_controller_M->solverInfo,
      &Pendulum_controller_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&Pendulum_controller_M->solverInfo,
      &Pendulum_controller_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&Pendulum_controller_M->solverInfo,
      &Pendulum_controller_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&Pendulum_controller_M->solverInfo,
      &Pendulum_controller_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&Pendulum_controller_M->solverInfo, (boolean_T**)
      &Pendulum_controller_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&Pendulum_controller_M->solverInfo,
                          (&rtmGetErrorStatus(Pendulum_controller_M)));
    rtsiSetRTModelPtr(&Pendulum_controller_M->solverInfo, Pendulum_controller_M);
  }

  rtsiSetSimTimeStep(&Pendulum_controller_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&Pendulum_controller_M->solverInfo, false);
  rtsiSetIsContModeFrozen(&Pendulum_controller_M->solverInfo, false);
  Pendulum_controller_M->intgData.y = Pendulum_controller_M->odeY;
  Pendulum_controller_M->intgData.f[0] = Pendulum_controller_M->odeF[0];
  Pendulum_controller_M->intgData.f[1] = Pendulum_controller_M->odeF[1];
  Pendulum_controller_M->intgData.f[2] = Pendulum_controller_M->odeF[2];
  Pendulum_controller_M->intgData.f[3] = Pendulum_controller_M->odeF[3];
  Pendulum_controller_M->contStates = ((real_T *) &Pendulum_controller_X);
  Pendulum_controller_M->contStateDisabled = ((boolean_T *)
    &Pendulum_controller_XDis);
  Pendulum_controller_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&Pendulum_controller_M->solverInfo, (void *)
                    &Pendulum_controller_M->intgData);
  rtsiSetSolverName(&Pendulum_controller_M->solverInfo,"ode4");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = Pendulum_controller_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    Pendulum_controller_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    Pendulum_controller_M->Timing.sampleTimes =
      (&Pendulum_controller_M->Timing.sampleTimesArray[0]);
    Pendulum_controller_M->Timing.offsetTimes =
      (&Pendulum_controller_M->Timing.offsetTimesArray[0]);

    /* task periods */
    Pendulum_controller_M->Timing.sampleTimes[0] = (0.0);
    Pendulum_controller_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    Pendulum_controller_M->Timing.offsetTimes[0] = (0.0);
    Pendulum_controller_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(Pendulum_controller_M, &Pendulum_controller_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = Pendulum_controller_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    Pendulum_controller_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(Pendulum_controller_M, -1);
  Pendulum_controller_M->Timing.stepSize0 = 0.002;
  Pendulum_controller_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  Pendulum_controller_M->Sizes.checksums[0] = (155879515U);
  Pendulum_controller_M->Sizes.checksums[1] = (3978354801U);
  Pendulum_controller_M->Sizes.checksums[2] = (1951927854U);
  Pendulum_controller_M->Sizes.checksums[3] = (2166938083U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[4];
    Pendulum_controller_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    systemRan[1] = &rtAlwaysEnabled;
    systemRan[2] = &rtAlwaysEnabled;
    systemRan[3] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(Pendulum_controller_M->extModeInfo,
      &Pendulum_controller_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(Pendulum_controller_M->extModeInfo,
                        Pendulum_controller_M->Sizes.checksums);
    rteiSetTPtr(Pendulum_controller_M->extModeInfo, rtmGetTPtr
                (Pendulum_controller_M));
  }

  Pendulum_controller_M->solverInfoPtr = (&Pendulum_controller_M->solverInfo);
  Pendulum_controller_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&Pendulum_controller_M->solverInfo, 0.002);
  rtsiSetSolverMode(&Pendulum_controller_M->solverInfo,
                    SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  Pendulum_controller_M->blockIO = ((void *) &Pendulum_controller_B);
  (void) memset(((void *) &Pendulum_controller_B), 0,
                sizeof(B_Pendulum_controller_T));

  /* parameters */
  Pendulum_controller_M->defaultParam = ((real_T *)&Pendulum_controller_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &Pendulum_controller_X;
    Pendulum_controller_M->contStates = (x);
    (void) memset((void *)&Pendulum_controller_X, 0,
                  sizeof(X_Pendulum_controller_T));
  }

  /* disabled states */
  {
    boolean_T *xdis = (boolean_T *) &Pendulum_controller_XDis;
    Pendulum_controller_M->contStateDisabled = (xdis);
    (void) memset((void *)&Pendulum_controller_XDis, 0,
                  sizeof(XDis_Pendulum_controller_T));
  }

  /* states (dwork) */
  Pendulum_controller_M->dwork = ((void *) &Pendulum_controller_DW);
  (void) memset((void *)&Pendulum_controller_DW, 0,
                sizeof(DW_Pendulum_controller_T));

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    Pendulum_controller_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 22;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  Pendulum_controller_M->Sizes.numContStates = (2);/* Number of continuous states */
  Pendulum_controller_M->Sizes.numPeriodicContStates = (0);
                                      /* Number of periodic continuous states */
  Pendulum_controller_M->Sizes.numY = (0);/* Number of model outputs */
  Pendulum_controller_M->Sizes.numU = (0);/* Number of model inputs */
  Pendulum_controller_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  Pendulum_controller_M->Sizes.numSampTimes = (2);/* Number of sample times */
  Pendulum_controller_M->Sizes.numBlocks = (27);/* Number of blocks */
  Pendulum_controller_M->Sizes.numBlockIO = (6);/* Number of block outputs */
  Pendulum_controller_M->Sizes.numBlockPrms = (106);/* Sum of parameter "widths" */
  return Pendulum_controller_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
