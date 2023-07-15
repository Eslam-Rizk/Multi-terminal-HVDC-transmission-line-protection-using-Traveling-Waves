#include "__cf_normal_operation_no_faults.h"
#ifndef RTW_HEADER_normal_operation_no_faults_acc_h_
#define RTW_HEADER_normal_operation_no_faults_acc_h_
#include <stddef.h>
#include <float.h>
#include <string.h>
#ifndef normal_operation_no_faults_acc_COMMON_INCLUDES_
#define normal_operation_no_faults_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn 
#define S_FUNCTION_LEVEL 2
#define RTW_GENERATED_S_FUNCTION
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simtarget/slSimTgtSigstreamRTW.h"
#include "simtarget/slSimTgtSlioCoreRTW.h"
#include "simtarget/slSimTgtSlioClientsRTW.h"
#include "simtarget/slSimTgtSlioSdiRTW.h"
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "normal_operation_no_faults_acc_types.h"
#include "multiword_types.h"
#include "mwmathutil.h"
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#include "rt_look.h"
#include "rt_look1d.h"
#include "rt_defines.h"
typedef struct { real_T B_0_0_0 ; real_T B_0_1_0 ; }
B_Subsystempi2delay_normal_operation_no_faults_T ; typedef struct { int32_T
Subsystempi2delay_sysIdxToRun ; int8_T Subsystempi2delay_SubsysRanBC ; char_T
pad_Subsystempi2delay_SubsysRanBC [ 3 ] ; }
DW_Subsystempi2delay_normal_operation_no_faults_T ; typedef struct { real_T
B_1_0_0 ; real_T B_1_1_0 ; } B_Subsystem1_normal_operation_no_faults_T ;
typedef struct { int32_T Subsystem1_sysIdxToRun ; int8_T
Subsystem1_SubsysRanBC ; char_T pad_Subsystem1_SubsysRanBC [ 3 ] ; }
DW_Subsystem1_normal_operation_no_faults_T ; typedef struct { real_T B_2_0_0
; real_T B_2_1_0 ; } B_Subsystempi2delay_normal_operation_no_faults_p_T ;
typedef struct { int32_T Subsystempi2delay_sysIdxToRun ; int8_T
Subsystempi2delay_SubsysRanBC ; boolean_T Subsystempi2delay_MODE ; char_T
pad_Subsystempi2delay_MODE [ 2 ] ; }
DW_Subsystempi2delay_normal_operation_no_faults_i_T ; typedef struct { real_T
B_3_0_0 ; real_T B_3_1_0 ; } B_Subsystem1_normal_operation_no_faults_l_T ;
typedef struct { int32_T Subsystem1_sysIdxToRun ; int8_T
Subsystem1_SubsysRanBC ; boolean_T Subsystem1_MODE ; char_T
pad_Subsystem1_MODE [ 2 ] ; } DW_Subsystem1_normal_operation_no_faults_a_T ;
typedef struct { real_T B_5_0_0 ; real_T B_5_1_0 ; }
B_Subsystempi2delay_normal_operation_no_faults_b_T ; typedef struct { int32_T
Subsystempi2delay_sysIdxToRun ; int8_T Subsystempi2delay_SubsysRanBC ;
boolean_T Subsystempi2delay_MODE ; char_T pad_Subsystempi2delay_MODE [ 2 ] ;
} DW_Subsystempi2delay_normal_operation_no_faults_k_T ; typedef struct {
real_T B_6_0_0 ; real_T B_6_1_0 ; }
B_Subsystem1_normal_operation_no_faults_o_T ; typedef struct { int32_T
Subsystem1_sysIdxToRun ; int8_T Subsystem1_SubsysRanBC ; boolean_T
Subsystem1_MODE ; char_T pad_Subsystem1_MODE [ 2 ] ; }
DW_Subsystem1_normal_operation_no_faults_p_T ; typedef struct { real_T
B_19_0_0 ; real_T B_19_2_0 ; real_T B_19_3_0 ; real_T B_19_4_0 ; real_T
B_19_5_0 ; real_T B_19_6_0 ; real_T B_19_7_0 ; real_T B_19_8_0 ; real_T
B_19_9_0 ; real_T B_19_10_0 ; real_T B_19_11_0 ; real_T B_19_12_0 ; real_T
B_19_13_0 ; real_T B_19_14_0 ; real_T B_19_15_0 ; real_T B_19_16_0 ; real_T
B_19_17_0 ; real_T B_19_18_0 ; real_T B_19_19_0 ; real_T B_19_20_0 ; real_T
B_19_21_0 ; real_T B_19_22_0 ; real_T B_19_23_0 ; real_T B_19_24_0 ; real_T
B_19_25_0 ; real_T B_19_26_0 ; real_T B_19_27_0 ; real_T B_19_28_0 ; real_T
B_19_29_0 ; real_T B_19_30_0 ; real_T B_19_31_0 ; real_T B_19_32_0 ; real_T
B_19_33_0 ; real_T B_19_34_0 ; real_T B_19_36_0 ; real_T B_18_0_0 ; real_T
B_18_1_0 ; real_T B_18_2_0 ; real_T B_18_3_0 ; real_T B_18_4_0 ; real_T
B_18_5_0 ; real_T B_18_6_0 ; }
B_ZeroSequenceController_normal_operation_no_faults_T ; typedef struct {
int32_T ZeroSequenceController_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int8_T
ZeroSequenceController_SubsysRanBC ; boolean_T ZeroSequenceController_MODE ;
char_T pad_ZeroSequenceController_MODE [ 6 ] ; }
DW_ZeroSequenceController_normal_operation_no_faults_T ; typedef struct {
real_T Integrator_x1_CSTATE_p ; real_T Integrator_x2_CSTATE_o ; real_T
Integrator_x1_CSTATE_c ; real_T Integrator_x2_CSTATE_l ; real_T
TransferFcn1_CSTATE [ 2 ] ; }
X_ZeroSequenceController_normal_operation_no_faults_T ; typedef struct {
real_T Integrator_x1_CSTATE_p ; real_T Integrator_x2_CSTATE_o ; real_T
Integrator_x1_CSTATE_c ; real_T Integrator_x2_CSTATE_l ; real_T
TransferFcn1_CSTATE [ 2 ] ; }
XDot_ZeroSequenceController_normal_operation_no_faults_T ; typedef struct {
boolean_T Integrator_x1_CSTATE_p ; boolean_T Integrator_x2_CSTATE_o ;
boolean_T Integrator_x1_CSTATE_c ; boolean_T Integrator_x2_CSTATE_l ;
boolean_T TransferFcn1_CSTATE [ 2 ] ; }
XDis_ZeroSequenceController_normal_operation_no_faults_T ; typedef struct {
real_T Integrator_x1_CSTATE_p ; real_T Integrator_x2_CSTATE_o ; real_T
Integrator_x1_CSTATE_c ; real_T Integrator_x2_CSTATE_l ; real_T
TransferFcn1_CSTATE [ 2 ] ; }
XAbsTol_ZeroSequenceController_normal_operation_no_faults_T ; typedef struct
{ creal_T B_22_16_0 [ 3 ] ; real_T B_22_0_0 [ 3 ] ; real_T B_22_1_0 [ 3 ] ;
real_T B_22_3_0 ; real_T B_22_5_0 [ 3 ] ; real_T B_22_7_0 [ 3 ] ; real_T
B_22_8_0 [ 3 ] ; real_T B_22_9_0 [ 3 ] ; real_T B_22_11_0 ; real_T B_22_13_0
[ 3 ] ; real_T B_22_15_0 [ 3 ] ; real_T B_22_19_0 [ 3 ] ; real_T B_22_21_0 [
3 ] ; real_T B_22_22_0 [ 3 ] ; real_T B_21_0_0 [ 3 ] ; real_T B_21_1_0 [ 3 ]
; real_T B_20_0_0 [ 3 ] ; real_T B_20_1_0 [ 3 ] ; }
B_RMS_normal_operation_no_faults_T ; typedef struct { real_T
Memory_PreviousInput [ 3 ] ; real_T Memory_PreviousInput_o [ 3 ] ; struct {
real_T modelTStart ; } TransportDelay_RWORK ; struct { real_T modelTStart ; }
TransportDelay_RWORK_n ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_f ; int32_T RMS_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_g ; struct { int_T Tail [ 3 ] ;
int_T Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK ; struct { int_T Tail [ 3 ] ; int_T
Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_b ; int8_T RMS_SubsysRanBC ; boolean_T
RMS_MODE ; char_T pad_RMS_MODE [ 2 ] ; } DW_RMS_normal_operation_no_faults_T
; typedef struct { real_T integrator_CSTATE_kw [ 3 ] ; real_T
integrator_CSTATE_l0 [ 3 ] ; } X_RMS_normal_operation_no_faults_T ; typedef
struct { real_T integrator_CSTATE_kw [ 3 ] ; real_T integrator_CSTATE_l0 [ 3
] ; } XDot_RMS_normal_operation_no_faults_T ; typedef struct { boolean_T
integrator_CSTATE_kw [ 3 ] ; boolean_T integrator_CSTATE_l0 [ 3 ] ; }
XDis_RMS_normal_operation_no_faults_T ; typedef struct { real_T
integrator_CSTATE_kw [ 3 ] ; real_T integrator_CSTATE_l0 [ 3 ] ; }
XAbsTol_RMS_normal_operation_no_faults_T ; typedef struct { real_T B_24_1_0 [
3 ] ; real_T B_24_2_0 [ 3 ] ; real_T B_24_3_0 ; real_T B_24_4_0 [ 3 ] ;
real_T B_24_7_0 [ 3 ] ; real_T B_24_8_0 [ 3 ] ; real_T B_24_9_0 [ 3 ] ;
real_T B_24_10_0 [ 3 ] ; real_T B_23_0_0 [ 3 ] ; real_T B_23_1_0 [ 3 ] ; }
B_TrueRMS_normal_operation_no_faults_T ; typedef struct { real_T
Memory_PreviousInput [ 3 ] ; struct { real_T modelTStart ; }
TransportDelay_RWORK ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK ; int32_T TrueRMS_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; struct { int_T Tail [ 3 ] ;
int_T Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK ; int_T
Saturationtoavoidnegativesqrt_MODE [ 3 ] ; int8_T TrueRMS_SubsysRanBC ;
int8_T Sqrt_DWORK1 ; boolean_T TrueRMS_MODE ; char_T pad_TrueRMS_MODE [ 5 ] ;
} DW_TrueRMS_normal_operation_no_faults_T ; typedef struct { real_T
integrator_CSTATE_el [ 3 ] ; } X_TrueRMS_normal_operation_no_faults_T ;
typedef struct { real_T integrator_CSTATE_el [ 3 ] ; }
XDot_TrueRMS_normal_operation_no_faults_T ; typedef struct { boolean_T
integrator_CSTATE_el [ 3 ] ; } XDis_TrueRMS_normal_operation_no_faults_T ;
typedef struct { real_T integrator_CSTATE_el [ 3 ] ; }
XAbsTol_TrueRMS_normal_operation_no_faults_T ; typedef struct { real_T
Saturationtoavoidnegativesqrt_UprLim_ZC [ 3 ] ; real_T
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 3 ] ; }
ZCV_TrueRMS_normal_operation_no_faults_T ; typedef struct { ZCSigState
Saturationtoavoidnegativesqrt_UprLim_ZCE [ 3 ] ; ZCSigState
Saturationtoavoidnegativesqrt_LwrLim_ZCE [ 3 ] ; }
ZCE_TrueRMS_normal_operation_no_faults_T ; typedef struct { real_T B_25_1_0 [
3 ] ; real_T B_25_2_0 [ 3 ] ; real_T B_25_8_0 [ 3 ] ; real_T B_25_9_0 [ 3 ] ;
real_T B_25_10_0 ; real_T B_25_11_0 ; real_T B_25_12_0 ; real_T B_25_13_0 ;
real_T B_25_14_0 ; real_T B_25_15_0 ; real_T B_25_16_0 ; real_T B_25_17_0 ;
real_T B_25_18_0 ; real_T B_25_19_0 ; boolean_T B_25_4_0 ; boolean_T B_25_6_0
; char_T pad_B_25_6_0 [ 6 ] ; B_TrueRMS_normal_operation_no_faults_T TrueRMS
; B_RMS_normal_operation_no_faults_T RMS ; }
B_ACVoltageController_normal_operation_no_faults_T ; typedef struct { real_T
UnitDelay_DSTATE [ 3 ] ; int32_T ACVoltageController_sysIdxToRun ; int_T
Saturation_MODE ; int8_T ACVoltageController_SubsysRanBC ; boolean_T
ACVoltageController_MODE ; char_T pad_ACVoltageController_MODE [ 6 ] ;
DW_TrueRMS_normal_operation_no_faults_T TrueRMS ;
DW_RMS_normal_operation_no_faults_T RMS ; }
DW_ACVoltageController_normal_operation_no_faults_T ; typedef struct { real_T
Integrator_CSTATE_or ; X_TrueRMS_normal_operation_no_faults_T TrueRMS ;
X_RMS_normal_operation_no_faults_T RMS ; }
X_ACVoltageController_normal_operation_no_faults_T ; typedef struct { real_T
Integrator_CSTATE_or ; XDot_TrueRMS_normal_operation_no_faults_T TrueRMS ;
XDot_RMS_normal_operation_no_faults_T RMS ; }
XDot_ACVoltageController_normal_operation_no_faults_T ; typedef struct {
boolean_T Integrator_CSTATE_or ; XDis_TrueRMS_normal_operation_no_faults_T
TrueRMS ; XDis_RMS_normal_operation_no_faults_T RMS ; }
XDis_ACVoltageController_normal_operation_no_faults_T ; typedef struct {
real_T Integrator_CSTATE_or ; XAbsTol_TrueRMS_normal_operation_no_faults_T
TrueRMS ; XAbsTol_RMS_normal_operation_no_faults_T RMS ; }
XAbsTol_ACVoltageController_normal_operation_no_faults_T ; typedef struct {
real_T Saturation_UprLim_ZC_g ; real_T Saturation_LwrLim_ZC_e ;
ZCV_TrueRMS_normal_operation_no_faults_T TrueRMS ; }
ZCV_ACVoltageController_normal_operation_no_faults_T ; typedef struct {
ZCSigState Saturation_UprLim_ZCE_j ; ZCSigState Saturation_LwrLim_ZCE_i ;
ZCE_TrueRMS_normal_operation_no_faults_T TrueRMS ; }
ZCE_ACVoltageController_normal_operation_no_faults_T ; typedef struct {
real_T B_30_0_0 ; real_T B_30_1_0 ; real_T B_30_2_0 ; real_T B_30_3_0 ;
real_T B_30_6_0 [ 6 ] ; real_T B_30_17_0 ; real_T B_30_26_0 ; real_T
B_30_27_0 ; real_T B_30_29_0 ; real_T B_30_30_0 ; real_T B_30_31_0 ; real_T
B_30_32_0 ; real_T B_30_34_0 ; real_T B_30_35_0 ; real_T B_30_36_0 ; real_T
B_30_37_0 ; real_T B_30_39_0 ; real_T B_30_40_0 ; real_T B_30_43_0 ; real_T
B_30_44_0 ; real_T B_30_45_0 ; real_T B_30_53_0 [ 2 ] ; real_T B_30_54_0 [ 3
] ; real_T B_30_55_0 [ 3 ] ; real_T B_30_56_0 [ 3 ] ; real_T B_30_57_0 [ 3 ]
; uint8_T B_30_20_0 ; uint8_T B_30_23_0 ; uint8_T B_30_48_0 ; uint8_T
B_30_51_0 ; char_T pad_B_30_51_0 [ 4 ] ;
B_Subsystem1_normal_operation_no_faults_l_T Subsystem1_k ;
B_Subsystempi2delay_normal_operation_no_faults_p_T Subsystempi2delay_e ;
B_Subsystem1_normal_operation_no_faults_T Subsystem1 ;
B_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay ; }
B_Circulatingcurrentsuppression_normal_operation_no_faults_T ; typedef struct
{ real_T UnitDelay9_DSTATE [ 6 ] ; real_T UnitDelay1_DSTATE ; int32_T
Circulatingcurrentsuppression_sysIdxToRun ; int_T Saturation1_MODE [ 3 ] ;
int8_T Circulatingcurrentsuppression_SubsysRanBC ; boolean_T
Circulatingcurrentsuppression_MODE ; char_T
pad_Circulatingcurrentsuppression_MODE [ 6 ] ;
DW_Subsystem1_normal_operation_no_faults_a_T Subsystem1_k ;
DW_Subsystempi2delay_normal_operation_no_faults_i_T Subsystempi2delay_e ;
DW_Subsystem1_normal_operation_no_faults_T Subsystem1 ;
DW_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay ; }
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T ; typedef
struct { real_T Integrator_CSTATE_f ; real_T Integrator_CSTATE_f0 ; }
X_Circulatingcurrentsuppression_normal_operation_no_faults_T ; typedef struct
{ real_T Integrator_CSTATE_f ; real_T Integrator_CSTATE_f0 ; }
XDot_Circulatingcurrentsuppression_normal_operation_no_faults_T ; typedef
struct { boolean_T Integrator_CSTATE_f ; boolean_T Integrator_CSTATE_f0 ; }
XDis_Circulatingcurrentsuppression_normal_operation_no_faults_T ; typedef
struct { real_T Integrator_CSTATE_f ; real_T Integrator_CSTATE_f0 ; }
XAbsTol_Circulatingcurrentsuppression_normal_operation_no_faults_T ; typedef
struct { real_T Saturation1_UprLim_ZC [ 3 ] ; real_T Saturation1_LwrLim_ZC [
3 ] ; } ZCV_Circulatingcurrentsuppression_normal_operation_no_faults_T ;
typedef struct { ZCSigState Saturation1_UprLim_ZCE [ 3 ] ; ZCSigState
Saturation1_LwrLim_ZCE [ 3 ] ; }
ZCE_Circulatingcurrentsuppression_normal_operation_no_faults_T ; typedef
struct { real_T B_34_0_0 ; real_T B_34_1_0 ; }
B_Subsystempi2delay_normal_operation_no_faults_e_T ; typedef struct { int32_T
Subsystempi2delay_sysIdxToRun ; int8_T Subsystempi2delay_SubsysRanBC ;
boolean_T Subsystempi2delay_MODE ; char_T pad_Subsystempi2delay_MODE [ 2 ] ;
} DW_Subsystempi2delay_normal_operation_no_faults_c_T ; typedef struct {
real_T B_35_0_0 ; real_T B_35_1_0 ; }
B_Subsystem1_normal_operation_no_faults_p_T ; typedef struct { int32_T
Subsystem1_sysIdxToRun ; int8_T Subsystem1_SubsysRanBC ; boolean_T
Subsystem1_MODE ; char_T pad_Subsystem1_MODE [ 2 ] ; }
DW_Subsystem1_normal_operation_no_faults_d_T ; typedef struct { creal_T
B_42_16_0 ; real_T B_42_0_0 ; real_T B_42_1_0 ; real_T B_42_2_0 ; real_T
B_42_3_0 ; real_T B_42_5_0 ; real_T B_42_7_0 ; real_T B_42_8_0 ; real_T
B_42_9_0 ; real_T B_42_10_0 ; real_T B_42_11_0 ; real_T B_42_13_0 ; real_T
B_42_15_0 ; real_T B_42_17_0 ; real_T B_42_18_0 ; real_T B_42_19_0 ; real_T
B_42_20_0 ; real_T B_42_21_0 ; real_T B_42_22_0 ; real_T B_42_23_0 ; real_T
B_42_29_0 [ 3 ] ; real_T B_42_30_0 [ 3 ] ; real_T B_42_33_0 [ 2 ] ; real_T
B_39_0_0 ; real_T B_39_1_0 ; real_T B_38_0_0 ; real_T B_38_1_0 ; uint8_T
B_42_26_0 ; uint8_T B_42_28_0 ; boolean_T B_42_4_0 ; boolean_T B_42_12_0 ;
char_T pad_B_42_12_0 [ 4 ] ; B_Subsystem1_normal_operation_no_faults_p_T
Subsystem1 ; B_Subsystempi2delay_normal_operation_no_faults_e_T
Subsystempi2delay ; } B_AutomaticGainControl_normal_operation_no_faults_T ;
typedef struct { real_T Memory_PreviousInput ; real_T Memory_PreviousInput_n
; struct { real_T modelTStart ; } VariableTransportDelay_RWORK ; struct {
real_T modelTStart ; } VariableTransportDelay_RWORK_g ; struct { void *
TUbufferPtrs [ 3 ] ; } VariableTransportDelay_PWORK ; struct { void *
TUbufferPtrs [ 3 ] ; } VariableTransportDelay_PWORK_d ; int32_T
AutomaticGainControl_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_h ; struct { int_T Tail ; int_T
Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
VariableTransportDelay_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last
; int_T CircularBufSize ; int_T MaxNewBufSize ; }
VariableTransportDelay_IWORK_o ; int_T Saturation_MODE ; int8_T
AutomaticGainControl_SubsysRanBC ; boolean_T RelationalOperator_Mode ;
boolean_T RelationalOperator_Mode_j ; boolean_T AutomaticGainControl_MODE ;
char_T pad_AutomaticGainControl_MODE [ 4 ] ;
DW_Subsystem1_normal_operation_no_faults_d_T Subsystem1 ;
DW_Subsystempi2delay_normal_operation_no_faults_c_T Subsystempi2delay ; }
DW_AutomaticGainControl_normal_operation_no_faults_T ; typedef struct {
real_T VariableTransportDelay_CSTATE_n ; real_T integrator_CSTATE_av ; real_T
VariableTransportDelay_CSTATE_c ; real_T integrator_CSTATE_cr ; }
X_AutomaticGainControl_normal_operation_no_faults_T ; typedef struct { real_T
VariableTransportDelay_CSTATE_n ; real_T integrator_CSTATE_av ; real_T
VariableTransportDelay_CSTATE_c ; real_T integrator_CSTATE_cr ; }
XDot_AutomaticGainControl_normal_operation_no_faults_T ; typedef struct {
boolean_T VariableTransportDelay_CSTATE_n ; boolean_T integrator_CSTATE_av ;
boolean_T VariableTransportDelay_CSTATE_c ; boolean_T integrator_CSTATE_cr ;
} XDis_AutomaticGainControl_normal_operation_no_faults_T ; typedef struct {
real_T VariableTransportDelay_CSTATE_n ; real_T integrator_CSTATE_av ; real_T
VariableTransportDelay_CSTATE_c ; real_T integrator_CSTATE_cr ; }
XAbsTol_AutomaticGainControl_normal_operation_no_faults_T ; typedef struct {
real_T RelationalOperator_RelopInput_ZC_b ; real_T
RelationalOperator_RelopInput_ZC_i ; real_T Saturation_UprLim_ZC_b ; real_T
Saturation_LwrLim_ZC_b ; }
ZCV_AutomaticGainControl_normal_operation_no_faults_T ; typedef struct {
ZCSigState RelationalOperator_RelopInput_ZCE_p ; ZCSigState
RelationalOperator_RelopInput_ZCE_e4 ; ZCSigState Saturation_UprLim_ZCE_f ;
ZCSigState Saturation_LwrLim_ZCE_ep ; }
ZCE_AutomaticGainControl_normal_operation_no_faults_T ; typedef struct {
real_T B_122_2_0 ; real_T B_122_9_0 [ 3 ] ; real_T B_122_10_0 ; real_T
B_122_16_0 [ 3 ] ; real_T B_122_19_0 ; real_T B_122_26_0 [ 3 ] ; real_T
B_122_27_0 ; real_T B_122_30_0 [ 3 ] ; }
B_HarmonicGenerator_normal_operation_no_faults_T ; typedef struct { int32_T
HarmonicGenerator_sysIdxToRun ; int8_T HarmonicGenerator_SubsysRanBC ;
boolean_T HarmonicGenerator_MODE ; char_T pad_HarmonicGenerator_MODE [ 2 ] ;
} DW_HarmonicGenerator_normal_operation_no_faults_T ; typedef struct { real_T
B_127_1_0 ; real_T B_127_3_0 ; real_T B_127_6_0 ; real_T B_127_7_0 ; real_T
B_127_9_0 ; real_T B_127_10_0 ; real_T B_127_14_0 ; real_T B_127_19_0 ;
real_T B_127_21_0 ; real_T B_127_22_0 ; real_T B_127_24_0 ; real_T B_127_25_0
; real_T B_127_28_0 ; real_T B_127_31_0 ; real_T B_127_33_0 ; real_T
B_127_34_0 ; real_T B_127_35_0 ; real_T B_127_36_0 ; real_T B_127_37_0 ;
real_T B_127_38_0 ; real_T B_127_39_0 ; real_T B_127_41_0 ; real_T B_127_44_0
; real_T B_127_47_0 ; real_T B_127_48_0 ; boolean_T B_127_17_0 ; char_T
pad_B_127_17_0 [ 7 ] ; } B_Signalgenerator_normal_operation_no_faults_T ;
typedef struct { real_T DiscreteTimeIntegrator_DSTATE ; real_T
DiscreteTimeIntegrator1_DSTATE ; real_T UnitDelay_DSTATE ; int32_T
Signalgenerator_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch2Inport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtMultiportSwitch1Inport2_sysIdxToRun ; int32_T
TmpAtomicSubsysAtMultiportSwitch1Inport4_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch3Inport1_sysIdxToRun ; int8_T
Signalgenerator_SubsysRanBC ; uint8_T DiscreteTimeIntegrator_NumInitCond ;
uint8_T DiscreteTimeIntegrator1_NumInitCond ; boolean_T Signalgenerator_MODE
; } DW_Signalgenerator_normal_operation_no_faults_T ; typedef struct { real_T
B_185_0_0 [ 95 ] ; real_T B_185_1_0 [ 2 ] ; real_T B_185_3_0 [ 3 ] ; real_T
B_185_7_0 [ 3 ] ; real_T B_185_8_0 ; real_T B_185_10_0 ; real_T B_185_11_0 ;
real_T B_185_12_0 ; real_T B_185_20_0 [ 2 ] ; real_T B_185_21_0 ; real_T
B_185_28_0 [ 3 ] ; real_T B_185_31_0 ; real_T B_185_32_0 ; real_T B_185_33_0
; real_T B_185_35_0 ; real_T B_185_37_0 ; real_T B_185_39_0 ; real_T
B_185_41_0 ; real_T B_185_46_0 ; real_T B_185_53_0 ; real_T B_185_54_0 ;
real_T B_185_55_0 ; real_T B_185_58_0 ; real_T B_185_60_0 [ 3 ] ; real_T
B_185_63_0 [ 3 ] ; real_T B_185_71_0 [ 2 ] ; real_T B_185_72_0 ; real_T
B_185_74_0 ; real_T B_185_76_0 ; real_T B_185_81_0 ; real_T B_185_88_0 ;
real_T B_185_89_0 ; real_T B_185_91_0 ; real_T B_185_93_0 ; real_T B_185_94_0
; real_T B_185_99_0 ; real_T B_185_102_0 ; real_T B_185_104_0 ; real_T
B_185_108_0 ; real_T B_185_109_0 ; real_T B_185_121_0 ; real_T B_185_125_0 ;
real_T B_185_127_0 [ 6 ] ; real_T B_185_128_0 [ 6 ] ; real_T B_185_130_0 [ 50
] ; real_T B_185_131_0 ; real_T B_185_132_0 ; real_T B_185_133_0 ; real_T
B_185_137_0 ; real_T B_185_141_0 [ 2 ] ; real_T B_185_143_0 [ 50 ] ; real_T
B_185_144_0 ; real_T B_185_145_0 ; real_T B_185_146_0 ; real_T B_185_150_0 ;
real_T B_185_154_0 [ 2 ] ; real_T B_185_156_0 [ 50 ] ; real_T B_185_157_0 ;
real_T B_185_158_0 ; real_T B_185_159_0 ; real_T B_185_163_0 ; real_T
B_185_167_0 [ 2 ] ; real_T B_185_169_0 [ 50 ] ; real_T B_185_170_0 ; real_T
B_185_171_0 ; real_T B_185_172_0 ; real_T B_185_176_0 ; real_T B_185_180_0 [
2 ] ; real_T B_185_182_0 [ 50 ] ; real_T B_185_183_0 ; real_T B_185_184_0 ;
real_T B_185_185_0 ; real_T B_185_189_0 ; real_T B_185_193_0 [ 2 ] ; real_T
B_185_195_0 [ 50 ] ; real_T B_185_196_0 ; real_T B_185_197_0 ; real_T
B_185_198_0 ; real_T B_185_202_0 ; real_T B_185_206_0 [ 2 ] ; real_T
B_185_207_0 [ 2 ] ; real_T B_185_209_0 [ 3 ] ; real_T B_185_213_0 [ 3 ] ;
real_T B_185_215_0 ; real_T B_185_224_0 ; real_T B_185_231_0 [ 3 ] ; real_T
B_185_232_0 ; real_T B_185_235_0 ; real_T B_185_237_0 ; real_T B_185_239_0 ;
real_T B_185_244_0 ; real_T B_185_251_0 ; real_T B_185_252_0 ; real_T
B_185_255_0 [ 3 ] ; real_T B_185_266_0 [ 2 ] ; real_T B_185_267_0 ; real_T
B_185_268_0 ; real_T B_185_269_0 ; real_T B_185_270_0 ; real_T B_185_271_0 ;
real_T B_185_272_0 ; real_T B_185_275_0 ; real_T B_185_277_0 ; real_T
B_185_279_0 ; real_T B_185_281_0 ; real_T B_185_282_0 ; real_T B_185_284_0 ;
real_T B_185_286_0 ; real_T B_185_287_0 ; real_T B_185_299_0 ; real_T
B_185_304_0 [ 6 ] ; real_T B_185_305_0 [ 6 ] ; real_T B_185_307_0 [ 50 ] ;
real_T B_185_309_0 ; real_T B_185_311_0 ; real_T B_185_313_0 [ 50 ] ; real_T
B_185_314_0 ; real_T B_185_318_0 ; real_T B_185_322_0 [ 2 ] ; real_T
B_185_324_0 [ 50 ] ; real_T B_185_325_0 ; real_T B_185_326_0 ; real_T
B_185_327_0 ; real_T B_185_331_0 ; real_T B_185_335_0 [ 2 ] ; real_T
B_185_337_0 [ 50 ] ; real_T B_185_338_0 ; real_T B_185_339_0 ; real_T
B_185_340_0 ; real_T B_185_344_0 ; real_T B_185_348_0 [ 2 ] ; real_T
B_185_350_0 [ 50 ] ; real_T B_185_351_0 ; real_T B_185_352_0 ; real_T
B_185_353_0 ; real_T B_185_357_0 ; real_T B_185_361_0 [ 2 ] ; real_T
B_185_363_0 [ 50 ] ; real_T B_185_364_0 ; real_T B_185_365_0 ; real_T
B_185_366_0 ; real_T B_185_370_0 ; real_T B_185_374_0 [ 2 ] ; real_T
B_185_376_0 [ 50 ] ; real_T B_185_377_0 ; real_T B_185_378_0 ; real_T
B_185_379_0 ; real_T B_185_383_0 ; real_T B_185_387_0 [ 2 ] ; real_T
B_185_388_0 [ 2 ] ; real_T B_185_390_0 [ 3 ] ; real_T B_185_394_0 [ 3 ] ;
real_T B_185_395_0 ; real_T B_185_397_0 ; real_T B_185_398_0 ; real_T
B_185_399_0 ; real_T B_185_407_0 [ 2 ] ; real_T B_185_408_0 ; real_T
B_185_415_0 [ 3 ] ; real_T B_185_418_0 ; real_T B_185_419_0 ; real_T
B_185_420_0 ; real_T B_185_421_0 ; real_T B_185_423_0 ; real_T B_185_425_0 ;
real_T B_185_427_0 ; real_T B_185_432_0 ; real_T B_185_439_0 ; real_T
B_185_440_0 ; real_T B_185_441_0 ; real_T B_185_444_0 ; real_T B_185_446_0 [
3 ] ; real_T B_185_449_0 [ 3 ] ; real_T B_185_457_0 [ 2 ] ; real_T
B_185_458_0 ; real_T B_185_460_0 ; real_T B_185_462_0 ; real_T B_185_467_0 ;
real_T B_185_474_0 ; real_T B_185_475_0 ; real_T B_185_477_0 ; real_T
B_185_479_0 ; real_T B_185_484_0 ; real_T B_185_487_0 ; real_T B_185_489_0 ;
real_T B_185_493_0 ; real_T B_185_494_0 ; real_T B_185_506_0 ; real_T
B_185_511_0 [ 6 ] ; real_T B_185_512_0 [ 6 ] ; real_T B_185_514_0 [ 50 ] ;
real_T B_185_515_0 ; real_T B_185_516_0 ; real_T B_185_517_0 ; real_T
B_185_521_0 ; real_T B_185_525_0 [ 2 ] ; real_T B_185_527_0 [ 50 ] ; real_T
B_185_528_0 ; real_T B_185_529_0 ; real_T B_185_530_0 ; real_T B_185_534_0 ;
real_T B_185_538_0 [ 2 ] ; real_T B_185_540_0 [ 50 ] ; real_T B_185_541_0 ;
real_T B_185_542_0 ; real_T B_185_543_0 ; real_T B_185_547_0 ; real_T
B_185_551_0 [ 2 ] ; real_T B_185_553_0 [ 50 ] ; real_T B_185_554_0 ; real_T
B_185_555_0 ; real_T B_185_556_0 ; real_T B_185_560_0 ; real_T B_185_564_0 [
2 ] ; real_T B_185_566_0 [ 50 ] ; real_T B_185_567_0 ; real_T B_185_568_0 ;
real_T B_185_569_0 ; real_T B_185_573_0 ; real_T B_185_577_0 [ 2 ] ; real_T
B_185_579_0 [ 50 ] ; real_T B_185_580_0 ; real_T B_185_581_0 ; real_T
B_185_582_0 ; real_T B_185_586_0 ; real_T B_185_590_0 [ 2 ] ; real_T
B_185_591_0 [ 2 ] ; real_T B_185_593_0 [ 3 ] ; real_T B_185_597_0 [ 3 ] ;
real_T B_185_598_0 ; real_T B_185_600_0 ; real_T B_185_601_0 ; real_T
B_185_602_0 ; real_T B_185_610_0 [ 2 ] ; real_T B_185_611_0 ; real_T
B_185_618_0 [ 3 ] ; real_T B_185_621_0 ; real_T B_185_622_0 ; real_T
B_185_623_0 ; real_T B_185_624_0 ; real_T B_185_626_0 ; real_T B_185_628_0 ;
real_T B_185_630_0 ; real_T B_185_635_0 ; real_T B_185_642_0 ; real_T
B_185_643_0 ; real_T B_185_644_0 ; real_T B_185_647_0 ; real_T B_185_649_0 [
3 ] ; real_T B_185_652_0 [ 3 ] ; real_T B_185_660_0 [ 2 ] ; real_T
B_185_661_0 ; real_T B_185_663_0 ; real_T B_185_665_0 ; real_T B_185_670_0 ;
real_T B_185_677_0 ; real_T B_185_678_0 ; real_T B_185_680_0 ; real_T
B_185_682_0 ; real_T B_185_687_0 ; real_T B_185_690_0 ; real_T B_185_692_0 ;
real_T B_185_696_0 ; real_T B_185_697_0 ; real_T B_185_709_0 ; real_T
B_185_714_0 [ 6 ] ; real_T B_185_715_0 [ 6 ] ; real_T B_185_717_0 [ 50 ] ;
real_T B_185_718_0 ; real_T B_185_719_0 ; real_T B_185_720_0 ; real_T
B_185_724_0 ; real_T B_185_728_0 [ 2 ] ; real_T B_185_730_0 [ 50 ] ; real_T
B_185_731_0 ; real_T B_185_732_0 ; real_T B_185_733_0 ; real_T B_185_737_0 ;
real_T B_185_741_0 [ 2 ] ; real_T B_185_743_0 [ 50 ] ; real_T B_185_744_0 ;
real_T B_185_745_0 ; real_T B_185_746_0 ; real_T B_185_750_0 ; real_T
B_185_754_0 [ 2 ] ; real_T B_185_756_0 [ 50 ] ; real_T B_185_757_0 ; real_T
B_185_758_0 ; real_T B_185_759_0 ; real_T B_185_763_0 ; real_T B_185_767_0 [
2 ] ; real_T B_185_769_0 [ 50 ] ; real_T B_185_770_0 ; real_T B_185_771_0 ;
real_T B_185_772_0 ; real_T B_185_776_0 ; real_T B_185_780_0 [ 2 ] ; real_T
B_185_782_0 [ 50 ] ; real_T B_185_783_0 ; real_T B_185_784_0 ; real_T
B_185_785_0 ; real_T B_185_789_0 ; real_T B_185_793_0 [ 2 ] ; real_T
B_185_794_0 ; real_T B_185_796_0 ; real_T B_185_799_0 ; real_T B_185_803_0 ;
real_T B_185_807_0 ; real_T B_185_808_0 [ 3 ] ; real_T B_185_815_0 [ 3 ] ;
real_T B_185_816_0 ; real_T B_185_818_0 ; real_T B_185_821_0 ; real_T
B_185_825_0 ; real_T B_185_829_0 ; real_T B_185_830_0 [ 3 ] ; real_T
B_185_837_0 [ 3 ] ; real_T B_185_838_0 ; real_T B_185_840_0 ; real_T
B_185_843_0 ; real_T B_185_847_0 ; real_T B_185_851_0 ; real_T B_185_852_0 [
3 ] ; real_T B_185_859_0 [ 3 ] ; real_T B_185_860_0 ; real_T B_185_862_0 ;
real_T B_185_865_0 ; real_T B_185_869_0 ; real_T B_185_873_0 ; real_T
B_185_874_0 [ 3 ] ; real_T B_185_881_0 [ 3 ] ; real_T B_185_882_0 ; real_T
B_185_884_0 ; real_T B_185_887_0 ; real_T B_185_891_0 ; real_T B_185_895_0 ;
real_T B_185_896_0 [ 3 ] ; real_T B_185_903_0 [ 3 ] ; real_T B_185_904_0 ;
real_T B_185_906_0 ; real_T B_185_909_0 ; real_T B_185_913_0 ; real_T
B_185_917_0 ; real_T B_185_918_0 [ 3 ] ; real_T B_185_925_0 [ 3 ] ; real_T
B_185_926_0 [ 241 ] ; real_T B_185_926_1 [ 95 ] ; real_T B_185_927_0 ; real_T
B_185_928_0 ; real_T B_185_929_0 ; real_T B_185_933_0 ; real_T B_185_934_0 ;
real_T B_185_935_0 ; real_T B_185_937_0 ; real_T B_185_938_0 ; real_T
B_185_939_0 ; real_T B_185_944_0 ; real_T B_185_945_0 ; real_T B_185_946_0 ;
real_T B_185_948_0 ; real_T B_185_949_0 ; real_T B_185_950_0 ; real_T
B_185_955_0 ; real_T B_185_956_0 ; real_T B_185_957_0 ; real_T B_185_959_0 ;
real_T B_185_960_0 ; real_T B_185_961_0 ; real_T B_185_973_0 ; real_T
B_185_974_0 ; real_T B_185_975_0 ; real_T B_185_979_0 ; real_T B_185_980_0 ;
real_T B_185_981_0 ; real_T B_185_983_0 ; real_T B_185_984_0 ; real_T
B_185_985_0 ; real_T B_185_990_0 ; real_T B_185_991_0 ; real_T B_185_992_0 ;
real_T B_185_994_0 ; real_T B_185_995_0 ; real_T B_185_996_0 ; real_T
B_185_1001_0 ; real_T B_185_1002_0 ; real_T B_185_1003_0 ; real_T
B_185_1005_0 ; real_T B_185_1006_0 ; real_T B_185_1007_0 ; real_T
B_185_1019_0 ; real_T B_185_1020_0 ; real_T B_185_1021_0 ; real_T
B_185_1025_0 ; real_T B_185_1026_0 ; real_T B_185_1027_0 ; real_T
B_185_1029_0 ; real_T B_185_1030_0 ; real_T B_185_1031_0 ; real_T
B_185_1036_0 ; real_T B_185_1037_0 ; real_T B_185_1038_0 ; real_T
B_185_1040_0 ; real_T B_185_1041_0 ; real_T B_185_1042_0 ; real_T
B_185_1047_0 ; real_T B_185_1048_0 ; real_T B_185_1049_0 ; real_T
B_185_1051_0 ; real_T B_185_1052_0 ; real_T B_185_1053_0 ; real_T
B_185_1065_0 ; real_T B_185_1066_0 ; real_T B_185_1067_0 ; real_T
B_185_1071_0 ; real_T B_185_1072_0 ; real_T B_185_1073_0 ; real_T
B_185_1075_0 ; real_T B_185_1076_0 ; real_T B_185_1077_0 ; real_T
B_185_1082_0 ; real_T B_185_1083_0 ; real_T B_185_1084_0 ; real_T
B_185_1086_0 ; real_T B_185_1087_0 ; real_T B_185_1088_0 ; real_T
B_185_1093_0 ; real_T B_185_1094_0 ; real_T B_185_1095_0 ; real_T
B_185_1097_0 ; real_T B_185_1098_0 ; real_T B_185_1099_0 ; real_T
B_185_1114_0 ; real_T B_185_1115_0 ; real_T B_185_1117_0 ; real_T
B_185_1118_0 ; real_T B_185_1121_0 ; real_T B_185_1129_0 [ 3 ] ; real_T
B_185_1131_0 [ 3 ] ; real_T B_185_1139_0 ; real_T B_185_1143_0 ; real_T
B_185_1150_0 ; real_T B_185_1151_0 ; real_T B_185_1168_0 ; real_T
B_185_1172_0 ; real_T B_185_1179_0 ; real_T B_185_1180_0 ; real_T
B_185_1197_0 ; real_T B_185_1201_0 ; real_T B_185_1208_0 ; real_T
B_185_1209_0 ; real_T B_185_1226_0 ; real_T B_185_1230_0 ; real_T
B_185_1237_0 ; real_T B_185_1238_0 ; real_T B_185_1255_0 ; real_T
B_185_1259_0 ; real_T B_185_1266_0 ; real_T B_185_1267_0 ; real_T
B_185_1284_0 ; real_T B_185_1288_0 ; real_T B_185_1295_0 ; real_T
B_185_1296_0 ; real_T B_185_1307_0 ; real_T B_185_1311_0 ; real_T
B_185_1315_0 ; real_T B_185_1319_0 ; real_T B_185_1323_0 ; real_T
B_185_1327_0 ; real_T B_185_1350_0 ; real_T B_185_1352_0 ; real_T
B_185_1354_0 ; real_T B_185_1359_0 ; real_T B_185_1362_0 ; real_T
B_185_1364_0 ; real_T B_185_1368_0 [ 3 ] ; real_T B_185_1369_0 ; real_T
B_185_1371_0 ; real_T B_185_1372_0 ; real_T B_185_1373_0 ; real_T
B_185_1374_0 ; real_T B_185_1375_0 ; real_T B_185_1377_0 ; real_T
B_185_1379_0 ; real_T B_185_1380_0 ; real_T B_185_1381_0 ; real_T
B_185_1382_0 ; real_T B_185_1383_0 ; real_T B_185_1384_0 ; real_T
B_185_1385_0 ; real_T B_185_1386_0 ; real_T B_185_1387_0 ; real_T
B_185_1388_0 ; real_T B_185_1389_0 ; real_T B_185_1390_0 ; real_T
B_185_1391_0 ; real_T B_185_1392_0 ; real_T B_185_1393_0 ; real_T
B_185_1394_0 ; real_T B_185_1395_0 ; real_T B_185_1396_0 ; real_T
B_185_1397_0 ; real_T B_185_1398_0 ; real_T B_185_1399_0 ; real_T
B_185_1400_0 ; real_T B_185_1405_0 ; real_T B_185_1406_0 ; real_T
B_185_1407_0 ; real_T B_185_1413_0 [ 3 ] ; real_T B_185_1414_0 [ 3 ] ; real_T
B_185_1417_0 [ 2 ] ; real_T B_185_1425_0 [ 3 ] ; real_T B_185_1427_0 [ 3 ] ;
real_T B_185_1434_0 ; real_T B_185_1438_0 ; real_T B_185_1445_0 ; real_T
B_185_1446_0 ; real_T B_185_1463_0 ; real_T B_185_1467_0 ; real_T
B_185_1474_0 ; real_T B_185_1475_0 ; real_T B_185_1492_0 ; real_T
B_185_1496_0 ; real_T B_185_1503_0 ; real_T B_185_1504_0 ; real_T
B_185_1521_0 ; real_T B_185_1525_0 ; real_T B_185_1532_0 ; real_T
B_185_1533_0 ; real_T B_185_1550_0 ; real_T B_185_1554_0 ; real_T
B_185_1561_0 ; real_T B_185_1562_0 ; real_T B_185_1579_0 ; real_T
B_185_1583_0 ; real_T B_185_1590_0 ; real_T B_185_1591_0 ; real_T
B_185_1602_0 ; real_T B_185_1606_0 ; real_T B_185_1610_0 ; real_T
B_185_1614_0 ; real_T B_185_1618_0 ; real_T B_185_1622_0 ; real_T
B_185_1646_0 ; real_T B_185_1648_0 ; real_T B_185_1650_0 ; real_T
B_185_1655_0 ; real_T B_185_1658_0 ; real_T B_185_1660_0 ; real_T
B_185_1664_0 [ 3 ] ; real_T B_185_1665_0 ; real_T B_185_1667_0 ; real_T
B_185_1668_0 ; real_T B_185_1669_0 ; real_T B_185_1670_0 ; real_T
B_185_1671_0 ; real_T B_185_1673_0 ; real_T B_185_1675_0 ; real_T
B_185_1676_0 ; real_T B_185_1677_0 ; real_T B_185_1678_0 ; real_T
B_185_1679_0 ; real_T B_185_1680_0 ; real_T B_185_1681_0 ; real_T
B_185_1682_0 ; real_T B_185_1683_0 ; real_T B_185_1684_0 ; real_T
B_185_1685_0 ; real_T B_185_1686_0 ; real_T B_185_1687_0 ; real_T
B_185_1688_0 ; real_T B_185_1689_0 ; real_T B_185_1690_0 ; real_T
B_185_1691_0 ; real_T B_185_1692_0 ; real_T B_185_1693_0 ; real_T
B_185_1694_0 ; real_T B_185_1695_0 ; real_T B_185_1696_0 ; real_T
B_185_1701_0 ; real_T B_185_1702_0 ; real_T B_185_1703_0 ; real_T
B_185_1709_0 [ 3 ] ; real_T B_185_1710_0 [ 3 ] ; real_T B_185_1713_0 [ 2 ] ;
real_T B_185_1721_0 [ 3 ] ; real_T B_185_1723_0 [ 3 ] ; real_T B_185_1730_0 ;
real_T B_185_1734_0 ; real_T B_185_1741_0 ; real_T B_185_1742_0 ; real_T
B_185_1759_0 ; real_T B_185_1763_0 ; real_T B_185_1770_0 ; real_T
B_185_1771_0 ; real_T B_185_1788_0 ; real_T B_185_1792_0 ; real_T
B_185_1799_0 ; real_T B_185_1800_0 ; real_T B_185_1817_0 ; real_T
B_185_1821_0 ; real_T B_185_1828_0 ; real_T B_185_1829_0 ; real_T
B_185_1846_0 ; real_T B_185_1850_0 ; real_T B_185_1857_0 ; real_T
B_185_1858_0 ; real_T B_185_1875_0 ; real_T B_185_1879_0 ; real_T
B_185_1886_0 ; real_T B_185_1887_0 ; real_T B_185_1898_0 ; real_T
B_185_1902_0 ; real_T B_185_1906_0 ; real_T B_185_1910_0 ; real_T
B_185_1914_0 ; real_T B_185_1918_0 ; real_T B_185_1942_0 ; real_T
B_185_1944_0 ; real_T B_185_1946_0 ; real_T B_185_1951_0 ; real_T
B_185_1954_0 ; real_T B_185_1956_0 ; real_T B_185_1960_0 [ 3 ] ; real_T
B_185_1961_0 ; real_T B_185_1963_0 ; real_T B_185_1964_0 ; real_T
B_185_1965_0 ; real_T B_185_1966_0 ; real_T B_185_1967_0 ; real_T
B_185_1969_0 ; real_T B_185_1971_0 ; real_T B_185_1972_0 ; real_T
B_185_1973_0 ; real_T B_185_1974_0 ; real_T B_185_1975_0 ; real_T
B_185_1976_0 ; real_T B_185_1977_0 ; real_T B_185_1978_0 ; real_T
B_185_1979_0 ; real_T B_185_1980_0 ; real_T B_185_1981_0 ; real_T
B_185_1982_0 ; real_T B_185_1983_0 ; real_T B_185_1984_0 ; real_T
B_185_1985_0 ; real_T B_185_1986_0 ; real_T B_185_1987_0 ; real_T
B_185_1988_0 ; real_T B_185_1989_0 ; real_T B_185_1990_0 ; real_T
B_185_1991_0 ; real_T B_185_1992_0 ; real_T B_185_1997_0 ; real_T
B_185_1998_0 ; real_T B_185_1999_0 ; real_T B_185_2005_0 [ 3 ] ; real_T
B_185_2006_0 [ 3 ] ; real_T B_185_2009_0 [ 2 ] ; real_T B_185_2017_0 [ 3 ] ;
real_T B_185_2019_0 [ 3 ] ; real_T B_185_2026_0 ; real_T B_185_2030_0 ;
real_T B_185_2037_0 ; real_T B_185_2038_0 ; real_T B_185_2055_0 ; real_T
B_185_2059_0 ; real_T B_185_2066_0 ; real_T B_185_2067_0 ; real_T
B_185_2084_0 ; real_T B_185_2088_0 ; real_T B_185_2095_0 ; real_T
B_185_2096_0 ; real_T B_185_2113_0 ; real_T B_185_2117_0 ; real_T
B_185_2124_0 ; real_T B_185_2125_0 ; real_T B_185_2142_0 ; real_T
B_185_2146_0 ; real_T B_185_2153_0 ; real_T B_185_2154_0 ; real_T
B_185_2171_0 ; real_T B_185_2175_0 ; real_T B_185_2182_0 ; real_T
B_185_2183_0 ; real_T B_185_2284_0 [ 3 ] ; real_T B_185_2287_0 ; real_T
B_185_2289_0 [ 3 ] ; real_T B_185_2291_0 [ 3 ] ; real_T B_185_2292_0 [ 3 ] ;
real_T B_185_2295_0 ; real_T B_185_2297_0 [ 3 ] ; real_T B_185_2299_0 [ 3 ] ;
real_T B_185_2302_0 [ 3 ] ; real_T B_185_2305_0 ; real_T B_185_2307_0 [ 3 ] ;
real_T B_185_2309_0 [ 3 ] ; real_T B_185_2310_0 [ 3 ] ; real_T B_185_2313_0 ;
real_T B_185_2315_0 [ 3 ] ; real_T B_185_2317_0 [ 3 ] ; real_T B_185_2328_0 ;
real_T B_185_2331_0 [ 3 ] ; real_T B_185_2333_0 [ 3 ] ; real_T B_185_2337_0 [
3 ] ; real_T B_185_2339_0 [ 3 ] ; real_T B_185_2341_0 [ 3 ] ; real_T
B_185_2342_0 [ 3 ] ; real_T B_185_2345_0 ; real_T B_185_2347_0 [ 3 ] ; real_T
B_185_2349_0 [ 3 ] ; real_T B_185_2350_0 [ 3 ] ; real_T B_185_2353_0 ; real_T
B_185_2355_0 [ 3 ] ; real_T B_185_2357_0 [ 3 ] ; real_T B_185_2360_0 [ 3 ] ;
real_T B_185_2363_0 ; real_T B_185_2365_0 [ 3 ] ; real_T B_185_2367_0 [ 3 ] ;
real_T B_185_2368_0 [ 3 ] ; real_T B_185_2371_0 ; real_T B_185_2373_0 [ 3 ] ;
real_T B_185_2375_0 [ 3 ] ; real_T B_185_2386_0 ; real_T B_185_2389_0 [ 3 ] ;
real_T B_185_2391_0 [ 3 ] ; real_T B_185_2395_0 [ 3 ] ; real_T B_185_2397_0 [
3 ] ; real_T B_185_2399_0 [ 3 ] ; real_T B_185_2400_0 [ 3 ] ; real_T
B_185_2403_0 ; real_T B_185_2405_0 [ 3 ] ; real_T B_185_2407_0 [ 3 ] ; real_T
B_185_2408_0 [ 3 ] ; real_T B_185_2411_0 ; real_T B_185_2413_0 [ 3 ] ; real_T
B_185_2415_0 [ 3 ] ; real_T B_185_2418_0 [ 3 ] ; real_T B_185_2421_0 ; real_T
B_185_2423_0 [ 3 ] ; real_T B_185_2425_0 [ 3 ] ; real_T B_185_2426_0 [ 3 ] ;
real_T B_185_2429_0 ; real_T B_185_2431_0 [ 3 ] ; real_T B_185_2433_0 [ 3 ] ;
real_T B_185_2444_0 ; real_T B_185_2447_0 [ 3 ] ; real_T B_185_2449_0 [ 3 ] ;
real_T B_185_2453_0 [ 3 ] ; real_T B_185_2455_0 [ 3 ] ; real_T B_185_2457_0 [
3 ] ; real_T B_185_2458_0 [ 3 ] ; real_T B_185_2461_0 ; real_T B_185_2463_0 [
3 ] ; real_T B_185_2465_0 [ 3 ] ; real_T B_185_2466_0 [ 3 ] ; real_T
B_185_2469_0 ; real_T B_185_2471_0 [ 3 ] ; real_T B_185_2473_0 [ 3 ] ; real_T
B_185_2476_0 [ 3 ] ; real_T B_185_2479_0 ; real_T B_185_2481_0 [ 3 ] ; real_T
B_185_2483_0 [ 3 ] ; real_T B_185_2484_0 [ 3 ] ; real_T B_185_2487_0 ; real_T
B_185_2489_0 [ 3 ] ; real_T B_185_2491_0 [ 3 ] ; real_T B_185_2502_0 ; real_T
B_185_2507_0 [ 3 ] ; real_T B_185_2509_0 [ 3 ] ; real_T B_185_2511_0 [ 3 ] ;
real_T B_185_2515_0 [ 3 ] ; real_T B_185_2517_0 [ 3 ] ; real_T B_185_2519_0 [
3 ] ; real_T B_185_2540_0 ; real_T B_185_2542_0 ; real_T B_185_2544_0 ;
real_T B_185_2546_0 ; real_T B_185_2548_0 ; real_T B_185_2550_0 ; real_T
B_185_2552_0 ; real_T B_185_2554_0 ; real_T B_185_2555_0 ; real_T
B_185_2560_0 [ 3 ] ; real_T B_185_2564_0 [ 3 ] ; real_T B_185_2574_0 [ 3 ] ;
real_T B_185_2577_0 ; real_T B_185_2579_0 [ 3 ] ; real_T B_185_2581_0 [ 3 ] ;
real_T B_185_2582_0 [ 3 ] ; real_T B_185_2585_0 ; real_T B_185_2587_0 [ 3 ] ;
real_T B_185_2589_0 [ 3 ] ; real_T B_185_2592_0 [ 3 ] ; real_T B_185_2595_0 ;
real_T B_185_2597_0 [ 3 ] ; real_T B_185_2599_0 [ 3 ] ; real_T B_185_2600_0 [
3 ] ; real_T B_185_2603_0 ; real_T B_185_2605_0 [ 3 ] ; real_T B_185_2607_0 [
3 ] ; real_T B_185_2618_0 ; real_T B_185_2620_0 ; real_T B_185_2623_0 [ 3 ] ;
real_T B_185_2625_0 [ 3 ] ; real_T B_185_2627_0 [ 3 ] ; real_T B_185_2629_0 [
3 ] ; real_T B_185_2631_0 ; real_T B_185_2632_0 ; real_T B_185_2633_0 ;
real_T B_185_2634_0 ; real_T B_185_2636_0 ; real_T B_185_2641_0 ; real_T
B_185_2645_0 ; real_T B_185_2646_0 ; real_T B_185_2647_0 ; real_T
B_185_2648_0 ; real_T B_185_2650_0 ; real_T B_185_2654_0 ; real_T
B_185_2656_0 ; real_T B_185_2657_0 ; real_T B_185_2658_0 ; real_T
B_185_2659_0 ; real_T B_185_2661_0 ; real_T B_185_2662_0 ; real_T
B_185_2663_0 ; real_T B_185_2664_0 ; real_T B_185_2666_0 ; real_T
B_185_2667_0 ; real_T B_185_2668_0 ; real_T B_185_2669_0 ; real_T
B_185_2671_0 ; real_T B_185_2672_0 ; real_T B_185_2673_0 ; real_T
B_185_2674_0 ; real_T B_185_2676_0 ; real_T B_185_2677_0 ; real_T
B_185_2678_0 ; real_T B_185_2679_0 ; real_T B_185_2681_0 ; real_T
B_185_2682_0 ; real_T B_185_2683_0 ; real_T B_185_2684_0 ; real_T
B_185_3180_0 ; real_T B_185_3181_0 ; real_T B_185_3183_0 ; real_T
B_185_3185_0 ; real_T B_185_3186_0 ; real_T B_185_3187_0 ; real_T
B_185_3189_0 ; real_T B_185_3190_0 ; real_T B_185_3191_0 ; real_T
B_185_3196_0 ; real_T B_185_3197_0 ; real_T B_185_3198_0 ; real_T
B_185_3200_0 ; real_T B_185_3201_0 ; real_T B_185_3202_0 ; real_T
B_185_3207_0 ; real_T B_185_3208_0 ; real_T B_185_3209_0 ; real_T
B_185_3211_0 ; real_T B_185_3212_0 ; real_T B_185_3213_0 ; real_T
B_185_3232_0 ; real_T B_185_3233_0 ; real_T B_185_3235_0 ; real_T
B_185_3237_0 ; real_T B_185_3238_0 ; real_T B_185_3239_0 ; real_T
B_185_3241_0 ; real_T B_185_3242_0 ; real_T B_185_3243_0 ; real_T
B_185_3248_0 ; real_T B_185_3249_0 ; real_T B_185_3250_0 ; real_T
B_185_3252_0 ; real_T B_185_3253_0 ; real_T B_185_3254_0 ; real_T
B_185_3259_0 ; real_T B_185_3260_0 ; real_T B_185_3261_0 ; real_T
B_185_3263_0 ; real_T B_185_3264_0 ; real_T B_185_3265_0 ; real_T
B_185_3285_0 ; real_T B_185_3286_0 ; real_T B_185_3288_0 ; real_T
B_185_3290_0 ; real_T B_185_3291_0 ; real_T B_185_3292_0 ; real_T
B_185_3294_0 ; real_T B_185_3295_0 ; real_T B_185_3296_0 ; real_T
B_185_3301_0 ; real_T B_185_3302_0 ; real_T B_185_3303_0 ; real_T
B_185_3305_0 ; real_T B_185_3306_0 ; real_T B_185_3307_0 ; real_T
B_185_3312_0 ; real_T B_185_3313_0 ; real_T B_185_3314_0 ; real_T
B_185_3316_0 ; real_T B_185_3317_0 ; real_T B_185_3318_0 ; real_T
B_185_3338_0 ; real_T B_185_3339_0 ; real_T B_185_3341_0 ; real_T
B_185_3343_0 ; real_T B_185_3344_0 ; real_T B_185_3345_0 ; real_T
B_185_3347_0 ; real_T B_185_3348_0 ; real_T B_185_3349_0 ; real_T
B_185_3354_0 ; real_T B_185_3355_0 ; real_T B_185_3356_0 ; real_T
B_185_3358_0 ; real_T B_185_3359_0 ; real_T B_185_3360_0 ; real_T
B_185_3365_0 ; real_T B_185_3366_0 ; real_T B_185_3367_0 ; real_T
B_185_3369_0 ; real_T B_185_3370_0 ; real_T B_185_3371_0 ; real_T
B_185_3391_0 ; real_T B_185_3393_0 ; real_T B_185_3395_0 ; real_T
B_185_3397_0 ; real_T B_185_3398_0 ; real_T B_185_3399_0 ; real_T
B_185_3401_0 ; real_T B_185_3402_0 ; real_T B_185_3403_0 ; real_T
B_185_3408_0 ; real_T B_185_3409_0 ; real_T B_185_3410_0 ; real_T
B_185_3412_0 ; real_T B_185_3413_0 ; real_T B_185_3414_0 ; real_T
B_185_3419_0 ; real_T B_185_3420_0 ; real_T B_185_3421_0 ; real_T
B_185_3423_0 ; real_T B_185_3424_0 ; real_T B_185_3425_0 ; real_T
B_185_3475_0 ; real_T B_111_0_0 ; real_T B_111_1_0 ; real_T B_77_0_0 ; real_T
B_77_1_0 ; real_T B_43_0_0 ; real_T B_43_1_0 ; real_T B_17_0_0 ; real_T
B_17_1_0 ; real_T B_17_3_0 [ 2 ] ; real_T B_17_7_0 ; real_T B_4_0_0 ; real_T
B_4_1_0 ; real_T B_4_2_0 ; real_T B_4_3_0 ; real_T B_4_6_0 [ 6 ] ; real_T
B_4_17_0 ; real_T B_4_26_0 ; real_T B_4_27_0 ; real_T B_4_29_0 ; real_T
B_4_30_0 ; real_T B_4_31_0 ; real_T B_4_32_0 ; real_T B_4_34_0 ; real_T
B_4_35_0 ; real_T B_4_36_0 ; real_T B_4_37_0 ; real_T B_4_39_0 ; real_T
B_4_40_0 ; real_T B_4_43_0 ; real_T B_4_44_0 ; real_T B_4_45_0 ; real_T
B_4_53_0 [ 2 ] ; real_T B_4_54_0 [ 3 ] ; real_T B_4_55_0 [ 3 ] ; real_T
B_4_56_0 [ 3 ] ; real_T B_4_57_0 [ 3 ] ; real_T B_185_122_0 [ 3 ] ; real_T
B_185_300_0 [ 3 ] ; real_T B_185_507_0 [ 3 ] ; real_T B_4_13_0 [ 3 ] ; real_T
B_185_710_0 [ 3 ] ; real_T B_185_2523_0 [ 3 ] ; uint8_T B_185_15_0 ; uint8_T
B_185_18_0 ; uint8_T B_185_66_0 ; uint8_T B_185_69_0 ; uint8_T B_185_113_0 ;
uint8_T B_185_116_0 ; uint8_T B_185_218_0 ; uint8_T B_185_221_0 ; uint8_T
B_185_261_0 ; uint8_T B_185_264_0 ; uint8_T B_185_291_0 ; uint8_T B_185_294_0
; uint8_T B_185_402_0 ; uint8_T B_185_405_0 ; uint8_T B_185_452_0 ; uint8_T
B_185_455_0 ; uint8_T B_185_498_0 ; uint8_T B_185_501_0 ; uint8_T B_185_605_0
; uint8_T B_185_608_0 ; uint8_T B_185_655_0 ; uint8_T B_185_658_0 ; uint8_T
B_185_701_0 ; uint8_T B_185_704_0 ; uint8_T B_185_1410_0 ; uint8_T
B_185_1412_0 ; uint8_T B_185_1706_0 ; uint8_T B_185_1708_0 ; uint8_T
B_185_2002_0 ; uint8_T B_185_2004_0 ; uint8_T B_4_20_0 ; uint8_T B_4_23_0 ;
uint8_T B_4_48_0 ; uint8_T B_4_51_0 ; boolean_T B_185_9_0 ; boolean_T
B_185_396_0 ; boolean_T B_185_599_0 ; boolean_T B_185_1376_0 ; boolean_T
B_185_1672_0 ; boolean_T B_185_1968_0 ; boolean_T B_185_2567_0 ; boolean_T
B_185_2569_0 ; char_T pad_B_185_2569_0 [ 6 ] ;
B_TrueRMS_normal_operation_no_faults_T TrueRMS ;
B_RMS_normal_operation_no_faults_T RMS ;
B_Signalgenerator_normal_operation_no_faults_T Signalgenerator_j ;
B_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_ca ;
B_Signalgenerator_normal_operation_no_faults_T Signalgenerator_m ;
B_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_a ;
B_Signalgenerator_normal_operation_no_faults_T Signalgenerator_e ;
B_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_b ;
B_Signalgenerator_normal_operation_no_faults_T Signalgenerator_p ;
B_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_c ;
B_Signalgenerator_normal_operation_no_faults_T Signalgenerator_d ;
B_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_d ;
B_Signalgenerator_normal_operation_no_faults_T Signalgenerator ;
B_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator ;
B_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_fb ; B_Subsystem1_normal_operation_no_faults_p_T
Subsystem1_p ; B_Subsystempi2delay_normal_operation_no_faults_e_T
Subsystempi2delay_en ; B_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl_a ; B_Subsystem1_normal_operation_no_faults_p_T
Subsystem1_l ; B_Subsystempi2delay_normal_operation_no_faults_e_T
Subsystempi2delay_j ; B_Subsystem1_normal_operation_no_faults_p_T
Subsystem1_jf ; B_Subsystempi2delay_normal_operation_no_faults_e_T
Subsystempi2delay_b3 ; B_Subsystem1_normal_operation_no_faults_o_T
Subsystem1_e ; B_Subsystempi2delay_normal_operation_no_faults_b_T
Subsystempi2delay_e ;
B_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_k ;
B_ACVoltageController_normal_operation_no_faults_T ACVoltageController_l ;
B_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_f ; B_Subsystem1_normal_operation_no_faults_p_T
Subsystem1_h1 ; B_Subsystempi2delay_normal_operation_no_faults_e_T
Subsystempi2delay_h4 ; B_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl_d ; B_Subsystem1_normal_operation_no_faults_p_T
Subsystem1_j ; B_Subsystempi2delay_normal_operation_no_faults_e_T
Subsystempi2delay_nu ; B_Subsystem1_normal_operation_no_faults_p_T
Subsystem1_c ; B_Subsystempi2delay_normal_operation_no_faults_e_T
Subsystempi2delay_n ; B_Subsystem1_normal_operation_no_faults_o_T
Subsystem1_g ; B_Subsystempi2delay_normal_operation_no_faults_b_T
Subsystempi2delay_h ;
B_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_d ;
B_ACVoltageController_normal_operation_no_faults_T ACVoltageController_n ;
B_ZeroSequenceController_normal_operation_no_faults_T ZeroSequenceController2
; B_Subsystem1_normal_operation_no_faults_p_T Subsystem1_f ;
B_Subsystempi2delay_normal_operation_no_faults_e_T Subsystempi2delay_i ;
B_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl ;
B_Subsystem1_normal_operation_no_faults_p_T Subsystem1_b ;
B_Subsystempi2delay_normal_operation_no_faults_e_T Subsystempi2delay_m1 ;
B_Subsystem1_normal_operation_no_faults_p_T Subsystem1_h ;
B_Subsystempi2delay_normal_operation_no_faults_e_T Subsystempi2delay_m ;
B_Subsystem1_normal_operation_no_faults_o_T Subsystem1_i3 ;
B_Subsystempi2delay_normal_operation_no_faults_b_T Subsystempi2delay_pq ;
B_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression ;
B_ACVoltageController_normal_operation_no_faults_T ACVoltageController ;
B_ZeroSequenceController_normal_operation_no_faults_T ZeroSequenceController
; B_Subsystem1_normal_operation_no_faults_T Subsystem1_i ;
B_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay_b ;
B_Subsystem1_normal_operation_no_faults_T Subsystem1_d ;
B_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay_p ;
B_Subsystem1_normal_operation_no_faults_o_T Subsystem1 ;
B_Subsystempi2delay_normal_operation_no_faults_b_T Subsystempi2delay ;
B_Subsystem1_normal_operation_no_faults_l_T Subsystem1_go ;
B_Subsystempi2delay_normal_operation_no_faults_p_T Subsystempi2delay_g ;
B_Subsystem1_normal_operation_no_faults_T Subsystem1_pi ;
B_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay_bk ; }
B_normal_operation_no_faults_T ; typedef struct { real_T UnitDelay5_DSTATE [
3 ] ; real_T Delay_x1_DSTATE [ 3 ] ; real_T Delay_x2_DSTATE [ 3 ] ; real_T
UnitDelay1_DSTATE ; real_T DiscreteTimeIntegrator1_DSTATE ; real_T
UnitDelay6_DSTATE [ 3 ] ; real_T DiscreteTimeIntegrator_DSTATE ; real_T
UnitDelay4_DSTATE ; real_T UnitDelay3_DSTATE ; real_T UnitDelay4_DSTATE_i ;
real_T UnitDelay3_DSTATE_f ; real_T UnitDelay4_DSTATE_e ; real_T
UnitDelay3_DSTATE_a ; real_T UnitDelay4_DSTATE_p ; real_T UnitDelay3_DSTATE_d
; real_T UnitDelay4_DSTATE_m ; real_T UnitDelay3_DSTATE_p ; real_T
UnitDelay4_DSTATE_c ; real_T UnitDelay3_DSTATE_de ; real_T
UnitDelay5_DSTATE_m [ 3 ] ; real_T UnitDelay1_DSTATE_b ; real_T
Delay_x1_DSTATE_i [ 3 ] ; real_T Delay_x2_DSTATE_f [ 3 ] ; real_T
DiscreteTimeIntegrator1_DSTATE_o ; real_T UnitDelay6_DSTATE_l [ 3 ] ; real_T
UnitDelay4_DSTATE_o ; real_T UnitDelay3_DSTATE_m ; real_T UnitDelay4_DSTATE_f
; real_T UnitDelay3_DSTATE_ff ; real_T UnitDelay4_DSTATE_ig ; real_T
UnitDelay3_DSTATE_o ; real_T UnitDelay4_DSTATE_ma ; real_T
UnitDelay3_DSTATE_pb ; real_T UnitDelay4_DSTATE_l ; real_T
UnitDelay3_DSTATE_d4 ; real_T UnitDelay4_DSTATE_c5 ; real_T
UnitDelay3_DSTATE_b ; real_T UnitDelay5_DSTATE_h [ 3 ] ; real_T
Delay_x1_DSTATE_h [ 3 ] ; real_T Delay_x2_DSTATE_m [ 3 ] ; real_T
UnitDelay1_DSTATE_a ; real_T DiscreteTimeIntegrator1_DSTATE_h ; real_T
UnitDelay6_DSTATE_h [ 3 ] ; real_T DiscreteTimeIntegrator_DSTATE_i ; real_T
UnitDelay4_DSTATE_n ; real_T UnitDelay3_DSTATE_od ; real_T
UnitDelay4_DSTATE_lc ; real_T UnitDelay3_DSTATE_o5 ; real_T
UnitDelay4_DSTATE_b ; real_T UnitDelay3_DSTATE_pu ; real_T
UnitDelay4_DSTATE_k ; real_T UnitDelay3_DSTATE_oh ; real_T
UnitDelay4_DSTATE_d ; real_T UnitDelay3_DSTATE_e ; real_T
UnitDelay4_DSTATE_cx ; real_T UnitDelay3_DSTATE_mu ; real_T
UnitDelay5_DSTATE_o [ 3 ] ; real_T Delay_x1_DSTATE_o [ 3 ] ; real_T
Delay_x2_DSTATE_n [ 3 ] ; real_T UnitDelay1_DSTATE_i ; real_T
DiscreteTimeIntegrator1_DSTATE_a ; real_T UnitDelay6_DSTATE_i [ 3 ] ; real_T
DiscreteTimeIntegrator_DSTATE_j ; real_T UnitDelay4_DSTATE_j ; real_T
UnitDelay3_DSTATE_bw ; real_T UnitDelay4_DSTATE_df ; real_T
UnitDelay3_DSTATE_ey ; real_T UnitDelay4_DSTATE_mc ; real_T
UnitDelay3_DSTATE_oa ; real_T UnitDelay4_DSTATE_h ; real_T
UnitDelay3_DSTATE_i ; real_T UnitDelay4_DSTATE_n3 ; real_T
UnitDelay3_DSTATE_n ; real_T UnitDelay4_DSTATE_ix ; real_T
UnitDelay3_DSTATE_ds ; real_T StateSpace_DSTATE [ 244 ] ; real_T
UnitDelay1_DSTATE_h [ 2 ] ; real_T DiscreteTimeIntegrator_DSTATE_p ; real_T
UnitDelay1_DSTATE_b2 [ 2 ] ; real_T DiscreteTimeIntegrator_DSTATE_b ; real_T
UnitDelay1_DSTATE_iy [ 2 ] ; real_T DiscreteTimeIntegrator_DSTATE_d ; real_T
UnitDelay1_DSTATE_f [ 2 ] ; real_T DiscreteTimeIntegrator_DSTATE_a ; real_T
UnitDelay1_DSTATE_p [ 2 ] ; real_T DiscreteTimeIntegrator_DSTATE_jm ; real_T
UnitDelay1_DSTATE_i1 [ 2 ] ; real_T DiscreteTimeIntegrator_DSTATE_c ; real_T
Delay_x1_DSTATE_a ; real_T Delay_x2_DSTATE_b ; real_T UnitDelay1_DSTATE_e [ 2
] ; real_T DiscreteTimeIntegrator_DSTATE_j1 ; real_T UnitDelay1_DSTATE_ef [ 2
] ; real_T DiscreteTimeIntegrator_DSTATE_h ; real_T UnitDelay1_DSTATE_hm [ 2
] ; real_T DiscreteTimeIntegrator_DSTATE_b5 ; real_T UnitDelay1_DSTATE_j [ 2
] ; real_T DiscreteTimeIntegrator_DSTATE_pu ; real_T UnitDelay1_DSTATE_he [ 2
] ; real_T DiscreteTimeIntegrator_DSTATE_pe ; real_T UnitDelay1_DSTATE_b5 [ 2
] ; real_T DiscreteTimeIntegrator_DSTATE_e ; real_T Delay_x1_DSTATE_c ;
real_T Delay_x2_DSTATE_mh ; real_T UnitDelay1_DSTATE_l [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_if ; real_T UnitDelay1_DSTATE_fb [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_l ; real_T UnitDelay1_DSTATE_d [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_m ; real_T UnitDelay1_DSTATE_n [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_g ; real_T UnitDelay1_DSTATE_hg [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_k ; real_T UnitDelay1_DSTATE_nq [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_jd ; real_T Delay_x1_DSTATE_b ; real_T
Delay_x2_DSTATE_e ; real_T UnitDelay1_DSTATE_f2 [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_mx ; real_T UnitDelay1_DSTATE_d1 [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_m3 ; real_T UnitDelay1_DSTATE_p2 [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_pv ; real_T UnitDelay1_DSTATE_l4 [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_c2 ; real_T UnitDelay1_DSTATE_m [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_er ; real_T UnitDelay1_DSTATE_c [ 2 ] ; real_T
DiscreteTimeIntegrator_DSTATE_j2 ; real_T Integ4_DSTATE ; real_T
UnitDelay_DSTATE ; real_T Integ4_DSTATE_i ; real_T UnitDelay_DSTATE_f ;
real_T Integ4_DSTATE_m ; real_T Integ4_DSTATE_k ; real_T Integ4_DSTATE_k4 ;
real_T Integ4_DSTATE_h ; real_T Integ4_DSTATE_n ; real_T Integ4_DSTATE_ho ;
real_T UnitDelay6_DSTATE_hi [ 2 ] ; real_T UnitDelay9_DSTATE [ 6 ] ; real_T
UnitDelay1_DSTATE_em ; real_T Initial_FirstOutputTime ; real_T
Initial_FirstOutputTime_a ; real_T Initial_FirstOutputTime_g ; real_T
TimeStampA ; real_T LastUAtTimeA ; real_T TimeStampB ; real_T LastUAtTimeB ;
real_T TimeStampA_f ; real_T LastUAtTimeA_o ; real_T TimeStampB_a ; real_T
LastUAtTimeB_o ; real_T TimeStampA_o ; real_T LastUAtTimeA_g ; real_T
TimeStampB_m ; real_T LastUAtTimeB_p ; real_T TimeStampA_n ; real_T
LastUAtTimeA_d ; real_T TimeStampB_o ; real_T LastUAtTimeB_k ; real_T
TimeStampA_g ; real_T LastUAtTimeA_a ; real_T TimeStampB_k ; real_T
LastUAtTimeB_g ; real_T TimeStampA_j ; real_T LastUAtTimeA_k ; real_T
TimeStampB_f ; real_T LastUAtTimeB_i ; real_T TimeStampA_i ; real_T
LastUAtTimeA_a1 ; real_T TimeStampB_c ; real_T LastUAtTimeB_e ; real_T
TimeStampA_p ; real_T LastUAtTimeA_n ; real_T TimeStampB_l ; real_T
LastUAtTimeB_h ; real_T TimeStampA_gh ; real_T LastUAtTimeA_n1 ; real_T
TimeStampB_l0 ; real_T LastUAtTimeB_g2 ; real_T TimeStampA_ii ; real_T
LastUAtTimeA_e ; real_T TimeStampB_fs ; real_T LastUAtTimeB_eq ; real_T
TimeStampA_oi ; real_T LastUAtTimeA_h ; real_T TimeStampB_cm ; real_T
LastUAtTimeB_j ; real_T TimeStampA_c ; real_T LastUAtTimeA_b ; real_T
TimeStampB_j ; real_T LastUAtTimeB_p1 ; real_T Memory_PreviousInput ; real_T
Memory_PreviousInput_f ; real_T PrevYA ; real_T PrevYB ; real_T
LastMajorTimeA ; real_T LastMajorTimeB ; real_T Memory_PreviousInput_f1 ;
real_T Memory_PreviousInput_o ; real_T PrevYA_p ; real_T PrevYB_b ; real_T
LastMajorTimeA_n ; real_T LastMajorTimeB_n ; real_T Memory_PreviousInput_c ;
real_T Memory_PreviousInput_d ; real_T PrevYA_a ; real_T PrevYB_h ; real_T
LastMajorTimeA_e ; real_T LastMajorTimeB_i ; real_T Memory_PreviousInput_b [
3 ] ; real_T Memory_PreviousInput_br [ 3 ] ; real_T Memory_PreviousInput_e [
3 ] ; real_T Memory_PreviousInput_j [ 3 ] ; real_T Memory_PreviousInput_k [ 3
] ; real_T Memory_PreviousInput_a [ 3 ] ; real_T Memory_PreviousInput_cv [ 3
] ; real_T Memory_PreviousInput_p [ 3 ] ; real_T Memory_PreviousInput_p1 [ 3
] ; real_T Memory_PreviousInput_g [ 3 ] ; real_T Memory_PreviousInput_dz [ 3
] ; real_T Memory_PreviousInput_fl [ 3 ] ; real_T Memory_PreviousInput_gn [ 3
] ; real_T Memory_PreviousInput_e2 [ 3 ] ; real_T Memory_PreviousInput_n [ 3
] ; real_T Memory_PreviousInput_i [ 3 ] ; real_T Memory_PreviousInput_bk [ 3
] ; real_T Memory_PreviousInput_ab [ 3 ] ; real_T Memory_PreviousInput_jf [ 3
] ; real_T Memory_PreviousInput_og [ 3 ] ; real_T TimeStampA_k ; real_T
LastUAtTimeA_dx ; real_T TimeStampB_o2 ; real_T LastUAtTimeB_b ; real_T
TimeStampA_fd ; real_T LastUAtTimeA_m ; real_T TimeStampB_aw ; real_T
LastUAtTimeB_e0 ; real_T TimeStampA_kq ; real_T LastUAtTimeA_nq ; real_T
TimeStampB_n ; real_T LastUAtTimeB_pd ; real_T TimeStampA_k4 ; real_T
LastUAtTimeA_gx ; real_T TimeStampB_a3 ; real_T LastUAtTimeB_ea ; real_T
TimeStampA_jj ; real_T LastUAtTimeA_ab ; real_T TimeStampB_la ; real_T
LastUAtTimeB_n ; real_T TimeStampA_l ; real_T LastUAtTimeA_g0 ; real_T
TimeStampB_jy ; real_T LastUAtTimeB_l ; real_T TimeStampA_jo ; real_T
LastUAtTimeA_br ; real_T TimeStampB_fi ; real_T LastUAtTimeB_ei ; real_T
TimeStampA_a ; real_T LastUAtTimeA_g1 ; real_T TimeStampB_b ; real_T
LastUAtTimeB_m ; real_T TimeStampA_oa ; real_T LastUAtTimeA_j ; real_T
TimeStampB_g ; real_T LastUAtTimeB_gx ; real_T TimeStampA_h ; real_T
LastUAtTimeA_gxl ; real_T TimeStampB_jo ; real_T LastUAtTimeB_pb ; real_T
TimeStampA_b ; real_T LastUAtTimeA_ea ; real_T TimeStampB_e ; real_T
LastUAtTimeB_a ; real_T TimeStampA_ns ; real_T LastUAtTimeA_l ; real_T
TimeStampB_ce ; real_T LastUAtTimeB_eq4 ; real_T TimeStampA_kp ; real_T
LastUAtTimeA_i ; real_T TimeStampB_d ; real_T LastUAtTimeB_o1 ; real_T
TimeStampA_j5 ; real_T LastUAtTimeA_gw ; real_T TimeStampB_av ; real_T
LastUAtTimeB_ol ; real_T TimeStampA_pp ; real_T LastUAtTimeA_hk ; real_T
TimeStampB_ec ; real_T LastUAtTimeB_ej ; struct { real_T modelTStart ; }
VariableTransportDelay_RWORK ; struct { real_T modelTStart ; }
VariableTransportDelay_RWORK_c ; struct { real_T modelTStart ; }
VariableTransportDelay_RWORK_a ; struct { real_T modelTStart ; }
TransportDelay_RWORK ; struct { real_T modelTStart ; } TransportDelay_RWORK_a
; struct { real_T modelTStart ; } TransportDelay_RWORK_m ; struct { real_T
modelTStart ; } TransportDelay_RWORK_m0 ; struct { real_T modelTStart ; }
TransportDelay_RWORK_l ; struct { real_T modelTStart ; }
TransportDelay_RWORK_lb ; struct { real_T modelTStart ; }
TransportDelay_RWORK_ai ; struct { real_T modelTStart ; }
TransportDelay_RWORK_e ; struct { real_T modelTStart ; }
TransportDelay_RWORK_mw ; struct { real_T modelTStart ; }
TransportDelay_RWORK_mn ; struct { real_T modelTStart ; }
TransportDelay_RWORK_h ; struct { real_T modelTStart ; }
TransportDelay_RWORK_o ; struct { real_T modelTStart ; }
TransportDelay_RWORK_k ; struct { real_T modelTStart ; }
TransportDelay_RWORK_i ; struct { real_T modelTStart ; }
TransportDelay_RWORK_kc ; struct { real_T modelTStart ; }
TransportDelay_RWORK_f ; struct { real_T modelTStart ; }
TransportDelay_RWORK_n ; struct { real_T modelTStart ; }
TransportDelay_RWORK_he ; struct { real_T modelTStart ; }
TransportDelay_RWORK_p ; struct { real_T modelTStart ; }
TransportDelay_RWORK_hq ; struct { void * AS ; void * BS ; void * CS ; void *
DS ; void * DX_COL ; void * BD_COL ; void * TMP1 ; void * TMP2 ; void * XTMP
; void * SWITCH_STATUS ; void * SWITCH_STATUS_INIT ; void * SW_CHG ; void *
G_STATE ; void * USWLAST ; void * XKM12 ; void * XKP12 ; void * XLAST ; void
* ULAST ; void * IDX_SW_CHG ; void * Y_SWITCH ; void * SWITCH_TYPES ; void *
IDX_OUT_SW ; void * SWITCH_TOPO_SAVED_IDX ; void * SWITCH_MAP ; }
StateSpace_PWORK ; void * Scope_PWORK [ 2 ] ; void * Scope1_PWORK ; void *
Scope4_PWORK ; void * Scope_PWORK_p ; void * Scope1_PWORK_k ; void *
Scope4_PWORK_m ; struct { void * TUbufferPtrs [ 3 ] ; }
VariableTransportDelay_PWORK ; void * Scope1_PWORK_j ; void * Scope4_PWORK_p
; struct { void * TUbufferPtrs [ 3 ] ; } VariableTransportDelay_PWORK_f ;
void * Scope1_PWORK_h ; void * Scope4_PWORK_o ; struct { void * TUbufferPtrs
[ 3 ] ; } VariableTransportDelay_PWORK_a ; struct { void * TUbufferPtrs [ 6 ]
; } TransportDelay_PWORK ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_i ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_p ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_o ; void * ACPOWERATBUS1_PWORK ; struct { void *
TUbufferPtrs [ 6 ] ; } TransportDelay_PWORK_a ; struct { void * TUbufferPtrs
[ 6 ] ; } TransportDelay_PWORK_j ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_f ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_n ; void * ACPOWERATBUS2_PWORK ; struct { void *
TUbufferPtrs [ 6 ] ; } TransportDelay_PWORK_ju ; struct { void * TUbufferPtrs
[ 6 ] ; } TransportDelay_PWORK_g ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_iy ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_i5 ; void * ACPOWERATBUS3_PWORK ; struct { void *
TUbufferPtrs [ 6 ] ; } TransportDelay_PWORK_nm ; struct { void * TUbufferPtrs
[ 6 ] ; } TransportDelay_PWORK_h ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_pw ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_d ; void * ACPOWERATBUS1INV_PWORK ; void *
ACVOLTAGEATBUS1_PWORK [ 2 ] ; void * ACVOLTAGEATBUS2_PWORK [ 2 ] ; void *
ACVOLTAGEATBUS3_PWORK [ 2 ] ; void * ACVOLTAGEATBUS4INV_PWORK [ 2 ] ; void *
DCCURRENTATBUS1_PWORK ; void * DCCURRENTATBUS2_PWORK ; void *
DCCURRENTATBUS3_PWORK ; void * DCCURRENTATBUS4_PWORK ; void *
DCPOWERANDDCVOLTAGEATBUS1_PWORK [ 2 ] ; void *
DCPOWERANDDCVOLTAGEATBUS2_PWORK [ 2 ] ; void *
DCPOWERANDDCVOLTAGEATBUS3_PWORK [ 2 ] ; void *
DCPOWERANDDCVOLTAGEATBUS4_PWORK [ 2 ] ; void * NominalLoadMeasurements_PWORK
[ 2 ] ; void * Scope8_PWORK ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_k ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_hv ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_e ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_dm ; void * Scope9_PWORK [ 2 ] ; struct { void *
uBuffers ; } SFunction_PWORK ; struct { void * AQHandles ; void * SlioLTF ; }
HiddenToAsyncQueue_InsertedFor_DiscreteMeanvalue5_at_outport_0_PWORK ; struct
{ void * uBuffers ; } SFunction_PWORK_b ; struct { void * uBuffers ; }
SFunction_PWORK_i ; struct { void * uBuffers ; } SFunction_PWORK_a ; struct {
void * uBuffers ; } SFunction_PWORK_h ; struct { void * uBuffers ; }
SFunction_PWORK_f ; struct { void * uBuffers ; } SFunction_PWORK_g ; struct {
void * uBuffers ; } SFunction_PWORK_e ; void *
DCPowerTransferInstantaneous_PWORK ; void *
DCPowerTransferInstantaneous1_PWORK ; void *
DCPowerTransferInstantaneous2_PWORK ; void *
DCPowerTransferInstantaneous3_PWORK ; void * Scope1_PWORK_b ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_k ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_f ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_h ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_h1 ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_h1b ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_d ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_c ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_o ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_p ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_l ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_e ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_n ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_lo ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_fv ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_ep ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_hl ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_hv ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_fk ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_b ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_a ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_m ; int32_T
TmpAtomicSubsysAtSwitchInport3_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch1Inport3_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch2Inport3_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch3Inport3_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch4Inport3_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch5Inport3_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_nh ; int32_T
TmpAtomicSubsysAtSwitch2Inport3_sysIdxToRun_a ; int32_T
TmpAtomicSubsysAtSwitchInport3_sysIdxToRun_m ; int32_T
TmpAtomicSubsysAtSwitch1Inport3_sysIdxToRun_e ; int32_T
TmpAtomicSubsysAtSwitch2Inport3_sysIdxToRun_ao ; int32_T
TmpAtomicSubsysAtSwitch3Inport3_sysIdxToRun_b ; int32_T
TmpAtomicSubsysAtSwitch4Inport3_sysIdxToRun_n ; int32_T
TmpAtomicSubsysAtSwitch5Inport3_sysIdxToRun_e ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_j ; int32_T
TmpAtomicSubsysAtSwitch2Inport3_sysIdxToRun_m ; int32_T
TmpAtomicSubsysAtSwitchInport3_sysIdxToRun_my ; int32_T
TmpAtomicSubsysAtSwitch1Inport3_sysIdxToRun_ei ; int32_T
TmpAtomicSubsysAtSwitch2Inport3_sysIdxToRun_aoc ; int32_T
TmpAtomicSubsysAtSwitch3Inport3_sysIdxToRun_bs ; int32_T
TmpAtomicSubsysAtSwitch4Inport3_sysIdxToRun_nf ; int32_T
TmpAtomicSubsysAtSwitch5Inport3_sysIdxToRun_eh ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_ne ; int32_T
TmpAtomicSubsysAtSwitch2Inport3_sysIdxToRun_d ; int32_T
ReactivepowerSetpoint_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport3_sysIdxToRun_myz ; int32_T
TmpAtomicSubsysAtSwitch1Inport3_sysIdxToRun_eil ; int32_T
TmpAtomicSubsysAtSwitch2Inport3_sysIdxToRun_aock ; int32_T
TmpAtomicSubsysAtSwitch3Inport3_sysIdxToRun_bs3 ; int32_T
TmpAtomicSubsysAtSwitch4Inport3_sysIdxToRun_nfh ; int32_T
TmpAtomicSubsysAtSwitch5Inport3_sysIdxToRun_ehg ; int32_T
Circulatingcurrentsuppression1_sysIdxToRun ; int_T Integrator_IWORK ; int_T
Integrator_IWORK_o ; int_T Integrator_IWORK_h ; int_T StateSpace_IWORK [ 11 ]
; struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ;
int_T MaxNewBufSize ; } VariableTransportDelay_IWORK ; struct { int_T Tail ;
int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
VariableTransportDelay_IWORK_g ; struct { int_T Tail ; int_T Head ; int_T
Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
VariableTransportDelay_IWORK_b ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ]
; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_p ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_i ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_m ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_a ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_e ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_l ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_ef ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ;
int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_as ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ;
int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_n ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_g ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_lh ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ;
int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_k ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_kd ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ;
int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_it ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ;
int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_c ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ; int_T
Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_mv ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ;
int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_a1 ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ;
int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_l0 ; struct { int_T Tail [ 3 ] ; int_T Head [ 3 ] ;
int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_b ; struct { int_T indBeg ; int_T indEnd ; int_T bufSz ;
int_T maxDiscrDelay ; } SFunction_IWORK ; struct { int_T indBeg ; int_T
indEnd ; int_T bufSz ; int_T maxDiscrDelay ; } SFunction_IWORK_f ; struct {
int_T indBeg ; int_T indEnd ; int_T bufSz ; int_T maxDiscrDelay ; }
SFunction_IWORK_i ; struct { int_T indBeg ; int_T indEnd ; int_T bufSz ;
int_T maxDiscrDelay ; } SFunction_IWORK_a ; struct { int_T indBeg ; int_T
indEnd ; int_T bufSz ; int_T maxDiscrDelay ; } SFunction_IWORK_d ; struct {
int_T indBeg ; int_T indEnd ; int_T bufSz ; int_T maxDiscrDelay ; }
SFunction_IWORK_aa ; struct { int_T indBeg ; int_T indEnd ; int_T bufSz ;
int_T maxDiscrDelay ; } SFunction_IWORK_ag ; struct { int_T indBeg ; int_T
indEnd ; int_T bufSz ; int_T maxDiscrDelay ; } SFunction_IWORK_c ; int_T
Integrator_MODE ; int_T Step_MODE [ 2 ] ; int_T
DCregulatorTURNONidreferenceforrectifier_MODE ; int_T
GridVoltagestabilizerTURNONiqreferenceforrectifier_MODE ; int_T
Saturation_MODE [ 3 ] ; int_T EnableCirculatingCurrentSuppresion_MODE ; int_T
EnableACRMSregulatorTURNONidreferenceforinverter_MODE ; int_T
EnableReactivepowersetpointregulationiqreferenceforinverter_MODE ; int_T
Saturation2_MODE [ 3 ] ; int_T Integrator_MODE_i ; int_T Step_MODE_n [ 2 ] ;
int_T Saturation_MODE_k [ 3 ] ; int_T Integrator_MODE_h ; int_T Step_MODE_a [
2 ] ; int_T Saturation_MODE_e [ 3 ] ; int_T Integrator_MODE_n ; int_T
Saturation2_MODE_f ; int_T Integrator_MODE_j ; int_T Saturation2_MODE_m ;
int_T Integrator_MODE_m ; int_T Saturation2_MODE_fe ; int_T
EnableHarmonicFilters_MODE ; int_T
EnergizeConverterswithstartupcurrentBypassesstartupresistors_MODE ; int_T
EnergizegridHarmonicFilter_MODE ; int_T Switchonloadnetwork_MODE ; int_T
Saturation_MODE_j [ 3 ] ; int8_T ReactivepowerSetpoint_SubsysRanBC ; int8_T
Circulatingcurrentsuppression1_SubsysRanBC ; uint8_T
DiscreteTimeIntegrator1_SYSTEM_ENABLE ; uint8_T
DiscreteTimeIntegrator1_NumInitCond ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE ; uint8_T
DiscreteTimeIntegrator_NumInitCond ; uint8_T
DiscreteTimeIntegrator1_SYSTEM_ENABLE_f ; uint8_T
DiscreteTimeIntegrator1_NumInitCond_m ; uint8_T
DiscreteTimeIntegrator1_SYSTEM_ENABLE_i ; uint8_T
DiscreteTimeIntegrator1_NumInitCond_f ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_e ; uint8_T
DiscreteTimeIntegrator_NumInitCond_i ; uint8_T
DiscreteTimeIntegrator1_SYSTEM_ENABLE_l ; uint8_T
DiscreteTimeIntegrator1_NumInitCond_o ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_c ; uint8_T
DiscreteTimeIntegrator_NumInitCond_k ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_h ; uint8_T
DiscreteTimeIntegrator_NumInitCond_h ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_g ; uint8_T
DiscreteTimeIntegrator_NumInitCond_f ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_i ; uint8_T
DiscreteTimeIntegrator_NumInitCond_j ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_j ; uint8_T
DiscreteTimeIntegrator_NumInitCond_g ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_g4 ; uint8_T
DiscreteTimeIntegrator_NumInitCond_j3 ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_m ; uint8_T
DiscreteTimeIntegrator_NumInitCond_d ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_hp ; uint8_T
DiscreteTimeIntegrator_NumInitCond_l ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_l ; uint8_T
DiscreteTimeIntegrator_NumInitCond_fb ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_lm ; uint8_T
DiscreteTimeIntegrator_NumInitCond_p ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_d ; uint8_T
DiscreteTimeIntegrator_NumInitCond_pn ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_o ; uint8_T
DiscreteTimeIntegrator_NumInitCond_o ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_a ; uint8_T
DiscreteTimeIntegrator_NumInitCond_hq ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_cg ; uint8_T
DiscreteTimeIntegrator_NumInitCond_n ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_f ; uint8_T
DiscreteTimeIntegrator_NumInitCond_a ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_ho ; uint8_T
DiscreteTimeIntegrator_NumInitCond_m ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_k ; uint8_T
DiscreteTimeIntegrator_NumInitCond_if ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_fr ; uint8_T
DiscreteTimeIntegrator_NumInitCond_e ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_lt ; uint8_T
DiscreteTimeIntegrator_NumInitCond_j1 ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_cp ; uint8_T
DiscreteTimeIntegrator_NumInitCond_kw ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_md ; uint8_T
DiscreteTimeIntegrator_NumInitCond_d2 ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_iy ; uint8_T
DiscreteTimeIntegrator_NumInitCond_oz ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_p ; uint8_T
DiscreteTimeIntegrator_NumInitCond_n1 ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_fi ; uint8_T
DiscreteTimeIntegrator_NumInitCond_av ; uint8_T
DiscreteTimeIntegrator_SYSTEM_ENABLE_hj ; uint8_T
DiscreteTimeIntegrator_NumInitCond_j5 ; uint8_T Integ4_SYSTEM_ENABLE ;
uint8_T Integ4_NumInitCond ; uint8_T Integ4_SYSTEM_ENABLE_e ; uint8_T
Integ4_NumInitCond_h ; uint8_T Integ4_SYSTEM_ENABLE_b ; uint8_T
Integ4_NumInitCond_f ; uint8_T Integ4_SYSTEM_ENABLE_p ; uint8_T
Integ4_NumInitCond_o ; uint8_T Integ4_SYSTEM_ENABLE_o ; uint8_T
Integ4_NumInitCond_n ; uint8_T Integ4_SYSTEM_ENABLE_b2 ; uint8_T
Integ4_NumInitCond_fw ; uint8_T Integ4_SYSTEM_ENABLE_m ; uint8_T
Integ4_NumInitCond_o4 ; uint8_T Integ4_SYSTEM_ENABLE_f ; uint8_T
Integ4_NumInitCond_k ; boolean_T RelationalOperator_Mode ; boolean_T
RelationalOperator_Mode_g ; boolean_T RelationalOperator_Mode_e ; boolean_T
RelationalOperator_Mode_n ; boolean_T RelationalOperator_Mode_f ; boolean_T
RelationalOperator_Mode_h ; boolean_T ReactivepowerSetpoint_MODE ; boolean_T
Circulatingcurrentsuppression1_MODE ; DW_TrueRMS_normal_operation_no_faults_T
TrueRMS ; DW_RMS_normal_operation_no_faults_T RMS ;
DW_Signalgenerator_normal_operation_no_faults_T Signalgenerator_j ;
DW_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_ca ;
DW_Signalgenerator_normal_operation_no_faults_T Signalgenerator_m ;
DW_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_a ;
DW_Signalgenerator_normal_operation_no_faults_T Signalgenerator_e ;
DW_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_b ;
DW_Signalgenerator_normal_operation_no_faults_T Signalgenerator_p ;
DW_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_c ;
DW_Signalgenerator_normal_operation_no_faults_T Signalgenerator_d ;
DW_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_d ;
DW_Signalgenerator_normal_operation_no_faults_T Signalgenerator ;
DW_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator ;
DW_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_fb ; DW_Subsystem1_normal_operation_no_faults_d_T
Subsystem1_p ; DW_Subsystempi2delay_normal_operation_no_faults_c_T
Subsystempi2delay_en ; DW_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl_a ; DW_Subsystem1_normal_operation_no_faults_d_T
Subsystem1_l ; DW_Subsystempi2delay_normal_operation_no_faults_c_T
Subsystempi2delay_j ; DW_Subsystem1_normal_operation_no_faults_d_T
Subsystem1_jf ; DW_Subsystempi2delay_normal_operation_no_faults_c_T
Subsystempi2delay_b3 ; DW_Subsystem1_normal_operation_no_faults_p_T
Subsystem1_e ; DW_Subsystempi2delay_normal_operation_no_faults_k_T
Subsystempi2delay_e ;
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_k ;
DW_ACVoltageController_normal_operation_no_faults_T ACVoltageController_l ;
DW_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_f ; DW_Subsystem1_normal_operation_no_faults_d_T
Subsystem1_h1 ; DW_Subsystempi2delay_normal_operation_no_faults_c_T
Subsystempi2delay_h4 ; DW_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl_d ; DW_Subsystem1_normal_operation_no_faults_d_T
Subsystem1_j ; DW_Subsystempi2delay_normal_operation_no_faults_c_T
Subsystempi2delay_nu ; DW_Subsystem1_normal_operation_no_faults_d_T
Subsystem1_c ; DW_Subsystempi2delay_normal_operation_no_faults_c_T
Subsystempi2delay_n ; DW_Subsystem1_normal_operation_no_faults_p_T
Subsystem1_g ; DW_Subsystempi2delay_normal_operation_no_faults_k_T
Subsystempi2delay_h ;
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_d ;
DW_ACVoltageController_normal_operation_no_faults_T ACVoltageController_n ;
DW_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2 ; DW_Subsystem1_normal_operation_no_faults_d_T
Subsystem1_f ; DW_Subsystempi2delay_normal_operation_no_faults_c_T
Subsystempi2delay_i ; DW_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl ; DW_Subsystem1_normal_operation_no_faults_d_T
Subsystem1_b ; DW_Subsystempi2delay_normal_operation_no_faults_c_T
Subsystempi2delay_m1 ; DW_Subsystem1_normal_operation_no_faults_d_T
Subsystem1_h ; DW_Subsystempi2delay_normal_operation_no_faults_c_T
Subsystempi2delay_m ; DW_Subsystem1_normal_operation_no_faults_p_T
Subsystem1_i3 ; DW_Subsystempi2delay_normal_operation_no_faults_k_T
Subsystempi2delay_pq ;
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression ;
DW_ACVoltageController_normal_operation_no_faults_T ACVoltageController ;
DW_ZeroSequenceController_normal_operation_no_faults_T ZeroSequenceController
; DW_Subsystem1_normal_operation_no_faults_T Subsystem1_i ;
DW_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay_b ;
DW_Subsystem1_normal_operation_no_faults_T Subsystem1_d ;
DW_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay_p ;
DW_Subsystem1_normal_operation_no_faults_p_T Subsystem1 ;
DW_Subsystempi2delay_normal_operation_no_faults_k_T Subsystempi2delay ;
DW_Subsystem1_normal_operation_no_faults_a_T Subsystem1_go ;
DW_Subsystempi2delay_normal_operation_no_faults_i_T Subsystempi2delay_g ;
DW_Subsystem1_normal_operation_no_faults_T Subsystem1_pi ;
DW_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay_bk ; }
DW_normal_operation_no_faults_T ; typedef struct { real_T Integrator_CSTATE ;
real_T Integrator_CSTATE_k ; real_T Integrator_CSTATE_j ; real_T
Integrator_CSTATE_n ; real_T Integrator_CSTATE_l ; real_T Integrator_CSTATE_o
; real_T Integrator_CSTATE_b ; real_T Integrator_CSTATE_m ; real_T
Integrator_CSTATE_ja ; real_T VariableTransportDelay_CSTATE ; real_T
integrator_CSTATE ; real_T TransferFcn_CSTATE ; real_T Integrator_x1_CSTATE ;
real_T Integrator_x2_CSTATE ; real_T Integrator_CSTATE_lt ; real_T
VariableTransportDelay_CSTATE_i ; real_T integrator_CSTATE_m ; real_T
TransferFcn_CSTATE_a ; real_T Integrator_x1_CSTATE_n ; real_T
Integrator_x2_CSTATE_c ; real_T Integrator_CSTATE_bf ; real_T
VariableTransportDelay_CSTATE_o ; real_T integrator_CSTATE_b ; real_T
TransferFcn_CSTATE_l ; real_T Integrator_x1_CSTATE_o ; real_T
Integrator_x2_CSTATE_e ; real_T integrator_CSTATE_b5 [ 3 ] ; real_T
integrator_CSTATE_l [ 3 ] ; real_T integrator_CSTATE_n [ 3 ] ; real_T
integrator_CSTATE_i [ 3 ] ; real_T integrator_CSTATE_k [ 3 ] ; real_T
integrator_CSTATE_lw [ 3 ] ; real_T integrator_CSTATE_d [ 3 ] ; real_T
integrator_CSTATE_mn [ 3 ] ; real_T integrator_CSTATE_p [ 3 ] ; real_T
integrator_CSTATE_a [ 3 ] ; real_T integrator_CSTATE_j [ 3 ] ; real_T
integrator_CSTATE_jl [ 3 ] ; real_T integrator_CSTATE_nm [ 3 ] ; real_T
integrator_CSTATE_c [ 3 ] ; real_T integrator_CSTATE_e [ 3 ] ; real_T
integrator_CSTATE_f [ 3 ] ; real_T integrator_CSTATE_kd [ 3 ] ; real_T
integrator_CSTATE_c1 [ 3 ] ; real_T integrator_CSTATE_js [ 3 ] ; real_T
integrator_CSTATE_po [ 3 ] ; X_TrueRMS_normal_operation_no_faults_T TrueRMS ;
X_RMS_normal_operation_no_faults_T RMS ;
X_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_fb ;
X_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl_a ;
X_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_k ;
X_ACVoltageController_normal_operation_no_faults_T ACVoltageController_l ;
X_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_f ;
X_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl_d ;
X_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_d ;
X_ACVoltageController_normal_operation_no_faults_T ACVoltageController_n ;
X_ZeroSequenceController_normal_operation_no_faults_T ZeroSequenceController2
; X_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl ;
X_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression ;
X_ACVoltageController_normal_operation_no_faults_T ACVoltageController ;
X_ZeroSequenceController_normal_operation_no_faults_T ZeroSequenceController
; real_T Integrator_CSTATE_b5 ; real_T Integrator_CSTATE_d ; }
X_normal_operation_no_faults_T ; typedef struct { real_T Integrator_CSTATE ;
real_T Integrator_CSTATE_k ; real_T Integrator_CSTATE_j ; real_T
Integrator_CSTATE_n ; real_T Integrator_CSTATE_l ; real_T Integrator_CSTATE_o
; real_T Integrator_CSTATE_b ; real_T Integrator_CSTATE_m ; real_T
Integrator_CSTATE_ja ; real_T VariableTransportDelay_CSTATE ; real_T
integrator_CSTATE ; real_T TransferFcn_CSTATE ; real_T Integrator_x1_CSTATE ;
real_T Integrator_x2_CSTATE ; real_T Integrator_CSTATE_lt ; real_T
VariableTransportDelay_CSTATE_i ; real_T integrator_CSTATE_m ; real_T
TransferFcn_CSTATE_a ; real_T Integrator_x1_CSTATE_n ; real_T
Integrator_x2_CSTATE_c ; real_T Integrator_CSTATE_bf ; real_T
VariableTransportDelay_CSTATE_o ; real_T integrator_CSTATE_b ; real_T
TransferFcn_CSTATE_l ; real_T Integrator_x1_CSTATE_o ; real_T
Integrator_x2_CSTATE_e ; real_T integrator_CSTATE_b5 [ 3 ] ; real_T
integrator_CSTATE_l [ 3 ] ; real_T integrator_CSTATE_n [ 3 ] ; real_T
integrator_CSTATE_i [ 3 ] ; real_T integrator_CSTATE_k [ 3 ] ; real_T
integrator_CSTATE_lw [ 3 ] ; real_T integrator_CSTATE_d [ 3 ] ; real_T
integrator_CSTATE_mn [ 3 ] ; real_T integrator_CSTATE_p [ 3 ] ; real_T
integrator_CSTATE_a [ 3 ] ; real_T integrator_CSTATE_j [ 3 ] ; real_T
integrator_CSTATE_jl [ 3 ] ; real_T integrator_CSTATE_nm [ 3 ] ; real_T
integrator_CSTATE_c [ 3 ] ; real_T integrator_CSTATE_e [ 3 ] ; real_T
integrator_CSTATE_f [ 3 ] ; real_T integrator_CSTATE_kd [ 3 ] ; real_T
integrator_CSTATE_c1 [ 3 ] ; real_T integrator_CSTATE_js [ 3 ] ; real_T
integrator_CSTATE_po [ 3 ] ; XDot_TrueRMS_normal_operation_no_faults_T
TrueRMS ; XDot_RMS_normal_operation_no_faults_T RMS ;
XDot_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_fb ;
XDot_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl_a
; XDot_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_k ;
XDot_ACVoltageController_normal_operation_no_faults_T ACVoltageController_l ;
XDot_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_f ;
XDot_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl_d
; XDot_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_d ;
XDot_ACVoltageController_normal_operation_no_faults_T ACVoltageController_n ;
XDot_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2 ;
XDot_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl ;
XDot_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression ;
XDot_ACVoltageController_normal_operation_no_faults_T ACVoltageController ;
XDot_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController ; real_T Integrator_CSTATE_b5 ; real_T
Integrator_CSTATE_d ; } XDot_normal_operation_no_faults_T ; typedef struct {
boolean_T Integrator_CSTATE ; boolean_T Integrator_CSTATE_k ; boolean_T
Integrator_CSTATE_j ; boolean_T Integrator_CSTATE_n ; boolean_T
Integrator_CSTATE_l ; boolean_T Integrator_CSTATE_o ; boolean_T
Integrator_CSTATE_b ; boolean_T Integrator_CSTATE_m ; boolean_T
Integrator_CSTATE_ja ; boolean_T VariableTransportDelay_CSTATE ; boolean_T
integrator_CSTATE ; boolean_T TransferFcn_CSTATE ; boolean_T
Integrator_x1_CSTATE ; boolean_T Integrator_x2_CSTATE ; boolean_T
Integrator_CSTATE_lt ; boolean_T VariableTransportDelay_CSTATE_i ; boolean_T
integrator_CSTATE_m ; boolean_T TransferFcn_CSTATE_a ; boolean_T
Integrator_x1_CSTATE_n ; boolean_T Integrator_x2_CSTATE_c ; boolean_T
Integrator_CSTATE_bf ; boolean_T VariableTransportDelay_CSTATE_o ; boolean_T
integrator_CSTATE_b ; boolean_T TransferFcn_CSTATE_l ; boolean_T
Integrator_x1_CSTATE_o ; boolean_T Integrator_x2_CSTATE_e ; boolean_T
integrator_CSTATE_b5 [ 3 ] ; boolean_T integrator_CSTATE_l [ 3 ] ; boolean_T
integrator_CSTATE_n [ 3 ] ; boolean_T integrator_CSTATE_i [ 3 ] ; boolean_T
integrator_CSTATE_k [ 3 ] ; boolean_T integrator_CSTATE_lw [ 3 ] ; boolean_T
integrator_CSTATE_d [ 3 ] ; boolean_T integrator_CSTATE_mn [ 3 ] ; boolean_T
integrator_CSTATE_p [ 3 ] ; boolean_T integrator_CSTATE_a [ 3 ] ; boolean_T
integrator_CSTATE_j [ 3 ] ; boolean_T integrator_CSTATE_jl [ 3 ] ; boolean_T
integrator_CSTATE_nm [ 3 ] ; boolean_T integrator_CSTATE_c [ 3 ] ; boolean_T
integrator_CSTATE_e [ 3 ] ; boolean_T integrator_CSTATE_f [ 3 ] ; boolean_T
integrator_CSTATE_kd [ 3 ] ; boolean_T integrator_CSTATE_c1 [ 3 ] ; boolean_T
integrator_CSTATE_js [ 3 ] ; boolean_T integrator_CSTATE_po [ 3 ] ;
XDis_TrueRMS_normal_operation_no_faults_T TrueRMS ;
XDis_RMS_normal_operation_no_faults_T RMS ;
XDis_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_fb ;
XDis_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl_a
; XDis_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_k ;
XDis_ACVoltageController_normal_operation_no_faults_T ACVoltageController_l ;
XDis_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_f ;
XDis_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl_d
; XDis_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_d ;
XDis_ACVoltageController_normal_operation_no_faults_T ACVoltageController_n ;
XDis_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2 ;
XDis_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl ;
XDis_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression ;
XDis_ACVoltageController_normal_operation_no_faults_T ACVoltageController ;
XDis_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController ; boolean_T Integrator_CSTATE_b5 ; boolean_T
Integrator_CSTATE_d ; } XDis_normal_operation_no_faults_T ; typedef struct {
real_T Integrator_CSTATE ; real_T Integrator_CSTATE_k ; real_T
Integrator_CSTATE_j ; real_T Integrator_CSTATE_n ; real_T Integrator_CSTATE_l
; real_T Integrator_CSTATE_o ; real_T Integrator_CSTATE_b ; real_T
Integrator_CSTATE_m ; real_T Integrator_CSTATE_ja ; real_T
VariableTransportDelay_CSTATE ; real_T integrator_CSTATE ; real_T
TransferFcn_CSTATE ; real_T Integrator_x1_CSTATE ; real_T
Integrator_x2_CSTATE ; real_T Integrator_CSTATE_lt ; real_T
VariableTransportDelay_CSTATE_i ; real_T integrator_CSTATE_m ; real_T
TransferFcn_CSTATE_a ; real_T Integrator_x1_CSTATE_n ; real_T
Integrator_x2_CSTATE_c ; real_T Integrator_CSTATE_bf ; real_T
VariableTransportDelay_CSTATE_o ; real_T integrator_CSTATE_b ; real_T
TransferFcn_CSTATE_l ; real_T Integrator_x1_CSTATE_o ; real_T
Integrator_x2_CSTATE_e ; real_T integrator_CSTATE_b5 [ 3 ] ; real_T
integrator_CSTATE_l [ 3 ] ; real_T integrator_CSTATE_n [ 3 ] ; real_T
integrator_CSTATE_i [ 3 ] ; real_T integrator_CSTATE_k [ 3 ] ; real_T
integrator_CSTATE_lw [ 3 ] ; real_T integrator_CSTATE_d [ 3 ] ; real_T
integrator_CSTATE_mn [ 3 ] ; real_T integrator_CSTATE_p [ 3 ] ; real_T
integrator_CSTATE_a [ 3 ] ; real_T integrator_CSTATE_j [ 3 ] ; real_T
integrator_CSTATE_jl [ 3 ] ; real_T integrator_CSTATE_nm [ 3 ] ; real_T
integrator_CSTATE_c [ 3 ] ; real_T integrator_CSTATE_e [ 3 ] ; real_T
integrator_CSTATE_f [ 3 ] ; real_T integrator_CSTATE_kd [ 3 ] ; real_T
integrator_CSTATE_c1 [ 3 ] ; real_T integrator_CSTATE_js [ 3 ] ; real_T
integrator_CSTATE_po [ 3 ] ; XAbsTol_TrueRMS_normal_operation_no_faults_T
TrueRMS ; XAbsTol_RMS_normal_operation_no_faults_T RMS ;
XAbsTol_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_fb ;
XAbsTol_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl_a ;
XAbsTol_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_k ;
XAbsTol_ACVoltageController_normal_operation_no_faults_T
ACVoltageController_l ;
XAbsTol_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_f ;
XAbsTol_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl_d ;
XAbsTol_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_d ;
XAbsTol_ACVoltageController_normal_operation_no_faults_T
ACVoltageController_n ;
XAbsTol_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2 ;
XAbsTol_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl ;
XAbsTol_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression ;
XAbsTol_ACVoltageController_normal_operation_no_faults_T ACVoltageController
; XAbsTol_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController ; real_T Integrator_CSTATE_b5 ; real_T
Integrator_CSTATE_d ; } CStateAbsTol_normal_operation_no_faults_T ; typedef
struct { real_T RelationalOperator_RelopInput_ZC ; real_T Integrator_Reset_ZC
; real_T Integrator_IntgUpLimit_ZC ; real_T Integrator_IntgLoLimit_ZC ;
real_T Integrator_LeaveSaturate_ZC ; real_T Step_StepTime_ZC ; real_T
DCregulatorTURNONidreferenceforrectifier_StepTime_ZC ; real_T
GridVoltagestabilizerTURNONiqreferenceforrectifier_StepTime_ZC ; real_T
Saturation_UprLim_ZC [ 3 ] ; real_T Saturation_LwrLim_ZC [ 3 ] ; real_T
EnableCirculatingCurrentSuppresion_StepTime_ZC ; real_T
EnableACRMSregulatorTURNONidreferenceforinverter_StepTime_ZC ; real_T
EnableReactivepowersetpointregulationiqreferenceforinverter_StepTime_ZC ;
real_T Saturation2_UprLim_ZC [ 3 ] ; real_T Saturation2_LwrLim_ZC [ 3 ] ;
real_T RelationalOperator_RelopInput_ZC_g ; real_T Integrator_Reset_ZC_l ;
real_T Integrator_IntgUpLimit_ZC_h ; real_T Integrator_IntgLoLimit_ZC_p ;
real_T Integrator_LeaveSaturate_ZC_m ; real_T Step_StepTime_ZC_o ; real_T
Saturation_UprLim_ZC_d [ 3 ] ; real_T Saturation_LwrLim_ZC_h [ 3 ] ; real_T
RelationalOperator_RelopInput_ZC_o ; real_T Integrator_Reset_ZC_e ; real_T
Integrator_IntgUpLimit_ZC_l ; real_T Integrator_IntgLoLimit_ZC_g ; real_T
Integrator_LeaveSaturate_ZC_j ; real_T Step_StepTime_ZC_k ; real_T
Saturation_UprLim_ZC_p [ 3 ] ; real_T Saturation_LwrLim_ZC_p [ 3 ] ; real_T
HitCrossing_HitNoOutput_ZC ; real_T HitCrossing_HitNoOutput_ZC_c ; real_T
HitCrossing_HitNoOutput_ZC_e ; real_T HitCrossing_HitNoOutput_ZC_cl ; real_T
HitCrossing_HitNoOutput_ZC_ci ; real_T HitCrossing_HitNoOutput_ZC_p ; real_T
HitCrossing_HitNoOutput_ZC_k ; real_T HitCrossing_HitNoOutput_ZC_kj ; real_T
HitCrossing_HitNoOutput_ZC_f ; real_T HitCrossing_HitNoOutput_ZC_n ; real_T
HitCrossing_HitNoOutput_ZC_cj ; real_T HitCrossing_HitNoOutput_ZC_eb ; real_T
HitCrossing_HitNoOutput_ZC_ec ; real_T Integrator_IntgUpLimit_ZC_g ; real_T
Integrator_IntgLoLimit_ZC_h ; real_T Integrator_LeaveSaturate_ZC_mr ; real_T
RelationalOperator_RelopInput_ZC_oa ; real_T Saturation2_UprLim_ZC_n ; real_T
Saturation2_LwrLim_ZC_l ; real_T HitCrossing_HitNoOutput_ZC_ebe ; real_T
Integrator_IntgUpLimit_ZC_m ; real_T Integrator_IntgLoLimit_ZC_k ; real_T
Integrator_LeaveSaturate_ZC_k ; real_T RelationalOperator_RelopInput_ZC_c ;
real_T Saturation2_UprLim_ZC_p ; real_T Saturation2_LwrLim_ZC_i ; real_T
HitCrossing_HitNoOutput_ZC_h ; real_T Integrator_IntgUpLimit_ZC_f ; real_T
Integrator_IntgLoLimit_ZC_b ; real_T Integrator_LeaveSaturate_ZC_f ; real_T
RelationalOperator_RelopInput_ZC_ob ; real_T Saturation2_UprLim_ZC_h ; real_T
Saturation2_LwrLim_ZC_m ; real_T EnableHarmonicFilters_StepTime_ZC ; real_T
EnergizeConverterswithstartupcurrentBypassesstartupresistors_StepTime_ZC ;
real_T EnergizegridHarmonicFilter_StepTime_ZC ; real_T
HitCrossing_HitNoOutput_ZC_j ; real_T HitCrossing_HitNoOutput_ZC_ju ; real_T
HitCrossing_HitNoOutput_ZC_a ; real_T HitCrossing_HitNoOutput_ZC_jt ; real_T
HitCrossing_HitNoOutput_ZC_p2 ; real_T HitCrossing_HitNoOutput_ZC_k2 ; real_T
HitCrossing_HitNoOutput_ZC_i ; real_T HitCrossing_HitNoOutput_ZC_pu ; real_T
HitCrossing_HitNoOutput_ZC_nn ; real_T HitCrossing_HitNoOutput_ZC_eh ; real_T
HitCrossing_HitNoOutput_ZC_ca ; real_T HitCrossing_HitNoOutput_ZC_cih ;
real_T Switchonloadnetwork_StepTime_ZC ; real_T HitCrossing_HitNoOutput_ZC_hn
; real_T HitCrossing_HitNoOutput_ZC_ft ; real_T HitCrossing_HitNoOutput_ZC_i4
; ZCV_TrueRMS_normal_operation_no_faults_T TrueRMS ;
ZCV_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl_a
; ZCV_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_k ;
ZCV_ACVoltageController_normal_operation_no_faults_T ACVoltageController_l ;
ZCV_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl_d
; ZCV_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_d ;
ZCV_ACVoltageController_normal_operation_no_faults_T ACVoltageController_n ;
ZCV_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl ;
ZCV_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression ;
ZCV_ACVoltageController_normal_operation_no_faults_T ACVoltageController ;
real_T Saturation_UprLim_ZC_k [ 3 ] ; real_T Saturation_LwrLim_ZC_pm [ 3 ] ;
} ZCV_normal_operation_no_faults_T ; typedef struct { ZCSigState
RelationalOperator_RelopInput_ZCE ; ZCSigState Integrator_Reset_ZCE ;
ZCSigState Integrator_IntgUpLimit_ZCE ; ZCSigState Integrator_IntgLoLimit_ZCE
; ZCSigState Integrator_LeaveSaturate_ZCE ; ZCSigState Step_StepTime_ZCE ;
ZCSigState DCregulatorTURNONidreferenceforrectifier_StepTime_ZCE ; ZCSigState
GridVoltagestabilizerTURNONiqreferenceforrectifier_StepTime_ZCE ; ZCSigState
Saturation_UprLim_ZCE [ 3 ] ; ZCSigState Saturation_LwrLim_ZCE [ 3 ] ;
ZCSigState EnableCirculatingCurrentSuppresion_StepTime_ZCE ; ZCSigState
EnableACRMSregulatorTURNONidreferenceforinverter_StepTime_ZCE ; ZCSigState
EnableReactivepowersetpointregulationiqreferenceforinverter_StepTime_ZCE ;
ZCSigState Saturation2_UprLim_ZCE [ 3 ] ; ZCSigState Saturation2_LwrLim_ZCE [
3 ] ; ZCSigState RelationalOperator_RelopInput_ZCE_e ; ZCSigState
Integrator_Reset_ZCE_h ; ZCSigState Integrator_IntgUpLimit_ZCE_n ; ZCSigState
Integrator_IntgLoLimit_ZCE_g ; ZCSigState Integrator_LeaveSaturate_ZCE_b ;
ZCSigState Step_StepTime_ZCE_k ; ZCSigState Saturation_UprLim_ZCE_a [ 3 ] ;
ZCSigState Saturation_LwrLim_ZCE_e [ 3 ] ; ZCSigState
RelationalOperator_RelopInput_ZCE_h ; ZCSigState Integrator_Reset_ZCE_n ;
ZCSigState Integrator_IntgUpLimit_ZCE_h ; ZCSigState
Integrator_IntgLoLimit_ZCE_c ; ZCSigState Integrator_LeaveSaturate_ZCE_i ;
ZCSigState Step_StepTime_ZCE_f ; ZCSigState Saturation_UprLim_ZCE_d [ 3 ] ;
ZCSigState Saturation_LwrLim_ZCE_a [ 3 ] ; ZCSigState
HitCrossing_HitNoOutput_ZCE ; ZCSigState HitCrossing_HitNoOutput_ZCE_l ;
ZCSigState HitCrossing_HitNoOutput_ZCE_c ; ZCSigState
HitCrossing_HitNoOutput_ZCE_a ; ZCSigState HitCrossing_HitNoOutput_ZCE_h ;
ZCSigState HitCrossing_HitNoOutput_ZCE_cm ; ZCSigState
HitCrossing_HitNoOutput_ZCE_o ; ZCSigState HitCrossing_HitNoOutput_ZCE_k ;
ZCSigState HitCrossing_HitNoOutput_ZCE_e ; ZCSigState
HitCrossing_HitNoOutput_ZCE_aq ; ZCSigState HitCrossing_HitNoOutput_ZCE_cs ;
ZCSigState HitCrossing_HitNoOutput_ZCE_o2 ; ZCSigState
HitCrossing_HitNoOutput_ZCE_g ; ZCSigState Integrator_IntgUpLimit_ZCE_c ;
ZCSigState Integrator_IntgLoLimit_ZCE_i ; ZCSigState
Integrator_LeaveSaturate_ZCE_p ; ZCSigState
RelationalOperator_RelopInput_ZCE_hv ; ZCSigState Saturation2_UprLim_ZCE_a ;
ZCSigState Saturation2_LwrLim_ZCE_a ; ZCSigState
HitCrossing_HitNoOutput_ZCE_cy ; ZCSigState Integrator_IntgUpLimit_ZCE_a ;
ZCSigState Integrator_IntgLoLimit_ZCE_k ; ZCSigState
Integrator_LeaveSaturate_ZCE_ia ; ZCSigState
RelationalOperator_RelopInput_ZCE_b ; ZCSigState Saturation2_UprLim_ZCE_i ;
ZCSigState Saturation2_LwrLim_ZCE_f ; ZCSigState
HitCrossing_HitNoOutput_ZCE_f ; ZCSigState Integrator_IntgUpLimit_ZCE_d ;
ZCSigState Integrator_IntgLoLimit_ZCE_n ; ZCSigState
Integrator_LeaveSaturate_ZCE_pq ; ZCSigState
RelationalOperator_RelopInput_ZCE_g ; ZCSigState Saturation2_UprLim_ZCE_m ;
ZCSigState Saturation2_LwrLim_ZCE_e ; ZCSigState
EnableHarmonicFilters_StepTime_ZCE ; ZCSigState
EnergizeConverterswithstartupcurrentBypassesstartupresistors_StepTime_ZCE ;
ZCSigState EnergizegridHarmonicFilter_StepTime_ZCE ; ZCSigState
HitCrossing_HitNoOutput_ZCE_n ; ZCSigState HitCrossing_HitNoOutput_ZCE_ex ;
ZCSigState HitCrossing_HitNoOutput_ZCE_d ; ZCSigState
HitCrossing_HitNoOutput_ZCE_hu ; ZCSigState HitCrossing_HitNoOutput_ZCE_i ;
ZCSigState HitCrossing_HitNoOutput_ZCE_oa ; ZCSigState
HitCrossing_HitNoOutput_ZCE_kn ; ZCSigState HitCrossing_HitNoOutput_ZCE_m ;
ZCSigState HitCrossing_HitNoOutput_ZCE_kw ; ZCSigState
HitCrossing_HitNoOutput_ZCE_exd ; ZCSigState HitCrossing_HitNoOutput_ZCE_gd ;
ZCSigState HitCrossing_HitNoOutput_ZCE_gp ; ZCSigState
Switchonloadnetwork_StepTime_ZCE ; ZCSigState HitCrossing_HitNoOutput_ZCE_j ;
ZCSigState HitCrossing_HitNoOutput_ZCE_ik ; ZCSigState
HitCrossing_HitNoOutput_ZCE_lo ; ZCE_TrueRMS_normal_operation_no_faults_T
TrueRMS ; ZCE_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl_a ;
ZCE_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_k ;
ZCE_ACVoltageController_normal_operation_no_faults_T ACVoltageController_l ;
ZCE_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl_d
; ZCE_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_d ;
ZCE_ACVoltageController_normal_operation_no_faults_T ACVoltageController_n ;
ZCE_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl ;
ZCE_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression ;
ZCE_ACVoltageController_normal_operation_no_faults_T ACVoltageController ;
ZCSigState Saturation_UprLim_ZCE_l [ 3 ] ; ZCSigState Saturation_LwrLim_ZCE_l
[ 3 ] ; } PrevZCX_normal_operation_no_faults_T ; typedef struct { const
real_T B_185_43_0 ; const real_T B_185_48_0 ; const real_T B_185_78_0 ; const
real_T B_185_83_0 ; const real_T B_185_241_0 ; const real_T B_185_246_0 ;
const real_T B_185_429_0 ; const real_T B_185_434_0 ; const real_T
B_185_464_0 ; const real_T B_185_469_0 ; const real_T B_185_632_0 ; const
real_T B_185_637_0 ; const real_T B_185_667_0 ; const real_T B_185_672_0 ; }
ConstB_normal_operation_no_faults_T ;
#define normal_operation_no_faults_rtC(S) ((ConstB_normal_operation_no_faults_T *) _ssGetConstBlockIO(S))
struct P_Subsystempi2delay_normal_operation_no_faults_T_ { real_T P_0 [ 2 ] ;
} ; struct P_Subsystem1_normal_operation_no_faults_T_ { real_T P_0 [ 2 ] ; }
; struct P_Subsystempi2delay_normal_operation_no_faults_h_T_ { real_T P_0 [ 2
] ; } ; struct P_Subsystem1_normal_operation_no_faults_g_T_ { real_T P_0 [ 2
] ; } ; struct P_Subsystempi2delay_normal_operation_no_faults_l_T_ { real_T
P_0 [ 2 ] ; } ; struct P_Subsystem1_normal_operation_no_faults_h_T_ { real_T
P_0 [ 2 ] ; } ; struct P_ZeroSequenceController_normal_operation_no_faults_T_
{ real_T P_0 ; real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5
; real_T P_6 ; real_T P_7 ; real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T
P_11 ; real_T P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ;
real_T P_17 ; real_T P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ; real_T
P_22 ; real_T P_23 ; real_T P_24 ; real_T P_25 ; real_T P_26 ; real_T P_27 [
2 ] ; real_T P_28 [ 2 ] ; real_T P_29 ; } ; struct
P_RMS_normal_operation_no_faults_T_ { real_T P_0 ; real_T P_1 ; real_T P_2 ;
real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; real_T P_8 ;
real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T P_13 ; real_T
P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T P_18 ; real_T P_19 ;
real_T P_20 ; real_T P_21 ; } ; struct
P_TrueRMS_normal_operation_no_faults_T_ { real_T P_0 ; real_T P_1 ; real_T
P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; real_T
P_8 ; } ; struct P_ACVoltageController_normal_operation_no_faults_T_ { real_T
P_0 ; real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T
P_6 ; real_T P_7 ; real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ;
real_T P_12 ; P_TrueRMS_normal_operation_no_faults_T TrueRMS ;
P_RMS_normal_operation_no_faults_T RMS ; } ; struct
P_Circulatingcurrentsuppression_normal_operation_no_faults_T_ { real_T P_0 ;
real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ;
real_T P_7 ; real_T P_8 ; real_T P_9 ; real_T P_10 [ 9 ] ; real_T P_11 ;
real_T P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T
P_17 ; real_T P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ; real_T P_22 ;
real_T P_23 ; real_T P_24 ; real_T P_25 [ 2 ] ; real_T P_26 [ 2 ] ; real_T
P_27 ; real_T P_28 ; real_T P_29 ; real_T P_30 [ 9 ] ; real_T P_31 ; real_T
P_32 ; real_T P_33 ; P_Subsystem1_normal_operation_no_faults_g_T Subsystem1_k
; P_Subsystempi2delay_normal_operation_no_faults_h_T Subsystempi2delay_e ;
P_Subsystem1_normal_operation_no_faults_T Subsystem1 ;
P_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay ; } ;
struct P_Subsystempi2delay_normal_operation_no_faults_g_T_ { real_T P_0 [ 2 ]
; } ; struct P_Subsystem1_normal_operation_no_faults_j_T_ { real_T P_0 [ 2 ]
; } ; struct P_AutomaticGainControl_normal_operation_no_faults_T_ { real_T
P_0 ; real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T
P_6 ; real_T P_7 ; real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ;
real_T P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T
P_17 ; real_T P_18 ; real_T P_19 ; real_T P_20 [ 9 ] ; real_T P_21 ;
P_Subsystem1_normal_operation_no_faults_j_T Subsystem1 ;
P_Subsystempi2delay_normal_operation_no_faults_g_T Subsystempi2delay ; } ;
struct P_HarmonicGenerator_normal_operation_no_faults_T_ { real_T P_0 ;
real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ;
real_T P_7 [ 3 ] ; real_T P_8 [ 3 ] ; real_T P_9 [ 3 ] ; real_T P_10 ; real_T
P_11 ; real_T P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ;
real_T P_17 ; real_T P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ; real_T
P_22 [ 3 ] ; real_T P_23 [ 3 ] ; real_T P_24 [ 3 ] ; real_T P_25 ; } ; struct
P_Signalgenerator_normal_operation_no_faults_T_ { real_T P_0 ; real_T P_1 ;
real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 [ 6 ] ; real_T
P_7 [ 6 ] ; real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12
; real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ;
real_T P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ; real_T P_22 ; real_T
P_23 ; real_T P_24 ; real_T P_25 ; real_T P_26 ; real_T P_27 ; real_T P_28 ;
real_T P_29 ; real_T P_30 ; real_T P_31 ; real_T P_32 ; real_T P_33 ; real_T
P_34 ; real_T P_35 ; real_T P_36 ; real_T P_37 ; real_T P_38 ; real_T P_39 ;
real_T P_40 ; real_T P_41 ; real_T P_42 ; real_T P_43 ; real_T P_44 ; real_T
P_45 ; } ; struct P_normal_operation_no_faults_T_ { real_T P_0 ; real_T P_1 ;
real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ;
real_T P_8 ; real_T P_9 ; real_T P_10 [ 9 ] ; real_T P_11 ; real_T P_12 ;
real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T
P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ; real_T P_22 ; real_T P_23 ;
real_T P_24 ; real_T P_25 [ 2 ] ; real_T P_26 [ 2 ] ; real_T P_27 ; real_T
P_28 ; real_T P_29 ; real_T P_30 [ 9 ] ; real_T P_31 ; real_T P_32 ; real_T
P_33 ; real_T P_34 [ 3 ] ; real_T P_35 [ 3 ] ; real_T P_36 [ 3 ] ; real_T
P_37 [ 3 ] ; real_T P_38 [ 3 ] ; real_T P_39 [ 3 ] ; real_T P_40 [ 3 ] ;
real_T P_41 [ 3 ] ; real_T P_42 [ 3 ] ; real_T P_43 [ 3 ] ; real_T P_44 ;
real_T P_45 ; real_T P_46 ; real_T P_47 ; real_T P_48 ; real_T P_49 ; real_T
P_50 ; real_T P_51 [ 3 ] ; real_T P_52 [ 3 ] ; real_T P_53 [ 3 ] ; real_T
P_54 [ 3 ] ; real_T P_55 [ 3 ] ; real_T P_56 [ 3 ] ; real_T P_57 [ 3 ] ;
real_T P_58 [ 3 ] ; real_T P_59 [ 3 ] ; real_T P_60 [ 3 ] ; real_T P_61 [ 3 ]
; real_T P_62 [ 3 ] ; real_T P_63 [ 3 ] ; real_T P_64 [ 3 ] ; real_T P_65 [ 3
] ; real_T P_66 [ 3 ] ; real_T P_67 [ 3 ] ; real_T P_68 [ 3 ] ; real_T P_69 [
3 ] ; real_T P_70 [ 3 ] ; real_T P_71 [ 3 ] ; real_T P_72 [ 3 ] ; real_T P_73
[ 3 ] ; real_T P_74 [ 3 ] ; real_T P_75 [ 3 ] ; real_T P_76 [ 3 ] ; real_T
P_77 [ 3 ] ; real_T P_78 [ 3 ] ; real_T P_79 [ 3 ] ; real_T P_80 [ 3 ] ;
real_T P_81 [ 3 ] ; real_T P_82 [ 3 ] ; real_T P_83 [ 3 ] ; real_T P_84 [ 3 ]
; real_T P_85 [ 3 ] ; real_T P_86 [ 3 ] ; real_T P_87 ; real_T P_88 ; real_T
P_89 ; real_T P_90 ; real_T P_91 ; real_T P_92 ; real_T P_93 ; real_T P_94 ;
real_T P_95 ; real_T P_96 ; real_T P_97 ; real_T P_98 ; real_T P_99 ; real_T
P_100 ; real_T P_101 ; real_T P_102 ; real_T P_103 ; real_T P_104 ; real_T
P_105 ; real_T P_106 ; real_T P_107 ; real_T P_108 ; real_T P_109 [ 95 ] ;
real_T P_110 [ 2 ] ; real_T P_111 ; real_T P_112 ; real_T P_113 ; real_T
P_114 [ 9 ] ; real_T P_115 ; real_T P_116 ; real_T P_117 ; real_T P_118 ;
real_T P_119 ; real_T P_120 ; real_T P_121 ; real_T P_122 ; real_T P_123 ;
real_T P_124 ; real_T P_125 ; real_T P_126 ; real_T P_127 ; real_T P_128 ;
real_T P_129 ; real_T P_130 [ 2 ] ; real_T P_131 [ 2 ] ; real_T P_132 ;
real_T P_133 ; real_T P_134 ; real_T P_135 ; real_T P_136 ; real_T P_137 ;
real_T P_138 ; real_T P_139 ; real_T P_140 ; real_T P_141 ; real_T P_142 ;
real_T P_143 ; real_T P_144 ; real_T P_145 ; real_T P_146 ; real_T P_147 ;
real_T P_148 ; real_T P_149 ; real_T P_150 ; real_T P_151 ; real_T P_152 ;
real_T P_153 [ 9 ] ; real_T P_154 ; real_T P_155 ; real_T P_156 ; real_T
P_157 ; real_T P_158 ; real_T P_159 ; real_T P_160 ; real_T P_161 ; real_T
P_162 ; real_T P_163 ; real_T P_164 ; real_T P_165 ; real_T P_166 ; real_T
P_167 ; real_T P_168 ; real_T P_169 ; real_T P_170 ; real_T P_171 ; real_T
P_172 ; real_T P_173 ; real_T P_174 ; real_T P_175 ; real_T P_176 ; real_T
P_177 ; real_T P_178 ; real_T P_179 ; real_T P_180 ; real_T P_181 ; real_T
P_182 ; real_T P_183 ; real_T P_184 ; real_T P_185 ; real_T P_186 ; real_T
P_187 ; real_T P_188 [ 9 ] ; real_T P_189 ; real_T P_190 ; real_T P_191 ;
real_T P_192 ; real_T P_193 [ 50 ] ; real_T P_194 ; real_T P_195 ; real_T
P_196 ; real_T P_197 ; real_T P_198 ; real_T P_199 ; real_T P_200 ; real_T
P_201 ; real_T P_202 [ 50 ] ; real_T P_203 ; real_T P_204 ; real_T P_205 ;
real_T P_206 ; real_T P_207 ; real_T P_208 ; real_T P_209 ; real_T P_210 ;
real_T P_211 [ 50 ] ; real_T P_212 ; real_T P_213 ; real_T P_214 ; real_T
P_215 ; real_T P_216 ; real_T P_217 ; real_T P_218 ; real_T P_219 ; real_T
P_220 [ 50 ] ; real_T P_221 ; real_T P_222 ; real_T P_223 ; real_T P_224 ;
real_T P_225 ; real_T P_226 ; real_T P_227 ; real_T P_228 ; real_T P_229 [ 50
] ; real_T P_230 ; real_T P_231 ; real_T P_232 ; real_T P_233 ; real_T P_234
; real_T P_235 ; real_T P_236 ; real_T P_237 ; real_T P_238 [ 50 ] ; real_T
P_239 ; real_T P_240 ; real_T P_241 ; real_T P_242 ; real_T P_243 ; real_T
P_244 ; real_T P_245 ; real_T P_246 ; real_T P_247 [ 2 ] ; real_T P_248 ;
real_T P_249 ; real_T P_250 ; real_T P_251 [ 9 ] ; real_T P_252 ; real_T
P_253 ; real_T P_254 ; real_T P_255 ; real_T P_256 ; real_T P_257 ; real_T
P_258 ; real_T P_259 ; real_T P_260 ; real_T P_261 ; real_T P_262 ; real_T
P_263 ; real_T P_264 ; real_T P_265 ; real_T P_266 ; real_T P_267 ; real_T
P_268 ; real_T P_269 ; real_T P_270 ; real_T P_271 ; real_T P_272 ; real_T
P_273 ; real_T P_274 ; real_T P_275 ; real_T P_276 ; real_T P_277 ; real_T
P_278 ; real_T P_279 ; real_T P_280 ; real_T P_281 [ 9 ] ; real_T P_282 ;
real_T P_283 ; real_T P_284 ; real_T P_285 ; real_T P_286 ; real_T P_287 ;
real_T P_288 ; real_T P_289 ; real_T P_290 ; real_T P_291 ; real_T P_292 ;
real_T P_293 ; real_T P_294 ; real_T P_295 ; real_T P_296 ; real_T P_297 ;
real_T P_298 ; real_T P_299 ; real_T P_300 ; real_T P_301 ; real_T P_302 ;
real_T P_303 ; real_T P_304 [ 9 ] ; real_T P_305 ; real_T P_306 [ 50 ] ;
real_T P_307 ; real_T P_308 ; real_T P_309 [ 3 ] ; real_T P_310 [ 3 ] ;
real_T P_311 ; real_T P_312 ; real_T P_313 ; real_T P_314 ; real_T P_315 ;
real_T P_316 ; real_T P_317 [ 50 ] ; real_T P_318 ; real_T P_319 ; real_T
P_320 ; real_T P_321 ; real_T P_322 ; real_T P_323 ; real_T P_324 ; real_T
P_325 ; real_T P_326 [ 50 ] ; real_T P_327 ; real_T P_328 ; real_T P_329 ;
real_T P_330 ; real_T P_331 ; real_T P_332 ; real_T P_333 ; real_T P_334 ;
real_T P_335 [ 50 ] ; real_T P_336 ; real_T P_337 ; real_T P_338 ; real_T
P_339 ; real_T P_340 ; real_T P_341 ; real_T P_342 ; real_T P_343 ; real_T
P_344 [ 50 ] ; real_T P_345 ; real_T P_346 ; real_T P_347 ; real_T P_348 ;
real_T P_349 ; real_T P_350 ; real_T P_351 ; real_T P_352 ; real_T P_353 [ 50
] ; real_T P_354 ; real_T P_355 ; real_T P_356 ; real_T P_357 ; real_T P_358
; real_T P_359 ; real_T P_360 ; real_T P_361 ; real_T P_362 [ 2 ] ; real_T
P_363 ; real_T P_364 ; real_T P_365 ; real_T P_366 [ 9 ] ; real_T P_367 ;
real_T P_368 ; real_T P_369 ; real_T P_370 ; real_T P_371 ; real_T P_372 ;
real_T P_373 ; real_T P_374 ; real_T P_375 ; real_T P_376 ; real_T P_377 ;
real_T P_378 ; real_T P_379 ; real_T P_380 ; real_T P_381 ; real_T P_382 [ 2
] ; real_T P_383 [ 2 ] ; real_T P_384 ; real_T P_385 ; real_T P_386 ; real_T
P_387 ; real_T P_388 ; real_T P_389 ; real_T P_390 ; real_T P_391 ; real_T
P_392 ; real_T P_393 ; real_T P_394 ; real_T P_395 ; real_T P_396 ; real_T
P_397 ; real_T P_398 ; real_T P_399 ; real_T P_400 ; real_T P_401 ; real_T
P_402 [ 9 ] ; real_T P_403 ; real_T P_404 ; real_T P_405 ; real_T P_406 ;
real_T P_407 ; real_T P_408 ; real_T P_409 ; real_T P_410 ; real_T P_411 ;
real_T P_412 ; real_T P_413 ; real_T P_414 ; real_T P_415 ; real_T P_416 ;
real_T P_417 ; real_T P_418 ; real_T P_419 ; real_T P_420 ; real_T P_421 ;
real_T P_422 ; real_T P_423 ; real_T P_424 ; real_T P_425 ; real_T P_426 ;
real_T P_427 ; real_T P_428 ; real_T P_429 ; real_T P_430 ; real_T P_431 ;
real_T P_432 ; real_T P_433 ; real_T P_434 [ 9 ] ; real_T P_435 ; real_T
P_436 [ 50 ] ; real_T P_437 ; real_T P_438 ; real_T P_439 ; real_T P_440 ;
real_T P_441 ; real_T P_442 ; real_T P_443 ; real_T P_444 ; real_T P_445 [ 50
] ; real_T P_446 ; real_T P_447 ; real_T P_448 ; real_T P_449 ; real_T P_450
; real_T P_451 ; real_T P_452 ; real_T P_453 ; real_T P_454 [ 50 ] ; real_T
P_455 ; real_T P_456 ; real_T P_457 ; real_T P_458 ; real_T P_459 ; real_T
P_460 ; real_T P_461 ; real_T P_462 ; real_T P_463 [ 50 ] ; real_T P_464 ;
real_T P_465 ; real_T P_466 ; real_T P_467 ; real_T P_468 ; real_T P_469 ;
real_T P_470 ; real_T P_471 ; real_T P_472 [ 50 ] ; real_T P_473 ; real_T
P_474 ; real_T P_475 ; real_T P_476 ; real_T P_477 ; real_T P_478 ; real_T
P_479 ; real_T P_480 ; real_T P_481 [ 50 ] ; real_T P_482 ; real_T P_483 ;
real_T P_484 ; real_T P_485 ; real_T P_486 ; real_T P_487 ; real_T P_488 ;
real_T P_489 ; real_T P_490 [ 2 ] ; real_T P_491 ; real_T P_492 ; real_T
P_493 ; real_T P_494 [ 9 ] ; real_T P_495 ; real_T P_496 ; real_T P_497 ;
real_T P_498 ; real_T P_499 ; real_T P_500 ; real_T P_501 ; real_T P_502 ;
real_T P_503 ; real_T P_504 ; real_T P_505 ; real_T P_506 ; real_T P_507 ;
real_T P_508 ; real_T P_509 ; real_T P_510 [ 2 ] ; real_T P_511 [ 2 ] ;
real_T P_512 ; real_T P_513 ; real_T P_514 ; real_T P_515 ; real_T P_516 ;
real_T P_517 ; real_T P_518 ; real_T P_519 ; real_T P_520 ; real_T P_521 ;
real_T P_522 ; real_T P_523 ; real_T P_524 ; real_T P_525 ; real_T P_526 ;
real_T P_527 ; real_T P_528 ; real_T P_529 ; real_T P_530 [ 9 ] ; real_T
P_531 ; real_T P_532 ; real_T P_533 ; real_T P_534 ; real_T P_535 ; real_T
P_536 ; real_T P_537 ; real_T P_538 ; real_T P_539 ; real_T P_540 ; real_T
P_541 ; real_T P_542 ; real_T P_543 ; real_T P_544 ; real_T P_545 ; real_T
P_546 ; real_T P_547 ; real_T P_548 ; real_T P_549 ; real_T P_550 ; real_T
P_551 ; real_T P_552 ; real_T P_553 ; real_T P_554 ; real_T P_555 ; real_T
P_556 ; real_T P_557 ; real_T P_558 ; real_T P_559 ; real_T P_560 ; real_T
P_561 ; real_T P_562 [ 9 ] ; real_T P_563 ; real_T P_564 [ 50 ] ; real_T
P_565 ; real_T P_566 ; real_T P_567 ; real_T P_568 ; real_T P_569 ; real_T
P_570 ; real_T P_571 ; real_T P_572 ; real_T P_573 [ 50 ] ; real_T P_574 ;
real_T P_575 ; real_T P_576 ; real_T P_577 ; real_T P_578 ; real_T P_579 ;
real_T P_580 ; real_T P_581 ; real_T P_582 [ 50 ] ; real_T P_583 ; real_T
P_584 ; real_T P_585 ; real_T P_586 ; real_T P_587 ; real_T P_588 ; real_T
P_589 ; real_T P_590 ; real_T P_591 [ 50 ] ; real_T P_592 ; real_T P_593 ;
real_T P_594 ; real_T P_595 ; real_T P_596 ; real_T P_597 ; real_T P_598 ;
real_T P_599 ; real_T P_600 [ 50 ] ; real_T P_601 ; real_T P_602 ; real_T
P_603 ; real_T P_604 ; real_T P_605 ; real_T P_606 ; real_T P_607 ; real_T
P_608 ; real_T P_609 [ 50 ] ; real_T P_610 ; real_T P_611 ; real_T P_612 ;
real_T P_613 ; real_T P_614 ; real_T P_615 ; real_T P_616 ; real_T P_617 ;
real_T P_618 ; real_T P_619 ; real_T P_620 ; real_T P_621 ; real_T P_622 ;
real_T P_623 ; real_T P_624 ; real_T P_625 ; real_T P_626 ; real_T P_627 [ 3
] ; real_T P_628 ; real_T P_629 ; real_T P_630 ; real_T P_631 ; real_T P_632
; real_T P_633 ; real_T P_634 ; real_T P_635 ; real_T P_636 ; real_T P_637 ;
real_T P_638 [ 3 ] ; real_T P_639 ; real_T P_640 ; real_T P_641 ; real_T
P_642 ; real_T P_643 ; real_T P_644 ; real_T P_645 ; real_T P_646 ; real_T
P_647 ; real_T P_648 ; real_T P_649 [ 3 ] ; real_T P_650 ; real_T P_651 ;
real_T P_652 ; real_T P_653 ; real_T P_654 ; real_T P_655 ; real_T P_656 ;
real_T P_657 ; real_T P_658 ; real_T P_659 ; real_T P_660 [ 3 ] ; real_T
P_661 ; real_T P_662 ; real_T P_663 ; real_T P_664 ; real_T P_665 ; real_T
P_666 ; real_T P_667 ; real_T P_668 ; real_T P_669 ; real_T P_670 ; real_T
P_671 [ 3 ] ; real_T P_672 ; real_T P_673 ; real_T P_674 ; real_T P_675 ;
real_T P_676 ; real_T P_677 ; real_T P_678 ; real_T P_679 ; real_T P_680 ;
real_T P_681 ; real_T P_682 [ 3 ] ; real_T P_683 ; real_T P_684 [ 2 ] ;
real_T P_685 [ 59536 ] ; real_T P_686 [ 2 ] ; real_T P_687 [ 39284 ] ; real_T
P_688 [ 2 ] ; real_T P_689 [ 58804 ] ; real_T P_690 [ 2 ] ; real_T P_691 [
38801 ] ; real_T P_692 [ 2 ] ; real_T P_693 [ 244 ] ; real_T P_694 ; real_T
P_695 ; real_T P_696 ; real_T P_697 [ 4 ] ; real_T P_698 [ 4 ] ; real_T P_699
; real_T P_700 ; real_T P_701 ; real_T P_702 ; real_T P_703 [ 5 ] ; real_T
P_704 [ 5 ] ; real_T P_705 ; real_T P_706 ; real_T P_707 ; real_T P_708 ;
real_T P_709 ; real_T P_710 [ 5 ] ; real_T P_711 [ 5 ] ; real_T P_712 ;
real_T P_713 ; real_T P_714 ; real_T P_715 ; real_T P_716 ; real_T P_717 [ 5
] ; real_T P_718 [ 5 ] ; real_T P_719 ; real_T P_720 ; real_T P_721 ; real_T
P_722 ; real_T P_723 ; real_T P_724 [ 4 ] ; real_T P_725 [ 4 ] ; real_T P_726
; real_T P_727 ; real_T P_728 ; real_T P_729 ; real_T P_730 [ 5 ] ; real_T
P_731 [ 5 ] ; real_T P_732 ; real_T P_733 ; real_T P_734 ; real_T P_735 ;
real_T P_736 ; real_T P_737 [ 5 ] ; real_T P_738 [ 5 ] ; real_T P_739 ;
real_T P_740 ; real_T P_741 ; real_T P_742 ; real_T P_743 ; real_T P_744 [ 5
] ; real_T P_745 [ 5 ] ; real_T P_746 ; real_T P_747 ; real_T P_748 ; real_T
P_749 ; real_T P_750 ; real_T P_751 [ 4 ] ; real_T P_752 [ 4 ] ; real_T P_753
; real_T P_754 ; real_T P_755 ; real_T P_756 ; real_T P_757 [ 5 ] ; real_T
P_758 [ 5 ] ; real_T P_759 ; real_T P_760 ; real_T P_761 ; real_T P_762 ;
real_T P_763 ; real_T P_764 [ 5 ] ; real_T P_765 [ 5 ] ; real_T P_766 ;
real_T P_767 ; real_T P_768 ; real_T P_769 ; real_T P_770 ; real_T P_771 [ 5
] ; real_T P_772 [ 5 ] ; real_T P_773 ; real_T P_774 ; real_T P_775 ; real_T
P_776 ; real_T P_777 ; real_T P_778 [ 4 ] ; real_T P_779 [ 4 ] ; real_T P_780
; real_T P_781 ; real_T P_782 ; real_T P_783 ; real_T P_784 [ 5 ] ; real_T
P_785 [ 5 ] ; real_T P_786 ; real_T P_787 ; real_T P_788 ; real_T P_789 ;
real_T P_790 ; real_T P_791 [ 5 ] ; real_T P_792 [ 5 ] ; real_T P_793 ;
real_T P_794 ; real_T P_795 ; real_T P_796 ; real_T P_797 ; real_T P_798 [ 5
] ; real_T P_799 [ 5 ] ; real_T P_800 ; real_T P_801 ; real_T P_802 ; real_T
P_803 ; real_T P_804 ; real_T P_805 [ 2 ] ; real_T P_806 [ 2 ] ; real_T P_807
; real_T P_808 ; real_T P_809 ; real_T P_810 ; real_T P_811 ; real_T P_812 ;
real_T P_813 ; real_T P_814 ; real_T P_815 ; real_T P_816 ; real_T P_817 ;
real_T P_818 ; real_T P_819 ; real_T P_820 ; real_T P_821 ; real_T P_822 ;
real_T P_823 ; real_T P_824 ; real_T P_825 ; real_T P_826 ; real_T P_827 ;
real_T P_828 ; real_T P_829 ; real_T P_830 ; real_T P_831 ; real_T P_832 ;
real_T P_833 ; real_T P_834 ; real_T P_835 ; real_T P_836 ; real_T P_837 ;
real_T P_838 ; real_T P_839 ; real_T P_840 ; real_T P_841 ; real_T P_842 ;
real_T P_843 ; real_T P_844 ; real_T P_845 ; real_T P_846 ; real_T P_847 ;
real_T P_848 ; real_T P_849 ; real_T P_850 ; real_T P_851 ; real_T P_852 ;
real_T P_853 ; real_T P_854 ; real_T P_855 ; real_T P_856 ; real_T P_857 ;
real_T P_858 ; real_T P_859 ; real_T P_860 ; real_T P_861 ; real_T P_862 ;
real_T P_863 ; real_T P_864 ; real_T P_865 ; real_T P_866 ; real_T P_867 ;
real_T P_868 ; real_T P_869 ; real_T P_870 ; real_T P_871 ; real_T P_872 ;
real_T P_873 ; real_T P_874 ; real_T P_875 ; real_T P_876 ; real_T P_877 ;
real_T P_878 ; real_T P_879 ; real_T P_880 ; real_T P_881 ; real_T P_882 ;
real_T P_883 ; real_T P_884 ; real_T P_885 ; real_T P_886 ; real_T P_887 ;
real_T P_888 ; real_T P_889 ; real_T P_890 ; real_T P_891 ; real_T P_892 ;
real_T P_893 ; real_T P_894 ; real_T P_895 ; real_T P_896 ; real_T P_897 ;
real_T P_898 ; real_T P_899 ; real_T P_900 ; real_T P_901 ; real_T P_902 ;
real_T P_903 ; real_T P_904 ; real_T P_905 ; real_T P_906 ; real_T P_907 ;
real_T P_908 ; real_T P_909 ; real_T P_910 [ 9 ] ; real_T P_911 ; real_T
P_912 ; real_T P_913 ; real_T P_914 ; real_T P_915 ; real_T P_916 ; real_T
P_917 ; real_T P_918 ; real_T P_919 ; real_T P_920 ; real_T P_921 ; real_T
P_922 ; real_T P_923 ; real_T P_924 ; real_T P_925 ; real_T P_926 ; real_T
P_927 ; real_T P_928 ; real_T P_929 ; real_T P_930 ; real_T P_931 ; real_T
P_932 ; real_T P_933 ; real_T P_934 ; real_T P_935 ; real_T P_936 ; real_T
P_937 ; real_T P_938 ; real_T P_939 ; real_T P_940 ; real_T P_941 ; real_T
P_942 ; real_T P_943 ; real_T P_944 ; real_T P_945 ; real_T P_946 ; real_T
P_947 ; real_T P_948 ; real_T P_949 ; real_T P_950 ; real_T P_951 ; real_T
P_952 ; real_T P_953 ; real_T P_954 ; real_T P_955 ; real_T P_956 ; real_T
P_957 ; real_T P_958 ; real_T P_959 ; real_T P_960 ; real_T P_961 ; real_T
P_962 ; real_T P_963 ; real_T P_964 ; real_T P_965 ; real_T P_966 ; real_T
P_967 ; real_T P_968 ; real_T P_969 ; real_T P_970 ; real_T P_971 ; real_T
P_972 ; real_T P_973 ; real_T P_974 ; real_T P_975 ; real_T P_976 ; real_T
P_977 ; real_T P_978 ; real_T P_979 ; real_T P_980 ; real_T P_981 ; real_T
P_982 ; real_T P_983 ; real_T P_984 ; real_T P_985 ; real_T P_986 ; real_T
P_987 ; real_T P_988 ; real_T P_989 ; real_T P_990 ; real_T P_991 ; real_T
P_992 ; real_T P_993 ; real_T P_994 ; real_T P_995 ; real_T P_996 ; real_T
P_997 ; real_T P_998 ; real_T P_999 ; real_T P_1000 ; real_T P_1001 ; real_T
P_1002 ; real_T P_1003 ; real_T P_1004 ; real_T P_1005 ; real_T P_1006 ;
real_T P_1007 ; real_T P_1008 ; real_T P_1009 ; real_T P_1010 ; real_T P_1011
; real_T P_1012 ; real_T P_1013 ; real_T P_1014 ; real_T P_1015 [ 9 ] ;
real_T P_1016 ; real_T P_1017 ; real_T P_1018 ; real_T P_1019 ; real_T P_1020
; real_T P_1021 ; real_T P_1022 ; real_T P_1023 ; real_T P_1024 ; real_T
P_1025 ; real_T P_1026 ; real_T P_1027 ; real_T P_1028 ; real_T P_1029 ;
real_T P_1030 ; real_T P_1031 ; real_T P_1032 ; real_T P_1033 ; real_T P_1034
; real_T P_1035 ; real_T P_1036 ; real_T P_1037 ; real_T P_1038 ; real_T
P_1039 ; real_T P_1040 ; real_T P_1041 ; real_T P_1042 ; real_T P_1043 ;
real_T P_1044 ; real_T P_1045 ; real_T P_1046 ; real_T P_1047 ; real_T P_1048
; real_T P_1049 ; real_T P_1050 ; real_T P_1051 ; real_T P_1052 ; real_T
P_1053 ; real_T P_1054 ; real_T P_1055 ; real_T P_1056 ; real_T P_1057 ;
real_T P_1058 ; real_T P_1059 ; real_T P_1060 ; real_T P_1061 ; real_T P_1062
; real_T P_1063 ; real_T P_1064 ; real_T P_1065 ; real_T P_1066 ; real_T
P_1067 ; real_T P_1068 ; real_T P_1069 ; real_T P_1070 ; real_T P_1071 ;
real_T P_1072 ; real_T P_1073 ; real_T P_1074 ; real_T P_1075 ; real_T P_1076
; real_T P_1077 ; real_T P_1078 ; real_T P_1079 ; real_T P_1080 ; real_T
P_1081 ; real_T P_1082 ; real_T P_1083 ; real_T P_1084 ; real_T P_1085 ;
real_T P_1086 ; real_T P_1087 ; real_T P_1088 ; real_T P_1089 ; real_T P_1090
; real_T P_1091 ; real_T P_1092 ; real_T P_1093 ; real_T P_1094 ; real_T
P_1095 ; real_T P_1096 ; real_T P_1097 ; real_T P_1098 ; real_T P_1099 ;
real_T P_1100 ; real_T P_1101 ; real_T P_1102 ; real_T P_1103 ; real_T P_1104
; real_T P_1105 ; real_T P_1106 ; real_T P_1107 ; real_T P_1108 ; real_T
P_1109 ; real_T P_1110 ; real_T P_1111 ; real_T P_1112 ; real_T P_1113 ;
real_T P_1114 ; real_T P_1115 ; real_T P_1116 ; real_T P_1117 ; real_T P_1118
; real_T P_1119 ; real_T P_1120 [ 9 ] ; real_T P_1121 ; real_T P_1122 ;
real_T P_1123 ; real_T P_1124 ; real_T P_1125 ; real_T P_1126 ; real_T P_1127
; real_T P_1128 ; real_T P_1129 ; real_T P_1130 ; real_T P_1131 ; real_T
P_1132 ; real_T P_1133 ; real_T P_1134 ; real_T P_1135 ; real_T P_1136 ;
real_T P_1137 ; real_T P_1138 ; real_T P_1139 ; real_T P_1140 ; real_T P_1141
; real_T P_1142 ; real_T P_1143 ; real_T P_1144 ; real_T P_1145 ; real_T
P_1146 ; real_T P_1147 ; real_T P_1148 ; real_T P_1149 ; real_T P_1150 ;
real_T P_1151 ; real_T P_1152 ; real_T P_1153 ; real_T P_1154 ; real_T P_1155
; real_T P_1156 ; real_T P_1157 ; real_T P_1158 ; real_T P_1159 ; real_T
P_1160 ; real_T P_1161 ; real_T P_1162 ; real_T P_1163 ; real_T P_1164 ;
real_T P_1165 ; real_T P_1166 ; real_T P_1167 ; real_T P_1168 ; real_T P_1169
; real_T P_1170 ; real_T P_1171 ; real_T P_1172 ; real_T P_1173 ; real_T
P_1174 ; real_T P_1175 ; real_T P_1176 ; real_T P_1177 ; real_T P_1178 ;
real_T P_1179 ; real_T P_1180 ; real_T P_1181 ; real_T P_1182 ; real_T P_1183
; real_T P_1184 ; real_T P_1185 ; real_T P_1186 ; real_T P_1187 ; real_T
P_1188 ; real_T P_1189 ; real_T P_1190 ; real_T P_1191 ; real_T P_1192 ;
real_T P_1193 ; real_T P_1194 ; real_T P_1195 ; real_T P_1196 ; real_T P_1197
; real_T P_1198 ; real_T P_1199 ; real_T P_1200 ; real_T P_1201 ; real_T
P_1202 ; real_T P_1203 ; real_T P_1204 ; real_T P_1205 ; real_T P_1206 ;
real_T P_1207 ; real_T P_1208 ; real_T P_1209 ; real_T P_1210 ; real_T P_1211
; real_T P_1212 ; real_T P_1213 ; real_T P_1214 ; real_T P_1215 ; real_T
P_1216 ; real_T P_1217 ; real_T P_1218 ; real_T P_1219 ; real_T P_1220 ;
real_T P_1221 ; real_T P_1222 ; real_T P_1223 ; real_T P_1224 ; real_T P_1225
; real_T P_1226 ; real_T P_1227 ; real_T P_1228 ; real_T P_1229 ; real_T
P_1230 ; real_T P_1231 ; real_T P_1232 ; real_T P_1233 ; real_T P_1234 ;
real_T P_1235 ; real_T P_1236 ; real_T P_1237 ; real_T P_1238 ; real_T P_1239
; real_T P_1240 ; real_T P_1241 ; real_T P_1242 ; real_T P_1243 ; real_T
P_1244 ; real_T P_1245 ; real_T P_1246 ; real_T P_1247 ; real_T P_1248 ;
real_T P_1249 ; real_T P_1250 ; real_T P_1251 ; real_T P_1252 ; real_T P_1253
; real_T P_1254 ; real_T P_1255 ; real_T P_1256 ; real_T P_1257 ; real_T
P_1258 ; real_T P_1259 ; real_T P_1260 ; real_T P_1261 ; real_T P_1262 ;
real_T P_1263 ; real_T P_1264 ; real_T P_1265 ; real_T P_1266 ; real_T P_1267
; real_T P_1268 ; real_T P_1269 ; real_T P_1270 ; real_T P_1271 ; real_T
P_1272 ; real_T P_1273 ; real_T P_1274 ; real_T P_1275 ; real_T P_1276 ;
real_T P_1277 ; real_T P_1278 ; real_T P_1279 ; real_T P_1280 ; real_T P_1281
; real_T P_1282 ; real_T P_1283 ; real_T P_1284 ; real_T P_1285 ; real_T
P_1286 ; real_T P_1287 ; real_T P_1288 ; real_T P_1289 ; real_T P_1290 ;
real_T P_1291 ; real_T P_1292 ; real_T P_1293 ; real_T P_1294 ; real_T P_1295
; real_T P_1296 ; real_T P_1297 ; real_T P_1298 ; real_T P_1299 ; real_T
P_1300 ; real_T P_1301 ; real_T P_1302 ; real_T P_1303 ; real_T P_1304 ;
real_T P_1305 ; real_T P_1306 ; real_T P_1307 ; real_T P_1308 ; real_T P_1309
; real_T P_1310 ; real_T P_1311 ; real_T P_1312 ; real_T P_1313 ; real_T
P_1314 ; real_T P_1315 ; real_T P_1316 ; real_T P_1317 ; real_T P_1318 ;
real_T P_1319 ; real_T P_1320 ; real_T P_1321 ; real_T P_1322 ; real_T P_1323
; real_T P_1324 ; real_T P_1325 ; real_T P_1326 ; real_T P_1327 ; real_T
P_1328 ; real_T P_1329 ; real_T P_1330 ; real_T P_1331 ; real_T P_1332 ;
real_T P_1333 ; real_T P_1334 ; real_T P_1335 ; real_T P_1336 ; real_T P_1337
; real_T P_1338 ; real_T P_1339 ; real_T P_1340 ; real_T P_1341 ; real_T
P_1342 ; real_T P_1343 ; real_T P_1344 ; real_T P_1345 ; real_T P_1346 ;
real_T P_1347 ; real_T P_1348 ; real_T P_1349 ; real_T P_1350 ; real_T P_1351
; real_T P_1352 ; real_T P_1353 ; real_T P_1354 ; real_T P_1355 ; real_T
P_1356 ; real_T P_1357 ; real_T P_1358 ; real_T P_1359 ; real_T P_1360 ;
real_T P_1361 ; real_T P_1362 ; real_T P_1363 ; real_T P_1364 ; real_T P_1365
; real_T P_1366 ; real_T P_1367 ; real_T P_1368 ; real_T P_1369 ; real_T
P_1370 ; real_T P_1371 ; real_T P_1372 ; real_T P_1373 ; real_T P_1374 ;
real_T P_1375 ; real_T P_1376 ; real_T P_1377 ; real_T P_1378 ; real_T P_1379
; real_T P_1380 ; real_T P_1381 ; real_T P_1382 ; real_T P_1383 ; real_T
P_1384 ; real_T P_1385 ; real_T P_1386 ; real_T P_1387 ; real_T P_1388 ;
real_T P_1389 ; real_T P_1390 ; real_T P_1391 ; real_T P_1392 ; real_T P_1393
; real_T P_1394 ; real_T P_1395 ; real_T P_1396 ; real_T P_1397 ; real_T
P_1398 ; real_T P_1399 ; real_T P_1400 ; real_T P_1401 ; real_T P_1402 ;
real_T P_1403 ; real_T P_1404 ; real_T P_1405 ; real_T P_1406 ; real_T P_1407
; real_T P_1408 ; real_T P_1409 ; real_T P_1410 ; real_T P_1411 ; real_T
P_1412 ; real_T P_1413 ; real_T P_1414 ; real_T P_1415 ; real_T P_1416 ;
real_T P_1417 ; real_T P_1418 ; real_T P_1419 ; real_T P_1420 ; real_T P_1421
; real_T P_1422 ; real_T P_1423 ; real_T P_1424 ; real_T P_1425 ; real_T
P_1426 ; real_T P_1427 ; real_T P_1428 ; real_T P_1429 ; real_T P_1430 ;
real_T P_1431 ; real_T P_1432 ; real_T P_1433 ; real_T P_1434 ; real_T P_1435
; real_T P_1436 ; real_T P_1437 ; real_T P_1438 ; real_T P_1439 ; real_T
P_1440 ; real_T P_1441 ; real_T P_1442 ; real_T P_1443 ; real_T P_1444 ;
real_T P_1445 ; real_T P_1446 ; real_T P_1447 ; real_T P_1448 ; real_T P_1449
; real_T P_1450 ; real_T P_1451 ; real_T P_1452 ; real_T P_1453 ; real_T
P_1454 ; real_T P_1455 ; real_T P_1456 ; real_T P_1457 ; real_T P_1458 ;
real_T P_1459 ; real_T P_1460 ; real_T P_1461 ; real_T P_1462 ; real_T P_1463
; real_T P_1464 ; real_T P_1465 ; real_T P_1466 [ 4 ] ; real_T P_1467 [ 4 ] ;
real_T P_1468 ; real_T P_1469 ; real_T P_1470 ; real_T P_1471 ; real_T P_1472
[ 5 ] ; real_T P_1473 [ 5 ] ; real_T P_1474 ; real_T P_1475 ; real_T P_1476 ;
real_T P_1477 ; real_T P_1478 ; real_T P_1479 [ 5 ] ; real_T P_1480 [ 5 ] ;
real_T P_1481 ; real_T P_1482 ; real_T P_1483 ; real_T P_1484 ; real_T P_1485
; real_T P_1486 [ 5 ] ; real_T P_1487 [ 5 ] ; real_T P_1488 ; real_T P_1489 ;
real_T P_1490 ; real_T P_1491 ; real_T P_1492 [ 4 ] ; real_T P_1493 [ 4 ] ;
real_T P_1494 ; real_T P_1495 ; real_T P_1496 ; real_T P_1497 ; real_T P_1498
[ 5 ] ; real_T P_1499 [ 5 ] ; real_T P_1500 ; real_T P_1501 ; real_T P_1502 ;
real_T P_1503 ; real_T P_1504 ; real_T P_1505 [ 5 ] ; real_T P_1506 [ 5 ] ;
real_T P_1507 ; real_T P_1508 ; real_T P_1509 ; real_T P_1510 ; real_T P_1511
; real_T P_1512 [ 5 ] ; real_T P_1513 [ 5 ] ; real_T P_1514 ; real_T P_1515 ;
real_T P_1516 ; real_T P_1517 ; real_T P_1518 [ 4 ] ; real_T P_1519 [ 4 ] ;
real_T P_1520 ; real_T P_1521 ; real_T P_1522 ; real_T P_1523 ; real_T P_1524
[ 5 ] ; real_T P_1525 [ 5 ] ; real_T P_1526 ; real_T P_1527 ; real_T P_1528 ;
real_T P_1529 ; real_T P_1530 ; real_T P_1531 [ 5 ] ; real_T P_1532 [ 5 ] ;
real_T P_1533 ; real_T P_1534 ; real_T P_1535 ; real_T P_1536 ; real_T P_1537
; real_T P_1538 [ 5 ] ; real_T P_1539 [ 5 ] ; real_T P_1540 ; real_T P_1541 ;
real_T P_1542 ; real_T P_1543 ; real_T P_1544 [ 4 ] ; real_T P_1545 [ 4 ] ;
real_T P_1546 ; real_T P_1547 ; real_T P_1548 ; real_T P_1549 ; real_T P_1550
[ 5 ] ; real_T P_1551 [ 5 ] ; real_T P_1552 ; real_T P_1553 ; real_T P_1554 ;
real_T P_1555 ; real_T P_1556 ; real_T P_1557 [ 5 ] ; real_T P_1558 [ 5 ] ;
real_T P_1559 ; real_T P_1560 ; real_T P_1561 ; real_T P_1562 ; real_T P_1563
; real_T P_1564 [ 5 ] ; real_T P_1565 [ 5 ] ; real_T P_1566 ; real_T P_1567 ;
real_T P_1568 ; real_T P_1569 ; real_T P_1570 ; real_T P_1571 ; real_T P_1572
; real_T P_1573 [ 4 ] ; real_T P_1574 [ 4 ] ; real_T P_1575 ; real_T P_1576 ;
real_T P_1577 ; real_T P_1578 ; real_T P_1579 [ 5 ] ; real_T P_1580 [ 5 ] ;
real_T P_1581 ; real_T P_1582 ; real_T P_1583 ; real_T P_1584 ; real_T P_1585
; real_T P_1586 [ 5 ] ; real_T P_1587 [ 5 ] ; real_T P_1588 ; real_T P_1589 ;
real_T P_1590 ; real_T P_1591 ; real_T P_1592 ; real_T P_1593 [ 5 ] ; real_T
P_1594 [ 5 ] ; real_T P_1595 ; real_T P_1596 ; real_T P_1597 ;
P_TrueRMS_normal_operation_no_faults_T TrueRMS ;
P_RMS_normal_operation_no_faults_T RMS ;
P_Signalgenerator_normal_operation_no_faults_T Signalgenerator_j ;
P_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_ca ;
P_Signalgenerator_normal_operation_no_faults_T Signalgenerator_m ;
P_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_a ;
P_Signalgenerator_normal_operation_no_faults_T Signalgenerator_e ;
P_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_b ;
P_Signalgenerator_normal_operation_no_faults_T Signalgenerator_p ;
P_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_c ;
P_Signalgenerator_normal_operation_no_faults_T Signalgenerator_d ;
P_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator_d ;
P_Signalgenerator_normal_operation_no_faults_T Signalgenerator ;
P_HarmonicGenerator_normal_operation_no_faults_T HarmonicGenerator ;
P_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_fb ; P_Subsystem1_normal_operation_no_faults_j_T
Subsystem1_p ; P_Subsystempi2delay_normal_operation_no_faults_g_T
Subsystempi2delay_en ; P_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl_a ; P_Subsystem1_normal_operation_no_faults_j_T
Subsystem1_l ; P_Subsystempi2delay_normal_operation_no_faults_g_T
Subsystempi2delay_j ; P_Subsystem1_normal_operation_no_faults_j_T
Subsystem1_jf ; P_Subsystempi2delay_normal_operation_no_faults_g_T
Subsystempi2delay_b3 ; P_Subsystem1_normal_operation_no_faults_h_T
Subsystem1_e ; P_Subsystempi2delay_normal_operation_no_faults_l_T
Subsystempi2delay_e ;
P_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_k ;
P_ACVoltageController_normal_operation_no_faults_T ACVoltageController_l ;
P_ZeroSequenceController_normal_operation_no_faults_T
ZeroSequenceController2_f ; P_Subsystem1_normal_operation_no_faults_j_T
Subsystem1_h1 ; P_Subsystempi2delay_normal_operation_no_faults_g_T
Subsystempi2delay_h4 ; P_AutomaticGainControl_normal_operation_no_faults_T
AutomaticGainControl_d ; P_Subsystem1_normal_operation_no_faults_j_T
Subsystem1_j ; P_Subsystempi2delay_normal_operation_no_faults_g_T
Subsystempi2delay_nu ; P_Subsystem1_normal_operation_no_faults_j_T
Subsystem1_c ; P_Subsystempi2delay_normal_operation_no_faults_g_T
Subsystempi2delay_n ; P_Subsystem1_normal_operation_no_faults_h_T
Subsystem1_g ; P_Subsystempi2delay_normal_operation_no_faults_l_T
Subsystempi2delay_h ;
P_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression_d ;
P_ACVoltageController_normal_operation_no_faults_T ACVoltageController_n ;
P_ZeroSequenceController_normal_operation_no_faults_T ZeroSequenceController2
; P_Subsystem1_normal_operation_no_faults_j_T Subsystem1_f ;
P_Subsystempi2delay_normal_operation_no_faults_g_T Subsystempi2delay_i ;
P_AutomaticGainControl_normal_operation_no_faults_T AutomaticGainControl ;
P_Subsystem1_normal_operation_no_faults_j_T Subsystem1_b ;
P_Subsystempi2delay_normal_operation_no_faults_g_T Subsystempi2delay_m1 ;
P_Subsystem1_normal_operation_no_faults_j_T Subsystem1_h ;
P_Subsystempi2delay_normal_operation_no_faults_g_T Subsystempi2delay_m ;
P_Subsystem1_normal_operation_no_faults_h_T Subsystem1_i3 ;
P_Subsystempi2delay_normal_operation_no_faults_l_T Subsystempi2delay_pq ;
P_Circulatingcurrentsuppression_normal_operation_no_faults_T
Circulatingcurrentsuppression ;
P_ACVoltageController_normal_operation_no_faults_T ACVoltageController ;
P_ZeroSequenceController_normal_operation_no_faults_T ZeroSequenceController
; P_Subsystem1_normal_operation_no_faults_T Subsystem1_i ;
P_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay_b ;
P_Subsystem1_normal_operation_no_faults_T Subsystem1_d ;
P_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay_p ;
P_Subsystem1_normal_operation_no_faults_h_T Subsystem1 ;
P_Subsystempi2delay_normal_operation_no_faults_l_T Subsystempi2delay ;
P_Subsystem1_normal_operation_no_faults_g_T Subsystem1_go ;
P_Subsystempi2delay_normal_operation_no_faults_h_T Subsystempi2delay_g ;
P_Subsystem1_normal_operation_no_faults_T Subsystem1_pi ;
P_Subsystempi2delay_normal_operation_no_faults_T Subsystempi2delay_bk ; } ;
extern P_normal_operation_no_faults_T normal_operation_no_faults_rtDefaultP ;
extern const ConstB_normal_operation_no_faults_T
normal_operation_no_faults_rtInvariant ;
#endif
