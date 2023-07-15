#include "__cf_normal_operation_no_faults.h"
#ifndef RTW_HEADER_normal_operation_no_faults_acc_private_h_
#define RTW_HEADER_normal_operation_no_faults_acc_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "normal_operation_no_faults_acc.h"
#if !defined(ss_VALIDATE_MEMORY)
#define ss_VALIDATE_MEMORY(S, ptr)   if(!(ptr)) {\
  ssSetErrorStatus(S, RT_MEMORY_ALLOCATION_ERROR);\
  }
#endif
#if !defined(rt_FREE)
#if !defined(_WIN32)
#define rt_FREE(ptr)   if((ptr) != (NULL)) {\
  free((ptr));\
  (ptr) = (NULL);\
  }
#else
#define rt_FREE(ptr)   if((ptr) != (NULL)) {\
  free((void *)(ptr));\
  (ptr) = (NULL);\
  }
#endif
#endif
#ifndef __RTW_UTFREE__
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T
* bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * tBufPtr , real_T * * uBufPtr , real_T * * xBufPtr ,
boolean_T isfixedbuf , boolean_T istransportdelay , int_T * maxNewBufSzPtr )
; real_T normal_operation_no_faults_acc_rt_TDelayInterpolate ( real_T
tMinusDelay , real_T tStart , real_T * tBuf , real_T * uBuf , int_T bufSz ,
int_T * lastIdx , int_T oldestIdx , int_T newIdx , real_T initOutput ,
boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput ) ; real_T
normal_operation_no_faults_acc_rt_VTDelayfindtDInterpolate ( real_T x ,
real_T * tBuf , real_T * uBuf , real_T * xBuf , int_T bufSz , int_T head ,
int_T tail , int_T * pLast , real_T t , real_T tStart , boolean_T discrete ,
boolean_T minorStepAndTAtLastMajorOutput , real_T initOutput , real_T *
appliedDelay ) ; void normal_operation_no_faults_acc_BINARYSEARCH_real_T (
uint32_T * piLeft , uint32_T * piRght , real_T u , const real_T * pData ,
uint32_T iHi ) ; void normal_operation_no_faults_acc_LookUp_real_T_real_T (
real_T * pY , const real_T * pYData , real_T u , const real_T * pUData ,
uint32_T iHi ) ; extern real_T look1_binlxpw ( real_T u0 , const real_T bp0 [
] , const real_T table [ ] , uint32_T maxIndex ) ; void
normal_operation_no_faults_Subsystempi2delay_Init ( SimStruct * S ,
B_Subsystempi2delay_normal_operation_no_faults_T * localB ,
P_Subsystempi2delay_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_Subsystempi2delay ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_normal_operation_no_faults_T * localB ,
DW_Subsystempi2delay_normal_operation_no_faults_T * localDW ) ; void
normal_operation_no_faults_Subsystem1_Init ( SimStruct * S ,
B_Subsystem1_normal_operation_no_faults_T * localB ,
P_Subsystem1_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_Subsystem1 ( SimStruct * S , uint8_T rtu_Enable ,
const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystem1_normal_operation_no_faults_T * localB ,
DW_Subsystem1_normal_operation_no_faults_T * localDW ) ; void
normal_operation_no_faults_Subsystempi2delay_m_Init ( SimStruct * S ,
B_Subsystempi2delay_normal_operation_no_faults_p_T * localB ,
P_Subsystempi2delay_normal_operation_no_faults_h_T * localP ) ; void
normal_operation_no_faults_Subsystempi2delay_Disable ( SimStruct * S ,
DW_Subsystempi2delay_normal_operation_no_faults_i_T * localDW ) ; void
normal_operation_no_faults_Subsystempi2delay_g ( SimStruct * S , uint8_T
rtu_Enable , real_T rtu_dq , real_T rtu_dq_c , real_T rtu_wt ,
B_Subsystempi2delay_normal_operation_no_faults_p_T * localB ,
DW_Subsystempi2delay_normal_operation_no_faults_i_T * localDW ) ; void
normal_operation_no_faults_Subsystem1_e_Init ( SimStruct * S ,
B_Subsystem1_normal_operation_no_faults_l_T * localB ,
P_Subsystem1_normal_operation_no_faults_g_T * localP ) ; void
normal_operation_no_faults_Subsystem1_Disable ( SimStruct * S ,
DW_Subsystem1_normal_operation_no_faults_a_T * localDW ) ; void
normal_operation_no_faults_Subsystem1_g ( SimStruct * S , uint8_T rtu_Enable
, real_T rtu_dq , real_T rtu_dq_n , real_T rtu_wt ,
B_Subsystem1_normal_operation_no_faults_l_T * localB ,
DW_Subsystem1_normal_operation_no_faults_a_T * localDW ) ; void
normal_operation_no_faults_Subsystempi2delay_j_Init ( SimStruct * S ,
B_Subsystempi2delay_normal_operation_no_faults_b_T * localB ,
P_Subsystempi2delay_normal_operation_no_faults_l_T * localP ) ; void
normal_operation_no_faults_Subsystempi2delay_e_Disable ( SimStruct * S ,
DW_Subsystempi2delay_normal_operation_no_faults_k_T * localDW ) ; void
normal_operation_no_faults_Subsystempi2delay_e ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_dq [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_normal_operation_no_faults_b_T * localB ,
DW_Subsystempi2delay_normal_operation_no_faults_k_T * localDW ) ; void
normal_operation_no_faults_Subsystem1_k_Init ( SimStruct * S ,
B_Subsystem1_normal_operation_no_faults_o_T * localB ,
P_Subsystem1_normal_operation_no_faults_h_T * localP ) ; void
normal_operation_no_faults_Subsystem1_c_Disable ( SimStruct * S ,
DW_Subsystem1_normal_operation_no_faults_p_T * localDW ) ; void
normal_operation_no_faults_Subsystem1_h ( SimStruct * S , uint8_T rtu_Enable
, const real_T rtu_dq [ 2 ] , real_T rtu_wt ,
B_Subsystem1_normal_operation_no_faults_o_T * localB ,
DW_Subsystem1_normal_operation_no_faults_p_T * localDW ) ; void
normal_operation_no_faults_ZeroSequenceController_Init ( SimStruct * S ,
B_ZeroSequenceController_normal_operation_no_faults_T * localB ,
P_ZeroSequenceController_normal_operation_no_faults_T * localP ,
X_ZeroSequenceController_normal_operation_no_faults_T * localX ) ; void
normal_operation_no_faults_ZeroSequenceController_Deriv ( SimStruct * S ,
B_ZeroSequenceController_normal_operation_no_faults_T * localB ,
DW_ZeroSequenceController_normal_operation_no_faults_T * localDW ,
P_ZeroSequenceController_normal_operation_no_faults_T * localP ,
X_ZeroSequenceController_normal_operation_no_faults_T * localX ,
XDot_ZeroSequenceController_normal_operation_no_faults_T * localXdot ) ; void
normal_operation_no_faults_ZeroSequenceController_Disable ( SimStruct * S ,
DW_ZeroSequenceController_normal_operation_no_faults_T * localDW ) ; void
normal_operation_no_faults_ZeroSequenceController ( SimStruct * S , real_T
rtu_V0_meas , real_T rtu_I0_meas , real_T rtu_Enable ,
B_ZeroSequenceController_normal_operation_no_faults_T * localB ,
DW_ZeroSequenceController_normal_operation_no_faults_T * localDW ,
P_ZeroSequenceController_normal_operation_no_faults_T * localP ,
X_ZeroSequenceController_normal_operation_no_faults_T * localX ) ; void
normal_operation_no_faults_ZeroSequenceControllerTID4 ( SimStruct * S ,
B_ZeroSequenceController_normal_operation_no_faults_T * localB ,
DW_ZeroSequenceController_normal_operation_no_faults_T * localDW ,
P_ZeroSequenceController_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_RMS_Init ( SimStruct * S ,
B_RMS_normal_operation_no_faults_T * localB ,
DW_RMS_normal_operation_no_faults_T * localDW ,
P_RMS_normal_operation_no_faults_T * localP ,
X_RMS_normal_operation_no_faults_T * localX ) ; void
normal_operation_no_faults_RMS_Deriv ( SimStruct * S ,
B_RMS_normal_operation_no_faults_T * localB ,
DW_RMS_normal_operation_no_faults_T * localDW ,
XDot_RMS_normal_operation_no_faults_T * localXdot ) ; void
normal_operation_no_faults_RMS_Disable ( SimStruct * S ,
DW_RMS_normal_operation_no_faults_T * localDW ) ; void
normal_operation_no_faults_RMS_Update ( SimStruct * S ,
B_RMS_normal_operation_no_faults_T * localB ,
DW_RMS_normal_operation_no_faults_T * localDW ,
P_RMS_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_RMS ( SimStruct * S , boolean_T rtu_Enable , const
real_T rtu_In [ 3 ] , B_RMS_normal_operation_no_faults_T * localB ,
DW_RMS_normal_operation_no_faults_T * localDW ,
P_RMS_normal_operation_no_faults_T * localP ,
X_RMS_normal_operation_no_faults_T * localX ,
XDis_RMS_normal_operation_no_faults_T * localXdis ) ; void
normal_operation_no_faults_TrueRMS_Init ( SimStruct * S ,
B_TrueRMS_normal_operation_no_faults_T * localB ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ,
P_TrueRMS_normal_operation_no_faults_T * localP ,
X_TrueRMS_normal_operation_no_faults_T * localX ) ; void
normal_operation_no_faults_TrueRMS_Deriv ( SimStruct * S ,
B_TrueRMS_normal_operation_no_faults_T * localB ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ,
XDot_TrueRMS_normal_operation_no_faults_T * localXdot ) ; void
normal_operation_no_faults_TrueRMS_ZC ( SimStruct * S ,
B_TrueRMS_normal_operation_no_faults_T * localB ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ,
P_TrueRMS_normal_operation_no_faults_T * localP ,
ZCV_TrueRMS_normal_operation_no_faults_T * localZCSV ) ; void
normal_operation_no_faults_TrueRMS_Disable ( SimStruct * S ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ) ; void
normal_operation_no_faults_TrueRMS_Update ( SimStruct * S ,
B_TrueRMS_normal_operation_no_faults_T * localB ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ,
P_TrueRMS_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_TrueRMS ( SimStruct * S , boolean_T rtu_Enable ,
const real_T rtu_In [ 3 ] , B_TrueRMS_normal_operation_no_faults_T * localB ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ,
P_TrueRMS_normal_operation_no_faults_T * localP ,
X_TrueRMS_normal_operation_no_faults_T * localX ,
XDis_TrueRMS_normal_operation_no_faults_T * localXdis ) ; void
normal_operation_no_faults_ACVoltageController_Init ( SimStruct * S ,
B_ACVoltageController_normal_operation_no_faults_T * localB ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
P_ACVoltageController_normal_operation_no_faults_T * localP ,
X_ACVoltageController_normal_operation_no_faults_T * localX ) ; void
normal_operation_no_faults_ACVoltageController_Deriv ( SimStruct * S ,
B_ACVoltageController_normal_operation_no_faults_T * localB ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
XDot_ACVoltageController_normal_operation_no_faults_T * localXdot ) ; void
normal_operation_no_faults_ACVoltageController_ZC ( SimStruct * S ,
B_ACVoltageController_normal_operation_no_faults_T * localB ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
P_ACVoltageController_normal_operation_no_faults_T * localP ,
ZCV_ACVoltageController_normal_operation_no_faults_T * localZCSV ) ; void
normal_operation_no_faults_ACVoltageController_Disable ( SimStruct * S ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ) ; void
normal_operation_no_faults_ACVoltageController_Update ( SimStruct * S , const
real_T rtu_V_ac [ 3 ] , B_ACVoltageController_normal_operation_no_faults_T *
localB , DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
P_ACVoltageController_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_ACVoltageController ( SimStruct * S , real_T
rtu_Enable , real_T rtu_V_ac_ref ,
B_ACVoltageController_normal_operation_no_faults_T * localB ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
P_ACVoltageController_normal_operation_no_faults_T * localP ,
X_ACVoltageController_normal_operation_no_faults_T * localX ,
XDis_ACVoltageController_normal_operation_no_faults_T * localXdis ) ; void
normal_operation_no_faults_ACVoltageControllerTID4 ( SimStruct * S ,
B_ACVoltageController_normal_operation_no_faults_T * localB ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
P_ACVoltageController_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_Circulatingcurrentsuppression_Init ( SimStruct * S
, B_Circulatingcurrentsuppression_normal_operation_no_faults_T * localB ,
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T * localDW ,
P_Circulatingcurrentsuppression_normal_operation_no_faults_T * localP ,
X_Circulatingcurrentsuppression_normal_operation_no_faults_T * localX ) ;
void normal_operation_no_faults_Circulatingcurrentsuppression_Deriv (
SimStruct * S , B_Circulatingcurrentsuppression_normal_operation_no_faults_T
* localB , DW_Circulatingcurrentsuppression_normal_operation_no_faults_T *
localDW , XDot_Circulatingcurrentsuppression_normal_operation_no_faults_T *
localXdot ) ; void
normal_operation_no_faults_Circulatingcurrentsuppression_ZC ( SimStruct * S ,
B_Circulatingcurrentsuppression_normal_operation_no_faults_T * localB ,
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T * localDW ,
P_Circulatingcurrentsuppression_normal_operation_no_faults_T * localP ,
ZCV_Circulatingcurrentsuppression_normal_operation_no_faults_T * localZCSV )
; void normal_operation_no_faults_Circulatingcurrentsuppression_Disable (
SimStruct * S , DW_Circulatingcurrentsuppression_normal_operation_no_faults_T
* localDW ) ; void
normal_operation_no_faults_Circulatingcurrentsuppression_Update ( SimStruct *
S , real_T rtu_I_arm , real_T rtu_I_arm_a , real_T rtu_I_arm_e , real_T
rtu_I_arm_f , real_T rtu_I_arm_g , real_T rtu_I_arm_ep ,
B_Circulatingcurrentsuppression_normal_operation_no_faults_T * localB ,
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T * localDW ) ;
void normal_operation_no_faults_Circulatingcurrentsuppression ( SimStruct * S
, real_T rtu_Enable ,
B_Circulatingcurrentsuppression_normal_operation_no_faults_T * localB ,
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T * localDW ,
P_Circulatingcurrentsuppression_normal_operation_no_faults_T * localP ,
X_Circulatingcurrentsuppression_normal_operation_no_faults_T * localX ,
XDis_Circulatingcurrentsuppression_normal_operation_no_faults_T * localXdis )
; void normal_operation_no_faults_CirculatingcurrentsuppressionTID4 (
SimStruct * S , B_Circulatingcurrentsuppression_normal_operation_no_faults_T
* localB , DW_Circulatingcurrentsuppression_normal_operation_no_faults_T *
localDW , P_Circulatingcurrentsuppression_normal_operation_no_faults_T *
localP ) ; void normal_operation_no_faults_Subsystempi2delay_d_Init (
SimStruct * S , B_Subsystempi2delay_normal_operation_no_faults_e_T * localB ,
P_Subsystempi2delay_normal_operation_no_faults_g_T * localP ) ; void
normal_operation_no_faults_Subsystempi2delay_j_Disable ( SimStruct * S ,
DW_Subsystempi2delay_normal_operation_no_faults_c_T * localDW ) ; void
normal_operation_no_faults_Subsystempi2delay_m ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_normal_operation_no_faults_e_T * localB ,
DW_Subsystempi2delay_normal_operation_no_faults_c_T * localDW ) ; void
normal_operation_no_faults_Subsystem1_i_Init ( SimStruct * S ,
B_Subsystem1_normal_operation_no_faults_p_T * localB ,
P_Subsystem1_normal_operation_no_faults_j_T * localP ) ; void
normal_operation_no_faults_Subsystem1_m_Disable ( SimStruct * S ,
DW_Subsystem1_normal_operation_no_faults_d_T * localDW ) ; void
normal_operation_no_faults_Subsystem1_h4 ( SimStruct * S , uint8_T rtu_Enable
, const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystem1_normal_operation_no_faults_p_T * localB ,
DW_Subsystem1_normal_operation_no_faults_d_T * localDW ) ; void
normal_operation_no_faults_AutomaticGainControl_Init ( SimStruct * S ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ,
X_AutomaticGainControl_normal_operation_no_faults_T * localX ) ; void
normal_operation_no_faults_AutomaticGainControl_Deriv ( SimStruct * S ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ,
XDot_AutomaticGainControl_normal_operation_no_faults_T * localXdot ) ; void
normal_operation_no_faults_AutomaticGainControl_ZC ( SimStruct * S ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ,
ZCV_AutomaticGainControl_normal_operation_no_faults_T * localZCSV ) ; void
normal_operation_no_faults_AutomaticGainControl_Disable ( SimStruct * S ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ) ; void
normal_operation_no_faults_AutomaticGainControl_Update ( SimStruct * S ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ,
X_AutomaticGainControl_normal_operation_no_faults_T * localX ) ; void
normal_operation_no_faults_AutomaticGainControl ( SimStruct * S , real_T
rtu_Enable , real_T rtu_Freq , real_T rtu_wt , const real_T rtu_Vabc [ 3 ] ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ,
X_AutomaticGainControl_normal_operation_no_faults_T * localX ,
XDis_AutomaticGainControl_normal_operation_no_faults_T * localXdis ) ; void
normal_operation_no_faults_AutomaticGainControlTID4 ( SimStruct * S ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_HarmonicGenerator_Init ( SimStruct * S ,
B_HarmonicGenerator_normal_operation_no_faults_T * localB ,
P_HarmonicGenerator_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_HarmonicGenerator_Disable ( SimStruct * S ,
B_HarmonicGenerator_normal_operation_no_faults_T * localB ,
DW_HarmonicGenerator_normal_operation_no_faults_T * localDW ,
P_HarmonicGenerator_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_HarmonicGenerator ( SimStruct * S , real_T
rtu_Enable , real_T rtu_wt , B_HarmonicGenerator_normal_operation_no_faults_T
* localB , DW_HarmonicGenerator_normal_operation_no_faults_T * localDW ,
P_HarmonicGenerator_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_HarmonicGeneratorTID4 ( SimStruct * S ,
B_HarmonicGenerator_normal_operation_no_faults_T * localB ,
DW_HarmonicGenerator_normal_operation_no_faults_T * localDW ,
P_HarmonicGenerator_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_Signalgenerator_Init ( SimStruct * S ,
B_Signalgenerator_normal_operation_no_faults_T * localB ,
DW_Signalgenerator_normal_operation_no_faults_T * localDW ,
P_Signalgenerator_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_Signalgenerator_Disable ( SimStruct * S ,
B_Signalgenerator_normal_operation_no_faults_T * localB ,
DW_Signalgenerator_normal_operation_no_faults_T * localDW ,
P_Signalgenerator_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_Signalgenerator_Update ( SimStruct * S ,
B_Signalgenerator_normal_operation_no_faults_T * localB ,
DW_Signalgenerator_normal_operation_no_faults_T * localDW ,
P_Signalgenerator_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_Signalgenerator ( SimStruct * S , real_T
rtu_Enable , B_Signalgenerator_normal_operation_no_faults_T * localB ,
DW_Signalgenerator_normal_operation_no_faults_T * localDW ,
P_Signalgenerator_normal_operation_no_faults_T * localP ) ; void
normal_operation_no_faults_SignalgeneratorTID4 ( SimStruct * S ,
B_Signalgenerator_normal_operation_no_faults_T * localB ,
DW_Signalgenerator_normal_operation_no_faults_T * localDW ,
P_Signalgenerator_normal_operation_no_faults_T * localP ) ;
#endif
