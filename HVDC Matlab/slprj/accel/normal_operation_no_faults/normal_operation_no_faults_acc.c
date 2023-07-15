#include "__cf_normal_operation_no_faults.h"
#include <math.h>
#include "normal_operation_no_faults_acc.h"
#include "normal_operation_no_faults_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T
* bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * tBufPtr , real_T * * uBufPtr , real_T * * xBufPtr ,
boolean_T isfixedbuf , boolean_T istransportdelay , int_T * maxNewBufSzPtr )
{ int_T testIdx ; int_T tail = * tailPtr ; int_T bufSz = * bufSzPtr ; real_T
* tBuf = * tBufPtr ; real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if (
istransportdelay ) { numBuffer = 3 ; xBuf = * xBufPtr ; } testIdx = ( tail <
( bufSz - 1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] )
&& ! isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T *
tempX = ( NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024
; if ( newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU
= ( real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if (
tempU == ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * tBufPtr = tempT ; * uBufPtr = tempU ; if (
istransportdelay ) * xBufPtr = tempX ; } else { * tailPtr = testIdx ; }
return ( true ) ; } real_T
normal_operation_no_faults_acc_rt_TDelayInterpolate ( real_T tMinusDelay ,
real_T tStart , real_T * tBuf , real_T * uBuf , int_T bufSz , int_T * lastIdx
, int_T oldestIdx , int_T newIdx , real_T initOutput , boolean_T discrete ,
boolean_T minorStepAndTAtLastMajorOutput ) { int_T i ; real_T yout , t1 , t2
, u1 , u2 ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 ) && ( tMinusDelay >
tStart ) ) return initOutput ; if ( tMinusDelay <= tStart ) return initOutput
; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if ( discrete ) { return (
uBuf [ oldestIdx ] ) ; } else { int_T tempIdx = oldestIdx + 1 ; if (
oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [ oldestIdx ] ; t2 = tBuf [
tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [ tempIdx ] ; if ( t2 == t1 )
{ if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } return yout ; } } if ( minorStepAndTAtLastMajorOutput
) { if ( newIdx != 0 ) { if ( * lastIdx == newIdx ) { ( * lastIdx ) -- ; }
newIdx -- ; } else { if ( * lastIdx == newIdx ) { * lastIdx = bufSz - 1 ; }
newIdx = bufSz - 1 ; } } i = * lastIdx ; if ( tBuf [ i ] < tMinusDelay ) {
while ( tBuf [ i ] < tMinusDelay ) { if ( i == newIdx ) break ; i = ( i < (
bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while ( tBuf [ i ] >= tMinusDelay
) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i = ( i < ( bufSz - 1 ) ) ? ( i
+ 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) { double tempEps = (
DBL_EPSILON ) * 128.0 ; double localEps = tempEps * muDoubleScalarAbs ( tBuf
[ i ] ) ; if ( tempEps > localEps ) { localEps = tempEps ; } localEps =
localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] - localEps ) ) { yout =
uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [ bufSz - 1 ] ; } else {
yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) { t1 = tBuf [ bufSz - 1 ]
; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i - 1 ] ; u1 = uBuf [ i - 1
] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 == t1 ) { if ( tMinusDelay
>= t2 ) { yout = u2 ; } else { yout = u1 ; } } else { real_T f1 = ( t2 -
tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2
; } } return ( yout ) ; } real_T
normal_operation_no_faults_acc_rt_VTDelayfindtDInterpolate ( real_T x ,
real_T * tBuf , real_T * uBuf , real_T * xBuf , int_T bufSz , int_T head ,
int_T tail , int_T * pLast , real_T t , real_T tStart , boolean_T discrete ,
boolean_T minorStepAndTAtLastMajorOutput , real_T initOutput , real_T *
appliedDelay ) { int_T n , k ; real_T f ; int_T kp1 ; real_T tminustD , tL ,
tR , uD , uL , uR , fU ; if ( minorStepAndTAtLastMajorOutput ) { if ( * pLast
== head ) { * pLast = ( * pLast == 0 ) ? bufSz - 1 : * pLast - 1 ; } head = (
head == 0 ) ? bufSz - 1 : head - 1 ; } if ( x <= 1 ) { return initOutput ; }
k = * pLast ; n = 0 ; for ( ; ; ) { n ++ ; if ( n > bufSz ) break ; if ( x -
xBuf [ k ] > 1.0 ) { if ( k == head ) { int_T km1 ; f = ( x - 1.0 - xBuf [ k
] ) / ( x - xBuf [ k ] ) ; tminustD = ( 1.0 - f ) * tBuf [ k ] + f * t ; km1
= k - 1 ; if ( km1 < 0 ) km1 = bufSz - 1 ; tL = tBuf [ km1 ] ; tR = tBuf [ k
] ; uL = uBuf [ km1 ] ; uR = uBuf [ k ] ; break ; } kp1 = k + 1 ; if ( kp1 ==
bufSz ) kp1 = 0 ; if ( x - xBuf [ kp1 ] <= 1.0 ) { f = ( x - 1.0 - xBuf [ k ]
) / ( xBuf [ kp1 ] - xBuf [ k ] ) ; tL = tBuf [ k ] ; tR = tBuf [ kp1 ] ; uL
= uBuf [ k ] ; uR = uBuf [ kp1 ] ; tminustD = ( 1.0 - f ) * tL + f * tR ;
break ; } k = kp1 ; } else { if ( k == tail ) { f = ( x - 1.0 ) / xBuf [ k ]
; if ( discrete ) { return ( uBuf [ tail ] ) ; } kp1 = k + 1 ; if ( kp1 ==
bufSz ) kp1 = 0 ; tminustD = ( 1 - f ) * tStart + f * tBuf [ k ] ; tL = tBuf
[ k ] ; tR = tBuf [ kp1 ] ; uL = uBuf [ k ] ; uR = uBuf [ kp1 ] ; break ; } k
= k - 1 ; if ( k < 0 ) k = bufSz - 1 ; } } * pLast = k ; if ( tR == tL ) { fU
= 1.0 ; } else { fU = ( tminustD - tL ) / ( tR - tL ) ; } if ( discrete ) {
uD = ( fU > ( 1.0 - fU ) ) ? uR : uL ; } else { uD = ( 1.0 - fU ) * uL + fU *
uR ; } * appliedDelay = t - tminustD ; return uD ; } void
normal_operation_no_faults_acc_BINARYSEARCH_real_T ( uint32_T * piLeft ,
uint32_T * piRght , real_T u , const real_T * pData , uint32_T iHi ) { *
piLeft = 0U ; * piRght = iHi ; if ( u <= pData [ 0 ] ) { * piRght = 0U ; }
else if ( u >= pData [ iHi ] ) { * piLeft = iHi ; } else { uint32_T i ; while
( ( * piRght - * piLeft ) > 1U ) { i = ( * piLeft + * piRght ) >> 1 ; if ( u
< pData [ i ] ) { * piRght = i ; } else { * piLeft = i ; } } } } void
normal_operation_no_faults_acc_LookUp_real_T_real_T ( real_T * pY , const
real_T * pYData , real_T u , const real_T * pUData , uint32_T iHi ) {
uint32_T iLeft ; uint32_T iRght ;
normal_operation_no_faults_acc_BINARYSEARCH_real_T ( & ( iLeft ) , & ( iRght
) , u , pUData , iHi ) ; { real_T lambda ; if ( pUData [ iRght ] > pUData [
iLeft ] ) { real_T num ; real_T den ; den = pUData [ iRght ] ; den -= pUData
[ iLeft ] ; num = u ; num -= pUData [ iLeft ] ; lambda = num / den ; } else {
lambda = 0.0 ; } { real_T yLeftCast ; real_T yRghtCast ; yLeftCast = pYData [
iLeft ] ; yRghtCast = pYData [ iRght ] ; yLeftCast += lambda * ( yRghtCast -
yLeftCast ) ; ( * pY ) = yLeftCast ; } } } real_T look1_binlxpw ( real_T u0 ,
const real_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) { real_T
frac ; uint32_T iRght ; uint32_T iLeft ; uint32_T bpIdx ; if ( u0 <= bp0 [ 0U
] ) { iLeft = 0U ; frac = ( u0 - bp0 [ 0U ] ) / ( bp0 [ 1U ] - bp0 [ 0U ] ) ;
} else if ( u0 < bp0 [ maxIndex ] ) { bpIdx = maxIndex >> 1U ; iLeft = 0U ;
iRght = maxIndex ; while ( iRght - iLeft > 1U ) { if ( u0 < bp0 [ bpIdx ] ) {
iRght = bpIdx ; } else { iLeft = bpIdx ; } bpIdx = ( iRght + iLeft ) >> 1U ;
} frac = ( u0 - bp0 [ iLeft ] ) / ( bp0 [ iLeft + 1U ] - bp0 [ iLeft ] ) ; }
else { iLeft = maxIndex - 1U ; frac = ( u0 - bp0 [ maxIndex - 1U ] ) / ( bp0
[ maxIndex ] - bp0 [ maxIndex - 1U ] ) ; } return ( table [ iLeft + 1U ] -
table [ iLeft ] ) * frac + table [ iLeft ] ; } void rt_ssGetBlockPath (
SimStruct * S , int_T sysIdx , int_T blkIdx , char_T * * path ) {
_ssGetBlockPath ( S , sysIdx , blkIdx , path ) ; } void rt_ssSet_slErrMsg (
SimStruct * S , void * diag ) { if ( ! _ssIsErrorStatusAslErrMsg ( S ) ) {
_ssSet_slErrMsg ( S , diag ) ; } } void rt_ssReportDiagnosticAsWarning (
SimStruct * S , void * diag ) { _ssReportDiagnosticAsWarning ( S , diag ) ; }
void normal_operation_no_faults_Subsystempi2delay_Init ( SimStruct * S ,
B_Subsystempi2delay_normal_operation_no_faults_T * localB ,
P_Subsystempi2delay_normal_operation_no_faults_T * localP ) { localB ->
B_0_0_0 = localP -> P_0 [ 0 ] ; localB -> B_0_1_0 = localP -> P_0 [ 1 ] ; }
void normal_operation_no_faults_Subsystempi2delay ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_normal_operation_no_faults_T * localB ,
DW_Subsystempi2delay_normal_operation_no_faults_T * localDW ) { if (
rtu_Enable > 0 ) { localB -> B_0_0_0 = rtu_alpha_beta [ 0 ] *
muDoubleScalarSin ( rtu_wt ) - rtu_alpha_beta [ 1 ] * muDoubleScalarCos (
rtu_wt ) ; localB -> B_0_1_0 = rtu_alpha_beta [ 0 ] * muDoubleScalarCos (
rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarSin ( rtu_wt ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Subsystempi2delay_SubsysRanBC ) ; } } } void
normal_operation_no_faults_Subsystempi2delay_Term ( SimStruct * const S ) { }
void normal_operation_no_faults_Subsystem1_Init ( SimStruct * S ,
B_Subsystem1_normal_operation_no_faults_T * localB ,
P_Subsystem1_normal_operation_no_faults_T * localP ) { localB -> B_1_0_0 =
localP -> P_0 [ 0 ] ; localB -> B_1_1_0 = localP -> P_0 [ 1 ] ; } void
normal_operation_no_faults_Subsystem1 ( SimStruct * S , uint8_T rtu_Enable ,
const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystem1_normal_operation_no_faults_T * localB ,
DW_Subsystem1_normal_operation_no_faults_T * localDW ) { if ( rtu_Enable > 0
) { localB -> B_1_0_0 = rtu_alpha_beta [ 0 ] * muDoubleScalarCos ( rtu_wt ) +
rtu_alpha_beta [ 1 ] * muDoubleScalarSin ( rtu_wt ) ; localB -> B_1_1_0 = -
rtu_alpha_beta [ 0 ] * muDoubleScalarSin ( rtu_wt ) + rtu_alpha_beta [ 1 ] *
muDoubleScalarCos ( rtu_wt ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( localDW -> Subsystem1_SubsysRanBC ) ; } } } void
normal_operation_no_faults_Subsystem1_Term ( SimStruct * const S ) { } void
normal_operation_no_faults_Subsystempi2delay_m_Init ( SimStruct * S ,
B_Subsystempi2delay_normal_operation_no_faults_p_T * localB ,
P_Subsystempi2delay_normal_operation_no_faults_h_T * localP ) { localB ->
B_2_0_0 = localP -> P_0 [ 0 ] ; localB -> B_2_1_0 = localP -> P_0 [ 1 ] ; }
void normal_operation_no_faults_Subsystempi2delay_Disable ( SimStruct * S ,
DW_Subsystempi2delay_normal_operation_no_faults_i_T * localDW ) { localDW ->
Subsystempi2delay_MODE = false ; } void
normal_operation_no_faults_Subsystempi2delay_g ( SimStruct * S , uint8_T
rtu_Enable , real_T rtu_dq , real_T rtu_dq_c , real_T rtu_wt ,
B_Subsystempi2delay_normal_operation_no_faults_p_T * localB ,
DW_Subsystempi2delay_normal_operation_no_faults_i_T * localDW ) { int32_T
isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0 ) { if ( ! localDW ->
Subsystempi2delay_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S )
) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Subsystempi2delay_MODE = true ; } } else { if ( localDW ->
Subsystempi2delay_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
normal_operation_no_faults_Subsystempi2delay_Disable ( S , localDW ) ; } } }
if ( localDW -> Subsystempi2delay_MODE ) { localB -> B_2_0_0 = rtu_dq *
muDoubleScalarSin ( rtu_wt ) + rtu_dq_c * muDoubleScalarCos ( rtu_wt ) ;
localB -> B_2_1_0 = - rtu_dq * muDoubleScalarCos ( rtu_wt ) + rtu_dq_c *
muDoubleScalarSin ( rtu_wt ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( localDW -> Subsystempi2delay_SubsysRanBC ) ; } } } void
normal_operation_no_faults_Subsystempi2delay_l_Term ( SimStruct * const S ) {
} void normal_operation_no_faults_Subsystem1_e_Init ( SimStruct * S ,
B_Subsystem1_normal_operation_no_faults_l_T * localB ,
P_Subsystem1_normal_operation_no_faults_g_T * localP ) { localB -> B_3_0_0 =
localP -> P_0 [ 0 ] ; localB -> B_3_1_0 = localP -> P_0 [ 1 ] ; } void
normal_operation_no_faults_Subsystem1_Disable ( SimStruct * S ,
DW_Subsystem1_normal_operation_no_faults_a_T * localDW ) { localDW ->
Subsystem1_MODE = false ; } void normal_operation_no_faults_Subsystem1_g (
SimStruct * S , uint8_T rtu_Enable , real_T rtu_dq , real_T rtu_dq_n , real_T
rtu_wt , B_Subsystem1_normal_operation_no_faults_l_T * localB ,
DW_Subsystem1_normal_operation_no_faults_a_T * localDW ) { int32_T isHit ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0 ) { if ( ! localDW ->
Subsystem1_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Subsystem1_MODE = true ; } } else { if ( localDW -> Subsystem1_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
normal_operation_no_faults_Subsystem1_Disable ( S , localDW ) ; } } } if (
localDW -> Subsystem1_MODE ) { localB -> B_3_0_0 = rtu_dq * muDoubleScalarCos
( rtu_wt ) - rtu_dq_n * muDoubleScalarSin ( rtu_wt ) ; localB -> B_3_1_0 =
rtu_dq * muDoubleScalarSin ( rtu_wt ) + rtu_dq_n * muDoubleScalarCos ( rtu_wt
) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Subsystem1_SubsysRanBC ) ; } } } void
normal_operation_no_faults_Subsystem1_a_Term ( SimStruct * const S ) { } void
normal_operation_no_faults_Subsystempi2delay_j_Init ( SimStruct * S ,
B_Subsystempi2delay_normal_operation_no_faults_b_T * localB ,
P_Subsystempi2delay_normal_operation_no_faults_l_T * localP ) { localB ->
B_5_0_0 = localP -> P_0 [ 0 ] ; localB -> B_5_1_0 = localP -> P_0 [ 1 ] ; }
void normal_operation_no_faults_Subsystempi2delay_e_Disable ( SimStruct * S ,
DW_Subsystempi2delay_normal_operation_no_faults_k_T * localDW ) { localDW ->
Subsystempi2delay_MODE = false ; } void
normal_operation_no_faults_Subsystempi2delay_e ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_dq [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_normal_operation_no_faults_b_T * localB ,
DW_Subsystempi2delay_normal_operation_no_faults_k_T * localDW ) { int32_T
isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0 ) { if ( ! localDW ->
Subsystempi2delay_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S )
) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Subsystempi2delay_MODE = true ; } } else { if ( localDW ->
Subsystempi2delay_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
normal_operation_no_faults_Subsystempi2delay_e_Disable ( S , localDW ) ; } }
} if ( localDW -> Subsystempi2delay_MODE ) { localB -> B_5_0_0 = rtu_dq [ 0 ]
* muDoubleScalarSin ( rtu_wt ) + rtu_dq [ 1 ] * muDoubleScalarCos ( rtu_wt )
; localB -> B_5_1_0 = - rtu_dq [ 0 ] * muDoubleScalarCos ( rtu_wt ) + rtu_dq
[ 1 ] * muDoubleScalarSin ( rtu_wt ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( localDW -> Subsystempi2delay_SubsysRanBC ) ; } } } void
normal_operation_no_faults_Subsystempi2delay_e_Term ( SimStruct * const S ) {
} void normal_operation_no_faults_Subsystem1_k_Init ( SimStruct * S ,
B_Subsystem1_normal_operation_no_faults_o_T * localB ,
P_Subsystem1_normal_operation_no_faults_h_T * localP ) { localB -> B_6_0_0 =
localP -> P_0 [ 0 ] ; localB -> B_6_1_0 = localP -> P_0 [ 1 ] ; } void
normal_operation_no_faults_Subsystem1_c_Disable ( SimStruct * S ,
DW_Subsystem1_normal_operation_no_faults_p_T * localDW ) { localDW ->
Subsystem1_MODE = false ; } void normal_operation_no_faults_Subsystem1_h (
SimStruct * S , uint8_T rtu_Enable , const real_T rtu_dq [ 2 ] , real_T
rtu_wt , B_Subsystem1_normal_operation_no_faults_o_T * localB ,
DW_Subsystem1_normal_operation_no_faults_p_T * localDW ) { int32_T isHit ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0 ) { if ( ! localDW ->
Subsystem1_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Subsystem1_MODE = true ; } } else { if ( localDW -> Subsystem1_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
normal_operation_no_faults_Subsystem1_c_Disable ( S , localDW ) ; } } } if (
localDW -> Subsystem1_MODE ) { localB -> B_6_0_0 = rtu_dq [ 0 ] *
muDoubleScalarCos ( rtu_wt ) - rtu_dq [ 1 ] * muDoubleScalarSin ( rtu_wt ) ;
localB -> B_6_1_0 = rtu_dq [ 0 ] * muDoubleScalarSin ( rtu_wt ) + rtu_dq [ 1
] * muDoubleScalarCos ( rtu_wt ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( localDW -> Subsystem1_SubsysRanBC ) ; } } } void
normal_operation_no_faults_Subsystem1_aw_Term ( SimStruct * const S ) { }
void normal_operation_no_faults_ZeroSequenceController_Init ( SimStruct * S ,
B_ZeroSequenceController_normal_operation_no_faults_T * localB ,
P_ZeroSequenceController_normal_operation_no_faults_T * localP ,
X_ZeroSequenceController_normal_operation_no_faults_T * localX ) { localX ->
Integrator_x1_CSTATE_p = localP -> P_7 ; localX -> Integrator_x2_CSTATE_o =
localP -> P_9 ; localX -> Integrator_x1_CSTATE_c = localP -> P_13 ; localX ->
Integrator_x2_CSTATE_l = localP -> P_15 ; localX -> TransferFcn1_CSTATE [ 0 ]
= 0.0 ; localX -> TransferFcn1_CSTATE [ 1 ] = 0.0 ; localB -> B_19_36_0 =
localP -> P_3 ; } void
normal_operation_no_faults_ZeroSequenceController_Disable ( SimStruct * S ,
DW_ZeroSequenceController_normal_operation_no_faults_T * localDW ) { localDW
-> ZeroSequenceController_MODE = false ; } void
normal_operation_no_faults_ZeroSequenceController ( SimStruct * S , real_T
rtu_V0_meas , real_T rtu_I0_meas , real_T rtu_Enable ,
B_ZeroSequenceController_normal_operation_no_faults_T * localB ,
DW_ZeroSequenceController_normal_operation_no_faults_T * localDW ,
P_ZeroSequenceController_normal_operation_no_faults_T * localP ,
X_ZeroSequenceController_normal_operation_no_faults_T * localX ) { int32_T
isHit ; real_T rtu_Enable_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if (
rtu_Enable > 0.0 ) { if ( ! localDW -> ZeroSequenceController_MODE ) { if (
ssGetTaskTime ( S , 0 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
ZeroSequenceController_MODE = true ; } } else { if ( localDW ->
ZeroSequenceController_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep (
S ) ; normal_operation_no_faults_ZeroSequenceController_Disable ( S , localDW
) ; } } } if ( localDW -> ZeroSequenceController_MODE ) { isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { if ( rtu_Enable > localP ->
P_5 ) { rtu_Enable_0 = rtu_I0_meas ; } else { rtu_Enable_0 = 0.0 ; } localB
-> B_19_2_0 = localB -> B_19_0_0 - rtu_Enable_0 ; } localB -> B_19_3_0 =
localP -> P_6 * localB -> B_19_2_0 ; localB -> B_19_4_0 = localX ->
Integrator_x1_CSTATE_p ; localB -> B_19_5_0 = localP -> P_8 * localB ->
B_19_4_0 ; localB -> B_19_6_0 = localX -> Integrator_x2_CSTATE_o ; localB ->
B_19_7_0 = localP -> P_10 * localB -> B_19_6_0 ; localB -> B_19_8_0 = localB
-> B_19_5_0 + localB -> B_19_7_0 ; localB -> B_19_9_0 = localB -> B_19_3_0 +
localB -> B_19_8_0 ; localB -> B_19_10_0 = localP -> P_11 * localB ->
B_19_9_0 ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { localB
-> B_19_11_0 = localP -> P_12 * localB -> B_19_2_0 ; } localB -> B_19_12_0 =
localX -> Integrator_x1_CSTATE_c ; localB -> B_19_13_0 = localP -> P_14 *
localB -> B_19_12_0 ; localB -> B_19_14_0 = localX -> Integrator_x2_CSTATE_l
; localB -> B_19_15_0 = localP -> P_16 * localB -> B_19_14_0 ; localB ->
B_19_16_0 = localP -> P_17 * localB -> B_19_12_0 ; localB -> B_19_17_0 =
localP -> P_18 * localB -> B_19_14_0 ; localB -> B_19_18_0 = localB ->
B_19_13_0 + localB -> B_19_15_0 ; localB -> B_19_19_0 = localB -> B_19_16_0 +
localB -> B_19_17_0 ; localB -> B_19_20_0 = localP -> P_19 * rtu_V0_meas ;
localB -> B_19_21_0 = localB -> B_19_18_0 + localB -> B_19_20_0 ; localB ->
B_19_22_0 = localP -> P_20 * rtu_V0_meas ; localB -> B_19_23_0 = localB ->
B_19_19_0 + localB -> B_19_22_0 ; localB -> B_19_24_0 = localP -> P_21 *
localB -> B_19_4_0 ; localB -> B_19_25_0 = localP -> P_22 * localB ->
B_19_6_0 ; localB -> B_19_26_0 = localP -> P_23 * localB -> B_19_4_0 ; localB
-> B_19_27_0 = localP -> P_24 * localB -> B_19_6_0 ; localB -> B_19_28_0 =
localB -> B_19_24_0 + localB -> B_19_25_0 ; localB -> B_19_29_0 = localB ->
B_19_26_0 + localB -> B_19_27_0 ; localB -> B_19_30_0 = localP -> P_25 *
localB -> B_19_2_0 ; localB -> B_19_31_0 = localB -> B_19_28_0 + localB ->
B_19_30_0 ; localB -> B_19_32_0 = localP -> P_26 * localB -> B_19_2_0 ;
localB -> B_19_33_0 = localB -> B_19_29_0 + localB -> B_19_32_0 ; localB ->
B_19_34_0 = 0.0 ; localB -> B_19_34_0 += localP -> P_28 [ 0 ] * localX ->
TransferFcn1_CSTATE [ 0 ] ; localB -> B_19_34_0 += localP -> P_28 [ 1 ] *
localX -> TransferFcn1_CSTATE [ 1 ] ; if ( rtu_Enable > localP -> P_29 ) {
localB -> B_18_0_0 = localB -> B_19_11_0 + localB -> B_19_34_0 ; localB ->
B_18_1_0 = localP -> P_0 * rtu_V0_meas ; localB -> B_18_2_0 = localP -> P_1 *
localB -> B_19_14_0 ; localB -> B_18_3_0 = localP -> P_2 * localB ->
B_19_12_0 ; localB -> B_18_4_0 = localB -> B_18_3_0 + localB -> B_18_2_0 ;
localB -> B_18_5_0 = localB -> B_18_1_0 + localB -> B_18_4_0 ; localB ->
B_18_6_0 = localB -> B_18_5_0 + localB -> B_18_0_0 ; localB -> B_19_36_0 =
localB -> B_18_6_0 ; } else { localB -> B_19_36_0 = 0.0 ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
ZeroSequenceController_SubsysRanBC ) ; } } } void
normal_operation_no_faults_ZeroSequenceControllerTID4 ( SimStruct * S ,
B_ZeroSequenceController_normal_operation_no_faults_T * localB ,
DW_ZeroSequenceController_normal_operation_no_faults_T * localDW ,
P_ZeroSequenceController_normal_operation_no_faults_T * localP ) { localB ->
B_19_0_0 = localP -> P_4 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC (
localDW -> ZeroSequenceController_SubsysRanBC ) ; } } void
normal_operation_no_faults_ZeroSequenceController_Deriv ( SimStruct * S ,
B_ZeroSequenceController_normal_operation_no_faults_T * localB ,
DW_ZeroSequenceController_normal_operation_no_faults_T * localDW ,
P_ZeroSequenceController_normal_operation_no_faults_T * localP ,
X_ZeroSequenceController_normal_operation_no_faults_T * localX ,
XDot_ZeroSequenceController_normal_operation_no_faults_T * localXdot ) { if (
localDW -> ZeroSequenceController_MODE ) { localXdot ->
Integrator_x1_CSTATE_p = localB -> B_19_31_0 ; localXdot ->
Integrator_x2_CSTATE_o = localB -> B_19_33_0 ; localXdot ->
Integrator_x1_CSTATE_c = localB -> B_19_21_0 ; localXdot ->
Integrator_x2_CSTATE_l = localB -> B_19_23_0 ; localXdot ->
TransferFcn1_CSTATE [ 0 ] = 0.0 ; localXdot -> TransferFcn1_CSTATE [ 0 ] +=
localP -> P_27 [ 0 ] * localX -> TransferFcn1_CSTATE [ 0 ] ; localXdot ->
TransferFcn1_CSTATE [ 1 ] = 0.0 ; localXdot -> TransferFcn1_CSTATE [ 0 ] +=
localP -> P_27 [ 1 ] * localX -> TransferFcn1_CSTATE [ 1 ] ; localXdot ->
TransferFcn1_CSTATE [ 1 ] += localX -> TransferFcn1_CSTATE [ 0 ] ; localXdot
-> TransferFcn1_CSTATE [ 0 ] += localB -> B_19_10_0 ; } else { { real_T * dx
; int_T i ; dx = & ( localXdot -> Integrator_x1_CSTATE_p ) ; for ( i = 0 ; i
< 6 ; i ++ ) { dx [ i ] = 0.0 ; } } } } void
normal_operation_no_faults_ZeroSequenceController_Term ( SimStruct * const S
) { } void normal_operation_no_faults_RMS_Init ( SimStruct * S ,
B_RMS_normal_operation_no_faults_T * localB ,
DW_RMS_normal_operation_no_faults_T * localDW ,
P_RMS_normal_operation_no_faults_T * localP ,
X_RMS_normal_operation_no_faults_T * localX ) { localX ->
integrator_CSTATE_kw [ 0 ] = localP -> P_3 ; localDW -> Memory_PreviousInput
[ 0 ] = localP -> P_7 ; localX -> integrator_CSTATE_l0 [ 0 ] = localP -> P_8
; localDW -> Memory_PreviousInput_o [ 0 ] = localP -> P_12 ; localB ->
B_22_22_0 [ 0 ] = localP -> P_2 ; localX -> integrator_CSTATE_kw [ 1 ] =
localP -> P_3 ; localDW -> Memory_PreviousInput [ 1 ] = localP -> P_7 ;
localX -> integrator_CSTATE_l0 [ 1 ] = localP -> P_8 ; localDW ->
Memory_PreviousInput_o [ 1 ] = localP -> P_12 ; localB -> B_22_22_0 [ 1 ] =
localP -> P_2 ; localX -> integrator_CSTATE_kw [ 2 ] = localP -> P_3 ;
localDW -> Memory_PreviousInput [ 2 ] = localP -> P_7 ; localX ->
integrator_CSTATE_l0 [ 2 ] = localP -> P_8 ; localDW ->
Memory_PreviousInput_o [ 2 ] = localP -> P_12 ; localB -> B_22_22_0 [ 2 ] =
localP -> P_2 ; } void normal_operation_no_faults_RMS_Disable ( SimStruct * S
, DW_RMS_normal_operation_no_faults_T * localDW ) { localDW -> RMS_MODE =
false ; } void normal_operation_no_faults_RMS ( SimStruct * S , boolean_T
rtu_Enable , const real_T rtu_In [ 3 ] , B_RMS_normal_operation_no_faults_T *
localB , DW_RMS_normal_operation_no_faults_T * localDW ,
P_RMS_normal_operation_no_faults_T * localP ,
X_RMS_normal_operation_no_faults_T * localX ,
XDis_RMS_normal_operation_no_faults_T * localXdis ) { boolean_T rtb_B_22_4_0
; real_T rtb_B_22_18_0 ; int32_T isHit ; isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( ( isHit != 0 ) && ( ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable
) { if ( ! localDW -> RMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) !=
ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } (
void ) memset ( & ( localXdis -> integrator_CSTATE_kw ) , 0 , 6 * sizeof (
boolean_T ) ) ; localDW -> RMS_MODE = true ; } } else { if ( localDW ->
RMS_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void )
memset ( & ( localXdis -> integrator_CSTATE_kw ) , 1 , 6 * sizeof ( boolean_T
) ) ; normal_operation_no_faults_RMS_Disable ( S , localDW ) ; } } } if (
localDW -> RMS_MODE ) { localB -> B_22_0_0 [ 0 ] = localX ->
integrator_CSTATE_kw [ 0 ] ; localB -> B_22_0_0 [ 1 ] = localX ->
integrator_CSTATE_kw [ 1 ] ; localB -> B_22_0_0 [ 2 ] = localX ->
integrator_CSTATE_kw [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & localDW
-> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 3 ] ; real_T simTime
= ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & localB ->
B_22_1_0 [ 0 ] ; int_T * iw_Tail = & localDW -> TransportDelay_IWORK . Tail [
0 ] ; int_T * iw_Head = & localDW -> TransportDelay_IWORK . Head [ 0 ] ;
int_T * iw_Last = & localDW -> TransportDelay_IWORK . Last [ 0 ] ; int_T *
iw_CircularBufSize = & localDW -> TransportDelay_IWORK . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( localP -> P_4 > 0.0 )
? localP -> P_4 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ] =
normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , localP -> P_5 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { localB -> B_22_3_0 = localP -> P_6 ; } rtb_B_22_4_0 = (
ssGetT ( S ) >= localB -> B_22_3_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ;
if ( isHit != 0 ) { localB -> B_22_5_0 [ 0 ] = localDW ->
Memory_PreviousInput [ 0 ] ; localB -> B_22_5_0 [ 1 ] = localDW ->
Memory_PreviousInput [ 1 ] ; localB -> B_22_5_0 [ 2 ] = localDW ->
Memory_PreviousInput [ 2 ] ; } if ( rtb_B_22_4_0 ) { localB -> B_21_0_0 [ 0 ]
= localB -> B_22_0_0 [ 0 ] - localB -> B_22_1_0 [ 0 ] ; localB -> B_21_1_0 [
0 ] = localP -> P_1 * localB -> B_21_0_0 [ 0 ] ; localB -> B_22_7_0 [ 0 ] =
localB -> B_21_1_0 [ 0 ] ; localB -> B_21_0_0 [ 1 ] = localB -> B_22_0_0 [ 1
] - localB -> B_22_1_0 [ 1 ] ; localB -> B_21_1_0 [ 1 ] = localP -> P_1 *
localB -> B_21_0_0 [ 1 ] ; localB -> B_22_7_0 [ 1 ] = localB -> B_21_1_0 [ 1
] ; localB -> B_21_0_0 [ 2 ] = localB -> B_22_0_0 [ 2 ] - localB -> B_22_1_0
[ 2 ] ; localB -> B_21_1_0 [ 2 ] = localP -> P_1 * localB -> B_21_0_0 [ 2 ] ;
localB -> B_22_7_0 [ 2 ] = localB -> B_21_1_0 [ 2 ] ; } else { localB ->
B_22_7_0 [ 0 ] = localB -> B_22_5_0 [ 0 ] ; localB -> B_22_7_0 [ 1 ] = localB
-> B_22_5_0 [ 1 ] ; localB -> B_22_7_0 [ 2 ] = localB -> B_22_5_0 [ 2 ] ; }
localB -> B_22_8_0 [ 0 ] = localX -> integrator_CSTATE_l0 [ 0 ] ; localB ->
B_22_8_0 [ 1 ] = localX -> integrator_CSTATE_l0 [ 1 ] ; localB -> B_22_8_0 [
2 ] = localX -> integrator_CSTATE_l0 [ 2 ] ; { real_T * * uBuffer = ( real_T
* * ) & localDW -> TransportDelay_PWORK_f . TUbufferPtrs [ 0 ] ; real_T * *
tBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK_f . TUbufferPtrs [
3 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ;
real_T * y0 = & localB -> B_22_9_0 [ 0 ] ; int_T * iw_Tail = & localDW ->
TransportDelay_IWORK_b . Tail [ 0 ] ; int_T * iw_Head = & localDW ->
TransportDelay_IWORK_b . Head [ 0 ] ; int_T * iw_Last = & localDW ->
TransportDelay_IWORK_b . Last [ 0 ] ; int_T * iw_CircularBufSize = & localDW
-> TransportDelay_IWORK_b . CircularBufSize [ 0 ] ; for ( i1 = 0 ; i1 < 3 ;
i1 ++ ) { tMinusDelay = ( ( localP -> P_9 > 0.0 ) ? localP -> P_9 : 0.0 ) ;
tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ] =
normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , localP -> P_10 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { localB -> B_22_11_0 = localP -> P_11 ; } rtb_B_22_4_0 = (
ssGetT ( S ) >= localB -> B_22_11_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ;
if ( isHit != 0 ) { localB -> B_22_13_0 [ 0 ] = localDW ->
Memory_PreviousInput_o [ 0 ] ; localB -> B_22_13_0 [ 1 ] = localDW ->
Memory_PreviousInput_o [ 1 ] ; localB -> B_22_13_0 [ 2 ] = localDW ->
Memory_PreviousInput_o [ 2 ] ; } if ( rtb_B_22_4_0 ) { localB -> B_20_0_0 [ 0
] = localB -> B_22_8_0 [ 0 ] - localB -> B_22_9_0 [ 0 ] ; localB -> B_20_1_0
[ 0 ] = localP -> P_0 * localB -> B_20_0_0 [ 0 ] ; localB -> B_22_15_0 [ 0 ]
= localB -> B_20_1_0 [ 0 ] ; localB -> B_20_0_0 [ 1 ] = localB -> B_22_8_0 [
1 ] - localB -> B_22_9_0 [ 1 ] ; localB -> B_20_1_0 [ 1 ] = localP -> P_0 *
localB -> B_20_0_0 [ 1 ] ; localB -> B_22_15_0 [ 1 ] = localB -> B_20_1_0 [ 1
] ; localB -> B_20_0_0 [ 2 ] = localB -> B_22_8_0 [ 2 ] - localB -> B_22_9_0
[ 2 ] ; localB -> B_20_1_0 [ 2 ] = localP -> P_0 * localB -> B_20_0_0 [ 2 ] ;
localB -> B_22_15_0 [ 2 ] = localB -> B_20_1_0 [ 2 ] ; } else { localB ->
B_22_15_0 [ 0 ] = localB -> B_22_13_0 [ 0 ] ; localB -> B_22_15_0 [ 1 ] =
localB -> B_22_13_0 [ 1 ] ; localB -> B_22_15_0 [ 2 ] = localB -> B_22_13_0 [
2 ] ; } localB -> B_22_16_0 [ 0 ] . re = localB -> B_22_7_0 [ 0 ] ; localB ->
B_22_16_0 [ 0 ] . im = localB -> B_22_15_0 [ 0 ] ; localB -> B_22_16_0 [ 1 ]
. re = localB -> B_22_7_0 [ 1 ] ; localB -> B_22_16_0 [ 1 ] . im = localB ->
B_22_15_0 [ 1 ] ; localB -> B_22_16_0 [ 2 ] . re = localB -> B_22_7_0 [ 2 ] ;
localB -> B_22_16_0 [ 2 ] . im = localB -> B_22_15_0 [ 2 ] ; rtb_B_22_18_0 =
muDoubleScalarSin ( localP -> P_15 * ssGetTaskTime ( S , 0 ) + localP -> P_16
) * localP -> P_13 + localP -> P_14 ; localB -> B_22_19_0 [ 0 ] = rtu_In [ 0
] * rtb_B_22_18_0 ; localB -> B_22_19_0 [ 1 ] = rtu_In [ 1 ] * rtb_B_22_18_0
; localB -> B_22_19_0 [ 2 ] = rtu_In [ 2 ] * rtb_B_22_18_0 ; rtb_B_22_18_0 =
muDoubleScalarSin ( localP -> P_19 * ssGetTaskTime ( S , 0 ) + localP -> P_20
) * localP -> P_17 + localP -> P_18 ; localB -> B_22_21_0 [ 0 ] = rtu_In [ 0
] * rtb_B_22_18_0 ; localB -> B_22_21_0 [ 1 ] = rtu_In [ 1 ] * rtb_B_22_18_0
; localB -> B_22_21_0 [ 2 ] = rtu_In [ 2 ] * rtb_B_22_18_0 ; localB ->
B_22_22_0 [ 0 ] = localP -> P_21 * muDoubleScalarHypot ( localB -> B_22_16_0
[ 0 ] . re , localB -> B_22_16_0 [ 0 ] . im ) ; localB -> B_22_22_0 [ 1 ] =
localP -> P_21 * muDoubleScalarHypot ( localB -> B_22_16_0 [ 1 ] . re ,
localB -> B_22_16_0 [ 1 ] . im ) ; localB -> B_22_22_0 [ 2 ] = localP -> P_21
* muDoubleScalarHypot ( localB -> B_22_16_0 [ 2 ] . re , localB -> B_22_16_0
[ 2 ] . im ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
RMS_SubsysRanBC ) ; } } } void normal_operation_no_faults_RMS_Update (
SimStruct * S , B_RMS_normal_operation_no_faults_T * localB ,
DW_RMS_normal_operation_no_faults_T * localDW ,
P_RMS_normal_operation_no_faults_T * localP ) { int32_T isHit ; if ( localDW
-> RMS_MODE ) { { real_T * * uBuffer = ( real_T * * ) & localDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * *
) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; localDW -> TransportDelay_IWORK . Head [ 0 ] = ( ( localDW ->
TransportDelay_IWORK . Head [ 0 ] < ( localDW -> TransportDelay_IWORK .
CircularBufSize [ 0 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK . Head [ 0
] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK . Head [ 0 ] == localDW
-> TransportDelay_IWORK . Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize [ 0 ] , & localDW ->
TransportDelay_IWORK . Tail [ 0 ] , & localDW -> TransportDelay_IWORK . Head
[ 0 ] , & localDW -> TransportDelay_IWORK . Last [ 0 ] , simTime - localP ->
P_4 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 0 ] ] = localB -> B_22_0_0 [ 0 ] ; localDW
-> TransportDelay_IWORK . Head [ 1 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 1 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 1 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 1 ] == localDW -> TransportDelay_IWORK .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize [ 1 ] , & localDW ->
TransportDelay_IWORK . Tail [ 1 ] , & localDW -> TransportDelay_IWORK . Head
[ 1 ] , & localDW -> TransportDelay_IWORK . Last [ 1 ] , simTime - localP ->
P_4 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 1 ] ] = localB -> B_22_0_0 [ 1 ] ; localDW
-> TransportDelay_IWORK . Head [ 2 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 2 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 2 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 2 ] == localDW -> TransportDelay_IWORK .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize [ 2 ] , & localDW ->
TransportDelay_IWORK . Tail [ 2 ] , & localDW -> TransportDelay_IWORK . Head
[ 2 ] , & localDW -> TransportDelay_IWORK . Last [ 2 ] , simTime - localP ->
P_4 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = localB -> B_22_0_0 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput [ 0 ] = localB -> B_22_7_0 [ 0 ] ; localDW ->
Memory_PreviousInput [ 1 ] = localB -> B_22_7_0 [ 1 ] ; localDW ->
Memory_PreviousInput [ 2 ] = localB -> B_22_7_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK_f . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK_f
. TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; localDW ->
TransportDelay_IWORK_b . Head [ 0 ] = ( ( localDW -> TransportDelay_IWORK_b .
Head [ 0 ] < ( localDW -> TransportDelay_IWORK_b . CircularBufSize [ 0 ] - 1
) ) ? ( localDW -> TransportDelay_IWORK_b . Head [ 0 ] + 1 ) : 0 ) ; if (
localDW -> TransportDelay_IWORK_b . Head [ 0 ] == localDW ->
TransportDelay_IWORK_b . Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK_b . CircularBufSize [ 0 ] , & localDW ->
TransportDelay_IWORK_b . Tail [ 0 ] , & localDW -> TransportDelay_IWORK_b .
Head [ 0 ] , & localDW -> TransportDelay_IWORK_b . Last [ 0 ] , simTime -
localP -> P_9 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , &
localDW -> TransportDelay_IWORK_b . MaxNewBufSize ) ) { ssSetErrorStatus ( S
, "tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [
localDW -> TransportDelay_IWORK_b . Head [ 0 ] ] = simTime ; ( * uBuffer ++ )
[ localDW -> TransportDelay_IWORK_b . Head [ 0 ] ] = localB -> B_22_8_0 [ 0 ]
; localDW -> TransportDelay_IWORK_b . Head [ 1 ] = ( ( localDW ->
TransportDelay_IWORK_b . Head [ 1 ] < ( localDW -> TransportDelay_IWORK_b .
CircularBufSize [ 1 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK_b . Head [
1 ] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK_b . Head [ 1 ] ==
localDW -> TransportDelay_IWORK_b . Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK_b . CircularBufSize [ 1 ] , & localDW ->
TransportDelay_IWORK_b . Tail [ 1 ] , & localDW -> TransportDelay_IWORK_b .
Head [ 1 ] , & localDW -> TransportDelay_IWORK_b . Last [ 1 ] , simTime -
localP -> P_9 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , &
localDW -> TransportDelay_IWORK_b . MaxNewBufSize ) ) { ssSetErrorStatus ( S
, "tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [
localDW -> TransportDelay_IWORK_b . Head [ 1 ] ] = simTime ; ( * uBuffer ++ )
[ localDW -> TransportDelay_IWORK_b . Head [ 1 ] ] = localB -> B_22_8_0 [ 1 ]
; localDW -> TransportDelay_IWORK_b . Head [ 2 ] = ( ( localDW ->
TransportDelay_IWORK_b . Head [ 2 ] < ( localDW -> TransportDelay_IWORK_b .
CircularBufSize [ 2 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK_b . Head [
2 ] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK_b . Head [ 2 ] ==
localDW -> TransportDelay_IWORK_b . Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK_b . CircularBufSize [ 2 ] , & localDW ->
TransportDelay_IWORK_b . Tail [ 2 ] , & localDW -> TransportDelay_IWORK_b .
Head [ 2 ] , & localDW -> TransportDelay_IWORK_b . Last [ 2 ] , simTime -
localP -> P_9 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , &
localDW -> TransportDelay_IWORK_b . MaxNewBufSize ) ) { ssSetErrorStatus ( S
, "tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ localDW
-> TransportDelay_IWORK_b . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ localDW
-> TransportDelay_IWORK_b . Head [ 2 ] ] = localB -> B_22_8_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput_o [ 0 ] = localB -> B_22_15_0 [ 0 ] ; localDW ->
Memory_PreviousInput_o [ 1 ] = localB -> B_22_15_0 [ 1 ] ; localDW ->
Memory_PreviousInput_o [ 2 ] = localB -> B_22_15_0 [ 2 ] ; } } } void
normal_operation_no_faults_RMS_Deriv ( SimStruct * S ,
B_RMS_normal_operation_no_faults_T * localB ,
DW_RMS_normal_operation_no_faults_T * localDW ,
XDot_RMS_normal_operation_no_faults_T * localXdot ) { if ( localDW ->
RMS_MODE ) { localXdot -> integrator_CSTATE_kw [ 0 ] = localB -> B_22_19_0 [
0 ] ; localXdot -> integrator_CSTATE_l0 [ 0 ] = localB -> B_22_21_0 [ 0 ] ;
localXdot -> integrator_CSTATE_kw [ 1 ] = localB -> B_22_19_0 [ 1 ] ;
localXdot -> integrator_CSTATE_l0 [ 1 ] = localB -> B_22_21_0 [ 1 ] ;
localXdot -> integrator_CSTATE_kw [ 2 ] = localB -> B_22_19_0 [ 2 ] ;
localXdot -> integrator_CSTATE_l0 [ 2 ] = localB -> B_22_21_0 [ 2 ] ; } else
{ { real_T * dx ; int_T i ; dx = & ( localXdot -> integrator_CSTATE_kw [ 0 ]
) ; for ( i = 0 ; i < 6 ; i ++ ) { dx [ i ] = 0.0 ; } } } } void
normal_operation_no_faults_RMS_Term ( SimStruct * const S ) { } void
normal_operation_no_faults_TrueRMS_Init ( SimStruct * S ,
B_TrueRMS_normal_operation_no_faults_T * localB ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ,
P_TrueRMS_normal_operation_no_faults_T * localP ,
X_TrueRMS_normal_operation_no_faults_T * localX ) { localX ->
integrator_CSTATE_el [ 0 ] = localP -> P_2 ; localDW -> Memory_PreviousInput
[ 0 ] = localP -> P_6 ; localB -> B_24_10_0 [ 0 ] = localP -> P_1 ; localX ->
integrator_CSTATE_el [ 1 ] = localP -> P_2 ; localDW -> Memory_PreviousInput
[ 1 ] = localP -> P_6 ; localB -> B_24_10_0 [ 1 ] = localP -> P_1 ; localX ->
integrator_CSTATE_el [ 2 ] = localP -> P_2 ; localDW -> Memory_PreviousInput
[ 2 ] = localP -> P_6 ; localB -> B_24_10_0 [ 2 ] = localP -> P_1 ; } void
normal_operation_no_faults_TrueRMS_Disable ( SimStruct * S ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ) { localDW -> TrueRMS_MODE
= false ; } void normal_operation_no_faults_TrueRMS ( SimStruct * S ,
boolean_T rtu_Enable , const real_T rtu_In [ 3 ] ,
B_TrueRMS_normal_operation_no_faults_T * localB ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ,
P_TrueRMS_normal_operation_no_faults_T * localP ,
X_TrueRMS_normal_operation_no_faults_T * localX ,
XDis_TrueRMS_normal_operation_no_faults_T * localXdis ) { int32_T isHit ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable ) { if ( ! localDW ->
TrueRMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void ) memset ( & (
localXdis -> integrator_CSTATE_el ) , 0 , 3 * sizeof ( boolean_T ) ) ;
localDW -> TrueRMS_MODE = true ; } } else { if ( localDW -> TrueRMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> integrator_CSTATE_el ) , 1 , 3 * sizeof ( boolean_T ) ) ;
normal_operation_no_faults_TrueRMS_Disable ( S , localDW ) ; } } } if (
localDW -> TrueRMS_MODE ) { localB -> B_24_1_0 [ 0 ] = localX ->
integrator_CSTATE_el [ 0 ] ; localB -> B_24_1_0 [ 1 ] = localX ->
integrator_CSTATE_el [ 1 ] ; localB -> B_24_1_0 [ 2 ] = localX ->
integrator_CSTATE_el [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & localDW
-> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 3 ] ; real_T simTime
= ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & localB ->
B_24_2_0 [ 0 ] ; int_T * iw_Tail = & localDW -> TransportDelay_IWORK . Tail [
0 ] ; int_T * iw_Head = & localDW -> TransportDelay_IWORK . Head [ 0 ] ;
int_T * iw_Last = & localDW -> TransportDelay_IWORK . Last [ 0 ] ; int_T *
iw_CircularBufSize = & localDW -> TransportDelay_IWORK . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( localP -> P_3 > 0.0 )
? localP -> P_3 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ] =
normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , localP -> P_4 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { localB -> B_24_3_0 = localP -> P_5 ; localB -> B_24_4_0 [ 0
] = localDW -> Memory_PreviousInput [ 0 ] ; localB -> B_24_4_0 [ 1 ] =
localDW -> Memory_PreviousInput [ 1 ] ; localB -> B_24_4_0 [ 2 ] = localDW ->
Memory_PreviousInput [ 2 ] ; } if ( ssGetT ( S ) >= localB -> B_24_3_0 ) {
localB -> B_23_0_0 [ 0 ] = localB -> B_24_1_0 [ 0 ] - localB -> B_24_2_0 [ 0
] ; localB -> B_23_1_0 [ 0 ] = localP -> P_0 * localB -> B_23_0_0 [ 0 ] ;
localB -> B_24_7_0 [ 0 ] = localB -> B_23_1_0 [ 0 ] ; localB -> B_23_0_0 [ 1
] = localB -> B_24_1_0 [ 1 ] - localB -> B_24_2_0 [ 1 ] ; localB -> B_23_1_0
[ 1 ] = localP -> P_0 * localB -> B_23_0_0 [ 1 ] ; localB -> B_24_7_0 [ 1 ] =
localB -> B_23_1_0 [ 1 ] ; localB -> B_23_0_0 [ 2 ] = localB -> B_24_1_0 [ 2
] - localB -> B_24_2_0 [ 2 ] ; localB -> B_23_1_0 [ 2 ] = localP -> P_0 *
localB -> B_23_0_0 [ 2 ] ; localB -> B_24_7_0 [ 2 ] = localB -> B_23_1_0 [ 2
] ; } else { localB -> B_24_7_0 [ 0 ] = localB -> B_24_4_0 [ 0 ] ; localB ->
B_24_7_0 [ 1 ] = localB -> B_24_4_0 [ 1 ] ; localB -> B_24_7_0 [ 2 ] = localB
-> B_24_4_0 [ 2 ] ; } localB -> B_24_8_0 [ 0 ] = rtu_In [ 0 ] * rtu_In [ 0 ]
; localB -> B_24_8_0 [ 1 ] = rtu_In [ 1 ] * rtu_In [ 1 ] ; localB -> B_24_8_0
[ 2 ] = rtu_In [ 2 ] * rtu_In [ 2 ] ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
localDW -> Saturationtoavoidnegativesqrt_MODE [ 0 ] = localB -> B_24_7_0 [ 0
] >= localP -> P_7 ? 1 : localB -> B_24_7_0 [ 0 ] > localP -> P_8 ? 0 : - 1 ;
localDW -> Saturationtoavoidnegativesqrt_MODE [ 1 ] = localB -> B_24_7_0 [ 1
] >= localP -> P_7 ? 1 : localB -> B_24_7_0 [ 1 ] > localP -> P_8 ? 0 : - 1 ;
localDW -> Saturationtoavoidnegativesqrt_MODE [ 2 ] = localB -> B_24_7_0 [ 2
] >= localP -> P_7 ? 1 : localB -> B_24_7_0 [ 2 ] > localP -> P_8 ? 0 : - 1 ;
} localB -> B_24_9_0 [ 0 ] = localDW -> Saturationtoavoidnegativesqrt_MODE [
0 ] == 1 ? localP -> P_7 : localDW -> Saturationtoavoidnegativesqrt_MODE [ 0
] == - 1 ? localP -> P_8 : localB -> B_24_7_0 [ 0 ] ; localB -> B_24_9_0 [ 1
] = localDW -> Saturationtoavoidnegativesqrt_MODE [ 1 ] == 1 ? localP -> P_7
: localDW -> Saturationtoavoidnegativesqrt_MODE [ 1 ] == - 1 ? localP -> P_8
: localB -> B_24_7_0 [ 1 ] ; localB -> B_24_9_0 [ 2 ] = localDW ->
Saturationtoavoidnegativesqrt_MODE [ 2 ] == 1 ? localP -> P_7 : localDW ->
Saturationtoavoidnegativesqrt_MODE [ 2 ] == - 1 ? localP -> P_8 : localB ->
B_24_7_0 [ 2 ] ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if ( localDW ->
Sqrt_DWORK1 != 0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
localDW -> Sqrt_DWORK1 = 0 ; } localB -> B_24_10_0 [ 0 ] = muDoubleScalarSqrt
( localB -> B_24_9_0 [ 0 ] ) ; localB -> B_24_10_0 [ 1 ] = muDoubleScalarSqrt
( localB -> B_24_9_0 [ 1 ] ) ; localB -> B_24_10_0 [ 2 ] = muDoubleScalarSqrt
( localB -> B_24_9_0 [ 2 ] ) ; srUpdateBC ( localDW -> TrueRMS_SubsysRanBC )
; } else { if ( localB -> B_24_9_0 [ 0 ] < 0.0 ) { localB -> B_24_10_0 [ 0 ]
= - muDoubleScalarSqrt ( muDoubleScalarAbs ( localB -> B_24_9_0 [ 0 ] ) ) ; }
else { localB -> B_24_10_0 [ 0 ] = muDoubleScalarSqrt ( localB -> B_24_9_0 [
0 ] ) ; } if ( localB -> B_24_9_0 [ 0 ] < 0.0 ) { localDW -> Sqrt_DWORK1 = 1
; } if ( localB -> B_24_9_0 [ 1 ] < 0.0 ) { localB -> B_24_10_0 [ 1 ] = -
muDoubleScalarSqrt ( muDoubleScalarAbs ( localB -> B_24_9_0 [ 1 ] ) ) ; }
else { localB -> B_24_10_0 [ 1 ] = muDoubleScalarSqrt ( localB -> B_24_9_0 [
1 ] ) ; } if ( localB -> B_24_9_0 [ 1 ] < 0.0 ) { localDW -> Sqrt_DWORK1 = 1
; } if ( localB -> B_24_9_0 [ 2 ] < 0.0 ) { localB -> B_24_10_0 [ 2 ] = -
muDoubleScalarSqrt ( muDoubleScalarAbs ( localB -> B_24_9_0 [ 2 ] ) ) ; }
else { localB -> B_24_10_0 [ 2 ] = muDoubleScalarSqrt ( localB -> B_24_9_0 [
2 ] ) ; } if ( localB -> B_24_9_0 [ 2 ] < 0.0 ) { localDW -> Sqrt_DWORK1 = 1
; } } } } void normal_operation_no_faults_TrueRMS_Update ( SimStruct * S ,
B_TrueRMS_normal_operation_no_faults_T * localB ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ,
P_TrueRMS_normal_operation_no_faults_T * localP ) { int32_T isHit ; if (
localDW -> TrueRMS_MODE ) { { real_T * * uBuffer = ( real_T * * ) & localDW
-> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 3 ] ; real_T simTime
= ssGetT ( S ) ; localDW -> TransportDelay_IWORK . Head [ 0 ] = ( ( localDW
-> TransportDelay_IWORK . Head [ 0 ] < ( localDW -> TransportDelay_IWORK .
CircularBufSize [ 0 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK . Head [ 0
] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK . Head [ 0 ] == localDW
-> TransportDelay_IWORK . Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize [ 0 ] , & localDW ->
TransportDelay_IWORK . Tail [ 0 ] , & localDW -> TransportDelay_IWORK . Head
[ 0 ] , & localDW -> TransportDelay_IWORK . Last [ 0 ] , simTime - localP ->
P_3 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 0 ] ] = localB -> B_24_1_0 [ 0 ] ; localDW
-> TransportDelay_IWORK . Head [ 1 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 1 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 1 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 1 ] == localDW -> TransportDelay_IWORK .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize [ 1 ] , & localDW ->
TransportDelay_IWORK . Tail [ 1 ] , & localDW -> TransportDelay_IWORK . Head
[ 1 ] , & localDW -> TransportDelay_IWORK . Last [ 1 ] , simTime - localP ->
P_3 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 1 ] ] = localB -> B_24_1_0 [ 1 ] ; localDW
-> TransportDelay_IWORK . Head [ 2 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 2 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 2 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 2 ] == localDW -> TransportDelay_IWORK .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize [ 2 ] , & localDW ->
TransportDelay_IWORK . Tail [ 2 ] , & localDW -> TransportDelay_IWORK . Head
[ 2 ] , & localDW -> TransportDelay_IWORK . Last [ 2 ] , simTime - localP ->
P_3 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = localB -> B_24_1_0 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput [ 0 ] = localB -> B_24_7_0 [ 0 ] ; localDW ->
Memory_PreviousInput [ 1 ] = localB -> B_24_7_0 [ 1 ] ; localDW ->
Memory_PreviousInput [ 2 ] = localB -> B_24_7_0 [ 2 ] ; } } } void
normal_operation_no_faults_TrueRMS_Deriv ( SimStruct * S ,
B_TrueRMS_normal_operation_no_faults_T * localB ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ,
XDot_TrueRMS_normal_operation_no_faults_T * localXdot ) { if ( localDW ->
TrueRMS_MODE ) { localXdot -> integrator_CSTATE_el [ 0 ] = localB -> B_24_8_0
[ 0 ] ; localXdot -> integrator_CSTATE_el [ 1 ] = localB -> B_24_8_0 [ 1 ] ;
localXdot -> integrator_CSTATE_el [ 2 ] = localB -> B_24_8_0 [ 2 ] ; } else {
{ real_T * dx ; int_T i ; dx = & ( localXdot -> integrator_CSTATE_el [ 0 ] )
; for ( i = 0 ; i < 3 ; i ++ ) { dx [ i ] = 0.0 ; } } } } void
normal_operation_no_faults_TrueRMS_ZC ( SimStruct * S ,
B_TrueRMS_normal_operation_no_faults_T * localB ,
DW_TrueRMS_normal_operation_no_faults_T * localDW ,
P_TrueRMS_normal_operation_no_faults_T * localP ,
ZCV_TrueRMS_normal_operation_no_faults_T * localZCSV ) { if ( localDW ->
TrueRMS_MODE ) { localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC [ 0 ] =
localB -> B_24_7_0 [ 0 ] - localP -> P_7 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 0 ] = localB -> B_24_7_0 [ 0 ] -
localP -> P_8 ; localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC [ 1 ] =
localB -> B_24_7_0 [ 1 ] - localP -> P_7 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 1 ] = localB -> B_24_7_0 [ 1 ] -
localP -> P_8 ; localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC [ 2 ] =
localB -> B_24_7_0 [ 2 ] - localP -> P_7 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 2 ] = localB -> B_24_7_0 [ 2 ] -
localP -> P_8 ; } else { { real_T * zcsv = & ( localZCSV ->
Saturationtoavoidnegativesqrt_UprLim_ZC [ 0 ] ) ; int_T i ; for ( i = 0 ; i <
6 ; i ++ ) { zcsv [ i ] = 0.0 ; } } } } void
normal_operation_no_faults_TrueRMS_Term ( SimStruct * const S ) { } void
normal_operation_no_faults_ACVoltageController_Init ( SimStruct * S ,
B_ACVoltageController_normal_operation_no_faults_T * localB ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
P_ACVoltageController_normal_operation_no_faults_T * localP ,
X_ACVoltageController_normal_operation_no_faults_T * localX ) { localDW ->
UnitDelay_DSTATE [ 0 ] = localP -> P_1 ; localDW -> UnitDelay_DSTATE [ 1 ] =
localP -> P_1 ; localDW -> UnitDelay_DSTATE [ 2 ] = localP -> P_1 ; localX ->
Integrator_CSTATE_or = localP -> P_8 ;
normal_operation_no_faults_TrueRMS_Init ( S , & localB -> TrueRMS , & localDW
-> TrueRMS , & localP -> TrueRMS , & localX -> TrueRMS ) ;
normal_operation_no_faults_RMS_Init ( S , & localB -> RMS , & localDW -> RMS
, & localP -> RMS , & localX -> RMS ) ; localB -> B_25_18_0 = localP -> P_0 ;
} void normal_operation_no_faults_ACVoltageController_Disable ( SimStruct * S
, DW_ACVoltageController_normal_operation_no_faults_T * localDW ) { if (
localDW -> TrueRMS . TrueRMS_MODE ) {
normal_operation_no_faults_TrueRMS_Disable ( S , & localDW -> TrueRMS ) ; }
if ( localDW -> RMS . RMS_MODE ) { normal_operation_no_faults_RMS_Disable ( S
, & localDW -> RMS ) ; } localDW -> ACVoltageController_MODE = false ; } void
normal_operation_no_faults_ACVoltageController ( SimStruct * S , real_T
rtu_Enable , real_T rtu_V_ac_ref ,
B_ACVoltageController_normal_operation_no_faults_T * localB ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
P_ACVoltageController_normal_operation_no_faults_T * localP ,
X_ACVoltageController_normal_operation_no_faults_T * localX ,
XDis_ACVoltageController_normal_operation_no_faults_T * localXdis ) { real_T
rtb_B_25_0_0 [ 3 ] ; int32_T isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( ( isHit != 0 ) && ( ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable >
0.0 ) { if ( ! localDW -> ACVoltageController_MODE ) { if ( ssGetTaskTime ( S
, 1 ) != ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; } ( void ) memset ( & ( localXdis -> Integrator_CSTATE_or ) , 0 , 10 *
sizeof ( boolean_T ) ) ; localDW -> ACVoltageController_MODE = true ; } }
else { if ( localDW -> ACVoltageController_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> Integrator_CSTATE_or ) , 1 , 10 * sizeof ( boolean_T ) ) ;
normal_operation_no_faults_ACVoltageController_Disable ( S , localDW ) ; } }
} if ( localDW -> ACVoltageController_MODE ) { isHit = ssIsSampleHit ( S , 2
, 0 ) ; if ( isHit != 0 ) { rtb_B_25_0_0 [ 0 ] = localDW -> UnitDelay_DSTATE
[ 0 ] ; rtb_B_25_0_0 [ 1 ] = localDW -> UnitDelay_DSTATE [ 1 ] ; rtb_B_25_0_0
[ 2 ] = localDW -> UnitDelay_DSTATE [ 2 ] ; isHit = ssIsSampleHit ( S , 3 , 0
) ; if ( isHit != 0 ) { localB -> B_25_1_0 [ 0 ] = rtb_B_25_0_0 [ 0 ] ;
localB -> B_25_1_0 [ 1 ] = rtb_B_25_0_0 [ 1 ] ; localB -> B_25_1_0 [ 2 ] =
rtb_B_25_0_0 [ 2 ] ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { localB -> B_25_2_0 [ 0 ] = localP -> P_2 * localB -> B_25_1_0 [ 0 ] ;
localB -> B_25_2_0 [ 1 ] = localP -> P_2 * localB -> B_25_1_0 [ 1 ] ; localB
-> B_25_2_0 [ 2 ] = localP -> P_2 * localB -> B_25_1_0 [ 2 ] ; }
normal_operation_no_faults_TrueRMS ( S , localB -> B_25_4_0 , localB ->
B_25_2_0 , & localB -> TrueRMS , & localDW -> TrueRMS , & localP -> TrueRMS ,
& localX -> TrueRMS , & localXdis -> TrueRMS ) ;
normal_operation_no_faults_RMS ( S , localB -> B_25_6_0 , localB -> B_25_2_0
, & localB -> RMS , & localDW -> RMS , & localP -> RMS , & localX -> RMS , &
localXdis -> RMS ) ; if ( localB -> B_25_4_0 ) { localB -> B_25_8_0 [ 0 ] =
localB -> TrueRMS . B_24_10_0 [ 0 ] ; } else { localB -> B_25_8_0 [ 0 ] =
localB -> RMS . B_22_22_0 [ 0 ] ; } localB -> B_25_9_0 [ 0 ] = localP -> P_4
* localB -> B_25_8_0 [ 0 ] ; if ( localB -> B_25_4_0 ) { localB -> B_25_8_0 [
1 ] = localB -> TrueRMS . B_24_10_0 [ 1 ] ; } else { localB -> B_25_8_0 [ 1 ]
= localB -> RMS . B_22_22_0 [ 1 ] ; } localB -> B_25_9_0 [ 1 ] = localP ->
P_4 * localB -> B_25_8_0 [ 1 ] ; if ( localB -> B_25_4_0 ) { localB ->
B_25_8_0 [ 2 ] = localB -> TrueRMS . B_24_10_0 [ 2 ] ; } else { localB ->
B_25_8_0 [ 2 ] = localB -> RMS . B_22_22_0 [ 2 ] ; } localB -> B_25_9_0 [ 2 ]
= localP -> P_4 * localB -> B_25_8_0 [ 2 ] ; localB -> B_25_10_0 = ( localB
-> B_25_9_0 [ 0 ] + localB -> B_25_9_0 [ 1 ] ) + localB -> B_25_9_0 [ 2 ] ;
if ( rtu_Enable > localP -> P_5 ) { localB -> B_25_11_0 = rtu_V_ac_ref ; }
else { localB -> B_25_11_0 = localB -> B_25_10_0 ; } localB -> B_25_12_0 =
localB -> B_25_11_0 - localB -> B_25_10_0 ; localB -> B_25_13_0 = localP ->
P_6 * localB -> B_25_12_0 ; localB -> B_25_14_0 = localP -> P_7 * localB ->
B_25_13_0 ; localB -> B_25_15_0 = localX -> Integrator_CSTATE_or ; localB ->
B_25_16_0 = localB -> B_25_14_0 + localB -> B_25_15_0 ; if (
ssIsMajorTimeStep ( S ) != 0 ) { localDW -> Saturation_MODE = localB ->
B_25_16_0 >= localP -> P_9 ? 1 : localB -> B_25_16_0 > localP -> P_10 ? 0 : -
1 ; } localB -> B_25_17_0 = localDW -> Saturation_MODE == 1 ? localP -> P_9 :
localDW -> Saturation_MODE == - 1 ? localP -> P_10 : localB -> B_25_16_0 ;
localB -> B_25_18_0 = localP -> P_11 * localB -> B_25_17_0 ; localB ->
B_25_19_0 = localP -> P_12 * localB -> B_25_13_0 ; if ( ssIsMajorTimeStep ( S
) != 0 ) { srUpdateBC ( localDW -> ACVoltageController_SubsysRanBC ) ; } } }
void normal_operation_no_faults_ACVoltageControllerTID4 ( SimStruct * S ,
B_ACVoltageController_normal_operation_no_faults_T * localB ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
P_ACVoltageController_normal_operation_no_faults_T * localP ) { localB ->
B_25_4_0 = ( localP -> P_3 != 0.0 ) ; localB -> B_25_6_0 = ! localB ->
B_25_4_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
ACVoltageController_SubsysRanBC ) ; } } void
normal_operation_no_faults_ACVoltageController_Update ( SimStruct * S , const
real_T rtu_V_ac [ 3 ] , B_ACVoltageController_normal_operation_no_faults_T *
localB , DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
P_ACVoltageController_normal_operation_no_faults_T * localP ) { int32_T isHit
; if ( localDW -> ACVoltageController_MODE ) { isHit = ssIsSampleHit ( S , 2
, 0 ) ; if ( isHit != 0 ) { localDW -> UnitDelay_DSTATE [ 0 ] = rtu_V_ac [ 0
] ; localDW -> UnitDelay_DSTATE [ 1 ] = rtu_V_ac [ 1 ] ; localDW ->
UnitDelay_DSTATE [ 2 ] = rtu_V_ac [ 2 ] ; }
normal_operation_no_faults_TrueRMS_Update ( S , & localB -> TrueRMS , &
localDW -> TrueRMS , & localP -> TrueRMS ) ;
normal_operation_no_faults_RMS_Update ( S , & localB -> RMS , & localDW ->
RMS , & localP -> RMS ) ; } } void
normal_operation_no_faults_ACVoltageController_Deriv ( SimStruct * S ,
B_ACVoltageController_normal_operation_no_faults_T * localB ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
XDot_ACVoltageController_normal_operation_no_faults_T * localXdot ) { if (
localDW -> ACVoltageController_MODE ) {
normal_operation_no_faults_TrueRMS_Deriv ( S , & localB -> TrueRMS , &
localDW -> TrueRMS , & localXdot -> TrueRMS ) ;
normal_operation_no_faults_RMS_Deriv ( S , & localB -> RMS , & localDW -> RMS
, & localXdot -> RMS ) ; localXdot -> Integrator_CSTATE_or = localB ->
B_25_19_0 ; } else { { real_T * dx ; int_T i ; dx = & ( localXdot ->
Integrator_CSTATE_or ) ; for ( i = 0 ; i < 10 ; i ++ ) { dx [ i ] = 0.0 ; } }
} } void normal_operation_no_faults_ACVoltageController_ZC ( SimStruct * S ,
B_ACVoltageController_normal_operation_no_faults_T * localB ,
DW_ACVoltageController_normal_operation_no_faults_T * localDW ,
P_ACVoltageController_normal_operation_no_faults_T * localP ,
ZCV_ACVoltageController_normal_operation_no_faults_T * localZCSV ) { if (
localDW -> ACVoltageController_MODE ) { normal_operation_no_faults_TrueRMS_ZC
( S , & localB -> TrueRMS , & localDW -> TrueRMS , & localP -> TrueRMS , &
localZCSV -> TrueRMS ) ; localZCSV -> Saturation_UprLim_ZC_g = localB ->
B_25_16_0 - localP -> P_9 ; localZCSV -> Saturation_LwrLim_ZC_e = localB ->
B_25_16_0 - localP -> P_10 ; } else { { real_T * zcsv = & ( localZCSV ->
Saturation_UprLim_ZC_g ) ; int_T i ; for ( i = 0 ; i < 8 ; i ++ ) { zcsv [ i
] = 0.0 ; } } } } void normal_operation_no_faults_ACVoltageController_Term (
SimStruct * const S ) { } void
normal_operation_no_faults_Circulatingcurrentsuppression_Init ( SimStruct * S
, B_Circulatingcurrentsuppression_normal_operation_no_faults_T * localB ,
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T * localDW ,
P_Circulatingcurrentsuppression_normal_operation_no_faults_T * localP ,
X_Circulatingcurrentsuppression_normal_operation_no_faults_T * localX ) {
int32_T i ; for ( i = 0 ; i < 6 ; i ++ ) { localDW -> UnitDelay9_DSTATE [ i ]
= localP -> P_5 ; } localDW -> UnitDelay1_DSTATE = localP -> P_12 ; localX ->
Integrator_CSTATE_f = localP -> P_19 ; localX -> Integrator_CSTATE_f0 =
localP -> P_22 ; normal_operation_no_faults_Subsystem1_Init ( S , & localB ->
Subsystem1 , & localP -> Subsystem1 ) ;
normal_operation_no_faults_Subsystempi2delay_Init ( S , & localB ->
Subsystempi2delay , & localP -> Subsystempi2delay ) ;
normal_operation_no_faults_Subsystem1_e_Init ( S , & localB -> Subsystem1_k ,
& localP -> Subsystem1_k ) ;
normal_operation_no_faults_Subsystempi2delay_m_Init ( S , & localB ->
Subsystempi2delay_e , & localP -> Subsystempi2delay_e ) ; localB -> B_30_57_0
[ 0 ] = localP -> P_0 ; localB -> B_30_57_0 [ 1 ] = localP -> P_0 ; localB ->
B_30_57_0 [ 2 ] = localP -> P_0 ; } void
normal_operation_no_faults_Circulatingcurrentsuppression_Disable ( SimStruct
* S , DW_Circulatingcurrentsuppression_normal_operation_no_faults_T * localDW
) { if ( localDW -> Subsystem1_k . Subsystem1_MODE ) {
normal_operation_no_faults_Subsystem1_Disable ( S , & localDW -> Subsystem1_k
) ; } if ( localDW -> Subsystempi2delay_e . Subsystempi2delay_MODE ) {
normal_operation_no_faults_Subsystempi2delay_Disable ( S , & localDW ->
Subsystempi2delay_e ) ; } localDW -> Circulatingcurrentsuppression_MODE =
false ; } void normal_operation_no_faults_Circulatingcurrentsuppression (
SimStruct * S , real_T rtu_Enable ,
B_Circulatingcurrentsuppression_normal_operation_no_faults_T * localB ,
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T * localDW ,
P_Circulatingcurrentsuppression_normal_operation_no_faults_T * localP ,
X_Circulatingcurrentsuppression_normal_operation_no_faults_T * localX ,
XDis_Circulatingcurrentsuppression_normal_operation_no_faults_T * localXdis )
{ real_T B_30_15_0 [ 3 ] ; real_T rtb_B_30_28_0 ; real_T rtb_B_30_33_0 ;
int32_T i ; real_T B_30_25_0_idx_0 ; real_T B_30_25_0_idx_1 ; real_T tmp ;
real_T tmp_0 ; int32_T isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( (
isHit != 0 ) && ( ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0.0 )
{ if ( ! localDW -> Circulatingcurrentsuppression_MODE ) { if ( ssGetTaskTime
( S , 1 ) != ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep
( S ) ; } ( void ) memset ( & ( localXdis -> Integrator_CSTATE_f ) , 0 , 2 *
sizeof ( boolean_T ) ) ; localDW -> Circulatingcurrentsuppression_MODE = true
; } } else { if ( localDW -> Circulatingcurrentsuppression_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> Integrator_CSTATE_f ) , 1 , 2 * sizeof ( boolean_T ) ) ;
normal_operation_no_faults_Circulatingcurrentsuppression_Disable ( S ,
localDW ) ; } } } if ( localDW -> Circulatingcurrentsuppression_MODE ) {
isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { isHit =
ssIsSampleHit ( S , 3 , 0 ) ; for ( i = 0 ; i < 6 ; i ++ ) { if ( isHit != 0
) { localB -> B_30_6_0 [ i ] = localP -> P_6 * localDW -> UnitDelay9_DSTATE [
i ] ; } } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) {
B_30_25_0_idx_0 = ( localB -> B_30_6_0 [ 0 ] + localB -> B_30_6_0 [ 1 ] ) *
localP -> P_7 ; tmp = ( localB -> B_30_6_0 [ 2 ] + localB -> B_30_6_0 [ 3 ] )
* localP -> P_8 ; tmp_0 = ( localB -> B_30_6_0 [ 4 ] + localB -> B_30_6_0 [ 5
] ) * localP -> P_9 ; for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { B_30_15_0 [
isHit ] = localP -> P_11 * ( localP -> P_10 [ isHit + 6 ] * tmp_0 + ( localP
-> P_10 [ isHit + 3 ] * tmp + localP -> P_10 [ isHit ] * B_30_25_0_idx_0 ) )
; } } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { localB -> B_30_17_0 =
localDW -> UnitDelay1_DSTATE ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if (
isHit != 0 ) { normal_operation_no_faults_Subsystem1 ( S , localB ->
B_30_20_0 , & B_30_15_0 [ 0 ] , localB -> B_30_17_0 , & localB -> Subsystem1
, & localDW -> Subsystem1 ) ; normal_operation_no_faults_Subsystempi2delay (
S , localB -> B_30_23_0 , & B_30_15_0 [ 0 ] , localB -> B_30_17_0 , & localB
-> Subsystempi2delay , & localDW -> Subsystempi2delay ) ; if ( localB ->
B_30_20_0 != 0 ) { B_30_25_0_idx_0 = localB -> Subsystem1 . B_1_0_0 ;
B_30_25_0_idx_1 = localB -> Subsystem1 . B_1_1_0 ; } else { B_30_25_0_idx_0 =
localB -> Subsystempi2delay . B_0_0_0 ; B_30_25_0_idx_1 = localB ->
Subsystempi2delay . B_0_1_0 ; } localB -> B_30_26_0 = localP -> P_16 *
B_30_25_0_idx_0 ; localB -> B_30_27_0 = localP -> P_17 * B_30_25_0_idx_1 ;
rtb_B_30_28_0 = localB -> B_30_1_0 - B_30_25_0_idx_0 ; localB -> B_30_29_0 =
localP -> P_18 * rtb_B_30_28_0 ; } localB -> B_30_30_0 = localX ->
Integrator_CSTATE_f ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 )
{ localB -> B_30_31_0 = localP -> P_20 * rtb_B_30_28_0 ; } localB ->
B_30_32_0 = localB -> B_30_31_0 + localB -> B_30_30_0 ; isHit = ssIsSampleHit
( S , 3 , 0 ) ; if ( isHit != 0 ) { rtb_B_30_33_0 = localB -> B_30_0_0 -
B_30_25_0_idx_1 ; localB -> B_30_34_0 = localP -> P_21 * rtb_B_30_33_0 ; }
localB -> B_30_35_0 = localX -> Integrator_CSTATE_f0 ; isHit = ssIsSampleHit
( S , 3 , 0 ) ; if ( isHit != 0 ) { localB -> B_30_36_0 = localP -> P_23 *
rtb_B_30_33_0 ; } localB -> B_30_37_0 = localB -> B_30_36_0 + localB ->
B_30_35_0 ; localB -> B_30_43_0 = look1_binlxpw ( muDoubleScalarRem ( ssGetT
( S ) - localB -> B_30_40_0 , localB -> B_30_39_0 ) , localP -> P_26 , localP
-> P_25 , 1U ) ; localB -> B_30_44_0 = localB -> B_30_32_0 + localB ->
B_30_27_0 ; localB -> B_30_45_0 = localB -> B_30_37_0 - localB -> B_30_26_0 ;
normal_operation_no_faults_Subsystem1_g ( S , localB -> B_30_48_0 , localB ->
B_30_44_0 , localB -> B_30_45_0 , localB -> B_30_17_0 , & localB ->
Subsystem1_k , & localDW -> Subsystem1_k ) ;
normal_operation_no_faults_Subsystempi2delay_g ( S , localB -> B_30_51_0 ,
localB -> B_30_44_0 , localB -> B_30_45_0 , localB -> B_30_17_0 , & localB ->
Subsystempi2delay_e , & localDW -> Subsystempi2delay_e ) ; if ( localB ->
B_30_48_0 != 0 ) { localB -> B_30_53_0 [ 0 ] = localB -> Subsystem1_k .
B_3_0_0 ; localB -> B_30_53_0 [ 1 ] = localB -> Subsystem1_k . B_3_1_0 ; }
else { localB -> B_30_53_0 [ 0 ] = localB -> Subsystempi2delay_e . B_2_0_0 ;
localB -> B_30_53_0 [ 1 ] = localB -> Subsystempi2delay_e . B_2_1_0 ; }
localB -> B_30_54_0 [ 0 ] = localB -> B_30_53_0 [ 0 ] ; localB -> B_30_54_0 [
1 ] = localB -> B_30_53_0 [ 1 ] ; localB -> B_30_54_0 [ 2 ] = localB ->
B_30_2_0 ; for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { localB -> B_30_55_0 [
isHit ] = 0.0 ; localB -> B_30_55_0 [ isHit ] += localP -> P_30 [ isHit ] *
localB -> B_30_54_0 [ 0 ] ; localB -> B_30_55_0 [ isHit ] += localP -> P_30 [
isHit + 3 ] * localB -> B_30_54_0 [ 1 ] ; localB -> B_30_55_0 [ isHit ] +=
localP -> P_30 [ isHit + 6 ] * localB -> B_30_54_0 [ 2 ] ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { localDW -> Saturation1_MODE [ 0 ] = localB
-> B_30_55_0 [ 0 ] >= localP -> P_31 ? 1 : localB -> B_30_55_0 [ 0 ] > localP
-> P_32 ? 0 : - 1 ; localDW -> Saturation1_MODE [ 1 ] = localB -> B_30_55_0 [
1 ] >= localP -> P_31 ? 1 : localB -> B_30_55_0 [ 1 ] > localP -> P_32 ? 0 :
- 1 ; localDW -> Saturation1_MODE [ 2 ] = localB -> B_30_55_0 [ 2 ] >= localP
-> P_31 ? 1 : localB -> B_30_55_0 [ 2 ] > localP -> P_32 ? 0 : - 1 ; } localB
-> B_30_56_0 [ 0 ] = localDW -> Saturation1_MODE [ 0 ] == 1 ? localP -> P_31
: localDW -> Saturation1_MODE [ 0 ] == - 1 ? localP -> P_32 : localB ->
B_30_55_0 [ 0 ] ; if ( rtu_Enable > localP -> P_33 ) { localB -> B_30_57_0 [
0 ] = localB -> B_30_56_0 [ 0 ] ; } else { localB -> B_30_57_0 [ 0 ] = localB
-> B_30_3_0 ; } localB -> B_30_56_0 [ 1 ] = localDW -> Saturation1_MODE [ 1 ]
== 1 ? localP -> P_31 : localDW -> Saturation1_MODE [ 1 ] == - 1 ? localP ->
P_32 : localB -> B_30_55_0 [ 1 ] ; if ( rtu_Enable > localP -> P_33 ) {
localB -> B_30_57_0 [ 1 ] = localB -> B_30_56_0 [ 1 ] ; } else { localB ->
B_30_57_0 [ 1 ] = localB -> B_30_3_0 ; } localB -> B_30_56_0 [ 2 ] = localDW
-> Saturation1_MODE [ 2 ] == 1 ? localP -> P_31 : localDW -> Saturation1_MODE
[ 2 ] == - 1 ? localP -> P_32 : localB -> B_30_55_0 [ 2 ] ; if ( rtu_Enable >
localP -> P_33 ) { localB -> B_30_57_0 [ 2 ] = localB -> B_30_56_0 [ 2 ] ; }
else { localB -> B_30_57_0 [ 2 ] = localB -> B_30_3_0 ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Circulatingcurrentsuppression_SubsysRanBC ) ; } } } void
normal_operation_no_faults_CirculatingcurrentsuppressionTID4 ( SimStruct * S
, B_Circulatingcurrentsuppression_normal_operation_no_faults_T * localB ,
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T * localDW ,
P_Circulatingcurrentsuppression_normal_operation_no_faults_T * localP ) {
localB -> B_30_0_0 = localP -> P_1 ; localB -> B_30_1_0 = localP -> P_2 ;
localB -> B_30_2_0 = localP -> P_3 ; localB -> B_30_3_0 = localP -> P_4 ;
localB -> B_30_20_0 = ( uint8_T ) ( localP -> P_13 == localP -> P_14 ) ;
localB -> B_30_23_0 = ( uint8_T ) ( localP -> P_13 == localP -> P_15 ) ;
localB -> B_30_39_0 = localP -> P_24 ; localB -> B_30_48_0 = ( uint8_T ) (
localP -> P_27 == localP -> P_28 ) ; localB -> B_30_51_0 = ( uint8_T ) (
localP -> P_27 == localP -> P_29 ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( localDW -> Circulatingcurrentsuppression_SubsysRanBC ) ; } }
void normal_operation_no_faults_Circulatingcurrentsuppression_Update (
SimStruct * S , real_T rtu_I_arm , real_T rtu_I_arm_a , real_T rtu_I_arm_e ,
real_T rtu_I_arm_f , real_T rtu_I_arm_g , real_T rtu_I_arm_ep ,
B_Circulatingcurrentsuppression_normal_operation_no_faults_T * localB ,
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T * localDW ) {
int32_T isHit ; if ( localDW -> Circulatingcurrentsuppression_MODE ) { isHit
= ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { localDW ->
UnitDelay9_DSTATE [ 0 ] = rtu_I_arm ; localDW -> UnitDelay9_DSTATE [ 1 ] =
rtu_I_arm_a ; localDW -> UnitDelay9_DSTATE [ 2 ] = rtu_I_arm_e ; localDW ->
UnitDelay9_DSTATE [ 3 ] = rtu_I_arm_f ; localDW -> UnitDelay9_DSTATE [ 4 ] =
rtu_I_arm_g ; localDW -> UnitDelay9_DSTATE [ 5 ] = rtu_I_arm_ep ; localDW ->
UnitDelay1_DSTATE = localB -> B_30_43_0 ; } } } void
normal_operation_no_faults_Circulatingcurrentsuppression_Deriv ( SimStruct *
S , B_Circulatingcurrentsuppression_normal_operation_no_faults_T * localB ,
DW_Circulatingcurrentsuppression_normal_operation_no_faults_T * localDW ,
XDot_Circulatingcurrentsuppression_normal_operation_no_faults_T * localXdot )
{ if ( localDW -> Circulatingcurrentsuppression_MODE ) { localXdot ->
Integrator_CSTATE_f = localB -> B_30_29_0 ; localXdot -> Integrator_CSTATE_f0
= localB -> B_30_34_0 ; } else { { real_T * dx ; int_T i ; dx = & ( localXdot
-> Integrator_CSTATE_f ) ; for ( i = 0 ; i < 2 ; i ++ ) { dx [ i ] = 0.0 ; }
} } } void normal_operation_no_faults_Circulatingcurrentsuppression_ZC (
SimStruct * S , B_Circulatingcurrentsuppression_normal_operation_no_faults_T
* localB , DW_Circulatingcurrentsuppression_normal_operation_no_faults_T *
localDW , P_Circulatingcurrentsuppression_normal_operation_no_faults_T *
localP , ZCV_Circulatingcurrentsuppression_normal_operation_no_faults_T *
localZCSV ) { if ( localDW -> Circulatingcurrentsuppression_MODE ) {
localZCSV -> Saturation1_UprLim_ZC [ 0 ] = localB -> B_30_55_0 [ 0 ] - localP
-> P_31 ; localZCSV -> Saturation1_LwrLim_ZC [ 0 ] = localB -> B_30_55_0 [ 0
] - localP -> P_32 ; localZCSV -> Saturation1_UprLim_ZC [ 1 ] = localB ->
B_30_55_0 [ 1 ] - localP -> P_31 ; localZCSV -> Saturation1_LwrLim_ZC [ 1 ] =
localB -> B_30_55_0 [ 1 ] - localP -> P_32 ; localZCSV ->
Saturation1_UprLim_ZC [ 2 ] = localB -> B_30_55_0 [ 2 ] - localP -> P_31 ;
localZCSV -> Saturation1_LwrLim_ZC [ 2 ] = localB -> B_30_55_0 [ 2 ] - localP
-> P_32 ; } else { { real_T * zcsv = & ( localZCSV -> Saturation1_UprLim_ZC [
0 ] ) ; int_T i ; for ( i = 0 ; i < 6 ; i ++ ) { zcsv [ i ] = 0.0 ; } } } }
void normal_operation_no_faults_Circulatingcurrentsuppression_Term (
SimStruct * const S ) { } void
normal_operation_no_faults_Subsystempi2delay_d_Init ( SimStruct * S ,
B_Subsystempi2delay_normal_operation_no_faults_e_T * localB ,
P_Subsystempi2delay_normal_operation_no_faults_g_T * localP ) { localB ->
B_34_0_0 = localP -> P_0 [ 0 ] ; localB -> B_34_1_0 = localP -> P_0 [ 1 ] ; }
void normal_operation_no_faults_Subsystempi2delay_j_Disable ( SimStruct * S ,
DW_Subsystempi2delay_normal_operation_no_faults_c_T * localDW ) { localDW ->
Subsystempi2delay_MODE = false ; } void
normal_operation_no_faults_Subsystempi2delay_m ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_normal_operation_no_faults_e_T * localB ,
DW_Subsystempi2delay_normal_operation_no_faults_c_T * localDW ) { int32_T
isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0 ) { if ( ! localDW ->
Subsystempi2delay_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S )
) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Subsystempi2delay_MODE = true ; } } else { if ( localDW ->
Subsystempi2delay_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
normal_operation_no_faults_Subsystempi2delay_j_Disable ( S , localDW ) ; } }
} if ( localDW -> Subsystempi2delay_MODE ) { localB -> B_34_0_0 =
rtu_alpha_beta [ 0 ] * muDoubleScalarSin ( rtu_wt ) - rtu_alpha_beta [ 1 ] *
muDoubleScalarCos ( rtu_wt ) ; localB -> B_34_1_0 = rtu_alpha_beta [ 0 ] *
muDoubleScalarCos ( rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarSin (
rtu_wt ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Subsystempi2delay_SubsysRanBC ) ; } } } void
normal_operation_no_faults_Subsystempi2delay_k_Term ( SimStruct * const S ) {
} void normal_operation_no_faults_Subsystem1_i_Init ( SimStruct * S ,
B_Subsystem1_normal_operation_no_faults_p_T * localB ,
P_Subsystem1_normal_operation_no_faults_j_T * localP ) { localB -> B_35_0_0 =
localP -> P_0 [ 0 ] ; localB -> B_35_1_0 = localP -> P_0 [ 1 ] ; } void
normal_operation_no_faults_Subsystem1_m_Disable ( SimStruct * S ,
DW_Subsystem1_normal_operation_no_faults_d_T * localDW ) { localDW ->
Subsystem1_MODE = false ; } void normal_operation_no_faults_Subsystem1_h4 (
SimStruct * S , uint8_T rtu_Enable , const real_T rtu_alpha_beta [ 2 ] ,
real_T rtu_wt , B_Subsystem1_normal_operation_no_faults_p_T * localB ,
DW_Subsystem1_normal_operation_no_faults_d_T * localDW ) { int32_T isHit ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0 ) { if ( ! localDW ->
Subsystem1_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Subsystem1_MODE = true ; } } else { if ( localDW -> Subsystem1_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
normal_operation_no_faults_Subsystem1_m_Disable ( S , localDW ) ; } } } if (
localDW -> Subsystem1_MODE ) { localB -> B_35_0_0 = rtu_alpha_beta [ 0 ] *
muDoubleScalarCos ( rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarSin (
rtu_wt ) ; localB -> B_35_1_0 = - rtu_alpha_beta [ 0 ] * muDoubleScalarSin (
rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarCos ( rtu_wt ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Subsystem1_SubsysRanBC ) ; } } } void
normal_operation_no_faults_Subsystem1_b_Term ( SimStruct * const S ) { } void
normal_operation_no_faults_AutomaticGainControl_Init ( SimStruct * S ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ,
X_AutomaticGainControl_normal_operation_no_faults_T * localX ) { localX ->
VariableTransportDelay_CSTATE_n = 0.0 ; localX -> integrator_CSTATE_av =
localP -> P_3 ; localDW -> Memory_PreviousInput = localP -> P_5 ; localX ->
VariableTransportDelay_CSTATE_c = 0.0 ; localX -> integrator_CSTATE_cr =
localP -> P_8 ; localDW -> Memory_PreviousInput_n = localP -> P_10 ;
normal_operation_no_faults_Subsystempi2delay_d_Init ( S , & localB ->
Subsystempi2delay , & localP -> Subsystempi2delay ) ;
normal_operation_no_faults_Subsystem1_i_Init ( S , & localB -> Subsystem1 , &
localP -> Subsystem1 ) ; localB -> B_42_19_0 = localP -> P_0 ; } void
normal_operation_no_faults_AutomaticGainControl_Disable ( SimStruct * S ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ) { if (
localDW -> Subsystempi2delay . Subsystempi2delay_MODE ) {
normal_operation_no_faults_Subsystempi2delay_j_Disable ( S , & localDW ->
Subsystempi2delay ) ; } if ( localDW -> Subsystem1 . Subsystem1_MODE ) {
normal_operation_no_faults_Subsystem1_m_Disable ( S , & localDW -> Subsystem1
) ; } localDW -> AutomaticGainControl_MODE = false ; } void
normal_operation_no_faults_AutomaticGainControl ( SimStruct * S , real_T
rtu_Enable , real_T rtu_Freq , real_T rtu_wt , const real_T rtu_Vabc [ 3 ] ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ,
X_AutomaticGainControl_normal_operation_no_faults_T * localX ,
XDis_AutomaticGainControl_normal_operation_no_faults_T * localXdis ) {
int32_T isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) &&
( ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0.0 ) { if ( ! localDW
-> AutomaticGainControl_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart
( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void )
memset ( & ( localXdis -> VariableTransportDelay_CSTATE_n ) , 0 , 4 * sizeof
( boolean_T ) ) ; localDW -> AutomaticGainControl_MODE = true ; } } else { if
( localDW -> AutomaticGainControl_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> VariableTransportDelay_CSTATE_n ) , 1 , 4 * sizeof ( boolean_T )
) ; normal_operation_no_faults_AutomaticGainControl_Disable ( S , localDW ) ;
} } } if ( localDW -> AutomaticGainControl_MODE ) { { real_T * * uBuffer = (
real_T * * ) & localDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 0 ] ;
real_T * * tBuffer = ( real_T * * ) & localDW -> VariableTransportDelay_PWORK
. TUbufferPtrs [ 1 ] ; real_T * * xBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK . TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT (
S ) ; real_T appliedDelay ; localB -> B_42_0_0 =
normal_operation_no_faults_acc_rt_VTDelayfindtDInterpolate ( localX ->
VariableTransportDelay_CSTATE_n , * tBuffer , * uBuffer , * xBuffer , localDW
-> VariableTransportDelay_IWORK . CircularBufSize , localDW ->
VariableTransportDelay_IWORK . Head , localDW -> VariableTransportDelay_IWORK
. Tail , & localDW -> VariableTransportDelay_IWORK . Last , simTime , 0.0 , 0
, ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) , localP -> P_2 , & appliedDelay ) ; } localB -> B_42_1_0 =
localX -> integrator_CSTATE_av ; localB -> B_42_2_0 = ssGetT ( S ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( ssIsMajorTimeStep ( S
) != 0 ) { localDW -> RelationalOperator_Mode = ( localB -> B_42_2_0 >=
localB -> B_42_3_0 ) ; } localB -> B_42_4_0 = localDW ->
RelationalOperator_Mode ; localB -> B_42_5_0 = localDW ->
Memory_PreviousInput ; } if ( localB -> B_42_4_0 ) { localB -> B_38_0_0 =
localB -> B_42_1_0 - localB -> B_42_0_0 ; localB -> B_38_1_0 = localB ->
B_38_0_0 * rtu_Freq ; localB -> B_42_7_0 = localB -> B_38_1_0 ; } else {
localB -> B_42_7_0 = localB -> B_42_5_0 ; } { real_T * * uBuffer = ( real_T *
* ) & localDW -> VariableTransportDelay_PWORK_d . TUbufferPtrs [ 0 ] ; real_T
* * tBuffer = ( real_T * * ) & localDW -> VariableTransportDelay_PWORK_d .
TUbufferPtrs [ 1 ] ; real_T * * xBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK_d . TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT
( S ) ; real_T appliedDelay ; localB -> B_42_8_0 =
normal_operation_no_faults_acc_rt_VTDelayfindtDInterpolate ( localX ->
VariableTransportDelay_CSTATE_c , * tBuffer , * uBuffer , * xBuffer , localDW
-> VariableTransportDelay_IWORK_o . CircularBufSize , localDW ->
VariableTransportDelay_IWORK_o . Head , localDW ->
VariableTransportDelay_IWORK_o . Tail , & localDW ->
VariableTransportDelay_IWORK_o . Last , simTime , 0.0 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
, localP -> P_7 , & appliedDelay ) ; } localB -> B_42_9_0 = localX ->
integrator_CSTATE_cr ; localB -> B_42_10_0 = ssGetT ( S ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( ssIsMajorTimeStep ( S
) != 0 ) { localDW -> RelationalOperator_Mode_j = ( localB -> B_42_10_0 >=
localB -> B_42_11_0 ) ; } localB -> B_42_12_0 = localDW ->
RelationalOperator_Mode_j ; localB -> B_42_13_0 = localDW ->
Memory_PreviousInput_n ; } if ( localB -> B_42_12_0 ) { localB -> B_39_0_0 =
localB -> B_42_9_0 - localB -> B_42_8_0 ; localB -> B_39_1_0 = localB ->
B_39_0_0 * rtu_Freq ; localB -> B_42_15_0 = localB -> B_39_1_0 ; } else {
localB -> B_42_15_0 = localB -> B_42_13_0 ; } localB -> B_42_16_0 . re =
localB -> B_42_7_0 ; localB -> B_42_16_0 . im = localB -> B_42_15_0 ; localB
-> B_42_17_0 = muDoubleScalarHypot ( localB -> B_42_16_0 . re , localB ->
B_42_16_0 . im ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { localDW ->
Saturation_MODE = localB -> B_42_17_0 >= localP -> P_11 ? 1 : localB ->
B_42_17_0 > localP -> P_12 ? 0 : - 1 ; } localB -> B_42_18_0 = localDW ->
Saturation_MODE == 1 ? localP -> P_11 : localDW -> Saturation_MODE == - 1 ?
localP -> P_12 : localB -> B_42_17_0 ; localB -> B_42_19_0 = 1.0 / localB ->
B_42_18_0 ; if ( rtu_Freq > localP -> P_13 ) { localB -> B_42_20_0 = localP
-> P_13 ; } else if ( rtu_Freq < localP -> P_14 ) { localB -> B_42_20_0 =
localP -> P_14 ; } else { localB -> B_42_20_0 = rtu_Freq ; } localB ->
B_42_21_0 = 1.0 / localB -> B_42_20_0 ; if ( rtu_Freq > localP -> P_15 ) {
localB -> B_42_22_0 = localP -> P_15 ; } else if ( rtu_Freq < localP -> P_16
) { localB -> B_42_22_0 = localP -> P_16 ; } else { localB -> B_42_22_0 =
rtu_Freq ; } localB -> B_42_23_0 = 1.0 / localB -> B_42_22_0 ; for ( isHit =
0 ; isHit < 3 ; isHit ++ ) { localB -> B_42_29_0 [ isHit ] = 0.0 ; localB ->
B_42_29_0 [ isHit ] += localP -> P_20 [ isHit ] * rtu_Vabc [ 0 ] ; localB ->
B_42_29_0 [ isHit ] += localP -> P_20 [ isHit + 3 ] * rtu_Vabc [ 1 ] ; localB
-> B_42_29_0 [ isHit ] += localP -> P_20 [ isHit + 6 ] * rtu_Vabc [ 2 ] ; }
localB -> B_42_30_0 [ 0 ] = localP -> P_21 * localB -> B_42_29_0 [ 0 ] ;
localB -> B_42_30_0 [ 1 ] = localP -> P_21 * localB -> B_42_29_0 [ 1 ] ;
localB -> B_42_30_0 [ 2 ] = localP -> P_21 * localB -> B_42_29_0 [ 2 ] ;
normal_operation_no_faults_Subsystempi2delay_m ( S , localB -> B_42_28_0 , &
localB -> B_42_30_0 [ 0 ] , rtu_wt , & localB -> Subsystempi2delay , &
localDW -> Subsystempi2delay ) ; normal_operation_no_faults_Subsystem1_h4 ( S
, localB -> B_42_26_0 , & localB -> B_42_30_0 [ 0 ] , rtu_wt , & localB ->
Subsystem1 , & localDW -> Subsystem1 ) ; if ( localB -> B_42_26_0 != 0 ) {
localB -> B_42_33_0 [ 0 ] = localB -> Subsystem1 . B_35_0_0 ; localB ->
B_42_33_0 [ 1 ] = localB -> Subsystem1 . B_35_1_0 ; } else { localB ->
B_42_33_0 [ 0 ] = localB -> Subsystempi2delay . B_34_0_0 ; localB ->
B_42_33_0 [ 1 ] = localB -> Subsystempi2delay . B_34_1_0 ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
AutomaticGainControl_SubsysRanBC ) ; } } } void
normal_operation_no_faults_AutomaticGainControlTID4 ( SimStruct * S ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ) { localB ->
B_42_3_0 = localP -> P_4 ; localB -> B_42_11_0 = localP -> P_9 ; localB ->
B_42_26_0 = ( uint8_T ) ( localP -> P_17 == localP -> P_18 ) ; localB ->
B_42_28_0 = ( uint8_T ) ( localP -> P_17 == localP -> P_19 ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
AutomaticGainControl_SubsysRanBC ) ; } } void
normal_operation_no_faults_AutomaticGainControl_Update ( SimStruct * S ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ,
X_AutomaticGainControl_normal_operation_no_faults_T * localX ) { int32_T
isHit ; if ( localDW -> AutomaticGainControl_MODE ) { { real_T * * uBuffer =
( real_T * * ) & localDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 0 ]
; real_T * * tBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK . TUbufferPtrs [ 1 ] ; real_T * * xBuffer = (
real_T * * ) & localDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 2 ] ;
real_T simTime = ssGetT ( S ) ; localDW -> VariableTransportDelay_IWORK .
Head = ( ( localDW -> VariableTransportDelay_IWORK . Head < ( localDW ->
VariableTransportDelay_IWORK . CircularBufSize - 1 ) ) ? ( localDW ->
VariableTransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( localDW ->
VariableTransportDelay_IWORK . Head == localDW ->
VariableTransportDelay_IWORK . Tail ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
VariableTransportDelay_IWORK . CircularBufSize , & localDW ->
VariableTransportDelay_IWORK . Tail , & localDW ->
VariableTransportDelay_IWORK . Head , & localDW ->
VariableTransportDelay_IWORK . Last , simTime - localP -> P_1 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , & localDW ->
VariableTransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ localDW ->
VariableTransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [ localDW ->
VariableTransportDelay_IWORK . Head ] = localB -> B_42_1_0 ; ( * xBuffer ) [
localDW -> VariableTransportDelay_IWORK . Head ] = localX ->
VariableTransportDelay_CSTATE_n ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { localDW -> Memory_PreviousInput = localB -> B_42_7_0 ; } {
real_T * * uBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK_d . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & localDW -> VariableTransportDelay_PWORK_d . TUbufferPtrs [ 1 ]
; real_T * * xBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK_d . TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT
( S ) ; localDW -> VariableTransportDelay_IWORK_o . Head = ( ( localDW ->
VariableTransportDelay_IWORK_o . Head < ( localDW ->
VariableTransportDelay_IWORK_o . CircularBufSize - 1 ) ) ? ( localDW ->
VariableTransportDelay_IWORK_o . Head + 1 ) : 0 ) ; if ( localDW ->
VariableTransportDelay_IWORK_o . Head == localDW ->
VariableTransportDelay_IWORK_o . Tail ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
VariableTransportDelay_IWORK_o . CircularBufSize , & localDW ->
VariableTransportDelay_IWORK_o . Tail , & localDW ->
VariableTransportDelay_IWORK_o . Head , & localDW ->
VariableTransportDelay_IWORK_o . Last , simTime - localP -> P_6 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , & localDW ->
VariableTransportDelay_IWORK_o . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ localDW ->
VariableTransportDelay_IWORK_o . Head ] = simTime ; ( * uBuffer ) [ localDW
-> VariableTransportDelay_IWORK_o . Head ] = localB -> B_42_9_0 ; ( * xBuffer
) [ localDW -> VariableTransportDelay_IWORK_o . Head ] = localX ->
VariableTransportDelay_CSTATE_c ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { localDW -> Memory_PreviousInput_n = localB -> B_42_15_0 ; }
} } void normal_operation_no_faults_AutomaticGainControl_Deriv ( SimStruct *
S , B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ,
XDot_AutomaticGainControl_normal_operation_no_faults_T * localXdot ) { if (
localDW -> AutomaticGainControl_MODE ) { { real_T instantDelay ; instantDelay
= localB -> B_42_21_0 ; if ( instantDelay > localP -> P_1 ) { instantDelay =
localP -> P_1 ; } if ( instantDelay < 0.0 ) { localXdot ->
VariableTransportDelay_CSTATE_n = 0 ; } else { localXdot ->
VariableTransportDelay_CSTATE_n = 1.0 / instantDelay ; } } localXdot ->
integrator_CSTATE_av = localB -> B_42_33_0 [ 0 ] ; { real_T instantDelay ;
instantDelay = localB -> B_42_23_0 ; if ( instantDelay > localP -> P_6 ) {
instantDelay = localP -> P_6 ; } if ( instantDelay < 0.0 ) { localXdot ->
VariableTransportDelay_CSTATE_c = 0 ; } else { localXdot ->
VariableTransportDelay_CSTATE_c = 1.0 / instantDelay ; } } localXdot ->
integrator_CSTATE_cr = localB -> B_42_33_0 [ 1 ] ; } else { { real_T * dx ;
int_T i ; dx = & ( localXdot -> VariableTransportDelay_CSTATE_n ) ; for ( i =
0 ; i < 4 ; i ++ ) { dx [ i ] = 0.0 ; } } } } void
normal_operation_no_faults_AutomaticGainControl_ZC ( SimStruct * S ,
B_AutomaticGainControl_normal_operation_no_faults_T * localB ,
DW_AutomaticGainControl_normal_operation_no_faults_T * localDW ,
P_AutomaticGainControl_normal_operation_no_faults_T * localP ,
ZCV_AutomaticGainControl_normal_operation_no_faults_T * localZCSV ) { if (
localDW -> AutomaticGainControl_MODE ) { localZCSV ->
RelationalOperator_RelopInput_ZC_b = localB -> B_42_2_0 - localB -> B_42_3_0
; localZCSV -> RelationalOperator_RelopInput_ZC_i = localB -> B_42_10_0 -
localB -> B_42_11_0 ; localZCSV -> Saturation_UprLim_ZC_b = localB ->
B_42_17_0 - localP -> P_11 ; localZCSV -> Saturation_LwrLim_ZC_b = localB ->
B_42_17_0 - localP -> P_12 ; } else { { real_T * zcsv = & ( localZCSV ->
RelationalOperator_RelopInput_ZC_b ) ; int_T i ; for ( i = 0 ; i < 4 ; i ++ )
{ zcsv [ i ] = 0.0 ; } } } } void
normal_operation_no_faults_AutomaticGainControl_Term ( SimStruct * const S )
{ } void normal_operation_no_faults_HarmonicGenerator_Init ( SimStruct * S ,
B_HarmonicGenerator_normal_operation_no_faults_T * localB ,
P_HarmonicGenerator_normal_operation_no_faults_T * localP ) { localB ->
B_122_16_0 [ 0 ] = localP -> P_0 ; localB -> B_122_30_0 [ 0 ] = localP -> P_1
; localB -> B_122_16_0 [ 1 ] = localP -> P_0 ; localB -> B_122_30_0 [ 1 ] =
localP -> P_1 ; localB -> B_122_16_0 [ 2 ] = localP -> P_0 ; localB ->
B_122_30_0 [ 2 ] = localP -> P_1 ; } void
normal_operation_no_faults_HarmonicGenerator_Disable ( SimStruct * S ,
B_HarmonicGenerator_normal_operation_no_faults_T * localB ,
DW_HarmonicGenerator_normal_operation_no_faults_T * localDW ,
P_HarmonicGenerator_normal_operation_no_faults_T * localP ) { localB ->
B_122_16_0 [ 0 ] = localP -> P_0 ; localB -> B_122_30_0 [ 0 ] = localP -> P_1
; localB -> B_122_16_0 [ 1 ] = localP -> P_0 ; localB -> B_122_30_0 [ 1 ] =
localP -> P_1 ; localB -> B_122_16_0 [ 2 ] = localP -> P_0 ; localB ->
B_122_30_0 [ 2 ] = localP -> P_1 ; localDW -> HarmonicGenerator_MODE = false
; } void normal_operation_no_faults_HarmonicGenerator ( SimStruct * S ,
real_T rtu_Enable , real_T rtu_wt ,
B_HarmonicGenerator_normal_operation_no_faults_T * localB ,
DW_HarmonicGenerator_normal_operation_no_faults_T * localDW ,
P_HarmonicGenerator_normal_operation_no_faults_T * localP ) { real_T
currentTime ; real_T rtb_B_122_0_0 ; real_T rtb_B_122_13_0 ; int32_T isHit ;
isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0.0 ) { if ( ! localDW
-> HarmonicGenerator_MODE ) { if ( ssGetTaskTime ( S , 2 ) != ssGetTStart ( S
) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
HarmonicGenerator_MODE = true ; } } else { if ( localDW ->
HarmonicGenerator_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
normal_operation_no_faults_HarmonicGenerator_Disable ( S , localB , localDW ,
localP ) ; } } } if ( localDW -> HarmonicGenerator_MODE ) { isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { rtb_B_122_0_0 = localP ->
P_2 * rtu_wt ; rtb_B_122_13_0 = ssGetTaskTime ( S , 2 ) ; currentTime =
ssGetTaskTime ( S , 2 ) ; if ( rtb_B_122_13_0 < localP -> P_11 ) {
rtb_B_122_13_0 = localP -> P_12 ; } else { rtb_B_122_13_0 = localP -> P_13 ;
} if ( currentTime < localP -> P_14 ) { currentTime = localP -> P_15 ; } else
{ currentTime = localP -> P_16 ; } rtb_B_122_13_0 += currentTime ; localB ->
B_122_16_0 [ 0 ] = muDoubleScalarSin ( ( rtb_B_122_0_0 + localB -> B_122_2_0
) + localB -> B_122_9_0 [ 0 ] ) * ( rtb_B_122_13_0 * localB -> B_122_10_0 ) ;
localB -> B_122_16_0 [ 1 ] = muDoubleScalarSin ( ( rtb_B_122_0_0 + localB ->
B_122_2_0 ) + localB -> B_122_9_0 [ 1 ] ) * ( rtb_B_122_13_0 * localB ->
B_122_10_0 ) ; localB -> B_122_16_0 [ 2 ] = muDoubleScalarSin ( (
rtb_B_122_0_0 + localB -> B_122_2_0 ) + localB -> B_122_9_0 [ 2 ] ) * (
rtb_B_122_13_0 * localB -> B_122_10_0 ) ; rtb_B_122_0_0 = localP -> P_17 *
rtu_wt ; localB -> B_122_30_0 [ 0 ] = muDoubleScalarSin ( ( rtb_B_122_0_0 +
localB -> B_122_19_0 ) + localB -> B_122_26_0 [ 0 ] ) * ( rtb_B_122_13_0 *
localB -> B_122_27_0 ) ; localB -> B_122_30_0 [ 1 ] = muDoubleScalarSin ( (
rtb_B_122_0_0 + localB -> B_122_19_0 ) + localB -> B_122_26_0 [ 1 ] ) * (
rtb_B_122_13_0 * localB -> B_122_27_0 ) ; localB -> B_122_30_0 [ 2 ] =
muDoubleScalarSin ( ( rtb_B_122_0_0 + localB -> B_122_19_0 ) + localB ->
B_122_26_0 [ 2 ] ) * ( rtb_B_122_13_0 * localB -> B_122_27_0 ) ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
HarmonicGenerator_SubsysRanBC ) ; } } } void
normal_operation_no_faults_HarmonicGeneratorTID4 ( SimStruct * S ,
B_HarmonicGenerator_normal_operation_no_faults_T * localB ,
DW_HarmonicGenerator_normal_operation_no_faults_T * localDW ,
P_HarmonicGenerator_normal_operation_no_faults_T * localP ) { localB ->
B_122_2_0 = localP -> P_4 * localP -> P_3 ; switch ( ( int32_T ) ( localP ->
P_5 + localP -> P_6 ) ) { case 1 : localB -> B_122_9_0 [ 0 ] = localP -> P_7
[ 0 ] ; localB -> B_122_9_0 [ 1 ] = localP -> P_7 [ 1 ] ; localB -> B_122_9_0
[ 2 ] = localP -> P_7 [ 2 ] ; break ; case 2 : localB -> B_122_9_0 [ 0 ] =
localP -> P_8 [ 0 ] ; localB -> B_122_9_0 [ 1 ] = localP -> P_8 [ 1 ] ;
localB -> B_122_9_0 [ 2 ] = localP -> P_8 [ 2 ] ; break ; default : localB ->
B_122_9_0 [ 0 ] = localP -> P_9 [ 0 ] ; localB -> B_122_9_0 [ 1 ] = localP ->
P_9 [ 1 ] ; localB -> B_122_9_0 [ 2 ] = localP -> P_9 [ 2 ] ; break ; }
localB -> B_122_10_0 = localP -> P_10 ; localB -> B_122_19_0 = localP -> P_19
* localP -> P_18 ; switch ( ( int32_T ) ( localP -> P_20 + localP -> P_21 ) )
{ case 1 : localB -> B_122_26_0 [ 0 ] = localP -> P_22 [ 0 ] ; localB ->
B_122_26_0 [ 1 ] = localP -> P_22 [ 1 ] ; localB -> B_122_26_0 [ 2 ] = localP
-> P_22 [ 2 ] ; break ; case 2 : localB -> B_122_26_0 [ 0 ] = localP -> P_23
[ 0 ] ; localB -> B_122_26_0 [ 1 ] = localP -> P_23 [ 1 ] ; localB ->
B_122_26_0 [ 2 ] = localP -> P_23 [ 2 ] ; break ; default : localB ->
B_122_26_0 [ 0 ] = localP -> P_24 [ 0 ] ; localB -> B_122_26_0 [ 1 ] = localP
-> P_24 [ 1 ] ; localB -> B_122_26_0 [ 2 ] = localP -> P_24 [ 2 ] ; break ; }
localB -> B_122_27_0 = localP -> P_25 ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( localDW -> HarmonicGenerator_SubsysRanBC ) ; } } void
normal_operation_no_faults_HarmonicGenerator_Term ( SimStruct * const S ) { }
void normal_operation_no_faults_Signalgenerator_Init ( SimStruct * S ,
B_Signalgenerator_normal_operation_no_faults_T * localB ,
DW_Signalgenerator_normal_operation_no_faults_T * localDW ,
P_Signalgenerator_normal_operation_no_faults_T * localP ) { localDW ->
DiscreteTimeIntegrator_DSTATE = localP -> P_16 ; localDW ->
DiscreteTimeIntegrator1_DSTATE = localP -> P_28 ; localDW -> UnitDelay_DSTATE
= localP -> P_33 ; localB -> B_127_1_0 = localP -> P_1 ; localB -> B_127_36_0
= localP -> P_2 ; localB -> B_127_39_0 = localP -> P_3 ; localB -> B_127_37_0
= localP -> P_4 ; localB -> B_127_41_0 = localP -> P_5 ; } void
normal_operation_no_faults_Signalgenerator_Disable ( SimStruct * S ,
B_Signalgenerator_normal_operation_no_faults_T * localB ,
DW_Signalgenerator_normal_operation_no_faults_T * localDW ,
P_Signalgenerator_normal_operation_no_faults_T * localP ) { localDW ->
DiscreteTimeIntegrator_DSTATE = localB -> B_127_9_0 ; localDW ->
DiscreteTimeIntegrator1_DSTATE = localB -> B_127_21_0 ; localB -> B_127_1_0 =
localP -> P_1 ; localB -> B_127_36_0 = localP -> P_2 ; localB -> B_127_39_0 =
localP -> P_3 ; localB -> B_127_37_0 = localP -> P_4 ; localB -> B_127_41_0 =
localP -> P_5 ; localDW -> Signalgenerator_MODE = false ; } void
normal_operation_no_faults_Signalgenerator ( SimStruct * S , real_T
rtu_Enable , B_Signalgenerator_normal_operation_no_faults_T * localB ,
DW_Signalgenerator_normal_operation_no_faults_T * localDW ,
P_Signalgenerator_normal_operation_no_faults_T * localP ) { real_T
rtb_B_127_11_0 ; real_T rtb_B_127_20_0 ; int32_T isHit ; real_T tmp ; isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( ( isHit != 0 ) && ( ssIsMajorTimeStep ( S
) != 0 ) ) { if ( rtu_Enable > 0.0 ) { if ( ! localDW -> Signalgenerator_MODE
) { if ( ssGetTaskTime ( S , 2 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Signalgenerator_MODE = true ; } } else { if ( localDW -> Signalgenerator_MODE
) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
normal_operation_no_faults_Signalgenerator_Disable ( S , localB , localDW ,
localP ) ; } } } if ( localDW -> Signalgenerator_MODE ) { isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { localB -> B_127_1_0 =
rt_Lookup ( localP -> P_6 , 6 , ssGetTaskTime ( S , 2 ) , localP -> P_7 ) ;
localB -> B_127_9_0 = localDW -> DiscreteTimeIntegrator_DSTATE ;
rtb_B_127_20_0 = ssGetTaskTime ( S , 2 ) ; if ( rtb_B_127_20_0 < localP ->
P_18 ) { rtb_B_127_11_0 = localP -> P_19 ; } else { rtb_B_127_11_0 = localP
-> P_20 ; } rtb_B_127_20_0 = ssGetTaskTime ( S , 2 ) ; if ( rtb_B_127_20_0 <
localP -> P_24 ) { rtb_B_127_20_0 = localP -> P_25 ; } else { rtb_B_127_20_0
= localP -> P_26 ; } localB -> B_127_21_0 = localDW ->
DiscreteTimeIntegrator1_DSTATE ; if ( rtb_B_127_11_0 >= localP -> P_34 ) {
switch ( ( int32_T ) localB -> B_127_14_0 ) { case 1 : localB -> B_127_28_0 =
localB -> B_127_19_0 * rtb_B_127_20_0 ; break ; case 2 : localB -> B_127_28_0
= localB -> B_127_21_0 ; break ; case 3 : localB -> B_127_28_0 =
muDoubleScalarSin ( localB -> B_127_21_0 * localB -> B_127_24_0 ) * localB ->
B_127_22_0 ; break ; default : localB -> B_127_28_0 = localB -> B_127_25_0 ;
break ; } } else { localB -> B_127_28_0 = localDW -> UnitDelay_DSTATE ; } if
( ( ! ( rtb_B_127_11_0 != 0.0 ) ) && localB -> B_127_17_0 ) { rtb_B_127_11_0
= localB -> B_127_10_0 ; } else { rtb_B_127_11_0 = localB -> B_127_28_0 ; }
if ( localB -> B_127_31_0 >= localP -> P_36 ) { tmp = rtb_B_127_11_0 ; } else
{ tmp = localB -> B_127_7_0 ; } localB -> B_127_33_0 = localP -> P_37 * tmp ;
localB -> B_127_36_0 = ( ( localB -> B_127_34_0 != 0.0 ) && ( localB ->
B_127_35_0 != 0.0 ) ) ; localB -> B_127_37_0 = localB -> B_127_9_0 ; if (
localB -> B_127_35_0 >= localP -> P_38 ) { localB -> B_127_39_0 =
rtb_B_127_11_0 ; } else { localB -> B_127_39_0 = localB -> B_127_3_0 ; } if (
localB -> B_127_38_0 >= localP -> P_39 ) { localB -> B_127_41_0 = localP ->
P_0 * rtb_B_127_11_0 ; } else { localB -> B_127_41_0 = localB -> B_127_6_0 ;
} if ( rtb_B_127_20_0 >= localP -> P_45 ) { localB -> B_127_48_0 = localB ->
B_127_47_0 ; } else { localB -> B_127_48_0 = localB -> B_127_44_0 ; } } if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Signalgenerator_SubsysRanBC ) ; } } } void
normal_operation_no_faults_SignalgeneratorTID4 ( SimStruct * S ,
B_Signalgenerator_normal_operation_no_faults_T * localB ,
DW_Signalgenerator_normal_operation_no_faults_T * localDW ,
P_Signalgenerator_normal_operation_no_faults_T * localP ) { localB ->
B_127_3_0 = localP -> P_9 ; localB -> B_127_6_0 = localP -> P_12 ; localB ->
B_127_7_0 = localP -> P_13 ; localB -> B_127_10_0 = localP -> P_17 ; localB
-> B_127_14_0 = localP -> P_21 ; localB -> B_127_17_0 = ( localB ->
B_127_14_0 == localP -> P_22 ) ; localB -> B_127_19_0 = localP -> P_23 ;
localB -> B_127_22_0 = localP -> P_29 ; localB -> B_127_24_0 = localP -> P_31
* localP -> P_30 ; localB -> B_127_25_0 = localP -> P_32 ; localB ->
B_127_31_0 = ( localP -> P_35 == localP -> P_11 ) ; localB -> B_127_34_0 = (
localB -> B_127_14_0 == localP -> P_14 ) ; localB -> B_127_35_0 = ( localP ->
P_35 == localP -> P_8 ) ; localB -> B_127_38_0 = ( localP -> P_35 == localP
-> P_10 ) ; localB -> B_127_44_0 = localP -> P_42 ; if ( ( localB ->
B_127_14_0 == localP -> P_40 ) >= localP -> P_44 ) { localB -> B_127_47_0 =
localP -> P_43 ; } else { localB -> B_127_47_0 = localP -> P_41 ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Signalgenerator_SubsysRanBC ) ; } } void
normal_operation_no_faults_Signalgenerator_Update ( SimStruct * S ,
B_Signalgenerator_normal_operation_no_faults_T * localB ,
DW_Signalgenerator_normal_operation_no_faults_T * localDW ,
P_Signalgenerator_normal_operation_no_faults_T * localP ) { int32_T isHit ;
if ( localDW -> Signalgenerator_MODE ) { isHit = ssIsSampleHit ( S , 2 , 0 )
; if ( isHit != 0 ) { localDW -> DiscreteTimeIntegrator_DSTATE += localP ->
P_15 * localB -> B_127_33_0 ; localDW -> DiscreteTimeIntegrator1_DSTATE +=
localP -> P_27 * localB -> B_127_48_0 ; localDW -> UnitDelay_DSTATE = localB
-> B_127_28_0 ; } } } void normal_operation_no_faults_Signalgenerator_Term (
SimStruct * const S ) { } static void mdlOutputs ( SimStruct * S , int_T tid
) { real_T B_185_110_0 [ 3 ] ; real_T B_185_258_0 [ 3 ] ; real_T B_185_288_0
[ 3 ] ; real_T B_185_312_0 [ 50 ] ; real_T B_185_495_0 [ 3 ] ; real_T
B_185_698_0 [ 3 ] ; real_T B_185_805_0 ; real_T B_185_813_0 ; real_T
B_185_827_0 ; real_T B_185_835_0 ; real_T B_185_849_0 ; real_T B_185_857_0 ;
real_T B_185_871_0 ; real_T B_185_879_0 ; real_T B_185_893_0 ; real_T
B_185_901_0 ; real_T B_185_915_0 ; real_T B_185_923_0 ; real_T B_185_2285_0 [
3 ] ; real_T B_185_2293_0 [ 3 ] ; real_T B_185_2303_0 [ 3 ] ; real_T
B_185_2311_0 [ 3 ] ; real_T B_185_2343_0 [ 3 ] ; real_T B_185_2351_0 [ 3 ] ;
real_T B_185_2361_0 [ 3 ] ; real_T B_185_2369_0 [ 3 ] ; real_T B_185_2401_0 [
3 ] ; real_T B_185_2409_0 [ 3 ] ; real_T B_185_2419_0 [ 3 ] ; real_T
B_185_2427_0 [ 3 ] ; real_T B_185_2459_0 [ 3 ] ; real_T B_185_2467_0 [ 3 ] ;
real_T B_185_2477_0 [ 3 ] ; real_T B_185_2485_0 [ 3 ] ; real_T B_185_2575_0 [
3 ] ; real_T B_185_2583_0 [ 3 ] ; real_T B_185_2593_0 [ 3 ] ; real_T
B_185_2601_0 [ 3 ] ; real_T B_119_2_0 [ 50 ] ; real_T B_118_2_0 [ 50 ] ;
real_T B_117_2_0 [ 50 ] ; real_T B_116_2_0 [ 50 ] ; real_T B_115_2_0 [ 50 ] ;
real_T B_114_2_0 [ 50 ] ; real_T B_85_2_0 [ 50 ] ; real_T B_84_2_0 [ 50 ] ;
real_T B_83_2_0 [ 50 ] ; real_T B_82_2_0 [ 50 ] ; real_T B_81_2_0 [ 50 ] ;
real_T B_80_2_0 [ 50 ] ; real_T B_51_2_0 [ 50 ] ; real_T B_50_2_0 [ 50 ] ;
real_T B_49_2_0 [ 50 ] ; real_T B_48_2_0 [ 50 ] ; real_T B_47_2_0 [ 50 ] ;
real_T B_46_2_0 [ 50 ] ; real_T B_15_2_0 [ 50 ] ; real_T B_14_2_0 [ 50 ] ;
real_T B_13_2_0 [ 50 ] ; real_T B_12_2_0 [ 50 ] ; real_T B_11_2_0 [ 50 ] ;
real_T B_4_15_0 [ 3 ] ; real_T B_119_0_0 [ 50 ] ; real_T B_118_0_0 [ 50 ] ;
real_T B_117_0_0 [ 50 ] ; real_T B_116_0_0 [ 50 ] ; real_T B_115_0_0 [ 50 ] ;
real_T B_114_0_0 [ 50 ] ; real_T B_85_0_0 [ 50 ] ; real_T B_84_0_0 [ 50 ] ;
real_T B_83_0_0 [ 50 ] ; real_T B_82_0_0 [ 50 ] ; real_T B_81_0_0 [ 50 ] ;
real_T B_80_0_0 [ 50 ] ; real_T B_51_0_0 [ 50 ] ; real_T B_50_0_0 [ 50 ] ;
real_T B_49_0_0 [ 50 ] ; real_T B_48_0_0 [ 50 ] ; real_T B_47_0_0 [ 50 ] ;
real_T B_46_0_0 [ 50 ] ; real_T B_15_0_0 [ 50 ] ; real_T B_14_0_0 [ 50 ] ;
real_T B_13_0_0 [ 50 ] ; real_T B_12_0_0 [ 50 ] ; real_T B_11_0_0 [ 50 ] ;
boolean_T stateChanged ; boolean_T didZcEventOccur ; real_T * lastU ; real_T
rtb_B_114_7_0 ; real_T rtb_B_116_7_0 ; real_T rtb_B_118_7_0 ; real_T
rtb_B_115_7_0 ; real_T rtb_B_119_7_0 ; real_T rtb_B_185_124_0 [ 3 ] ; real_T
rtb_B_83_7_0 ; real_T rtb_B_82_7_0 ; real_T rtb_B_84_7_0 ; real_T
rtb_B_81_7_0 ; real_T rtb_B_85_7_0 ; real_T rtb_B_13_7_0 ; real_T
rtb_B_15_7_0 ; real_T rtb_B_12_7_0 ; real_T rtb_B_16_3_0 ; real_T
rtb_B_49_7_0 ; real_T rtb_B_48_7_0 ; real_T rtb_B_50_7_0 ; real_T
rtb_B_47_7_0 ; real_T rtb_B_51_7_0 ; boolean_T rtb_B_51_4_0 [ 50 ] ; real_T
rtb_B_4_33_0 ; real_T rtb_B_185_1941_0 ; real_T rtb_B_185_1940_0 ; real_T
rtb_B_185_1645_0 ; real_T rtb_B_185_1644_0 ; real_T rtb_B_185_1349_0 ; real_T
rtb_B_185_1348_0 ; real_T rtb_B_185_280_0 ; real_T rtb_B_185_40_0 ; real_T
rtb_B_185_2649_0 ; real_T rtb_B_185_2635_0 ; int8_T tmpForInput [ 50 ] ;
real_T rtb_B_185_3396_0 ; real_T rtb_B_185_3392_0 ; real_T rtb_B_185_3342_0 ;
real_T rtb_B_185_3030_0 ; real_T rtb_B_185_3289_0 ; real_T rtb_B_185_3236_0 ;
real_T rtb_B_185_3184_0 ; real_T rtb_B_185_2643_0 ; boolean_T
rtb_B_185_2288_0 ; real_T rtb_B_185_1936_0 ; real_T rtb_B_185_1934_0 ; real_T
rtb_B_185_1640_0 ; real_T rtb_B_185_1638_0 ; real_T rtb_B_185_1344_0 ; real_T
rtb_B_185_1342_0 ; real_T rtb_B_185_1070_0 ; real_T rtb_B_185_1024_0 ; real_T
rtb_B_185_978_0 ; real_T rtb_B_185_2_0 [ 3 ] ; real_T rtb_B_185_34_0 ; real_T
rtb_B_185_30_0 ; real_T rtb_B_185_236_0 ; int32_T i ; real_T rtb_B_185_23_0 ;
real_T rtb_B_185_25_0 ; real_T B_185_223_0_idx_1 ; real_T B_185_223_0_idx_0 ;
real_T B_185_23_0_idx_2 ; real_T B_185_23_0_idx_1 ; real_T B_185_23_0_idx_0 ;
real_T B_185_2616_0_idx_2 ; real_T B_185_2616_1_idx_2 ; real_T
B_185_2616_0_idx_1 ; real_T B_185_2616_1_idx_1 ; real_T B_185_2616_0_idx_0 ;
real_T B_185_2616_1_idx_0 ; real_T B_185_25_0_idx_2 ; real_T B_185_25_0_idx_1
; real_T B_185_25_0_idx_0 ; real_T B_185_2500_1_idx_2 ; real_T
B_185_2500_1_idx_1 ; real_T B_185_2500_1_idx_0 ; real_T B_185_2500_0_idx_2 ;
real_T B_185_2500_0_idx_1 ; real_T B_185_2500_0_idx_0 ; real_T
B_185_2442_1_idx_2 ; real_T B_185_2442_1_idx_1 ; real_T B_185_2442_1_idx_0 ;
real_T B_185_2442_0_idx_2 ; real_T B_185_2442_0_idx_1 ; real_T
B_185_2442_0_idx_0 ; real_T B_185_2384_1_idx_2 ; real_T B_185_2384_1_idx_1 ;
real_T B_185_2384_1_idx_0 ; real_T B_185_2384_0_idx_1 ; real_T
B_185_2384_0_idx_0 ; real_T B_185_787_0_idx_1 ; real_T B_185_787_0_idx_0 ;
real_T B_185_774_0_idx_1 ; real_T B_185_761_0_idx_1 ; real_T
B_185_748_0_idx_1 ; real_T B_185_735_0_idx_1 ; real_T B_185_722_0_idx_1 ;
real_T B_185_722_0_idx_0 ; real_T B_185_584_0_idx_1 ; real_T
B_185_615_0_idx_1 ; real_T B_185_613_0_idx_1 ; real_T B_185_615_0_idx_0 ;
real_T B_185_613_0_idx_0 ; real_T B_185_571_0_idx_1 ; real_T
B_185_558_0_idx_1 ; real_T B_185_545_0_idx_1 ; real_T B_185_532_0_idx_1 ;
real_T B_185_519_0_idx_1 ; real_T B_185_519_0_idx_0 ; real_T
B_185_200_0_idx_1 ; real_T B_185_412_0_idx_1 ; real_T B_185_410_0_idx_1 ;
real_T B_185_412_0_idx_0 ; real_T B_185_410_0_idx_0 ; real_T
B_185_187_0_idx_1 ; real_T B_185_174_0_idx_1 ; real_T B_185_161_0_idx_1 ;
real_T B_185_148_0_idx_1 ; real_T B_185_135_0_idx_1 ; real_T
B_185_135_0_idx_0 ; real_T B_185_381_0_idx_1 ; real_T B_185_381_0_idx_0 ;
real_T B_185_368_0_idx_1 ; real_T B_185_355_0_idx_1 ; real_T
B_185_342_0_idx_1 ; real_T B_185_329_0_idx_1 ; real_T B_185_316_0_idx_1 ;
real_T B_185_316_0_idx_0 ; real_T B_185_228_0_idx_1 ; real_T
B_185_226_0_idx_1 ; real_T B_185_228_0_idx_0 ; real_T B_185_226_0_idx_0 ;
real_T B_185_619_0_idx_1 ; real_T B_185_619_0_idx_0 ; real_T
B_185_615_0_idx_2 ; real_T B_185_613_0_idx_2 ; real_T B_185_416_0_idx_1 ;
real_T B_185_416_0_idx_0 ; real_T B_185_412_0_idx_2 ; real_T
B_185_410_0_idx_2 ; real_T B_185_29_0_idx_1 ; real_T B_185_29_0_idx_0 ;
real_T B_185_228_0_idx_2 ; real_T B_185_226_0_idx_2 ; real_T B_4_25_0_idx_1 ;
real_T B_4_25_0_idx_0 ; int32_T isHit ; B_normal_operation_no_faults_T * _rtB
; P_normal_operation_no_faults_T * _rtP ; X_normal_operation_no_faults_T *
_rtX ; PrevZCX_normal_operation_no_faults_T * _rtZCE ;
DW_normal_operation_no_faults_T * _rtDW ; _rtDW = ( (
DW_normal_operation_no_faults_T * ) ssGetRootDWork ( S ) ) ; _rtZCE = ( (
PrevZCX_normal_operation_no_faults_T * ) _ssGetPrevZCSigState ( S ) ) ; _rtX
= ( ( X_normal_operation_no_faults_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_normal_operation_no_faults_T * ) _ssGetModelBlockIO ( S ) ) ; isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_2_0 [ 0 ] = _rtDW
-> UnitDelay5_DSTATE [ 0 ] ; rtb_B_185_2_0 [ 1 ] = _rtDW -> UnitDelay5_DSTATE
[ 1 ] ; rtb_B_185_2_0 [ 2 ] = _rtDW -> UnitDelay5_DSTATE [ 2 ] ; isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_3_0 [ 0 ] =
rtb_B_185_2_0 [ 0 ] ; _rtB -> B_185_3_0 [ 1 ] = rtb_B_185_2_0 [ 1 ] ; _rtB ->
B_185_3_0 [ 2 ] = rtb_B_185_2_0 [ 2 ] ; } } isHit = ssIsSampleHit ( S , 3 , 0
) ; if ( isHit != 0 ) { for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { _rtB ->
B_185_7_0 [ isHit ] = _rtP -> P_115 * ( _rtP -> P_112 * _rtB -> B_185_3_0 [ 2
] * _rtP -> P_113 * _rtP -> P_114 [ isHit + 6 ] + ( _rtP -> P_112 * _rtB ->
B_185_3_0 [ 1 ] * _rtP -> P_113 * _rtP -> P_114 [ isHit + 3 ] + _rtP -> P_112
* _rtB -> B_185_3_0 [ 0 ] * _rtP -> P_113 * _rtP -> P_114 [ isHit ] ) ) ; } }
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> RelationalOperator_Mode = ( _rtX ->
Integrator_CSTATE > _rtB -> B_185_8_0 ) ; } _rtB -> B_185_9_0 = _rtDW ->
RelationalOperator_Mode ; } _rtB -> B_185_10_0 = _rtX -> Integrator_CSTATE -
_rtB -> B_185_8_0 ; if ( ( _rtDW -> Initial_FirstOutputTime == ( rtMinusInf )
) || ( _rtDW -> Initial_FirstOutputTime == ssGetTaskTime ( S , 0 ) ) ) {
_rtDW -> Initial_FirstOutputTime = ssGetTaskTime ( S , 0 ) ; _rtB ->
B_185_11_0 = _rtP -> P_117 ; } else { _rtB -> B_185_11_0 = _rtB -> B_185_10_0
; } if ( ssIsMajorTimeStep ( S ) != 0 ) { rtb_B_185_2288_0 = false ;
stateChanged = false ; didZcEventOccur = ( _rtB -> B_185_9_0 && ( _rtZCE ->
Integrator_Reset_ZCE != POS_ZCSIG ) ) ; _rtZCE -> Integrator_Reset_ZCE = _rtB
-> B_185_9_0 ; if ( didZcEventOccur || ( _rtDW -> Integrator_IWORK != 0 ) ) {
rtb_B_185_2288_0 = true ; _rtX -> Integrator_CSTATE = _rtB -> B_185_11_0 ;
stateChanged = true ; } if ( rtb_B_185_2288_0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( stateChanged ) {
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } } if ( _rtX ->
Integrator_CSTATE >= _rtP -> P_118 ) { _rtX -> Integrator_CSTATE = _rtP ->
P_118 ; } else { if ( _rtX -> Integrator_CSTATE <= _rtP -> P_119 ) { _rtX ->
Integrator_CSTATE = _rtP -> P_119 ; } } } _rtB -> B_185_12_0 = _rtX ->
Integrator_CSTATE ; normal_operation_no_faults_Subsystem1_h4 ( S , _rtB ->
B_185_15_0 , & _rtB -> B_185_7_0 [ 0 ] , _rtB -> B_185_12_0 , & _rtB ->
Subsystem1_b , & _rtDW -> Subsystem1_b ) ;
normal_operation_no_faults_Subsystempi2delay_m ( S , _rtB -> B_185_18_0 , &
_rtB -> B_185_7_0 [ 0 ] , _rtB -> B_185_12_0 , & _rtB -> Subsystempi2delay_m1
, & _rtDW -> Subsystempi2delay_m1 ) ; if ( _rtB -> B_185_15_0 != 0 ) { _rtB
-> B_185_20_0 [ 0 ] = _rtB -> Subsystem1_b . B_35_0_0 ; _rtB -> B_185_20_0 [
1 ] = _rtB -> Subsystem1_b . B_35_1_0 ; } else { _rtB -> B_185_20_0 [ 0 ] =
_rtB -> Subsystempi2delay_m1 . B_34_0_0 ; _rtB -> B_185_20_0 [ 1 ] = _rtB ->
Subsystempi2delay_m1 . B_34_1_0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if
( isHit != 0 ) { B_185_23_0_idx_0 = _rtDW -> Delay_x1_DSTATE [ 0 ] ;
B_185_25_0_idx_0 = _rtDW -> Delay_x2_DSTATE [ 0 ] ; B_185_23_0_idx_1 = _rtDW
-> Delay_x1_DSTATE [ 1 ] ; B_185_25_0_idx_1 = _rtDW -> Delay_x2_DSTATE [ 1 ]
; B_185_23_0_idx_2 = _rtDW -> Delay_x1_DSTATE [ 2 ] ; B_185_25_0_idx_2 =
_rtDW -> Delay_x2_DSTATE [ 2 ] ; _rtB -> B_185_28_0 [ 0 ] = ( _rtP -> P_126 *
_rtDW -> Delay_x1_DSTATE [ 0 ] + _rtP -> P_128 * _rtDW -> Delay_x2_DSTATE [ 0
] ) + _rtP -> P_124 * _rtB -> B_185_20_0 [ 0 ] ; _rtB -> B_185_28_0 [ 1 ] = (
_rtP -> P_126 * _rtDW -> Delay_x1_DSTATE [ 1 ] + _rtP -> P_128 * _rtDW ->
Delay_x2_DSTATE [ 1 ] ) + _rtP -> P_124 * _rtB -> B_185_20_0 [ 1 ] ; _rtB ->
B_185_28_0 [ 2 ] = ( _rtP -> P_126 * _rtDW -> Delay_x1_DSTATE [ 2 ] + _rtP ->
P_128 * _rtDW -> Delay_x2_DSTATE [ 2 ] ) + _rtP -> P_124 * _rtB -> B_185_21_0
; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_30_0
= ssGetTaskTime ( S , 1 ) ; _rtDW -> Step_MODE [ 0 ] = ( rtb_B_185_30_0 >=
_rtP -> P_129 ) ; if ( _rtDW -> Step_MODE [ 0 ] == 1 ) { B_185_29_0_idx_0 =
_rtP -> P_131 [ 0 ] ; } else { B_185_29_0_idx_0 = _rtP -> P_130 [ 0 ] ; }
_rtDW -> Step_MODE [ 1 ] = ( rtb_B_185_30_0 >= _rtP -> P_129 ) ; if ( _rtDW
-> Step_MODE [ 1 ] == 1 ) { B_185_29_0_idx_1 = _rtP -> P_131 [ 1 ] ; } else {
B_185_29_0_idx_1 = _rtP -> P_130 [ 1 ] ; } } isHit = ssIsSampleHit ( S , 2 ,
0 ) ; if ( isHit != 0 ) { rtb_B_185_30_0 = _rtDW -> UnitDelay1_DSTATE ; isHit
= ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_31_0 =
rtb_B_185_30_0 ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 )
{ _rtB -> B_185_33_0 = _rtB -> B_185_32_0 - _rtB -> B_185_31_0 ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_30_0 =
ssGetTaskTime ( S , 1 ) ; _rtDW ->
DCregulatorTURNONidreferenceforrectifier_MODE = ( rtb_B_185_30_0 >= _rtP ->
P_134 ) ; if ( _rtDW -> DCregulatorTURNONidreferenceforrectifier_MODE == 1 )
{ rtb_B_185_34_0 = _rtP -> P_136 ; } else { rtb_B_185_34_0 = _rtP -> P_135 ;
} if ( rtb_B_185_34_0 > _rtP -> P_138 ) { rtb_B_185_30_0 = _rtB -> B_185_33_0
; } else { rtb_B_185_30_0 = _rtB -> B_185_35_0 ; } _rtB -> B_185_37_0 = _rtP
-> P_139 * rtb_B_185_30_0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if (
isHit != 0 ) { rtb_B_185_30_0 = _rtP -> P_140 * _rtB -> B_185_37_0 ;
rtb_B_185_40_0 = _rtDW -> DiscreteTimeIntegrator1_DSTATE + rtb_B_185_30_0 ;
if ( ( ( rtb_B_185_40_0 >= _rtB -> B_185_41_0 ) && ( _rtB -> B_185_37_0 >
normal_operation_no_faults_rtC ( S ) -> B_185_43_0 ) ) || ( ( rtb_B_185_40_0
<= _rtB -> B_185_46_0 ) && ( _rtB -> B_185_37_0 <
normal_operation_no_faults_rtC ( S ) -> B_185_48_0 ) ) ) { _rtB -> B_185_53_0
= _rtB -> B_185_39_0 ; } else { _rtB -> B_185_53_0 = _rtP -> P_146 * _rtB ->
B_185_37_0 ; } if ( _rtDW -> DiscreteTimeIntegrator1_SYSTEM_ENABLE != 0 ) {
_rtB -> B_185_54_0 = _rtDW -> DiscreteTimeIntegrator1_DSTATE ; } else { _rtB
-> B_185_54_0 = _rtP -> P_147 * _rtB -> B_185_53_0 + _rtDW ->
DiscreteTimeIntegrator1_DSTATE ; } if ( _rtB -> B_185_54_0 >= _rtP -> P_149 )
{ _rtB -> B_185_54_0 = _rtP -> P_149 ; } else { if ( _rtB -> B_185_54_0 <=
_rtP -> P_150 ) { _rtB -> B_185_54_0 = _rtP -> P_150 ; } } _rtB -> B_185_55_0
= rtb_B_185_30_0 + _rtB -> B_185_54_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( isHit != 0 ) { if ( _rtB -> B_185_55_0 > B_185_29_0_idx_0 ) { _rtB ->
B_185_58_0 = B_185_29_0_idx_0 ; } else if ( _rtB -> B_185_55_0 <
B_185_29_0_idx_1 ) { _rtB -> B_185_58_0 = B_185_29_0_idx_1 ; } else { _rtB ->
B_185_58_0 = _rtB -> B_185_55_0 ; } } isHit = ssIsSampleHit ( S , 2 , 0 ) ;
if ( isHit != 0 ) { rtb_B_185_2_0 [ 0 ] = _rtDW -> UnitDelay6_DSTATE [ 0 ] ;
rtb_B_185_2_0 [ 1 ] = _rtDW -> UnitDelay6_DSTATE [ 1 ] ; rtb_B_185_2_0 [ 2 ]
= _rtDW -> UnitDelay6_DSTATE [ 2 ] ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_60_0 [ 0 ] = rtb_B_185_2_0 [ 0 ] ; _rtB ->
B_185_60_0 [ 1 ] = rtb_B_185_2_0 [ 1 ] ; _rtB -> B_185_60_0 [ 2 ] =
rtb_B_185_2_0 [ 2 ] ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { _rtB -> B_185_63_0 [ isHit ]
= _rtP -> P_154 * ( _rtP -> P_153 [ isHit + 6 ] * ( _rtP -> P_152 * _rtB ->
B_185_60_0 [ 2 ] ) + ( _rtP -> P_153 [ isHit + 3 ] * ( _rtP -> P_152 * _rtB
-> B_185_60_0 [ 1 ] ) + _rtP -> P_152 * _rtB -> B_185_60_0 [ 0 ] * _rtP ->
P_153 [ isHit ] ) ) ; } } normal_operation_no_faults_Subsystem1_h4 ( S , _rtB
-> B_185_66_0 , & _rtB -> B_185_63_0 [ 0 ] , _rtB -> B_185_12_0 , & _rtB ->
Subsystem1_h , & _rtDW -> Subsystem1_h ) ;
normal_operation_no_faults_Subsystempi2delay_m ( S , _rtB -> B_185_69_0 , &
_rtB -> B_185_63_0 [ 0 ] , _rtB -> B_185_12_0 , & _rtB -> Subsystempi2delay_m
, & _rtDW -> Subsystempi2delay_m ) ; if ( _rtB -> B_185_66_0 != 0 ) { _rtB ->
B_185_71_0 [ 0 ] = _rtB -> Subsystem1_h . B_35_0_0 ; _rtB -> B_185_71_0 [ 1 ]
= _rtB -> Subsystem1_h . B_35_1_0 ; } else { _rtB -> B_185_71_0 [ 0 ] = _rtB
-> Subsystempi2delay_m . B_34_0_0 ; _rtB -> B_185_71_0 [ 1 ] = _rtB ->
Subsystempi2delay_m . B_34_1_0 ; } _rtB -> B_185_72_0 = _rtB -> B_185_58_0 -
_rtB -> B_185_71_0 [ 0 ] ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit
!= 0 ) { rtb_B_185_30_0 = _rtP -> P_158 * _rtB -> B_185_72_0 ; rtb_B_185_40_0
= _rtDW -> DiscreteTimeIntegrator_DSTATE + rtb_B_185_30_0 ; if ( ( (
rtb_B_185_40_0 >= _rtB -> B_185_76_0 ) && ( _rtB -> B_185_72_0 >
normal_operation_no_faults_rtC ( S ) -> B_185_78_0 ) ) || ( ( rtb_B_185_40_0
<= _rtB -> B_185_81_0 ) && ( _rtB -> B_185_72_0 <
normal_operation_no_faults_rtC ( S ) -> B_185_83_0 ) ) ) { _rtB -> B_185_88_0
= _rtB -> B_185_74_0 ; } else { _rtB -> B_185_88_0 = _rtP -> P_164 * _rtB ->
B_185_72_0 ; } if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE != 0 ) {
_rtB -> B_185_89_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE ; } else { _rtB
-> B_185_89_0 = _rtP -> P_165 * _rtB -> B_185_88_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE ; } if ( _rtB -> B_185_89_0 >= _rtP -> P_167 )
{ _rtB -> B_185_89_0 = _rtP -> P_167 ; } else { if ( _rtB -> B_185_89_0 <=
_rtP -> P_168 ) { _rtB -> B_185_89_0 = _rtP -> P_168 ; } } B_185_29_0_idx_0 =
rtb_B_185_30_0 + _rtB -> B_185_89_0 ; if ( B_185_29_0_idx_0 > _rtP -> P_169 )
{ _rtB -> B_185_91_0 = _rtP -> P_169 ; } else if ( B_185_29_0_idx_0 < _rtP ->
P_170 ) { _rtB -> B_185_91_0 = _rtP -> P_170 ; } else { _rtB -> B_185_91_0 =
B_185_29_0_idx_0 ; } } rtb_B_185_40_0 = _rtP -> P_171 * _rtB -> B_185_71_0 [
0 ] ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
rtb_B_185_30_0 = ssGetTaskTime ( S , 1 ) ; _rtDW ->
GridVoltagestabilizerTURNONiqreferenceforrectifier_MODE = ( rtb_B_185_30_0 >=
_rtP -> P_172 ) ; if ( _rtDW ->
GridVoltagestabilizerTURNONiqreferenceforrectifier_MODE == 1 ) { _rtB ->
B_185_93_0 = _rtP -> P_174 ; } else { _rtB -> B_185_93_0 = _rtP -> P_173 ; }
} normal_operation_no_faults_ACVoltageController ( S , _rtB -> B_185_93_0 ,
_rtB -> B_185_94_0 , & _rtB -> ACVoltageController , & _rtDW ->
ACVoltageController , & _rtP -> ACVoltageController , & _rtX ->
ACVoltageController , & ( ( XDis_normal_operation_no_faults_T * )
ssGetContStateDisabled ( S ) ) -> ACVoltageController ) ; _rtB -> B_185_99_0
= _rtB -> B_185_28_0 [ 0 ] - ( ( _rtB -> B_185_91_0 + rtb_B_185_40_0 ) - (
_rtB -> B_185_71_0 [ 1 ] + _rtB -> ACVoltageController . B_25_18_0 ) * _rtP
-> P_176 ) ; _rtB -> B_185_102_0 = _rtB -> ACVoltageController . B_25_18_0 -
_rtB -> B_185_71_0 [ 1 ] ; _rtB -> B_185_104_0 = _rtX -> Integrator_CSTATE_k
; _rtB -> B_185_108_0 = _rtB -> B_185_28_0 [ 1 ] - ( ( ( _rtB -> B_185_71_0 [
0 ] + _rtB -> B_185_58_0 ) * _rtP -> P_177 + ( _rtP -> P_178 * _rtB ->
B_185_102_0 + _rtB -> B_185_104_0 ) ) + _rtP -> P_180 * _rtB -> B_185_71_0 [
1 ] ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Saturation_MODE [ 0 ]
= _rtB -> B_185_99_0 >= _rtP -> P_182 ? 1 : _rtB -> B_185_99_0 > _rtP ->
P_183 ? 0 : - 1 ; _rtDW -> Saturation_MODE [ 1 ] = _rtB -> B_185_108_0 >=
_rtP -> P_182 ? 1 : _rtB -> B_185_108_0 > _rtP -> P_183 ? 0 : - 1 ; _rtDW ->
Saturation_MODE [ 2 ] = _rtB -> B_185_109_0 >= _rtP -> P_182 ? 1 : _rtB ->
B_185_109_0 > _rtP -> P_183 ? 0 : - 1 ; } B_185_110_0 [ 0 ] = _rtDW ->
Saturation_MODE [ 0 ] == 1 ? _rtP -> P_182 : _rtDW -> Saturation_MODE [ 0 ]
== - 1 ? _rtP -> P_183 : _rtB -> B_185_99_0 ; B_185_110_0 [ 1 ] = _rtDW ->
Saturation_MODE [ 1 ] == 1 ? _rtP -> P_182 : _rtDW -> Saturation_MODE [ 1 ]
== - 1 ? _rtP -> P_183 : _rtB -> B_185_108_0 ; B_185_110_0 [ 2 ] = _rtDW ->
Saturation_MODE [ 2 ] == 1 ? _rtP -> P_182 : _rtDW -> Saturation_MODE [ 2 ]
== - 1 ? _rtP -> P_183 : _rtB -> B_185_109_0 ;
normal_operation_no_faults_Subsystem1_h ( S , _rtB -> B_185_113_0 , &
B_185_110_0 [ 0 ] , _rtB -> B_185_12_0 , & _rtB -> Subsystem1_i3 , & _rtDW ->
Subsystem1_i3 ) ; normal_operation_no_faults_Subsystempi2delay_e ( S , _rtB
-> B_185_116_0 , & B_185_110_0 [ 0 ] , _rtB -> B_185_12_0 , & _rtB ->
Subsystempi2delay_pq , & _rtDW -> Subsystempi2delay_pq ) ; if ( _rtB ->
B_185_113_0 != 0 ) { B_185_29_0_idx_0 = _rtB -> Subsystem1_i3 . B_6_0_0 ;
B_185_29_0_idx_1 = _rtB -> Subsystem1_i3 . B_6_1_0 ; } else {
B_185_29_0_idx_0 = _rtB -> Subsystempi2delay_pq . B_5_0_0 ; B_185_29_0_idx_1
= _rtB -> Subsystempi2delay_pq . B_5_1_0 ; }
normal_operation_no_faults_ZeroSequenceController ( S , _rtB -> B_185_7_0 [ 2
] , _rtB -> B_185_63_0 [ 2 ] , 0.0 , & _rtB -> ZeroSequenceController2 , &
_rtDW -> ZeroSequenceController2 , & _rtP -> ZeroSequenceController2 , & _rtX
-> ZeroSequenceController2 ) ; _rtB -> B_185_122_0 [ 0 ] = B_185_29_0_idx_0 *
_rtB -> B_185_121_0 ; _rtB -> B_185_122_0 [ 1 ] = B_185_29_0_idx_1 * _rtB ->
B_185_121_0 ; _rtB -> B_185_122_0 [ 2 ] = ( B_185_110_0 [ 2 ] + _rtB ->
ZeroSequenceController2 . B_19_36_0 ) * _rtB -> B_185_121_0 ; for ( i = 0 ; i
< 3 ; i ++ ) { B_185_29_0_idx_0 = _rtP -> P_188 [ i + 6 ] * _rtB ->
B_185_122_0 [ 2 ] + ( _rtP -> P_188 [ i + 3 ] * _rtB -> B_185_122_0 [ 1 ] +
_rtP -> P_188 [ i ] * _rtB -> B_185_122_0 [ 0 ] ) ; rtb_B_185_124_0 [ i ] =
_rtP -> P_189 * B_185_29_0_idx_0 ; rtb_B_185_2_0 [ i ] = B_185_29_0_idx_0 ; }
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_30_0 =
ssGetTaskTime ( S , 1 ) ; _rtDW -> EnableCirculatingCurrentSuppresion_MODE =
( rtb_B_185_30_0 >= _rtP -> P_190 ) ; if ( _rtDW ->
EnableCirculatingCurrentSuppresion_MODE == 1 ) { _rtB -> B_185_125_0 = _rtP
-> P_192 ; } else { _rtB -> B_185_125_0 = _rtP -> P_191 ; } }
normal_operation_no_faults_Circulatingcurrentsuppression ( S , _rtB ->
B_185_125_0 , & _rtB -> Circulatingcurrentsuppression , & _rtDW ->
Circulatingcurrentsuppression , & _rtP -> Circulatingcurrentsuppression , &
_rtX -> Circulatingcurrentsuppression , & ( (
XDis_normal_operation_no_faults_T * ) ssGetContStateDisabled ( S ) ) ->
Circulatingcurrentsuppression ) ; _rtB -> B_185_127_0 [ 0 ] = rtb_B_185_124_0
[ 0 ] - _rtB -> Circulatingcurrentsuppression . B_30_57_0 [ 0 ] ; _rtB ->
B_185_127_0 [ 3 ] = rtb_B_185_2_0 [ 0 ] - _rtB ->
Circulatingcurrentsuppression . B_30_57_0 [ 0 ] ; _rtB -> B_185_127_0 [ 1 ] =
rtb_B_185_124_0 [ 1 ] - _rtB -> Circulatingcurrentsuppression . B_30_57_0 [ 1
] ; _rtB -> B_185_127_0 [ 4 ] = rtb_B_185_2_0 [ 1 ] - _rtB ->
Circulatingcurrentsuppression . B_30_57_0 [ 1 ] ; _rtB -> B_185_127_0 [ 2 ] =
rtb_B_185_124_0 [ 2 ] - _rtB -> Circulatingcurrentsuppression . B_30_57_0 [ 2
] ; _rtB -> B_185_127_0 [ 5 ] = rtb_B_185_2_0 [ 2 ] - _rtB ->
Circulatingcurrentsuppression . B_30_57_0 [ 2 ] ; isHit = ssIsSampleHit ( S ,
1 , 0 ) ; if ( isHit != 0 ) { isHit = ssIsSampleHit ( S , 2 , 0 ) ; if (
isHit != 0 ) { for ( i = 0 ; i < 6 ; i ++ ) { _rtB -> B_185_128_0 [ i ] =
_rtB -> B_185_127_0 [ i ] ; } } } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if (
isHit != 0 ) { rtb_B_185_30_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i <
50 ; i ++ ) { B_51_0_0 [ i ] = rtb_B_185_30_0 + _rtB -> B_185_130_0 [ i ] ;
B_51_0_0 [ i ] = muDoubleScalarRem ( B_51_0_0 [ i ] , _rtB -> B_185_131_0 ) ;
B_51_2_0 [ i ] = B_51_0_0 [ i ] * _rtB -> B_185_132_0 ; } { int_T i1 ; const
real_T * u0 = & B_51_2_0 [ 0 ] ; real_T * y0 = & B_51_0_0 [ 0 ] ; for ( i1 =
0 ; i1 < 50 ; i1 ++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T (
& ( y0 [ i1 ] ) , _rtP -> P_62 , u0 [ i1 ] , _rtP -> P_61 , 2U ) ; } } for (
i = 0 ; i < 50 ; i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_128_0 [ 0 ] >=
B_51_0_0 [ i ] ) ; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ;
tmpForInput [ 1 ] = ( int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = (
int8_T ) rtb_B_51_4_0 [ 2 ] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3
] ; tmpForInput [ 4 ] = ( int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = (
int8_T ) rtb_B_51_4_0 [ 5 ] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6
] ; tmpForInput [ 7 ] = ( int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = (
int8_T ) rtb_B_51_4_0 [ 8 ] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9
] ; tmpForInput [ 10 ] = ( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ]
= ( int8_T ) rtb_B_51_4_0 [ 11 ] ; tmpForInput [ 12 ] = ( int8_T )
rtb_B_51_4_0 [ 12 ] ; tmpForInput [ 13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ;
tmpForInput [ 14 ] = ( int8_T ) rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = (
int8_T ) rtb_B_51_4_0 [ 15 ] ; tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [
16 ] ; tmpForInput [ 17 ] = ( int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18
] = ( int8_T ) rtb_B_51_4_0 [ 18 ] ; tmpForInput [ 19 ] = ( int8_T )
rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20 ] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ;
tmpForInput [ 21 ] = ( int8_T ) rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = (
int8_T ) rtb_B_51_4_0 [ 22 ] ; tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [
23 ] ; tmpForInput [ 24 ] = ( int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25
] = ( int8_T ) rtb_B_51_4_0 [ 25 ] ; tmpForInput [ 26 ] = ( int8_T )
rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27 ] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ;
tmpForInput [ 28 ] = ( int8_T ) rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = (
int8_T ) rtb_B_51_4_0 [ 29 ] ; tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [
30 ] ; tmpForInput [ 31 ] = ( int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32
] = ( int8_T ) rtb_B_51_4_0 [ 32 ] ; tmpForInput [ 33 ] = ( int8_T )
rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34 ] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ;
tmpForInput [ 35 ] = ( int8_T ) rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = (
int8_T ) rtb_B_51_4_0 [ 36 ] ; tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [
37 ] ; tmpForInput [ 38 ] = ( int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39
] = ( int8_T ) rtb_B_51_4_0 [ 39 ] ; tmpForInput [ 40 ] = ( int8_T )
rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41 ] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ;
tmpForInput [ 42 ] = ( int8_T ) rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = (
int8_T ) rtb_B_51_4_0 [ 43 ] ; tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [
44 ] ; tmpForInput [ 45 ] = ( int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46
] = ( int8_T ) rtb_B_51_4_0 [ 46 ] ; tmpForInput [ 47 ] = ( int8_T )
rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48 ] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ;
tmpForInput [ 49 ] = ( int8_T ) rtb_B_51_4_0 [ 49 ] ; rtb_B_51_7_0 = - 0.0 ;
for ( isHit = 0 ; isHit < 50 ; isHit ++ ) { rtb_B_51_7_0 += ( real_T )
tmpForInput [ isHit ] ; } B_185_135_0_idx_0 = rtb_B_51_7_0 ; _rtB ->
B_185_137_0 = _rtDW -> UnitDelay4_DSTATE ; rtb_B_51_7_0 = _rtP -> P_200 *
_rtB -> B_185_137_0 - _rtDW -> UnitDelay3_DSTATE ; B_185_135_0_idx_1 =
B_185_135_0_idx_0 ; if ( B_185_135_0_idx_0 > _rtP -> P_197 ) {
B_185_135_0_idx_1 = _rtP -> P_197 ; } else { if ( B_185_135_0_idx_0 < _rtP ->
P_198 ) { B_185_135_0_idx_1 = _rtP -> P_198 ; } } _rtB -> B_185_141_0 [ 0 ] =
B_185_135_0_idx_1 * rtb_B_51_7_0 ; B_185_135_0_idx_0 = B_185_135_0_idx_1 ;
B_185_135_0_idx_1 = _rtB -> B_185_133_0 ; if ( _rtB -> B_185_133_0 > _rtP ->
P_197 ) { B_185_135_0_idx_1 = _rtP -> P_197 ; } else { if ( _rtB ->
B_185_133_0 < _rtP -> P_198 ) { B_185_135_0_idx_1 = _rtP -> P_198 ; } } _rtB
-> B_185_141_0 [ 1 ] = B_185_135_0_idx_1 * rtb_B_51_7_0 ; rtb_B_51_7_0 =
ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) { B_47_0_0 [ i ] =
rtb_B_51_7_0 + _rtB -> B_185_143_0 [ i ] ; B_47_0_0 [ i ] = muDoubleScalarRem
( B_47_0_0 [ i ] , _rtB -> B_185_144_0 ) ; B_47_2_0 [ i ] = B_47_0_0 [ i ] *
_rtB -> B_185_145_0 ; } { int_T i1 ; const real_T * u0 = & B_47_2_0 [ 0 ] ;
real_T * y0 = & B_47_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1 ++ ) {
normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] ) , _rtP
-> P_54 , u0 [ i1 ] , _rtP -> P_53 , 2U ) ; } } for ( i = 0 ; i < 50 ; i ++ )
{ rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_128_0 [ 4 ] >= B_47_0_0 [ i ] ) ; }
tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_47_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_47_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_51_7_0 = rtb_B_47_7_0 ; _rtB -> B_185_150_0 = _rtDW ->
UnitDelay4_DSTATE_i ; rtb_B_47_7_0 = _rtP -> P_209 * _rtB -> B_185_150_0 -
_rtDW -> UnitDelay3_DSTATE_f ; B_185_148_0_idx_1 = rtb_B_51_7_0 ; if (
rtb_B_51_7_0 > _rtP -> P_206 ) { B_185_148_0_idx_1 = _rtP -> P_206 ; } else {
if ( rtb_B_51_7_0 < _rtP -> P_207 ) { B_185_148_0_idx_1 = _rtP -> P_207 ; } }
_rtB -> B_185_154_0 [ 0 ] = B_185_148_0_idx_1 * rtb_B_47_7_0 ; rtb_B_51_7_0 =
B_185_148_0_idx_1 ; B_185_148_0_idx_1 = _rtB -> B_185_146_0 ; if ( _rtB ->
B_185_146_0 > _rtP -> P_206 ) { B_185_148_0_idx_1 = _rtP -> P_206 ; } else {
if ( _rtB -> B_185_146_0 < _rtP -> P_207 ) { B_185_148_0_idx_1 = _rtP ->
P_207 ; } } _rtB -> B_185_154_0 [ 1 ] = B_185_148_0_idx_1 * rtb_B_47_7_0 ;
rtb_B_47_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_50_0_0 [ i ] = rtb_B_47_7_0 + _rtB -> B_185_156_0 [ i ] ; B_50_0_0 [ i ] =
muDoubleScalarRem ( B_50_0_0 [ i ] , _rtB -> B_185_157_0 ) ; B_50_2_0 [ i ] =
B_50_0_0 [ i ] * _rtB -> B_185_158_0 ; } { int_T i1 ; const real_T * u0 = &
B_50_2_0 [ 0 ] ; real_T * y0 = & B_50_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_60 , u0 [ i1 ] , _rtP -> P_59 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_128_0 [ 1 ] >= B_50_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_50_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_50_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_47_7_0 = rtb_B_50_7_0 ; _rtB -> B_185_163_0 = _rtDW ->
UnitDelay4_DSTATE_e ; rtb_B_50_7_0 = _rtP -> P_218 * _rtB -> B_185_163_0 -
_rtDW -> UnitDelay3_DSTATE_a ; B_185_161_0_idx_1 = rtb_B_47_7_0 ; if (
rtb_B_47_7_0 > _rtP -> P_215 ) { B_185_161_0_idx_1 = _rtP -> P_215 ; } else {
if ( rtb_B_47_7_0 < _rtP -> P_216 ) { B_185_161_0_idx_1 = _rtP -> P_216 ; } }
_rtB -> B_185_167_0 [ 0 ] = B_185_161_0_idx_1 * rtb_B_50_7_0 ; rtb_B_47_7_0 =
B_185_161_0_idx_1 ; B_185_161_0_idx_1 = _rtB -> B_185_159_0 ; if ( _rtB ->
B_185_159_0 > _rtP -> P_215 ) { B_185_161_0_idx_1 = _rtP -> P_215 ; } else {
if ( _rtB -> B_185_159_0 < _rtP -> P_216 ) { B_185_161_0_idx_1 = _rtP ->
P_216 ; } } _rtB -> B_185_167_0 [ 1 ] = B_185_161_0_idx_1 * rtb_B_50_7_0 ;
rtb_B_50_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_48_0_0 [ i ] = rtb_B_50_7_0 + _rtB -> B_185_169_0 [ i ] ; B_48_0_0 [ i ] =
muDoubleScalarRem ( B_48_0_0 [ i ] , _rtB -> B_185_170_0 ) ; B_48_2_0 [ i ] =
B_48_0_0 [ i ] * _rtB -> B_185_171_0 ; } { int_T i1 ; const real_T * u0 = &
B_48_2_0 [ 0 ] ; real_T * y0 = & B_48_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_56 , u0 [ i1 ] , _rtP -> P_55 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_128_0 [ 3 ] >= B_48_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_48_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_48_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_50_7_0 = rtb_B_48_7_0 ; _rtB -> B_185_176_0 = _rtDW ->
UnitDelay4_DSTATE_p ; rtb_B_48_7_0 = _rtP -> P_227 * _rtB -> B_185_176_0 -
_rtDW -> UnitDelay3_DSTATE_d ; B_185_174_0_idx_1 = rtb_B_50_7_0 ; if (
rtb_B_50_7_0 > _rtP -> P_224 ) { B_185_174_0_idx_1 = _rtP -> P_224 ; } else {
if ( rtb_B_50_7_0 < _rtP -> P_225 ) { B_185_174_0_idx_1 = _rtP -> P_225 ; } }
_rtB -> B_185_180_0 [ 0 ] = B_185_174_0_idx_1 * rtb_B_48_7_0 ; rtb_B_50_7_0 =
B_185_174_0_idx_1 ; B_185_174_0_idx_1 = _rtB -> B_185_172_0 ; if ( _rtB ->
B_185_172_0 > _rtP -> P_224 ) { B_185_174_0_idx_1 = _rtP -> P_224 ; } else {
if ( _rtB -> B_185_172_0 < _rtP -> P_225 ) { B_185_174_0_idx_1 = _rtP ->
P_225 ; } } _rtB -> B_185_180_0 [ 1 ] = B_185_174_0_idx_1 * rtb_B_48_7_0 ;
rtb_B_48_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_49_0_0 [ i ] = rtb_B_48_7_0 + _rtB -> B_185_182_0 [ i ] ; B_49_0_0 [ i ] =
muDoubleScalarRem ( B_49_0_0 [ i ] , _rtB -> B_185_183_0 ) ; B_49_2_0 [ i ] =
B_49_0_0 [ i ] * _rtB -> B_185_184_0 ; } { int_T i1 ; const real_T * u0 = &
B_49_2_0 [ 0 ] ; real_T * y0 = & B_49_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_58 , u0 [ i1 ] , _rtP -> P_57 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_128_0 [ 2 ] >= B_49_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_49_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_49_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_48_7_0 = rtb_B_49_7_0 ; _rtB -> B_185_189_0 = _rtDW ->
UnitDelay4_DSTATE_m ; rtb_B_49_7_0 = _rtP -> P_236 * _rtB -> B_185_189_0 -
_rtDW -> UnitDelay3_DSTATE_p ; B_185_187_0_idx_1 = rtb_B_48_7_0 ; if (
rtb_B_48_7_0 > _rtP -> P_233 ) { B_185_187_0_idx_1 = _rtP -> P_233 ; } else {
if ( rtb_B_48_7_0 < _rtP -> P_234 ) { B_185_187_0_idx_1 = _rtP -> P_234 ; } }
_rtB -> B_185_193_0 [ 0 ] = B_185_187_0_idx_1 * rtb_B_49_7_0 ; rtb_B_48_7_0 =
B_185_187_0_idx_1 ; B_185_187_0_idx_1 = _rtB -> B_185_185_0 ; if ( _rtB ->
B_185_185_0 > _rtP -> P_233 ) { B_185_187_0_idx_1 = _rtP -> P_233 ; } else {
if ( _rtB -> B_185_185_0 < _rtP -> P_234 ) { B_185_187_0_idx_1 = _rtP ->
P_234 ; } } _rtB -> B_185_193_0 [ 1 ] = B_185_187_0_idx_1 * rtb_B_49_7_0 ;
rtb_B_49_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_46_0_0 [ i ] = rtb_B_49_7_0 + _rtB -> B_185_195_0 [ i ] ; B_46_0_0 [ i ] =
muDoubleScalarRem ( B_46_0_0 [ i ] , _rtB -> B_185_196_0 ) ; B_46_2_0 [ i ] =
B_46_0_0 [ i ] * _rtB -> B_185_197_0 ; } { int_T i1 ; const real_T * u0 = &
B_46_2_0 [ 0 ] ; real_T * y0 = & B_46_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_52 , u0 [ i1 ] , _rtP -> P_51 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_128_0 [ 5 ] >= B_46_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; B_185_29_0_idx_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50
; isHit ++ ) { B_185_29_0_idx_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_49_7_0 = B_185_29_0_idx_0 ; _rtB -> B_185_202_0 = _rtDW ->
UnitDelay4_DSTATE_c ; B_185_29_0_idx_0 = _rtP -> P_245 * _rtB -> B_185_202_0
- _rtDW -> UnitDelay3_DSTATE_de ; B_185_200_0_idx_1 = rtb_B_49_7_0 ; if (
rtb_B_49_7_0 > _rtP -> P_242 ) { B_185_200_0_idx_1 = _rtP -> P_242 ; } else {
if ( rtb_B_49_7_0 < _rtP -> P_243 ) { B_185_200_0_idx_1 = _rtP -> P_243 ; } }
_rtB -> B_185_206_0 [ 0 ] = B_185_200_0_idx_1 * B_185_29_0_idx_0 ;
rtb_B_49_7_0 = B_185_200_0_idx_1 ; B_185_200_0_idx_1 = _rtB -> B_185_198_0 ;
if ( _rtB -> B_185_198_0 > _rtP -> P_242 ) { B_185_200_0_idx_1 = _rtP ->
P_242 ; } else { if ( _rtB -> B_185_198_0 < _rtP -> P_243 ) {
B_185_200_0_idx_1 = _rtP -> P_243 ; } } _rtB -> B_185_206_0 [ 1 ] =
B_185_200_0_idx_1 * B_185_29_0_idx_0 ; rtb_B_185_2_0 [ 0 ] = _rtDW ->
UnitDelay5_DSTATE_m [ 0 ] ; rtb_B_185_2_0 [ 1 ] = _rtDW ->
UnitDelay5_DSTATE_m [ 1 ] ; rtb_B_185_2_0 [ 2 ] = _rtDW ->
UnitDelay5_DSTATE_m [ 2 ] ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit
!= 0 ) { _rtB -> B_185_209_0 [ 0 ] = rtb_B_185_2_0 [ 0 ] ; _rtB ->
B_185_209_0 [ 1 ] = rtb_B_185_2_0 [ 1 ] ; _rtB -> B_185_209_0 [ 2 ] =
rtb_B_185_2_0 [ 2 ] ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { _rtB -> B_185_213_0 [ isHit
] = _rtP -> P_252 * ( _rtP -> P_249 * _rtB -> B_185_209_0 [ 2 ] * _rtP ->
P_250 * _rtP -> P_251 [ isHit + 6 ] + ( _rtP -> P_249 * _rtB -> B_185_209_0 [
1 ] * _rtP -> P_250 * _rtP -> P_251 [ isHit + 3 ] + _rtP -> P_249 * _rtB ->
B_185_209_0 [ 0 ] * _rtP -> P_250 * _rtP -> P_251 [ isHit ] ) ) ; } } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { isHit = ssIsSampleHit ( S ,
3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_215_0 = _rtDW ->
UnitDelay1_DSTATE_b ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { normal_operation_no_faults_Subsystem1 ( S , _rtB -> B_185_218_0 , &
_rtB -> B_185_213_0 [ 0 ] , _rtB -> B_185_215_0 , & _rtB -> Subsystem1_i , &
_rtDW -> Subsystem1_i ) ; normal_operation_no_faults_Subsystempi2delay ( S ,
_rtB -> B_185_221_0 , & _rtB -> B_185_213_0 [ 0 ] , _rtB -> B_185_215_0 , &
_rtB -> Subsystempi2delay_b , & _rtDW -> Subsystempi2delay_b ) ; if ( _rtB ->
B_185_218_0 != 0 ) { B_185_223_0_idx_0 = _rtB -> Subsystem1_i . B_1_0_0 ;
B_185_223_0_idx_1 = _rtB -> Subsystem1_i . B_1_1_0 ; } else {
B_185_223_0_idx_0 = _rtB -> Subsystempi2delay_b . B_0_0_0 ; B_185_223_0_idx_1
= _rtB -> Subsystempi2delay_b . B_0_1_0 ; } B_185_226_0_idx_0 = _rtDW ->
Delay_x1_DSTATE_i [ 0 ] ; B_185_228_0_idx_0 = _rtDW -> Delay_x2_DSTATE_f [ 0
] ; B_185_226_0_idx_1 = _rtDW -> Delay_x1_DSTATE_i [ 1 ] ; B_185_228_0_idx_1
= _rtDW -> Delay_x2_DSTATE_f [ 1 ] ; B_185_226_0_idx_2 = _rtDW ->
Delay_x1_DSTATE_i [ 2 ] ; B_185_228_0_idx_2 = _rtDW -> Delay_x2_DSTATE_f [ 2
] ; _rtB -> B_185_231_0 [ 0 ] = ( _rtP -> P_260 * _rtDW -> Delay_x1_DSTATE_i
[ 0 ] + _rtP -> P_262 * _rtDW -> Delay_x2_DSTATE_f [ 0 ] ) + _rtP -> P_258 *
B_185_223_0_idx_0 ; _rtB -> B_185_231_0 [ 1 ] = ( _rtP -> P_260 * _rtDW ->
Delay_x1_DSTATE_i [ 1 ] + _rtP -> P_262 * _rtDW -> Delay_x2_DSTATE_f [ 1 ] )
+ _rtP -> P_258 * B_185_223_0_idx_1 ; _rtB -> B_185_231_0 [ 2 ] = ( _rtP ->
P_260 * _rtDW -> Delay_x1_DSTATE_i [ 2 ] + _rtP -> P_262 * _rtDW ->
Delay_x2_DSTATE_f [ 2 ] ) + _rtP -> P_258 * _rtB -> B_185_224_0 ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_30_0 =
ssGetTaskTime ( S , 1 ) ; _rtDW ->
EnableACRMSregulatorTURNONidreferenceforinverter_MODE = ( rtb_B_185_30_0 >=
_rtP -> P_264 ) ; if ( _rtDW ->
EnableACRMSregulatorTURNONidreferenceforinverter_MODE == 1 ) {
B_185_29_0_idx_0 = _rtP -> P_266 ; } else { B_185_29_0_idx_0 = _rtP -> P_265
; } if ( B_185_29_0_idx_0 > _rtP -> P_267 ) { B_185_29_0_idx_0 = _rtB ->
B_185_232_0 ; } else { B_185_29_0_idx_0 = _rtB -> B_185_231_0 [ 0 ] ; } _rtB
-> B_185_235_0 = B_185_29_0_idx_0 - _rtB -> B_185_231_0 [ 0 ] ; } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_236_0 = _rtP ->
P_268 * _rtB -> B_185_235_0 ; B_185_29_0_idx_0 = _rtDW ->
DiscreteTimeIntegrator1_DSTATE_o + rtb_B_185_236_0 ; if ( ( (
B_185_29_0_idx_0 >= _rtB -> B_185_239_0 ) && ( _rtB -> B_185_235_0 >
normal_operation_no_faults_rtC ( S ) -> B_185_241_0 ) ) || ( (
B_185_29_0_idx_0 <= _rtB -> B_185_244_0 ) && ( _rtB -> B_185_235_0 <
normal_operation_no_faults_rtC ( S ) -> B_185_246_0 ) ) ) { _rtB ->
B_185_251_0 = _rtB -> B_185_237_0 ; } else { _rtB -> B_185_251_0 = _rtP ->
P_274 * _rtB -> B_185_235_0 ; } if ( _rtDW ->
DiscreteTimeIntegrator1_SYSTEM_ENABLE_f != 0 ) { _rtB -> B_185_252_0 = _rtDW
-> DiscreteTimeIntegrator1_DSTATE_o ; } else { _rtB -> B_185_252_0 = _rtP ->
P_275 * _rtB -> B_185_251_0 + _rtDW -> DiscreteTimeIntegrator1_DSTATE_o ; }
if ( _rtB -> B_185_252_0 >= _rtP -> P_277 ) { _rtB -> B_185_252_0 = _rtP ->
P_277 ; } else { if ( _rtB -> B_185_252_0 <= _rtP -> P_278 ) { _rtB ->
B_185_252_0 = _rtP -> P_278 ; } } rtb_B_185_236_0 += _rtB -> B_185_252_0 ; }
isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_2_0 [ 0 ]
= _rtDW -> UnitDelay6_DSTATE_l [ 0 ] ; rtb_B_185_2_0 [ 1 ] = _rtDW ->
UnitDelay6_DSTATE_l [ 1 ] ; rtb_B_185_2_0 [ 2 ] = _rtDW ->
UnitDelay6_DSTATE_l [ 2 ] ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit
!= 0 ) { _rtB -> B_185_255_0 [ 0 ] = rtb_B_185_2_0 [ 0 ] ; _rtB ->
B_185_255_0 [ 1 ] = rtb_B_185_2_0 [ 1 ] ; _rtB -> B_185_255_0 [ 2 ] =
rtb_B_185_2_0 [ 2 ] ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { B_185_258_0 [ isHit ] = _rtP
-> P_282 * ( _rtP -> P_281 [ isHit + 6 ] * ( _rtP -> P_280 * _rtB ->
B_185_255_0 [ 2 ] ) + ( _rtP -> P_281 [ isHit + 3 ] * ( _rtP -> P_280 * _rtB
-> B_185_255_0 [ 1 ] ) + _rtP -> P_280 * _rtB -> B_185_255_0 [ 0 ] * _rtP ->
P_281 [ isHit ] ) ) ; } normal_operation_no_faults_Subsystem1 ( S , _rtB ->
B_185_261_0 , & B_185_258_0 [ 0 ] , _rtB -> B_185_215_0 , & _rtB ->
Subsystem1_d , & _rtDW -> Subsystem1_d ) ;
normal_operation_no_faults_Subsystempi2delay ( S , _rtB -> B_185_264_0 , &
B_185_258_0 [ 0 ] , _rtB -> B_185_215_0 , & _rtB -> Subsystempi2delay_p , &
_rtDW -> Subsystempi2delay_p ) ; if ( _rtB -> B_185_261_0 != 0 ) { _rtB ->
B_185_266_0 [ 0 ] = _rtB -> Subsystem1_d . B_1_0_0 ; _rtB -> B_185_266_0 [ 1
] = _rtB -> Subsystem1_d . B_1_1_0 ; } else { _rtB -> B_185_266_0 [ 0 ] =
_rtB -> Subsystempi2delay_p . B_0_0_0 ; _rtB -> B_185_266_0 [ 1 ] = _rtB ->
Subsystempi2delay_p . B_0_1_0 ; } _rtB -> B_185_267_0 = rtb_B_185_236_0 -
_rtB -> B_185_266_0 [ 0 ] ; _rtB -> B_185_268_0 = _rtP -> P_286 * _rtB ->
B_185_267_0 ; } _rtB -> B_185_269_0 = _rtX -> Integrator_CSTATE_j ; _rtB ->
B_185_270_0 = _rtB -> B_185_268_0 + _rtB -> B_185_269_0 ; isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_271_0 = _rtP
-> P_288 * _rtB -> B_185_266_0 [ 0 ] ; } isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( isHit != 0 ) { rtb_B_185_30_0 = ssGetTaskTime ( S , 1 ) ; _rtDW ->
EnableReactivepowersetpointregulationiqreferenceforinverter_MODE = (
rtb_B_185_30_0 >= _rtP -> P_289 ) ; if ( _rtDW ->
EnableReactivepowersetpointregulationiqreferenceforinverter_MODE == 1 ) {
_rtB -> B_185_272_0 = _rtP -> P_291 ; } else { _rtB -> B_185_272_0 = _rtP ->
P_290 ; } if ( ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtB -> B_185_272_0 >
0.0 ) { if ( ! _rtDW -> ReactivepowerSetpoint_MODE ) { if ( ssGetTaskTime ( S
, 1 ) != ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; } _rtDW -> ReactivepowerSetpoint_MODE = true ; } } else { if ( _rtDW ->
ReactivepowerSetpoint_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; _rtDW -> ReactivepowerSetpoint_MODE = false ; } } } } if ( _rtDW ->
ReactivepowerSetpoint_MODE ) { isHit = ssIsSampleHit ( S , 2 , 0 ) ; if (
isHit != 0 ) { isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtB
-> B_17_3_0 [ 0 ] = _rtDW -> UnitDelay6_DSTATE_hi [ 0 ] ; _rtB -> B_17_3_0 [
1 ] = _rtDW -> UnitDelay6_DSTATE_hi [ 1 ] ; } } isHit = ssIsSampleHit ( S , 1
, 0 ) ; if ( isHit != 0 ) { if ( _rtB -> B_185_272_0 > _rtP -> P_48 ) {
B_185_29_0_idx_0 = _rtB -> B_17_3_0 [ 1 ] ; } else { B_185_29_0_idx_0 = _rtB
-> B_17_0_0 ; } if ( _rtB -> B_185_272_0 > _rtP -> P_49 ) { B_185_29_0_idx_1
= _rtB -> B_185_231_0 [ 0 ] ; } else { B_185_29_0_idx_1 = _rtB -> B_17_1_0 ;
} _rtB -> B_17_7_0 = B_185_29_0_idx_0 / B_185_29_0_idx_1 * _rtP -> P_50 ; }
if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( _rtDW ->
ReactivepowerSetpoint_SubsysRanBC ) ; } } isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( isHit != 0 ) { _rtB -> B_185_275_0 = ( _rtB -> B_185_266_0 [ 1 ] +
_rtB -> B_17_7_0 ) * _rtP -> P_292 ; } _rtB -> B_185_277_0 = _rtB ->
B_185_231_0 [ 0 ] - ( ( _rtB -> B_185_270_0 + _rtB -> B_185_271_0 ) - _rtB ->
B_185_275_0 ) ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_185_279_0 = ( _rtB -> B_185_266_0 [ 0 ] + rtb_B_185_236_0 ) * _rtP
-> P_293 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
rtb_B_185_280_0 = _rtB -> B_17_7_0 - _rtB -> B_185_266_0 [ 1 ] ; _rtB ->
B_185_281_0 = _rtP -> P_294 * rtb_B_185_280_0 ; } _rtB -> B_185_282_0 = _rtX
-> Integrator_CSTATE_n ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { _rtB -> B_185_284_0 = _rtP -> P_296 * _rtB -> B_185_266_0 [ 1 ] ; }
_rtB -> B_185_286_0 = _rtB -> B_185_231_0 [ 1 ] - ( ( ( _rtB -> B_185_281_0 +
_rtB -> B_185_282_0 ) + _rtB -> B_185_279_0 ) + _rtB -> B_185_284_0 ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Saturation2_MODE [ 0 ] = _rtB ->
B_185_277_0 >= _rtP -> P_298 ? 1 : _rtB -> B_185_277_0 > _rtP -> P_299 ? 0 :
- 1 ; _rtDW -> Saturation2_MODE [ 1 ] = _rtB -> B_185_286_0 >= _rtP -> P_298
? 1 : _rtB -> B_185_286_0 > _rtP -> P_299 ? 0 : - 1 ; _rtDW ->
Saturation2_MODE [ 2 ] = _rtB -> B_185_287_0 >= _rtP -> P_298 ? 1 : _rtB ->
B_185_287_0 > _rtP -> P_299 ? 0 : - 1 ; } B_185_288_0 [ 0 ] = _rtDW ->
Saturation2_MODE [ 0 ] == 1 ? _rtP -> P_298 : _rtDW -> Saturation2_MODE [ 0 ]
== - 1 ? _rtP -> P_299 : _rtB -> B_185_277_0 ; B_185_288_0 [ 1 ] = _rtDW ->
Saturation2_MODE [ 1 ] == 1 ? _rtP -> P_298 : _rtDW -> Saturation2_MODE [ 1 ]
== - 1 ? _rtP -> P_299 : _rtB -> B_185_286_0 ; B_185_288_0 [ 2 ] = _rtDW ->
Saturation2_MODE [ 2 ] == 1 ? _rtP -> P_298 : _rtDW -> Saturation2_MODE [ 2 ]
== - 1 ? _rtP -> P_299 : _rtB -> B_185_287_0 ;
normal_operation_no_faults_Subsystem1_h ( S , _rtB -> B_185_291_0 , &
B_185_288_0 [ 0 ] , _rtB -> B_185_215_0 , & _rtB -> Subsystem1 , & _rtDW ->
Subsystem1 ) ; normal_operation_no_faults_Subsystempi2delay_e ( S , _rtB ->
B_185_294_0 , & B_185_288_0 [ 0 ] , _rtB -> B_185_215_0 , & _rtB ->
Subsystempi2delay , & _rtDW -> Subsystempi2delay ) ; if ( _rtB -> B_185_291_0
!= 0 ) { B_185_29_0_idx_0 = _rtB -> Subsystem1 . B_6_0_0 ; B_185_29_0_idx_1 =
_rtB -> Subsystem1 . B_6_1_0 ; } else { B_185_29_0_idx_0 = _rtB ->
Subsystempi2delay . B_5_0_0 ; B_185_29_0_idx_1 = _rtB -> Subsystempi2delay .
B_5_1_0 ; } normal_operation_no_faults_ZeroSequenceController ( S , _rtB ->
B_185_213_0 [ 2 ] , B_185_258_0 [ 2 ] , 0.0 , & _rtB ->
ZeroSequenceController , & _rtDW -> ZeroSequenceController , & _rtP ->
ZeroSequenceController , & _rtX -> ZeroSequenceController ) ; _rtB ->
B_185_300_0 [ 0 ] = B_185_29_0_idx_0 * _rtB -> B_185_299_0 ; _rtB ->
B_185_300_0 [ 1 ] = B_185_29_0_idx_1 * _rtB -> B_185_299_0 ; _rtB ->
B_185_300_0 [ 2 ] = ( B_185_288_0 [ 2 ] + _rtB -> ZeroSequenceController .
B_19_36_0 ) * _rtB -> B_185_299_0 ; for ( i = 0 ; i < 3 ; i ++ ) {
rtb_B_185_236_0 = _rtP -> P_304 [ i + 6 ] * _rtB -> B_185_300_0 [ 2 ] + (
_rtP -> P_304 [ i + 3 ] * _rtB -> B_185_300_0 [ 1 ] + _rtP -> P_304 [ i ] *
_rtB -> B_185_300_0 [ 0 ] ) ; rtb_B_185_2_0 [ i ] = _rtP -> P_305 *
rtb_B_185_236_0 ; rtb_B_185_124_0 [ i ] = rtb_B_185_236_0 ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && ( ssIsMajorTimeStep ( S
) != 0 ) ) { if ( _rtB -> B_185_125_0 > 0.0 ) { if ( ! _rtDW ->
Circulatingcurrentsuppression1_MODE ) { if ( ssGetTaskTime ( S , 1 ) !=
ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } (
void ) memset ( & ( ( ( XDis_normal_operation_no_faults_T * )
ssGetContStateDisabled ( S ) ) -> Integrator_CSTATE_b5 ) , 0 , 2 * sizeof (
boolean_T ) ) ; _rtDW -> Circulatingcurrentsuppression1_MODE = true ; } }
else { if ( _rtDW -> Circulatingcurrentsuppression1_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & ( ( (
XDis_normal_operation_no_faults_T * ) ssGetContStateDisabled ( S ) ) ->
Integrator_CSTATE_b5 ) , 1 , 2 * sizeof ( boolean_T ) ) ; if ( _rtDW ->
Subsystem1_go . Subsystem1_MODE ) {
normal_operation_no_faults_Subsystem1_Disable ( S , & _rtDW -> Subsystem1_go
) ; } if ( _rtDW -> Subsystempi2delay_g . Subsystempi2delay_MODE ) {
normal_operation_no_faults_Subsystempi2delay_Disable ( S , & _rtDW ->
Subsystempi2delay_g ) ; } _rtDW -> Circulatingcurrentsuppression1_MODE =
false ; } } } if ( _rtDW -> Circulatingcurrentsuppression1_MODE ) { isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { isHit = ssIsSampleHit ( S ,
3 , 0 ) ; for ( i = 0 ; i < 6 ; i ++ ) { if ( isHit != 0 ) { _rtB -> B_4_6_0
[ i ] = _rtP -> P_6 * _rtDW -> UnitDelay9_DSTATE [ i ] ; } } } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_4_13_0 [ 0 ] = (
_rtB -> B_4_6_0 [ 0 ] + _rtB -> B_4_6_0 [ 1 ] ) * _rtP -> P_7 ; _rtB ->
B_4_13_0 [ 1 ] = ( _rtB -> B_4_6_0 [ 2 ] + _rtB -> B_4_6_0 [ 3 ] ) * _rtP ->
P_8 ; _rtB -> B_4_13_0 [ 2 ] = ( _rtB -> B_4_6_0 [ 4 ] + _rtB -> B_4_6_0 [ 5
] ) * _rtP -> P_9 ; for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { B_4_15_0 [
isHit ] = _rtP -> P_11 * ( _rtP -> P_10 [ isHit + 6 ] * _rtB -> B_4_13_0 [ 2
] + ( _rtP -> P_10 [ isHit + 3 ] * _rtB -> B_4_13_0 [ 1 ] + _rtP -> P_10 [
isHit ] * _rtB -> B_4_13_0 [ 0 ] ) ) ; } } isHit = ssIsSampleHit ( S , 2 , 0
) ; if ( isHit != 0 ) { isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0
) { _rtB -> B_4_17_0 = _rtDW -> UnitDelay1_DSTATE_em ; } } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) {
normal_operation_no_faults_Subsystem1 ( S , _rtB -> B_4_20_0 , & B_4_15_0 [ 0
] , _rtB -> B_4_17_0 , & _rtB -> Subsystem1_pi , & _rtDW -> Subsystem1_pi ) ;
normal_operation_no_faults_Subsystempi2delay ( S , _rtB -> B_4_23_0 , &
B_4_15_0 [ 0 ] , _rtB -> B_4_17_0 , & _rtB -> Subsystempi2delay_bk , & _rtDW
-> Subsystempi2delay_bk ) ; if ( _rtB -> B_4_20_0 != 0 ) { B_4_25_0_idx_0 =
_rtB -> Subsystem1_pi . B_1_0_0 ; B_4_25_0_idx_1 = _rtB -> Subsystem1_pi .
B_1_1_0 ; } else { B_4_25_0_idx_0 = _rtB -> Subsystempi2delay_bk . B_0_0_0 ;
B_4_25_0_idx_1 = _rtB -> Subsystempi2delay_bk . B_0_1_0 ; } _rtB -> B_4_26_0
= _rtP -> P_16 * B_4_25_0_idx_0 ; _rtB -> B_4_27_0 = _rtP -> P_17 *
B_4_25_0_idx_1 ; B_4_25_0_idx_1 = _rtB -> B_4_0_0 - B_4_25_0_idx_1 ; _rtB ->
B_4_29_0 = _rtP -> P_18 * B_4_25_0_idx_1 ; } _rtB -> B_4_30_0 = _rtX ->
Integrator_CSTATE_b5 ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0
) { _rtB -> B_4_31_0 = _rtP -> P_20 * B_4_25_0_idx_1 ; } _rtB -> B_4_32_0 =
_rtB -> B_4_31_0 + _rtB -> B_4_30_0 ; isHit = ssIsSampleHit ( S , 3 , 0 ) ;
if ( isHit != 0 ) { rtb_B_4_33_0 = _rtB -> B_4_1_0 - B_4_25_0_idx_0 ; _rtB ->
B_4_34_0 = _rtP -> P_21 * rtb_B_4_33_0 ; } _rtB -> B_4_35_0 = _rtX ->
Integrator_CSTATE_d ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 )
{ _rtB -> B_4_36_0 = _rtP -> P_23 * rtb_B_4_33_0 ; } _rtB -> B_4_37_0 = _rtB
-> B_4_36_0 + _rtB -> B_4_35_0 ; _rtB -> B_4_43_0 = look1_binlxpw (
muDoubleScalarRem ( ssGetT ( S ) - _rtB -> B_4_40_0 , _rtB -> B_4_39_0 ) ,
_rtP -> P_26 , _rtP -> P_25 , 1U ) ; _rtB -> B_4_44_0 = _rtB -> B_4_37_0 +
_rtB -> B_4_27_0 ; _rtB -> B_4_45_0 = _rtB -> B_4_32_0 - _rtB -> B_4_26_0 ;
normal_operation_no_faults_Subsystem1_g ( S , _rtB -> B_4_48_0 , _rtB ->
B_4_44_0 , _rtB -> B_4_45_0 , _rtB -> B_4_17_0 , & _rtB -> Subsystem1_go , &
_rtDW -> Subsystem1_go ) ; normal_operation_no_faults_Subsystempi2delay_g ( S
, _rtB -> B_4_51_0 , _rtB -> B_4_44_0 , _rtB -> B_4_45_0 , _rtB -> B_4_17_0 ,
& _rtB -> Subsystempi2delay_g , & _rtDW -> Subsystempi2delay_g ) ; if ( _rtB
-> B_4_48_0 != 0 ) { _rtB -> B_4_53_0 [ 0 ] = _rtB -> Subsystem1_go . B_3_0_0
; _rtB -> B_4_53_0 [ 1 ] = _rtB -> Subsystem1_go . B_3_1_0 ; } else { _rtB ->
B_4_53_0 [ 0 ] = _rtB -> Subsystempi2delay_g . B_2_0_0 ; _rtB -> B_4_53_0 [ 1
] = _rtB -> Subsystempi2delay_g . B_2_1_0 ; } _rtB -> B_4_54_0 [ 0 ] = _rtB
-> B_4_53_0 [ 0 ] ; _rtB -> B_4_54_0 [ 1 ] = _rtB -> B_4_53_0 [ 1 ] ; _rtB ->
B_4_54_0 [ 2 ] = _rtB -> B_4_2_0 ; for ( isHit = 0 ; isHit < 3 ; isHit ++ ) {
_rtB -> B_4_55_0 [ isHit ] = 0.0 ; _rtB -> B_4_55_0 [ isHit ] += _rtP -> P_30
[ isHit ] * _rtB -> B_4_54_0 [ 0 ] ; _rtB -> B_4_55_0 [ isHit ] += _rtP ->
P_30 [ isHit + 3 ] * _rtB -> B_4_54_0 [ 1 ] ; _rtB -> B_4_55_0 [ isHit ] +=
_rtP -> P_30 [ isHit + 6 ] * _rtB -> B_4_54_0 [ 2 ] ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Saturation_MODE_j [ 0 ] = _rtB ->
B_4_55_0 [ 0 ] >= _rtP -> P_31 ? 1 : _rtB -> B_4_55_0 [ 0 ] > _rtP -> P_32 ?
0 : - 1 ; _rtDW -> Saturation_MODE_j [ 1 ] = _rtB -> B_4_55_0 [ 1 ] >= _rtP
-> P_31 ? 1 : _rtB -> B_4_55_0 [ 1 ] > _rtP -> P_32 ? 0 : - 1 ; _rtDW ->
Saturation_MODE_j [ 2 ] = _rtB -> B_4_55_0 [ 2 ] >= _rtP -> P_31 ? 1 : _rtB
-> B_4_55_0 [ 2 ] > _rtP -> P_32 ? 0 : - 1 ; } _rtB -> B_4_56_0 [ 0 ] = _rtDW
-> Saturation_MODE_j [ 0 ] == 1 ? _rtP -> P_31 : _rtDW -> Saturation_MODE_j [
0 ] == - 1 ? _rtP -> P_32 : _rtB -> B_4_55_0 [ 0 ] ; if ( _rtB -> B_185_125_0
> _rtP -> P_33 ) { _rtB -> B_4_57_0 [ 0 ] = _rtB -> B_4_56_0 [ 0 ] ; } else {
_rtB -> B_4_57_0 [ 0 ] = _rtB -> B_4_3_0 ; } _rtB -> B_4_56_0 [ 1 ] = _rtDW
-> Saturation_MODE_j [ 1 ] == 1 ? _rtP -> P_31 : _rtDW -> Saturation_MODE_j [
1 ] == - 1 ? _rtP -> P_32 : _rtB -> B_4_55_0 [ 1 ] ; if ( _rtB -> B_185_125_0
> _rtP -> P_33 ) { _rtB -> B_4_57_0 [ 1 ] = _rtB -> B_4_56_0 [ 1 ] ; } else {
_rtB -> B_4_57_0 [ 1 ] = _rtB -> B_4_3_0 ; } _rtB -> B_4_56_0 [ 2 ] = _rtDW
-> Saturation_MODE_j [ 2 ] == 1 ? _rtP -> P_31 : _rtDW -> Saturation_MODE_j [
2 ] == - 1 ? _rtP -> P_32 : _rtB -> B_4_55_0 [ 2 ] ; if ( _rtB -> B_185_125_0
> _rtP -> P_33 ) { _rtB -> B_4_57_0 [ 2 ] = _rtB -> B_4_56_0 [ 2 ] ; } else {
_rtB -> B_4_57_0 [ 2 ] = _rtB -> B_4_3_0 ; } isHit = ssIsSampleHit ( S , 3 ,
0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S , 4 , 58 ,
SS_CALL_MDL_OUTPUTS ) ; } if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC (
_rtDW -> Circulatingcurrentsuppression1_SubsysRanBC ) ; } } _rtB ->
B_185_304_0 [ 0 ] = rtb_B_185_124_0 [ 0 ] - _rtB -> B_4_57_0 [ 0 ] ; _rtB ->
B_185_304_0 [ 3 ] = rtb_B_185_2_0 [ 0 ] - _rtB -> B_4_57_0 [ 0 ] ; _rtB ->
B_185_304_0 [ 1 ] = rtb_B_185_124_0 [ 1 ] - _rtB -> B_4_57_0 [ 1 ] ; _rtB ->
B_185_304_0 [ 4 ] = rtb_B_185_2_0 [ 1 ] - _rtB -> B_4_57_0 [ 1 ] ; _rtB ->
B_185_304_0 [ 2 ] = rtb_B_185_124_0 [ 2 ] - _rtB -> B_4_57_0 [ 2 ] ; _rtB ->
B_185_304_0 [ 5 ] = rtb_B_185_2_0 [ 2 ] - _rtB -> B_4_57_0 [ 2 ] ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { isHit = ssIsSampleHit ( S ,
2 , 0 ) ; if ( isHit != 0 ) { for ( i = 0 ; i < 6 ; i ++ ) { _rtB ->
B_185_305_0 [ i ] = _rtB -> B_185_304_0 [ i ] ; } } } isHit = ssIsSampleHit (
S , 2 , 0 ) ; if ( isHit != 0 ) { B_4_25_0_idx_1 = ssGetTaskTime ( S , 2 ) ;
for ( i = 0 ; i < 50 ; i ++ ) { B_185_312_0 [ i ] = muDoubleScalarRem (
B_4_25_0_idx_1 + _rtB -> B_185_307_0 [ i ] , _rtB -> B_185_309_0 ) * _rtB ->
B_185_311_0 ; } { int_T i1 ; const real_T * u0 = & B_185_312_0 [ 0 ] ; real_T
* y0 = & _rtB -> B_185_313_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1 ++ ) {
normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] ) , _rtP
-> P_310 , u0 [ i1 ] , _rtP -> P_309 , 2U ) ; } } for ( i = 0 ; i < 50 ; i ++
) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_305_0 [ 0 ] >= _rtB -> B_185_313_0 [
i ] ) ; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1
] = ( int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T )
rtb_B_51_4_0 [ 2 ] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ;
tmpForInput [ 4 ] = ( int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = (
int8_T ) rtb_B_51_4_0 [ 5 ] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6
] ; tmpForInput [ 7 ] = ( int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = (
int8_T ) rtb_B_51_4_0 [ 8 ] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9
] ; tmpForInput [ 10 ] = ( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ]
= ( int8_T ) rtb_B_51_4_0 [ 11 ] ; tmpForInput [ 12 ] = ( int8_T )
rtb_B_51_4_0 [ 12 ] ; tmpForInput [ 13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ;
tmpForInput [ 14 ] = ( int8_T ) rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = (
int8_T ) rtb_B_51_4_0 [ 15 ] ; tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [
16 ] ; tmpForInput [ 17 ] = ( int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18
] = ( int8_T ) rtb_B_51_4_0 [ 18 ] ; tmpForInput [ 19 ] = ( int8_T )
rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20 ] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ;
tmpForInput [ 21 ] = ( int8_T ) rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = (
int8_T ) rtb_B_51_4_0 [ 22 ] ; tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [
23 ] ; tmpForInput [ 24 ] = ( int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25
] = ( int8_T ) rtb_B_51_4_0 [ 25 ] ; tmpForInput [ 26 ] = ( int8_T )
rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27 ] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ;
tmpForInput [ 28 ] = ( int8_T ) rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = (
int8_T ) rtb_B_51_4_0 [ 29 ] ; tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [
30 ] ; tmpForInput [ 31 ] = ( int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32
] = ( int8_T ) rtb_B_51_4_0 [ 32 ] ; tmpForInput [ 33 ] = ( int8_T )
rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34 ] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ;
tmpForInput [ 35 ] = ( int8_T ) rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = (
int8_T ) rtb_B_51_4_0 [ 36 ] ; tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [
37 ] ; tmpForInput [ 38 ] = ( int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39
] = ( int8_T ) rtb_B_51_4_0 [ 39 ] ; tmpForInput [ 40 ] = ( int8_T )
rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41 ] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ;
tmpForInput [ 42 ] = ( int8_T ) rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = (
int8_T ) rtb_B_51_4_0 [ 43 ] ; tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [
44 ] ; tmpForInput [ 45 ] = ( int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46
] = ( int8_T ) rtb_B_51_4_0 [ 46 ] ; tmpForInput [ 47 ] = ( int8_T )
rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48 ] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ;
tmpForInput [ 49 ] = ( int8_T ) rtb_B_51_4_0 [ 49 ] ; rtb_B_16_3_0 = - 0.0 ;
for ( isHit = 0 ; isHit < 50 ; isHit ++ ) { rtb_B_16_3_0 += ( real_T )
tmpForInput [ isHit ] ; } B_185_316_0_idx_0 = rtb_B_16_3_0 ; _rtB ->
B_185_318_0 = _rtDW -> UnitDelay4_DSTATE_o ; rtb_B_16_3_0 = _rtP -> P_315 *
_rtB -> B_185_318_0 - _rtDW -> UnitDelay3_DSTATE_m ; B_185_316_0_idx_1 =
B_185_316_0_idx_0 ; if ( B_185_316_0_idx_0 > _rtP -> P_312 ) {
B_185_316_0_idx_1 = _rtP -> P_312 ; } else { if ( B_185_316_0_idx_0 < _rtP ->
P_313 ) { B_185_316_0_idx_1 = _rtP -> P_313 ; } } _rtB -> B_185_322_0 [ 0 ] =
B_185_316_0_idx_1 * rtb_B_16_3_0 ; B_185_316_0_idx_0 = B_185_316_0_idx_1 ;
B_185_316_0_idx_1 = _rtB -> B_185_314_0 ; if ( _rtB -> B_185_314_0 > _rtP ->
P_312 ) { B_185_316_0_idx_1 = _rtP -> P_312 ; } else { if ( _rtB ->
B_185_314_0 < _rtP -> P_313 ) { B_185_316_0_idx_1 = _rtP -> P_313 ; } } _rtB
-> B_185_322_0 [ 1 ] = B_185_316_0_idx_1 * rtb_B_16_3_0 ; rtb_B_16_3_0 =
ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) { B_12_0_0 [ i ] =
rtb_B_16_3_0 + _rtB -> B_185_324_0 [ i ] ; B_12_0_0 [ i ] = muDoubleScalarRem
( B_12_0_0 [ i ] , _rtB -> B_185_325_0 ) ; B_12_2_0 [ i ] = B_12_0_0 [ i ] *
_rtB -> B_185_326_0 ; } { int_T i1 ; const real_T * u0 = & B_12_2_0 [ 0 ] ;
real_T * y0 = & B_12_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1 ++ ) {
normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] ) , _rtP
-> P_37 , u0 [ i1 ] , _rtP -> P_36 , 2U ) ; } } for ( i = 0 ; i < 50 ; i ++ )
{ rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_305_0 [ 4 ] >= B_12_0_0 [ i ] ) ; }
tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_12_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_12_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_16_3_0 = rtb_B_12_7_0 ; _rtB -> B_185_331_0 = _rtDW ->
UnitDelay4_DSTATE_f ; rtb_B_12_7_0 = _rtP -> P_324 * _rtB -> B_185_331_0 -
_rtDW -> UnitDelay3_DSTATE_ff ; B_185_329_0_idx_1 = rtb_B_16_3_0 ; if (
rtb_B_16_3_0 > _rtP -> P_321 ) { B_185_329_0_idx_1 = _rtP -> P_321 ; } else {
if ( rtb_B_16_3_0 < _rtP -> P_322 ) { B_185_329_0_idx_1 = _rtP -> P_322 ; } }
_rtB -> B_185_335_0 [ 0 ] = B_185_329_0_idx_1 * rtb_B_12_7_0 ; rtb_B_16_3_0 =
B_185_329_0_idx_1 ; B_185_329_0_idx_1 = _rtB -> B_185_327_0 ; if ( _rtB ->
B_185_327_0 > _rtP -> P_321 ) { B_185_329_0_idx_1 = _rtP -> P_321 ; } else {
if ( _rtB -> B_185_327_0 < _rtP -> P_322 ) { B_185_329_0_idx_1 = _rtP ->
P_322 ; } } _rtB -> B_185_335_0 [ 1 ] = B_185_329_0_idx_1 * rtb_B_12_7_0 ;
rtb_B_12_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_15_0_0 [ i ] = rtb_B_12_7_0 + _rtB -> B_185_337_0 [ i ] ; B_15_0_0 [ i ] =
muDoubleScalarRem ( B_15_0_0 [ i ] , _rtB -> B_185_338_0 ) ; B_15_2_0 [ i ] =
B_15_0_0 [ i ] * _rtB -> B_185_339_0 ; } { int_T i1 ; const real_T * u0 = &
B_15_2_0 [ 0 ] ; real_T * y0 = & B_15_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_43 , u0 [ i1 ] , _rtP -> P_42 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_305_0 [ 1 ] >= B_15_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_15_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_15_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_12_7_0 = rtb_B_15_7_0 ; _rtB -> B_185_344_0 = _rtDW ->
UnitDelay4_DSTATE_ig ; rtb_B_15_7_0 = _rtP -> P_333 * _rtB -> B_185_344_0 -
_rtDW -> UnitDelay3_DSTATE_o ; B_185_342_0_idx_1 = rtb_B_12_7_0 ; if (
rtb_B_12_7_0 > _rtP -> P_330 ) { B_185_342_0_idx_1 = _rtP -> P_330 ; } else {
if ( rtb_B_12_7_0 < _rtP -> P_331 ) { B_185_342_0_idx_1 = _rtP -> P_331 ; } }
_rtB -> B_185_348_0 [ 0 ] = B_185_342_0_idx_1 * rtb_B_15_7_0 ; rtb_B_12_7_0 =
B_185_342_0_idx_1 ; B_185_342_0_idx_1 = _rtB -> B_185_340_0 ; if ( _rtB ->
B_185_340_0 > _rtP -> P_330 ) { B_185_342_0_idx_1 = _rtP -> P_330 ; } else {
if ( _rtB -> B_185_340_0 < _rtP -> P_331 ) { B_185_342_0_idx_1 = _rtP ->
P_331 ; } } _rtB -> B_185_348_0 [ 1 ] = B_185_342_0_idx_1 * rtb_B_15_7_0 ;
rtb_B_15_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_13_0_0 [ i ] = rtb_B_15_7_0 + _rtB -> B_185_350_0 [ i ] ; B_13_0_0 [ i ] =
muDoubleScalarRem ( B_13_0_0 [ i ] , _rtB -> B_185_351_0 ) ; B_13_2_0 [ i ] =
B_13_0_0 [ i ] * _rtB -> B_185_352_0 ; } { int_T i1 ; const real_T * u0 = &
B_13_2_0 [ 0 ] ; real_T * y0 = & B_13_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_39 , u0 [ i1 ] , _rtP -> P_38 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_305_0 [ 3 ] >= B_13_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_13_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_13_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_15_7_0 = rtb_B_13_7_0 ; _rtB -> B_185_357_0 = _rtDW ->
UnitDelay4_DSTATE_ma ; rtb_B_13_7_0 = _rtP -> P_342 * _rtB -> B_185_357_0 -
_rtDW -> UnitDelay3_DSTATE_pb ; B_185_355_0_idx_1 = rtb_B_15_7_0 ; if (
rtb_B_15_7_0 > _rtP -> P_339 ) { B_185_355_0_idx_1 = _rtP -> P_339 ; } else {
if ( rtb_B_15_7_0 < _rtP -> P_340 ) { B_185_355_0_idx_1 = _rtP -> P_340 ; } }
_rtB -> B_185_361_0 [ 0 ] = B_185_355_0_idx_1 * rtb_B_13_7_0 ; rtb_B_15_7_0 =
B_185_355_0_idx_1 ; B_185_355_0_idx_1 = _rtB -> B_185_353_0 ; if ( _rtB ->
B_185_353_0 > _rtP -> P_339 ) { B_185_355_0_idx_1 = _rtP -> P_339 ; } else {
if ( _rtB -> B_185_353_0 < _rtP -> P_340 ) { B_185_355_0_idx_1 = _rtP ->
P_340 ; } } _rtB -> B_185_361_0 [ 1 ] = B_185_355_0_idx_1 * rtb_B_13_7_0 ;
rtb_B_13_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_14_0_0 [ i ] = rtb_B_13_7_0 + _rtB -> B_185_363_0 [ i ] ; B_14_0_0 [ i ] =
muDoubleScalarRem ( B_14_0_0 [ i ] , _rtB -> B_185_364_0 ) ; B_14_2_0 [ i ] =
B_14_0_0 [ i ] * _rtB -> B_185_365_0 ; } { int_T i1 ; const real_T * u0 = &
B_14_2_0 [ 0 ] ; real_T * y0 = & B_14_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_41 , u0 [ i1 ] , _rtP -> P_40 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_305_0 [ 2 ] >= B_14_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; B_4_25_0_idx_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { B_4_25_0_idx_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_13_7_0 = B_4_25_0_idx_0 ; _rtB -> B_185_370_0 = _rtDW ->
UnitDelay4_DSTATE_l ; B_4_25_0_idx_0 = _rtP -> P_351 * _rtB -> B_185_370_0 -
_rtDW -> UnitDelay3_DSTATE_d4 ; B_185_368_0_idx_1 = rtb_B_13_7_0 ; if (
rtb_B_13_7_0 > _rtP -> P_348 ) { B_185_368_0_idx_1 = _rtP -> P_348 ; } else {
if ( rtb_B_13_7_0 < _rtP -> P_349 ) { B_185_368_0_idx_1 = _rtP -> P_349 ; } }
_rtB -> B_185_374_0 [ 0 ] = B_185_368_0_idx_1 * B_4_25_0_idx_0 ; rtb_B_13_7_0
= B_185_368_0_idx_1 ; B_185_368_0_idx_1 = _rtB -> B_185_366_0 ; if ( _rtB ->
B_185_366_0 > _rtP -> P_348 ) { B_185_368_0_idx_1 = _rtP -> P_348 ; } else {
if ( _rtB -> B_185_366_0 < _rtP -> P_349 ) { B_185_368_0_idx_1 = _rtP ->
P_349 ; } } _rtB -> B_185_374_0 [ 1 ] = B_185_368_0_idx_1 * B_4_25_0_idx_0 ;
B_4_25_0_idx_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_11_0_0 [ i ] = B_4_25_0_idx_0 + _rtB -> B_185_376_0 [ i ] ; B_11_0_0 [ i ]
= muDoubleScalarRem ( B_11_0_0 [ i ] , _rtB -> B_185_377_0 ) ; B_11_2_0 [ i ]
= B_11_0_0 [ i ] * _rtB -> B_185_378_0 ; } { int_T i1 ; const real_T * u0 = &
B_11_2_0 [ 0 ] ; real_T * y0 = & B_11_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_35 , u0 [ i1 ] , _rtP -> P_34 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_305_0 [ 5 ] >= B_11_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_4_33_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_4_33_0 += ( real_T ) tmpForInput [ isHit ] ; }
B_185_381_0_idx_0 = rtb_B_4_33_0 ; _rtB -> B_185_383_0 = _rtDW ->
UnitDelay4_DSTATE_c5 ; rtb_B_4_33_0 = _rtP -> P_360 * _rtB -> B_185_383_0 -
_rtDW -> UnitDelay3_DSTATE_b ; B_185_381_0_idx_1 = B_185_381_0_idx_0 ; if (
B_185_381_0_idx_0 > _rtP -> P_357 ) { B_185_381_0_idx_1 = _rtP -> P_357 ; }
else { if ( B_185_381_0_idx_0 < _rtP -> P_358 ) { B_185_381_0_idx_1 = _rtP ->
P_358 ; } } _rtB -> B_185_387_0 [ 0 ] = B_185_381_0_idx_1 * rtb_B_4_33_0 ;
B_185_381_0_idx_0 = B_185_381_0_idx_1 ; B_185_381_0_idx_1 = _rtB ->
B_185_379_0 ; if ( _rtB -> B_185_379_0 > _rtP -> P_357 ) { B_185_381_0_idx_1
= _rtP -> P_357 ; } else { if ( _rtB -> B_185_379_0 < _rtP -> P_358 ) {
B_185_381_0_idx_1 = _rtP -> P_358 ; } } _rtB -> B_185_387_0 [ 1 ] =
B_185_381_0_idx_1 * rtb_B_4_33_0 ; rtb_B_185_2_0 [ 0 ] = _rtDW ->
UnitDelay5_DSTATE_h [ 0 ] ; rtb_B_185_2_0 [ 1 ] = _rtDW ->
UnitDelay5_DSTATE_h [ 1 ] ; rtb_B_185_2_0 [ 2 ] = _rtDW ->
UnitDelay5_DSTATE_h [ 2 ] ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit
!= 0 ) { _rtB -> B_185_390_0 [ 0 ] = rtb_B_185_2_0 [ 0 ] ; _rtB ->
B_185_390_0 [ 1 ] = rtb_B_185_2_0 [ 1 ] ; _rtB -> B_185_390_0 [ 2 ] =
rtb_B_185_2_0 [ 2 ] ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { _rtB -> B_185_394_0 [ isHit
] = _rtP -> P_367 * ( _rtP -> P_364 * _rtB -> B_185_390_0 [ 2 ] * _rtP ->
P_365 * _rtP -> P_366 [ isHit + 6 ] + ( _rtP -> P_364 * _rtB -> B_185_390_0 [
1 ] * _rtP -> P_365 * _rtP -> P_366 [ isHit + 3 ] + _rtP -> P_364 * _rtB ->
B_185_390_0 [ 0 ] * _rtP -> P_365 * _rtP -> P_366 [ isHit ] ) ) ; } } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( ssIsMajorTimeStep ( S
) != 0 ) { _rtDW -> RelationalOperator_Mode_g = ( _rtX -> Integrator_CSTATE_l
> _rtB -> B_185_395_0 ) ; } _rtB -> B_185_396_0 = _rtDW ->
RelationalOperator_Mode_g ; } _rtB -> B_185_397_0 = _rtX ->
Integrator_CSTATE_l - _rtB -> B_185_395_0 ; if ( ( _rtDW ->
Initial_FirstOutputTime_a == ( rtMinusInf ) ) || ( _rtDW ->
Initial_FirstOutputTime_a == ssGetTaskTime ( S , 0 ) ) ) { _rtDW ->
Initial_FirstOutputTime_a = ssGetTaskTime ( S , 0 ) ; _rtB -> B_185_398_0 =
_rtP -> P_369 ; } else { _rtB -> B_185_398_0 = _rtB -> B_185_397_0 ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { rtb_B_185_2288_0 = false ; stateChanged =
false ; didZcEventOccur = ( _rtB -> B_185_396_0 && ( _rtZCE ->
Integrator_Reset_ZCE_h != POS_ZCSIG ) ) ; _rtZCE -> Integrator_Reset_ZCE_h =
_rtB -> B_185_396_0 ; if ( didZcEventOccur || ( _rtDW -> Integrator_IWORK_o
!= 0 ) ) { rtb_B_185_2288_0 = true ; _rtX -> Integrator_CSTATE_l = _rtB ->
B_185_398_0 ; stateChanged = true ; } if ( rtb_B_185_2288_0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( stateChanged ) {
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } } if ( _rtX ->
Integrator_CSTATE_l >= _rtP -> P_370 ) { _rtX -> Integrator_CSTATE_l = _rtP
-> P_370 ; } else { if ( _rtX -> Integrator_CSTATE_l <= _rtP -> P_371 ) {
_rtX -> Integrator_CSTATE_l = _rtP -> P_371 ; } } } _rtB -> B_185_399_0 =
_rtX -> Integrator_CSTATE_l ; normal_operation_no_faults_Subsystem1_h4 ( S ,
_rtB -> B_185_402_0 , & _rtB -> B_185_394_0 [ 0 ] , _rtB -> B_185_399_0 , &
_rtB -> Subsystem1_j , & _rtDW -> Subsystem1_j ) ;
normal_operation_no_faults_Subsystempi2delay_m ( S , _rtB -> B_185_405_0 , &
_rtB -> B_185_394_0 [ 0 ] , _rtB -> B_185_399_0 , & _rtB ->
Subsystempi2delay_nu , & _rtDW -> Subsystempi2delay_nu ) ; if ( _rtB ->
B_185_402_0 != 0 ) { _rtB -> B_185_407_0 [ 0 ] = _rtB -> Subsystem1_j .
B_35_0_0 ; _rtB -> B_185_407_0 [ 1 ] = _rtB -> Subsystem1_j . B_35_1_0 ; }
else { _rtB -> B_185_407_0 [ 0 ] = _rtB -> Subsystempi2delay_nu . B_34_0_0 ;
_rtB -> B_185_407_0 [ 1 ] = _rtB -> Subsystempi2delay_nu . B_34_1_0 ; } isHit
= ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { B_185_410_0_idx_0 = _rtDW
-> Delay_x1_DSTATE_h [ 0 ] ; B_185_412_0_idx_0 = _rtDW -> Delay_x2_DSTATE_m [
0 ] ; B_185_410_0_idx_1 = _rtDW -> Delay_x1_DSTATE_h [ 1 ] ;
B_185_412_0_idx_1 = _rtDW -> Delay_x2_DSTATE_m [ 1 ] ; B_185_410_0_idx_2 =
_rtDW -> Delay_x1_DSTATE_h [ 2 ] ; B_185_412_0_idx_2 = _rtDW ->
Delay_x2_DSTATE_m [ 2 ] ; _rtB -> B_185_415_0 [ 0 ] = ( _rtP -> P_378 * _rtDW
-> Delay_x1_DSTATE_h [ 0 ] + _rtP -> P_380 * _rtDW -> Delay_x2_DSTATE_m [ 0 ]
) + _rtP -> P_376 * _rtB -> B_185_407_0 [ 0 ] ; _rtB -> B_185_415_0 [ 1 ] = (
_rtP -> P_378 * _rtDW -> Delay_x1_DSTATE_h [ 1 ] + _rtP -> P_380 * _rtDW ->
Delay_x2_DSTATE_m [ 1 ] ) + _rtP -> P_376 * _rtB -> B_185_407_0 [ 1 ] ; _rtB
-> B_185_415_0 [ 2 ] = ( _rtP -> P_378 * _rtDW -> Delay_x1_DSTATE_h [ 2 ] +
_rtP -> P_380 * _rtDW -> Delay_x2_DSTATE_m [ 2 ] ) + _rtP -> P_376 * _rtB ->
B_185_408_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
rtb_B_185_30_0 = ssGetTaskTime ( S , 1 ) ; _rtDW -> Step_MODE_n [ 0 ] = (
rtb_B_185_30_0 >= _rtP -> P_381 ) ; if ( _rtDW -> Step_MODE_n [ 0 ] == 1 ) {
B_185_416_0_idx_0 = _rtP -> P_383 [ 0 ] ; } else { B_185_416_0_idx_0 = _rtP
-> P_382 [ 0 ] ; } _rtDW -> Step_MODE_n [ 1 ] = ( rtb_B_185_30_0 >= _rtP ->
P_381 ) ; if ( _rtDW -> Step_MODE_n [ 1 ] == 1 ) { B_185_416_0_idx_1 = _rtP
-> P_383 [ 1 ] ; } else { B_185_416_0_idx_1 = _rtP -> P_382 [ 1 ] ; } } isHit
= ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { isHit = ssIsSampleHit ( S
, 3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_418_0 = _rtDW ->
UnitDelay1_DSTATE_a ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { _rtB -> B_185_420_0 = _rtB -> B_185_419_0 - _rtB -> B_185_418_0 ; }
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( rtb_B_185_34_0
> _rtP -> P_387 ) { rtb_B_185_30_0 = _rtB -> B_185_420_0 ; } else {
rtb_B_185_30_0 = _rtB -> B_185_421_0 ; } _rtB -> B_185_423_0 = _rtP -> P_388
* rtb_B_185_30_0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 )
{ rtb_B_4_33_0 = _rtP -> P_389 * _rtB -> B_185_423_0 ; B_4_25_0_idx_0 = _rtDW
-> DiscreteTimeIntegrator1_DSTATE_h + rtb_B_4_33_0 ; if ( ( ( B_4_25_0_idx_0
>= _rtB -> B_185_427_0 ) && ( _rtB -> B_185_423_0 >
normal_operation_no_faults_rtC ( S ) -> B_185_429_0 ) ) || ( ( B_4_25_0_idx_0
<= _rtB -> B_185_432_0 ) && ( _rtB -> B_185_423_0 <
normal_operation_no_faults_rtC ( S ) -> B_185_434_0 ) ) ) { _rtB ->
B_185_439_0 = _rtB -> B_185_425_0 ; } else { _rtB -> B_185_439_0 = _rtP ->
P_395 * _rtB -> B_185_423_0 ; } if ( _rtDW ->
DiscreteTimeIntegrator1_SYSTEM_ENABLE_i != 0 ) { _rtB -> B_185_440_0 = _rtDW
-> DiscreteTimeIntegrator1_DSTATE_h ; } else { _rtB -> B_185_440_0 = _rtP ->
P_396 * _rtB -> B_185_439_0 + _rtDW -> DiscreteTimeIntegrator1_DSTATE_h ; }
if ( _rtB -> B_185_440_0 >= _rtP -> P_398 ) { _rtB -> B_185_440_0 = _rtP ->
P_398 ; } else { if ( _rtB -> B_185_440_0 <= _rtP -> P_399 ) { _rtB ->
B_185_440_0 = _rtP -> P_399 ; } } _rtB -> B_185_441_0 = rtb_B_4_33_0 + _rtB
-> B_185_440_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
if ( _rtB -> B_185_441_0 > B_185_416_0_idx_0 ) { _rtB -> B_185_444_0 =
B_185_416_0_idx_0 ; } else if ( _rtB -> B_185_441_0 < B_185_416_0_idx_1 ) {
_rtB -> B_185_444_0 = B_185_416_0_idx_1 ; } else { _rtB -> B_185_444_0 = _rtB
-> B_185_441_0 ; } } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 )
{ rtb_B_185_2_0 [ 0 ] = _rtDW -> UnitDelay6_DSTATE_h [ 0 ] ; rtb_B_185_2_0 [
1 ] = _rtDW -> UnitDelay6_DSTATE_h [ 1 ] ; rtb_B_185_2_0 [ 2 ] = _rtDW ->
UnitDelay6_DSTATE_h [ 2 ] ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit
!= 0 ) { _rtB -> B_185_446_0 [ 0 ] = rtb_B_185_2_0 [ 0 ] ; _rtB ->
B_185_446_0 [ 1 ] = rtb_B_185_2_0 [ 1 ] ; _rtB -> B_185_446_0 [ 2 ] =
rtb_B_185_2_0 [ 2 ] ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { _rtB -> B_185_449_0 [ isHit
] = _rtP -> P_403 * ( _rtP -> P_402 [ isHit + 6 ] * ( _rtP -> P_401 * _rtB ->
B_185_446_0 [ 2 ] ) + ( _rtP -> P_402 [ isHit + 3 ] * ( _rtP -> P_401 * _rtB
-> B_185_446_0 [ 1 ] ) + _rtP -> P_401 * _rtB -> B_185_446_0 [ 0 ] * _rtP ->
P_402 [ isHit ] ) ) ; } } normal_operation_no_faults_Subsystem1_h4 ( S , _rtB
-> B_185_452_0 , & _rtB -> B_185_449_0 [ 0 ] , _rtB -> B_185_399_0 , & _rtB
-> Subsystem1_c , & _rtDW -> Subsystem1_c ) ;
normal_operation_no_faults_Subsystempi2delay_m ( S , _rtB -> B_185_455_0 , &
_rtB -> B_185_449_0 [ 0 ] , _rtB -> B_185_399_0 , & _rtB ->
Subsystempi2delay_n , & _rtDW -> Subsystempi2delay_n ) ; if ( _rtB ->
B_185_452_0 != 0 ) { _rtB -> B_185_457_0 [ 0 ] = _rtB -> Subsystem1_c .
B_35_0_0 ; _rtB -> B_185_457_0 [ 1 ] = _rtB -> Subsystem1_c . B_35_1_0 ; }
else { _rtB -> B_185_457_0 [ 0 ] = _rtB -> Subsystempi2delay_n . B_34_0_0 ;
_rtB -> B_185_457_0 [ 1 ] = _rtB -> Subsystempi2delay_n . B_34_1_0 ; } _rtB
-> B_185_458_0 = _rtB -> B_185_444_0 - _rtB -> B_185_457_0 [ 0 ] ; isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { rtb_B_4_33_0 = _rtP ->
P_407 * _rtB -> B_185_458_0 ; B_4_25_0_idx_0 = _rtDW ->
DiscreteTimeIntegrator_DSTATE_i + rtb_B_4_33_0 ; if ( ( ( B_4_25_0_idx_0 >=
_rtB -> B_185_462_0 ) && ( _rtB -> B_185_458_0 >
normal_operation_no_faults_rtC ( S ) -> B_185_464_0 ) ) || ( ( B_4_25_0_idx_0
<= _rtB -> B_185_467_0 ) && ( _rtB -> B_185_458_0 <
normal_operation_no_faults_rtC ( S ) -> B_185_469_0 ) ) ) { _rtB ->
B_185_474_0 = _rtB -> B_185_460_0 ; } else { _rtB -> B_185_474_0 = _rtP ->
P_413 * _rtB -> B_185_458_0 ; } if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_e != 0 ) { _rtB -> B_185_475_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_i ; } else { _rtB -> B_185_475_0 = _rtP ->
P_414 * _rtB -> B_185_474_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_i ; } if
( _rtB -> B_185_475_0 >= _rtP -> P_416 ) { _rtB -> B_185_475_0 = _rtP ->
P_416 ; } else { if ( _rtB -> B_185_475_0 <= _rtP -> P_417 ) { _rtB ->
B_185_475_0 = _rtP -> P_417 ; } } B_185_29_0_idx_0 = rtb_B_4_33_0 + _rtB ->
B_185_475_0 ; if ( B_185_29_0_idx_0 > _rtP -> P_418 ) { _rtB -> B_185_477_0 =
_rtP -> P_418 ; } else if ( B_185_29_0_idx_0 < _rtP -> P_419 ) { _rtB ->
B_185_477_0 = _rtP -> P_419 ; } else { _rtB -> B_185_477_0 = B_185_29_0_idx_0
; } } rtb_B_4_33_0 = _rtP -> P_420 * _rtB -> B_185_457_0 [ 0 ] ;
normal_operation_no_faults_ACVoltageController ( S , _rtB -> B_185_93_0 ,
_rtB -> B_185_479_0 , & _rtB -> ACVoltageController_n , & _rtDW ->
ACVoltageController_n , & _rtP -> ACVoltageController_n , & _rtX ->
ACVoltageController_n , & ( ( XDis_normal_operation_no_faults_T * )
ssGetContStateDisabled ( S ) ) -> ACVoltageController_n ) ; _rtB ->
B_185_484_0 = _rtB -> B_185_415_0 [ 0 ] - ( ( _rtB -> B_185_477_0 +
rtb_B_4_33_0 ) - ( _rtB -> B_185_457_0 [ 1 ] + _rtB -> ACVoltageController_n
. B_25_18_0 ) * _rtP -> P_422 ) ; _rtB -> B_185_487_0 = _rtB ->
ACVoltageController_n . B_25_18_0 - _rtB -> B_185_457_0 [ 1 ] ; _rtB ->
B_185_489_0 = _rtX -> Integrator_CSTATE_o ; _rtB -> B_185_493_0 = _rtB ->
B_185_415_0 [ 1 ] - ( ( ( _rtB -> B_185_457_0 [ 0 ] + _rtB -> B_185_444_0 ) *
_rtP -> P_423 + ( _rtP -> P_424 * _rtB -> B_185_487_0 + _rtB -> B_185_489_0 )
) + _rtP -> P_426 * _rtB -> B_185_457_0 [ 1 ] ) ; if ( ssIsMajorTimeStep ( S
) != 0 ) { _rtDW -> Saturation_MODE_k [ 0 ] = _rtB -> B_185_484_0 >= _rtP ->
P_428 ? 1 : _rtB -> B_185_484_0 > _rtP -> P_429 ? 0 : - 1 ; _rtDW ->
Saturation_MODE_k [ 1 ] = _rtB -> B_185_493_0 >= _rtP -> P_428 ? 1 : _rtB ->
B_185_493_0 > _rtP -> P_429 ? 0 : - 1 ; _rtDW -> Saturation_MODE_k [ 2 ] =
_rtB -> B_185_494_0 >= _rtP -> P_428 ? 1 : _rtB -> B_185_494_0 > _rtP ->
P_429 ? 0 : - 1 ; } B_185_495_0 [ 0 ] = _rtDW -> Saturation_MODE_k [ 0 ] == 1
? _rtP -> P_428 : _rtDW -> Saturation_MODE_k [ 0 ] == - 1 ? _rtP -> P_429 :
_rtB -> B_185_484_0 ; B_185_495_0 [ 1 ] = _rtDW -> Saturation_MODE_k [ 1 ] ==
1 ? _rtP -> P_428 : _rtDW -> Saturation_MODE_k [ 1 ] == - 1 ? _rtP -> P_429 :
_rtB -> B_185_493_0 ; B_185_495_0 [ 2 ] = _rtDW -> Saturation_MODE_k [ 2 ] ==
1 ? _rtP -> P_428 : _rtDW -> Saturation_MODE_k [ 2 ] == - 1 ? _rtP -> P_429 :
_rtB -> B_185_494_0 ; normal_operation_no_faults_Subsystem1_h ( S , _rtB ->
B_185_498_0 , & B_185_495_0 [ 0 ] , _rtB -> B_185_399_0 , & _rtB ->
Subsystem1_g , & _rtDW -> Subsystem1_g ) ;
normal_operation_no_faults_Subsystempi2delay_e ( S , _rtB -> B_185_501_0 , &
B_185_495_0 [ 0 ] , _rtB -> B_185_399_0 , & _rtB -> Subsystempi2delay_h , &
_rtDW -> Subsystempi2delay_h ) ; if ( _rtB -> B_185_498_0 != 0 ) {
B_185_416_0_idx_0 = _rtB -> Subsystem1_g . B_6_0_0 ; B_185_416_0_idx_1 = _rtB
-> Subsystem1_g . B_6_1_0 ; } else { B_185_416_0_idx_0 = _rtB ->
Subsystempi2delay_h . B_5_0_0 ; B_185_416_0_idx_1 = _rtB ->
Subsystempi2delay_h . B_5_1_0 ; }
normal_operation_no_faults_ZeroSequenceController ( S , _rtB -> B_185_394_0 [
2 ] , _rtB -> B_185_449_0 [ 2 ] , 0.0 , & _rtB -> ZeroSequenceController2_f ,
& _rtDW -> ZeroSequenceController2_f , & _rtP -> ZeroSequenceController2_f ,
& _rtX -> ZeroSequenceController2_f ) ; _rtB -> B_185_507_0 [ 0 ] =
B_185_416_0_idx_0 * _rtB -> B_185_506_0 ; _rtB -> B_185_507_0 [ 1 ] =
B_185_416_0_idx_1 * _rtB -> B_185_506_0 ; _rtB -> B_185_507_0 [ 2 ] = (
B_185_495_0 [ 2 ] + _rtB -> ZeroSequenceController2_f . B_19_36_0 ) * _rtB ->
B_185_506_0 ; for ( i = 0 ; i < 3 ; i ++ ) { rtb_B_185_236_0 = _rtP -> P_434
[ i + 6 ] * _rtB -> B_185_507_0 [ 2 ] + ( _rtP -> P_434 [ i + 3 ] * _rtB ->
B_185_507_0 [ 1 ] + _rtP -> P_434 [ i ] * _rtB -> B_185_507_0 [ 0 ] ) ;
rtb_B_185_2_0 [ i ] = _rtP -> P_435 * rtb_B_185_236_0 ; rtb_B_185_124_0 [ i ]
= rtb_B_185_236_0 ; }
normal_operation_no_faults_Circulatingcurrentsuppression ( S , _rtB ->
B_185_125_0 , & _rtB -> Circulatingcurrentsuppression_d , & _rtDW ->
Circulatingcurrentsuppression_d , & _rtP -> Circulatingcurrentsuppression_d ,
& _rtX -> Circulatingcurrentsuppression_d , & ( (
XDis_normal_operation_no_faults_T * ) ssGetContStateDisabled ( S ) ) ->
Circulatingcurrentsuppression_d ) ; _rtB -> B_185_511_0 [ 0 ] = rtb_B_185_2_0
[ 0 ] - _rtB -> Circulatingcurrentsuppression_d . B_30_57_0 [ 0 ] ; _rtB ->
B_185_511_0 [ 3 ] = rtb_B_185_124_0 [ 0 ] - _rtB ->
Circulatingcurrentsuppression_d . B_30_57_0 [ 0 ] ; _rtB -> B_185_511_0 [ 1 ]
= rtb_B_185_2_0 [ 1 ] - _rtB -> Circulatingcurrentsuppression_d . B_30_57_0 [
1 ] ; _rtB -> B_185_511_0 [ 4 ] = rtb_B_185_124_0 [ 1 ] - _rtB ->
Circulatingcurrentsuppression_d . B_30_57_0 [ 1 ] ; _rtB -> B_185_511_0 [ 2 ]
= rtb_B_185_2_0 [ 2 ] - _rtB -> Circulatingcurrentsuppression_d . B_30_57_0 [
2 ] ; _rtB -> B_185_511_0 [ 5 ] = rtb_B_185_124_0 [ 2 ] - _rtB ->
Circulatingcurrentsuppression_d . B_30_57_0 [ 2 ] ; isHit = ssIsSampleHit ( S
, 1 , 0 ) ; if ( isHit != 0 ) { isHit = ssIsSampleHit ( S , 2 , 0 ) ; if (
isHit != 0 ) { for ( i = 0 ; i < 6 ; i ++ ) { _rtB -> B_185_512_0 [ i ] =
_rtB -> B_185_511_0 [ i ] ; } } } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if (
isHit != 0 ) { rtb_B_4_33_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50
; i ++ ) { B_85_0_0 [ i ] = rtb_B_4_33_0 + _rtB -> B_185_514_0 [ i ] ;
B_85_0_0 [ i ] = muDoubleScalarRem ( B_85_0_0 [ i ] , _rtB -> B_185_515_0 ) ;
B_85_2_0 [ i ] = B_85_0_0 [ i ] * _rtB -> B_185_516_0 ; } { int_T i1 ; const
real_T * u0 = & B_85_2_0 [ 0 ] ; real_T * y0 = & B_85_0_0 [ 0 ] ; for ( i1 =
0 ; i1 < 50 ; i1 ++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T (
& ( y0 [ i1 ] ) , _rtP -> P_74 , u0 [ i1 ] , _rtP -> P_73 , 2U ) ; } } for (
i = 0 ; i < 50 ; i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_512_0 [ 0 ] >=
B_85_0_0 [ i ] ) ; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ;
tmpForInput [ 1 ] = ( int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = (
int8_T ) rtb_B_51_4_0 [ 2 ] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3
] ; tmpForInput [ 4 ] = ( int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = (
int8_T ) rtb_B_51_4_0 [ 5 ] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6
] ; tmpForInput [ 7 ] = ( int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = (
int8_T ) rtb_B_51_4_0 [ 8 ] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9
] ; tmpForInput [ 10 ] = ( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ]
= ( int8_T ) rtb_B_51_4_0 [ 11 ] ; tmpForInput [ 12 ] = ( int8_T )
rtb_B_51_4_0 [ 12 ] ; tmpForInput [ 13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ;
tmpForInput [ 14 ] = ( int8_T ) rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = (
int8_T ) rtb_B_51_4_0 [ 15 ] ; tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [
16 ] ; tmpForInput [ 17 ] = ( int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18
] = ( int8_T ) rtb_B_51_4_0 [ 18 ] ; tmpForInput [ 19 ] = ( int8_T )
rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20 ] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ;
tmpForInput [ 21 ] = ( int8_T ) rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = (
int8_T ) rtb_B_51_4_0 [ 22 ] ; tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [
23 ] ; tmpForInput [ 24 ] = ( int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25
] = ( int8_T ) rtb_B_51_4_0 [ 25 ] ; tmpForInput [ 26 ] = ( int8_T )
rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27 ] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ;
tmpForInput [ 28 ] = ( int8_T ) rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = (
int8_T ) rtb_B_51_4_0 [ 29 ] ; tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [
30 ] ; tmpForInput [ 31 ] = ( int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32
] = ( int8_T ) rtb_B_51_4_0 [ 32 ] ; tmpForInput [ 33 ] = ( int8_T )
rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34 ] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ;
tmpForInput [ 35 ] = ( int8_T ) rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = (
int8_T ) rtb_B_51_4_0 [ 36 ] ; tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [
37 ] ; tmpForInput [ 38 ] = ( int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39
] = ( int8_T ) rtb_B_51_4_0 [ 39 ] ; tmpForInput [ 40 ] = ( int8_T )
rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41 ] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ;
tmpForInput [ 42 ] = ( int8_T ) rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = (
int8_T ) rtb_B_51_4_0 [ 43 ] ; tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [
44 ] ; tmpForInput [ 45 ] = ( int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46
] = ( int8_T ) rtb_B_51_4_0 [ 46 ] ; tmpForInput [ 47 ] = ( int8_T )
rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48 ] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ;
tmpForInput [ 49 ] = ( int8_T ) rtb_B_51_4_0 [ 49 ] ; rtb_B_85_7_0 = - 0.0 ;
for ( isHit = 0 ; isHit < 50 ; isHit ++ ) { rtb_B_85_7_0 += ( real_T )
tmpForInput [ isHit ] ; } B_185_519_0_idx_0 = rtb_B_85_7_0 ; _rtB ->
B_185_521_0 = _rtDW -> UnitDelay4_DSTATE_n ; rtb_B_85_7_0 = _rtP -> P_443 *
_rtB -> B_185_521_0 - _rtDW -> UnitDelay3_DSTATE_od ; B_185_519_0_idx_1 =
B_185_519_0_idx_0 ; if ( B_185_519_0_idx_0 > _rtP -> P_440 ) {
B_185_519_0_idx_1 = _rtP -> P_440 ; } else { if ( B_185_519_0_idx_0 < _rtP ->
P_441 ) { B_185_519_0_idx_1 = _rtP -> P_441 ; } } _rtB -> B_185_525_0 [ 0 ] =
B_185_519_0_idx_1 * rtb_B_85_7_0 ; B_185_519_0_idx_0 = B_185_519_0_idx_1 ;
B_185_519_0_idx_1 = _rtB -> B_185_517_0 ; if ( _rtB -> B_185_517_0 > _rtP ->
P_440 ) { B_185_519_0_idx_1 = _rtP -> P_440 ; } else { if ( _rtB ->
B_185_517_0 < _rtP -> P_441 ) { B_185_519_0_idx_1 = _rtP -> P_441 ; } } _rtB
-> B_185_525_0 [ 1 ] = B_185_519_0_idx_1 * rtb_B_85_7_0 ; rtb_B_85_7_0 =
ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) { B_81_0_0 [ i ] =
rtb_B_85_7_0 + _rtB -> B_185_527_0 [ i ] ; B_81_0_0 [ i ] = muDoubleScalarRem
( B_81_0_0 [ i ] , _rtB -> B_185_528_0 ) ; B_81_2_0 [ i ] = B_81_0_0 [ i ] *
_rtB -> B_185_529_0 ; } { int_T i1 ; const real_T * u0 = & B_81_2_0 [ 0 ] ;
real_T * y0 = & B_81_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1 ++ ) {
normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] ) , _rtP
-> P_66 , u0 [ i1 ] , _rtP -> P_65 , 2U ) ; } } for ( i = 0 ; i < 50 ; i ++ )
{ rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_512_0 [ 4 ] >= B_81_0_0 [ i ] ) ; }
tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_81_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_81_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_85_7_0 = rtb_B_81_7_0 ; _rtB -> B_185_534_0 = _rtDW ->
UnitDelay4_DSTATE_lc ; rtb_B_81_7_0 = _rtP -> P_452 * _rtB -> B_185_534_0 -
_rtDW -> UnitDelay3_DSTATE_o5 ; B_185_532_0_idx_1 = rtb_B_85_7_0 ; if (
rtb_B_85_7_0 > _rtP -> P_449 ) { B_185_532_0_idx_1 = _rtP -> P_449 ; } else {
if ( rtb_B_85_7_0 < _rtP -> P_450 ) { B_185_532_0_idx_1 = _rtP -> P_450 ; } }
_rtB -> B_185_538_0 [ 0 ] = B_185_532_0_idx_1 * rtb_B_81_7_0 ; rtb_B_85_7_0 =
B_185_532_0_idx_1 ; B_185_532_0_idx_1 = _rtB -> B_185_530_0 ; if ( _rtB ->
B_185_530_0 > _rtP -> P_449 ) { B_185_532_0_idx_1 = _rtP -> P_449 ; } else {
if ( _rtB -> B_185_530_0 < _rtP -> P_450 ) { B_185_532_0_idx_1 = _rtP ->
P_450 ; } } _rtB -> B_185_538_0 [ 1 ] = B_185_532_0_idx_1 * rtb_B_81_7_0 ;
rtb_B_81_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_84_0_0 [ i ] = rtb_B_81_7_0 + _rtB -> B_185_540_0 [ i ] ; B_84_0_0 [ i ] =
muDoubleScalarRem ( B_84_0_0 [ i ] , _rtB -> B_185_541_0 ) ; B_84_2_0 [ i ] =
B_84_0_0 [ i ] * _rtB -> B_185_542_0 ; } { int_T i1 ; const real_T * u0 = &
B_84_2_0 [ 0 ] ; real_T * y0 = & B_84_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_72 , u0 [ i1 ] , _rtP -> P_71 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_512_0 [ 1 ] >= B_84_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_84_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_84_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_81_7_0 = rtb_B_84_7_0 ; _rtB -> B_185_547_0 = _rtDW ->
UnitDelay4_DSTATE_b ; rtb_B_84_7_0 = _rtP -> P_461 * _rtB -> B_185_547_0 -
_rtDW -> UnitDelay3_DSTATE_pu ; B_185_545_0_idx_1 = rtb_B_81_7_0 ; if (
rtb_B_81_7_0 > _rtP -> P_458 ) { B_185_545_0_idx_1 = _rtP -> P_458 ; } else {
if ( rtb_B_81_7_0 < _rtP -> P_459 ) { B_185_545_0_idx_1 = _rtP -> P_459 ; } }
_rtB -> B_185_551_0 [ 0 ] = B_185_545_0_idx_1 * rtb_B_84_7_0 ; rtb_B_81_7_0 =
B_185_545_0_idx_1 ; B_185_545_0_idx_1 = _rtB -> B_185_543_0 ; if ( _rtB ->
B_185_543_0 > _rtP -> P_458 ) { B_185_545_0_idx_1 = _rtP -> P_458 ; } else {
if ( _rtB -> B_185_543_0 < _rtP -> P_459 ) { B_185_545_0_idx_1 = _rtP ->
P_459 ; } } _rtB -> B_185_551_0 [ 1 ] = B_185_545_0_idx_1 * rtb_B_84_7_0 ;
rtb_B_84_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_82_0_0 [ i ] = rtb_B_84_7_0 + _rtB -> B_185_553_0 [ i ] ; B_82_0_0 [ i ] =
muDoubleScalarRem ( B_82_0_0 [ i ] , _rtB -> B_185_554_0 ) ; B_82_2_0 [ i ] =
B_82_0_0 [ i ] * _rtB -> B_185_555_0 ; } { int_T i1 ; const real_T * u0 = &
B_82_2_0 [ 0 ] ; real_T * y0 = & B_82_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_68 , u0 [ i1 ] , _rtP -> P_67 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_512_0 [ 3 ] >= B_82_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_82_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_82_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_84_7_0 = rtb_B_82_7_0 ; _rtB -> B_185_560_0 = _rtDW ->
UnitDelay4_DSTATE_k ; rtb_B_82_7_0 = _rtP -> P_470 * _rtB -> B_185_560_0 -
_rtDW -> UnitDelay3_DSTATE_oh ; B_185_558_0_idx_1 = rtb_B_84_7_0 ; if (
rtb_B_84_7_0 > _rtP -> P_467 ) { B_185_558_0_idx_1 = _rtP -> P_467 ; } else {
if ( rtb_B_84_7_0 < _rtP -> P_468 ) { B_185_558_0_idx_1 = _rtP -> P_468 ; } }
_rtB -> B_185_564_0 [ 0 ] = B_185_558_0_idx_1 * rtb_B_82_7_0 ; rtb_B_84_7_0 =
B_185_558_0_idx_1 ; B_185_558_0_idx_1 = _rtB -> B_185_556_0 ; if ( _rtB ->
B_185_556_0 > _rtP -> P_467 ) { B_185_558_0_idx_1 = _rtP -> P_467 ; } else {
if ( _rtB -> B_185_556_0 < _rtP -> P_468 ) { B_185_558_0_idx_1 = _rtP ->
P_468 ; } } _rtB -> B_185_564_0 [ 1 ] = B_185_558_0_idx_1 * rtb_B_82_7_0 ;
rtb_B_82_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_83_0_0 [ i ] = rtb_B_82_7_0 + _rtB -> B_185_566_0 [ i ] ; B_83_0_0 [ i ] =
muDoubleScalarRem ( B_83_0_0 [ i ] , _rtB -> B_185_567_0 ) ; B_83_2_0 [ i ] =
B_83_0_0 [ i ] * _rtB -> B_185_568_0 ; } { int_T i1 ; const real_T * u0 = &
B_83_2_0 [ 0 ] ; real_T * y0 = & B_83_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_70 , u0 [ i1 ] , _rtP -> P_69 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_512_0 [ 2 ] >= B_83_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_83_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_83_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_82_7_0 = rtb_B_83_7_0 ; _rtB -> B_185_573_0 = _rtDW ->
UnitDelay4_DSTATE_d ; rtb_B_83_7_0 = _rtP -> P_479 * _rtB -> B_185_573_0 -
_rtDW -> UnitDelay3_DSTATE_e ; B_185_571_0_idx_1 = rtb_B_82_7_0 ; if (
rtb_B_82_7_0 > _rtP -> P_476 ) { B_185_571_0_idx_1 = _rtP -> P_476 ; } else {
if ( rtb_B_82_7_0 < _rtP -> P_477 ) { B_185_571_0_idx_1 = _rtP -> P_477 ; } }
_rtB -> B_185_577_0 [ 0 ] = B_185_571_0_idx_1 * rtb_B_83_7_0 ; rtb_B_82_7_0 =
B_185_571_0_idx_1 ; B_185_571_0_idx_1 = _rtB -> B_185_569_0 ; if ( _rtB ->
B_185_569_0 > _rtP -> P_476 ) { B_185_571_0_idx_1 = _rtP -> P_476 ; } else {
if ( _rtB -> B_185_569_0 < _rtP -> P_477 ) { B_185_571_0_idx_1 = _rtP ->
P_477 ; } } _rtB -> B_185_577_0 [ 1 ] = B_185_571_0_idx_1 * rtb_B_83_7_0 ;
rtb_B_83_7_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i < 50 ; i ++ ) {
B_80_0_0 [ i ] = rtb_B_83_7_0 + _rtB -> B_185_579_0 [ i ] ; B_80_0_0 [ i ] =
muDoubleScalarRem ( B_80_0_0 [ i ] , _rtB -> B_185_580_0 ) ; B_80_2_0 [ i ] =
B_80_0_0 [ i ] * _rtB -> B_185_581_0 ; } { int_T i1 ; const real_T * u0 = &
B_80_2_0 [ 0 ] ; real_T * y0 = & B_80_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1
++ ) { normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] )
, _rtP -> P_64 , u0 [ i1 ] , _rtP -> P_63 , 2U ) ; } } for ( i = 0 ; i < 50 ;
i ++ ) { rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_512_0 [ 5 ] >= B_80_0_0 [ i ] )
; } tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; B_185_416_0_idx_0 = - 0.0 ; for ( isHit = 0 ; isHit <
50 ; isHit ++ ) { B_185_416_0_idx_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_83_7_0 = B_185_416_0_idx_0 ; _rtB -> B_185_586_0 = _rtDW ->
UnitDelay4_DSTATE_cx ; B_185_416_0_idx_0 = _rtP -> P_488 * _rtB ->
B_185_586_0 - _rtDW -> UnitDelay3_DSTATE_mu ; B_185_584_0_idx_1 =
rtb_B_83_7_0 ; if ( rtb_B_83_7_0 > _rtP -> P_485 ) { B_185_584_0_idx_1 = _rtP
-> P_485 ; } else { if ( rtb_B_83_7_0 < _rtP -> P_486 ) { B_185_584_0_idx_1 =
_rtP -> P_486 ; } } _rtB -> B_185_590_0 [ 0 ] = B_185_584_0_idx_1 *
B_185_416_0_idx_0 ; rtb_B_83_7_0 = B_185_584_0_idx_1 ; B_185_584_0_idx_1 =
_rtB -> B_185_582_0 ; if ( _rtB -> B_185_582_0 > _rtP -> P_485 ) {
B_185_584_0_idx_1 = _rtP -> P_485 ; } else { if ( _rtB -> B_185_582_0 < _rtP
-> P_486 ) { B_185_584_0_idx_1 = _rtP -> P_486 ; } } _rtB -> B_185_590_0 [ 1
] = B_185_584_0_idx_1 * B_185_416_0_idx_0 ; rtb_B_185_2_0 [ 0 ] = _rtDW ->
UnitDelay5_DSTATE_o [ 0 ] ; rtb_B_185_2_0 [ 1 ] = _rtDW ->
UnitDelay5_DSTATE_o [ 1 ] ; rtb_B_185_2_0 [ 2 ] = _rtDW ->
UnitDelay5_DSTATE_o [ 2 ] ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit
!= 0 ) { _rtB -> B_185_593_0 [ 0 ] = rtb_B_185_2_0 [ 0 ] ; _rtB ->
B_185_593_0 [ 1 ] = rtb_B_185_2_0 [ 1 ] ; _rtB -> B_185_593_0 [ 2 ] =
rtb_B_185_2_0 [ 2 ] ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { _rtB -> B_185_597_0 [ isHit
] = _rtP -> P_495 * ( _rtP -> P_492 * _rtB -> B_185_593_0 [ 2 ] * _rtP ->
P_493 * _rtP -> P_494 [ isHit + 6 ] + ( _rtP -> P_492 * _rtB -> B_185_593_0 [
1 ] * _rtP -> P_493 * _rtP -> P_494 [ isHit + 3 ] + _rtP -> P_492 * _rtB ->
B_185_593_0 [ 0 ] * _rtP -> P_493 * _rtP -> P_494 [ isHit ] ) ) ; } } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( ssIsMajorTimeStep ( S
) != 0 ) { _rtDW -> RelationalOperator_Mode_e = ( _rtX -> Integrator_CSTATE_b
> _rtB -> B_185_598_0 ) ; } _rtB -> B_185_599_0 = _rtDW ->
RelationalOperator_Mode_e ; } _rtB -> B_185_600_0 = _rtX ->
Integrator_CSTATE_b - _rtB -> B_185_598_0 ; if ( ( _rtDW ->
Initial_FirstOutputTime_g == ( rtMinusInf ) ) || ( _rtDW ->
Initial_FirstOutputTime_g == ssGetTaskTime ( S , 0 ) ) ) { _rtDW ->
Initial_FirstOutputTime_g = ssGetTaskTime ( S , 0 ) ; _rtB -> B_185_601_0 =
_rtP -> P_497 ; } else { _rtB -> B_185_601_0 = _rtB -> B_185_600_0 ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { rtb_B_185_2288_0 = false ; stateChanged =
false ; didZcEventOccur = ( _rtB -> B_185_599_0 && ( _rtZCE ->
Integrator_Reset_ZCE_n != POS_ZCSIG ) ) ; _rtZCE -> Integrator_Reset_ZCE_n =
_rtB -> B_185_599_0 ; if ( didZcEventOccur || ( _rtDW -> Integrator_IWORK_h
!= 0 ) ) { rtb_B_185_2288_0 = true ; _rtX -> Integrator_CSTATE_b = _rtB ->
B_185_601_0 ; stateChanged = true ; } if ( rtb_B_185_2288_0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( stateChanged ) {
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } } if ( _rtX ->
Integrator_CSTATE_b >= _rtP -> P_498 ) { _rtX -> Integrator_CSTATE_b = _rtP
-> P_498 ; } else { if ( _rtX -> Integrator_CSTATE_b <= _rtP -> P_499 ) {
_rtX -> Integrator_CSTATE_b = _rtP -> P_499 ; } } } _rtB -> B_185_602_0 =
_rtX -> Integrator_CSTATE_b ; normal_operation_no_faults_Subsystem1_h4 ( S ,
_rtB -> B_185_605_0 , & _rtB -> B_185_597_0 [ 0 ] , _rtB -> B_185_602_0 , &
_rtB -> Subsystem1_l , & _rtDW -> Subsystem1_l ) ;
normal_operation_no_faults_Subsystempi2delay_m ( S , _rtB -> B_185_608_0 , &
_rtB -> B_185_597_0 [ 0 ] , _rtB -> B_185_602_0 , & _rtB ->
Subsystempi2delay_j , & _rtDW -> Subsystempi2delay_j ) ; if ( _rtB ->
B_185_605_0 != 0 ) { _rtB -> B_185_610_0 [ 0 ] = _rtB -> Subsystem1_l .
B_35_0_0 ; _rtB -> B_185_610_0 [ 1 ] = _rtB -> Subsystem1_l . B_35_1_0 ; }
else { _rtB -> B_185_610_0 [ 0 ] = _rtB -> Subsystempi2delay_j . B_34_0_0 ;
_rtB -> B_185_610_0 [ 1 ] = _rtB -> Subsystempi2delay_j . B_34_1_0 ; } isHit
= ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { B_185_613_0_idx_0 = _rtDW
-> Delay_x1_DSTATE_o [ 0 ] ; B_185_615_0_idx_0 = _rtDW -> Delay_x2_DSTATE_n [
0 ] ; B_185_613_0_idx_1 = _rtDW -> Delay_x1_DSTATE_o [ 1 ] ;
B_185_615_0_idx_1 = _rtDW -> Delay_x2_DSTATE_n [ 1 ] ; B_185_613_0_idx_2 =
_rtDW -> Delay_x1_DSTATE_o [ 2 ] ; B_185_615_0_idx_2 = _rtDW ->
Delay_x2_DSTATE_n [ 2 ] ; _rtB -> B_185_618_0 [ 0 ] = ( _rtP -> P_506 * _rtDW
-> Delay_x1_DSTATE_o [ 0 ] + _rtP -> P_508 * _rtDW -> Delay_x2_DSTATE_n [ 0 ]
) + _rtP -> P_504 * _rtB -> B_185_610_0 [ 0 ] ; _rtB -> B_185_618_0 [ 1 ] = (
_rtP -> P_506 * _rtDW -> Delay_x1_DSTATE_o [ 1 ] + _rtP -> P_508 * _rtDW ->
Delay_x2_DSTATE_n [ 1 ] ) + _rtP -> P_504 * _rtB -> B_185_610_0 [ 1 ] ; _rtB
-> B_185_618_0 [ 2 ] = ( _rtP -> P_506 * _rtDW -> Delay_x1_DSTATE_o [ 2 ] +
_rtP -> P_508 * _rtDW -> Delay_x2_DSTATE_n [ 2 ] ) + _rtP -> P_504 * _rtB ->
B_185_611_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
rtb_B_185_30_0 = ssGetTaskTime ( S , 1 ) ; _rtDW -> Step_MODE_a [ 0 ] = (
rtb_B_185_30_0 >= _rtP -> P_509 ) ; if ( _rtDW -> Step_MODE_a [ 0 ] == 1 ) {
B_185_619_0_idx_0 = _rtP -> P_511 [ 0 ] ; } else { B_185_619_0_idx_0 = _rtP
-> P_510 [ 0 ] ; } _rtDW -> Step_MODE_a [ 1 ] = ( rtb_B_185_30_0 >= _rtP ->
P_509 ) ; if ( _rtDW -> Step_MODE_a [ 1 ] == 1 ) { B_185_619_0_idx_1 = _rtP
-> P_511 [ 1 ] ; } else { B_185_619_0_idx_1 = _rtP -> P_510 [ 1 ] ; } } isHit
= ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { isHit = ssIsSampleHit ( S
, 3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_621_0 = _rtDW ->
UnitDelay1_DSTATE_i ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { _rtB -> B_185_623_0 = _rtB -> B_185_622_0 - _rtB -> B_185_621_0 ; }
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( rtb_B_185_34_0
> _rtP -> P_515 ) { rtb_B_185_34_0 = _rtB -> B_185_623_0 ; } else {
rtb_B_185_34_0 = _rtB -> B_185_624_0 ; } _rtB -> B_185_626_0 = _rtP -> P_516
* rtb_B_185_34_0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 )
{ rtb_B_185_34_0 = _rtP -> P_517 * _rtB -> B_185_626_0 ; B_185_416_0_idx_0 =
_rtDW -> DiscreteTimeIntegrator1_DSTATE_a + rtb_B_185_34_0 ; if ( ( (
B_185_416_0_idx_0 >= _rtB -> B_185_630_0 ) && ( _rtB -> B_185_626_0 >
normal_operation_no_faults_rtC ( S ) -> B_185_632_0 ) ) || ( (
B_185_416_0_idx_0 <= _rtB -> B_185_635_0 ) && ( _rtB -> B_185_626_0 <
normal_operation_no_faults_rtC ( S ) -> B_185_637_0 ) ) ) { _rtB ->
B_185_642_0 = _rtB -> B_185_628_0 ; } else { _rtB -> B_185_642_0 = _rtP ->
P_523 * _rtB -> B_185_626_0 ; } if ( _rtDW ->
DiscreteTimeIntegrator1_SYSTEM_ENABLE_l != 0 ) { _rtB -> B_185_643_0 = _rtDW
-> DiscreteTimeIntegrator1_DSTATE_a ; } else { _rtB -> B_185_643_0 = _rtP ->
P_524 * _rtB -> B_185_642_0 + _rtDW -> DiscreteTimeIntegrator1_DSTATE_a ; }
if ( _rtB -> B_185_643_0 >= _rtP -> P_526 ) { _rtB -> B_185_643_0 = _rtP ->
P_526 ; } else { if ( _rtB -> B_185_643_0 <= _rtP -> P_527 ) { _rtB ->
B_185_643_0 = _rtP -> P_527 ; } } _rtB -> B_185_644_0 = rtb_B_185_34_0 + _rtB
-> B_185_643_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
if ( _rtB -> B_185_644_0 > B_185_619_0_idx_0 ) { _rtB -> B_185_647_0 =
B_185_619_0_idx_0 ; } else if ( _rtB -> B_185_644_0 < B_185_619_0_idx_1 ) {
_rtB -> B_185_647_0 = B_185_619_0_idx_1 ; } else { _rtB -> B_185_647_0 = _rtB
-> B_185_644_0 ; } } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 )
{ rtb_B_185_2_0 [ 0 ] = _rtDW -> UnitDelay6_DSTATE_i [ 0 ] ; rtb_B_185_2_0 [
1 ] = _rtDW -> UnitDelay6_DSTATE_i [ 1 ] ; rtb_B_185_2_0 [ 2 ] = _rtDW ->
UnitDelay6_DSTATE_i [ 2 ] ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit
!= 0 ) { _rtB -> B_185_649_0 [ 0 ] = rtb_B_185_2_0 [ 0 ] ; _rtB ->
B_185_649_0 [ 1 ] = rtb_B_185_2_0 [ 1 ] ; _rtB -> B_185_649_0 [ 2 ] =
rtb_B_185_2_0 [ 2 ] ; } } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { _rtB -> B_185_652_0 [ isHit
] = _rtP -> P_531 * ( _rtP -> P_530 [ isHit + 6 ] * ( _rtP -> P_529 * _rtB ->
B_185_649_0 [ 2 ] ) + ( _rtP -> P_530 [ isHit + 3 ] * ( _rtP -> P_529 * _rtB
-> B_185_649_0 [ 1 ] ) + _rtP -> P_529 * _rtB -> B_185_649_0 [ 0 ] * _rtP ->
P_530 [ isHit ] ) ) ; } } normal_operation_no_faults_Subsystem1_h4 ( S , _rtB
-> B_185_655_0 , & _rtB -> B_185_652_0 [ 0 ] , _rtB -> B_185_602_0 , & _rtB
-> Subsystem1_jf , & _rtDW -> Subsystem1_jf ) ;
normal_operation_no_faults_Subsystempi2delay_m ( S , _rtB -> B_185_658_0 , &
_rtB -> B_185_652_0 [ 0 ] , _rtB -> B_185_602_0 , & _rtB ->
Subsystempi2delay_b3 , & _rtDW -> Subsystempi2delay_b3 ) ; if ( _rtB ->
B_185_655_0 != 0 ) { _rtB -> B_185_660_0 [ 0 ] = _rtB -> Subsystem1_jf .
B_35_0_0 ; _rtB -> B_185_660_0 [ 1 ] = _rtB -> Subsystem1_jf . B_35_1_0 ; }
else { _rtB -> B_185_660_0 [ 0 ] = _rtB -> Subsystempi2delay_b3 . B_34_0_0 ;
_rtB -> B_185_660_0 [ 1 ] = _rtB -> Subsystempi2delay_b3 . B_34_1_0 ; } _rtB
-> B_185_661_0 = _rtB -> B_185_647_0 - _rtB -> B_185_660_0 [ 0 ] ; isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_34_0 = _rtP ->
P_535 * _rtB -> B_185_661_0 ; B_185_416_0_idx_0 = _rtDW ->
DiscreteTimeIntegrator_DSTATE_j + rtb_B_185_34_0 ; if ( ( ( B_185_416_0_idx_0
>= _rtB -> B_185_665_0 ) && ( _rtB -> B_185_661_0 >
normal_operation_no_faults_rtC ( S ) -> B_185_667_0 ) ) || ( (
B_185_416_0_idx_0 <= _rtB -> B_185_670_0 ) && ( _rtB -> B_185_661_0 <
normal_operation_no_faults_rtC ( S ) -> B_185_672_0 ) ) ) { _rtB ->
B_185_677_0 = _rtB -> B_185_663_0 ; } else { _rtB -> B_185_677_0 = _rtP ->
P_541 * _rtB -> B_185_661_0 ; } if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_c != 0 ) { _rtB -> B_185_678_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_j ; } else { _rtB -> B_185_678_0 = _rtP ->
P_542 * _rtB -> B_185_677_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_j ; } if
( _rtB -> B_185_678_0 >= _rtP -> P_544 ) { _rtB -> B_185_678_0 = _rtP ->
P_544 ; } else { if ( _rtB -> B_185_678_0 <= _rtP -> P_545 ) { _rtB ->
B_185_678_0 = _rtP -> P_545 ; } } B_185_29_0_idx_0 = rtb_B_185_34_0 + _rtB ->
B_185_678_0 ; if ( B_185_29_0_idx_0 > _rtP -> P_546 ) { _rtB -> B_185_680_0 =
_rtP -> P_546 ; } else if ( B_185_29_0_idx_0 < _rtP -> P_547 ) { _rtB ->
B_185_680_0 = _rtP -> P_547 ; } else { _rtB -> B_185_680_0 = B_185_29_0_idx_0
; } } rtb_B_185_34_0 = _rtP -> P_548 * _rtB -> B_185_660_0 [ 0 ] ;
normal_operation_no_faults_ACVoltageController ( S , _rtB -> B_185_93_0 ,
_rtB -> B_185_682_0 , & _rtB -> ACVoltageController_l , & _rtDW ->
ACVoltageController_l , & _rtP -> ACVoltageController_l , & _rtX ->
ACVoltageController_l , & ( ( XDis_normal_operation_no_faults_T * )
ssGetContStateDisabled ( S ) ) -> ACVoltageController_l ) ; _rtB ->
B_185_687_0 = _rtB -> B_185_618_0 [ 0 ] - ( ( _rtB -> B_185_680_0 +
rtb_B_185_34_0 ) - ( _rtB -> B_185_660_0 [ 1 ] + _rtB ->
ACVoltageController_l . B_25_18_0 ) * _rtP -> P_550 ) ; _rtB -> B_185_690_0 =
_rtB -> ACVoltageController_l . B_25_18_0 - _rtB -> B_185_660_0 [ 1 ] ; _rtB
-> B_185_692_0 = _rtX -> Integrator_CSTATE_m ; _rtB -> B_185_696_0 = _rtB ->
B_185_618_0 [ 1 ] - ( ( ( _rtB -> B_185_660_0 [ 0 ] + _rtB -> B_185_647_0 ) *
_rtP -> P_551 + ( _rtP -> P_552 * _rtB -> B_185_690_0 + _rtB -> B_185_692_0 )
) + _rtP -> P_554 * _rtB -> B_185_660_0 [ 1 ] ) ; if ( ssIsMajorTimeStep ( S
) != 0 ) { _rtDW -> Saturation_MODE_e [ 0 ] = _rtB -> B_185_687_0 >= _rtP ->
P_556 ? 1 : _rtB -> B_185_687_0 > _rtP -> P_557 ? 0 : - 1 ; _rtDW ->
Saturation_MODE_e [ 1 ] = _rtB -> B_185_696_0 >= _rtP -> P_556 ? 1 : _rtB ->
B_185_696_0 > _rtP -> P_557 ? 0 : - 1 ; _rtDW -> Saturation_MODE_e [ 2 ] =
_rtB -> B_185_697_0 >= _rtP -> P_556 ? 1 : _rtB -> B_185_697_0 > _rtP ->
P_557 ? 0 : - 1 ; } B_185_698_0 [ 0 ] = _rtDW -> Saturation_MODE_e [ 0 ] == 1
? _rtP -> P_556 : _rtDW -> Saturation_MODE_e [ 0 ] == - 1 ? _rtP -> P_557 :
_rtB -> B_185_687_0 ; B_185_698_0 [ 1 ] = _rtDW -> Saturation_MODE_e [ 1 ] ==
1 ? _rtP -> P_556 : _rtDW -> Saturation_MODE_e [ 1 ] == - 1 ? _rtP -> P_557 :
_rtB -> B_185_696_0 ; B_185_698_0 [ 2 ] = _rtDW -> Saturation_MODE_e [ 2 ] ==
1 ? _rtP -> P_556 : _rtDW -> Saturation_MODE_e [ 2 ] == - 1 ? _rtP -> P_557 :
_rtB -> B_185_697_0 ; normal_operation_no_faults_Subsystem1_h ( S , _rtB ->
B_185_701_0 , & B_185_698_0 [ 0 ] , _rtB -> B_185_602_0 , & _rtB ->
Subsystem1_e , & _rtDW -> Subsystem1_e ) ;
normal_operation_no_faults_Subsystempi2delay_e ( S , _rtB -> B_185_704_0 , &
B_185_698_0 [ 0 ] , _rtB -> B_185_602_0 , & _rtB -> Subsystempi2delay_e , &
_rtDW -> Subsystempi2delay_e ) ; if ( _rtB -> B_185_701_0 != 0 ) {
B_185_619_0_idx_0 = _rtB -> Subsystem1_e . B_6_0_0 ; B_185_619_0_idx_1 = _rtB
-> Subsystem1_e . B_6_1_0 ; } else { B_185_619_0_idx_0 = _rtB ->
Subsystempi2delay_e . B_5_0_0 ; B_185_619_0_idx_1 = _rtB ->
Subsystempi2delay_e . B_5_1_0 ; }
normal_operation_no_faults_ZeroSequenceController ( S , _rtB -> B_185_597_0 [
2 ] , _rtB -> B_185_652_0 [ 2 ] , 0.0 , & _rtB -> ZeroSequenceController2_fb
, & _rtDW -> ZeroSequenceController2_fb , & _rtP ->
ZeroSequenceController2_fb , & _rtX -> ZeroSequenceController2_fb ) ; _rtB ->
B_185_710_0 [ 0 ] = B_185_619_0_idx_0 * _rtB -> B_185_709_0 ; _rtB ->
B_185_710_0 [ 1 ] = B_185_619_0_idx_1 * _rtB -> B_185_709_0 ; _rtB ->
B_185_710_0 [ 2 ] = ( B_185_698_0 [ 2 ] + _rtB -> ZeroSequenceController2_fb
. B_19_36_0 ) * _rtB -> B_185_709_0 ; for ( i = 0 ; i < 3 ; i ++ ) {
rtb_B_185_236_0 = _rtP -> P_562 [ i + 6 ] * _rtB -> B_185_710_0 [ 2 ] + (
_rtP -> P_562 [ i + 3 ] * _rtB -> B_185_710_0 [ 1 ] + _rtP -> P_562 [ i ] *
_rtB -> B_185_710_0 [ 0 ] ) ; rtb_B_185_2_0 [ i ] = _rtP -> P_563 *
rtb_B_185_236_0 ; rtb_B_185_124_0 [ i ] = rtb_B_185_236_0 ; }
normal_operation_no_faults_Circulatingcurrentsuppression ( S , _rtB ->
B_185_125_0 , & _rtB -> Circulatingcurrentsuppression_k , & _rtDW ->
Circulatingcurrentsuppression_k , & _rtP -> Circulatingcurrentsuppression_k ,
& _rtX -> Circulatingcurrentsuppression_k , & ( (
XDis_normal_operation_no_faults_T * ) ssGetContStateDisabled ( S ) ) ->
Circulatingcurrentsuppression_k ) ; _rtB -> B_185_714_0 [ 0 ] = rtb_B_185_2_0
[ 0 ] - _rtB -> Circulatingcurrentsuppression_k . B_30_57_0 [ 0 ] ; _rtB ->
B_185_714_0 [ 3 ] = rtb_B_185_124_0 [ 0 ] - _rtB ->
Circulatingcurrentsuppression_k . B_30_57_0 [ 0 ] ; _rtB -> B_185_714_0 [ 1 ]
= rtb_B_185_2_0 [ 1 ] - _rtB -> Circulatingcurrentsuppression_k . B_30_57_0 [
1 ] ; _rtB -> B_185_714_0 [ 4 ] = rtb_B_185_124_0 [ 1 ] - _rtB ->
Circulatingcurrentsuppression_k . B_30_57_0 [ 1 ] ; _rtB -> B_185_714_0 [ 2 ]
= rtb_B_185_2_0 [ 2 ] - _rtB -> Circulatingcurrentsuppression_k . B_30_57_0 [
2 ] ; _rtB -> B_185_714_0 [ 5 ] = rtb_B_185_124_0 [ 2 ] - _rtB ->
Circulatingcurrentsuppression_k . B_30_57_0 [ 2 ] ; isHit = ssIsSampleHit ( S
, 1 , 0 ) ; if ( isHit != 0 ) { isHit = ssIsSampleHit ( S , 2 , 0 ) ; if (
isHit != 0 ) { for ( i = 0 ; i < 6 ; i ++ ) { _rtB -> B_185_715_0 [ i ] =
_rtB -> B_185_714_0 [ i ] ; } } } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if (
isHit != 0 ) { rtb_B_185_34_0 = ssGetTaskTime ( S , 2 ) ; for ( i = 0 ; i <
50 ; i ++ ) { B_119_0_0 [ i ] = rtb_B_185_34_0 + _rtB -> B_185_717_0 [ i ] ;
B_119_0_0 [ i ] = muDoubleScalarRem ( B_119_0_0 [ i ] , _rtB -> B_185_718_0 )
; B_119_2_0 [ i ] = B_119_0_0 [ i ] * _rtB -> B_185_719_0 ; } { int_T i1 ;
const real_T * u0 = & B_119_2_0 [ 0 ] ; real_T * y0 = & B_119_0_0 [ 0 ] ; for
( i1 = 0 ; i1 < 50 ; i1 ++ ) {
normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] ) , _rtP
-> P_86 , u0 [ i1 ] , _rtP -> P_85 , 2U ) ; } } for ( i = 0 ; i < 50 ; i ++ )
{ rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_715_0 [ 0 ] >= B_119_0_0 [ i ] ) ; }
tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_119_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_119_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
B_185_722_0_idx_0 = rtb_B_119_7_0 ; _rtB -> B_185_724_0 = _rtDW ->
UnitDelay4_DSTATE_j ; rtb_B_119_7_0 = _rtP -> P_571 * _rtB -> B_185_724_0 -
_rtDW -> UnitDelay3_DSTATE_bw ; B_185_722_0_idx_1 = B_185_722_0_idx_0 ; if (
B_185_722_0_idx_0 > _rtP -> P_568 ) { B_185_722_0_idx_1 = _rtP -> P_568 ; }
else { if ( B_185_722_0_idx_0 < _rtP -> P_569 ) { B_185_722_0_idx_1 = _rtP ->
P_569 ; } } _rtB -> B_185_728_0 [ 0 ] = B_185_722_0_idx_1 * rtb_B_119_7_0 ;
B_185_722_0_idx_0 = B_185_722_0_idx_1 ; B_185_722_0_idx_1 = _rtB ->
B_185_720_0 ; if ( _rtB -> B_185_720_0 > _rtP -> P_568 ) { B_185_722_0_idx_1
= _rtP -> P_568 ; } else { if ( _rtB -> B_185_720_0 < _rtP -> P_569 ) {
B_185_722_0_idx_1 = _rtP -> P_569 ; } } _rtB -> B_185_728_0 [ 1 ] =
B_185_722_0_idx_1 * rtb_B_119_7_0 ; rtb_B_119_7_0 = ssGetTaskTime ( S , 2 ) ;
for ( i = 0 ; i < 50 ; i ++ ) { B_115_0_0 [ i ] = rtb_B_119_7_0 + _rtB ->
B_185_730_0 [ i ] ; B_115_0_0 [ i ] = muDoubleScalarRem ( B_115_0_0 [ i ] ,
_rtB -> B_185_731_0 ) ; B_115_2_0 [ i ] = B_115_0_0 [ i ] * _rtB ->
B_185_732_0 ; } { int_T i1 ; const real_T * u0 = & B_115_2_0 [ 0 ] ; real_T *
y0 = & B_115_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1 ++ ) {
normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] ) , _rtP
-> P_78 , u0 [ i1 ] , _rtP -> P_77 , 2U ) ; } } for ( i = 0 ; i < 50 ; i ++ )
{ rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_715_0 [ 4 ] >= B_115_0_0 [ i ] ) ; }
tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_115_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_115_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_119_7_0 = rtb_B_115_7_0 ; _rtB -> B_185_737_0 = _rtDW ->
UnitDelay4_DSTATE_df ; rtb_B_115_7_0 = _rtP -> P_580 * _rtB -> B_185_737_0 -
_rtDW -> UnitDelay3_DSTATE_ey ; B_185_735_0_idx_1 = rtb_B_119_7_0 ; if (
rtb_B_119_7_0 > _rtP -> P_577 ) { B_185_735_0_idx_1 = _rtP -> P_577 ; } else
{ if ( rtb_B_119_7_0 < _rtP -> P_578 ) { B_185_735_0_idx_1 = _rtP -> P_578 ;
} } _rtB -> B_185_741_0 [ 0 ] = B_185_735_0_idx_1 * rtb_B_115_7_0 ;
rtb_B_119_7_0 = B_185_735_0_idx_1 ; B_185_735_0_idx_1 = _rtB -> B_185_733_0 ;
if ( _rtB -> B_185_733_0 > _rtP -> P_577 ) { B_185_735_0_idx_1 = _rtP ->
P_577 ; } else { if ( _rtB -> B_185_733_0 < _rtP -> P_578 ) {
B_185_735_0_idx_1 = _rtP -> P_578 ; } } _rtB -> B_185_741_0 [ 1 ] =
B_185_735_0_idx_1 * rtb_B_115_7_0 ; rtb_B_115_7_0 = ssGetTaskTime ( S , 2 ) ;
for ( i = 0 ; i < 50 ; i ++ ) { B_118_0_0 [ i ] = rtb_B_115_7_0 + _rtB ->
B_185_743_0 [ i ] ; B_118_0_0 [ i ] = muDoubleScalarRem ( B_118_0_0 [ i ] ,
_rtB -> B_185_744_0 ) ; B_118_2_0 [ i ] = B_118_0_0 [ i ] * _rtB ->
B_185_745_0 ; } { int_T i1 ; const real_T * u0 = & B_118_2_0 [ 0 ] ; real_T *
y0 = & B_118_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1 ++ ) {
normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] ) , _rtP
-> P_84 , u0 [ i1 ] , _rtP -> P_83 , 2U ) ; } } for ( i = 0 ; i < 50 ; i ++ )
{ rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_715_0 [ 1 ] >= B_118_0_0 [ i ] ) ; }
tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_118_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_118_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_115_7_0 = rtb_B_118_7_0 ; _rtB -> B_185_750_0 = _rtDW ->
UnitDelay4_DSTATE_mc ; rtb_B_118_7_0 = _rtP -> P_589 * _rtB -> B_185_750_0 -
_rtDW -> UnitDelay3_DSTATE_oa ; B_185_748_0_idx_1 = rtb_B_115_7_0 ; if (
rtb_B_115_7_0 > _rtP -> P_586 ) { B_185_748_0_idx_1 = _rtP -> P_586 ; } else
{ if ( rtb_B_115_7_0 < _rtP -> P_587 ) { B_185_748_0_idx_1 = _rtP -> P_587 ;
} } _rtB -> B_185_754_0 [ 0 ] = B_185_748_0_idx_1 * rtb_B_118_7_0 ;
rtb_B_115_7_0 = B_185_748_0_idx_1 ; B_185_748_0_idx_1 = _rtB -> B_185_746_0 ;
if ( _rtB -> B_185_746_0 > _rtP -> P_586 ) { B_185_748_0_idx_1 = _rtP ->
P_586 ; } else { if ( _rtB -> B_185_746_0 < _rtP -> P_587 ) {
B_185_748_0_idx_1 = _rtP -> P_587 ; } } _rtB -> B_185_754_0 [ 1 ] =
B_185_748_0_idx_1 * rtb_B_118_7_0 ; rtb_B_118_7_0 = ssGetTaskTime ( S , 2 ) ;
for ( i = 0 ; i < 50 ; i ++ ) { B_116_0_0 [ i ] = rtb_B_118_7_0 + _rtB ->
B_185_756_0 [ i ] ; B_116_0_0 [ i ] = muDoubleScalarRem ( B_116_0_0 [ i ] ,
_rtB -> B_185_757_0 ) ; B_116_2_0 [ i ] = B_116_0_0 [ i ] * _rtB ->
B_185_758_0 ; } { int_T i1 ; const real_T * u0 = & B_116_2_0 [ 0 ] ; real_T *
y0 = & B_116_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1 ++ ) {
normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] ) , _rtP
-> P_80 , u0 [ i1 ] , _rtP -> P_79 , 2U ) ; } } for ( i = 0 ; i < 50 ; i ++ )
{ rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_715_0 [ 3 ] >= B_116_0_0 [ i ] ) ; }
tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_116_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_116_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_118_7_0 = rtb_B_116_7_0 ; _rtB -> B_185_763_0 = _rtDW ->
UnitDelay4_DSTATE_h ; rtb_B_116_7_0 = _rtP -> P_598 * _rtB -> B_185_763_0 -
_rtDW -> UnitDelay3_DSTATE_i ; B_185_761_0_idx_1 = rtb_B_118_7_0 ; if (
rtb_B_118_7_0 > _rtP -> P_595 ) { B_185_761_0_idx_1 = _rtP -> P_595 ; } else
{ if ( rtb_B_118_7_0 < _rtP -> P_596 ) { B_185_761_0_idx_1 = _rtP -> P_596 ;
} } _rtB -> B_185_767_0 [ 0 ] = B_185_761_0_idx_1 * rtb_B_116_7_0 ;
rtb_B_118_7_0 = B_185_761_0_idx_1 ; B_185_761_0_idx_1 = _rtB -> B_185_759_0 ;
if ( _rtB -> B_185_759_0 > _rtP -> P_595 ) { B_185_761_0_idx_1 = _rtP ->
P_595 ; } else { if ( _rtB -> B_185_759_0 < _rtP -> P_596 ) {
B_185_761_0_idx_1 = _rtP -> P_596 ; } } _rtB -> B_185_767_0 [ 1 ] =
B_185_761_0_idx_1 * rtb_B_116_7_0 ; rtb_B_116_7_0 = ssGetTaskTime ( S , 2 ) ;
for ( i = 0 ; i < 50 ; i ++ ) { B_117_0_0 [ i ] = rtb_B_116_7_0 + _rtB ->
B_185_769_0 [ i ] ; B_117_0_0 [ i ] = muDoubleScalarRem ( B_117_0_0 [ i ] ,
_rtB -> B_185_770_0 ) ; B_117_2_0 [ i ] = B_117_0_0 [ i ] * _rtB ->
B_185_771_0 ; } { int_T i1 ; const real_T * u0 = & B_117_2_0 [ 0 ] ; real_T *
y0 = & B_117_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1 ++ ) {
normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] ) , _rtP
-> P_82 , u0 [ i1 ] , _rtP -> P_81 , 2U ) ; } } for ( i = 0 ; i < 50 ; i ++ )
{ rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_715_0 [ 2 ] >= B_117_0_0 [ i ] ) ; }
tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_185_34_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_185_34_0 += ( real_T ) tmpForInput [ isHit ] ; }
rtb_B_116_7_0 = rtb_B_185_34_0 ; _rtB -> B_185_776_0 = _rtDW ->
UnitDelay4_DSTATE_n3 ; rtb_B_185_34_0 = _rtP -> P_607 * _rtB -> B_185_776_0 -
_rtDW -> UnitDelay3_DSTATE_n ; B_185_774_0_idx_1 = rtb_B_116_7_0 ; if (
rtb_B_116_7_0 > _rtP -> P_604 ) { B_185_774_0_idx_1 = _rtP -> P_604 ; } else
{ if ( rtb_B_116_7_0 < _rtP -> P_605 ) { B_185_774_0_idx_1 = _rtP -> P_605 ;
} } _rtB -> B_185_780_0 [ 0 ] = B_185_774_0_idx_1 * rtb_B_185_34_0 ;
rtb_B_116_7_0 = B_185_774_0_idx_1 ; B_185_774_0_idx_1 = _rtB -> B_185_772_0 ;
if ( _rtB -> B_185_772_0 > _rtP -> P_604 ) { B_185_774_0_idx_1 = _rtP ->
P_604 ; } else { if ( _rtB -> B_185_772_0 < _rtP -> P_605 ) {
B_185_774_0_idx_1 = _rtP -> P_605 ; } } _rtB -> B_185_780_0 [ 1 ] =
B_185_774_0_idx_1 * rtb_B_185_34_0 ; rtb_B_185_34_0 = ssGetTaskTime ( S , 2 )
; for ( i = 0 ; i < 50 ; i ++ ) { B_114_0_0 [ i ] = rtb_B_185_34_0 + _rtB ->
B_185_782_0 [ i ] ; B_114_0_0 [ i ] = muDoubleScalarRem ( B_114_0_0 [ i ] ,
_rtB -> B_185_783_0 ) ; B_114_2_0 [ i ] = B_114_0_0 [ i ] * _rtB ->
B_185_784_0 ; } { int_T i1 ; const real_T * u0 = & B_114_2_0 [ 0 ] ; real_T *
y0 = & B_114_0_0 [ 0 ] ; for ( i1 = 0 ; i1 < 50 ; i1 ++ ) {
normal_operation_no_faults_acc_LookUp_real_T_real_T ( & ( y0 [ i1 ] ) , _rtP
-> P_76 , u0 [ i1 ] , _rtP -> P_75 , 2U ) ; } } for ( i = 0 ; i < 50 ; i ++ )
{ rtb_B_51_4_0 [ i ] = ( _rtB -> B_185_715_0 [ 5 ] >= B_114_0_0 [ i ] ) ; }
tmpForInput [ 0 ] = ( int8_T ) rtb_B_51_4_0 [ 0 ] ; tmpForInput [ 1 ] = (
int8_T ) rtb_B_51_4_0 [ 1 ] ; tmpForInput [ 2 ] = ( int8_T ) rtb_B_51_4_0 [ 2
] ; tmpForInput [ 3 ] = ( int8_T ) rtb_B_51_4_0 [ 3 ] ; tmpForInput [ 4 ] = (
int8_T ) rtb_B_51_4_0 [ 4 ] ; tmpForInput [ 5 ] = ( int8_T ) rtb_B_51_4_0 [ 5
] ; tmpForInput [ 6 ] = ( int8_T ) rtb_B_51_4_0 [ 6 ] ; tmpForInput [ 7 ] = (
int8_T ) rtb_B_51_4_0 [ 7 ] ; tmpForInput [ 8 ] = ( int8_T ) rtb_B_51_4_0 [ 8
] ; tmpForInput [ 9 ] = ( int8_T ) rtb_B_51_4_0 [ 9 ] ; tmpForInput [ 10 ] =
( int8_T ) rtb_B_51_4_0 [ 10 ] ; tmpForInput [ 11 ] = ( int8_T ) rtb_B_51_4_0
[ 11 ] ; tmpForInput [ 12 ] = ( int8_T ) rtb_B_51_4_0 [ 12 ] ; tmpForInput [
13 ] = ( int8_T ) rtb_B_51_4_0 [ 13 ] ; tmpForInput [ 14 ] = ( int8_T )
rtb_B_51_4_0 [ 14 ] ; tmpForInput [ 15 ] = ( int8_T ) rtb_B_51_4_0 [ 15 ] ;
tmpForInput [ 16 ] = ( int8_T ) rtb_B_51_4_0 [ 16 ] ; tmpForInput [ 17 ] = (
int8_T ) rtb_B_51_4_0 [ 17 ] ; tmpForInput [ 18 ] = ( int8_T ) rtb_B_51_4_0 [
18 ] ; tmpForInput [ 19 ] = ( int8_T ) rtb_B_51_4_0 [ 19 ] ; tmpForInput [ 20
] = ( int8_T ) rtb_B_51_4_0 [ 20 ] ; tmpForInput [ 21 ] = ( int8_T )
rtb_B_51_4_0 [ 21 ] ; tmpForInput [ 22 ] = ( int8_T ) rtb_B_51_4_0 [ 22 ] ;
tmpForInput [ 23 ] = ( int8_T ) rtb_B_51_4_0 [ 23 ] ; tmpForInput [ 24 ] = (
int8_T ) rtb_B_51_4_0 [ 24 ] ; tmpForInput [ 25 ] = ( int8_T ) rtb_B_51_4_0 [
25 ] ; tmpForInput [ 26 ] = ( int8_T ) rtb_B_51_4_0 [ 26 ] ; tmpForInput [ 27
] = ( int8_T ) rtb_B_51_4_0 [ 27 ] ; tmpForInput [ 28 ] = ( int8_T )
rtb_B_51_4_0 [ 28 ] ; tmpForInput [ 29 ] = ( int8_T ) rtb_B_51_4_0 [ 29 ] ;
tmpForInput [ 30 ] = ( int8_T ) rtb_B_51_4_0 [ 30 ] ; tmpForInput [ 31 ] = (
int8_T ) rtb_B_51_4_0 [ 31 ] ; tmpForInput [ 32 ] = ( int8_T ) rtb_B_51_4_0 [
32 ] ; tmpForInput [ 33 ] = ( int8_T ) rtb_B_51_4_0 [ 33 ] ; tmpForInput [ 34
] = ( int8_T ) rtb_B_51_4_0 [ 34 ] ; tmpForInput [ 35 ] = ( int8_T )
rtb_B_51_4_0 [ 35 ] ; tmpForInput [ 36 ] = ( int8_T ) rtb_B_51_4_0 [ 36 ] ;
tmpForInput [ 37 ] = ( int8_T ) rtb_B_51_4_0 [ 37 ] ; tmpForInput [ 38 ] = (
int8_T ) rtb_B_51_4_0 [ 38 ] ; tmpForInput [ 39 ] = ( int8_T ) rtb_B_51_4_0 [
39 ] ; tmpForInput [ 40 ] = ( int8_T ) rtb_B_51_4_0 [ 40 ] ; tmpForInput [ 41
] = ( int8_T ) rtb_B_51_4_0 [ 41 ] ; tmpForInput [ 42 ] = ( int8_T )
rtb_B_51_4_0 [ 42 ] ; tmpForInput [ 43 ] = ( int8_T ) rtb_B_51_4_0 [ 43 ] ;
tmpForInput [ 44 ] = ( int8_T ) rtb_B_51_4_0 [ 44 ] ; tmpForInput [ 45 ] = (
int8_T ) rtb_B_51_4_0 [ 45 ] ; tmpForInput [ 46 ] = ( int8_T ) rtb_B_51_4_0 [
46 ] ; tmpForInput [ 47 ] = ( int8_T ) rtb_B_51_4_0 [ 47 ] ; tmpForInput [ 48
] = ( int8_T ) rtb_B_51_4_0 [ 48 ] ; tmpForInput [ 49 ] = ( int8_T )
rtb_B_51_4_0 [ 49 ] ; rtb_B_114_7_0 = - 0.0 ; for ( isHit = 0 ; isHit < 50 ;
isHit ++ ) { rtb_B_114_7_0 += ( real_T ) tmpForInput [ isHit ] ; }
B_185_787_0_idx_0 = rtb_B_114_7_0 ; _rtB -> B_185_789_0 = _rtDW ->
UnitDelay4_DSTATE_ix ; rtb_B_114_7_0 = _rtP -> P_616 * _rtB -> B_185_789_0 -
_rtDW -> UnitDelay3_DSTATE_ds ; B_185_787_0_idx_1 = B_185_787_0_idx_0 ; if (
B_185_787_0_idx_0 > _rtP -> P_613 ) { B_185_787_0_idx_1 = _rtP -> P_613 ; }
else { if ( B_185_787_0_idx_0 < _rtP -> P_614 ) { B_185_787_0_idx_1 = _rtP ->
P_614 ; } } _rtB -> B_185_793_0 [ 0 ] = B_185_787_0_idx_1 * rtb_B_114_7_0 ;
B_185_787_0_idx_0 = B_185_787_0_idx_1 ; B_185_787_0_idx_1 = _rtB ->
B_185_785_0 ; if ( _rtB -> B_185_785_0 > _rtP -> P_613 ) { B_185_787_0_idx_1
= _rtP -> P_613 ; } else { if ( _rtB -> B_185_785_0 < _rtP -> P_614 ) {
B_185_787_0_idx_1 = _rtP -> P_614 ; } } _rtB -> B_185_793_0 [ 1 ] =
B_185_787_0_idx_1 * rtb_B_114_7_0 ; _rtB -> B_185_794_0 = _rtP -> P_618 ;
normal_operation_no_faults_Signalgenerator ( S , _rtB -> B_185_794_0 , & _rtB
-> Signalgenerator , & _rtDW -> Signalgenerator , & _rtP -> Signalgenerator )
; rtb_B_114_7_0 = _rtB -> Signalgenerator . B_127_39_0 + _rtB -> B_185_796_0
; if ( _rtB -> Signalgenerator . B_127_36_0 >= _rtP -> P_620 ) {
rtb_B_185_34_0 = _rtB -> Signalgenerator . B_127_1_0 ; } else {
rtb_B_185_34_0 = rtb_B_114_7_0 ; } rtb_B_185_2_0 [ 0 ] = rtb_B_185_34_0 ; if
( _rtB -> B_185_799_0 >= _rtP -> P_622 ) { rtb_B_185_2_0 [ 1 ] =
rtb_B_114_7_0 ; rtb_B_185_2_0 [ 2 ] = rtb_B_114_7_0 ; } else { rtb_B_185_2_0
[ 1 ] = rtb_B_185_34_0 ; rtb_B_185_2_0 [ 2 ] = rtb_B_185_34_0 ; } B_185_805_0
= ssGetTaskTime ( S , 2 ) * _rtB -> B_185_803_0 + _rtB -> Signalgenerator .
B_127_37_0 ; rtb_B_185_124_0 [ 0 ] = muDoubleScalarSin ( ( ( _rtB ->
B_185_807_0 + _rtB -> B_185_808_0 [ 0 ] ) + _rtB -> Signalgenerator .
B_127_41_0 ) + B_185_805_0 ) ; rtb_B_185_124_0 [ 1 ] = muDoubleScalarSin ( (
( _rtB -> B_185_807_0 + _rtB -> B_185_808_0 [ 1 ] ) + _rtB -> Signalgenerator
. B_127_41_0 ) + B_185_805_0 ) ; rtb_B_185_124_0 [ 2 ] = muDoubleScalarSin (
( ( _rtB -> B_185_807_0 + _rtB -> B_185_808_0 [ 2 ] ) + _rtB ->
Signalgenerator . B_127_41_0 ) + B_185_805_0 ) ; B_185_813_0 = _rtP -> P_628
; normal_operation_no_faults_HarmonicGenerator ( S , B_185_813_0 ,
B_185_805_0 , & _rtB -> HarmonicGenerator , & _rtDW -> HarmonicGenerator , &
_rtP -> HarmonicGenerator ) ; _rtB -> B_185_815_0 [ 0 ] = ( rtb_B_185_2_0 [ 0
] * rtb_B_185_124_0 [ 0 ] + _rtB -> HarmonicGenerator . B_122_16_0 [ 0 ] ) +
_rtB -> HarmonicGenerator . B_122_30_0 [ 0 ] ; _rtB -> B_185_815_0 [ 1 ] = (
rtb_B_185_2_0 [ 1 ] * rtb_B_185_124_0 [ 1 ] + _rtB -> HarmonicGenerator .
B_122_16_0 [ 1 ] ) + _rtB -> HarmonicGenerator . B_122_30_0 [ 1 ] ; _rtB ->
B_185_815_0 [ 2 ] = ( rtb_B_185_2_0 [ 2 ] * rtb_B_185_124_0 [ 2 ] + _rtB ->
HarmonicGenerator . B_122_16_0 [ 2 ] ) + _rtB -> HarmonicGenerator .
B_122_30_0 [ 2 ] ; _rtB -> B_185_816_0 = _rtP -> P_629 ;
normal_operation_no_faults_Signalgenerator ( S , _rtB -> B_185_816_0 , & _rtB
-> Signalgenerator_d , & _rtDW -> Signalgenerator_d , & _rtP ->
Signalgenerator_d ) ; rtb_B_114_7_0 = _rtB -> Signalgenerator_d . B_127_39_0
+ _rtB -> B_185_818_0 ; if ( _rtB -> Signalgenerator_d . B_127_36_0 >= _rtP
-> P_631 ) { rtb_B_185_34_0 = _rtB -> Signalgenerator_d . B_127_1_0 ; } else
{ rtb_B_185_34_0 = rtb_B_114_7_0 ; } rtb_B_185_2_0 [ 0 ] = rtb_B_185_34_0 ;
if ( _rtB -> B_185_821_0 >= _rtP -> P_633 ) { rtb_B_185_2_0 [ 1 ] =
rtb_B_114_7_0 ; rtb_B_185_2_0 [ 2 ] = rtb_B_114_7_0 ; } else { rtb_B_185_2_0
[ 1 ] = rtb_B_185_34_0 ; rtb_B_185_2_0 [ 2 ] = rtb_B_185_34_0 ; } B_185_827_0
= ssGetTaskTime ( S , 2 ) * _rtB -> B_185_825_0 + _rtB -> Signalgenerator_d .
B_127_37_0 ; rtb_B_185_124_0 [ 0 ] = muDoubleScalarSin ( ( ( _rtB ->
B_185_829_0 + _rtB -> B_185_830_0 [ 0 ] ) + _rtB -> Signalgenerator_d .
B_127_41_0 ) + B_185_827_0 ) ; rtb_B_185_124_0 [ 1 ] = muDoubleScalarSin ( (
( _rtB -> B_185_829_0 + _rtB -> B_185_830_0 [ 1 ] ) + _rtB ->
Signalgenerator_d . B_127_41_0 ) + B_185_827_0 ) ; rtb_B_185_124_0 [ 2 ] =
muDoubleScalarSin ( ( ( _rtB -> B_185_829_0 + _rtB -> B_185_830_0 [ 2 ] ) +
_rtB -> Signalgenerator_d . B_127_41_0 ) + B_185_827_0 ) ; B_185_835_0 = _rtP
-> P_639 ; normal_operation_no_faults_HarmonicGenerator ( S , B_185_835_0 ,
B_185_827_0 , & _rtB -> HarmonicGenerator_d , & _rtDW -> HarmonicGenerator_d
, & _rtP -> HarmonicGenerator_d ) ; _rtB -> B_185_837_0 [ 0 ] = (
rtb_B_185_2_0 [ 0 ] * rtb_B_185_124_0 [ 0 ] + _rtB -> HarmonicGenerator_d .
B_122_16_0 [ 0 ] ) + _rtB -> HarmonicGenerator_d . B_122_30_0 [ 0 ] ; _rtB ->
B_185_837_0 [ 1 ] = ( rtb_B_185_2_0 [ 1 ] * rtb_B_185_124_0 [ 1 ] + _rtB ->
HarmonicGenerator_d . B_122_16_0 [ 1 ] ) + _rtB -> HarmonicGenerator_d .
B_122_30_0 [ 1 ] ; _rtB -> B_185_837_0 [ 2 ] = ( rtb_B_185_2_0 [ 2 ] *
rtb_B_185_124_0 [ 2 ] + _rtB -> HarmonicGenerator_d . B_122_16_0 [ 2 ] ) +
_rtB -> HarmonicGenerator_d . B_122_30_0 [ 2 ] ; _rtB -> B_185_838_0 = _rtP
-> P_640 ; normal_operation_no_faults_Signalgenerator ( S , _rtB ->
B_185_838_0 , & _rtB -> Signalgenerator_p , & _rtDW -> Signalgenerator_p , &
_rtP -> Signalgenerator_p ) ; rtb_B_114_7_0 = _rtB -> Signalgenerator_p .
B_127_39_0 + _rtB -> B_185_840_0 ; if ( _rtB -> Signalgenerator_p .
B_127_36_0 >= _rtP -> P_642 ) { rtb_B_185_34_0 = _rtB -> Signalgenerator_p .
B_127_1_0 ; } else { rtb_B_185_34_0 = rtb_B_114_7_0 ; } rtb_B_185_2_0 [ 0 ] =
rtb_B_185_34_0 ; if ( _rtB -> B_185_843_0 >= _rtP -> P_644 ) { rtb_B_185_2_0
[ 1 ] = rtb_B_114_7_0 ; rtb_B_185_2_0 [ 2 ] = rtb_B_114_7_0 ; } else {
rtb_B_185_2_0 [ 1 ] = rtb_B_185_34_0 ; rtb_B_185_2_0 [ 2 ] = rtb_B_185_34_0 ;
} B_185_849_0 = ssGetTaskTime ( S , 2 ) * _rtB -> B_185_847_0 + _rtB ->
Signalgenerator_p . B_127_37_0 ; rtb_B_185_124_0 [ 0 ] = muDoubleScalarSin (
( ( _rtB -> B_185_851_0 + _rtB -> B_185_852_0 [ 0 ] ) + _rtB ->
Signalgenerator_p . B_127_41_0 ) + B_185_849_0 ) ; rtb_B_185_124_0 [ 1 ] =
muDoubleScalarSin ( ( ( _rtB -> B_185_851_0 + _rtB -> B_185_852_0 [ 1 ] ) +
_rtB -> Signalgenerator_p . B_127_41_0 ) + B_185_849_0 ) ; rtb_B_185_124_0 [
2 ] = muDoubleScalarSin ( ( ( _rtB -> B_185_851_0 + _rtB -> B_185_852_0 [ 2 ]
) + _rtB -> Signalgenerator_p . B_127_41_0 ) + B_185_849_0 ) ; B_185_857_0 =
_rtP -> P_650 ; normal_operation_no_faults_HarmonicGenerator ( S ,
B_185_857_0 , B_185_849_0 , & _rtB -> HarmonicGenerator_c , & _rtDW ->
HarmonicGenerator_c , & _rtP -> HarmonicGenerator_c ) ; _rtB -> B_185_859_0 [
0 ] = ( rtb_B_185_2_0 [ 0 ] * rtb_B_185_124_0 [ 0 ] + _rtB ->
HarmonicGenerator_c . B_122_16_0 [ 0 ] ) + _rtB -> HarmonicGenerator_c .
B_122_30_0 [ 0 ] ; _rtB -> B_185_859_0 [ 1 ] = ( rtb_B_185_2_0 [ 1 ] *
rtb_B_185_124_0 [ 1 ] + _rtB -> HarmonicGenerator_c . B_122_16_0 [ 1 ] ) +
_rtB -> HarmonicGenerator_c . B_122_30_0 [ 1 ] ; _rtB -> B_185_859_0 [ 2 ] =
( rtb_B_185_2_0 [ 2 ] * rtb_B_185_124_0 [ 2 ] + _rtB -> HarmonicGenerator_c .
B_122_16_0 [ 2 ] ) + _rtB -> HarmonicGenerator_c . B_122_30_0 [ 2 ] ; _rtB ->
B_185_860_0 = _rtP -> P_651 ; normal_operation_no_faults_Signalgenerator ( S
, _rtB -> B_185_860_0 , & _rtB -> Signalgenerator_e , & _rtDW ->
Signalgenerator_e , & _rtP -> Signalgenerator_e ) ; rtb_B_114_7_0 = _rtB ->
Signalgenerator_e . B_127_39_0 + _rtB -> B_185_862_0 ; if ( _rtB ->
Signalgenerator_e . B_127_36_0 >= _rtP -> P_653 ) { rtb_B_185_34_0 = _rtB ->
Signalgenerator_e . B_127_1_0 ; } else { rtb_B_185_34_0 = rtb_B_114_7_0 ; }
rtb_B_185_2_0 [ 0 ] = rtb_B_185_34_0 ; if ( _rtB -> B_185_865_0 >= _rtP ->
P_655 ) { rtb_B_185_2_0 [ 1 ] = rtb_B_114_7_0 ; rtb_B_185_2_0 [ 2 ] =
rtb_B_114_7_0 ; } else { rtb_B_185_2_0 [ 1 ] = rtb_B_185_34_0 ; rtb_B_185_2_0
[ 2 ] = rtb_B_185_34_0 ; } B_185_871_0 = ssGetTaskTime ( S , 2 ) * _rtB ->
B_185_869_0 + _rtB -> Signalgenerator_e . B_127_37_0 ; rtb_B_185_124_0 [ 0 ]
= muDoubleScalarSin ( ( ( _rtB -> B_185_873_0 + _rtB -> B_185_874_0 [ 0 ] ) +
_rtB -> Signalgenerator_e . B_127_41_0 ) + B_185_871_0 ) ; rtb_B_185_124_0 [
1 ] = muDoubleScalarSin ( ( ( _rtB -> B_185_873_0 + _rtB -> B_185_874_0 [ 1 ]
) + _rtB -> Signalgenerator_e . B_127_41_0 ) + B_185_871_0 ) ;
rtb_B_185_124_0 [ 2 ] = muDoubleScalarSin ( ( ( _rtB -> B_185_873_0 + _rtB ->
B_185_874_0 [ 2 ] ) + _rtB -> Signalgenerator_e . B_127_41_0 ) + B_185_871_0
) ; B_185_879_0 = _rtP -> P_661 ;
normal_operation_no_faults_HarmonicGenerator ( S , B_185_879_0 , B_185_871_0
, & _rtB -> HarmonicGenerator_b , & _rtDW -> HarmonicGenerator_b , & _rtP ->
HarmonicGenerator_b ) ; _rtB -> B_185_881_0 [ 0 ] = ( rtb_B_185_2_0 [ 0 ] *
rtb_B_185_124_0 [ 0 ] + _rtB -> HarmonicGenerator_b . B_122_16_0 [ 0 ] ) +
_rtB -> HarmonicGenerator_b . B_122_30_0 [ 0 ] ; _rtB -> B_185_881_0 [ 1 ] =
( rtb_B_185_2_0 [ 1 ] * rtb_B_185_124_0 [ 1 ] + _rtB -> HarmonicGenerator_b .
B_122_16_0 [ 1 ] ) + _rtB -> HarmonicGenerator_b . B_122_30_0 [ 1 ] ; _rtB ->
B_185_881_0 [ 2 ] = ( rtb_B_185_2_0 [ 2 ] * rtb_B_185_124_0 [ 2 ] + _rtB ->
HarmonicGenerator_b . B_122_16_0 [ 2 ] ) + _rtB -> HarmonicGenerator_b .
B_122_30_0 [ 2 ] ; _rtB -> B_185_882_0 = _rtP -> P_662 ;
normal_operation_no_faults_Signalgenerator ( S , _rtB -> B_185_882_0 , & _rtB
-> Signalgenerator_m , & _rtDW -> Signalgenerator_m , & _rtP ->
Signalgenerator_m ) ; rtb_B_114_7_0 = _rtB -> Signalgenerator_m . B_127_39_0
+ _rtB -> B_185_884_0 ; if ( _rtB -> Signalgenerator_m . B_127_36_0 >= _rtP
-> P_664 ) { rtb_B_185_34_0 = _rtB -> Signalgenerator_m . B_127_1_0 ; } else
{ rtb_B_185_34_0 = rtb_B_114_7_0 ; } rtb_B_185_2_0 [ 0 ] = rtb_B_185_34_0 ;
if ( _rtB -> B_185_887_0 >= _rtP -> P_666 ) { rtb_B_185_2_0 [ 1 ] =
rtb_B_114_7_0 ; rtb_B_185_2_0 [ 2 ] = rtb_B_114_7_0 ; } else { rtb_B_185_2_0
[ 1 ] = rtb_B_185_34_0 ; rtb_B_185_2_0 [ 2 ] = rtb_B_185_34_0 ; } B_185_893_0
= ssGetTaskTime ( S , 2 ) * _rtB -> B_185_891_0 + _rtB -> Signalgenerator_m .
B_127_37_0 ; rtb_B_185_124_0 [ 0 ] = muDoubleScalarSin ( ( ( _rtB ->
B_185_895_0 + _rtB -> B_185_896_0 [ 0 ] ) + _rtB -> Signalgenerator_m .
B_127_41_0 ) + B_185_893_0 ) ; rtb_B_185_124_0 [ 1 ] = muDoubleScalarSin ( (
( _rtB -> B_185_895_0 + _rtB -> B_185_896_0 [ 1 ] ) + _rtB ->
Signalgenerator_m . B_127_41_0 ) + B_185_893_0 ) ; rtb_B_185_124_0 [ 2 ] =
muDoubleScalarSin ( ( ( _rtB -> B_185_895_0 + _rtB -> B_185_896_0 [ 2 ] ) +
_rtB -> Signalgenerator_m . B_127_41_0 ) + B_185_893_0 ) ; B_185_901_0 = _rtP
-> P_672 ; normal_operation_no_faults_HarmonicGenerator ( S , B_185_901_0 ,
B_185_893_0 , & _rtB -> HarmonicGenerator_a , & _rtDW -> HarmonicGenerator_a
, & _rtP -> HarmonicGenerator_a ) ; _rtB -> B_185_903_0 [ 0 ] = (
rtb_B_185_2_0 [ 0 ] * rtb_B_185_124_0 [ 0 ] + _rtB -> HarmonicGenerator_a .
B_122_16_0 [ 0 ] ) + _rtB -> HarmonicGenerator_a . B_122_30_0 [ 0 ] ; _rtB ->
B_185_903_0 [ 1 ] = ( rtb_B_185_2_0 [ 1 ] * rtb_B_185_124_0 [ 1 ] + _rtB ->
HarmonicGenerator_a . B_122_16_0 [ 1 ] ) + _rtB -> HarmonicGenerator_a .
B_122_30_0 [ 1 ] ; _rtB -> B_185_903_0 [ 2 ] = ( rtb_B_185_2_0 [ 2 ] *
rtb_B_185_124_0 [ 2 ] + _rtB -> HarmonicGenerator_a . B_122_16_0 [ 2 ] ) +
_rtB -> HarmonicGenerator_a . B_122_30_0 [ 2 ] ; _rtB -> B_185_904_0 = _rtP
-> P_673 ; normal_operation_no_faults_Signalgenerator ( S , _rtB ->
B_185_904_0 , & _rtB -> Signalgenerator_j , & _rtDW -> Signalgenerator_j , &
_rtP -> Signalgenerator_j ) ; rtb_B_114_7_0 = _rtB -> Signalgenerator_j .
B_127_39_0 + _rtB -> B_185_906_0 ; if ( _rtB -> Signalgenerator_j .
B_127_36_0 >= _rtP -> P_675 ) { rtb_B_185_34_0 = _rtB -> Signalgenerator_j .
B_127_1_0 ; } else { rtb_B_185_34_0 = rtb_B_114_7_0 ; } rtb_B_185_2_0 [ 0 ] =
rtb_B_185_34_0 ; if ( _rtB -> B_185_909_0 >= _rtP -> P_677 ) { rtb_B_185_2_0
[ 1 ] = rtb_B_114_7_0 ; rtb_B_185_2_0 [ 2 ] = rtb_B_114_7_0 ; } else {
rtb_B_185_2_0 [ 1 ] = rtb_B_185_34_0 ; rtb_B_185_2_0 [ 2 ] = rtb_B_185_34_0 ;
} B_185_915_0 = ssGetTaskTime ( S , 2 ) * _rtB -> B_185_913_0 + _rtB ->
Signalgenerator_j . B_127_37_0 ; rtb_B_185_124_0 [ 0 ] = muDoubleScalarSin (
( ( _rtB -> B_185_917_0 + _rtB -> B_185_918_0 [ 0 ] ) + _rtB ->
Signalgenerator_j . B_127_41_0 ) + B_185_915_0 ) ; rtb_B_185_124_0 [ 1 ] =
muDoubleScalarSin ( ( ( _rtB -> B_185_917_0 + _rtB -> B_185_918_0 [ 1 ] ) +
_rtB -> Signalgenerator_j . B_127_41_0 ) + B_185_915_0 ) ; rtb_B_185_124_0 [
2 ] = muDoubleScalarSin ( ( ( _rtB -> B_185_917_0 + _rtB -> B_185_918_0 [ 2 ]
) + _rtB -> Signalgenerator_j . B_127_41_0 ) + B_185_915_0 ) ; B_185_923_0 =
_rtP -> P_683 ; normal_operation_no_faults_HarmonicGenerator ( S ,
B_185_923_0 , B_185_915_0 , & _rtB -> HarmonicGenerator_ca , & _rtDW ->
HarmonicGenerator_ca , & _rtP -> HarmonicGenerator_ca ) ; _rtB -> B_185_925_0
[ 0 ] = ( rtb_B_185_2_0 [ 0 ] * rtb_B_185_124_0 [ 0 ] + _rtB ->
HarmonicGenerator_ca . B_122_16_0 [ 0 ] ) + _rtB -> HarmonicGenerator_ca .
B_122_30_0 [ 0 ] ; _rtB -> B_185_925_0 [ 1 ] = ( rtb_B_185_2_0 [ 1 ] *
rtb_B_185_124_0 [ 1 ] + _rtB -> HarmonicGenerator_ca . B_122_16_0 [ 1 ] ) +
_rtB -> HarmonicGenerator_ca . B_122_30_0 [ 1 ] ; _rtB -> B_185_925_0 [ 2 ] =
( rtb_B_185_2_0 [ 2 ] * rtb_B_185_124_0 [ 2 ] + _rtB -> HarmonicGenerator_ca
. B_122_16_0 [ 2 ] ) + _rtB -> HarmonicGenerator_ca . B_122_30_0 [ 2 ] ;
ssCallAccelRunBlock ( S , 185 , 926 , SS_CALL_MDL_OUTPUTS ) ; if ( _rtB ->
B_185_929_0 >= _rtP -> P_699 ) { rtb_B_114_7_0 = _rtB -> B_185_928_0 ; } else
{ rtb_B_114_7_0 = rt_Lookup ( _rtP -> P_697 , 4 , ssGetTaskTime ( S , 2 ) ,
_rtP -> P_698 ) ; } if ( _rtB -> B_185_933_0 >= _rtP -> P_702 ) { _rtB ->
B_185_935_0 = rtb_B_114_7_0 ; } else { _rtB -> B_185_935_0 = _rtB ->
B_185_934_0 ; } } _rtB -> B_185_937_0 = rt_Lookup ( _rtP -> P_703 , 5 ,
ssGetT ( S ) , _rtP -> P_704 ) ; if ( _rtB -> B_185_927_0 >= _rtP -> P_705 )
{ _rtB -> B_185_938_0 = _rtB -> B_185_935_0 ; } else { _rtB -> B_185_938_0 =
_rtB -> B_185_937_0 ; } if ( ( _rtDW -> TimeStampA >= ssGetT ( S ) ) && (
_rtDW -> TimeStampB >= ssGetT ( S ) ) ) { _rtB -> B_185_939_0 = 0.0 ; } else
{ rtb_B_185_34_0 = _rtDW -> TimeStampA ; lastU = & _rtDW -> LastUAtTimeA ; if
( _rtDW -> TimeStampA < _rtDW -> TimeStampB ) { if ( _rtDW -> TimeStampB <
ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB ; lastU = & _rtDW ->
LastUAtTimeB ; } } else { if ( _rtDW -> TimeStampA >= ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB ; lastU = & _rtDW -> LastUAtTimeB ; } }
_rtB -> B_185_939_0 = ( _rtB -> B_185_937_0 - * lastU ) / ( ssGetT ( S ) -
rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 )
{ if ( _rtB -> B_185_945_0 >= _rtP -> P_709 ) { _rtB -> B_185_946_0 =
rtb_B_114_7_0 ; } else { _rtB -> B_185_946_0 = _rtB -> B_185_934_0 ; } } _rtB
-> B_185_948_0 = rt_Lookup ( _rtP -> P_710 , 5 , ssGetT ( S ) , _rtP -> P_711
) ; if ( _rtB -> B_185_944_0 >= _rtP -> P_712 ) { _rtB -> B_185_949_0 = _rtB
-> B_185_946_0 ; } else { _rtB -> B_185_949_0 = _rtB -> B_185_948_0 ; } if (
( _rtDW -> TimeStampA_f >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB_a >=
ssGetT ( S ) ) ) { _rtB -> B_185_950_0 = 0.0 ; } else { rtb_B_185_34_0 =
_rtDW -> TimeStampA_f ; lastU = & _rtDW -> LastUAtTimeA_o ; if ( _rtDW ->
TimeStampA_f < _rtDW -> TimeStampB_a ) { if ( _rtDW -> TimeStampB_a < ssGetT
( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_a ; lastU = & _rtDW ->
LastUAtTimeB_o ; } } else { if ( _rtDW -> TimeStampA_f >= ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_a ; lastU = & _rtDW -> LastUAtTimeB_o ;
} } _rtB -> B_185_950_0 = ( _rtB -> B_185_948_0 - * lastU ) / ( ssGetT ( S )
- rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0
) { if ( _rtB -> B_185_956_0 >= _rtP -> P_716 ) { _rtB -> B_185_957_0 =
rtb_B_114_7_0 ; } else { _rtB -> B_185_957_0 = _rtB -> B_185_934_0 ; } } _rtB
-> B_185_959_0 = rt_Lookup ( _rtP -> P_717 , 5 , ssGetT ( S ) , _rtP -> P_718
) ; if ( _rtB -> B_185_955_0 >= _rtP -> P_719 ) { _rtB -> B_185_960_0 = _rtB
-> B_185_957_0 ; } else { _rtB -> B_185_960_0 = _rtB -> B_185_959_0 ; } if (
( _rtDW -> TimeStampA_o >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB_m >=
ssGetT ( S ) ) ) { _rtB -> B_185_961_0 = 0.0 ; } else { rtb_B_185_34_0 =
_rtDW -> TimeStampA_o ; lastU = & _rtDW -> LastUAtTimeA_g ; if ( _rtDW ->
TimeStampA_o < _rtDW -> TimeStampB_m ) { if ( _rtDW -> TimeStampB_m < ssGetT
( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_m ; lastU = & _rtDW ->
LastUAtTimeB_p ; } } else { if ( _rtDW -> TimeStampA_o >= ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_m ; lastU = & _rtDW -> LastUAtTimeB_p ;
} } _rtB -> B_185_961_0 = ( _rtB -> B_185_959_0 - * lastU ) / ( ssGetT ( S )
- rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0
) { if ( _rtB -> B_185_975_0 >= _rtP -> P_726 ) { rtb_B_185_978_0 = _rtB ->
B_185_974_0 ; } else { rtb_B_185_978_0 = rt_Lookup ( _rtP -> P_724 , 4 ,
ssGetTaskTime ( S , 2 ) , _rtP -> P_725 ) ; } if ( _rtB -> B_185_979_0 >=
_rtP -> P_729 ) { _rtB -> B_185_981_0 = rtb_B_185_978_0 ; } else { _rtB ->
B_185_981_0 = _rtB -> B_185_980_0 ; } } _rtB -> B_185_983_0 = rt_Lookup (
_rtP -> P_730 , 5 , ssGetT ( S ) , _rtP -> P_731 ) ; if ( _rtB -> B_185_973_0
>= _rtP -> P_732 ) { _rtB -> B_185_984_0 = _rtB -> B_185_981_0 ; } else {
_rtB -> B_185_984_0 = _rtB -> B_185_983_0 ; } if ( ( _rtDW -> TimeStampA_n >=
ssGetT ( S ) ) && ( _rtDW -> TimeStampB_o >= ssGetT ( S ) ) ) { _rtB ->
B_185_985_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW -> TimeStampA_n ; lastU =
& _rtDW -> LastUAtTimeA_d ; if ( _rtDW -> TimeStampA_n < _rtDW ->
TimeStampB_o ) { if ( _rtDW -> TimeStampB_o < ssGetT ( S ) ) { rtb_B_185_34_0
= _rtDW -> TimeStampB_o ; lastU = & _rtDW -> LastUAtTimeB_k ; } } else { if (
_rtDW -> TimeStampA_n >= ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW ->
TimeStampB_o ; lastU = & _rtDW -> LastUAtTimeB_k ; } } _rtB -> B_185_985_0 =
( _rtB -> B_185_983_0 - * lastU ) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; }
isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { if ( _rtB ->
B_185_991_0 >= _rtP -> P_736 ) { _rtB -> B_185_992_0 = rtb_B_185_978_0 ; }
else { _rtB -> B_185_992_0 = _rtB -> B_185_980_0 ; } } _rtB -> B_185_994_0 =
rt_Lookup ( _rtP -> P_737 , 5 , ssGetT ( S ) , _rtP -> P_738 ) ; if ( _rtB ->
B_185_990_0 >= _rtP -> P_739 ) { _rtB -> B_185_995_0 = _rtB -> B_185_992_0 ;
} else { _rtB -> B_185_995_0 = _rtB -> B_185_994_0 ; } if ( ( _rtDW ->
TimeStampA_g >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB_k >= ssGetT ( S ) ) )
{ _rtB -> B_185_996_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW -> TimeStampA_g
; lastU = & _rtDW -> LastUAtTimeA_a ; if ( _rtDW -> TimeStampA_g < _rtDW ->
TimeStampB_k ) { if ( _rtDW -> TimeStampB_k < ssGetT ( S ) ) { rtb_B_185_34_0
= _rtDW -> TimeStampB_k ; lastU = & _rtDW -> LastUAtTimeB_g ; } } else { if (
_rtDW -> TimeStampA_g >= ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW ->
TimeStampB_k ; lastU = & _rtDW -> LastUAtTimeB_g ; } } _rtB -> B_185_996_0 =
( _rtB -> B_185_994_0 - * lastU ) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; }
isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { if ( _rtB ->
B_185_1002_0 >= _rtP -> P_743 ) { _rtB -> B_185_1003_0 = rtb_B_185_978_0 ; }
else { _rtB -> B_185_1003_0 = _rtB -> B_185_980_0 ; } } _rtB -> B_185_1005_0
= rt_Lookup ( _rtP -> P_744 , 5 , ssGetT ( S ) , _rtP -> P_745 ) ; if ( _rtB
-> B_185_1001_0 >= _rtP -> P_746 ) { _rtB -> B_185_1006_0 = _rtB ->
B_185_1003_0 ; } else { _rtB -> B_185_1006_0 = _rtB -> B_185_1005_0 ; } if (
( _rtDW -> TimeStampA_j >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB_f >=
ssGetT ( S ) ) ) { _rtB -> B_185_1007_0 = 0.0 ; } else { rtb_B_185_34_0 =
_rtDW -> TimeStampA_j ; lastU = & _rtDW -> LastUAtTimeA_k ; if ( _rtDW ->
TimeStampA_j < _rtDW -> TimeStampB_f ) { if ( _rtDW -> TimeStampB_f < ssGetT
( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_f ; lastU = & _rtDW ->
LastUAtTimeB_i ; } } else { if ( _rtDW -> TimeStampA_j >= ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_f ; lastU = & _rtDW -> LastUAtTimeB_i ;
} } _rtB -> B_185_1007_0 = ( _rtB -> B_185_1005_0 - * lastU ) / ( ssGetT ( S
) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit !=
0 ) { if ( _rtB -> B_185_1021_0 >= _rtP -> P_753 ) { rtb_B_185_1024_0 = _rtB
-> B_185_1020_0 ; } else { rtb_B_185_1024_0 = rt_Lookup ( _rtP -> P_751 , 4 ,
ssGetTaskTime ( S , 2 ) , _rtP -> P_752 ) ; } if ( _rtB -> B_185_1025_0 >=
_rtP -> P_756 ) { _rtB -> B_185_1027_0 = rtb_B_185_1024_0 ; } else { _rtB ->
B_185_1027_0 = _rtB -> B_185_1026_0 ; } } _rtB -> B_185_1029_0 = rt_Lookup (
_rtP -> P_757 , 5 , ssGetT ( S ) , _rtP -> P_758 ) ; if ( _rtB ->
B_185_1019_0 >= _rtP -> P_759 ) { _rtB -> B_185_1030_0 = _rtB -> B_185_1027_0
; } else { _rtB -> B_185_1030_0 = _rtB -> B_185_1029_0 ; } if ( ( _rtDW ->
TimeStampA_i >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB_c >= ssGetT ( S ) ) )
{ _rtB -> B_185_1031_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW ->
TimeStampA_i ; lastU = & _rtDW -> LastUAtTimeA_a1 ; if ( _rtDW ->
TimeStampA_i < _rtDW -> TimeStampB_c ) { if ( _rtDW -> TimeStampB_c < ssGetT
( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_c ; lastU = & _rtDW ->
LastUAtTimeB_e ; } } else { if ( _rtDW -> TimeStampA_i >= ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_c ; lastU = & _rtDW -> LastUAtTimeB_e ;
} } _rtB -> B_185_1031_0 = ( _rtB -> B_185_1029_0 - * lastU ) / ( ssGetT ( S
) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit !=
0 ) { if ( _rtB -> B_185_1037_0 >= _rtP -> P_763 ) { _rtB -> B_185_1038_0 =
rtb_B_185_1024_0 ; } else { _rtB -> B_185_1038_0 = _rtB -> B_185_1026_0 ; } }
_rtB -> B_185_1040_0 = rt_Lookup ( _rtP -> P_764 , 5 , ssGetT ( S ) , _rtP ->
P_765 ) ; if ( _rtB -> B_185_1036_0 >= _rtP -> P_766 ) { _rtB -> B_185_1041_0
= _rtB -> B_185_1038_0 ; } else { _rtB -> B_185_1041_0 = _rtB -> B_185_1040_0
; } if ( ( _rtDW -> TimeStampA_p >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB_l
>= ssGetT ( S ) ) ) { _rtB -> B_185_1042_0 = 0.0 ; } else { rtb_B_185_34_0 =
_rtDW -> TimeStampA_p ; lastU = & _rtDW -> LastUAtTimeA_n ; if ( _rtDW ->
TimeStampA_p < _rtDW -> TimeStampB_l ) { if ( _rtDW -> TimeStampB_l < ssGetT
( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_l ; lastU = & _rtDW ->
LastUAtTimeB_h ; } } else { if ( _rtDW -> TimeStampA_p >= ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_l ; lastU = & _rtDW -> LastUAtTimeB_h ;
} } _rtB -> B_185_1042_0 = ( _rtB -> B_185_1040_0 - * lastU ) / ( ssGetT ( S
) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit !=
0 ) { if ( _rtB -> B_185_1048_0 >= _rtP -> P_770 ) { _rtB -> B_185_1049_0 =
rtb_B_185_1024_0 ; } else { _rtB -> B_185_1049_0 = _rtB -> B_185_1026_0 ; } }
_rtB -> B_185_1051_0 = rt_Lookup ( _rtP -> P_771 , 5 , ssGetT ( S ) , _rtP ->
P_772 ) ; if ( _rtB -> B_185_1047_0 >= _rtP -> P_773 ) { _rtB -> B_185_1052_0
= _rtB -> B_185_1049_0 ; } else { _rtB -> B_185_1052_0 = _rtB -> B_185_1051_0
; } if ( ( _rtDW -> TimeStampA_gh >= ssGetT ( S ) ) && ( _rtDW ->
TimeStampB_l0 >= ssGetT ( S ) ) ) { _rtB -> B_185_1053_0 = 0.0 ; } else {
rtb_B_185_34_0 = _rtDW -> TimeStampA_gh ; lastU = & _rtDW -> LastUAtTimeA_n1
; if ( _rtDW -> TimeStampA_gh < _rtDW -> TimeStampB_l0 ) { if ( _rtDW ->
TimeStampB_l0 < ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_l0 ;
lastU = & _rtDW -> LastUAtTimeB_g2 ; } } else { if ( _rtDW -> TimeStampA_gh
>= ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_l0 ; lastU = & _rtDW
-> LastUAtTimeB_g2 ; } } _rtB -> B_185_1053_0 = ( _rtB -> B_185_1051_0 - *
lastU ) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2
, 0 ) ; if ( isHit != 0 ) { if ( _rtB -> B_185_1067_0 >= _rtP -> P_780 ) {
rtb_B_185_1070_0 = _rtB -> B_185_1066_0 ; } else { rtb_B_185_1070_0 =
rt_Lookup ( _rtP -> P_778 , 4 , ssGetTaskTime ( S , 2 ) , _rtP -> P_779 ) ; }
if ( _rtB -> B_185_1071_0 >= _rtP -> P_783 ) { _rtB -> B_185_1073_0 =
rtb_B_185_1070_0 ; } else { _rtB -> B_185_1073_0 = _rtB -> B_185_1072_0 ; } }
_rtB -> B_185_1075_0 = rt_Lookup ( _rtP -> P_784 , 5 , ssGetT ( S ) , _rtP ->
P_785 ) ; if ( _rtB -> B_185_1065_0 >= _rtP -> P_786 ) { _rtB -> B_185_1076_0
= _rtB -> B_185_1073_0 ; } else { _rtB -> B_185_1076_0 = _rtB -> B_185_1075_0
; } if ( ( _rtDW -> TimeStampA_ii >= ssGetT ( S ) ) && ( _rtDW ->
TimeStampB_fs >= ssGetT ( S ) ) ) { _rtB -> B_185_1077_0 = 0.0 ; } else {
rtb_B_185_34_0 = _rtDW -> TimeStampA_ii ; lastU = & _rtDW -> LastUAtTimeA_e ;
if ( _rtDW -> TimeStampA_ii < _rtDW -> TimeStampB_fs ) { if ( _rtDW ->
TimeStampB_fs < ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_fs ;
lastU = & _rtDW -> LastUAtTimeB_eq ; } } else { if ( _rtDW -> TimeStampA_ii
>= ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_fs ; lastU = & _rtDW
-> LastUAtTimeB_eq ; } } _rtB -> B_185_1077_0 = ( _rtB -> B_185_1075_0 - *
lastU ) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2
, 0 ) ; if ( isHit != 0 ) { if ( _rtB -> B_185_1083_0 >= _rtP -> P_790 ) {
_rtB -> B_185_1084_0 = rtb_B_185_1070_0 ; } else { _rtB -> B_185_1084_0 =
_rtB -> B_185_1072_0 ; } } _rtB -> B_185_1086_0 = rt_Lookup ( _rtP -> P_791 ,
5 , ssGetT ( S ) , _rtP -> P_792 ) ; if ( _rtB -> B_185_1082_0 >= _rtP ->
P_793 ) { _rtB -> B_185_1087_0 = _rtB -> B_185_1084_0 ; } else { _rtB ->
B_185_1087_0 = _rtB -> B_185_1086_0 ; } if ( ( _rtDW -> TimeStampA_oi >=
ssGetT ( S ) ) && ( _rtDW -> TimeStampB_cm >= ssGetT ( S ) ) ) { _rtB ->
B_185_1088_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW -> TimeStampA_oi ; lastU
= & _rtDW -> LastUAtTimeA_h ; if ( _rtDW -> TimeStampA_oi < _rtDW ->
TimeStampB_cm ) { if ( _rtDW -> TimeStampB_cm < ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_cm ; lastU = & _rtDW -> LastUAtTimeB_j ;
} } else { if ( _rtDW -> TimeStampA_oi >= ssGetT ( S ) ) { rtb_B_185_34_0 =
_rtDW -> TimeStampB_cm ; lastU = & _rtDW -> LastUAtTimeB_j ; } } _rtB ->
B_185_1088_0 = ( _rtB -> B_185_1086_0 - * lastU ) / ( ssGetT ( S ) -
rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 )
{ if ( _rtB -> B_185_1094_0 >= _rtP -> P_797 ) { _rtB -> B_185_1095_0 =
rtb_B_185_1070_0 ; } else { _rtB -> B_185_1095_0 = _rtB -> B_185_1072_0 ; } }
_rtB -> B_185_1097_0 = rt_Lookup ( _rtP -> P_798 , 5 , ssGetT ( S ) , _rtP ->
P_799 ) ; if ( _rtB -> B_185_1093_0 >= _rtP -> P_800 ) { _rtB -> B_185_1098_0
= _rtB -> B_185_1095_0 ; } else { _rtB -> B_185_1098_0 = _rtB -> B_185_1097_0
; } if ( ( _rtDW -> TimeStampA_c >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB_j
>= ssGetT ( S ) ) ) { _rtB -> B_185_1099_0 = 0.0 ; } else { rtb_B_185_34_0 =
_rtDW -> TimeStampA_c ; lastU = & _rtDW -> LastUAtTimeA_b ; if ( _rtDW ->
TimeStampA_c < _rtDW -> TimeStampB_j ) { if ( _rtDW -> TimeStampB_j < ssGetT
( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_j ; lastU = & _rtDW ->
LastUAtTimeB_p1 ; } } else { if ( _rtDW -> TimeStampA_c >= ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_j ; lastU = & _rtDW -> LastUAtTimeB_p1 ;
} } _rtB -> B_185_1099_0 = ( _rtB -> B_185_1097_0 - * lastU ) / ( ssGetT ( S
) - rtb_B_185_34_0 ) ; } ssCallAccelRunBlock ( S , 185 , 1111 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 185 , 1112 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 185 , 1113 ,
SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0
) { _rtB -> B_185_1114_0 = _rtP -> P_802 * rtb_B_185_280_0 ; } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_1115_0 = _rtP
-> P_803 * _rtB -> B_185_267_0 ; } _rtB -> B_185_1121_0 = look1_binlxpw (
muDoubleScalarRem ( ssGetT ( S ) - _rtB -> B_185_1118_0 , _rtB ->
B_185_1117_0 ) , _rtP -> P_806 , _rtP -> P_805 , 1U ) ; isHit = ssIsSampleHit
( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_1129_0 [ 2 ] = ( _rtP ->
P_807 * B_185_226_0_idx_2 + _rtP -> P_808 * B_185_228_0_idx_2 ) + _rtP ->
P_811 * _rtB -> B_185_224_0 ; _rtB -> B_185_1129_0 [ 0 ] = ( _rtP -> P_807 *
B_185_226_0_idx_0 + _rtP -> P_808 * B_185_228_0_idx_0 ) + _rtP -> P_811 *
B_185_223_0_idx_0 ; _rtB -> B_185_1131_0 [ 0 ] = ( _rtP -> P_809 *
B_185_226_0_idx_0 + _rtP -> P_810 * B_185_228_0_idx_0 ) + _rtP -> P_812 *
B_185_223_0_idx_0 ; _rtB -> B_185_1129_0 [ 1 ] = ( _rtP -> P_807 *
B_185_226_0_idx_1 + _rtP -> P_808 * B_185_228_0_idx_1 ) + _rtP -> P_811 *
B_185_223_0_idx_1 ; _rtB -> B_185_1131_0 [ 1 ] = ( _rtP -> P_809 *
B_185_226_0_idx_1 + _rtP -> P_810 * B_185_228_0_idx_1 ) + _rtP -> P_812 *
B_185_223_0_idx_1 ; _rtB -> B_185_1131_0 [ 2 ] = ( _rtP -> P_809 *
B_185_226_0_idx_2 + _rtP -> P_810 * B_185_228_0_idx_2 ) + _rtP -> P_812 *
_rtB -> B_185_224_0 ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0
) { ssCallAccelRunBlock ( S , 185 , 1132 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_185_1139_0 = _rtP -> P_813 * _rtB -> B_185_926_0 [ 193 ] ; _rtB ->
B_185_1143_0 = _rtP -> P_814 * _rtB -> B_185_926_0 [ 194 ] ;
B_185_316_0_idx_0 *= _rtDW -> UnitDelay1_DSTATE_h [ 0 ] ; rtb_B_16_3_0 *=
_rtDW -> UnitDelay1_DSTATE_b2 [ 0 ] ; _rtB -> B_185_1150_0 = (
B_185_316_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_h [ 1 ] + B_185_316_0_idx_0 )
* _rtP -> P_816 ; if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_h != 0 )
{ _rtB -> B_185_1151_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_p ; } else {
_rtB -> B_185_1151_0 = _rtP -> P_817 * _rtB -> B_185_1150_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_p ; } _rtB -> B_185_1168_0 = _rtP -> P_819 *
_rtB -> B_185_926_0 [ 195 ] ; _rtB -> B_185_1172_0 = _rtP -> P_820 * _rtB ->
B_185_926_0 [ 196 ] ; rtb_B_12_7_0 *= _rtDW -> UnitDelay1_DSTATE_iy [ 0 ] ;
_rtB -> B_185_1179_0 = ( B_185_329_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_b2 [
1 ] + rtb_B_16_3_0 ) * _rtP -> P_822 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_g != 0 ) { _rtB -> B_185_1180_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_b ; } else { _rtB -> B_185_1180_0 = _rtP ->
P_823 * _rtB -> B_185_1179_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_b ; }
_rtB -> B_185_1197_0 = _rtP -> P_825 * _rtB -> B_185_926_0 [ 197 ] ; _rtB ->
B_185_1201_0 = _rtP -> P_826 * _rtB -> B_185_926_0 [ 198 ] ; rtb_B_15_7_0 *=
_rtDW -> UnitDelay1_DSTATE_f [ 0 ] ; _rtB -> B_185_1208_0 = (
B_185_342_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_iy [ 1 ] + rtb_B_12_7_0 ) *
_rtP -> P_828 ; if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_i != 0 ) {
_rtB -> B_185_1209_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_d ; } else {
_rtB -> B_185_1209_0 = _rtP -> P_829 * _rtB -> B_185_1208_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_d ; } _rtB -> B_185_1226_0 = _rtP -> P_831 *
_rtB -> B_185_926_0 [ 199 ] ; _rtB -> B_185_1230_0 = _rtP -> P_832 * _rtB ->
B_185_926_0 [ 200 ] ; rtb_B_13_7_0 *= _rtDW -> UnitDelay1_DSTATE_p [ 0 ] ;
_rtB -> B_185_1237_0 = ( B_185_355_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_f [ 1
] + rtb_B_15_7_0 ) * _rtP -> P_834 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_j != 0 ) { _rtB -> B_185_1238_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_a ; } else { _rtB -> B_185_1238_0 = _rtP ->
P_835 * _rtB -> B_185_1237_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_a ; }
_rtB -> B_185_1255_0 = _rtP -> P_837 * _rtB -> B_185_926_0 [ 201 ] ; _rtB ->
B_185_1259_0 = _rtP -> P_838 * _rtB -> B_185_926_0 [ 202 ] ;
B_185_381_0_idx_0 *= _rtDW -> UnitDelay1_DSTATE_i1 [ 0 ] ; _rtB ->
B_185_1266_0 = ( B_185_368_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_p [ 1 ] +
rtb_B_13_7_0 ) * _rtP -> P_840 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_g4 != 0 ) { _rtB -> B_185_1267_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_jm ; } else { _rtB -> B_185_1267_0 = _rtP ->
P_841 * _rtB -> B_185_1266_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_jm ; }
_rtB -> B_185_1284_0 = _rtP -> P_843 * _rtB -> B_185_926_0 [ 203 ] ; _rtB ->
B_185_1288_0 = _rtP -> P_844 * _rtB -> B_185_926_0 [ 204 ] ; _rtB ->
B_185_1295_0 = ( B_185_381_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_i1 [ 1 ] +
B_185_381_0_idx_0 ) * _rtP -> P_846 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_m != 0 ) { _rtB -> B_185_1296_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_c ; } else { _rtB -> B_185_1296_0 = _rtP ->
P_847 * _rtB -> B_185_1295_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_c ; }
_rtB -> B_185_1307_0 = _rtP -> P_849 * _rtB -> B_185_926_0 [ 187 ] ; _rtB ->
B_185_1311_0 = _rtP -> P_850 * _rtB -> B_185_926_0 [ 188 ] ; _rtB ->
B_185_1315_0 = _rtP -> P_851 * _rtB -> B_185_926_0 [ 189 ] ; _rtB ->
B_185_1319_0 = _rtP -> P_852 * _rtB -> B_185_926_0 [ 190 ] ; _rtB ->
B_185_1323_0 = _rtP -> P_853 * _rtB -> B_185_926_0 [ 191 ] ; _rtB ->
B_185_1327_0 = _rtP -> P_854 * _rtB -> B_185_926_0 [ 192 ] ; } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_1342_0 = _rtDW ->
Delay_x1_DSTATE_a ; rtb_B_185_1344_0 = _rtDW -> Delay_x2_DSTATE_b ;
rtb_B_185_1348_0 = _rtP -> P_856 * _rtDW -> Delay_x1_DSTATE_a + _rtP -> P_858
* _rtDW -> Delay_x2_DSTATE_b ; rtb_B_185_1349_0 = _rtP -> P_859 * _rtDW ->
Delay_x1_DSTATE_a + _rtP -> P_860 * _rtDW -> Delay_x2_DSTATE_b ; } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_1350_0 = _rtP
-> P_861 * _rtB -> B_185_926_0 [ 122 ] ; } isHit = ssIsSampleHit ( S , 3 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_1352_0 = _rtP -> P_862 * _rtB ->
B_185_1350_0 + rtb_B_185_1348_0 ; _rtB -> B_185_1354_0 = _rtP -> P_863 * _rtB
-> B_185_1350_0 + rtb_B_185_1349_0 ; _rtB -> B_185_1359_0 = ( _rtP -> P_865 *
rtb_B_185_1342_0 + _rtP -> P_866 * rtb_B_185_1344_0 ) + _rtP -> P_864 * _rtB
-> B_185_1350_0 ; } ssCallAccelRunBlock ( S , 185 , 1360 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 185 , 1361 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_185_1362_0 = _rtP -> P_867 * _rtB ->
B_185_102_0 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB
-> B_185_1364_0 = _rtDW -> Memory_PreviousInput ; } isHit = ssIsSampleHit ( S
, 2 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_1368_0 [ 0 ] = _rtP -> P_870 *
_rtB -> B_185_926_0 [ 98 ] * _rtP -> P_873 ; _rtB -> B_185_1368_0 [ 1 ] =
_rtP -> P_871 * _rtB -> B_185_926_0 [ 99 ] * _rtP -> P_873 ; _rtB ->
B_185_1368_0 [ 2 ] = _rtP -> P_872 * _rtB -> B_185_926_0 [ 100 ] * _rtP ->
P_873 ; } normal_operation_no_faults_AutomaticGainControl ( S , _rtB ->
B_185_1369_0 , _rtB -> B_185_1364_0 , _rtB -> B_185_12_0 , _rtB ->
B_185_1368_0 , & _rtB -> AutomaticGainControl , & _rtDW ->
AutomaticGainControl , & _rtP -> AutomaticGainControl , & _rtX ->
AutomaticGainControl , & ( ( XDis_normal_operation_no_faults_T * )
ssGetContStateDisabled ( S ) ) -> AutomaticGainControl ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtX -> Integrator_CSTATE_ja >= _rtP ->
P_876 ) { _rtX -> Integrator_CSTATE_ja = _rtP -> P_876 ; } else { if ( _rtX
-> Integrator_CSTATE_ja <= _rtP -> P_877 ) { _rtX -> Integrator_CSTATE_ja =
_rtP -> P_877 ; } } } _rtB -> B_185_1371_0 = _rtX -> Integrator_CSTATE_ja ; {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK . TUbufferPtrs [ 1 ] ; real_T * * xBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 2 ] ;
real_T simTime = ssGetT ( S ) ; real_T appliedDelay ; _rtB -> B_185_1372_0 =
normal_operation_no_faults_acc_rt_VTDelayfindtDInterpolate ( ( (
X_normal_operation_no_faults_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE , * tBuffer , * uBuffer , * xBuffer , _rtDW ->
VariableTransportDelay_IWORK . CircularBufSize , _rtDW ->
VariableTransportDelay_IWORK . Head , _rtDW -> VariableTransportDelay_IWORK .
Tail , & _rtDW -> VariableTransportDelay_IWORK . Last , simTime , 0.0 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) , _rtP -> P_879 , & appliedDelay ) ; } _rtB -> B_185_1373_0
= _rtX -> integrator_CSTATE ; _rtB -> B_185_1374_0 = ssGetT ( S ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( ssIsMajorTimeStep ( S
) != 0 ) { _rtDW -> RelationalOperator_Mode_n = ( _rtB -> B_185_1374_0 >=
_rtB -> B_185_1375_0 ) ; } _rtB -> B_185_1376_0 = _rtDW ->
RelationalOperator_Mode_n ; _rtB -> B_185_1377_0 = _rtDW ->
Memory_PreviousInput_f ; } if ( _rtB -> B_185_1376_0 ) { _rtB -> B_43_0_0 =
_rtB -> B_185_1373_0 - _rtB -> B_185_1372_0 ; _rtB -> B_43_1_0 = _rtB ->
B_43_0_0 * _rtB -> B_185_1364_0 ; _rtB -> B_185_1379_0 = _rtB -> B_43_1_0 ; }
else { _rtB -> B_185_1379_0 = _rtB -> B_185_1377_0 ; } _rtB -> B_185_1380_0 =
_rtB -> B_185_1379_0 * _rtB -> AutomaticGainControl . B_42_19_0 ; _rtB ->
B_185_1381_0 = _rtP -> P_883 * _rtB -> B_185_1380_0 ; _rtB -> B_185_1382_0 =
_rtP -> P_884 * _rtB -> B_185_1380_0 ; _rtB -> B_185_1383_0 = _rtP -> P_885 *
_rtB -> B_185_1380_0 ; _rtB -> B_185_1384_0 = 0.0 ; _rtB -> B_185_1384_0 +=
_rtP -> P_887 * _rtX -> TransferFcn_CSTATE ; _rtB -> B_185_1384_0 += _rtP ->
P_888 * _rtB -> B_185_1383_0 ; _rtB -> B_185_1385_0 = ( _rtB -> B_185_1381_0
+ _rtB -> B_185_1371_0 ) + _rtB -> B_185_1384_0 ; if ( ssIsMajorTimeStep ( S
) != 0 ) { _rtDW -> Saturation2_MODE_f = _rtB -> B_185_1385_0 >= _rtP ->
P_889 ? 1 : _rtB -> B_185_1385_0 > _rtP -> P_890 ? 0 : - 1 ; } _rtB ->
B_185_1386_0 = _rtDW -> Saturation2_MODE_f == 1 ? _rtP -> P_889 : _rtDW ->
Saturation2_MODE_f == - 1 ? _rtP -> P_890 : _rtB -> B_185_1385_0 ; _rtB ->
B_185_1387_0 = _rtP -> P_891 * _rtB -> B_185_1386_0 ; if ( ( _rtDW ->
LastMajorTimeA >= ssGetTaskTime ( S , 0 ) ) && ( _rtDW -> LastMajorTimeB >=
ssGetTaskTime ( S , 0 ) ) ) { _rtB -> B_185_1388_0 = _rtB -> B_185_1387_0 ; }
else { if ( ( ( _rtDW -> LastMajorTimeA < _rtDW -> LastMajorTimeB ) && (
_rtDW -> LastMajorTimeB < ssGetTaskTime ( S , 0 ) ) ) || ( ( _rtDW ->
LastMajorTimeA >= _rtDW -> LastMajorTimeB ) && ( _rtDW -> LastMajorTimeA >=
ssGetTaskTime ( S , 0 ) ) ) ) { rtb_B_185_1024_0 = ssGetTaskTime ( S , 0 ) -
_rtDW -> LastMajorTimeB ; rtb_B_185_280_0 = _rtDW -> PrevYB ; } else {
rtb_B_185_1024_0 = ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeA ;
rtb_B_185_280_0 = _rtDW -> PrevYA ; } rtb_B_185_1070_0 = rtb_B_185_1024_0 *
_rtP -> P_892 ; rtb_B_185_978_0 = _rtB -> B_185_1387_0 - rtb_B_185_280_0 ; if
( rtb_B_185_978_0 > rtb_B_185_1070_0 ) { _rtB -> B_185_1388_0 =
rtb_B_185_280_0 + rtb_B_185_1070_0 ; } else { rtb_B_185_1024_0 *= _rtP ->
P_893 ; if ( rtb_B_185_978_0 < rtb_B_185_1024_0 ) { _rtB -> B_185_1388_0 =
rtb_B_185_280_0 + rtb_B_185_1024_0 ; } else { _rtB -> B_185_1388_0 = _rtB ->
B_185_1387_0 ; } } } _rtB -> B_185_1389_0 = _rtX -> Integrator_x1_CSTATE ;
_rtB -> B_185_1390_0 = _rtP -> P_895 * _rtB -> B_185_1389_0 ; _rtB ->
B_185_1391_0 = _rtX -> Integrator_x2_CSTATE ; _rtB -> B_185_1392_0 = _rtP ->
P_897 * _rtB -> B_185_1391_0 ; _rtB -> B_185_1393_0 = _rtP -> P_898 * _rtB ->
B_185_1389_0 ; _rtB -> B_185_1394_0 = _rtP -> P_899 * _rtB -> B_185_1391_0 ;
_rtB -> B_185_1395_0 = _rtB -> B_185_1390_0 + _rtB -> B_185_1392_0 ; _rtB ->
B_185_1396_0 = _rtB -> B_185_1393_0 + _rtB -> B_185_1394_0 ; _rtB ->
B_185_1397_0 = _rtP -> P_900 * _rtB -> B_185_1388_0 ; _rtB -> B_185_1398_0 =
_rtB -> B_185_1395_0 + _rtB -> B_185_1397_0 ; _rtB -> B_185_1399_0 = _rtP ->
P_901 * _rtB -> B_185_1388_0 ; _rtB -> B_185_1400_0 = _rtB -> B_185_1396_0 +
_rtB -> B_185_1399_0 ; _rtB -> B_185_1405_0 = ( _rtP -> P_902 * _rtB ->
B_185_1389_0 + _rtP -> P_903 * _rtB -> B_185_1391_0 ) + _rtP -> P_904 * _rtB
-> B_185_1388_0 ; if ( _rtB -> B_185_1364_0 > _rtP -> P_905 ) { _rtB ->
B_185_1406_0 = _rtP -> P_905 ; } else if ( _rtB -> B_185_1364_0 < _rtP ->
P_906 ) { _rtB -> B_185_1406_0 = _rtP -> P_906 ; } else { _rtB ->
B_185_1406_0 = _rtB -> B_185_1364_0 ; } _rtB -> B_185_1407_0 = 1.0 / _rtB ->
B_185_1406_0 ; for ( i = 0 ; i < 3 ; i ++ ) { _rtB -> B_185_1413_0 [ i ] =
0.0 ; _rtB -> B_185_1413_0 [ i ] += _rtP -> P_910 [ i ] * _rtB ->
B_185_1368_0 [ 0 ] ; _rtB -> B_185_1413_0 [ i ] += _rtP -> P_910 [ i + 3 ] *
_rtB -> B_185_1368_0 [ 1 ] ; _rtB -> B_185_1413_0 [ i ] += _rtP -> P_910 [ i
+ 6 ] * _rtB -> B_185_1368_0 [ 2 ] ; _rtB -> B_185_1414_0 [ i ] = _rtP ->
P_911 * _rtB -> B_185_1413_0 [ i ] ; }
normal_operation_no_faults_Subsystempi2delay_m ( S , _rtB -> B_185_1412_0 , &
_rtB -> B_185_1414_0 [ 0 ] , _rtB -> B_185_12_0 , & _rtB ->
Subsystempi2delay_i , & _rtDW -> Subsystempi2delay_i ) ;
normal_operation_no_faults_Subsystem1_h4 ( S , _rtB -> B_185_1410_0 , & _rtB
-> B_185_1414_0 [ 0 ] , _rtB -> B_185_12_0 , & _rtB -> Subsystem1_f , & _rtDW
-> Subsystem1_f ) ; if ( _rtB -> B_185_1410_0 != 0 ) { _rtB -> B_185_1417_0 [
0 ] = _rtB -> Subsystem1_f . B_35_0_0 ; _rtB -> B_185_1417_0 [ 1 ] = _rtB ->
Subsystem1_f . B_35_1_0 ; } else { _rtB -> B_185_1417_0 [ 0 ] = _rtB ->
Subsystempi2delay_i . B_34_0_0 ; _rtB -> B_185_1417_0 [ 1 ] = _rtB ->
Subsystempi2delay_i . B_34_1_0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if (
isHit != 0 ) { _rtB -> B_185_1425_0 [ 2 ] = ( _rtP -> P_912 *
B_185_23_0_idx_2 + _rtP -> P_913 * B_185_25_0_idx_2 ) + _rtP -> P_916 * _rtB
-> B_185_21_0 ; _rtB -> B_185_1425_0 [ 0 ] = ( _rtP -> P_912 *
B_185_23_0_idx_0 + _rtP -> P_913 * B_185_25_0_idx_0 ) + _rtP -> P_916 * _rtB
-> B_185_20_0 [ 0 ] ; _rtB -> B_185_1427_0 [ 0 ] = ( _rtP -> P_914 *
B_185_23_0_idx_0 + _rtP -> P_915 * B_185_25_0_idx_0 ) + _rtP -> P_917 * _rtB
-> B_185_20_0 [ 0 ] ; _rtB -> B_185_1425_0 [ 1 ] = ( _rtP -> P_912 *
B_185_23_0_idx_1 + _rtP -> P_913 * B_185_25_0_idx_1 ) + _rtP -> P_916 * _rtB
-> B_185_20_0 [ 1 ] ; _rtB -> B_185_1427_0 [ 1 ] = ( _rtP -> P_914 *
B_185_23_0_idx_1 + _rtP -> P_915 * B_185_25_0_idx_1 ) + _rtP -> P_917 * _rtB
-> B_185_20_0 [ 1 ] ; _rtB -> B_185_1427_0 [ 2 ] = ( _rtP -> P_914 *
B_185_23_0_idx_2 + _rtP -> P_915 * B_185_25_0_idx_2 ) + _rtP -> P_917 * _rtB
-> B_185_21_0 ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_185_1434_0 = _rtP -> P_918 * _rtB -> B_185_926_0 [ 175 ] ; _rtB ->
B_185_1438_0 = _rtP -> P_919 * _rtB -> B_185_926_0 [ 176 ] ;
B_185_135_0_idx_0 *= _rtDW -> UnitDelay1_DSTATE_e [ 0 ] ; rtb_B_51_7_0 *=
_rtDW -> UnitDelay1_DSTATE_ef [ 0 ] ; _rtB -> B_185_1445_0 = (
B_185_135_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_e [ 1 ] + B_185_135_0_idx_0 )
* _rtP -> P_921 ; if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_hp != 0
) { _rtB -> B_185_1446_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_j1 ; } else
{ _rtB -> B_185_1446_0 = _rtP -> P_922 * _rtB -> B_185_1445_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_j1 ; } _rtB -> B_185_1463_0 = _rtP -> P_924 *
_rtB -> B_185_926_0 [ 177 ] ; _rtB -> B_185_1467_0 = _rtP -> P_925 * _rtB ->
B_185_926_0 [ 178 ] ; rtb_B_47_7_0 *= _rtDW -> UnitDelay1_DSTATE_hm [ 0 ] ;
_rtB -> B_185_1474_0 = ( B_185_148_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_ef [
1 ] + rtb_B_51_7_0 ) * _rtP -> P_927 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_l != 0 ) { _rtB -> B_185_1475_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_h ; } else { _rtB -> B_185_1475_0 = _rtP ->
P_928 * _rtB -> B_185_1474_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_h ; }
_rtB -> B_185_1492_0 = _rtP -> P_930 * _rtB -> B_185_926_0 [ 179 ] ; _rtB ->
B_185_1496_0 = _rtP -> P_931 * _rtB -> B_185_926_0 [ 180 ] ; rtb_B_50_7_0 *=
_rtDW -> UnitDelay1_DSTATE_j [ 0 ] ; _rtB -> B_185_1503_0 = (
B_185_161_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_hm [ 1 ] + rtb_B_47_7_0 ) *
_rtP -> P_933 ; if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_lm != 0 )
{ _rtB -> B_185_1504_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_b5 ; } else {
_rtB -> B_185_1504_0 = _rtP -> P_934 * _rtB -> B_185_1503_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_b5 ; } _rtB -> B_185_1521_0 = _rtP -> P_936 *
_rtB -> B_185_926_0 [ 181 ] ; _rtB -> B_185_1525_0 = _rtP -> P_937 * _rtB ->
B_185_926_0 [ 182 ] ; rtb_B_48_7_0 *= _rtDW -> UnitDelay1_DSTATE_he [ 0 ] ;
_rtB -> B_185_1532_0 = ( B_185_174_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_j [ 1
] + rtb_B_50_7_0 ) * _rtP -> P_939 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_d != 0 ) { _rtB -> B_185_1533_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_pu ; } else { _rtB -> B_185_1533_0 = _rtP ->
P_940 * _rtB -> B_185_1532_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_pu ; }
_rtB -> B_185_1550_0 = _rtP -> P_942 * _rtB -> B_185_926_0 [ 183 ] ; _rtB ->
B_185_1554_0 = _rtP -> P_943 * _rtB -> B_185_926_0 [ 184 ] ; rtb_B_49_7_0 *=
_rtDW -> UnitDelay1_DSTATE_b5 [ 0 ] ; _rtB -> B_185_1561_0 = (
B_185_187_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_he [ 1 ] + rtb_B_48_7_0 ) *
_rtP -> P_945 ; if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_o != 0 ) {
_rtB -> B_185_1562_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_pe ; } else {
_rtB -> B_185_1562_0 = _rtP -> P_946 * _rtB -> B_185_1561_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_pe ; } _rtB -> B_185_1579_0 = _rtP -> P_948 *
_rtB -> B_185_926_0 [ 185 ] ; _rtB -> B_185_1583_0 = _rtP -> P_949 * _rtB ->
B_185_926_0 [ 186 ] ; _rtB -> B_185_1590_0 = ( B_185_200_0_idx_1 * _rtDW ->
UnitDelay1_DSTATE_b5 [ 1 ] + rtb_B_49_7_0 ) * _rtP -> P_951 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_a != 0 ) { _rtB -> B_185_1591_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_e ; } else { _rtB -> B_185_1591_0 = _rtP ->
P_952 * _rtB -> B_185_1590_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_e ; }
_rtB -> B_185_1602_0 = _rtP -> P_954 * _rtB -> B_185_926_0 [ 166 ] ; _rtB ->
B_185_1606_0 = _rtP -> P_955 * _rtB -> B_185_926_0 [ 170 ] ; _rtB ->
B_185_1610_0 = _rtP -> P_956 * _rtB -> B_185_926_0 [ 171 ] ; _rtB ->
B_185_1614_0 = _rtP -> P_957 * _rtB -> B_185_926_0 [ 172 ] ; _rtB ->
B_185_1618_0 = _rtP -> P_958 * _rtB -> B_185_926_0 [ 173 ] ; _rtB ->
B_185_1622_0 = _rtP -> P_959 * _rtB -> B_185_926_0 [ 174 ] ; } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_1638_0 = _rtDW ->
Delay_x1_DSTATE_c ; rtb_B_185_1640_0 = _rtDW -> Delay_x2_DSTATE_mh ;
rtb_B_185_1644_0 = _rtP -> P_961 * _rtDW -> Delay_x1_DSTATE_c + _rtP -> P_963
* _rtDW -> Delay_x2_DSTATE_mh ; rtb_B_185_1645_0 = _rtP -> P_964 * _rtDW ->
Delay_x1_DSTATE_c + _rtP -> P_965 * _rtDW -> Delay_x2_DSTATE_mh ; } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_1646_0 = _rtP
-> P_966 * _rtB -> B_185_926_0 [ 123 ] ; } isHit = ssIsSampleHit ( S , 3 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_1648_0 = _rtP -> P_967 * _rtB ->
B_185_1646_0 + rtb_B_185_1644_0 ; _rtB -> B_185_1650_0 = _rtP -> P_968 * _rtB
-> B_185_1646_0 + rtb_B_185_1645_0 ; _rtB -> B_185_1655_0 = ( _rtP -> P_970 *
rtb_B_185_1638_0 + _rtP -> P_971 * rtb_B_185_1640_0 ) + _rtP -> P_969 * _rtB
-> B_185_1646_0 ; } ssCallAccelRunBlock ( S , 185 , 1656 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 185 , 1657 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_185_1658_0 = _rtP -> P_972 * _rtB ->
B_185_487_0 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB
-> B_185_1660_0 = _rtDW -> Memory_PreviousInput_f1 ; } isHit = ssIsSampleHit
( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_1664_0 [ 0 ] = _rtP ->
P_975 * _rtB -> B_185_926_0 [ 104 ] * _rtP -> P_978 ; _rtB -> B_185_1664_0 [
1 ] = _rtP -> P_976 * _rtB -> B_185_926_0 [ 105 ] * _rtP -> P_978 ; _rtB ->
B_185_1664_0 [ 2 ] = _rtP -> P_977 * _rtB -> B_185_926_0 [ 106 ] * _rtP ->
P_978 ; } normal_operation_no_faults_AutomaticGainControl ( S , _rtB ->
B_185_1665_0 , _rtB -> B_185_1660_0 , _rtB -> B_185_399_0 , _rtB ->
B_185_1664_0 , & _rtB -> AutomaticGainControl_d , & _rtDW ->
AutomaticGainControl_d , & _rtP -> AutomaticGainControl_d , & _rtX ->
AutomaticGainControl_d , & ( ( XDis_normal_operation_no_faults_T * )
ssGetContStateDisabled ( S ) ) -> AutomaticGainControl_d ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtX -> Integrator_CSTATE_lt >= _rtP ->
P_981 ) { _rtX -> Integrator_CSTATE_lt = _rtP -> P_981 ; } else { if ( _rtX
-> Integrator_CSTATE_lt <= _rtP -> P_982 ) { _rtX -> Integrator_CSTATE_lt =
_rtP -> P_982 ; } } } _rtB -> B_185_1667_0 = _rtX -> Integrator_CSTATE_lt ; {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_f
. TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK_f . TUbufferPtrs [ 1 ] ; real_T * * xBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_f . TUbufferPtrs [ 2 ] ;
real_T simTime = ssGetT ( S ) ; real_T appliedDelay ; _rtB -> B_185_1668_0 =
normal_operation_no_faults_acc_rt_VTDelayfindtDInterpolate ( ( (
X_normal_operation_no_faults_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_i , * tBuffer , * uBuffer , * xBuffer , _rtDW
-> VariableTransportDelay_IWORK_g . CircularBufSize , _rtDW ->
VariableTransportDelay_IWORK_g . Head , _rtDW ->
VariableTransportDelay_IWORK_g . Tail , & _rtDW ->
VariableTransportDelay_IWORK_g . Last , simTime , 0.0 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
, _rtP -> P_984 , & appliedDelay ) ; } _rtB -> B_185_1669_0 = _rtX ->
integrator_CSTATE_m ; _rtB -> B_185_1670_0 = ssGetT ( S ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( ssIsMajorTimeStep ( S
) != 0 ) { _rtDW -> RelationalOperator_Mode_f = ( _rtB -> B_185_1670_0 >=
_rtB -> B_185_1671_0 ) ; } _rtB -> B_185_1672_0 = _rtDW ->
RelationalOperator_Mode_f ; _rtB -> B_185_1673_0 = _rtDW ->
Memory_PreviousInput_o ; } if ( _rtB -> B_185_1672_0 ) { _rtB -> B_77_0_0 =
_rtB -> B_185_1669_0 - _rtB -> B_185_1668_0 ; _rtB -> B_77_1_0 = _rtB ->
B_77_0_0 * _rtB -> B_185_1660_0 ; _rtB -> B_185_1675_0 = _rtB -> B_77_1_0 ; }
else { _rtB -> B_185_1675_0 = _rtB -> B_185_1673_0 ; } _rtB -> B_185_1676_0 =
_rtB -> B_185_1675_0 * _rtB -> AutomaticGainControl_d . B_42_19_0 ; _rtB ->
B_185_1677_0 = _rtP -> P_988 * _rtB -> B_185_1676_0 ; _rtB -> B_185_1678_0 =
_rtP -> P_989 * _rtB -> B_185_1676_0 ; _rtB -> B_185_1679_0 = _rtP -> P_990 *
_rtB -> B_185_1676_0 ; _rtB -> B_185_1680_0 = 0.0 ; _rtB -> B_185_1680_0 +=
_rtP -> P_992 * _rtX -> TransferFcn_CSTATE_a ; _rtB -> B_185_1680_0 += _rtP
-> P_993 * _rtB -> B_185_1679_0 ; _rtB -> B_185_1681_0 = ( _rtB ->
B_185_1677_0 + _rtB -> B_185_1667_0 ) + _rtB -> B_185_1680_0 ; if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Saturation2_MODE_m = _rtB ->
B_185_1681_0 >= _rtP -> P_994 ? 1 : _rtB -> B_185_1681_0 > _rtP -> P_995 ? 0
: - 1 ; } _rtB -> B_185_1682_0 = _rtDW -> Saturation2_MODE_m == 1 ? _rtP ->
P_994 : _rtDW -> Saturation2_MODE_m == - 1 ? _rtP -> P_995 : _rtB ->
B_185_1681_0 ; _rtB -> B_185_1683_0 = _rtP -> P_996 * _rtB -> B_185_1682_0 ;
if ( ( _rtDW -> LastMajorTimeA_n >= ssGetTaskTime ( S , 0 ) ) && ( _rtDW ->
LastMajorTimeB_n >= ssGetTaskTime ( S , 0 ) ) ) { _rtB -> B_185_1684_0 = _rtB
-> B_185_1683_0 ; } else { if ( ( ( _rtDW -> LastMajorTimeA_n < _rtDW ->
LastMajorTimeB_n ) && ( _rtDW -> LastMajorTimeB_n < ssGetTaskTime ( S , 0 ) )
) || ( ( _rtDW -> LastMajorTimeA_n >= _rtDW -> LastMajorTimeB_n ) && ( _rtDW
-> LastMajorTimeA_n >= ssGetTaskTime ( S , 0 ) ) ) ) { rtb_B_185_1024_0 =
ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeB_n ; rtb_B_185_280_0 = _rtDW
-> PrevYB_b ; } else { rtb_B_185_1024_0 = ssGetTaskTime ( S , 0 ) - _rtDW ->
LastMajorTimeA_n ; rtb_B_185_280_0 = _rtDW -> PrevYA_p ; } rtb_B_185_1070_0 =
rtb_B_185_1024_0 * _rtP -> P_997 ; rtb_B_185_978_0 = _rtB -> B_185_1683_0 -
rtb_B_185_280_0 ; if ( rtb_B_185_978_0 > rtb_B_185_1070_0 ) { _rtB ->
B_185_1684_0 = rtb_B_185_280_0 + rtb_B_185_1070_0 ; } else { rtb_B_185_1024_0
*= _rtP -> P_998 ; if ( rtb_B_185_978_0 < rtb_B_185_1024_0 ) { _rtB ->
B_185_1684_0 = rtb_B_185_280_0 + rtb_B_185_1024_0 ; } else { _rtB ->
B_185_1684_0 = _rtB -> B_185_1683_0 ; } } } _rtB -> B_185_1685_0 = _rtX ->
Integrator_x1_CSTATE_n ; _rtB -> B_185_1686_0 = _rtP -> P_1000 * _rtB ->
B_185_1685_0 ; _rtB -> B_185_1687_0 = _rtX -> Integrator_x2_CSTATE_c ; _rtB
-> B_185_1688_0 = _rtP -> P_1002 * _rtB -> B_185_1687_0 ; _rtB ->
B_185_1689_0 = _rtP -> P_1003 * _rtB -> B_185_1685_0 ; _rtB -> B_185_1690_0 =
_rtP -> P_1004 * _rtB -> B_185_1687_0 ; _rtB -> B_185_1691_0 = _rtB ->
B_185_1686_0 + _rtB -> B_185_1688_0 ; _rtB -> B_185_1692_0 = _rtB ->
B_185_1689_0 + _rtB -> B_185_1690_0 ; _rtB -> B_185_1693_0 = _rtP -> P_1005 *
_rtB -> B_185_1684_0 ; _rtB -> B_185_1694_0 = _rtB -> B_185_1691_0 + _rtB ->
B_185_1693_0 ; _rtB -> B_185_1695_0 = _rtP -> P_1006 * _rtB -> B_185_1684_0 ;
_rtB -> B_185_1696_0 = _rtB -> B_185_1692_0 + _rtB -> B_185_1695_0 ; _rtB ->
B_185_1701_0 = ( _rtP -> P_1007 * _rtB -> B_185_1685_0 + _rtP -> P_1008 *
_rtB -> B_185_1687_0 ) + _rtP -> P_1009 * _rtB -> B_185_1684_0 ; if ( _rtB ->
B_185_1660_0 > _rtP -> P_1010 ) { _rtB -> B_185_1702_0 = _rtP -> P_1010 ; }
else if ( _rtB -> B_185_1660_0 < _rtP -> P_1011 ) { _rtB -> B_185_1702_0 =
_rtP -> P_1011 ; } else { _rtB -> B_185_1702_0 = _rtB -> B_185_1660_0 ; }
_rtB -> B_185_1703_0 = 1.0 / _rtB -> B_185_1702_0 ; for ( i = 0 ; i < 3 ; i
++ ) { _rtB -> B_185_1709_0 [ i ] = 0.0 ; _rtB -> B_185_1709_0 [ i ] += _rtP
-> P_1015 [ i ] * _rtB -> B_185_1664_0 [ 0 ] ; _rtB -> B_185_1709_0 [ i ] +=
_rtP -> P_1015 [ i + 3 ] * _rtB -> B_185_1664_0 [ 1 ] ; _rtB -> B_185_1709_0
[ i ] += _rtP -> P_1015 [ i + 6 ] * _rtB -> B_185_1664_0 [ 2 ] ; _rtB ->
B_185_1710_0 [ i ] = _rtP -> P_1016 * _rtB -> B_185_1709_0 [ i ] ; }
normal_operation_no_faults_Subsystempi2delay_m ( S , _rtB -> B_185_1708_0 , &
_rtB -> B_185_1710_0 [ 0 ] , _rtB -> B_185_399_0 , & _rtB ->
Subsystempi2delay_h4 , & _rtDW -> Subsystempi2delay_h4 ) ;
normal_operation_no_faults_Subsystem1_h4 ( S , _rtB -> B_185_1706_0 , & _rtB
-> B_185_1710_0 [ 0 ] , _rtB -> B_185_399_0 , & _rtB -> Subsystem1_h1 , &
_rtDW -> Subsystem1_h1 ) ; if ( _rtB -> B_185_1706_0 != 0 ) { _rtB ->
B_185_1713_0 [ 0 ] = _rtB -> Subsystem1_h1 . B_35_0_0 ; _rtB -> B_185_1713_0
[ 1 ] = _rtB -> Subsystem1_h1 . B_35_1_0 ; } else { _rtB -> B_185_1713_0 [ 0
] = _rtB -> Subsystempi2delay_h4 . B_34_0_0 ; _rtB -> B_185_1713_0 [ 1 ] =
_rtB -> Subsystempi2delay_h4 . B_34_1_0 ; } isHit = ssIsSampleHit ( S , 3 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_1721_0 [ 2 ] = ( _rtP -> P_1017 *
B_185_410_0_idx_2 + _rtP -> P_1018 * B_185_412_0_idx_2 ) + _rtP -> P_1021 *
_rtB -> B_185_408_0 ; _rtB -> B_185_1721_0 [ 0 ] = ( _rtP -> P_1017 *
B_185_410_0_idx_0 + _rtP -> P_1018 * B_185_412_0_idx_0 ) + _rtP -> P_1021 *
_rtB -> B_185_407_0 [ 0 ] ; _rtB -> B_185_1723_0 [ 0 ] = ( _rtP -> P_1019 *
B_185_410_0_idx_0 + _rtP -> P_1020 * B_185_412_0_idx_0 ) + _rtP -> P_1022 *
_rtB -> B_185_407_0 [ 0 ] ; _rtB -> B_185_1721_0 [ 1 ] = ( _rtP -> P_1017 *
B_185_410_0_idx_1 + _rtP -> P_1018 * B_185_412_0_idx_1 ) + _rtP -> P_1021 *
_rtB -> B_185_407_0 [ 1 ] ; _rtB -> B_185_1723_0 [ 1 ] = ( _rtP -> P_1019 *
B_185_410_0_idx_1 + _rtP -> P_1020 * B_185_412_0_idx_1 ) + _rtP -> P_1022 *
_rtB -> B_185_407_0 [ 1 ] ; _rtB -> B_185_1723_0 [ 2 ] = ( _rtP -> P_1019 *
B_185_410_0_idx_2 + _rtP -> P_1020 * B_185_412_0_idx_2 ) + _rtP -> P_1022 *
_rtB -> B_185_408_0 ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0
) { _rtB -> B_185_1730_0 = _rtP -> P_1023 * _rtB -> B_185_926_0 [ 211 ] ;
_rtB -> B_185_1734_0 = _rtP -> P_1024 * _rtB -> B_185_926_0 [ 212 ] ;
B_185_519_0_idx_0 *= _rtDW -> UnitDelay1_DSTATE_l [ 0 ] ; rtb_B_85_7_0 *=
_rtDW -> UnitDelay1_DSTATE_fb [ 0 ] ; _rtB -> B_185_1741_0 = (
B_185_519_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_l [ 1 ] + B_185_519_0_idx_0 )
* _rtP -> P_1026 ; if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_cg != 0
) { _rtB -> B_185_1742_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_if ; } else
{ _rtB -> B_185_1742_0 = _rtP -> P_1027 * _rtB -> B_185_1741_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_if ; } _rtB -> B_185_1759_0 = _rtP -> P_1029 *
_rtB -> B_185_926_0 [ 213 ] ; _rtB -> B_185_1763_0 = _rtP -> P_1030 * _rtB ->
B_185_926_0 [ 214 ] ; rtb_B_81_7_0 *= _rtDW -> UnitDelay1_DSTATE_d [ 0 ] ;
_rtB -> B_185_1770_0 = ( B_185_532_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_fb [
1 ] + rtb_B_85_7_0 ) * _rtP -> P_1032 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_f != 0 ) { _rtB -> B_185_1771_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_l ; } else { _rtB -> B_185_1771_0 = _rtP ->
P_1033 * _rtB -> B_185_1770_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_l ; }
_rtB -> B_185_1788_0 = _rtP -> P_1035 * _rtB -> B_185_926_0 [ 215 ] ; _rtB ->
B_185_1792_0 = _rtP -> P_1036 * _rtB -> B_185_926_0 [ 216 ] ; rtb_B_84_7_0 *=
_rtDW -> UnitDelay1_DSTATE_n [ 0 ] ; _rtB -> B_185_1799_0 = (
B_185_545_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_d [ 1 ] + rtb_B_81_7_0 ) *
_rtP -> P_1038 ; if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_ho != 0 )
{ _rtB -> B_185_1800_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_m ; } else {
_rtB -> B_185_1800_0 = _rtP -> P_1039 * _rtB -> B_185_1799_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_m ; } _rtB -> B_185_1817_0 = _rtP -> P_1041 *
_rtB -> B_185_926_0 [ 217 ] ; _rtB -> B_185_1821_0 = _rtP -> P_1042 * _rtB ->
B_185_926_0 [ 218 ] ; rtb_B_82_7_0 *= _rtDW -> UnitDelay1_DSTATE_hg [ 0 ] ;
_rtB -> B_185_1828_0 = ( B_185_558_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_n [ 1
] + rtb_B_84_7_0 ) * _rtP -> P_1044 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_k != 0 ) { _rtB -> B_185_1829_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_g ; } else { _rtB -> B_185_1829_0 = _rtP ->
P_1045 * _rtB -> B_185_1828_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_g ; }
_rtB -> B_185_1846_0 = _rtP -> P_1047 * _rtB -> B_185_926_0 [ 219 ] ; _rtB ->
B_185_1850_0 = _rtP -> P_1048 * _rtB -> B_185_926_0 [ 220 ] ; rtb_B_83_7_0 *=
_rtDW -> UnitDelay1_DSTATE_nq [ 0 ] ; _rtB -> B_185_1857_0 = (
B_185_571_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_hg [ 1 ] + rtb_B_82_7_0 ) *
_rtP -> P_1050 ; if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_fr != 0 )
{ _rtB -> B_185_1858_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_k ; } else {
_rtB -> B_185_1858_0 = _rtP -> P_1051 * _rtB -> B_185_1857_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_k ; } _rtB -> B_185_1875_0 = _rtP -> P_1053 *
_rtB -> B_185_926_0 [ 221 ] ; _rtB -> B_185_1879_0 = _rtP -> P_1054 * _rtB ->
B_185_926_0 [ 222 ] ; _rtB -> B_185_1886_0 = ( B_185_584_0_idx_1 * _rtDW ->
UnitDelay1_DSTATE_nq [ 1 ] + rtb_B_83_7_0 ) * _rtP -> P_1056 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_lt != 0 ) { _rtB -> B_185_1887_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_jd ; } else { _rtB -> B_185_1887_0 = _rtP ->
P_1057 * _rtB -> B_185_1886_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_jd ; }
_rtB -> B_185_1898_0 = _rtP -> P_1059 * _rtB -> B_185_926_0 [ 205 ] ; _rtB ->
B_185_1902_0 = _rtP -> P_1060 * _rtB -> B_185_926_0 [ 206 ] ; _rtB ->
B_185_1906_0 = _rtP -> P_1061 * _rtB -> B_185_926_0 [ 207 ] ; _rtB ->
B_185_1910_0 = _rtP -> P_1062 * _rtB -> B_185_926_0 [ 208 ] ; _rtB ->
B_185_1914_0 = _rtP -> P_1063 * _rtB -> B_185_926_0 [ 209 ] ; _rtB ->
B_185_1918_0 = _rtP -> P_1064 * _rtB -> B_185_926_0 [ 210 ] ; } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_1934_0 = _rtDW ->
Delay_x1_DSTATE_b ; rtb_B_185_1936_0 = _rtDW -> Delay_x2_DSTATE_e ;
rtb_B_185_1940_0 = _rtP -> P_1066 * _rtDW -> Delay_x1_DSTATE_b + _rtP ->
P_1068 * _rtDW -> Delay_x2_DSTATE_e ; rtb_B_185_1941_0 = _rtP -> P_1069 *
_rtDW -> Delay_x1_DSTATE_b + _rtP -> P_1070 * _rtDW -> Delay_x2_DSTATE_e ; }
isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_185_1942_0 = _rtP -> P_1071 * _rtB -> B_185_926_0 [ 124 ] ; } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_1944_0 = _rtP
-> P_1072 * _rtB -> B_185_1942_0 + rtb_B_185_1940_0 ; _rtB -> B_185_1946_0 =
_rtP -> P_1073 * _rtB -> B_185_1942_0 + rtb_B_185_1941_0 ; _rtB ->
B_185_1951_0 = ( _rtP -> P_1075 * rtb_B_185_1934_0 + _rtP -> P_1076 *
rtb_B_185_1936_0 ) + _rtP -> P_1074 * _rtB -> B_185_1942_0 ; }
ssCallAccelRunBlock ( S , 185 , 1952 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 185 , 1953 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_185_1954_0 = _rtP -> P_1077 * _rtB -> B_185_690_0 ; isHit = ssIsSampleHit (
S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_1956_0 = _rtDW ->
Memory_PreviousInput_c ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit
!= 0 ) { _rtB -> B_185_1960_0 [ 0 ] = _rtP -> P_1080 * _rtB -> B_185_926_0 [
110 ] * _rtP -> P_1083 ; _rtB -> B_185_1960_0 [ 1 ] = _rtP -> P_1081 * _rtB
-> B_185_926_0 [ 111 ] * _rtP -> P_1083 ; _rtB -> B_185_1960_0 [ 2 ] = _rtP
-> P_1082 * _rtB -> B_185_926_0 [ 112 ] * _rtP -> P_1083 ; }
normal_operation_no_faults_AutomaticGainControl ( S , _rtB -> B_185_1961_0 ,
_rtB -> B_185_1956_0 , _rtB -> B_185_602_0 , _rtB -> B_185_1960_0 , & _rtB ->
AutomaticGainControl_a , & _rtDW -> AutomaticGainControl_a , & _rtP ->
AutomaticGainControl_a , & _rtX -> AutomaticGainControl_a , & ( (
XDis_normal_operation_no_faults_T * ) ssGetContStateDisabled ( S ) ) ->
AutomaticGainControl_a ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtX ->
Integrator_CSTATE_bf >= _rtP -> P_1086 ) { _rtX -> Integrator_CSTATE_bf =
_rtP -> P_1086 ; } else { if ( _rtX -> Integrator_CSTATE_bf <= _rtP -> P_1087
) { _rtX -> Integrator_CSTATE_bf = _rtP -> P_1087 ; } } } _rtB ->
B_185_1963_0 = _rtX -> Integrator_CSTATE_bf ; { real_T * * uBuffer = ( real_T
* * ) & _rtDW -> VariableTransportDelay_PWORK_a . TUbufferPtrs [ 0 ] ; real_T
* * tBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_a .
TUbufferPtrs [ 1 ] ; real_T * * xBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK_a . TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT
( S ) ; real_T appliedDelay ; _rtB -> B_185_1964_0 =
normal_operation_no_faults_acc_rt_VTDelayfindtDInterpolate ( ( (
X_normal_operation_no_faults_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_o , * tBuffer , * uBuffer , * xBuffer , _rtDW
-> VariableTransportDelay_IWORK_b . CircularBufSize , _rtDW ->
VariableTransportDelay_IWORK_b . Head , _rtDW ->
VariableTransportDelay_IWORK_b . Tail , & _rtDW ->
VariableTransportDelay_IWORK_b . Last , simTime , 0.0 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
, _rtP -> P_1089 , & appliedDelay ) ; } _rtB -> B_185_1965_0 = _rtX ->
integrator_CSTATE_b ; _rtB -> B_185_1966_0 = ssGetT ( S ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( ssIsMajorTimeStep ( S
) != 0 ) { _rtDW -> RelationalOperator_Mode_h = ( _rtB -> B_185_1966_0 >=
_rtB -> B_185_1967_0 ) ; } _rtB -> B_185_1968_0 = _rtDW ->
RelationalOperator_Mode_h ; _rtB -> B_185_1969_0 = _rtDW ->
Memory_PreviousInput_d ; } if ( _rtB -> B_185_1968_0 ) { _rtB -> B_111_0_0 =
_rtB -> B_185_1965_0 - _rtB -> B_185_1964_0 ; _rtB -> B_111_1_0 = _rtB ->
B_111_0_0 * _rtB -> B_185_1956_0 ; _rtB -> B_185_1971_0 = _rtB -> B_111_1_0 ;
} else { _rtB -> B_185_1971_0 = _rtB -> B_185_1969_0 ; } _rtB -> B_185_1972_0
= _rtB -> B_185_1971_0 * _rtB -> AutomaticGainControl_a . B_42_19_0 ; _rtB ->
B_185_1973_0 = _rtP -> P_1093 * _rtB -> B_185_1972_0 ; _rtB -> B_185_1974_0 =
_rtP -> P_1094 * _rtB -> B_185_1972_0 ; _rtB -> B_185_1975_0 = _rtP -> P_1095
* _rtB -> B_185_1972_0 ; _rtB -> B_185_1976_0 = 0.0 ; _rtB -> B_185_1976_0 +=
_rtP -> P_1097 * _rtX -> TransferFcn_CSTATE_l ; _rtB -> B_185_1976_0 += _rtP
-> P_1098 * _rtB -> B_185_1975_0 ; _rtB -> B_185_1977_0 = ( _rtB ->
B_185_1973_0 + _rtB -> B_185_1963_0 ) + _rtB -> B_185_1976_0 ; if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Saturation2_MODE_fe = _rtB ->
B_185_1977_0 >= _rtP -> P_1099 ? 1 : _rtB -> B_185_1977_0 > _rtP -> P_1100 ?
0 : - 1 ; } _rtB -> B_185_1978_0 = _rtDW -> Saturation2_MODE_fe == 1 ? _rtP
-> P_1099 : _rtDW -> Saturation2_MODE_fe == - 1 ? _rtP -> P_1100 : _rtB ->
B_185_1977_0 ; _rtB -> B_185_1979_0 = _rtP -> P_1101 * _rtB -> B_185_1978_0 ;
if ( ( _rtDW -> LastMajorTimeA_e >= ssGetTaskTime ( S , 0 ) ) && ( _rtDW ->
LastMajorTimeB_i >= ssGetTaskTime ( S , 0 ) ) ) { _rtB -> B_185_1980_0 = _rtB
-> B_185_1979_0 ; } else { if ( ( ( _rtDW -> LastMajorTimeA_e < _rtDW ->
LastMajorTimeB_i ) && ( _rtDW -> LastMajorTimeB_i < ssGetTaskTime ( S , 0 ) )
) || ( ( _rtDW -> LastMajorTimeA_e >= _rtDW -> LastMajorTimeB_i ) && ( _rtDW
-> LastMajorTimeA_e >= ssGetTaskTime ( S , 0 ) ) ) ) { rtb_B_185_1024_0 =
ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeB_i ; rtb_B_185_280_0 = _rtDW
-> PrevYB_h ; } else { rtb_B_185_1024_0 = ssGetTaskTime ( S , 0 ) - _rtDW ->
LastMajorTimeA_e ; rtb_B_185_280_0 = _rtDW -> PrevYA_a ; } rtb_B_185_1070_0 =
rtb_B_185_1024_0 * _rtP -> P_1102 ; rtb_B_185_978_0 = _rtB -> B_185_1979_0 -
rtb_B_185_280_0 ; if ( rtb_B_185_978_0 > rtb_B_185_1070_0 ) { _rtB ->
B_185_1980_0 = rtb_B_185_280_0 + rtb_B_185_1070_0 ; } else { rtb_B_185_1024_0
*= _rtP -> P_1103 ; if ( rtb_B_185_978_0 < rtb_B_185_1024_0 ) { _rtB ->
B_185_1980_0 = rtb_B_185_280_0 + rtb_B_185_1024_0 ; } else { _rtB ->
B_185_1980_0 = _rtB -> B_185_1979_0 ; } } } _rtB -> B_185_1981_0 = _rtX ->
Integrator_x1_CSTATE_o ; _rtB -> B_185_1982_0 = _rtP -> P_1105 * _rtB ->
B_185_1981_0 ; _rtB -> B_185_1983_0 = _rtX -> Integrator_x2_CSTATE_e ; _rtB
-> B_185_1984_0 = _rtP -> P_1107 * _rtB -> B_185_1983_0 ; _rtB ->
B_185_1985_0 = _rtP -> P_1108 * _rtB -> B_185_1981_0 ; _rtB -> B_185_1986_0 =
_rtP -> P_1109 * _rtB -> B_185_1983_0 ; _rtB -> B_185_1987_0 = _rtB ->
B_185_1982_0 + _rtB -> B_185_1984_0 ; _rtB -> B_185_1988_0 = _rtB ->
B_185_1985_0 + _rtB -> B_185_1986_0 ; _rtB -> B_185_1989_0 = _rtP -> P_1110 *
_rtB -> B_185_1980_0 ; _rtB -> B_185_1990_0 = _rtB -> B_185_1987_0 + _rtB ->
B_185_1989_0 ; _rtB -> B_185_1991_0 = _rtP -> P_1111 * _rtB -> B_185_1980_0 ;
_rtB -> B_185_1992_0 = _rtB -> B_185_1988_0 + _rtB -> B_185_1991_0 ; _rtB ->
B_185_1997_0 = ( _rtP -> P_1112 * _rtB -> B_185_1981_0 + _rtP -> P_1113 *
_rtB -> B_185_1983_0 ) + _rtP -> P_1114 * _rtB -> B_185_1980_0 ; if ( _rtB ->
B_185_1956_0 > _rtP -> P_1115 ) { _rtB -> B_185_1998_0 = _rtP -> P_1115 ; }
else if ( _rtB -> B_185_1956_0 < _rtP -> P_1116 ) { _rtB -> B_185_1998_0 =
_rtP -> P_1116 ; } else { _rtB -> B_185_1998_0 = _rtB -> B_185_1956_0 ; }
_rtB -> B_185_1999_0 = 1.0 / _rtB -> B_185_1998_0 ; for ( i = 0 ; i < 3 ; i
++ ) { _rtB -> B_185_2005_0 [ i ] = 0.0 ; _rtB -> B_185_2005_0 [ i ] += _rtP
-> P_1120 [ i ] * _rtB -> B_185_1960_0 [ 0 ] ; _rtB -> B_185_2005_0 [ i ] +=
_rtP -> P_1120 [ i + 3 ] * _rtB -> B_185_1960_0 [ 1 ] ; _rtB -> B_185_2005_0
[ i ] += _rtP -> P_1120 [ i + 6 ] * _rtB -> B_185_1960_0 [ 2 ] ; _rtB ->
B_185_2006_0 [ i ] = _rtP -> P_1121 * _rtB -> B_185_2005_0 [ i ] ; }
normal_operation_no_faults_Subsystempi2delay_m ( S , _rtB -> B_185_2004_0 , &
_rtB -> B_185_2006_0 [ 0 ] , _rtB -> B_185_602_0 , & _rtB ->
Subsystempi2delay_en , & _rtDW -> Subsystempi2delay_en ) ;
normal_operation_no_faults_Subsystem1_h4 ( S , _rtB -> B_185_2002_0 , & _rtB
-> B_185_2006_0 [ 0 ] , _rtB -> B_185_602_0 , & _rtB -> Subsystem1_p , &
_rtDW -> Subsystem1_p ) ; if ( _rtB -> B_185_2002_0 != 0 ) { _rtB ->
B_185_2009_0 [ 0 ] = _rtB -> Subsystem1_p . B_35_0_0 ; _rtB -> B_185_2009_0 [
1 ] = _rtB -> Subsystem1_p . B_35_1_0 ; } else { _rtB -> B_185_2009_0 [ 0 ] =
_rtB -> Subsystempi2delay_en . B_34_0_0 ; _rtB -> B_185_2009_0 [ 1 ] = _rtB
-> Subsystempi2delay_en . B_34_1_0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ;
if ( isHit != 0 ) { _rtB -> B_185_2017_0 [ 2 ] = ( _rtP -> P_1122 *
B_185_613_0_idx_2 + _rtP -> P_1123 * B_185_615_0_idx_2 ) + _rtP -> P_1126 *
_rtB -> B_185_611_0 ; _rtB -> B_185_2017_0 [ 0 ] = ( _rtP -> P_1122 *
B_185_613_0_idx_0 + _rtP -> P_1123 * B_185_615_0_idx_0 ) + _rtP -> P_1126 *
_rtB -> B_185_610_0 [ 0 ] ; _rtB -> B_185_2019_0 [ 0 ] = ( _rtP -> P_1124 *
B_185_613_0_idx_0 + _rtP -> P_1125 * B_185_615_0_idx_0 ) + _rtP -> P_1127 *
_rtB -> B_185_610_0 [ 0 ] ; _rtB -> B_185_2017_0 [ 1 ] = ( _rtP -> P_1122 *
B_185_613_0_idx_1 + _rtP -> P_1123 * B_185_615_0_idx_1 ) + _rtP -> P_1126 *
_rtB -> B_185_610_0 [ 1 ] ; _rtB -> B_185_2019_0 [ 1 ] = ( _rtP -> P_1124 *
B_185_613_0_idx_1 + _rtP -> P_1125 * B_185_615_0_idx_1 ) + _rtP -> P_1127 *
_rtB -> B_185_610_0 [ 1 ] ; _rtB -> B_185_2019_0 [ 2 ] = ( _rtP -> P_1124 *
B_185_613_0_idx_2 + _rtP -> P_1125 * B_185_615_0_idx_2 ) + _rtP -> P_1127 *
_rtB -> B_185_611_0 ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0
) { _rtB -> B_185_2026_0 = _rtP -> P_1128 * _rtB -> B_185_926_0 [ 229 ] ;
_rtB -> B_185_2030_0 = _rtP -> P_1129 * _rtB -> B_185_926_0 [ 230 ] ;
B_185_722_0_idx_0 *= _rtDW -> UnitDelay1_DSTATE_f2 [ 0 ] ; rtb_B_119_7_0 *=
_rtDW -> UnitDelay1_DSTATE_d1 [ 0 ] ; _rtB -> B_185_2037_0 = (
B_185_722_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_f2 [ 1 ] + B_185_722_0_idx_0 )
* _rtP -> P_1131 ; if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_cp != 0
) { _rtB -> B_185_2038_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_mx ; } else
{ _rtB -> B_185_2038_0 = _rtP -> P_1132 * _rtB -> B_185_2037_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_mx ; } _rtB -> B_185_2055_0 = _rtP -> P_1134 *
_rtB -> B_185_926_0 [ 231 ] ; _rtB -> B_185_2059_0 = _rtP -> P_1135 * _rtB ->
B_185_926_0 [ 232 ] ; rtb_B_115_7_0 *= _rtDW -> UnitDelay1_DSTATE_p2 [ 0 ] ;
_rtB -> B_185_2066_0 = ( B_185_735_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_d1 [
1 ] + rtb_B_119_7_0 ) * _rtP -> P_1137 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_md != 0 ) { _rtB -> B_185_2067_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_m3 ; } else { _rtB -> B_185_2067_0 = _rtP ->
P_1138 * _rtB -> B_185_2066_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_m3 ; }
_rtB -> B_185_2084_0 = _rtP -> P_1140 * _rtB -> B_185_926_0 [ 233 ] ; _rtB ->
B_185_2088_0 = _rtP -> P_1141 * _rtB -> B_185_926_0 [ 234 ] ; rtb_B_118_7_0
*= _rtDW -> UnitDelay1_DSTATE_l4 [ 0 ] ; _rtB -> B_185_2095_0 = (
B_185_748_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_p2 [ 1 ] + rtb_B_115_7_0 ) *
_rtP -> P_1143 ; if ( _rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_iy != 0 )
{ _rtB -> B_185_2096_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_pv ; } else {
_rtB -> B_185_2096_0 = _rtP -> P_1144 * _rtB -> B_185_2095_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_pv ; } _rtB -> B_185_2113_0 = _rtP -> P_1146 *
_rtB -> B_185_926_0 [ 235 ] ; _rtB -> B_185_2117_0 = _rtP -> P_1147 * _rtB ->
B_185_926_0 [ 236 ] ; rtb_B_116_7_0 *= _rtDW -> UnitDelay1_DSTATE_m [ 0 ] ;
_rtB -> B_185_2124_0 = ( B_185_761_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_l4 [
1 ] + rtb_B_118_7_0 ) * _rtP -> P_1149 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_p != 0 ) { _rtB -> B_185_2125_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_c2 ; } else { _rtB -> B_185_2125_0 = _rtP ->
P_1150 * _rtB -> B_185_2124_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_c2 ; }
_rtB -> B_185_2142_0 = _rtP -> P_1152 * _rtB -> B_185_926_0 [ 237 ] ; _rtB ->
B_185_2146_0 = _rtP -> P_1153 * _rtB -> B_185_926_0 [ 238 ] ;
B_185_787_0_idx_0 *= _rtDW -> UnitDelay1_DSTATE_c [ 0 ] ; _rtB ->
B_185_2153_0 = ( B_185_774_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_m [ 1 ] +
rtb_B_116_7_0 ) * _rtP -> P_1155 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_fi != 0 ) { _rtB -> B_185_2154_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_er ; } else { _rtB -> B_185_2154_0 = _rtP ->
P_1156 * _rtB -> B_185_2153_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_er ; }
_rtB -> B_185_2171_0 = _rtP -> P_1158 * _rtB -> B_185_926_0 [ 239 ] ; _rtB ->
B_185_2175_0 = _rtP -> P_1159 * _rtB -> B_185_926_0 [ 240 ] ; _rtB ->
B_185_2182_0 = ( B_185_787_0_idx_1 * _rtDW -> UnitDelay1_DSTATE_c [ 1 ] +
B_185_787_0_idx_0 ) * _rtP -> P_1161 ; if ( _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_hj != 0 ) { _rtB -> B_185_2183_0 = _rtDW
-> DiscreteTimeIntegrator_DSTATE_j2 ; } else { _rtB -> B_185_2183_0 = _rtP ->
P_1162 * _rtB -> B_185_2182_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_j2 ; }
} _rtB -> B_185_2284_0 [ 0 ] = _rtX -> integrator_CSTATE_b5 [ 0 ] ; _rtB ->
B_185_2284_0 [ 1 ] = _rtX -> integrator_CSTATE_b5 [ 1 ] ; _rtB ->
B_185_2284_0 [ 2 ] = _rtX -> integrator_CSTATE_b5 [ 2 ] ; { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ]
; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay ; {
int_T i1 ; real_T * y0 = & B_185_2285_0 [ 0 ] ; int_T * iw_Tail = & _rtDW ->
TransportDelay_IWORK . Tail [ 0 ] ; int_T * iw_Head = & _rtDW ->
TransportDelay_IWORK . Head [ 0 ] ; int_T * iw_Last = & _rtDW ->
TransportDelay_IWORK . Last [ 0 ] ; int_T * iw_CircularBufSize = & _rtDW ->
TransportDelay_IWORK . CircularBufSize [ 0 ] ; for ( i1 = 0 ; i1 < 3 ; i1 ++
) { tMinusDelay = ( ( _rtP -> P_1165 > 0.0 ) ? _rtP -> P_1165 : 0.0 ) ;
tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ] =
normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1166 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2287_0 = _rtP -> P_1167 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2287_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2289_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_b [ 0 ] ; _rtB -> B_185_2289_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_b [ 1 ] ; _rtB -> B_185_2289_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_b [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2291_0 [ 0 ] = ( _rtB -> B_185_2284_0 [ 0 ] - B_185_2285_0 [ 0 ] ) *
_rtP -> P_94 ; _rtB -> B_185_2291_0 [ 1 ] = ( _rtB -> B_185_2284_0 [ 1 ] -
B_185_2285_0 [ 1 ] ) * _rtP -> P_94 ; _rtB -> B_185_2291_0 [ 2 ] = ( _rtB ->
B_185_2284_0 [ 2 ] - B_185_2285_0 [ 2 ] ) * _rtP -> P_94 ; } else { _rtB ->
B_185_2291_0 [ 0 ] = _rtB -> B_185_2289_0 [ 0 ] ; _rtB -> B_185_2291_0 [ 1 ]
= _rtB -> B_185_2289_0 [ 1 ] ; _rtB -> B_185_2291_0 [ 2 ] = _rtB ->
B_185_2289_0 [ 2 ] ; } _rtB -> B_185_2292_0 [ 0 ] = _rtX ->
integrator_CSTATE_l [ 0 ] ; _rtB -> B_185_2292_0 [ 1 ] = _rtX ->
integrator_CSTATE_l [ 1 ] ; _rtB -> B_185_2292_0 [ 2 ] = _rtX ->
integrator_CSTATE_l [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_i . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_i . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & B_185_2293_0
[ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_p . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_p . Head [ 0 ] ; int_T *
iw_Last = & _rtDW -> TransportDelay_IWORK_p . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_p . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1170 > 0.0
) ? _rtP -> P_1170 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1171 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2295_0 = _rtP -> P_1172 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2295_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2297_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_br [ 0 ] ; _rtB -> B_185_2297_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_br [ 1 ] ; _rtB -> B_185_2297_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_br [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2299_0 [ 0 ] = ( _rtB -> B_185_2292_0 [ 0 ] - B_185_2293_0 [ 0 ] ) *
_rtP -> P_93 ; _rtB -> B_185_2299_0 [ 1 ] = ( _rtB -> B_185_2292_0 [ 1 ] -
B_185_2293_0 [ 1 ] ) * _rtP -> P_93 ; _rtB -> B_185_2299_0 [ 2 ] = ( _rtB ->
B_185_2292_0 [ 2 ] - B_185_2293_0 [ 2 ] ) * _rtP -> P_93 ; } else { _rtB ->
B_185_2299_0 [ 0 ] = _rtB -> B_185_2297_0 [ 0 ] ; _rtB -> B_185_2299_0 [ 1 ]
= _rtB -> B_185_2297_0 [ 1 ] ; _rtB -> B_185_2299_0 [ 2 ] = _rtB ->
B_185_2297_0 [ 2 ] ; } rtb_B_185_124_0 [ 0 ] = muDoubleScalarHypot ( _rtB ->
B_185_2291_0 [ 0 ] , _rtB -> B_185_2299_0 [ 0 ] ) ; rtb_B_185_2_0 [ 0 ] =
muDoubleScalarAtan2 ( _rtB -> B_185_2299_0 [ 0 ] , _rtB -> B_185_2291_0 [ 0 ]
) ; _rtB -> B_185_2302_0 [ 0 ] = _rtX -> integrator_CSTATE_n [ 0 ] ;
rtb_B_185_124_0 [ 1 ] = muDoubleScalarHypot ( _rtB -> B_185_2291_0 [ 1 ] ,
_rtB -> B_185_2299_0 [ 1 ] ) ; rtb_B_185_2_0 [ 1 ] = muDoubleScalarAtan2 (
_rtB -> B_185_2299_0 [ 1 ] , _rtB -> B_185_2291_0 [ 1 ] ) ; _rtB ->
B_185_2302_0 [ 1 ] = _rtX -> integrator_CSTATE_n [ 1 ] ; rtb_B_185_124_0 [ 2
] = muDoubleScalarHypot ( _rtB -> B_185_2291_0 [ 2 ] , _rtB -> B_185_2299_0 [
2 ] ) ; rtb_B_185_2_0 [ 2 ] = muDoubleScalarAtan2 ( _rtB -> B_185_2299_0 [ 2
] , _rtB -> B_185_2291_0 [ 2 ] ) ; _rtB -> B_185_2302_0 [ 2 ] = _rtX ->
integrator_CSTATE_n [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_p . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_p . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & B_185_2303_0
[ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_i . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_i . Head [ 0 ] ; int_T *
iw_Last = & _rtDW -> TransportDelay_IWORK_i . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_i . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1175 > 0.0
) ? _rtP -> P_1175 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1176 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2305_0 = _rtP -> P_1177 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2305_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2307_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_e [ 0 ] ; _rtB -> B_185_2307_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_e [ 1 ] ; _rtB -> B_185_2307_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_e [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2309_0 [ 0 ] = ( _rtB -> B_185_2302_0 [ 0 ] - B_185_2303_0 [ 0 ] ) *
_rtP -> P_96 ; _rtB -> B_185_2309_0 [ 1 ] = ( _rtB -> B_185_2302_0 [ 1 ] -
B_185_2303_0 [ 1 ] ) * _rtP -> P_96 ; _rtB -> B_185_2309_0 [ 2 ] = ( _rtB ->
B_185_2302_0 [ 2 ] - B_185_2303_0 [ 2 ] ) * _rtP -> P_96 ; } else { _rtB ->
B_185_2309_0 [ 0 ] = _rtB -> B_185_2307_0 [ 0 ] ; _rtB -> B_185_2309_0 [ 1 ]
= _rtB -> B_185_2307_0 [ 1 ] ; _rtB -> B_185_2309_0 [ 2 ] = _rtB ->
B_185_2307_0 [ 2 ] ; } _rtB -> B_185_2310_0 [ 0 ] = _rtX ->
integrator_CSTATE_i [ 0 ] ; _rtB -> B_185_2310_0 [ 1 ] = _rtX ->
integrator_CSTATE_i [ 1 ] ; _rtB -> B_185_2310_0 [ 2 ] = _rtX ->
integrator_CSTATE_i [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_o . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_o . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & B_185_2311_0
[ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_m . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_m . Head [ 0 ] ; int_T *
iw_Last = & _rtDW -> TransportDelay_IWORK_m . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_m . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1180 > 0.0
) ? _rtP -> P_1180 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1181 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2313_0 = _rtP -> P_1182 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2313_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2315_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_j [ 0 ] ; _rtB -> B_185_2315_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_j [ 1 ] ; _rtB -> B_185_2315_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_j [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2317_0 [ 0 ] = ( _rtB -> B_185_2310_0 [ 0 ] - B_185_2311_0 [ 0 ] ) *
_rtP -> P_95 ; _rtB -> B_185_2317_0 [ 1 ] = ( _rtB -> B_185_2310_0 [ 1 ] -
B_185_2311_0 [ 1 ] ) * _rtP -> P_95 ; _rtB -> B_185_2317_0 [ 2 ] = ( _rtB ->
B_185_2310_0 [ 2 ] - B_185_2311_0 [ 2 ] ) * _rtP -> P_95 ; } else { _rtB ->
B_185_2317_0 [ 0 ] = _rtB -> B_185_2315_0 [ 0 ] ; _rtB -> B_185_2317_0 [ 1 ]
= _rtB -> B_185_2315_0 [ 1 ] ; _rtB -> B_185_2317_0 [ 2 ] = _rtB ->
B_185_2315_0 [ 2 ] ; } rtb_B_185_124_0 [ 0 ] = rtb_B_185_124_0 [ 0 ] *
muDoubleScalarHypot ( _rtB -> B_185_2309_0 [ 0 ] , _rtB -> B_185_2317_0 [ 0 ]
) * _rtP -> P_1184 ; rtb_B_185_2_0 [ 0 ] = ( _rtP -> P_1185 * rtb_B_185_2_0 [
0 ] - _rtP -> P_1186 * muDoubleScalarAtan2 ( _rtB -> B_185_2317_0 [ 0 ] ,
_rtB -> B_185_2309_0 [ 0 ] ) ) * _rtP -> P_1187 ; rtb_B_185_124_0 [ 1 ] =
rtb_B_185_124_0 [ 1 ] * muDoubleScalarHypot ( _rtB -> B_185_2309_0 [ 1 ] ,
_rtB -> B_185_2317_0 [ 1 ] ) * _rtP -> P_1184 ; rtb_B_185_2_0 [ 1 ] = ( _rtP
-> P_1185 * rtb_B_185_2_0 [ 1 ] - _rtP -> P_1186 * muDoubleScalarAtan2 ( _rtB
-> B_185_2317_0 [ 1 ] , _rtB -> B_185_2309_0 [ 1 ] ) ) * _rtP -> P_1187 ;
rtb_B_185_124_0 [ 2 ] = rtb_B_185_124_0 [ 2 ] * muDoubleScalarHypot ( _rtB ->
B_185_2309_0 [ 2 ] , _rtB -> B_185_2317_0 [ 2 ] ) * _rtP -> P_1184 ;
rtb_B_185_2_0 [ 2 ] = ( _rtP -> P_1185 * rtb_B_185_2_0 [ 2 ] - _rtP -> P_1186
* muDoubleScalarAtan2 ( _rtB -> B_185_2317_0 [ 2 ] , _rtB -> B_185_2309_0 [ 2
] ) ) * _rtP -> P_1187 ; muDoubleScalarSinCos ( rtb_B_185_2_0 [ 0 ] , &
B_185_23_0_idx_1 , & B_185_23_0_idx_0 ) ; muDoubleScalarSinCos (
rtb_B_185_2_0 [ 1 ] , & B_185_25_0_idx_1 , & B_185_23_0_idx_1 ) ;
muDoubleScalarSinCos ( rtb_B_185_2_0 [ 2 ] , & B_185_25_0_idx_1 , &
B_185_25_0_idx_0 ) ; rtb_B_185_124_0 [ 0 ] *= B_185_23_0_idx_0 ;
rtb_B_185_124_0 [ 1 ] *= B_185_23_0_idx_1 ; _rtB -> B_185_2328_0 = (
rtb_B_185_124_0 [ 0 ] + rtb_B_185_124_0 [ 1 ] ) + rtb_B_185_124_0 [ 2 ] *
B_185_25_0_idx_0 ; ssCallAccelRunBlock ( S , 185 , 2329 , SS_CALL_MDL_OUTPUTS
) ; B_185_23_0_idx_0 = muDoubleScalarSin ( _rtP -> P_1190 * ssGetTaskTime ( S
, 0 ) + _rtP -> P_1191 ) * _rtP -> P_1188 + _rtP -> P_1189 ; _rtB ->
B_185_2331_0 [ 0 ] = _rtB -> B_185_1368_0 [ 0 ] * B_185_23_0_idx_0 ; _rtB ->
B_185_2331_0 [ 1 ] = _rtB -> B_185_1368_0 [ 1 ] * B_185_23_0_idx_0 ; _rtB ->
B_185_2331_0 [ 2 ] = _rtB -> B_185_1368_0 [ 2 ] * B_185_23_0_idx_0 ;
B_185_23_0_idx_0 = muDoubleScalarSin ( _rtP -> P_1194 * ssGetTaskTime ( S , 0
) + _rtP -> P_1195 ) * _rtP -> P_1192 + _rtP -> P_1193 ; _rtB -> B_185_2333_0
[ 0 ] = _rtB -> B_185_1368_0 [ 0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2333_0
[ 1 ] = _rtB -> B_185_1368_0 [ 1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2333_0
[ 2 ] = _rtB -> B_185_1368_0 [ 2 ] * B_185_23_0_idx_0 ; isHit = ssIsSampleHit
( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_2337_0 [ 0 ] = _rtP ->
P_1196 * _rtB -> B_185_926_0 [ 138 ] * _rtP -> P_1199 ; _rtB -> B_185_2337_0
[ 1 ] = _rtP -> P_1197 * _rtB -> B_185_926_0 [ 139 ] * _rtP -> P_1199 ; _rtB
-> B_185_2337_0 [ 2 ] = _rtP -> P_1198 * _rtB -> B_185_926_0 [ 140 ] * _rtP
-> P_1199 ; } B_185_23_0_idx_0 = muDoubleScalarSin ( _rtP -> P_1202 *
ssGetTaskTime ( S , 0 ) + _rtP -> P_1203 ) * _rtP -> P_1200 + _rtP -> P_1201
; _rtB -> B_185_2339_0 [ 0 ] = _rtB -> B_185_2337_0 [ 0 ] * B_185_23_0_idx_0
; _rtB -> B_185_2339_0 [ 1 ] = _rtB -> B_185_2337_0 [ 1 ] * B_185_23_0_idx_0
; _rtB -> B_185_2339_0 [ 2 ] = _rtB -> B_185_2337_0 [ 2 ] * B_185_23_0_idx_0
; B_185_23_0_idx_0 = muDoubleScalarSin ( _rtP -> P_1206 * ssGetTaskTime ( S ,
0 ) + _rtP -> P_1207 ) * _rtP -> P_1204 + _rtP -> P_1205 ; _rtB ->
B_185_2341_0 [ 0 ] = _rtB -> B_185_2337_0 [ 0 ] * B_185_23_0_idx_0 ; _rtB ->
B_185_2342_0 [ 0 ] = _rtX -> integrator_CSTATE_k [ 0 ] ; _rtB -> B_185_2341_0
[ 1 ] = _rtB -> B_185_2337_0 [ 1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2342_0
[ 1 ] = _rtX -> integrator_CSTATE_k [ 1 ] ; _rtB -> B_185_2341_0 [ 2 ] = _rtB
-> B_185_2337_0 [ 2 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2342_0 [ 2 ] = _rtX
-> integrator_CSTATE_k [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW
-> TransportDelay_PWORK_a . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK_a . TUbufferPtrs [ 3 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = &
B_185_2343_0 [ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_a .
Tail [ 0 ] ; int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_a . Head [ 0 ]
; int_T * iw_Last = & _rtDW -> TransportDelay_IWORK_a . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_a . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1209 > 0.0
) ? _rtP -> P_1209 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1210 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2345_0 = _rtP -> P_1211 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2345_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2347_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_k [ 0 ] ; _rtB -> B_185_2347_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_k [ 1 ] ; _rtB -> B_185_2347_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_k [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2349_0 [ 0 ] = ( _rtB -> B_185_2342_0 [ 0 ] - B_185_2343_0 [ 0 ] ) *
_rtP -> P_98 ; _rtB -> B_185_2349_0 [ 1 ] = ( _rtB -> B_185_2342_0 [ 1 ] -
B_185_2343_0 [ 1 ] ) * _rtP -> P_98 ; _rtB -> B_185_2349_0 [ 2 ] = ( _rtB ->
B_185_2342_0 [ 2 ] - B_185_2343_0 [ 2 ] ) * _rtP -> P_98 ; } else { _rtB ->
B_185_2349_0 [ 0 ] = _rtB -> B_185_2347_0 [ 0 ] ; _rtB -> B_185_2349_0 [ 1 ]
= _rtB -> B_185_2347_0 [ 1 ] ; _rtB -> B_185_2349_0 [ 2 ] = _rtB ->
B_185_2347_0 [ 2 ] ; } _rtB -> B_185_2350_0 [ 0 ] = _rtX ->
integrator_CSTATE_lw [ 0 ] ; _rtB -> B_185_2350_0 [ 1 ] = _rtX ->
integrator_CSTATE_lw [ 1 ] ; _rtB -> B_185_2350_0 [ 2 ] = _rtX ->
integrator_CSTATE_lw [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_j . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_j . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & B_185_2351_0
[ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_e . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_e . Head [ 0 ] ; int_T *
iw_Last = & _rtDW -> TransportDelay_IWORK_e . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_e . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1214 > 0.0
) ? _rtP -> P_1214 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1215 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2353_0 = _rtP -> P_1216 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2353_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2355_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_a [ 0 ] ; _rtB -> B_185_2355_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_a [ 1 ] ; _rtB -> B_185_2355_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_a [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2357_0 [ 0 ] = ( _rtB -> B_185_2350_0 [ 0 ] - B_185_2351_0 [ 0 ] ) *
_rtP -> P_97 ; _rtB -> B_185_2357_0 [ 1 ] = ( _rtB -> B_185_2350_0 [ 1 ] -
B_185_2351_0 [ 1 ] ) * _rtP -> P_97 ; _rtB -> B_185_2357_0 [ 2 ] = ( _rtB ->
B_185_2350_0 [ 2 ] - B_185_2351_0 [ 2 ] ) * _rtP -> P_97 ; } else { _rtB ->
B_185_2357_0 [ 0 ] = _rtB -> B_185_2355_0 [ 0 ] ; _rtB -> B_185_2357_0 [ 1 ]
= _rtB -> B_185_2355_0 [ 1 ] ; _rtB -> B_185_2357_0 [ 2 ] = _rtB ->
B_185_2355_0 [ 2 ] ; } B_185_23_0_idx_0 = muDoubleScalarHypot ( _rtB ->
B_185_2349_0 [ 0 ] , _rtB -> B_185_2357_0 [ 0 ] ) ; B_185_25_0_idx_0 =
muDoubleScalarAtan2 ( _rtB -> B_185_2357_0 [ 0 ] , _rtB -> B_185_2349_0 [ 0 ]
) ; _rtB -> B_185_2360_0 [ 0 ] = _rtX -> integrator_CSTATE_d [ 0 ] ;
B_185_23_0_idx_1 = muDoubleScalarHypot ( _rtB -> B_185_2349_0 [ 1 ] , _rtB ->
B_185_2357_0 [ 1 ] ) ; B_185_25_0_idx_1 = muDoubleScalarAtan2 ( _rtB ->
B_185_2357_0 [ 1 ] , _rtB -> B_185_2349_0 [ 1 ] ) ; _rtB -> B_185_2360_0 [ 1
] = _rtX -> integrator_CSTATE_d [ 1 ] ; B_185_23_0_idx_2 =
muDoubleScalarHypot ( _rtB -> B_185_2349_0 [ 2 ] , _rtB -> B_185_2357_0 [ 2 ]
) ; B_185_25_0_idx_2 = muDoubleScalarAtan2 ( _rtB -> B_185_2357_0 [ 2 ] ,
_rtB -> B_185_2349_0 [ 2 ] ) ; _rtB -> B_185_2360_0 [ 2 ] = _rtX ->
integrator_CSTATE_d [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_f . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_f . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & B_185_2361_0
[ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_l . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_l . Head [ 0 ] ; int_T *
iw_Last = & _rtDW -> TransportDelay_IWORK_l . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_l . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1219 > 0.0
) ? _rtP -> P_1219 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1220 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2363_0 = _rtP -> P_1221 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2363_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2365_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_cv [ 0 ] ; _rtB -> B_185_2365_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_cv [ 1 ] ; _rtB -> B_185_2365_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_cv [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2367_0 [ 0 ] = ( _rtB -> B_185_2360_0 [ 0 ] - B_185_2361_0 [ 0 ] ) *
_rtP -> P_100 ; _rtB -> B_185_2367_0 [ 1 ] = ( _rtB -> B_185_2360_0 [ 1 ] -
B_185_2361_0 [ 1 ] ) * _rtP -> P_100 ; _rtB -> B_185_2367_0 [ 2 ] = ( _rtB ->
B_185_2360_0 [ 2 ] - B_185_2361_0 [ 2 ] ) * _rtP -> P_100 ; } else { _rtB ->
B_185_2367_0 [ 0 ] = _rtB -> B_185_2365_0 [ 0 ] ; _rtB -> B_185_2367_0 [ 1 ]
= _rtB -> B_185_2365_0 [ 1 ] ; _rtB -> B_185_2367_0 [ 2 ] = _rtB ->
B_185_2365_0 [ 2 ] ; } _rtB -> B_185_2368_0 [ 0 ] = _rtX ->
integrator_CSTATE_mn [ 0 ] ; _rtB -> B_185_2368_0 [ 1 ] = _rtX ->
integrator_CSTATE_mn [ 1 ] ; _rtB -> B_185_2368_0 [ 2 ] = _rtX ->
integrator_CSTATE_mn [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_n . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_n . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & B_185_2369_0
[ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_ef . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_ef . Head [ 0 ] ; int_T *
iw_Last = & _rtDW -> TransportDelay_IWORK_ef . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_ef . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1224 > 0.0
) ? _rtP -> P_1224 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1225 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2371_0 = _rtP -> P_1226 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2371_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2373_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_p [ 0 ] ; _rtB -> B_185_2373_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_p [ 1 ] ; _rtB -> B_185_2373_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_p [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2375_0 [ 0 ] = ( _rtB -> B_185_2368_0 [ 0 ] - B_185_2369_0 [ 0 ] ) *
_rtP -> P_99 ; _rtB -> B_185_2375_0 [ 1 ] = ( _rtB -> B_185_2368_0 [ 1 ] -
B_185_2369_0 [ 1 ] ) * _rtP -> P_99 ; _rtB -> B_185_2375_0 [ 2 ] = ( _rtB ->
B_185_2368_0 [ 2 ] - B_185_2369_0 [ 2 ] ) * _rtP -> P_99 ; } else { _rtB ->
B_185_2375_0 [ 0 ] = _rtB -> B_185_2373_0 [ 0 ] ; _rtB -> B_185_2375_0 [ 1 ]
= _rtB -> B_185_2373_0 [ 1 ] ; _rtB -> B_185_2375_0 [ 2 ] = _rtB ->
B_185_2373_0 [ 2 ] ; } B_185_23_0_idx_0 = B_185_23_0_idx_0 *
muDoubleScalarHypot ( _rtB -> B_185_2367_0 [ 0 ] , _rtB -> B_185_2375_0 [ 0 ]
) * _rtP -> P_1228 ; B_185_25_0_idx_0 = ( _rtP -> P_1229 * B_185_25_0_idx_0 -
_rtP -> P_1230 * muDoubleScalarAtan2 ( _rtB -> B_185_2375_0 [ 0 ] , _rtB ->
B_185_2367_0 [ 0 ] ) ) * _rtP -> P_1231 ; B_185_23_0_idx_1 = B_185_23_0_idx_1
* muDoubleScalarHypot ( _rtB -> B_185_2367_0 [ 1 ] , _rtB -> B_185_2375_0 [ 1
] ) * _rtP -> P_1228 ; B_185_25_0_idx_1 = ( _rtP -> P_1229 * B_185_25_0_idx_1
- _rtP -> P_1230 * muDoubleScalarAtan2 ( _rtB -> B_185_2375_0 [ 1 ] , _rtB ->
B_185_2367_0 [ 1 ] ) ) * _rtP -> P_1231 ; rtb_B_185_23_0 = B_185_23_0_idx_2 *
muDoubleScalarHypot ( _rtB -> B_185_2367_0 [ 2 ] , _rtB -> B_185_2375_0 [ 2 ]
) * _rtP -> P_1228 ; rtb_B_185_25_0 = ( _rtP -> P_1229 * B_185_25_0_idx_2 -
_rtP -> P_1230 * muDoubleScalarAtan2 ( _rtB -> B_185_2375_0 [ 2 ] , _rtB ->
B_185_2367_0 [ 2 ] ) ) * _rtP -> P_1231 ; muDoubleScalarSinCos (
B_185_25_0_idx_0 , & B_185_2384_0_idx_0 , & B_185_2384_1_idx_0 ) ;
muDoubleScalarSinCos ( B_185_25_0_idx_1 , & B_185_2384_0_idx_1 , &
B_185_2384_1_idx_1 ) ; muDoubleScalarSinCos ( rtb_B_185_25_0 , &
B_185_25_0_idx_1 , & B_185_2384_1_idx_2 ) ; B_185_23_0_idx_0 *=
B_185_2384_1_idx_0 ; B_185_23_0_idx_1 *= B_185_2384_1_idx_1 ; _rtB ->
B_185_2386_0 = ( B_185_23_0_idx_0 + B_185_23_0_idx_1 ) + rtb_B_185_23_0 *
B_185_2384_1_idx_2 ; ssCallAccelRunBlock ( S , 185 , 2387 ,
SS_CALL_MDL_OUTPUTS ) ; B_185_23_0_idx_0 = muDoubleScalarSin ( _rtP -> P_1234
* ssGetTaskTime ( S , 0 ) + _rtP -> P_1235 ) * _rtP -> P_1232 + _rtP ->
P_1233 ; _rtB -> B_185_2389_0 [ 0 ] = _rtB -> B_185_1664_0 [ 0 ] *
B_185_23_0_idx_0 ; _rtB -> B_185_2389_0 [ 1 ] = _rtB -> B_185_1664_0 [ 1 ] *
B_185_23_0_idx_0 ; _rtB -> B_185_2389_0 [ 2 ] = _rtB -> B_185_1664_0 [ 2 ] *
B_185_23_0_idx_0 ; B_185_23_0_idx_0 = muDoubleScalarSin ( _rtP -> P_1238 *
ssGetTaskTime ( S , 0 ) + _rtP -> P_1239 ) * _rtP -> P_1236 + _rtP -> P_1237
; _rtB -> B_185_2391_0 [ 0 ] = _rtB -> B_185_1664_0 [ 0 ] * B_185_23_0_idx_0
; _rtB -> B_185_2391_0 [ 1 ] = _rtB -> B_185_1664_0 [ 1 ] * B_185_23_0_idx_0
; _rtB -> B_185_2391_0 [ 2 ] = _rtB -> B_185_1664_0 [ 2 ] * B_185_23_0_idx_0
; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_185_2395_0 [ 0 ] = _rtP -> P_1240 * _rtB -> B_185_926_0 [ 141 ] * _rtP ->
P_1243 ; _rtB -> B_185_2395_0 [ 1 ] = _rtP -> P_1241 * _rtB -> B_185_926_0 [
142 ] * _rtP -> P_1243 ; _rtB -> B_185_2395_0 [ 2 ] = _rtP -> P_1242 * _rtB
-> B_185_926_0 [ 143 ] * _rtP -> P_1243 ; } B_185_23_0_idx_0 =
muDoubleScalarSin ( _rtP -> P_1246 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1247
) * _rtP -> P_1244 + _rtP -> P_1245 ; _rtB -> B_185_2397_0 [ 0 ] = _rtB ->
B_185_2395_0 [ 0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2397_0 [ 1 ] = _rtB ->
B_185_2395_0 [ 1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2397_0 [ 2 ] = _rtB ->
B_185_2395_0 [ 2 ] * B_185_23_0_idx_0 ; B_185_23_0_idx_0 = muDoubleScalarSin
( _rtP -> P_1250 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1251 ) * _rtP ->
P_1248 + _rtP -> P_1249 ; _rtB -> B_185_2399_0 [ 0 ] = _rtB -> B_185_2395_0 [
0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2400_0 [ 0 ] = _rtX ->
integrator_CSTATE_p [ 0 ] ; _rtB -> B_185_2399_0 [ 1 ] = _rtB -> B_185_2395_0
[ 1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2400_0 [ 1 ] = _rtX ->
integrator_CSTATE_p [ 1 ] ; _rtB -> B_185_2399_0 [ 2 ] = _rtB -> B_185_2395_0
[ 2 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2400_0 [ 2 ] = _rtX ->
integrator_CSTATE_p [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_ju . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay_PWORK_ju . TUbufferPtrs [ 3 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = &
B_185_2401_0 [ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_as .
Tail [ 0 ] ; int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_as . Head [ 0
] ; int_T * iw_Last = & _rtDW -> TransportDelay_IWORK_as . Last [ 0 ] ; int_T
* iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_as . CircularBufSize [
0 ] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1253 >
0.0 ) ? _rtP -> P_1253 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [
i1 ] = normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay ,
0.0 , * tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] ,
iw_Tail [ i1 ] , iw_Head [ i1 ] , _rtP -> P_1254 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2403_0 = _rtP -> P_1255 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2403_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2405_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_p1 [ 0 ] ; _rtB -> B_185_2405_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_p1 [ 1 ] ; _rtB -> B_185_2405_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_p1 [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2407_0 [ 0 ] = ( _rtB -> B_185_2400_0 [ 0 ] - B_185_2401_0 [ 0 ] ) *
_rtP -> P_102 ; _rtB -> B_185_2407_0 [ 1 ] = ( _rtB -> B_185_2400_0 [ 1 ] -
B_185_2401_0 [ 1 ] ) * _rtP -> P_102 ; _rtB -> B_185_2407_0 [ 2 ] = ( _rtB ->
B_185_2400_0 [ 2 ] - B_185_2401_0 [ 2 ] ) * _rtP -> P_102 ; } else { _rtB ->
B_185_2407_0 [ 0 ] = _rtB -> B_185_2405_0 [ 0 ] ; _rtB -> B_185_2407_0 [ 1 ]
= _rtB -> B_185_2405_0 [ 1 ] ; _rtB -> B_185_2407_0 [ 2 ] = _rtB ->
B_185_2405_0 [ 2 ] ; } _rtB -> B_185_2408_0 [ 0 ] = _rtX ->
integrator_CSTATE_a [ 0 ] ; _rtB -> B_185_2408_0 [ 1 ] = _rtX ->
integrator_CSTATE_a [ 1 ] ; _rtB -> B_185_2408_0 [ 2 ] = _rtX ->
integrator_CSTATE_a [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_g . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_g . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & B_185_2409_0
[ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_n . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_n . Head [ 0 ] ; int_T *
iw_Last = & _rtDW -> TransportDelay_IWORK_n . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_n . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1258 > 0.0
) ? _rtP -> P_1258 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1259 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2411_0 = _rtP -> P_1260 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2411_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2413_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_g [ 0 ] ; _rtB -> B_185_2413_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_g [ 1 ] ; _rtB -> B_185_2413_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_g [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2415_0 [ 0 ] = ( _rtB -> B_185_2408_0 [ 0 ] - B_185_2409_0 [ 0 ] ) *
_rtP -> P_101 ; _rtB -> B_185_2415_0 [ 1 ] = ( _rtB -> B_185_2408_0 [ 1 ] -
B_185_2409_0 [ 1 ] ) * _rtP -> P_101 ; _rtB -> B_185_2415_0 [ 2 ] = ( _rtB ->
B_185_2408_0 [ 2 ] - B_185_2409_0 [ 2 ] ) * _rtP -> P_101 ; } else { _rtB ->
B_185_2415_0 [ 0 ] = _rtB -> B_185_2413_0 [ 0 ] ; _rtB -> B_185_2415_0 [ 1 ]
= _rtB -> B_185_2413_0 [ 1 ] ; _rtB -> B_185_2415_0 [ 2 ] = _rtB ->
B_185_2413_0 [ 2 ] ; } B_185_23_0_idx_0 = muDoubleScalarHypot ( _rtB ->
B_185_2407_0 [ 0 ] , _rtB -> B_185_2415_0 [ 0 ] ) ; B_185_25_0_idx_0 =
muDoubleScalarAtan2 ( _rtB -> B_185_2415_0 [ 0 ] , _rtB -> B_185_2407_0 [ 0 ]
) ; _rtB -> B_185_2418_0 [ 0 ] = _rtX -> integrator_CSTATE_j [ 0 ] ;
B_185_23_0_idx_1 = muDoubleScalarHypot ( _rtB -> B_185_2407_0 [ 1 ] , _rtB ->
B_185_2415_0 [ 1 ] ) ; B_185_25_0_idx_1 = muDoubleScalarAtan2 ( _rtB ->
B_185_2415_0 [ 1 ] , _rtB -> B_185_2407_0 [ 1 ] ) ; _rtB -> B_185_2418_0 [ 1
] = _rtX -> integrator_CSTATE_j [ 1 ] ; B_185_23_0_idx_2 =
muDoubleScalarHypot ( _rtB -> B_185_2407_0 [ 2 ] , _rtB -> B_185_2415_0 [ 2 ]
) ; B_185_25_0_idx_2 = muDoubleScalarAtan2 ( _rtB -> B_185_2415_0 [ 2 ] ,
_rtB -> B_185_2407_0 [ 2 ] ) ; _rtB -> B_185_2418_0 [ 2 ] = _rtX ->
integrator_CSTATE_j [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_iy . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay_PWORK_iy . TUbufferPtrs [ 3 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = &
B_185_2419_0 [ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_g .
Tail [ 0 ] ; int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_g . Head [ 0 ]
; int_T * iw_Last = & _rtDW -> TransportDelay_IWORK_g . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_g . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1263 > 0.0
) ? _rtP -> P_1263 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1264 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2421_0 = _rtP -> P_1265 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2421_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2423_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_dz [ 0 ] ; _rtB -> B_185_2423_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_dz [ 1 ] ; _rtB -> B_185_2423_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_dz [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2425_0 [ 0 ] = ( _rtB -> B_185_2418_0 [ 0 ] - B_185_2419_0 [ 0 ] ) *
_rtP -> P_104 ; _rtB -> B_185_2425_0 [ 1 ] = ( _rtB -> B_185_2418_0 [ 1 ] -
B_185_2419_0 [ 1 ] ) * _rtP -> P_104 ; _rtB -> B_185_2425_0 [ 2 ] = ( _rtB ->
B_185_2418_0 [ 2 ] - B_185_2419_0 [ 2 ] ) * _rtP -> P_104 ; } else { _rtB ->
B_185_2425_0 [ 0 ] = _rtB -> B_185_2423_0 [ 0 ] ; _rtB -> B_185_2425_0 [ 1 ]
= _rtB -> B_185_2423_0 [ 1 ] ; _rtB -> B_185_2425_0 [ 2 ] = _rtB ->
B_185_2423_0 [ 2 ] ; } _rtB -> B_185_2426_0 [ 0 ] = _rtX ->
integrator_CSTATE_jl [ 0 ] ; _rtB -> B_185_2426_0 [ 1 ] = _rtX ->
integrator_CSTATE_jl [ 1 ] ; _rtB -> B_185_2426_0 [ 2 ] = _rtX ->
integrator_CSTATE_jl [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_i5 . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay_PWORK_i5 . TUbufferPtrs [ 3 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = &
B_185_2427_0 [ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_lh .
Tail [ 0 ] ; int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_lh . Head [ 0
] ; int_T * iw_Last = & _rtDW -> TransportDelay_IWORK_lh . Last [ 0 ] ; int_T
* iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_lh . CircularBufSize [
0 ] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1268 >
0.0 ) ? _rtP -> P_1268 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [
i1 ] = normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay ,
0.0 , * tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] ,
iw_Tail [ i1 ] , iw_Head [ i1 ] , _rtP -> P_1269 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2429_0 = _rtP -> P_1270 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2429_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2431_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_fl [ 0 ] ; _rtB -> B_185_2431_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_fl [ 1 ] ; _rtB -> B_185_2431_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_fl [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2433_0 [ 0 ] = ( _rtB -> B_185_2426_0 [ 0 ] - B_185_2427_0 [ 0 ] ) *
_rtP -> P_103 ; _rtB -> B_185_2433_0 [ 1 ] = ( _rtB -> B_185_2426_0 [ 1 ] -
B_185_2427_0 [ 1 ] ) * _rtP -> P_103 ; _rtB -> B_185_2433_0 [ 2 ] = ( _rtB ->
B_185_2426_0 [ 2 ] - B_185_2427_0 [ 2 ] ) * _rtP -> P_103 ; } else { _rtB ->
B_185_2433_0 [ 0 ] = _rtB -> B_185_2431_0 [ 0 ] ; _rtB -> B_185_2433_0 [ 1 ]
= _rtB -> B_185_2431_0 [ 1 ] ; _rtB -> B_185_2433_0 [ 2 ] = _rtB ->
B_185_2431_0 [ 2 ] ; } B_185_23_0_idx_0 = B_185_23_0_idx_0 *
muDoubleScalarHypot ( _rtB -> B_185_2425_0 [ 0 ] , _rtB -> B_185_2433_0 [ 0 ]
) * _rtP -> P_1272 ; B_185_25_0_idx_0 = ( _rtP -> P_1273 * B_185_25_0_idx_0 -
_rtP -> P_1274 * muDoubleScalarAtan2 ( _rtB -> B_185_2433_0 [ 0 ] , _rtB ->
B_185_2425_0 [ 0 ] ) ) * _rtP -> P_1275 ; B_185_23_0_idx_1 = B_185_23_0_idx_1
* muDoubleScalarHypot ( _rtB -> B_185_2425_0 [ 1 ] , _rtB -> B_185_2433_0 [ 1
] ) * _rtP -> P_1272 ; B_185_25_0_idx_1 = ( _rtP -> P_1273 * B_185_25_0_idx_1
- _rtP -> P_1274 * muDoubleScalarAtan2 ( _rtB -> B_185_2433_0 [ 1 ] , _rtB ->
B_185_2425_0 [ 1 ] ) ) * _rtP -> P_1275 ; rtb_B_185_23_0 = B_185_23_0_idx_2 *
muDoubleScalarHypot ( _rtB -> B_185_2425_0 [ 2 ] , _rtB -> B_185_2433_0 [ 2 ]
) * _rtP -> P_1272 ; rtb_B_185_25_0 = ( _rtP -> P_1273 * B_185_25_0_idx_2 -
_rtP -> P_1274 * muDoubleScalarAtan2 ( _rtB -> B_185_2433_0 [ 2 ] , _rtB ->
B_185_2425_0 [ 2 ] ) ) * _rtP -> P_1275 ; muDoubleScalarSinCos (
B_185_25_0_idx_0 , & B_185_2442_0_idx_0 , & B_185_2442_1_idx_0 ) ;
muDoubleScalarSinCos ( B_185_25_0_idx_1 , & B_185_2442_0_idx_1 , &
B_185_2442_1_idx_1 ) ; muDoubleScalarSinCos ( rtb_B_185_25_0 , &
B_185_2442_0_idx_2 , & B_185_2442_1_idx_2 ) ; B_185_23_0_idx_0 *=
B_185_2442_1_idx_0 ; B_185_23_0_idx_1 *= B_185_2442_1_idx_1 ; _rtB ->
B_185_2444_0 = ( B_185_23_0_idx_0 + B_185_23_0_idx_1 ) + rtb_B_185_23_0 *
B_185_2442_1_idx_2 ; ssCallAccelRunBlock ( S , 185 , 2445 ,
SS_CALL_MDL_OUTPUTS ) ; B_185_23_0_idx_0 = muDoubleScalarSin ( _rtP -> P_1278
* ssGetTaskTime ( S , 0 ) + _rtP -> P_1279 ) * _rtP -> P_1276 + _rtP ->
P_1277 ; _rtB -> B_185_2447_0 [ 0 ] = _rtB -> B_185_1960_0 [ 0 ] *
B_185_23_0_idx_0 ; _rtB -> B_185_2447_0 [ 1 ] = _rtB -> B_185_1960_0 [ 1 ] *
B_185_23_0_idx_0 ; _rtB -> B_185_2447_0 [ 2 ] = _rtB -> B_185_1960_0 [ 2 ] *
B_185_23_0_idx_0 ; B_185_23_0_idx_0 = muDoubleScalarSin ( _rtP -> P_1282 *
ssGetTaskTime ( S , 0 ) + _rtP -> P_1283 ) * _rtP -> P_1280 + _rtP -> P_1281
; _rtB -> B_185_2449_0 [ 0 ] = _rtB -> B_185_1960_0 [ 0 ] * B_185_23_0_idx_0
; _rtB -> B_185_2449_0 [ 1 ] = _rtB -> B_185_1960_0 [ 1 ] * B_185_23_0_idx_0
; _rtB -> B_185_2449_0 [ 2 ] = _rtB -> B_185_1960_0 [ 2 ] * B_185_23_0_idx_0
; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_185_2453_0 [ 0 ] = _rtP -> P_1284 * _rtB -> B_185_926_0 [ 144 ] * _rtP ->
P_1287 ; _rtB -> B_185_2453_0 [ 1 ] = _rtP -> P_1285 * _rtB -> B_185_926_0 [
145 ] * _rtP -> P_1287 ; _rtB -> B_185_2453_0 [ 2 ] = _rtP -> P_1286 * _rtB
-> B_185_926_0 [ 146 ] * _rtP -> P_1287 ; } B_185_23_0_idx_0 =
muDoubleScalarSin ( _rtP -> P_1290 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1291
) * _rtP -> P_1288 + _rtP -> P_1289 ; _rtB -> B_185_2455_0 [ 0 ] = _rtB ->
B_185_2453_0 [ 0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2455_0 [ 1 ] = _rtB ->
B_185_2453_0 [ 1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2455_0 [ 2 ] = _rtB ->
B_185_2453_0 [ 2 ] * B_185_23_0_idx_0 ; B_185_23_0_idx_0 = muDoubleScalarSin
( _rtP -> P_1294 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1295 ) * _rtP ->
P_1292 + _rtP -> P_1293 ; _rtB -> B_185_2457_0 [ 0 ] = _rtB -> B_185_2453_0 [
0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2458_0 [ 0 ] = _rtX ->
integrator_CSTATE_nm [ 0 ] ; _rtB -> B_185_2457_0 [ 1 ] = _rtB ->
B_185_2453_0 [ 1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2458_0 [ 1 ] = _rtX ->
integrator_CSTATE_nm [ 1 ] ; _rtB -> B_185_2457_0 [ 2 ] = _rtB ->
B_185_2453_0 [ 2 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2458_0 [ 2 ] = _rtX ->
integrator_CSTATE_nm [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_nm . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay_PWORK_nm . TUbufferPtrs [ 3 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = &
B_185_2459_0 [ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_k .
Tail [ 0 ] ; int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_k . Head [ 0 ]
; int_T * iw_Last = & _rtDW -> TransportDelay_IWORK_k . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_k . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1297 > 0.0
) ? _rtP -> P_1297 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1298 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2461_0 = _rtP -> P_1299 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2461_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2463_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_gn [ 0 ] ; _rtB -> B_185_2463_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_gn [ 1 ] ; _rtB -> B_185_2463_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_gn [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2465_0 [ 0 ] = ( _rtB -> B_185_2458_0 [ 0 ] - B_185_2459_0 [ 0 ] ) *
_rtP -> P_106 ; _rtB -> B_185_2465_0 [ 1 ] = ( _rtB -> B_185_2458_0 [ 1 ] -
B_185_2459_0 [ 1 ] ) * _rtP -> P_106 ; _rtB -> B_185_2465_0 [ 2 ] = ( _rtB ->
B_185_2458_0 [ 2 ] - B_185_2459_0 [ 2 ] ) * _rtP -> P_106 ; } else { _rtB ->
B_185_2465_0 [ 0 ] = _rtB -> B_185_2463_0 [ 0 ] ; _rtB -> B_185_2465_0 [ 1 ]
= _rtB -> B_185_2463_0 [ 1 ] ; _rtB -> B_185_2465_0 [ 2 ] = _rtB ->
B_185_2463_0 [ 2 ] ; } _rtB -> B_185_2466_0 [ 0 ] = _rtX ->
integrator_CSTATE_c [ 0 ] ; _rtB -> B_185_2466_0 [ 1 ] = _rtX ->
integrator_CSTATE_c [ 1 ] ; _rtB -> B_185_2466_0 [ 2 ] = _rtX ->
integrator_CSTATE_c [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_h . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_h . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & B_185_2467_0
[ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_kd . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_kd . Head [ 0 ] ; int_T *
iw_Last = & _rtDW -> TransportDelay_IWORK_kd . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_kd . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1302 > 0.0
) ? _rtP -> P_1302 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1303 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2469_0 = _rtP -> P_1304 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2469_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2471_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_e2 [ 0 ] ; _rtB -> B_185_2471_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_e2 [ 1 ] ; _rtB -> B_185_2471_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_e2 [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2473_0 [ 0 ] = ( _rtB -> B_185_2466_0 [ 0 ] - B_185_2467_0 [ 0 ] ) *
_rtP -> P_105 ; _rtB -> B_185_2473_0 [ 1 ] = ( _rtB -> B_185_2466_0 [ 1 ] -
B_185_2467_0 [ 1 ] ) * _rtP -> P_105 ; _rtB -> B_185_2473_0 [ 2 ] = ( _rtB ->
B_185_2466_0 [ 2 ] - B_185_2467_0 [ 2 ] ) * _rtP -> P_105 ; } else { _rtB ->
B_185_2473_0 [ 0 ] = _rtB -> B_185_2471_0 [ 0 ] ; _rtB -> B_185_2473_0 [ 1 ]
= _rtB -> B_185_2471_0 [ 1 ] ; _rtB -> B_185_2473_0 [ 2 ] = _rtB ->
B_185_2471_0 [ 2 ] ; } B_185_23_0_idx_0 = muDoubleScalarHypot ( _rtB ->
B_185_2465_0 [ 0 ] , _rtB -> B_185_2473_0 [ 0 ] ) ; B_185_25_0_idx_0 =
muDoubleScalarAtan2 ( _rtB -> B_185_2473_0 [ 0 ] , _rtB -> B_185_2465_0 [ 0 ]
) ; _rtB -> B_185_2476_0 [ 0 ] = _rtX -> integrator_CSTATE_e [ 0 ] ;
B_185_23_0_idx_1 = muDoubleScalarHypot ( _rtB -> B_185_2465_0 [ 1 ] , _rtB ->
B_185_2473_0 [ 1 ] ) ; B_185_25_0_idx_1 = muDoubleScalarAtan2 ( _rtB ->
B_185_2473_0 [ 1 ] , _rtB -> B_185_2465_0 [ 1 ] ) ; _rtB -> B_185_2476_0 [ 1
] = _rtX -> integrator_CSTATE_e [ 1 ] ; B_185_23_0_idx_2 =
muDoubleScalarHypot ( _rtB -> B_185_2465_0 [ 2 ] , _rtB -> B_185_2473_0 [ 2 ]
) ; B_185_25_0_idx_2 = muDoubleScalarAtan2 ( _rtB -> B_185_2473_0 [ 2 ] ,
_rtB -> B_185_2465_0 [ 2 ] ) ; _rtB -> B_185_2476_0 [ 2 ] = _rtX ->
integrator_CSTATE_e [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_pw . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay_PWORK_pw . TUbufferPtrs [ 3 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = &
B_185_2477_0 [ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_it .
Tail [ 0 ] ; int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_it . Head [ 0
] ; int_T * iw_Last = & _rtDW -> TransportDelay_IWORK_it . Last [ 0 ] ; int_T
* iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_it . CircularBufSize [
0 ] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1307 >
0.0 ) ? _rtP -> P_1307 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [
i1 ] = normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay ,
0.0 , * tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] ,
iw_Tail [ i1 ] , iw_Head [ i1 ] , _rtP -> P_1308 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2479_0 = _rtP -> P_1309 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2479_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2481_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_n [ 0 ] ; _rtB -> B_185_2481_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_n [ 1 ] ; _rtB -> B_185_2481_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_n [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2483_0 [ 0 ] = ( _rtB -> B_185_2476_0 [ 0 ] - B_185_2477_0 [ 0 ] ) *
_rtP -> P_108 ; _rtB -> B_185_2483_0 [ 1 ] = ( _rtB -> B_185_2476_0 [ 1 ] -
B_185_2477_0 [ 1 ] ) * _rtP -> P_108 ; _rtB -> B_185_2483_0 [ 2 ] = ( _rtB ->
B_185_2476_0 [ 2 ] - B_185_2477_0 [ 2 ] ) * _rtP -> P_108 ; } else { _rtB ->
B_185_2483_0 [ 0 ] = _rtB -> B_185_2481_0 [ 0 ] ; _rtB -> B_185_2483_0 [ 1 ]
= _rtB -> B_185_2481_0 [ 1 ] ; _rtB -> B_185_2483_0 [ 2 ] = _rtB ->
B_185_2481_0 [ 2 ] ; } _rtB -> B_185_2484_0 [ 0 ] = _rtX ->
integrator_CSTATE_f [ 0 ] ; _rtB -> B_185_2484_0 [ 1 ] = _rtX ->
integrator_CSTATE_f [ 1 ] ; _rtB -> B_185_2484_0 [ 2 ] = _rtX ->
integrator_CSTATE_f [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_d . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_d . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & B_185_2485_0
[ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_c . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_c . Head [ 0 ] ; int_T *
iw_Last = & _rtDW -> TransportDelay_IWORK_c . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_c . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1312 > 0.0
) ? _rtP -> P_1312 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1313 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2487_0 = _rtP -> P_1314 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2487_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2489_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_i [ 0 ] ; _rtB -> B_185_2489_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_i [ 1 ] ; _rtB -> B_185_2489_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_i [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2491_0 [ 0 ] = ( _rtB -> B_185_2484_0 [ 0 ] - B_185_2485_0 [ 0 ] ) *
_rtP -> P_107 ; _rtB -> B_185_2491_0 [ 1 ] = ( _rtB -> B_185_2484_0 [ 1 ] -
B_185_2485_0 [ 1 ] ) * _rtP -> P_107 ; _rtB -> B_185_2491_0 [ 2 ] = ( _rtB ->
B_185_2484_0 [ 2 ] - B_185_2485_0 [ 2 ] ) * _rtP -> P_107 ; } else { _rtB ->
B_185_2491_0 [ 0 ] = _rtB -> B_185_2489_0 [ 0 ] ; _rtB -> B_185_2491_0 [ 1 ]
= _rtB -> B_185_2489_0 [ 1 ] ; _rtB -> B_185_2491_0 [ 2 ] = _rtB ->
B_185_2489_0 [ 2 ] ; } B_185_23_0_idx_0 = B_185_23_0_idx_0 *
muDoubleScalarHypot ( _rtB -> B_185_2483_0 [ 0 ] , _rtB -> B_185_2491_0 [ 0 ]
) * _rtP -> P_1316 ; B_185_25_0_idx_0 = ( _rtP -> P_1317 * B_185_25_0_idx_0 -
_rtP -> P_1318 * muDoubleScalarAtan2 ( _rtB -> B_185_2491_0 [ 0 ] , _rtB ->
B_185_2483_0 [ 0 ] ) ) * _rtP -> P_1319 ; B_185_23_0_idx_1 = B_185_23_0_idx_1
* muDoubleScalarHypot ( _rtB -> B_185_2483_0 [ 1 ] , _rtB -> B_185_2491_0 [ 1
] ) * _rtP -> P_1316 ; B_185_25_0_idx_1 = ( _rtP -> P_1317 * B_185_25_0_idx_1
- _rtP -> P_1318 * muDoubleScalarAtan2 ( _rtB -> B_185_2491_0 [ 1 ] , _rtB ->
B_185_2483_0 [ 1 ] ) ) * _rtP -> P_1319 ; rtb_B_185_23_0 = B_185_23_0_idx_2 *
muDoubleScalarHypot ( _rtB -> B_185_2483_0 [ 2 ] , _rtB -> B_185_2491_0 [ 2 ]
) * _rtP -> P_1316 ; rtb_B_185_25_0 = ( _rtP -> P_1317 * B_185_25_0_idx_2 -
_rtP -> P_1318 * muDoubleScalarAtan2 ( _rtB -> B_185_2491_0 [ 2 ] , _rtB ->
B_185_2483_0 [ 2 ] ) ) * _rtP -> P_1319 ; muDoubleScalarSinCos (
B_185_25_0_idx_0 , & B_185_2500_0_idx_0 , & B_185_2500_1_idx_0 ) ;
muDoubleScalarSinCos ( B_185_25_0_idx_1 , & B_185_2500_0_idx_1 , &
B_185_2500_1_idx_1 ) ; muDoubleScalarSinCos ( rtb_B_185_25_0 , &
B_185_2500_0_idx_2 , & B_185_2500_1_idx_2 ) ; B_185_23_0_idx_0 *=
B_185_2500_1_idx_0 ; B_185_23_0_idx_1 *= B_185_2500_1_idx_1 ; _rtB ->
B_185_2502_0 = ( B_185_23_0_idx_0 + B_185_23_0_idx_1 ) + rtb_B_185_23_0 *
B_185_2500_1_idx_2 ; ssCallAccelRunBlock ( S , 185 , 2503 ,
SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0
) { _rtB -> B_185_2507_0 [ 0 ] = _rtP -> P_1320 * _rtB -> B_185_926_0 [ 116 ]
* _rtP -> P_1323 ; _rtB -> B_185_2507_0 [ 1 ] = _rtP -> P_1321 * _rtB ->
B_185_926_0 [ 117 ] * _rtP -> P_1323 ; _rtB -> B_185_2507_0 [ 2 ] = _rtP ->
P_1322 * _rtB -> B_185_926_0 [ 118 ] * _rtP -> P_1323 ; } B_185_23_0_idx_0 =
muDoubleScalarSin ( _rtP -> P_1326 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1327
) * _rtP -> P_1324 + _rtP -> P_1325 ; _rtB -> B_185_2509_0 [ 0 ] = _rtB ->
B_185_2507_0 [ 0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2509_0 [ 1 ] = _rtB ->
B_185_2507_0 [ 1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2509_0 [ 2 ] = _rtB ->
B_185_2507_0 [ 2 ] * B_185_23_0_idx_0 ; B_185_23_0_idx_0 = muDoubleScalarSin
( _rtP -> P_1330 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1331 ) * _rtP ->
P_1328 + _rtP -> P_1329 ; _rtB -> B_185_2511_0 [ 0 ] = _rtB -> B_185_2507_0 [
0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2511_0 [ 1 ] = _rtB -> B_185_2507_0 [
1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2511_0 [ 2 ] = _rtB -> B_185_2507_0 [
2 ] * B_185_23_0_idx_0 ; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit !=
0 ) { _rtB -> B_185_2515_0 [ 0 ] = _rtP -> P_1332 * _rtB -> B_185_926_0 [ 147
] * _rtP -> P_1335 ; _rtB -> B_185_2515_0 [ 1 ] = _rtP -> P_1333 * _rtB ->
B_185_926_0 [ 148 ] * _rtP -> P_1335 ; _rtB -> B_185_2515_0 [ 2 ] = _rtP ->
P_1334 * _rtB -> B_185_926_0 [ 149 ] * _rtP -> P_1335 ; } B_185_23_0_idx_0 =
muDoubleScalarSin ( _rtP -> P_1338 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1339
) * _rtP -> P_1336 + _rtP -> P_1337 ; _rtB -> B_185_2517_0 [ 0 ] = _rtB ->
B_185_2515_0 [ 0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2517_0 [ 1 ] = _rtB ->
B_185_2515_0 [ 1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2517_0 [ 2 ] = _rtB ->
B_185_2515_0 [ 2 ] * B_185_23_0_idx_0 ; B_185_23_0_idx_0 = muDoubleScalarSin
( _rtP -> P_1342 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1343 ) * _rtP ->
P_1340 + _rtP -> P_1341 ; _rtB -> B_185_2519_0 [ 0 ] = _rtB -> B_185_2515_0 [
0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2519_0 [ 1 ] = _rtB -> B_185_2515_0 [
1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2519_0 [ 2 ] = _rtB -> B_185_2515_0 [
2 ] * B_185_23_0_idx_0 ; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit !=
0 ) { _rtB -> B_185_2523_0 [ 0 ] = _rtP -> P_1344 * _rtB -> B_185_926_0 [ 95
] * _rtP -> P_1347 ; _rtB -> B_185_2523_0 [ 1 ] = _rtP -> P_1345 * _rtB ->
B_185_926_0 [ 96 ] * _rtP -> P_1347 ; _rtB -> B_185_2523_0 [ 2 ] = _rtP ->
P_1346 * _rtB -> B_185_926_0 [ 97 ] * _rtP -> P_1347 ; ssCallAccelRunBlock (
S , 185 , 2524 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_185_2523_0 [ 0 ] = _rtP ->
P_1348 * _rtB -> B_185_926_0 [ 101 ] * _rtP -> P_1351 ; _rtB -> B_185_2523_0
[ 1 ] = _rtP -> P_1349 * _rtB -> B_185_926_0 [ 102 ] * _rtP -> P_1351 ; _rtB
-> B_185_2523_0 [ 2 ] = _rtP -> P_1350 * _rtB -> B_185_926_0 [ 103 ] * _rtP
-> P_1351 ; ssCallAccelRunBlock ( S , 185 , 2529 , SS_CALL_MDL_OUTPUTS ) ;
_rtB -> B_185_2523_0 [ 0 ] = _rtP -> P_1352 * _rtB -> B_185_926_0 [ 107 ] *
_rtP -> P_1355 ; _rtB -> B_185_2523_0 [ 1 ] = _rtP -> P_1353 * _rtB ->
B_185_926_0 [ 108 ] * _rtP -> P_1355 ; _rtB -> B_185_2523_0 [ 2 ] = _rtP ->
P_1354 * _rtB -> B_185_926_0 [ 109 ] * _rtP -> P_1355 ; ssCallAccelRunBlock (
S , 185 , 2534 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_185_2523_0 [ 0 ] = _rtP ->
P_1356 * _rtB -> B_185_926_0 [ 113 ] * _rtP -> P_1359 ; _rtB -> B_185_2523_0
[ 1 ] = _rtP -> P_1357 * _rtB -> B_185_926_0 [ 114 ] * _rtP -> P_1359 ; _rtB
-> B_185_2523_0 [ 2 ] = _rtP -> P_1358 * _rtB -> B_185_926_0 [ 115 ] * _rtP
-> P_1359 ; ssCallAccelRunBlock ( S , 185 , 2539 , SS_CALL_MDL_OUTPUTS ) ;
_rtB -> B_185_2540_0 = _rtP -> P_1360 * _rtB -> B_185_926_0 [ 153 ] ;
ssCallAccelRunBlock ( S , 185 , 2541 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_185_2542_0 = _rtP -> P_1361 * _rtB -> B_185_926_0 [ 154 ] ;
ssCallAccelRunBlock ( S , 185 , 2543 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_185_2544_0 = _rtP -> P_1362 * _rtB -> B_185_926_0 [ 155 ] ;
ssCallAccelRunBlock ( S , 185 , 2545 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_185_2546_0 = _rtP -> P_1363 * _rtB -> B_185_926_0 [ 156 ] ;
ssCallAccelRunBlock ( S , 185 , 2547 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_185_2548_0 = _rtB -> B_185_1350_0 * _rtB -> B_185_2540_0 ;
ssCallAccelRunBlock ( S , 185 , 2549 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_185_2550_0 = _rtB -> B_185_1646_0 * _rtB -> B_185_2542_0 ;
ssCallAccelRunBlock ( S , 185 , 2551 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_185_2552_0 = _rtB -> B_185_1942_0 * _rtB -> B_185_2544_0 ;
ssCallAccelRunBlock ( S , 185 , 2553 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_185_2554_0 = _rtP -> P_1364 * _rtB -> B_185_926_0 [ 125 ] ; _rtB ->
B_185_2555_0 = _rtB -> B_185_2554_0 * _rtB -> B_185_2546_0 ;
ssCallAccelRunBlock ( S , 185 , 2556 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_185_2560_0 [ 0 ] = _rtP -> P_1365 * _rtB -> B_185_926_0 [ 135 ] * _rtP ->
P_1368 ; _rtB -> B_185_2560_0 [ 1 ] = _rtP -> P_1366 * _rtB -> B_185_926_0 [
136 ] * _rtP -> P_1368 ; _rtB -> B_185_2560_0 [ 2 ] = _rtP -> P_1367 * _rtB
-> B_185_926_0 [ 137 ] * _rtP -> P_1368 ; _rtB -> B_185_2564_0 [ 0 ] = _rtP
-> P_1369 * _rtB -> B_185_926_0 [ 167 ] * _rtP -> P_1372 ; _rtB ->
B_185_2564_0 [ 1 ] = _rtP -> P_1370 * _rtB -> B_185_926_0 [ 168 ] * _rtP ->
P_1372 ; _rtB -> B_185_2564_0 [ 2 ] = _rtP -> P_1371 * _rtB -> B_185_926_0 [
169 ] * _rtP -> P_1372 ; ssCallAccelRunBlock ( S , 185 , 2565 ,
SS_CALL_MDL_OUTPUTS ) ; } normal_operation_no_faults_TrueRMS ( S , _rtB ->
B_185_2567_0 , _rtB -> B_185_2560_0 , & _rtB -> TrueRMS , & _rtDW -> TrueRMS
, & _rtP -> TrueRMS , & _rtX -> TrueRMS , & ( (
XDis_normal_operation_no_faults_T * ) ssGetContStateDisabled ( S ) ) ->
TrueRMS ) ; normal_operation_no_faults_RMS ( S , _rtB -> B_185_2569_0 , _rtB
-> B_185_2560_0 , & _rtB -> RMS , & _rtDW -> RMS , & _rtP -> RMS , & _rtX ->
RMS , & ( ( XDis_normal_operation_no_faults_T * ) ssGetContStateDisabled ( S
) ) -> RMS ) ; if ( _rtB -> B_185_2567_0 ) { B_185_29_0_idx_0 = _rtB ->
TrueRMS . B_24_10_0 [ 0 ] ; } else { B_185_29_0_idx_0 = _rtB -> RMS .
B_22_22_0 [ 0 ] ; } _rtB -> B_185_710_0 [ 0 ] = _rtP -> P_1374 *
B_185_29_0_idx_0 ; if ( _rtB -> B_185_2567_0 ) { B_185_29_0_idx_0 = _rtB ->
TrueRMS . B_24_10_0 [ 1 ] ; } else { B_185_29_0_idx_0 = _rtB -> RMS .
B_22_22_0 [ 1 ] ; } _rtB -> B_185_710_0 [ 1 ] = _rtP -> P_1374 *
B_185_29_0_idx_0 ; if ( _rtB -> B_185_2567_0 ) { B_185_29_0_idx_0 = _rtB ->
TrueRMS . B_24_10_0 [ 2 ] ; } else { B_185_29_0_idx_0 = _rtB -> RMS .
B_22_22_0 [ 2 ] ; } _rtB -> B_185_710_0 [ 2 ] = _rtP -> P_1374 *
B_185_29_0_idx_0 ; ssCallAccelRunBlock ( S , 185 , 2573 , SS_CALL_MDL_OUTPUTS
) ; _rtB -> B_185_2574_0 [ 0 ] = _rtX -> integrator_CSTATE_kd [ 0 ] ; _rtB ->
B_185_2574_0 [ 1 ] = _rtX -> integrator_CSTATE_kd [ 1 ] ; _rtB ->
B_185_2574_0 [ 2 ] = _rtX -> integrator_CSTATE_kd [ 2 ] ; { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_k . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_k .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay ; {
int_T i1 ; real_T * y0 = & B_185_2575_0 [ 0 ] ; int_T * iw_Tail = & _rtDW ->
TransportDelay_IWORK_mv . Tail [ 0 ] ; int_T * iw_Head = & _rtDW ->
TransportDelay_IWORK_mv . Head [ 0 ] ; int_T * iw_Last = & _rtDW ->
TransportDelay_IWORK_mv . Last [ 0 ] ; int_T * iw_CircularBufSize = & _rtDW
-> TransportDelay_IWORK_mv . CircularBufSize [ 0 ] ; for ( i1 = 0 ; i1 < 3 ;
i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1376 > 0.0 ) ? _rtP -> P_1376 : 0.0 ) ;
tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ] =
normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1377 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2577_0 = _rtP -> P_1378 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2577_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2579_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_bk [ 0 ] ; _rtB -> B_185_2579_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_bk [ 1 ] ; _rtB -> B_185_2579_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_bk [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2581_0 [ 0 ] = ( _rtB -> B_185_2574_0 [ 0 ] - B_185_2575_0 [ 0 ] ) *
_rtP -> P_88 ; _rtB -> B_185_2581_0 [ 1 ] = ( _rtB -> B_185_2574_0 [ 1 ] -
B_185_2575_0 [ 1 ] ) * _rtP -> P_88 ; _rtB -> B_185_2581_0 [ 2 ] = ( _rtB ->
B_185_2574_0 [ 2 ] - B_185_2575_0 [ 2 ] ) * _rtP -> P_88 ; } else { _rtB ->
B_185_2581_0 [ 0 ] = _rtB -> B_185_2579_0 [ 0 ] ; _rtB -> B_185_2581_0 [ 1 ]
= _rtB -> B_185_2579_0 [ 1 ] ; _rtB -> B_185_2581_0 [ 2 ] = _rtB ->
B_185_2579_0 [ 2 ] ; } _rtB -> B_185_2582_0 [ 0 ] = _rtX ->
integrator_CSTATE_c1 [ 0 ] ; _rtB -> B_185_2582_0 [ 1 ] = _rtX ->
integrator_CSTATE_c1 [ 1 ] ; _rtB -> B_185_2582_0 [ 2 ] = _rtX ->
integrator_CSTATE_c1 [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_hv . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay_PWORK_hv . TUbufferPtrs [ 3 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = &
B_185_2583_0 [ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_a1 .
Tail [ 0 ] ; int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_a1 . Head [ 0
] ; int_T * iw_Last = & _rtDW -> TransportDelay_IWORK_a1 . Last [ 0 ] ; int_T
* iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_a1 . CircularBufSize [
0 ] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1381 >
0.0 ) ? _rtP -> P_1381 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [
i1 ] = normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay ,
0.0 , * tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] ,
iw_Tail [ i1 ] , iw_Head [ i1 ] , _rtP -> P_1382 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2585_0 = _rtP -> P_1383 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2585_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2587_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_ab [ 0 ] ; _rtB -> B_185_2587_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_ab [ 1 ] ; _rtB -> B_185_2587_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_ab [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2589_0 [ 0 ] = ( _rtB -> B_185_2582_0 [ 0 ] - B_185_2583_0 [ 0 ] ) *
_rtP -> P_87 ; _rtB -> B_185_2589_0 [ 1 ] = ( _rtB -> B_185_2582_0 [ 1 ] -
B_185_2583_0 [ 1 ] ) * _rtP -> P_87 ; _rtB -> B_185_2589_0 [ 2 ] = ( _rtB ->
B_185_2582_0 [ 2 ] - B_185_2583_0 [ 2 ] ) * _rtP -> P_87 ; } else { _rtB ->
B_185_2589_0 [ 0 ] = _rtB -> B_185_2587_0 [ 0 ] ; _rtB -> B_185_2589_0 [ 1 ]
= _rtB -> B_185_2587_0 [ 1 ] ; _rtB -> B_185_2589_0 [ 2 ] = _rtB ->
B_185_2587_0 [ 2 ] ; } B_185_23_0_idx_0 = muDoubleScalarHypot ( _rtB ->
B_185_2581_0 [ 0 ] , _rtB -> B_185_2589_0 [ 0 ] ) ; B_185_25_0_idx_0 =
muDoubleScalarAtan2 ( _rtB -> B_185_2589_0 [ 0 ] , _rtB -> B_185_2581_0 [ 0 ]
) ; _rtB -> B_185_2592_0 [ 0 ] = _rtX -> integrator_CSTATE_js [ 0 ] ;
B_185_23_0_idx_1 = muDoubleScalarHypot ( _rtB -> B_185_2581_0 [ 1 ] , _rtB ->
B_185_2589_0 [ 1 ] ) ; B_185_25_0_idx_1 = muDoubleScalarAtan2 ( _rtB ->
B_185_2589_0 [ 1 ] , _rtB -> B_185_2581_0 [ 1 ] ) ; _rtB -> B_185_2592_0 [ 1
] = _rtX -> integrator_CSTATE_js [ 1 ] ; B_185_23_0_idx_2 =
muDoubleScalarHypot ( _rtB -> B_185_2581_0 [ 2 ] , _rtB -> B_185_2589_0 [ 2 ]
) ; B_185_25_0_idx_2 = muDoubleScalarAtan2 ( _rtB -> B_185_2589_0 [ 2 ] ,
_rtB -> B_185_2581_0 [ 2 ] ) ; _rtB -> B_185_2592_0 [ 2 ] = _rtX ->
integrator_CSTATE_js [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_e . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_e . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & B_185_2593_0
[ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_l0 . Tail [ 0 ] ;
int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_l0 . Head [ 0 ] ; int_T *
iw_Last = & _rtDW -> TransportDelay_IWORK_l0 . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_l0 . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1386 > 0.0
) ? _rtP -> P_1386 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1387 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2595_0 = _rtP -> P_1388 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2595_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2597_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_jf [ 0 ] ; _rtB -> B_185_2597_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_jf [ 1 ] ; _rtB -> B_185_2597_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_jf [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2599_0 [ 0 ] = ( _rtB -> B_185_2592_0 [ 0 ] - B_185_2593_0 [ 0 ] ) *
_rtP -> P_90 ; _rtB -> B_185_2599_0 [ 1 ] = ( _rtB -> B_185_2592_0 [ 1 ] -
B_185_2593_0 [ 1 ] ) * _rtP -> P_90 ; _rtB -> B_185_2599_0 [ 2 ] = ( _rtB ->
B_185_2592_0 [ 2 ] - B_185_2593_0 [ 2 ] ) * _rtP -> P_90 ; } else { _rtB ->
B_185_2599_0 [ 0 ] = _rtB -> B_185_2597_0 [ 0 ] ; _rtB -> B_185_2599_0 [ 1 ]
= _rtB -> B_185_2597_0 [ 1 ] ; _rtB -> B_185_2599_0 [ 2 ] = _rtB ->
B_185_2597_0 [ 2 ] ; } _rtB -> B_185_2600_0 [ 0 ] = _rtX ->
integrator_CSTATE_po [ 0 ] ; _rtB -> B_185_2600_0 [ 1 ] = _rtX ->
integrator_CSTATE_po [ 1 ] ; _rtB -> B_185_2600_0 [ 2 ] = _rtX ->
integrator_CSTATE_po [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_dm . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay_PWORK_dm . TUbufferPtrs [ 3 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = &
B_185_2601_0 [ 0 ] ; int_T * iw_Tail = & _rtDW -> TransportDelay_IWORK_b .
Tail [ 0 ] ; int_T * iw_Head = & _rtDW -> TransportDelay_IWORK_b . Head [ 0 ]
; int_T * iw_Last = & _rtDW -> TransportDelay_IWORK_b . Last [ 0 ] ; int_T *
iw_CircularBufSize = & _rtDW -> TransportDelay_IWORK_b . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( _rtP -> P_1391 > 0.0
) ? _rtP -> P_1391 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ]
= normal_operation_no_faults_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail
[ i1 ] , iw_Head [ i1 ] , _rtP -> P_1392 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_185_2603_0 = _rtP -> P_1393 ; } rtb_B_185_2288_0 =
( ssGetT ( S ) >= _rtB -> B_185_2603_0 ) ; isHit = ssIsSampleHit ( S , 1 , 0
) ; if ( isHit != 0 ) { _rtB -> B_185_2605_0 [ 0 ] = _rtDW ->
Memory_PreviousInput_og [ 0 ] ; _rtB -> B_185_2605_0 [ 1 ] = _rtDW ->
Memory_PreviousInput_og [ 1 ] ; _rtB -> B_185_2605_0 [ 2 ] = _rtDW ->
Memory_PreviousInput_og [ 2 ] ; } if ( rtb_B_185_2288_0 ) { _rtB ->
B_185_2607_0 [ 0 ] = ( _rtB -> B_185_2600_0 [ 0 ] - B_185_2601_0 [ 0 ] ) *
_rtP -> P_89 ; _rtB -> B_185_2607_0 [ 1 ] = ( _rtB -> B_185_2600_0 [ 1 ] -
B_185_2601_0 [ 1 ] ) * _rtP -> P_89 ; _rtB -> B_185_2607_0 [ 2 ] = ( _rtB ->
B_185_2600_0 [ 2 ] - B_185_2601_0 [ 2 ] ) * _rtP -> P_89 ; } else { _rtB ->
B_185_2607_0 [ 0 ] = _rtB -> B_185_2605_0 [ 0 ] ; _rtB -> B_185_2607_0 [ 1 ]
= _rtB -> B_185_2605_0 [ 1 ] ; _rtB -> B_185_2607_0 [ 2 ] = _rtB ->
B_185_2605_0 [ 2 ] ; } B_185_23_0_idx_0 = B_185_23_0_idx_0 *
muDoubleScalarHypot ( _rtB -> B_185_2599_0 [ 0 ] , _rtB -> B_185_2607_0 [ 0 ]
) * _rtP -> P_1395 ; B_185_25_0_idx_0 = ( _rtP -> P_1396 * B_185_25_0_idx_0 -
_rtP -> P_1397 * muDoubleScalarAtan2 ( _rtB -> B_185_2607_0 [ 0 ] , _rtB ->
B_185_2599_0 [ 0 ] ) ) * _rtP -> P_1398 ; B_185_23_0_idx_1 = B_185_23_0_idx_1
* muDoubleScalarHypot ( _rtB -> B_185_2599_0 [ 1 ] , _rtB -> B_185_2607_0 [ 1
] ) * _rtP -> P_1395 ; B_185_25_0_idx_1 = ( _rtP -> P_1396 * B_185_25_0_idx_1
- _rtP -> P_1397 * muDoubleScalarAtan2 ( _rtB -> B_185_2607_0 [ 1 ] , _rtB ->
B_185_2599_0 [ 1 ] ) ) * _rtP -> P_1398 ; rtb_B_185_23_0 = B_185_23_0_idx_2 *
muDoubleScalarHypot ( _rtB -> B_185_2599_0 [ 2 ] , _rtB -> B_185_2607_0 [ 2 ]
) * _rtP -> P_1395 ; rtb_B_185_25_0 = ( _rtP -> P_1396 * B_185_25_0_idx_2 -
_rtP -> P_1397 * muDoubleScalarAtan2 ( _rtB -> B_185_2607_0 [ 2 ] , _rtB ->
B_185_2599_0 [ 2 ] ) ) * _rtP -> P_1398 ; muDoubleScalarSinCos (
B_185_25_0_idx_0 , & B_185_2616_0_idx_0 , & B_185_2616_1_idx_0 ) ;
muDoubleScalarSinCos ( B_185_25_0_idx_1 , & B_185_2616_0_idx_1 , &
B_185_2616_1_idx_1 ) ; muDoubleScalarSinCos ( rtb_B_185_25_0 , &
B_185_2616_0_idx_2 , & B_185_2616_1_idx_2 ) ; B_185_2616_1_idx_0 *=
B_185_23_0_idx_0 ; B_185_2616_1_idx_1 *= B_185_23_0_idx_1 ; B_185_23_0_idx_0
*= B_185_2616_0_idx_0 ; B_185_23_0_idx_1 *= B_185_2616_0_idx_1 ; _rtB ->
B_185_2618_0 = ( B_185_2616_1_idx_0 + B_185_2616_1_idx_1 ) + rtb_B_185_23_0 *
B_185_2616_1_idx_2 ; _rtB -> B_185_2620_0 = ( B_185_23_0_idx_0 +
B_185_23_0_idx_1 ) + rtb_B_185_23_0 * B_185_2616_0_idx_2 ;
ssCallAccelRunBlock ( S , 185 , 2621 , SS_CALL_MDL_OUTPUTS ) ;
B_185_23_0_idx_0 = muDoubleScalarSin ( _rtP -> P_1401 * ssGetTaskTime ( S , 0
) + _rtP -> P_1402 ) * _rtP -> P_1399 + _rtP -> P_1400 ; _rtB -> B_185_2623_0
[ 0 ] = _rtB -> B_185_2560_0 [ 0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2623_0
[ 1 ] = _rtB -> B_185_2560_0 [ 1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2623_0
[ 2 ] = _rtB -> B_185_2560_0 [ 2 ] * B_185_23_0_idx_0 ; B_185_23_0_idx_0 =
muDoubleScalarSin ( _rtP -> P_1405 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1406
) * _rtP -> P_1403 + _rtP -> P_1404 ; _rtB -> B_185_2625_0 [ 0 ] = _rtB ->
B_185_2560_0 [ 0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2625_0 [ 1 ] = _rtB ->
B_185_2560_0 [ 1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2625_0 [ 2 ] = _rtB ->
B_185_2560_0 [ 2 ] * B_185_23_0_idx_0 ; B_185_23_0_idx_0 = muDoubleScalarSin
( _rtP -> P_1409 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1410 ) * _rtP ->
P_1407 + _rtP -> P_1408 ; _rtB -> B_185_2627_0 [ 0 ] = _rtB -> B_185_2564_0 [
0 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2627_0 [ 1 ] = _rtB -> B_185_2564_0 [
1 ] * B_185_23_0_idx_0 ; _rtB -> B_185_2627_0 [ 2 ] = _rtB -> B_185_2564_0 [
2 ] * B_185_23_0_idx_0 ; B_185_23_0_idx_0 = muDoubleScalarSin ( _rtP ->
P_1413 * ssGetTaskTime ( S , 0 ) + _rtP -> P_1414 ) * _rtP -> P_1411 + _rtP
-> P_1412 ; _rtB -> B_185_2629_0 [ 0 ] = _rtB -> B_185_2564_0 [ 0 ] *
B_185_23_0_idx_0 ; _rtB -> B_185_2629_0 [ 1 ] = _rtB -> B_185_2564_0 [ 1 ] *
B_185_23_0_idx_0 ; _rtB -> B_185_2629_0 [ 2 ] = _rtB -> B_185_2564_0 [ 2 ] *
B_185_23_0_idx_0 ; isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_185_2631_0 = _rtB -> B_185_231_0 [ 0 ] * _rtB -> B_185_266_0 [ 0 ]
+ _rtB -> B_185_231_0 [ 1 ] * _rtB -> B_185_266_0 [ 1 ] ; } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { if ( _rtDW ->
Integ4_SYSTEM_ENABLE != 0 ) { _rtB -> B_185_2632_0 = _rtDW -> Integ4_DSTATE ;
} else { _rtB -> B_185_2632_0 = _rtP -> P_1415 * _rtB -> B_185_2631_0 + _rtDW
-> Integ4_DSTATE ; } _rtB -> B_185_2633_0 = _rtP -> P_1417 ;
ssCallAccelRunBlock ( S , 185 , 2634 , SS_CALL_MDL_OUTPUTS ) ;
rtb_B_185_2635_0 = _rtP -> P_1418 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ;
if ( isHit != 0 ) { _rtB -> B_185_2636_0 = _rtP -> P_1419 * _rtB ->
B_185_2631_0 ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) {
rtb_B_185_30_0 = ssGetTaskTime ( S , 2 ) ; if ( rtb_B_185_30_0 < _rtP ->
P_1421 ) { rtb_B_185_30_0 = _rtP -> P_1422 ; } else { rtb_B_185_30_0 = _rtP
-> P_1423 ; } if ( rtb_B_185_30_0 >= _rtP -> P_1425 ) { _rtB -> B_185_2641_0
= ( _rtB -> B_185_2632_0 - _rtB -> B_185_2634_0 ) * rtb_B_185_2635_0 + ( _rtB
-> B_185_2636_0 - _rtP -> P_92 * _rtDW -> UnitDelay_DSTATE ) ; } else { _rtB
-> B_185_2641_0 = _rtP -> P_1424 ; } { if ( ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_DiscreteMeanvalue5_at_outport_0_PWORK .
AQHandles || _rtDW ->
HiddenToAsyncQueue_InsertedFor_DiscreteMeanvalue5_at_outport_0_PWORK .
SlioLTF ) && ssGetLogOutput ( S ) ) { sdiSlioSdiWriteSignal ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_DiscreteMeanvalue5_at_outport_0_PWORK .
AQHandles , _rtDW ->
HiddenToAsyncQueue_InsertedFor_DiscreteMeanvalue5_at_outport_0_PWORK .
SlioLTF , 0 , ssGetTaskTime ( S , 2 ) , ( void * ) & _rtB -> B_185_2641_0 ) ;
} } rtb_B_185_2643_0 = _rtP -> P_1426 ; } isHit = ssIsSampleHit ( S , 3 , 0 )
; if ( isHit != 0 ) { _rtB -> B_185_2645_0 = _rtB -> B_185_231_0 [ 1 ] * _rtB
-> B_185_266_0 [ 0 ] - _rtB -> B_185_231_0 [ 0 ] * _rtB -> B_185_266_0 [ 1 ]
; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { if ( _rtDW ->
Integ4_SYSTEM_ENABLE_e != 0 ) { _rtB -> B_185_2646_0 = _rtDW ->
Integ4_DSTATE_i ; } else { _rtB -> B_185_2646_0 = _rtP -> P_1427 * _rtB ->
B_185_2645_0 + _rtDW -> Integ4_DSTATE_i ; } _rtB -> B_185_2647_0 = _rtP ->
P_1429 ; ssCallAccelRunBlock ( S , 185 , 2648 , SS_CALL_MDL_OUTPUTS ) ;
rtb_B_185_2649_0 = _rtDW -> UnitDelay_DSTATE_f ; } isHit = ssIsSampleHit ( S
, 3 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_2650_0 = _rtP -> P_1431 * _rtB
-> B_185_2645_0 ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) {
rtb_B_185_30_0 = ssGetTaskTime ( S , 2 ) ; if ( rtb_B_185_30_0 < _rtP ->
P_1433 ) { rtb_B_185_30_0 = _rtP -> P_1434 ; } else { rtb_B_185_30_0 = _rtP
-> P_1435 ; } if ( rtb_B_185_30_0 >= _rtP -> P_1436 ) { _rtB -> B_185_2654_0
= ( _rtB -> B_185_2646_0 - _rtB -> B_185_2648_0 ) * _rtP -> P_1432 + ( _rtB
-> B_185_2650_0 - _rtP -> P_91 * rtb_B_185_2649_0 ) ; } else { _rtB ->
B_185_2654_0 = rtb_B_185_2643_0 ; } } _rtB -> B_185_2656_0 = _rtB ->
B_185_28_0 [ 0 ] * _rtB -> B_185_71_0 [ 0 ] + _rtB -> B_185_28_0 [ 1 ] * _rtB
-> B_185_71_0 [ 1 ] ; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 )
{ if ( _rtDW -> Integ4_SYSTEM_ENABLE_b != 0 ) { _rtB -> B_185_2657_0 = _rtDW
-> Integ4_DSTATE_m ; } else { _rtB -> B_185_2657_0 = _rtP -> P_1437 * _rtB ->
B_185_2656_0 + _rtDW -> Integ4_DSTATE_m ; } _rtB -> B_185_2658_0 = _rtP ->
P_1439 ; ssCallAccelRunBlock ( S , 185 , 2659 , SS_CALL_MDL_OUTPUTS ) ; }
_rtB -> B_185_2661_0 = _rtB -> B_185_28_0 [ 1 ] * _rtB -> B_185_71_0 [ 0 ] -
_rtB -> B_185_28_0 [ 0 ] * _rtB -> B_185_71_0 [ 1 ] ; isHit = ssIsSampleHit (
S , 2 , 0 ) ; if ( isHit != 0 ) { if ( _rtDW -> Integ4_SYSTEM_ENABLE_p != 0 )
{ _rtB -> B_185_2662_0 = _rtDW -> Integ4_DSTATE_k ; } else { _rtB ->
B_185_2662_0 = _rtP -> P_1440 * _rtB -> B_185_2661_0 + _rtDW ->
Integ4_DSTATE_k ; } _rtB -> B_185_2663_0 = _rtP -> P_1442 ;
ssCallAccelRunBlock ( S , 185 , 2664 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
B_185_2666_0 = _rtB -> B_185_618_0 [ 0 ] * _rtB -> B_185_660_0 [ 0 ] + _rtB
-> B_185_618_0 [ 1 ] * _rtB -> B_185_660_0 [ 1 ] ; isHit = ssIsSampleHit ( S
, 2 , 0 ) ; if ( isHit != 0 ) { if ( _rtDW -> Integ4_SYSTEM_ENABLE_o != 0 ) {
_rtB -> B_185_2667_0 = _rtDW -> Integ4_DSTATE_k4 ; } else { _rtB ->
B_185_2667_0 = _rtP -> P_1443 * _rtB -> B_185_2666_0 + _rtDW ->
Integ4_DSTATE_k4 ; } _rtB -> B_185_2668_0 = _rtP -> P_1445 ;
ssCallAccelRunBlock ( S , 185 , 2669 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
B_185_2671_0 = _rtB -> B_185_618_0 [ 1 ] * _rtB -> B_185_660_0 [ 0 ] - _rtB
-> B_185_618_0 [ 0 ] * _rtB -> B_185_660_0 [ 1 ] ; isHit = ssIsSampleHit ( S
, 2 , 0 ) ; if ( isHit != 0 ) { if ( _rtDW -> Integ4_SYSTEM_ENABLE_b2 != 0 )
{ _rtB -> B_185_2672_0 = _rtDW -> Integ4_DSTATE_h ; } else { _rtB ->
B_185_2672_0 = _rtP -> P_1446 * _rtB -> B_185_2671_0 + _rtDW ->
Integ4_DSTATE_h ; } _rtB -> B_185_2673_0 = _rtP -> P_1448 ;
ssCallAccelRunBlock ( S , 185 , 2674 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
B_185_2676_0 = _rtB -> B_185_415_0 [ 0 ] * _rtB -> B_185_457_0 [ 0 ] + _rtB
-> B_185_415_0 [ 1 ] * _rtB -> B_185_457_0 [ 1 ] ; isHit = ssIsSampleHit ( S
, 2 , 0 ) ; if ( isHit != 0 ) { if ( _rtDW -> Integ4_SYSTEM_ENABLE_m != 0 ) {
_rtB -> B_185_2677_0 = _rtDW -> Integ4_DSTATE_n ; } else { _rtB ->
B_185_2677_0 = _rtP -> P_1449 * _rtB -> B_185_2676_0 + _rtDW ->
Integ4_DSTATE_n ; } _rtB -> B_185_2678_0 = _rtP -> P_1451 ;
ssCallAccelRunBlock ( S , 185 , 2679 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
B_185_2681_0 = _rtB -> B_185_415_0 [ 1 ] * _rtB -> B_185_457_0 [ 0 ] - _rtB
-> B_185_415_0 [ 0 ] * _rtB -> B_185_457_0 [ 1 ] ; isHit = ssIsSampleHit ( S
, 2 , 0 ) ; if ( isHit != 0 ) { if ( _rtDW -> Integ4_SYSTEM_ENABLE_f != 0 ) {
_rtB -> B_185_2682_0 = _rtDW -> Integ4_DSTATE_ho ; } else { _rtB ->
B_185_2682_0 = _rtP -> P_1452 * _rtB -> B_185_2681_0 + _rtDW ->
Integ4_DSTATE_ho ; } _rtB -> B_185_2683_0 = _rtP -> P_1454 ;
ssCallAccelRunBlock ( S , 185 , 2684 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 185 , 2685 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 185 , 2686 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 185 , 2687 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 185 , 2688 , SS_CALL_MDL_OUTPUTS ) ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { rtb_B_185_30_0 =
ssGetTaskTime ( S , 1 ) ; _rtDW -> EnableHarmonicFilters_MODE = (
rtb_B_185_30_0 >= _rtP -> P_1455 ) ; if ( _rtDW -> EnableHarmonicFilters_MODE
== 1 ) { rtb_B_185_3030_0 = _rtP -> P_1457 ; } else { rtb_B_185_3030_0 = _rtP
-> P_1456 ; } rtb_B_185_30_0 = ssGetTaskTime ( S , 1 ) ; _rtDW ->
EnergizeConverterswithstartupcurrentBypassesstartupresistors_MODE = (
rtb_B_185_30_0 >= _rtP -> P_1458 ) ; rtb_B_185_30_0 = ssGetTaskTime ( S , 1 )
; _rtDW -> EnergizegridHarmonicFilter_MODE = ( rtb_B_185_30_0 >= _rtP ->
P_1461 ) ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB
-> B_185_3183_0 = rt_Lookup ( _rtP -> P_1466 , 4 , ssGetTaskTime ( S , 2 ) ,
_rtP -> P_1467 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ if ( _rtB -> B_185_3181_0 >= _rtP -> P_1468 ) { rtb_B_185_3184_0 =
rtb_B_185_3030_0 ; } else { rtb_B_185_3184_0 = _rtB -> B_185_3183_0 ; } if (
_rtB -> B_185_3185_0 >= _rtP -> P_1471 ) { _rtB -> B_185_3187_0 =
rtb_B_185_3184_0 ; } else { _rtB -> B_185_3187_0 = _rtB -> B_185_3186_0 ; } }
_rtB -> B_185_3189_0 = rt_Lookup ( _rtP -> P_1472 , 5 , ssGetT ( S ) , _rtP
-> P_1473 ) ; if ( _rtB -> B_185_3180_0 >= _rtP -> P_1474 ) { _rtB ->
B_185_3190_0 = _rtB -> B_185_3187_0 ; } else { _rtB -> B_185_3190_0 = _rtB ->
B_185_3189_0 ; } if ( ( _rtDW -> TimeStampA_k >= ssGetT ( S ) ) && ( _rtDW ->
TimeStampB_o2 >= ssGetT ( S ) ) ) { _rtB -> B_185_3191_0 = 0.0 ; } else {
rtb_B_185_34_0 = _rtDW -> TimeStampA_k ; lastU = & _rtDW -> LastUAtTimeA_dx ;
if ( _rtDW -> TimeStampA_k < _rtDW -> TimeStampB_o2 ) { if ( _rtDW ->
TimeStampB_o2 < ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_o2 ;
lastU = & _rtDW -> LastUAtTimeB_b ; } } else { if ( _rtDW -> TimeStampA_k >=
ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_o2 ; lastU = & _rtDW ->
LastUAtTimeB_b ; } } _rtB -> B_185_3191_0 = ( _rtB -> B_185_3189_0 - * lastU
) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( isHit != 0 ) { if ( _rtB -> B_185_3197_0 >= _rtP -> P_1478 ) { _rtB ->
B_185_3198_0 = rtb_B_185_3184_0 ; } else { _rtB -> B_185_3198_0 = _rtB ->
B_185_3186_0 ; } } _rtB -> B_185_3200_0 = rt_Lookup ( _rtP -> P_1479 , 5 ,
ssGetT ( S ) , _rtP -> P_1480 ) ; if ( _rtB -> B_185_3196_0 >= _rtP -> P_1481
) { _rtB -> B_185_3201_0 = _rtB -> B_185_3198_0 ; } else { _rtB ->
B_185_3201_0 = _rtB -> B_185_3200_0 ; } if ( ( _rtDW -> TimeStampA_fd >=
ssGetT ( S ) ) && ( _rtDW -> TimeStampB_aw >= ssGetT ( S ) ) ) { _rtB ->
B_185_3202_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW -> TimeStampA_fd ; lastU
= & _rtDW -> LastUAtTimeA_m ; if ( _rtDW -> TimeStampA_fd < _rtDW ->
TimeStampB_aw ) { if ( _rtDW -> TimeStampB_aw < ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_aw ; lastU = & _rtDW -> LastUAtTimeB_e0
; } } else { if ( _rtDW -> TimeStampA_fd >= ssGetT ( S ) ) { rtb_B_185_34_0 =
_rtDW -> TimeStampB_aw ; lastU = & _rtDW -> LastUAtTimeB_e0 ; } } _rtB ->
B_185_3202_0 = ( _rtB -> B_185_3200_0 - * lastU ) / ( ssGetT ( S ) -
rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ if ( _rtB -> B_185_3208_0 >= _rtP -> P_1485 ) { _rtB -> B_185_3209_0 =
rtb_B_185_3184_0 ; } else { _rtB -> B_185_3209_0 = _rtB -> B_185_3186_0 ; } }
_rtB -> B_185_3211_0 = rt_Lookup ( _rtP -> P_1486 , 5 , ssGetT ( S ) , _rtP
-> P_1487 ) ; if ( _rtB -> B_185_3207_0 >= _rtP -> P_1488 ) { _rtB ->
B_185_3212_0 = _rtB -> B_185_3209_0 ; } else { _rtB -> B_185_3212_0 = _rtB ->
B_185_3211_0 ; } if ( ( _rtDW -> TimeStampA_kq >= ssGetT ( S ) ) && ( _rtDW
-> TimeStampB_n >= ssGetT ( S ) ) ) { _rtB -> B_185_3213_0 = 0.0 ; } else {
rtb_B_185_34_0 = _rtDW -> TimeStampA_kq ; lastU = & _rtDW -> LastUAtTimeA_nq
; if ( _rtDW -> TimeStampA_kq < _rtDW -> TimeStampB_n ) { if ( _rtDW ->
TimeStampB_n < ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_n ;
lastU = & _rtDW -> LastUAtTimeB_pd ; } } else { if ( _rtDW -> TimeStampA_kq
>= ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_n ; lastU = & _rtDW
-> LastUAtTimeB_pd ; } } _rtB -> B_185_3213_0 = ( _rtB -> B_185_3211_0 - *
lastU ) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2
, 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_3235_0 = rt_Lookup ( _rtP -> P_1492
, 4 , ssGetTaskTime ( S , 2 ) , _rtP -> P_1493 ) ; } isHit = ssIsSampleHit (
S , 1 , 0 ) ; if ( isHit != 0 ) { if ( _rtB -> B_185_3233_0 >= _rtP -> P_1494
) { rtb_B_185_3236_0 = rtb_B_185_3030_0 ; } else { rtb_B_185_3236_0 = _rtB ->
B_185_3235_0 ; } if ( _rtB -> B_185_3237_0 >= _rtP -> P_1497 ) { _rtB ->
B_185_3239_0 = rtb_B_185_3236_0 ; } else { _rtB -> B_185_3239_0 = _rtB ->
B_185_3238_0 ; } } _rtB -> B_185_3241_0 = rt_Lookup ( _rtP -> P_1498 , 5 ,
ssGetT ( S ) , _rtP -> P_1499 ) ; if ( _rtB -> B_185_3232_0 >= _rtP -> P_1500
) { _rtB -> B_185_3242_0 = _rtB -> B_185_3239_0 ; } else { _rtB ->
B_185_3242_0 = _rtB -> B_185_3241_0 ; } if ( ( _rtDW -> TimeStampA_k4 >=
ssGetT ( S ) ) && ( _rtDW -> TimeStampB_a3 >= ssGetT ( S ) ) ) { _rtB ->
B_185_3243_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW -> TimeStampA_k4 ; lastU
= & _rtDW -> LastUAtTimeA_gx ; if ( _rtDW -> TimeStampA_k4 < _rtDW ->
TimeStampB_a3 ) { if ( _rtDW -> TimeStampB_a3 < ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_a3 ; lastU = & _rtDW -> LastUAtTimeB_ea
; } } else { if ( _rtDW -> TimeStampA_k4 >= ssGetT ( S ) ) { rtb_B_185_34_0 =
_rtDW -> TimeStampB_a3 ; lastU = & _rtDW -> LastUAtTimeB_ea ; } } _rtB ->
B_185_3243_0 = ( _rtB -> B_185_3241_0 - * lastU ) / ( ssGetT ( S ) -
rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ if ( _rtB -> B_185_3249_0 >= _rtP -> P_1504 ) { _rtB -> B_185_3250_0 =
rtb_B_185_3236_0 ; } else { _rtB -> B_185_3250_0 = _rtB -> B_185_3238_0 ; } }
_rtB -> B_185_3252_0 = rt_Lookup ( _rtP -> P_1505 , 5 , ssGetT ( S ) , _rtP
-> P_1506 ) ; if ( _rtB -> B_185_3248_0 >= _rtP -> P_1507 ) { _rtB ->
B_185_3253_0 = _rtB -> B_185_3250_0 ; } else { _rtB -> B_185_3253_0 = _rtB ->
B_185_3252_0 ; } if ( ( _rtDW -> TimeStampA_jj >= ssGetT ( S ) ) && ( _rtDW
-> TimeStampB_la >= ssGetT ( S ) ) ) { _rtB -> B_185_3254_0 = 0.0 ; } else {
rtb_B_185_34_0 = _rtDW -> TimeStampA_jj ; lastU = & _rtDW -> LastUAtTimeA_ab
; if ( _rtDW -> TimeStampA_jj < _rtDW -> TimeStampB_la ) { if ( _rtDW ->
TimeStampB_la < ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_la ;
lastU = & _rtDW -> LastUAtTimeB_n ; } } else { if ( _rtDW -> TimeStampA_jj >=
ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_la ; lastU = & _rtDW ->
LastUAtTimeB_n ; } } _rtB -> B_185_3254_0 = ( _rtB -> B_185_3252_0 - * lastU
) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( isHit != 0 ) { if ( _rtB -> B_185_3260_0 >= _rtP -> P_1511 ) { _rtB ->
B_185_3261_0 = rtb_B_185_3236_0 ; } else { _rtB -> B_185_3261_0 = _rtB ->
B_185_3238_0 ; } } _rtB -> B_185_3263_0 = rt_Lookup ( _rtP -> P_1512 , 5 ,
ssGetT ( S ) , _rtP -> P_1513 ) ; if ( _rtB -> B_185_3259_0 >= _rtP -> P_1514
) { _rtB -> B_185_3264_0 = _rtB -> B_185_3261_0 ; } else { _rtB ->
B_185_3264_0 = _rtB -> B_185_3263_0 ; } if ( ( _rtDW -> TimeStampA_l >=
ssGetT ( S ) ) && ( _rtDW -> TimeStampB_jy >= ssGetT ( S ) ) ) { _rtB ->
B_185_3265_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW -> TimeStampA_l ; lastU
= & _rtDW -> LastUAtTimeA_g0 ; if ( _rtDW -> TimeStampA_l < _rtDW ->
TimeStampB_jy ) { if ( _rtDW -> TimeStampB_jy < ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_jy ; lastU = & _rtDW -> LastUAtTimeB_l ;
} } else { if ( _rtDW -> TimeStampA_l >= ssGetT ( S ) ) { rtb_B_185_34_0 =
_rtDW -> TimeStampB_jy ; lastU = & _rtDW -> LastUAtTimeB_l ; } } _rtB ->
B_185_3265_0 = ( _rtB -> B_185_3263_0 - * lastU ) / ( ssGetT ( S ) -
rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 )
{ _rtB -> B_185_3288_0 = rt_Lookup ( _rtP -> P_1518 , 4 , ssGetTaskTime ( S ,
2 ) , _rtP -> P_1519 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit
!= 0 ) { if ( _rtB -> B_185_3286_0 >= _rtP -> P_1520 ) { rtb_B_185_3289_0 =
rtb_B_185_3030_0 ; } else { rtb_B_185_3289_0 = _rtB -> B_185_3288_0 ; } if (
_rtB -> B_185_3290_0 >= _rtP -> P_1523 ) { _rtB -> B_185_3292_0 =
rtb_B_185_3289_0 ; } else { _rtB -> B_185_3292_0 = _rtB -> B_185_3291_0 ; } }
_rtB -> B_185_3294_0 = rt_Lookup ( _rtP -> P_1524 , 5 , ssGetT ( S ) , _rtP
-> P_1525 ) ; if ( _rtB -> B_185_3285_0 >= _rtP -> P_1526 ) { _rtB ->
B_185_3295_0 = _rtB -> B_185_3292_0 ; } else { _rtB -> B_185_3295_0 = _rtB ->
B_185_3294_0 ; } if ( ( _rtDW -> TimeStampA_jo >= ssGetT ( S ) ) && ( _rtDW
-> TimeStampB_fi >= ssGetT ( S ) ) ) { _rtB -> B_185_3296_0 = 0.0 ; } else {
rtb_B_185_34_0 = _rtDW -> TimeStampA_jo ; lastU = & _rtDW -> LastUAtTimeA_br
; if ( _rtDW -> TimeStampA_jo < _rtDW -> TimeStampB_fi ) { if ( _rtDW ->
TimeStampB_fi < ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_fi ;
lastU = & _rtDW -> LastUAtTimeB_ei ; } } else { if ( _rtDW -> TimeStampA_jo
>= ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_fi ; lastU = & _rtDW
-> LastUAtTimeB_ei ; } } _rtB -> B_185_3296_0 = ( _rtB -> B_185_3294_0 - *
lastU ) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 1
, 0 ) ; if ( isHit != 0 ) { if ( _rtB -> B_185_3302_0 >= _rtP -> P_1530 ) {
_rtB -> B_185_3303_0 = rtb_B_185_3289_0 ; } else { _rtB -> B_185_3303_0 =
_rtB -> B_185_3291_0 ; } } _rtB -> B_185_3305_0 = rt_Lookup ( _rtP -> P_1531
, 5 , ssGetT ( S ) , _rtP -> P_1532 ) ; if ( _rtB -> B_185_3301_0 >= _rtP ->
P_1533 ) { _rtB -> B_185_3306_0 = _rtB -> B_185_3303_0 ; } else { _rtB ->
B_185_3306_0 = _rtB -> B_185_3305_0 ; } if ( ( _rtDW -> TimeStampA_a >=
ssGetT ( S ) ) && ( _rtDW -> TimeStampB_b >= ssGetT ( S ) ) ) { _rtB ->
B_185_3307_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW -> TimeStampA_a ; lastU
= & _rtDW -> LastUAtTimeA_g1 ; if ( _rtDW -> TimeStampA_a < _rtDW ->
TimeStampB_b ) { if ( _rtDW -> TimeStampB_b < ssGetT ( S ) ) { rtb_B_185_34_0
= _rtDW -> TimeStampB_b ; lastU = & _rtDW -> LastUAtTimeB_m ; } } else { if (
_rtDW -> TimeStampA_a >= ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW ->
TimeStampB_b ; lastU = & _rtDW -> LastUAtTimeB_m ; } } _rtB -> B_185_3307_0 =
( _rtB -> B_185_3305_0 - * lastU ) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; }
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( _rtB ->
B_185_3313_0 >= _rtP -> P_1537 ) { _rtB -> B_185_3314_0 = rtb_B_185_3289_0 ;
} else { _rtB -> B_185_3314_0 = _rtB -> B_185_3291_0 ; } } _rtB ->
B_185_3316_0 = rt_Lookup ( _rtP -> P_1538 , 5 , ssGetT ( S ) , _rtP -> P_1539
) ; if ( _rtB -> B_185_3312_0 >= _rtP -> P_1540 ) { _rtB -> B_185_3317_0 =
_rtB -> B_185_3314_0 ; } else { _rtB -> B_185_3317_0 = _rtB -> B_185_3316_0 ;
} if ( ( _rtDW -> TimeStampA_oa >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB_g
>= ssGetT ( S ) ) ) { _rtB -> B_185_3318_0 = 0.0 ; } else { rtb_B_185_34_0 =
_rtDW -> TimeStampA_oa ; lastU = & _rtDW -> LastUAtTimeA_j ; if ( _rtDW ->
TimeStampA_oa < _rtDW -> TimeStampB_g ) { if ( _rtDW -> TimeStampB_g < ssGetT
( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_g ; lastU = & _rtDW ->
LastUAtTimeB_gx ; } } else { if ( _rtDW -> TimeStampA_oa >= ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_g ; lastU = & _rtDW -> LastUAtTimeB_gx ;
} } _rtB -> B_185_3318_0 = ( _rtB -> B_185_3316_0 - * lastU ) / ( ssGetT ( S
) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit !=
0 ) { _rtB -> B_185_3341_0 = rt_Lookup ( _rtP -> P_1544 , 4 , ssGetTaskTime (
S , 2 ) , _rtP -> P_1545 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { if ( _rtB -> B_185_3339_0 >= _rtP -> P_1546 ) {
rtb_B_185_3342_0 = rtb_B_185_3030_0 ; } else { rtb_B_185_3342_0 = _rtB ->
B_185_3341_0 ; } if ( _rtB -> B_185_3343_0 >= _rtP -> P_1549 ) { _rtB ->
B_185_3345_0 = rtb_B_185_3342_0 ; } else { _rtB -> B_185_3345_0 = _rtB ->
B_185_3344_0 ; } } _rtB -> B_185_3347_0 = rt_Lookup ( _rtP -> P_1550 , 5 ,
ssGetT ( S ) , _rtP -> P_1551 ) ; if ( _rtB -> B_185_3338_0 >= _rtP -> P_1552
) { _rtB -> B_185_3348_0 = _rtB -> B_185_3345_0 ; } else { _rtB ->
B_185_3348_0 = _rtB -> B_185_3347_0 ; } if ( ( _rtDW -> TimeStampA_h >=
ssGetT ( S ) ) && ( _rtDW -> TimeStampB_jo >= ssGetT ( S ) ) ) { _rtB ->
B_185_3349_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW -> TimeStampA_h ; lastU
= & _rtDW -> LastUAtTimeA_gxl ; if ( _rtDW -> TimeStampA_h < _rtDW ->
TimeStampB_jo ) { if ( _rtDW -> TimeStampB_jo < ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_jo ; lastU = & _rtDW -> LastUAtTimeB_pb
; } } else { if ( _rtDW -> TimeStampA_h >= ssGetT ( S ) ) { rtb_B_185_34_0 =
_rtDW -> TimeStampB_jo ; lastU = & _rtDW -> LastUAtTimeB_pb ; } } _rtB ->
B_185_3349_0 = ( _rtB -> B_185_3347_0 - * lastU ) / ( ssGetT ( S ) -
rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ if ( _rtB -> B_185_3355_0 >= _rtP -> P_1556 ) { _rtB -> B_185_3356_0 =
rtb_B_185_3342_0 ; } else { _rtB -> B_185_3356_0 = _rtB -> B_185_3344_0 ; } }
_rtB -> B_185_3358_0 = rt_Lookup ( _rtP -> P_1557 , 5 , ssGetT ( S ) , _rtP
-> P_1558 ) ; if ( _rtB -> B_185_3354_0 >= _rtP -> P_1559 ) { _rtB ->
B_185_3359_0 = _rtB -> B_185_3356_0 ; } else { _rtB -> B_185_3359_0 = _rtB ->
B_185_3358_0 ; } if ( ( _rtDW -> TimeStampA_b >= ssGetT ( S ) ) && ( _rtDW ->
TimeStampB_e >= ssGetT ( S ) ) ) { _rtB -> B_185_3360_0 = 0.0 ; } else {
rtb_B_185_34_0 = _rtDW -> TimeStampA_b ; lastU = & _rtDW -> LastUAtTimeA_ea ;
if ( _rtDW -> TimeStampA_b < _rtDW -> TimeStampB_e ) { if ( _rtDW ->
TimeStampB_e < ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_e ;
lastU = & _rtDW -> LastUAtTimeB_a ; } } else { if ( _rtDW -> TimeStampA_b >=
ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_e ; lastU = & _rtDW ->
LastUAtTimeB_a ; } } _rtB -> B_185_3360_0 = ( _rtB -> B_185_3358_0 - * lastU
) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( isHit != 0 ) { if ( _rtB -> B_185_3366_0 >= _rtP -> P_1563 ) { _rtB ->
B_185_3367_0 = rtb_B_185_3342_0 ; } else { _rtB -> B_185_3367_0 = _rtB ->
B_185_3344_0 ; } } _rtB -> B_185_3369_0 = rt_Lookup ( _rtP -> P_1564 , 5 ,
ssGetT ( S ) , _rtP -> P_1565 ) ; if ( _rtB -> B_185_3365_0 >= _rtP -> P_1566
) { _rtB -> B_185_3370_0 = _rtB -> B_185_3367_0 ; } else { _rtB ->
B_185_3370_0 = _rtB -> B_185_3369_0 ; } if ( ( _rtDW -> TimeStampA_ns >=
ssGetT ( S ) ) && ( _rtDW -> TimeStampB_ce >= ssGetT ( S ) ) ) { _rtB ->
B_185_3371_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW -> TimeStampA_ns ; lastU
= & _rtDW -> LastUAtTimeA_l ; if ( _rtDW -> TimeStampA_ns < _rtDW ->
TimeStampB_ce ) { if ( _rtDW -> TimeStampB_ce < ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_ce ; lastU = & _rtDW -> LastUAtTimeB_eq4
; } } else { if ( _rtDW -> TimeStampA_ns >= ssGetT ( S ) ) { rtb_B_185_34_0 =
_rtDW -> TimeStampB_ce ; lastU = & _rtDW -> LastUAtTimeB_eq4 ; } } _rtB ->
B_185_3371_0 = ( _rtB -> B_185_3369_0 - * lastU ) / ( ssGetT ( S ) -
rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ rtb_B_185_30_0 = ssGetTaskTime ( S , 1 ) ; _rtDW ->
Switchonloadnetwork_MODE = ( rtb_B_185_30_0 >= _rtP -> P_1569 ) ; if ( _rtDW
-> Switchonloadnetwork_MODE == 1 ) { rtb_B_185_3392_0 = _rtP -> P_1571 ; }
else { rtb_B_185_3392_0 = _rtP -> P_1570 ; } } isHit = ssIsSampleHit ( S , 2
, 0 ) ; if ( isHit != 0 ) { _rtB -> B_185_3395_0 = rt_Lookup ( _rtP -> P_1573
, 4 , ssGetTaskTime ( S , 2 ) , _rtP -> P_1574 ) ; } isHit = ssIsSampleHit (
S , 1 , 0 ) ; if ( isHit != 0 ) { if ( _rtB -> B_185_3393_0 >= _rtP -> P_1575
) { rtb_B_185_3396_0 = rtb_B_185_3392_0 ; } else { rtb_B_185_3396_0 = _rtB ->
B_185_3395_0 ; } if ( _rtB -> B_185_3397_0 >= _rtP -> P_1578 ) { _rtB ->
B_185_3399_0 = rtb_B_185_3396_0 ; } else { _rtB -> B_185_3399_0 = _rtB ->
B_185_3398_0 ; } } _rtB -> B_185_3401_0 = rt_Lookup ( _rtP -> P_1579 , 5 ,
ssGetT ( S ) , _rtP -> P_1580 ) ; if ( _rtB -> B_185_3391_0 >= _rtP -> P_1581
) { _rtB -> B_185_3402_0 = _rtB -> B_185_3399_0 ; } else { _rtB ->
B_185_3402_0 = _rtB -> B_185_3401_0 ; } if ( ( _rtDW -> TimeStampA_kp >=
ssGetT ( S ) ) && ( _rtDW -> TimeStampB_d >= ssGetT ( S ) ) ) { _rtB ->
B_185_3403_0 = 0.0 ; } else { rtb_B_185_34_0 = _rtDW -> TimeStampA_kp ; lastU
= & _rtDW -> LastUAtTimeA_i ; if ( _rtDW -> TimeStampA_kp < _rtDW ->
TimeStampB_d ) { if ( _rtDW -> TimeStampB_d < ssGetT ( S ) ) { rtb_B_185_34_0
= _rtDW -> TimeStampB_d ; lastU = & _rtDW -> LastUAtTimeB_o1 ; } } else { if
( _rtDW -> TimeStampA_kp >= ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW ->
TimeStampB_d ; lastU = & _rtDW -> LastUAtTimeB_o1 ; } } _rtB -> B_185_3403_0
= ( _rtB -> B_185_3401_0 - * lastU ) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; }
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( _rtB ->
B_185_3409_0 >= _rtP -> P_1585 ) { _rtB -> B_185_3410_0 = rtb_B_185_3396_0 ;
} else { _rtB -> B_185_3410_0 = _rtB -> B_185_3398_0 ; } } _rtB ->
B_185_3412_0 = rt_Lookup ( _rtP -> P_1586 , 5 , ssGetT ( S ) , _rtP -> P_1587
) ; if ( _rtB -> B_185_3408_0 >= _rtP -> P_1588 ) { _rtB -> B_185_3413_0 =
_rtB -> B_185_3410_0 ; } else { _rtB -> B_185_3413_0 = _rtB -> B_185_3412_0 ;
} if ( ( _rtDW -> TimeStampA_j5 >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB_av
>= ssGetT ( S ) ) ) { _rtB -> B_185_3414_0 = 0.0 ; } else { rtb_B_185_34_0 =
_rtDW -> TimeStampA_j5 ; lastU = & _rtDW -> LastUAtTimeA_gw ; if ( _rtDW ->
TimeStampA_j5 < _rtDW -> TimeStampB_av ) { if ( _rtDW -> TimeStampB_av <
ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_av ; lastU = & _rtDW ->
LastUAtTimeB_ol ; } } else { if ( _rtDW -> TimeStampA_j5 >= ssGetT ( S ) ) {
rtb_B_185_34_0 = _rtDW -> TimeStampB_av ; lastU = & _rtDW -> LastUAtTimeB_ol
; } } _rtB -> B_185_3414_0 = ( _rtB -> B_185_3412_0 - * lastU ) / ( ssGetT (
S ) - rtb_B_185_34_0 ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit
!= 0 ) { if ( _rtB -> B_185_3420_0 >= _rtP -> P_1592 ) { _rtB -> B_185_3421_0
= rtb_B_185_3396_0 ; } else { _rtB -> B_185_3421_0 = _rtB -> B_185_3398_0 ; }
} _rtB -> B_185_3423_0 = rt_Lookup ( _rtP -> P_1593 , 5 , ssGetT ( S ) , _rtP
-> P_1594 ) ; if ( _rtB -> B_185_3419_0 >= _rtP -> P_1595 ) { _rtB ->
B_185_3424_0 = _rtB -> B_185_3421_0 ; } else { _rtB -> B_185_3424_0 = _rtB ->
B_185_3423_0 ; } if ( ( _rtDW -> TimeStampA_pp >= ssGetT ( S ) ) && ( _rtDW
-> TimeStampB_ec >= ssGetT ( S ) ) ) { _rtB -> B_185_3425_0 = 0.0 ; } else {
rtb_B_185_34_0 = _rtDW -> TimeStampA_pp ; lastU = & _rtDW -> LastUAtTimeA_hk
; if ( _rtDW -> TimeStampA_pp < _rtDW -> TimeStampB_ec ) { if ( _rtDW ->
TimeStampB_ec < ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_ec ;
lastU = & _rtDW -> LastUAtTimeB_ej ; } } else { if ( _rtDW -> TimeStampA_pp
>= ssGetT ( S ) ) { rtb_B_185_34_0 = _rtDW -> TimeStampB_ec ; lastU = & _rtDW
-> LastUAtTimeB_ej ; } } _rtB -> B_185_3425_0 = ( _rtB -> B_185_3423_0 - *
lastU ) / ( ssGetT ( S ) - rtb_B_185_34_0 ) ; } UNUSED_PARAMETER ( tid ) ; }
static void mdlOutputsTID4 ( SimStruct * S , int_T tid ) {
B_normal_operation_no_faults_T * _rtB ; P_normal_operation_no_faults_T * _rtP
; DW_normal_operation_no_faults_T * _rtDW ; _rtDW = ( (
DW_normal_operation_no_faults_T * ) ssGetRootDWork ( S ) ) ; _rtP = ( (
P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_normal_operation_no_faults_T * ) _ssGetModelBlockIO ( S ) ) ; memcpy ( &
_rtB -> B_185_0_0 [ 0 ] , & _rtP -> P_109 [ 0 ] , 95U * sizeof ( real_T ) ) ;
_rtB -> B_185_1_0 [ 0 ] = _rtP -> P_110 [ 0 ] ; _rtB -> B_185_1_0 [ 1 ] =
_rtP -> P_110 [ 1 ] ; _rtB -> B_185_8_0 = _rtP -> P_116 ; _rtB -> B_185_15_0
= ( uint8_T ) ( _rtP -> P_120 == _rtP -> P_121 ) ; _rtB -> B_185_18_0 = (
uint8_T ) ( _rtP -> P_120 == _rtP -> P_122 ) ; _rtB -> B_185_21_0 = _rtP ->
P_123 ; _rtB -> B_185_32_0 = _rtP -> P_133 ; _rtB -> B_185_35_0 = _rtP ->
P_137 ; _rtB -> B_185_39_0 = _rtP -> P_141 ; _rtB -> B_185_41_0 = _rtP ->
P_142 ; _rtB -> B_185_46_0 = _rtP -> P_144 ; _rtB -> B_185_66_0 = ( uint8_T )
( _rtP -> P_155 == _rtP -> P_156 ) ; _rtB -> B_185_69_0 = ( uint8_T ) ( _rtP
-> P_155 == _rtP -> P_157 ) ; _rtB -> B_185_74_0 = _rtP -> P_159 ; _rtB ->
B_185_76_0 = _rtP -> P_160 ; _rtB -> B_185_81_0 = _rtP -> P_162 ; _rtB ->
B_185_94_0 = _rtP -> P_175 ;
normal_operation_no_faults_ACVoltageControllerTID4 ( S , & _rtB ->
ACVoltageController , & _rtDW -> ACVoltageController , & _rtP ->
ACVoltageController ) ; _rtB -> B_185_109_0 = _rtP -> P_181 ; _rtB ->
B_185_113_0 = ( uint8_T ) ( _rtP -> P_184 == _rtP -> P_185 ) ; _rtB ->
B_185_116_0 = ( uint8_T ) ( _rtP -> P_184 == _rtP -> P_186 ) ;
normal_operation_no_faults_ZeroSequenceControllerTID4 ( S , & _rtB ->
ZeroSequenceController2 , & _rtDW -> ZeroSequenceController2 , & _rtP ->
ZeroSequenceController2 ) ; _rtB -> B_185_121_0 = _rtP -> P_187 ;
normal_operation_no_faults_CirculatingcurrentsuppressionTID4 ( S , & _rtB ->
Circulatingcurrentsuppression , & _rtDW -> Circulatingcurrentsuppression , &
_rtP -> Circulatingcurrentsuppression ) ; _rtB -> B_185_131_0 = _rtP -> P_194
; _rtB -> B_185_132_0 = _rtP -> P_195 ; _rtB -> B_185_133_0 = _rtP -> P_196 ;
_rtB -> B_185_144_0 = _rtP -> P_203 ; _rtB -> B_185_145_0 = _rtP -> P_204 ;
_rtB -> B_185_146_0 = _rtP -> P_205 ; _rtB -> B_185_157_0 = _rtP -> P_212 ;
_rtB -> B_185_158_0 = _rtP -> P_213 ; _rtB -> B_185_159_0 = _rtP -> P_214 ;
_rtB -> B_185_170_0 = _rtP -> P_221 ; _rtB -> B_185_171_0 = _rtP -> P_222 ;
_rtB -> B_185_172_0 = _rtP -> P_223 ; _rtB -> B_185_183_0 = _rtP -> P_230 ;
_rtB -> B_185_184_0 = _rtP -> P_231 ; _rtB -> B_185_185_0 = _rtP -> P_232 ;
memcpy ( & _rtB -> B_185_130_0 [ 0 ] , & _rtP -> P_193 [ 0 ] , 50U * sizeof (
real_T ) ) ; memcpy ( & _rtB -> B_185_143_0 [ 0 ] , & _rtP -> P_202 [ 0 ] ,
50U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_185_156_0 [ 0 ] , & _rtP ->
P_211 [ 0 ] , 50U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_185_169_0 [ 0
] , & _rtP -> P_220 [ 0 ] , 50U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_185_182_0 [ 0 ] , & _rtP -> P_229 [ 0 ] , 50U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_185_195_0 [ 0 ] , & _rtP -> P_238 [ 0 ] , 50U * sizeof (
real_T ) ) ; _rtB -> B_185_196_0 = _rtP -> P_239 ; _rtB -> B_185_197_0 = _rtP
-> P_240 ; _rtB -> B_185_198_0 = _rtP -> P_241 ; _rtB -> B_185_207_0 [ 0 ] =
_rtP -> P_247 [ 0 ] ; _rtB -> B_185_207_0 [ 1 ] = _rtP -> P_247 [ 1 ] ; _rtB
-> B_185_218_0 = ( uint8_T ) ( _rtP -> P_254 == _rtP -> P_255 ) ; _rtB ->
B_185_221_0 = ( uint8_T ) ( _rtP -> P_254 == _rtP -> P_256 ) ; _rtB ->
B_185_224_0 = _rtP -> P_257 ; _rtB -> B_185_232_0 = _rtP -> P_263 ; _rtB ->
B_185_237_0 = _rtP -> P_269 ; _rtB -> B_185_239_0 = _rtP -> P_270 ; _rtB ->
B_185_244_0 = _rtP -> P_272 ; _rtB -> B_185_261_0 = ( uint8_T ) ( _rtP ->
P_283 == _rtP -> P_284 ) ; _rtB -> B_185_264_0 = ( uint8_T ) ( _rtP -> P_283
== _rtP -> P_285 ) ; _rtB -> B_17_0_0 = _rtP -> P_45 ; _rtB -> B_17_1_0 =
_rtP -> P_46 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( _rtDW ->
ReactivepowerSetpoint_SubsysRanBC ) ; } _rtB -> B_185_287_0 = _rtP -> P_297 ;
_rtB -> B_185_291_0 = ( uint8_T ) ( _rtP -> P_300 == _rtP -> P_301 ) ; _rtB
-> B_185_294_0 = ( uint8_T ) ( _rtP -> P_300 == _rtP -> P_302 ) ;
normal_operation_no_faults_ZeroSequenceControllerTID4 ( S , & _rtB ->
ZeroSequenceController , & _rtDW -> ZeroSequenceController , & _rtP ->
ZeroSequenceController ) ; _rtB -> B_185_299_0 = _rtP -> P_303 ; _rtB ->
B_4_0_0 = _rtP -> P_1 ; _rtB -> B_4_1_0 = _rtP -> P_2 ; _rtB -> B_4_2_0 =
_rtP -> P_3 ; _rtB -> B_4_3_0 = _rtP -> P_4 ; _rtB -> B_4_20_0 = ( uint8_T )
( _rtP -> P_13 == _rtP -> P_14 ) ; _rtB -> B_4_23_0 = ( uint8_T ) ( _rtP ->
P_13 == _rtP -> P_15 ) ; _rtB -> B_4_39_0 = _rtP -> P_24 ; _rtB -> B_4_48_0 =
( uint8_T ) ( _rtP -> P_27 == _rtP -> P_28 ) ; _rtB -> B_4_51_0 = ( uint8_T )
( _rtP -> P_27 == _rtP -> P_29 ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( _rtDW -> Circulatingcurrentsuppression1_SubsysRanBC ) ; } _rtB
-> B_185_309_0 = _rtP -> P_307 ; _rtB -> B_185_311_0 = _rtP -> P_308 ; _rtB
-> B_185_314_0 = _rtP -> P_311 ; _rtB -> B_185_325_0 = _rtP -> P_318 ; _rtB
-> B_185_326_0 = _rtP -> P_319 ; _rtB -> B_185_327_0 = _rtP -> P_320 ; _rtB
-> B_185_338_0 = _rtP -> P_327 ; _rtB -> B_185_339_0 = _rtP -> P_328 ; _rtB
-> B_185_340_0 = _rtP -> P_329 ; _rtB -> B_185_351_0 = _rtP -> P_336 ; _rtB
-> B_185_352_0 = _rtP -> P_337 ; _rtB -> B_185_353_0 = _rtP -> P_338 ; _rtB
-> B_185_364_0 = _rtP -> P_345 ; _rtB -> B_185_365_0 = _rtP -> P_346 ; _rtB
-> B_185_366_0 = _rtP -> P_347 ; memcpy ( & _rtB -> B_185_307_0 [ 0 ] , &
_rtP -> P_306 [ 0 ] , 50U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_185_324_0 [ 0 ] , & _rtP -> P_317 [ 0 ] , 50U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_185_337_0 [ 0 ] , & _rtP -> P_326 [ 0 ] , 50U * sizeof (
real_T ) ) ; memcpy ( & _rtB -> B_185_350_0 [ 0 ] , & _rtP -> P_335 [ 0 ] ,
50U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_185_363_0 [ 0 ] , & _rtP ->
P_344 [ 0 ] , 50U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_185_376_0 [ 0
] , & _rtP -> P_353 [ 0 ] , 50U * sizeof ( real_T ) ) ; _rtB -> B_185_377_0 =
_rtP -> P_354 ; _rtB -> B_185_378_0 = _rtP -> P_355 ; _rtB -> B_185_379_0 =
_rtP -> P_356 ; _rtB -> B_185_388_0 [ 0 ] = _rtP -> P_362 [ 0 ] ; _rtB ->
B_185_388_0 [ 1 ] = _rtP -> P_362 [ 1 ] ; _rtB -> B_185_395_0 = _rtP -> P_368
; _rtB -> B_185_402_0 = ( uint8_T ) ( _rtP -> P_372 == _rtP -> P_373 ) ; _rtB
-> B_185_405_0 = ( uint8_T ) ( _rtP -> P_372 == _rtP -> P_374 ) ; _rtB ->
B_185_408_0 = _rtP -> P_375 ; _rtB -> B_185_419_0 = _rtP -> P_385 ; _rtB ->
B_185_421_0 = _rtP -> P_386 ; _rtB -> B_185_425_0 = _rtP -> P_390 ; _rtB ->
B_185_427_0 = _rtP -> P_391 ; _rtB -> B_185_432_0 = _rtP -> P_393 ; _rtB ->
B_185_452_0 = ( uint8_T ) ( _rtP -> P_404 == _rtP -> P_405 ) ; _rtB ->
B_185_455_0 = ( uint8_T ) ( _rtP -> P_404 == _rtP -> P_406 ) ; _rtB ->
B_185_460_0 = _rtP -> P_408 ; _rtB -> B_185_462_0 = _rtP -> P_409 ; _rtB ->
B_185_467_0 = _rtP -> P_411 ; _rtB -> B_185_479_0 = _rtP -> P_421 ;
normal_operation_no_faults_ACVoltageControllerTID4 ( S , & _rtB ->
ACVoltageController_n , & _rtDW -> ACVoltageController_n , & _rtP ->
ACVoltageController_n ) ; _rtB -> B_185_494_0 = _rtP -> P_427 ; _rtB ->
B_185_498_0 = ( uint8_T ) ( _rtP -> P_430 == _rtP -> P_431 ) ; _rtB ->
B_185_501_0 = ( uint8_T ) ( _rtP -> P_430 == _rtP -> P_432 ) ;
normal_operation_no_faults_ZeroSequenceControllerTID4 ( S , & _rtB ->
ZeroSequenceController2_f , & _rtDW -> ZeroSequenceController2_f , & _rtP ->
ZeroSequenceController2_f ) ; _rtB -> B_185_506_0 = _rtP -> P_433 ;
normal_operation_no_faults_CirculatingcurrentsuppressionTID4 ( S , & _rtB ->
Circulatingcurrentsuppression_d , & _rtDW -> Circulatingcurrentsuppression_d
, & _rtP -> Circulatingcurrentsuppression_d ) ; _rtB -> B_185_515_0 = _rtP ->
P_437 ; _rtB -> B_185_516_0 = _rtP -> P_438 ; _rtB -> B_185_517_0 = _rtP ->
P_439 ; _rtB -> B_185_528_0 = _rtP -> P_446 ; _rtB -> B_185_529_0 = _rtP ->
P_447 ; _rtB -> B_185_530_0 = _rtP -> P_448 ; _rtB -> B_185_541_0 = _rtP ->
P_455 ; _rtB -> B_185_542_0 = _rtP -> P_456 ; _rtB -> B_185_543_0 = _rtP ->
P_457 ; _rtB -> B_185_554_0 = _rtP -> P_464 ; _rtB -> B_185_555_0 = _rtP ->
P_465 ; _rtB -> B_185_556_0 = _rtP -> P_466 ; _rtB -> B_185_567_0 = _rtP ->
P_473 ; _rtB -> B_185_568_0 = _rtP -> P_474 ; _rtB -> B_185_569_0 = _rtP ->
P_475 ; memcpy ( & _rtB -> B_185_514_0 [ 0 ] , & _rtP -> P_436 [ 0 ] , 50U *
sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_185_527_0 [ 0 ] , & _rtP -> P_445
[ 0 ] , 50U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_185_540_0 [ 0 ] , &
_rtP -> P_454 [ 0 ] , 50U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_185_553_0 [ 0 ] , & _rtP -> P_463 [ 0 ] , 50U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_185_566_0 [ 0 ] , & _rtP -> P_472 [ 0 ] , 50U * sizeof (
real_T ) ) ; memcpy ( & _rtB -> B_185_579_0 [ 0 ] , & _rtP -> P_481 [ 0 ] ,
50U * sizeof ( real_T ) ) ; _rtB -> B_185_580_0 = _rtP -> P_482 ; _rtB ->
B_185_581_0 = _rtP -> P_483 ; _rtB -> B_185_582_0 = _rtP -> P_484 ; _rtB ->
B_185_591_0 [ 0 ] = _rtP -> P_490 [ 0 ] ; _rtB -> B_185_591_0 [ 1 ] = _rtP ->
P_490 [ 1 ] ; _rtB -> B_185_598_0 = _rtP -> P_496 ; _rtB -> B_185_605_0 = (
uint8_T ) ( _rtP -> P_500 == _rtP -> P_501 ) ; _rtB -> B_185_608_0 = (
uint8_T ) ( _rtP -> P_500 == _rtP -> P_502 ) ; _rtB -> B_185_611_0 = _rtP ->
P_503 ; _rtB -> B_185_622_0 = _rtP -> P_513 ; _rtB -> B_185_624_0 = _rtP ->
P_514 ; _rtB -> B_185_628_0 = _rtP -> P_518 ; _rtB -> B_185_630_0 = _rtP ->
P_519 ; _rtB -> B_185_635_0 = _rtP -> P_521 ; _rtB -> B_185_655_0 = ( uint8_T
) ( _rtP -> P_532 == _rtP -> P_533 ) ; _rtB -> B_185_658_0 = ( uint8_T ) (
_rtP -> P_532 == _rtP -> P_534 ) ; _rtB -> B_185_663_0 = _rtP -> P_536 ; _rtB
-> B_185_665_0 = _rtP -> P_537 ; _rtB -> B_185_670_0 = _rtP -> P_539 ; _rtB
-> B_185_682_0 = _rtP -> P_549 ;
normal_operation_no_faults_ACVoltageControllerTID4 ( S , & _rtB ->
ACVoltageController_l , & _rtDW -> ACVoltageController_l , & _rtP ->
ACVoltageController_l ) ; _rtB -> B_185_697_0 = _rtP -> P_555 ; _rtB ->
B_185_701_0 = ( uint8_T ) ( _rtP -> P_558 == _rtP -> P_559 ) ; _rtB ->
B_185_704_0 = ( uint8_T ) ( _rtP -> P_558 == _rtP -> P_560 ) ;
normal_operation_no_faults_ZeroSequenceControllerTID4 ( S , & _rtB ->
ZeroSequenceController2_fb , & _rtDW -> ZeroSequenceController2_fb , & _rtP
-> ZeroSequenceController2_fb ) ; _rtB -> B_185_709_0 = _rtP -> P_561 ;
normal_operation_no_faults_CirculatingcurrentsuppressionTID4 ( S , & _rtB ->
Circulatingcurrentsuppression_k , & _rtDW -> Circulatingcurrentsuppression_k
, & _rtP -> Circulatingcurrentsuppression_k ) ; _rtB -> B_185_718_0 = _rtP ->
P_565 ; _rtB -> B_185_719_0 = _rtP -> P_566 ; _rtB -> B_185_720_0 = _rtP ->
P_567 ; _rtB -> B_185_731_0 = _rtP -> P_574 ; _rtB -> B_185_732_0 = _rtP ->
P_575 ; _rtB -> B_185_733_0 = _rtP -> P_576 ; _rtB -> B_185_744_0 = _rtP ->
P_583 ; _rtB -> B_185_745_0 = _rtP -> P_584 ; _rtB -> B_185_746_0 = _rtP ->
P_585 ; _rtB -> B_185_757_0 = _rtP -> P_592 ; _rtB -> B_185_758_0 = _rtP ->
P_593 ; _rtB -> B_185_759_0 = _rtP -> P_594 ; _rtB -> B_185_770_0 = _rtP ->
P_601 ; _rtB -> B_185_771_0 = _rtP -> P_602 ; _rtB -> B_185_772_0 = _rtP ->
P_603 ; memcpy ( & _rtB -> B_185_717_0 [ 0 ] , & _rtP -> P_564 [ 0 ] , 50U *
sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_185_730_0 [ 0 ] , & _rtP -> P_573
[ 0 ] , 50U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_185_743_0 [ 0 ] , &
_rtP -> P_582 [ 0 ] , 50U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_185_756_0 [ 0 ] , & _rtP -> P_591 [ 0 ] , 50U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_185_769_0 [ 0 ] , & _rtP -> P_600 [ 0 ] , 50U * sizeof (
real_T ) ) ; memcpy ( & _rtB -> B_185_782_0 [ 0 ] , & _rtP -> P_609 [ 0 ] ,
50U * sizeof ( real_T ) ) ; _rtB -> B_185_783_0 = _rtP -> P_610 ; _rtB ->
B_185_784_0 = _rtP -> P_611 ; _rtB -> B_185_785_0 = _rtP -> P_612 ;
normal_operation_no_faults_SignalgeneratorTID4 ( S , & _rtB ->
Signalgenerator , & _rtDW -> Signalgenerator , & _rtP -> Signalgenerator ) ;
_rtB -> B_185_796_0 = _rtP -> P_619 ; _rtB -> B_185_799_0 = _rtP -> P_621 ;
_rtB -> B_185_803_0 = _rtP -> P_624 * _rtP -> P_623 ; _rtB -> B_185_807_0 =
_rtP -> P_626 * _rtP -> P_625 ; _rtB -> B_185_808_0 [ 0 ] = _rtP -> P_627 [ 0
] ; _rtB -> B_185_808_0 [ 1 ] = _rtP -> P_627 [ 1 ] ; _rtB -> B_185_808_0 [ 2
] = _rtP -> P_627 [ 2 ] ; normal_operation_no_faults_HarmonicGeneratorTID4 (
S , & _rtB -> HarmonicGenerator , & _rtDW -> HarmonicGenerator , & _rtP ->
HarmonicGenerator ) ; normal_operation_no_faults_SignalgeneratorTID4 ( S , &
_rtB -> Signalgenerator_d , & _rtDW -> Signalgenerator_d , & _rtP ->
Signalgenerator_d ) ; _rtB -> B_185_818_0 = _rtP -> P_630 ; _rtB ->
B_185_821_0 = _rtP -> P_632 ; _rtB -> B_185_825_0 = _rtP -> P_635 * _rtP ->
P_634 ; _rtB -> B_185_829_0 = _rtP -> P_637 * _rtP -> P_636 ; _rtB ->
B_185_830_0 [ 0 ] = _rtP -> P_638 [ 0 ] ; _rtB -> B_185_830_0 [ 1 ] = _rtP ->
P_638 [ 1 ] ; _rtB -> B_185_830_0 [ 2 ] = _rtP -> P_638 [ 2 ] ;
normal_operation_no_faults_HarmonicGeneratorTID4 ( S , & _rtB ->
HarmonicGenerator_d , & _rtDW -> HarmonicGenerator_d , & _rtP ->
HarmonicGenerator_d ) ; normal_operation_no_faults_SignalgeneratorTID4 ( S ,
& _rtB -> Signalgenerator_p , & _rtDW -> Signalgenerator_p , & _rtP ->
Signalgenerator_p ) ; _rtB -> B_185_840_0 = _rtP -> P_641 ; _rtB ->
B_185_843_0 = _rtP -> P_643 ; _rtB -> B_185_847_0 = _rtP -> P_646 * _rtP ->
P_645 ; _rtB -> B_185_851_0 = _rtP -> P_648 * _rtP -> P_647 ; _rtB ->
B_185_852_0 [ 0 ] = _rtP -> P_649 [ 0 ] ; _rtB -> B_185_852_0 [ 1 ] = _rtP ->
P_649 [ 1 ] ; _rtB -> B_185_852_0 [ 2 ] = _rtP -> P_649 [ 2 ] ;
normal_operation_no_faults_HarmonicGeneratorTID4 ( S , & _rtB ->
HarmonicGenerator_c , & _rtDW -> HarmonicGenerator_c , & _rtP ->
HarmonicGenerator_c ) ; normal_operation_no_faults_SignalgeneratorTID4 ( S ,
& _rtB -> Signalgenerator_e , & _rtDW -> Signalgenerator_e , & _rtP ->
Signalgenerator_e ) ; _rtB -> B_185_862_0 = _rtP -> P_652 ; _rtB ->
B_185_865_0 = _rtP -> P_654 ; _rtB -> B_185_869_0 = _rtP -> P_657 * _rtP ->
P_656 ; _rtB -> B_185_873_0 = _rtP -> P_659 * _rtP -> P_658 ; _rtB ->
B_185_874_0 [ 0 ] = _rtP -> P_660 [ 0 ] ; _rtB -> B_185_874_0 [ 1 ] = _rtP ->
P_660 [ 1 ] ; _rtB -> B_185_874_0 [ 2 ] = _rtP -> P_660 [ 2 ] ;
normal_operation_no_faults_HarmonicGeneratorTID4 ( S , & _rtB ->
HarmonicGenerator_b , & _rtDW -> HarmonicGenerator_b , & _rtP ->
HarmonicGenerator_b ) ; normal_operation_no_faults_SignalgeneratorTID4 ( S ,
& _rtB -> Signalgenerator_m , & _rtDW -> Signalgenerator_m , & _rtP ->
Signalgenerator_m ) ; _rtB -> B_185_884_0 = _rtP -> P_663 ; _rtB ->
B_185_887_0 = _rtP -> P_665 ; _rtB -> B_185_891_0 = _rtP -> P_668 * _rtP ->
P_667 ; _rtB -> B_185_895_0 = _rtP -> P_670 * _rtP -> P_669 ; _rtB ->
B_185_896_0 [ 0 ] = _rtP -> P_671 [ 0 ] ; _rtB -> B_185_896_0 [ 1 ] = _rtP ->
P_671 [ 1 ] ; _rtB -> B_185_896_0 [ 2 ] = _rtP -> P_671 [ 2 ] ;
normal_operation_no_faults_HarmonicGeneratorTID4 ( S , & _rtB ->
HarmonicGenerator_a , & _rtDW -> HarmonicGenerator_a , & _rtP ->
HarmonicGenerator_a ) ; normal_operation_no_faults_SignalgeneratorTID4 ( S ,
& _rtB -> Signalgenerator_j , & _rtDW -> Signalgenerator_j , & _rtP ->
Signalgenerator_j ) ; _rtB -> B_185_906_0 = _rtP -> P_674 ; _rtB ->
B_185_909_0 = _rtP -> P_676 ; _rtB -> B_185_913_0 = _rtP -> P_679 * _rtP ->
P_678 ; _rtB -> B_185_917_0 = _rtP -> P_681 * _rtP -> P_680 ; _rtB ->
B_185_918_0 [ 0 ] = _rtP -> P_682 [ 0 ] ; _rtB -> B_185_918_0 [ 1 ] = _rtP ->
P_682 [ 1 ] ; _rtB -> B_185_918_0 [ 2 ] = _rtP -> P_682 [ 2 ] ;
normal_operation_no_faults_HarmonicGeneratorTID4 ( S , & _rtB ->
HarmonicGenerator_ca , & _rtDW -> HarmonicGenerator_ca , & _rtP ->
HarmonicGenerator_ca ) ; _rtB -> B_185_927_0 = _rtP -> P_694 ; _rtB ->
B_185_928_0 = _rtP -> P_695 ; _rtB -> B_185_929_0 = _rtP -> P_696 ; _rtB ->
B_185_933_0 = _rtP -> P_700 ; _rtB -> B_185_934_0 = _rtP -> P_701 ; _rtB ->
B_185_944_0 = _rtP -> P_707 ; _rtB -> B_185_945_0 = _rtP -> P_708 ; _rtB ->
B_185_955_0 = _rtP -> P_714 ; _rtB -> B_185_956_0 = _rtP -> P_715 ; _rtB ->
B_185_973_0 = _rtP -> P_721 ; _rtB -> B_185_974_0 = _rtP -> P_722 ; _rtB ->
B_185_975_0 = _rtP -> P_723 ; _rtB -> B_185_979_0 = _rtP -> P_727 ; _rtB ->
B_185_980_0 = _rtP -> P_728 ; _rtB -> B_185_990_0 = _rtP -> P_734 ; _rtB ->
B_185_991_0 = _rtP -> P_735 ; _rtB -> B_185_1001_0 = _rtP -> P_741 ; _rtB ->
B_185_1002_0 = _rtP -> P_742 ; _rtB -> B_185_1019_0 = _rtP -> P_748 ; _rtB ->
B_185_1020_0 = _rtP -> P_749 ; _rtB -> B_185_1021_0 = _rtP -> P_750 ; _rtB ->
B_185_1025_0 = _rtP -> P_754 ; _rtB -> B_185_1026_0 = _rtP -> P_755 ; _rtB ->
B_185_1036_0 = _rtP -> P_761 ; _rtB -> B_185_1037_0 = _rtP -> P_762 ; _rtB ->
B_185_1047_0 = _rtP -> P_768 ; _rtB -> B_185_1048_0 = _rtP -> P_769 ; _rtB ->
B_185_1065_0 = _rtP -> P_775 ; _rtB -> B_185_1066_0 = _rtP -> P_776 ; _rtB ->
B_185_1067_0 = _rtP -> P_777 ; _rtB -> B_185_1071_0 = _rtP -> P_781 ; _rtB ->
B_185_1072_0 = _rtP -> P_782 ; _rtB -> B_185_1082_0 = _rtP -> P_788 ; _rtB ->
B_185_1083_0 = _rtP -> P_789 ; _rtB -> B_185_1093_0 = _rtP -> P_795 ; _rtB ->
B_185_1094_0 = _rtP -> P_796 ; _rtB -> B_185_1117_0 = _rtP -> P_804 ; _rtB ->
B_185_1369_0 = _rtP -> P_874 ;
normal_operation_no_faults_AutomaticGainControlTID4 ( S , & _rtB ->
AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl ) ; _rtB -> B_185_1375_0 = _rtP -> P_881 ; _rtB ->
B_185_1410_0 = ( uint8_T ) ( _rtP -> P_907 == _rtP -> P_908 ) ; _rtB ->
B_185_1412_0 = ( uint8_T ) ( _rtP -> P_907 == _rtP -> P_909 ) ; _rtB ->
B_185_1665_0 = _rtP -> P_979 ;
normal_operation_no_faults_AutomaticGainControlTID4 ( S , & _rtB ->
AutomaticGainControl_d , & _rtDW -> AutomaticGainControl_d , & _rtP ->
AutomaticGainControl_d ) ; _rtB -> B_185_1671_0 = _rtP -> P_986 ; _rtB ->
B_185_1706_0 = ( uint8_T ) ( _rtP -> P_1012 == _rtP -> P_1013 ) ; _rtB ->
B_185_1708_0 = ( uint8_T ) ( _rtP -> P_1012 == _rtP -> P_1014 ) ; _rtB ->
B_185_1961_0 = _rtP -> P_1084 ;
normal_operation_no_faults_AutomaticGainControlTID4 ( S , & _rtB ->
AutomaticGainControl_a , & _rtDW -> AutomaticGainControl_a , & _rtP ->
AutomaticGainControl_a ) ; _rtB -> B_185_1967_0 = _rtP -> P_1091 ; _rtB ->
B_185_2002_0 = ( uint8_T ) ( _rtP -> P_1117 == _rtP -> P_1118 ) ; _rtB ->
B_185_2004_0 = ( uint8_T ) ( _rtP -> P_1117 == _rtP -> P_1119 ) ; _rtB ->
B_185_2567_0 = ( _rtP -> P_1373 != 0.0 ) ; _rtB -> B_185_2569_0 = ! _rtB ->
B_185_2567_0 ; _rtB -> B_185_3180_0 = _rtP -> P_1464 ; _rtB -> B_185_3181_0 =
_rtP -> P_1465 ; _rtB -> B_185_3185_0 = _rtP -> P_1469 ; _rtB -> B_185_3186_0
= _rtP -> P_1470 ; _rtB -> B_185_3196_0 = _rtP -> P_1476 ; _rtB ->
B_185_3197_0 = _rtP -> P_1477 ; _rtB -> B_185_3207_0 = _rtP -> P_1483 ; _rtB
-> B_185_3208_0 = _rtP -> P_1484 ; _rtB -> B_185_3232_0 = _rtP -> P_1490 ;
_rtB -> B_185_3233_0 = _rtP -> P_1491 ; _rtB -> B_185_3237_0 = _rtP -> P_1495
; _rtB -> B_185_3238_0 = _rtP -> P_1496 ; _rtB -> B_185_3248_0 = _rtP ->
P_1502 ; _rtB -> B_185_3249_0 = _rtP -> P_1503 ; _rtB -> B_185_3259_0 = _rtP
-> P_1509 ; _rtB -> B_185_3260_0 = _rtP -> P_1510 ; _rtB -> B_185_3285_0 =
_rtP -> P_1516 ; _rtB -> B_185_3286_0 = _rtP -> P_1517 ; _rtB -> B_185_3290_0
= _rtP -> P_1521 ; _rtB -> B_185_3291_0 = _rtP -> P_1522 ; _rtB ->
B_185_3301_0 = _rtP -> P_1528 ; _rtB -> B_185_3302_0 = _rtP -> P_1529 ; _rtB
-> B_185_3312_0 = _rtP -> P_1535 ; _rtB -> B_185_3313_0 = _rtP -> P_1536 ;
_rtB -> B_185_3338_0 = _rtP -> P_1542 ; _rtB -> B_185_3339_0 = _rtP -> P_1543
; _rtB -> B_185_3343_0 = _rtP -> P_1547 ; _rtB -> B_185_3344_0 = _rtP ->
P_1548 ; _rtB -> B_185_3354_0 = _rtP -> P_1554 ; _rtB -> B_185_3355_0 = _rtP
-> P_1555 ; _rtB -> B_185_3365_0 = _rtP -> P_1561 ; _rtB -> B_185_3366_0 =
_rtP -> P_1562 ; _rtB -> B_185_3391_0 = _rtP -> P_1568 ; _rtB -> B_185_3393_0
= _rtP -> P_1572 ; _rtB -> B_185_3397_0 = _rtP -> P_1576 ; _rtB ->
B_185_3398_0 = _rtP -> P_1577 ; _rtB -> B_185_3408_0 = _rtP -> P_1583 ; _rtB
-> B_185_3409_0 = _rtP -> P_1584 ; _rtB -> B_185_3419_0 = _rtP -> P_1590 ;
_rtB -> B_185_3420_0 = _rtP -> P_1591 ; _rtB -> B_185_3475_0 = _rtP -> P_1597
; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { real_T * lastU ;
int32_T isHit ; B_normal_operation_no_faults_T * _rtB ;
P_normal_operation_no_faults_T * _rtP ; X_normal_operation_no_faults_T * _rtX
; DW_normal_operation_no_faults_T * _rtDW ; _rtDW = ( (
DW_normal_operation_no_faults_T * ) ssGetRootDWork ( S ) ) ; _rtX = ( (
X_normal_operation_no_faults_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_normal_operation_no_faults_T * ) _ssGetModelBlockIO ( S ) ) ; isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW -> UnitDelay5_DSTATE
[ 0 ] = _rtB -> B_185_1368_0 [ 0 ] ; _rtDW -> UnitDelay5_DSTATE [ 1 ] = _rtB
-> B_185_1368_0 [ 1 ] ; _rtDW -> UnitDelay5_DSTATE [ 2 ] = _rtB ->
B_185_1368_0 [ 2 ] ; } _rtDW -> Integrator_IWORK = 0 ; if ( _rtX ->
Integrator_CSTATE == _rtP -> P_118 ) { switch ( _rtDW -> Integrator_MODE ) {
case 3 : if ( _rtB -> B_185_1386_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE =
1 ; } break ; case 1 : if ( _rtB -> B_185_1386_0 >= 0.0 ) { _rtDW ->
Integrator_MODE = 3 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_185_1386_0 < 0.0 ) { _rtDW -> Integrator_MODE = 1 ; } else { _rtDW
-> Integrator_MODE = 3 ; } break ; } } else if ( _rtX -> Integrator_CSTATE ==
_rtP -> P_119 ) { switch ( _rtDW -> Integrator_MODE ) { case 4 : if ( _rtB ->
B_185_1386_0 > 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
_rtDW -> Integrator_MODE = 2 ; } break ; case 2 : if ( _rtB -> B_185_1386_0
<= 0.0 ) { _rtDW -> Integrator_MODE = 4 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_185_1386_0
> 0.0 ) { _rtDW -> Integrator_MODE = 2 ; } else { _rtDW -> Integrator_MODE =
4 ; } break ; } } else { _rtDW -> Integrator_MODE = 0 ; } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtDW -> Delay_x1_DSTATE [
0 ] = _rtB -> B_185_1425_0 [ 0 ] ; _rtDW -> Delay_x2_DSTATE [ 0 ] = _rtB ->
B_185_1427_0 [ 0 ] ; _rtDW -> Delay_x1_DSTATE [ 1 ] = _rtB -> B_185_1425_0 [
1 ] ; _rtDW -> Delay_x2_DSTATE [ 1 ] = _rtB -> B_185_1427_0 [ 1 ] ; _rtDW ->
Delay_x1_DSTATE [ 2 ] = _rtB -> B_185_1425_0 [ 2 ] ; _rtDW -> Delay_x2_DSTATE
[ 2 ] = _rtB -> B_185_1427_0 [ 2 ] ; } isHit = ssIsSampleHit ( S , 2 , 0 ) ;
if ( isHit != 0 ) { _rtDW -> UnitDelay1_DSTATE = _rtB -> B_185_1359_0 ; }
isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
DiscreteTimeIntegrator1_SYSTEM_ENABLE = 0U ; _rtDW ->
DiscreteTimeIntegrator1_DSTATE = _rtP -> P_147 * _rtB -> B_185_53_0 + _rtB ->
B_185_54_0 ; if ( _rtDW -> DiscreteTimeIntegrator1_DSTATE >= _rtP -> P_149 )
{ _rtDW -> DiscreteTimeIntegrator1_DSTATE = _rtP -> P_149 ; } else { if (
_rtDW -> DiscreteTimeIntegrator1_DSTATE <= _rtP -> P_150 ) { _rtDW ->
DiscreteTimeIntegrator1_DSTATE = _rtP -> P_150 ; } } } isHit = ssIsSampleHit
( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW -> UnitDelay6_DSTATE [ 0 ] = _rtB
-> B_185_2337_0 [ 0 ] ; _rtDW -> UnitDelay6_DSTATE [ 1 ] = _rtB ->
B_185_2337_0 [ 1 ] ; _rtDW -> UnitDelay6_DSTATE [ 2 ] = _rtB -> B_185_2337_0
[ 2 ] ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE = _rtP -> P_165 * _rtB -> B_185_88_0 + _rtB ->
B_185_89_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE >= _rtP -> P_167 ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE = _rtP -> P_167 ; } else { if ( _rtDW
-> DiscreteTimeIntegrator_DSTATE <= _rtP -> P_168 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE = _rtP -> P_168 ; } } }
normal_operation_no_faults_ACVoltageController_Update ( S , _rtB ->
B_185_1368_0 , & _rtB -> ACVoltageController , & _rtDW -> ACVoltageController
, & _rtP -> ACVoltageController ) ;
normal_operation_no_faults_Circulatingcurrentsuppression_Update ( S , _rtB ->
B_185_1602_0 , _rtB -> B_185_1606_0 , _rtB -> B_185_1610_0 , _rtB ->
B_185_1618_0 , _rtB -> B_185_1614_0 , _rtB -> B_185_1622_0 , & _rtB ->
Circulatingcurrentsuppression , & _rtDW -> Circulatingcurrentsuppression ) ;
isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay4_DSTATE = _rtB -> B_185_1446_0 ; _rtDW -> UnitDelay3_DSTATE = _rtB
-> B_185_137_0 ; _rtDW -> UnitDelay4_DSTATE_i = _rtB -> B_185_1475_0 ; _rtDW
-> UnitDelay3_DSTATE_f = _rtB -> B_185_150_0 ; _rtDW -> UnitDelay4_DSTATE_e =
_rtB -> B_185_1504_0 ; _rtDW -> UnitDelay3_DSTATE_a = _rtB -> B_185_163_0 ;
_rtDW -> UnitDelay4_DSTATE_p = _rtB -> B_185_1533_0 ; _rtDW ->
UnitDelay3_DSTATE_d = _rtB -> B_185_176_0 ; _rtDW -> UnitDelay4_DSTATE_m =
_rtB -> B_185_1562_0 ; _rtDW -> UnitDelay3_DSTATE_p = _rtB -> B_185_189_0 ;
_rtDW -> UnitDelay4_DSTATE_c = _rtB -> B_185_1591_0 ; _rtDW ->
UnitDelay3_DSTATE_de = _rtB -> B_185_202_0 ; _rtDW -> UnitDelay5_DSTATE_m [ 0
] = _rtB -> B_185_2507_0 [ 0 ] ; _rtDW -> UnitDelay5_DSTATE_m [ 1 ] = _rtB ->
B_185_2507_0 [ 1 ] ; _rtDW -> UnitDelay5_DSTATE_m [ 2 ] = _rtB ->
B_185_2507_0 [ 2 ] ; _rtDW -> UnitDelay1_DSTATE_b = _rtB -> B_185_1121_0 ; }
isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Delay_x1_DSTATE_i [ 0 ] = _rtB -> B_185_1129_0 [ 0 ] ; _rtDW ->
Delay_x2_DSTATE_f [ 0 ] = _rtB -> B_185_1131_0 [ 0 ] ; _rtDW ->
Delay_x1_DSTATE_i [ 1 ] = _rtB -> B_185_1129_0 [ 1 ] ; _rtDW ->
Delay_x2_DSTATE_f [ 1 ] = _rtB -> B_185_1131_0 [ 1 ] ; _rtDW ->
Delay_x1_DSTATE_i [ 2 ] = _rtB -> B_185_1129_0 [ 2 ] ; _rtDW ->
Delay_x2_DSTATE_f [ 2 ] = _rtB -> B_185_1131_0 [ 2 ] ; _rtDW ->
DiscreteTimeIntegrator1_SYSTEM_ENABLE_f = 0U ; _rtDW ->
DiscreteTimeIntegrator1_DSTATE_o = _rtP -> P_275 * _rtB -> B_185_251_0 + _rtB
-> B_185_252_0 ; if ( _rtDW -> DiscreteTimeIntegrator1_DSTATE_o >= _rtP ->
P_277 ) { _rtDW -> DiscreteTimeIntegrator1_DSTATE_o = _rtP -> P_277 ; } else
{ if ( _rtDW -> DiscreteTimeIntegrator1_DSTATE_o <= _rtP -> P_278 ) { _rtDW
-> DiscreteTimeIntegrator1_DSTATE_o = _rtP -> P_278 ; } } } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay6_DSTATE_l [ 0 ] = _rtB -> B_185_2515_0 [ 0 ] ; _rtDW ->
UnitDelay6_DSTATE_l [ 1 ] = _rtB -> B_185_2515_0 [ 1 ] ; _rtDW ->
UnitDelay6_DSTATE_l [ 2 ] = _rtB -> B_185_2515_0 [ 2 ] ; } if ( _rtDW ->
ReactivepowerSetpoint_MODE ) { isHit = ssIsSampleHit ( S , 2 , 0 ) ; if (
isHit != 0 ) { _rtDW -> UnitDelay6_DSTATE_hi [ 0 ] = _rtB -> B_185_2641_0 ;
_rtDW -> UnitDelay6_DSTATE_hi [ 1 ] = _rtB -> B_185_2654_0 ; } } if ( _rtDW
-> Circulatingcurrentsuppression1_MODE ) { isHit = ssIsSampleHit ( S , 2 , 0
) ; if ( isHit != 0 ) { _rtDW -> UnitDelay9_DSTATE [ 0 ] = _rtB ->
B_185_1307_0 ; _rtDW -> UnitDelay9_DSTATE [ 1 ] = _rtB -> B_185_1311_0 ;
_rtDW -> UnitDelay9_DSTATE [ 2 ] = _rtB -> B_185_1315_0 ; _rtDW ->
UnitDelay9_DSTATE [ 3 ] = _rtB -> B_185_1323_0 ; _rtDW -> UnitDelay9_DSTATE [
4 ] = _rtB -> B_185_1319_0 ; _rtDW -> UnitDelay9_DSTATE [ 5 ] = _rtB ->
B_185_1327_0 ; _rtDW -> UnitDelay1_DSTATE_em = _rtB -> B_4_43_0 ; } } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay4_DSTATE_o = _rtB -> B_185_1151_0 ; _rtDW -> UnitDelay3_DSTATE_m =
_rtB -> B_185_318_0 ; _rtDW -> UnitDelay4_DSTATE_f = _rtB -> B_185_1180_0 ;
_rtDW -> UnitDelay3_DSTATE_ff = _rtB -> B_185_331_0 ; _rtDW ->
UnitDelay4_DSTATE_ig = _rtB -> B_185_1209_0 ; _rtDW -> UnitDelay3_DSTATE_o =
_rtB -> B_185_344_0 ; _rtDW -> UnitDelay4_DSTATE_ma = _rtB -> B_185_1238_0 ;
_rtDW -> UnitDelay3_DSTATE_pb = _rtB -> B_185_357_0 ; _rtDW ->
UnitDelay4_DSTATE_l = _rtB -> B_185_1267_0 ; _rtDW -> UnitDelay3_DSTATE_d4 =
_rtB -> B_185_370_0 ; _rtDW -> UnitDelay4_DSTATE_c5 = _rtB -> B_185_1296_0 ;
_rtDW -> UnitDelay3_DSTATE_b = _rtB -> B_185_383_0 ; _rtDW ->
UnitDelay5_DSTATE_h [ 0 ] = _rtB -> B_185_1664_0 [ 0 ] ; _rtDW ->
UnitDelay5_DSTATE_h [ 1 ] = _rtB -> B_185_1664_0 [ 1 ] ; _rtDW ->
UnitDelay5_DSTATE_h [ 2 ] = _rtB -> B_185_1664_0 [ 2 ] ; } _rtDW ->
Integrator_IWORK_o = 0 ; if ( _rtX -> Integrator_CSTATE_l == _rtP -> P_370 )
{ switch ( _rtDW -> Integrator_MODE_i ) { case 3 : if ( _rtB -> B_185_1682_0
< 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW ->
Integrator_MODE_i = 1 ; } break ; case 1 : if ( _rtB -> B_185_1682_0 >= 0.0 )
{ _rtDW -> Integrator_MODE_i = 3 ; ssSetBlockStateForSolverChangedAtMajorStep
( S ) ; } break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S )
; if ( _rtB -> B_185_1682_0 < 0.0 ) { _rtDW -> Integrator_MODE_i = 1 ; } else
{ _rtDW -> Integrator_MODE_i = 3 ; } break ; } } else if ( _rtX ->
Integrator_CSTATE_l == _rtP -> P_371 ) { switch ( _rtDW -> Integrator_MODE_i
) { case 4 : if ( _rtB -> B_185_1682_0 > 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_i
= 2 ; } break ; case 2 : if ( _rtB -> B_185_1682_0 <= 0.0 ) { _rtDW ->
Integrator_MODE_i = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_185_1682_0 > 0.0 ) { _rtDW -> Integrator_MODE_i = 2 ; } else {
_rtDW -> Integrator_MODE_i = 4 ; } break ; } } else { _rtDW ->
Integrator_MODE_i = 0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { _rtDW -> Delay_x1_DSTATE_h [ 0 ] = _rtB -> B_185_1721_0 [ 0 ] ; _rtDW
-> Delay_x2_DSTATE_m [ 0 ] = _rtB -> B_185_1723_0 [ 0 ] ; _rtDW ->
Delay_x1_DSTATE_h [ 1 ] = _rtB -> B_185_1721_0 [ 1 ] ; _rtDW ->
Delay_x2_DSTATE_m [ 1 ] = _rtB -> B_185_1723_0 [ 1 ] ; _rtDW ->
Delay_x1_DSTATE_h [ 2 ] = _rtB -> B_185_1721_0 [ 2 ] ; _rtDW ->
Delay_x2_DSTATE_m [ 2 ] = _rtB -> B_185_1723_0 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay1_DSTATE_a = _rtB -> B_185_1655_0 ; } isHit = ssIsSampleHit ( S , 3
, 0 ) ; if ( isHit != 0 ) { _rtDW -> DiscreteTimeIntegrator1_SYSTEM_ENABLE_i
= 0U ; _rtDW -> DiscreteTimeIntegrator1_DSTATE_h = _rtP -> P_396 * _rtB ->
B_185_439_0 + _rtB -> B_185_440_0 ; if ( _rtDW ->
DiscreteTimeIntegrator1_DSTATE_h >= _rtP -> P_398 ) { _rtDW ->
DiscreteTimeIntegrator1_DSTATE_h = _rtP -> P_398 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator1_DSTATE_h <= _rtP -> P_399 ) { _rtDW ->
DiscreteTimeIntegrator1_DSTATE_h = _rtP -> P_399 ; } } } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay6_DSTATE_h [ 0 ] = _rtB -> B_185_2395_0 [ 0 ] ; _rtDW ->
UnitDelay6_DSTATE_h [ 1 ] = _rtB -> B_185_2395_0 [ 1 ] ; _rtDW ->
UnitDelay6_DSTATE_h [ 2 ] = _rtB -> B_185_2395_0 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_e = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_i = _rtP -> P_414 * _rtB -> B_185_474_0 + _rtB
-> B_185_475_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_i >= _rtP ->
P_416 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_i = _rtP -> P_416 ; } else {
if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_i <= _rtP -> P_417 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_i = _rtP -> P_417 ; } } }
normal_operation_no_faults_ACVoltageController_Update ( S , _rtB ->
B_185_1664_0 , & _rtB -> ACVoltageController_n , & _rtDW ->
ACVoltageController_n , & _rtP -> ACVoltageController_n ) ;
normal_operation_no_faults_Circulatingcurrentsuppression_Update ( S , _rtB ->
B_185_1898_0 , _rtB -> B_185_1902_0 , _rtB -> B_185_1906_0 , _rtB ->
B_185_1914_0 , _rtB -> B_185_1910_0 , _rtB -> B_185_1918_0 , & _rtB ->
Circulatingcurrentsuppression_d , & _rtDW -> Circulatingcurrentsuppression_d
) ; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay4_DSTATE_n = _rtB -> B_185_1742_0 ; _rtDW -> UnitDelay3_DSTATE_od =
_rtB -> B_185_521_0 ; _rtDW -> UnitDelay4_DSTATE_lc = _rtB -> B_185_1771_0 ;
_rtDW -> UnitDelay3_DSTATE_o5 = _rtB -> B_185_534_0 ; _rtDW ->
UnitDelay4_DSTATE_b = _rtB -> B_185_1800_0 ; _rtDW -> UnitDelay3_DSTATE_pu =
_rtB -> B_185_547_0 ; _rtDW -> UnitDelay4_DSTATE_k = _rtB -> B_185_1829_0 ;
_rtDW -> UnitDelay3_DSTATE_oh = _rtB -> B_185_560_0 ; _rtDW ->
UnitDelay4_DSTATE_d = _rtB -> B_185_1858_0 ; _rtDW -> UnitDelay3_DSTATE_e =
_rtB -> B_185_573_0 ; _rtDW -> UnitDelay4_DSTATE_cx = _rtB -> B_185_1887_0 ;
_rtDW -> UnitDelay3_DSTATE_mu = _rtB -> B_185_586_0 ; _rtDW ->
UnitDelay5_DSTATE_o [ 0 ] = _rtB -> B_185_1960_0 [ 0 ] ; _rtDW ->
UnitDelay5_DSTATE_o [ 1 ] = _rtB -> B_185_1960_0 [ 1 ] ; _rtDW ->
UnitDelay5_DSTATE_o [ 2 ] = _rtB -> B_185_1960_0 [ 2 ] ; } _rtDW ->
Integrator_IWORK_h = 0 ; if ( _rtX -> Integrator_CSTATE_b == _rtP -> P_498 )
{ switch ( _rtDW -> Integrator_MODE_h ) { case 3 : if ( _rtB -> B_185_1978_0
< 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW ->
Integrator_MODE_h = 1 ; } break ; case 1 : if ( _rtB -> B_185_1978_0 >= 0.0 )
{ _rtDW -> Integrator_MODE_h = 3 ; ssSetBlockStateForSolverChangedAtMajorStep
( S ) ; } break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S )
; if ( _rtB -> B_185_1978_0 < 0.0 ) { _rtDW -> Integrator_MODE_h = 1 ; } else
{ _rtDW -> Integrator_MODE_h = 3 ; } break ; } } else if ( _rtX ->
Integrator_CSTATE_b == _rtP -> P_499 ) { switch ( _rtDW -> Integrator_MODE_h
) { case 4 : if ( _rtB -> B_185_1978_0 > 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_h
= 2 ; } break ; case 2 : if ( _rtB -> B_185_1978_0 <= 0.0 ) { _rtDW ->
Integrator_MODE_h = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_185_1978_0 > 0.0 ) { _rtDW -> Integrator_MODE_h = 2 ; } else {
_rtDW -> Integrator_MODE_h = 4 ; } break ; } } else { _rtDW ->
Integrator_MODE_h = 0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { _rtDW -> Delay_x1_DSTATE_o [ 0 ] = _rtB -> B_185_2017_0 [ 0 ] ; _rtDW
-> Delay_x2_DSTATE_n [ 0 ] = _rtB -> B_185_2019_0 [ 0 ] ; _rtDW ->
Delay_x1_DSTATE_o [ 1 ] = _rtB -> B_185_2017_0 [ 1 ] ; _rtDW ->
Delay_x2_DSTATE_n [ 1 ] = _rtB -> B_185_2019_0 [ 1 ] ; _rtDW ->
Delay_x1_DSTATE_o [ 2 ] = _rtB -> B_185_2017_0 [ 2 ] ; _rtDW ->
Delay_x2_DSTATE_n [ 2 ] = _rtB -> B_185_2019_0 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay1_DSTATE_i = _rtB -> B_185_1951_0 ; } isHit = ssIsSampleHit ( S , 3
, 0 ) ; if ( isHit != 0 ) { _rtDW -> DiscreteTimeIntegrator1_SYSTEM_ENABLE_l
= 0U ; _rtDW -> DiscreteTimeIntegrator1_DSTATE_a = _rtP -> P_524 * _rtB ->
B_185_642_0 + _rtB -> B_185_643_0 ; if ( _rtDW ->
DiscreteTimeIntegrator1_DSTATE_a >= _rtP -> P_526 ) { _rtDW ->
DiscreteTimeIntegrator1_DSTATE_a = _rtP -> P_526 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator1_DSTATE_a <= _rtP -> P_527 ) { _rtDW ->
DiscreteTimeIntegrator1_DSTATE_a = _rtP -> P_527 ; } } } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay6_DSTATE_i [ 0 ] = _rtB -> B_185_2453_0 [ 0 ] ; _rtDW ->
UnitDelay6_DSTATE_i [ 1 ] = _rtB -> B_185_2453_0 [ 1 ] ; _rtDW ->
UnitDelay6_DSTATE_i [ 2 ] = _rtB -> B_185_2453_0 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_c = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_j = _rtP -> P_542 * _rtB -> B_185_677_0 + _rtB
-> B_185_678_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_j >= _rtP ->
P_544 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_j = _rtP -> P_544 ; } else {
if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_j <= _rtP -> P_545 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_j = _rtP -> P_545 ; } } }
normal_operation_no_faults_ACVoltageController_Update ( S , _rtB ->
B_185_1960_0 , & _rtB -> ACVoltageController_l , & _rtDW ->
ACVoltageController_l , & _rtP -> ACVoltageController_l ) ;
normal_operation_no_faults_Circulatingcurrentsuppression_Update ( S , _rtB ->
B_185_1898_0 , _rtB -> B_185_1902_0 , _rtB -> B_185_1906_0 , _rtB ->
B_185_1914_0 , _rtB -> B_185_1910_0 , _rtB -> B_185_1918_0 , & _rtB ->
Circulatingcurrentsuppression_k , & _rtDW -> Circulatingcurrentsuppression_k
) ; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay4_DSTATE_j = _rtB -> B_185_2038_0 ; _rtDW -> UnitDelay3_DSTATE_bw =
_rtB -> B_185_724_0 ; _rtDW -> UnitDelay4_DSTATE_df = _rtB -> B_185_2067_0 ;
_rtDW -> UnitDelay3_DSTATE_ey = _rtB -> B_185_737_0 ; _rtDW ->
UnitDelay4_DSTATE_mc = _rtB -> B_185_2096_0 ; _rtDW -> UnitDelay3_DSTATE_oa =
_rtB -> B_185_750_0 ; _rtDW -> UnitDelay4_DSTATE_h = _rtB -> B_185_2125_0 ;
_rtDW -> UnitDelay3_DSTATE_i = _rtB -> B_185_763_0 ; _rtDW ->
UnitDelay4_DSTATE_n3 = _rtB -> B_185_2154_0 ; _rtDW -> UnitDelay3_DSTATE_n =
_rtB -> B_185_776_0 ; _rtDW -> UnitDelay4_DSTATE_ix = _rtB -> B_185_2183_0 ;
_rtDW -> UnitDelay3_DSTATE_ds = _rtB -> B_185_789_0 ;
normal_operation_no_faults_Signalgenerator_Update ( S , & _rtB ->
Signalgenerator , & _rtDW -> Signalgenerator , & _rtP -> Signalgenerator ) ;
normal_operation_no_faults_Signalgenerator_Update ( S , & _rtB ->
Signalgenerator_d , & _rtDW -> Signalgenerator_d , & _rtP ->
Signalgenerator_d ) ; normal_operation_no_faults_Signalgenerator_Update ( S ,
& _rtB -> Signalgenerator_p , & _rtDW -> Signalgenerator_p , & _rtP ->
Signalgenerator_p ) ; normal_operation_no_faults_Signalgenerator_Update ( S ,
& _rtB -> Signalgenerator_e , & _rtDW -> Signalgenerator_e , & _rtP ->
Signalgenerator_e ) ; normal_operation_no_faults_Signalgenerator_Update ( S ,
& _rtB -> Signalgenerator_m , & _rtDW -> Signalgenerator_m , & _rtP ->
Signalgenerator_m ) ; normal_operation_no_faults_Signalgenerator_Update ( S ,
& _rtB -> Signalgenerator_j , & _rtDW -> Signalgenerator_j , & _rtP ->
Signalgenerator_j ) ; ssCallAccelRunBlock ( S , 185 , 926 ,
SS_CALL_MDL_UPDATE ) ; } if ( _rtDW -> TimeStampA == ( rtInf ) ) { _rtDW ->
TimeStampA = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA ; } else if (
_rtDW -> TimeStampB == ( rtInf ) ) { _rtDW -> TimeStampB = ssGetT ( S ) ;
lastU = & _rtDW -> LastUAtTimeB ; } else if ( _rtDW -> TimeStampA < _rtDW ->
TimeStampB ) { _rtDW -> TimeStampA = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA ; } else { _rtDW -> TimeStampB = ssGetT ( S ) ; lastU = & _rtDW
-> LastUAtTimeB ; } * lastU = _rtB -> B_185_937_0 ; if ( _rtDW ->
TimeStampA_f == ( rtInf ) ) { _rtDW -> TimeStampA_f = ssGetT ( S ) ; lastU =
& _rtDW -> LastUAtTimeA_o ; } else if ( _rtDW -> TimeStampB_a == ( rtInf ) )
{ _rtDW -> TimeStampB_a = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_o ;
} else if ( _rtDW -> TimeStampA_f < _rtDW -> TimeStampB_a ) { _rtDW ->
TimeStampA_f = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_o ; } else {
_rtDW -> TimeStampB_a = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_o ; }
* lastU = _rtB -> B_185_948_0 ; if ( _rtDW -> TimeStampA_o == ( rtInf ) ) {
_rtDW -> TimeStampA_o = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_g ; }
else if ( _rtDW -> TimeStampB_m == ( rtInf ) ) { _rtDW -> TimeStampB_m =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_p ; } else if ( _rtDW ->
TimeStampA_o < _rtDW -> TimeStampB_m ) { _rtDW -> TimeStampA_o = ssGetT ( S )
; lastU = & _rtDW -> LastUAtTimeA_g ; } else { _rtDW -> TimeStampB_m = ssGetT
( S ) ; lastU = & _rtDW -> LastUAtTimeB_p ; } * lastU = _rtB -> B_185_959_0 ;
if ( _rtDW -> TimeStampA_n == ( rtInf ) ) { _rtDW -> TimeStampA_n = ssGetT (
S ) ; lastU = & _rtDW -> LastUAtTimeA_d ; } else if ( _rtDW -> TimeStampB_o
== ( rtInf ) ) { _rtDW -> TimeStampB_o = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_k ; } else if ( _rtDW -> TimeStampA_n < _rtDW -> TimeStampB_o )
{ _rtDW -> TimeStampA_n = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_d ;
} else { _rtDW -> TimeStampB_o = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_k ; } * lastU = _rtB -> B_185_983_0 ; if ( _rtDW -> TimeStampA_g
== ( rtInf ) ) { _rtDW -> TimeStampA_g = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_a ; } else if ( _rtDW -> TimeStampB_k == ( rtInf ) ) { _rtDW ->
TimeStampB_k = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_g ; } else if (
_rtDW -> TimeStampA_g < _rtDW -> TimeStampB_k ) { _rtDW -> TimeStampA_g =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_a ; } else { _rtDW ->
TimeStampB_k = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_g ; } * lastU =
_rtB -> B_185_994_0 ; if ( _rtDW -> TimeStampA_j == ( rtInf ) ) { _rtDW ->
TimeStampA_j = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_k ; } else if (
_rtDW -> TimeStampB_f == ( rtInf ) ) { _rtDW -> TimeStampB_f = ssGetT ( S ) ;
lastU = & _rtDW -> LastUAtTimeB_i ; } else if ( _rtDW -> TimeStampA_j < _rtDW
-> TimeStampB_f ) { _rtDW -> TimeStampA_j = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_k ; } else { _rtDW -> TimeStampB_f = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeB_i ; } * lastU = _rtB -> B_185_1005_0 ; if ( _rtDW ->
TimeStampA_i == ( rtInf ) ) { _rtDW -> TimeStampA_i = ssGetT ( S ) ; lastU =
& _rtDW -> LastUAtTimeA_a1 ; } else if ( _rtDW -> TimeStampB_c == ( rtInf ) )
{ _rtDW -> TimeStampB_c = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_e ;
} else if ( _rtDW -> TimeStampA_i < _rtDW -> TimeStampB_c ) { _rtDW ->
TimeStampA_i = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_a1 ; } else {
_rtDW -> TimeStampB_c = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_e ; }
* lastU = _rtB -> B_185_1029_0 ; if ( _rtDW -> TimeStampA_p == ( rtInf ) ) {
_rtDW -> TimeStampA_p = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_n ; }
else if ( _rtDW -> TimeStampB_l == ( rtInf ) ) { _rtDW -> TimeStampB_l =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_h ; } else if ( _rtDW ->
TimeStampA_p < _rtDW -> TimeStampB_l ) { _rtDW -> TimeStampA_p = ssGetT ( S )
; lastU = & _rtDW -> LastUAtTimeA_n ; } else { _rtDW -> TimeStampB_l = ssGetT
( S ) ; lastU = & _rtDW -> LastUAtTimeB_h ; } * lastU = _rtB -> B_185_1040_0
; if ( _rtDW -> TimeStampA_gh == ( rtInf ) ) { _rtDW -> TimeStampA_gh =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_n1 ; } else if ( _rtDW ->
TimeStampB_l0 == ( rtInf ) ) { _rtDW -> TimeStampB_l0 = ssGetT ( S ) ; lastU
= & _rtDW -> LastUAtTimeB_g2 ; } else if ( _rtDW -> TimeStampA_gh < _rtDW ->
TimeStampB_l0 ) { _rtDW -> TimeStampA_gh = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_n1 ; } else { _rtDW -> TimeStampB_l0 = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeB_g2 ; } * lastU = _rtB -> B_185_1051_0 ; if ( _rtDW ->
TimeStampA_ii == ( rtInf ) ) { _rtDW -> TimeStampA_ii = ssGetT ( S ) ; lastU
= & _rtDW -> LastUAtTimeA_e ; } else if ( _rtDW -> TimeStampB_fs == ( rtInf )
) { _rtDW -> TimeStampB_fs = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_eq ; } else if ( _rtDW -> TimeStampA_ii < _rtDW -> TimeStampB_fs
) { _rtDW -> TimeStampA_ii = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_e
; } else { _rtDW -> TimeStampB_fs = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_eq ; } * lastU = _rtB -> B_185_1075_0 ; if ( _rtDW ->
TimeStampA_oi == ( rtInf ) ) { _rtDW -> TimeStampA_oi = ssGetT ( S ) ; lastU
= & _rtDW -> LastUAtTimeA_h ; } else if ( _rtDW -> TimeStampB_cm == ( rtInf )
) { _rtDW -> TimeStampB_cm = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_j
; } else if ( _rtDW -> TimeStampA_oi < _rtDW -> TimeStampB_cm ) { _rtDW ->
TimeStampA_oi = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_h ; } else {
_rtDW -> TimeStampB_cm = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_j ; }
* lastU = _rtB -> B_185_1086_0 ; if ( _rtDW -> TimeStampA_c == ( rtInf ) ) {
_rtDW -> TimeStampA_c = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_b ; }
else if ( _rtDW -> TimeStampB_j == ( rtInf ) ) { _rtDW -> TimeStampB_j =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_p1 ; } else if ( _rtDW ->
TimeStampA_c < _rtDW -> TimeStampB_j ) { _rtDW -> TimeStampA_c = ssGetT ( S )
; lastU = & _rtDW -> LastUAtTimeA_b ; } else { _rtDW -> TimeStampB_j = ssGetT
( S ) ; lastU = & _rtDW -> LastUAtTimeB_p1 ; } * lastU = _rtB -> B_185_1097_0
; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
UnitDelay1_DSTATE_h [ 0 ] = _rtB -> B_185_1139_0 ; _rtDW ->
UnitDelay1_DSTATE_h [ 1 ] = _rtB -> B_185_1143_0 ; _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_h = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_p = _rtP -> P_817 * _rtB -> B_185_1150_0 + _rtB
-> B_185_1151_0 ; _rtDW -> UnitDelay1_DSTATE_b2 [ 0 ] = _rtB -> B_185_1168_0
; _rtDW -> UnitDelay1_DSTATE_b2 [ 1 ] = _rtB -> B_185_1172_0 ; _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_g = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_b = _rtP -> P_823 * _rtB -> B_185_1179_0 + _rtB
-> B_185_1180_0 ; _rtDW -> UnitDelay1_DSTATE_iy [ 0 ] = _rtB -> B_185_1197_0
; _rtDW -> UnitDelay1_DSTATE_iy [ 1 ] = _rtB -> B_185_1201_0 ; _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_i = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_d = _rtP -> P_829 * _rtB -> B_185_1208_0 + _rtB
-> B_185_1209_0 ; _rtDW -> UnitDelay1_DSTATE_f [ 0 ] = _rtB -> B_185_1226_0 ;
_rtDW -> UnitDelay1_DSTATE_f [ 1 ] = _rtB -> B_185_1230_0 ; _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_j = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_a = _rtP -> P_835 * _rtB -> B_185_1237_0 + _rtB
-> B_185_1238_0 ; _rtDW -> UnitDelay1_DSTATE_p [ 0 ] = _rtB -> B_185_1255_0 ;
_rtDW -> UnitDelay1_DSTATE_p [ 1 ] = _rtB -> B_185_1259_0 ; _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_g4 = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_jm = _rtP -> P_841 * _rtB -> B_185_1266_0 +
_rtB -> B_185_1267_0 ; _rtDW -> UnitDelay1_DSTATE_i1 [ 0 ] = _rtB ->
B_185_1284_0 ; _rtDW -> UnitDelay1_DSTATE_i1 [ 1 ] = _rtB -> B_185_1288_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_m = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_c = _rtP -> P_847 * _rtB -> B_185_1295_0 + _rtB
-> B_185_1296_0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) {
_rtDW -> Delay_x1_DSTATE_a = _rtB -> B_185_1352_0 ; _rtDW ->
Delay_x2_DSTATE_b = _rtB -> B_185_1354_0 ; } isHit = ssIsSampleHit ( S , 1 ,
0 ) ; if ( isHit != 0 ) { _rtDW -> Memory_PreviousInput = _rtB ->
B_185_1405_0 ; } normal_operation_no_faults_AutomaticGainControl_Update ( S ,
& _rtB -> AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl , & ( ( X_normal_operation_no_faults_T * )
ssGetContStates ( S ) ) -> AutomaticGainControl ) ; if ( _rtX ->
Integrator_CSTATE_ja == _rtP -> P_876 ) { switch ( _rtDW -> Integrator_MODE_n
) { case 3 : if ( _rtB -> B_185_1382_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_n
= 1 ; } break ; case 1 : if ( _rtB -> B_185_1382_0 >= 0.0 ) { _rtDW ->
Integrator_MODE_n = 3 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_185_1382_0 < 0.0 ) { _rtDW -> Integrator_MODE_n = 1 ; } else {
_rtDW -> Integrator_MODE_n = 3 ; } break ; } } else if ( _rtX ->
Integrator_CSTATE_ja == _rtP -> P_877 ) { switch ( _rtDW -> Integrator_MODE_n
) { case 4 : if ( _rtB -> B_185_1382_0 > 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_n
= 2 ; } break ; case 2 : if ( _rtB -> B_185_1382_0 <= 0.0 ) { _rtDW ->
Integrator_MODE_n = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_185_1382_0 > 0.0 ) { _rtDW -> Integrator_MODE_n = 2 ; } else {
_rtDW -> Integrator_MODE_n = 4 ; } break ; } } else { _rtDW ->
Integrator_MODE_n = 0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 1 ] ;
real_T * * xBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK .
TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
VariableTransportDelay_IWORK . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK . Head < ( _rtDW -> VariableTransportDelay_IWORK
. CircularBufSize - 1 ) ) ? ( _rtDW -> VariableTransportDelay_IWORK . Head +
1 ) : 0 ) ; if ( _rtDW -> VariableTransportDelay_IWORK . Head == _rtDW ->
VariableTransportDelay_IWORK . Tail ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
VariableTransportDelay_IWORK . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK . Tail , & _rtDW -> VariableTransportDelay_IWORK
. Head , & _rtDW -> VariableTransportDelay_IWORK . Last , simTime - _rtP ->
P_878 , tBuffer , uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , &
_rtDW -> VariableTransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus
( S , "vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [
_rtDW -> VariableTransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [
_rtDW -> VariableTransportDelay_IWORK . Head ] = _rtB -> B_185_1373_0 ; ( *
xBuffer ) [ _rtDW -> VariableTransportDelay_IWORK . Head ] = ( (
X_normal_operation_no_faults_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { _rtDW -> Memory_PreviousInput_f = _rtB -> B_185_1379_0 ; } if
( _rtDW -> LastMajorTimeA == ( rtInf ) ) { _rtDW -> LastMajorTimeA =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA = _rtB -> B_185_1388_0 ; } else if
( _rtDW -> LastMajorTimeB == ( rtInf ) ) { _rtDW -> LastMajorTimeB =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB = _rtB -> B_185_1388_0 ; } else if
( _rtDW -> LastMajorTimeA < _rtDW -> LastMajorTimeB ) { _rtDW ->
LastMajorTimeA = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA = _rtB ->
B_185_1388_0 ; } else { _rtDW -> LastMajorTimeB = ssGetTaskTime ( S , 0 ) ;
_rtDW -> PrevYB = _rtB -> B_185_1388_0 ; } isHit = ssIsSampleHit ( S , 2 , 0
) ; if ( isHit != 0 ) { _rtDW -> UnitDelay1_DSTATE_e [ 0 ] = _rtB ->
B_185_1434_0 ; _rtDW -> UnitDelay1_DSTATE_e [ 1 ] = _rtB -> B_185_1438_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_hp = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_j1 = _rtP -> P_922 * _rtB -> B_185_1445_0 +
_rtB -> B_185_1446_0 ; _rtDW -> UnitDelay1_DSTATE_ef [ 0 ] = _rtB ->
B_185_1463_0 ; _rtDW -> UnitDelay1_DSTATE_ef [ 1 ] = _rtB -> B_185_1467_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_l = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_h = _rtP -> P_928 * _rtB -> B_185_1474_0 + _rtB
-> B_185_1475_0 ; _rtDW -> UnitDelay1_DSTATE_hm [ 0 ] = _rtB -> B_185_1492_0
; _rtDW -> UnitDelay1_DSTATE_hm [ 1 ] = _rtB -> B_185_1496_0 ; _rtDW ->
DiscreteTimeIntegrator_SYSTEM_ENABLE_lm = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_b5 = _rtP -> P_934 * _rtB -> B_185_1503_0 +
_rtB -> B_185_1504_0 ; _rtDW -> UnitDelay1_DSTATE_j [ 0 ] = _rtB ->
B_185_1521_0 ; _rtDW -> UnitDelay1_DSTATE_j [ 1 ] = _rtB -> B_185_1525_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_d = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_pu = _rtP -> P_940 * _rtB -> B_185_1532_0 +
_rtB -> B_185_1533_0 ; _rtDW -> UnitDelay1_DSTATE_he [ 0 ] = _rtB ->
B_185_1550_0 ; _rtDW -> UnitDelay1_DSTATE_he [ 1 ] = _rtB -> B_185_1554_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_o = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_pe = _rtP -> P_946 * _rtB -> B_185_1561_0 +
_rtB -> B_185_1562_0 ; _rtDW -> UnitDelay1_DSTATE_b5 [ 0 ] = _rtB ->
B_185_1579_0 ; _rtDW -> UnitDelay1_DSTATE_b5 [ 1 ] = _rtB -> B_185_1583_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_a = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_e = _rtP -> P_952 * _rtB -> B_185_1590_0 + _rtB
-> B_185_1591_0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit != 0 ) {
_rtDW -> Delay_x1_DSTATE_c = _rtB -> B_185_1648_0 ; _rtDW ->
Delay_x2_DSTATE_mh = _rtB -> B_185_1650_0 ; } isHit = ssIsSampleHit ( S , 1 ,
0 ) ; if ( isHit != 0 ) { _rtDW -> Memory_PreviousInput_f1 = _rtB ->
B_185_1701_0 ; } normal_operation_no_faults_AutomaticGainControl_Update ( S ,
& _rtB -> AutomaticGainControl_d , & _rtDW -> AutomaticGainControl_d , & _rtP
-> AutomaticGainControl_d , & ( ( X_normal_operation_no_faults_T * )
ssGetContStates ( S ) ) -> AutomaticGainControl_d ) ; if ( _rtX ->
Integrator_CSTATE_lt == _rtP -> P_981 ) { switch ( _rtDW -> Integrator_MODE_j
) { case 3 : if ( _rtB -> B_185_1678_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_j
= 1 ; } break ; case 1 : if ( _rtB -> B_185_1678_0 >= 0.0 ) { _rtDW ->
Integrator_MODE_j = 3 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_185_1678_0 < 0.0 ) { _rtDW -> Integrator_MODE_j = 1 ; } else {
_rtDW -> Integrator_MODE_j = 3 ; } break ; } } else if ( _rtX ->
Integrator_CSTATE_lt == _rtP -> P_982 ) { switch ( _rtDW -> Integrator_MODE_j
) { case 4 : if ( _rtB -> B_185_1678_0 > 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_j
= 2 ; } break ; case 2 : if ( _rtB -> B_185_1678_0 <= 0.0 ) { _rtDW ->
Integrator_MODE_j = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_185_1678_0 > 0.0 ) { _rtDW -> Integrator_MODE_j = 2 ; } else {
_rtDW -> Integrator_MODE_j = 4 ; } break ; } } else { _rtDW ->
Integrator_MODE_j = 0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK_f . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_f . TUbufferPtrs [ 1 ] ;
real_T * * xBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_f
. TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
VariableTransportDelay_IWORK_g . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK_g . Head < ( _rtDW ->
VariableTransportDelay_IWORK_g . CircularBufSize - 1 ) ) ? ( _rtDW ->
VariableTransportDelay_IWORK_g . Head + 1 ) : 0 ) ; if ( _rtDW ->
VariableTransportDelay_IWORK_g . Head == _rtDW ->
VariableTransportDelay_IWORK_g . Tail ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
VariableTransportDelay_IWORK_g . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK_g . Tail , & _rtDW ->
VariableTransportDelay_IWORK_g . Head , & _rtDW ->
VariableTransportDelay_IWORK_g . Last , simTime - _rtP -> P_983 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , & _rtDW ->
VariableTransportDelay_IWORK_g . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_g . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_g . Head ] = _rtB -> B_185_1669_0 ; ( * xBuffer
) [ _rtDW -> VariableTransportDelay_IWORK_g . Head ] = ( (
X_normal_operation_no_faults_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_i ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtDW -> Memory_PreviousInput_o = _rtB -> B_185_1675_0 ; }
if ( _rtDW -> LastMajorTimeA_n == ( rtInf ) ) { _rtDW -> LastMajorTimeA_n =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_p = _rtB -> B_185_1684_0 ; } else
if ( _rtDW -> LastMajorTimeB_n == ( rtInf ) ) { _rtDW -> LastMajorTimeB_n =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB_b = _rtB -> B_185_1684_0 ; } else
if ( _rtDW -> LastMajorTimeA_n < _rtDW -> LastMajorTimeB_n ) { _rtDW ->
LastMajorTimeA_n = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_p = _rtB ->
B_185_1684_0 ; } else { _rtDW -> LastMajorTimeB_n = ssGetTaskTime ( S , 0 ) ;
_rtDW -> PrevYB_b = _rtB -> B_185_1684_0 ; } isHit = ssIsSampleHit ( S , 2 ,
0 ) ; if ( isHit != 0 ) { _rtDW -> UnitDelay1_DSTATE_l [ 0 ] = _rtB ->
B_185_1730_0 ; _rtDW -> UnitDelay1_DSTATE_l [ 1 ] = _rtB -> B_185_1734_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_cg = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_if = _rtP -> P_1027 * _rtB -> B_185_1741_0 +
_rtB -> B_185_1742_0 ; _rtDW -> UnitDelay1_DSTATE_fb [ 0 ] = _rtB ->
B_185_1759_0 ; _rtDW -> UnitDelay1_DSTATE_fb [ 1 ] = _rtB -> B_185_1763_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_f = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_l = _rtP -> P_1033 * _rtB -> B_185_1770_0 +
_rtB -> B_185_1771_0 ; _rtDW -> UnitDelay1_DSTATE_d [ 0 ] = _rtB ->
B_185_1788_0 ; _rtDW -> UnitDelay1_DSTATE_d [ 1 ] = _rtB -> B_185_1792_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_ho = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_m = _rtP -> P_1039 * _rtB -> B_185_1799_0 +
_rtB -> B_185_1800_0 ; _rtDW -> UnitDelay1_DSTATE_n [ 0 ] = _rtB ->
B_185_1817_0 ; _rtDW -> UnitDelay1_DSTATE_n [ 1 ] = _rtB -> B_185_1821_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_k = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_g = _rtP -> P_1045 * _rtB -> B_185_1828_0 +
_rtB -> B_185_1829_0 ; _rtDW -> UnitDelay1_DSTATE_hg [ 0 ] = _rtB ->
B_185_1846_0 ; _rtDW -> UnitDelay1_DSTATE_hg [ 1 ] = _rtB -> B_185_1850_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_fr = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_k = _rtP -> P_1051 * _rtB -> B_185_1857_0 +
_rtB -> B_185_1858_0 ; _rtDW -> UnitDelay1_DSTATE_nq [ 0 ] = _rtB ->
B_185_1875_0 ; _rtDW -> UnitDelay1_DSTATE_nq [ 1 ] = _rtB -> B_185_1879_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_lt = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_jd = _rtP -> P_1057 * _rtB -> B_185_1886_0 +
_rtB -> B_185_1887_0 ; } isHit = ssIsSampleHit ( S , 3 , 0 ) ; if ( isHit !=
0 ) { _rtDW -> Delay_x1_DSTATE_b = _rtB -> B_185_1944_0 ; _rtDW ->
Delay_x2_DSTATE_e = _rtB -> B_185_1946_0 ; } isHit = ssIsSampleHit ( S , 1 ,
0 ) ; if ( isHit != 0 ) { _rtDW -> Memory_PreviousInput_c = _rtB ->
B_185_1997_0 ; } normal_operation_no_faults_AutomaticGainControl_Update ( S ,
& _rtB -> AutomaticGainControl_a , & _rtDW -> AutomaticGainControl_a , & _rtP
-> AutomaticGainControl_a , & ( ( X_normal_operation_no_faults_T * )
ssGetContStates ( S ) ) -> AutomaticGainControl_a ) ; if ( _rtX ->
Integrator_CSTATE_bf == _rtP -> P_1086 ) { switch ( _rtDW ->
Integrator_MODE_m ) { case 3 : if ( _rtB -> B_185_1974_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_m
= 1 ; } break ; case 1 : if ( _rtB -> B_185_1974_0 >= 0.0 ) { _rtDW ->
Integrator_MODE_m = 3 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_185_1974_0 < 0.0 ) { _rtDW -> Integrator_MODE_m = 1 ; } else {
_rtDW -> Integrator_MODE_m = 3 ; } break ; } } else if ( _rtX ->
Integrator_CSTATE_bf == _rtP -> P_1087 ) { switch ( _rtDW ->
Integrator_MODE_m ) { case 4 : if ( _rtB -> B_185_1974_0 > 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_m
= 2 ; } break ; case 2 : if ( _rtB -> B_185_1974_0 <= 0.0 ) { _rtDW ->
Integrator_MODE_m = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_185_1974_0 > 0.0 ) { _rtDW -> Integrator_MODE_m = 2 ; } else {
_rtDW -> Integrator_MODE_m = 4 ; } break ; } } else { _rtDW ->
Integrator_MODE_m = 0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK_a . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_a . TUbufferPtrs [ 1 ] ;
real_T * * xBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_a
. TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
VariableTransportDelay_IWORK_b . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK_b . Head < ( _rtDW ->
VariableTransportDelay_IWORK_b . CircularBufSize - 1 ) ) ? ( _rtDW ->
VariableTransportDelay_IWORK_b . Head + 1 ) : 0 ) ; if ( _rtDW ->
VariableTransportDelay_IWORK_b . Head == _rtDW ->
VariableTransportDelay_IWORK_b . Tail ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
VariableTransportDelay_IWORK_b . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK_b . Tail , & _rtDW ->
VariableTransportDelay_IWORK_b . Head , & _rtDW ->
VariableTransportDelay_IWORK_b . Last , simTime - _rtP -> P_1088 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , & _rtDW ->
VariableTransportDelay_IWORK_b . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_b . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_b . Head ] = _rtB -> B_185_1965_0 ; ( * xBuffer
) [ _rtDW -> VariableTransportDelay_IWORK_b . Head ] = ( (
X_normal_operation_no_faults_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_o ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtDW -> Memory_PreviousInput_d = _rtB -> B_185_1971_0 ; }
if ( _rtDW -> LastMajorTimeA_e == ( rtInf ) ) { _rtDW -> LastMajorTimeA_e =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_a = _rtB -> B_185_1980_0 ; } else
if ( _rtDW -> LastMajorTimeB_i == ( rtInf ) ) { _rtDW -> LastMajorTimeB_i =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB_h = _rtB -> B_185_1980_0 ; } else
if ( _rtDW -> LastMajorTimeA_e < _rtDW -> LastMajorTimeB_i ) { _rtDW ->
LastMajorTimeA_e = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_a = _rtB ->
B_185_1980_0 ; } else { _rtDW -> LastMajorTimeB_i = ssGetTaskTime ( S , 0 ) ;
_rtDW -> PrevYB_h = _rtB -> B_185_1980_0 ; } isHit = ssIsSampleHit ( S , 2 ,
0 ) ; if ( isHit != 0 ) { _rtDW -> UnitDelay1_DSTATE_f2 [ 0 ] = _rtB ->
B_185_2026_0 ; _rtDW -> UnitDelay1_DSTATE_f2 [ 1 ] = _rtB -> B_185_2030_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_cp = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_mx = _rtP -> P_1132 * _rtB -> B_185_2037_0 +
_rtB -> B_185_2038_0 ; _rtDW -> UnitDelay1_DSTATE_d1 [ 0 ] = _rtB ->
B_185_2055_0 ; _rtDW -> UnitDelay1_DSTATE_d1 [ 1 ] = _rtB -> B_185_2059_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_md = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_m3 = _rtP -> P_1138 * _rtB -> B_185_2066_0 +
_rtB -> B_185_2067_0 ; _rtDW -> UnitDelay1_DSTATE_p2 [ 0 ] = _rtB ->
B_185_2084_0 ; _rtDW -> UnitDelay1_DSTATE_p2 [ 1 ] = _rtB -> B_185_2088_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_iy = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_pv = _rtP -> P_1144 * _rtB -> B_185_2095_0 +
_rtB -> B_185_2096_0 ; _rtDW -> UnitDelay1_DSTATE_l4 [ 0 ] = _rtB ->
B_185_2113_0 ; _rtDW -> UnitDelay1_DSTATE_l4 [ 1 ] = _rtB -> B_185_2117_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_p = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_c2 = _rtP -> P_1150 * _rtB -> B_185_2124_0 +
_rtB -> B_185_2125_0 ; _rtDW -> UnitDelay1_DSTATE_m [ 0 ] = _rtB ->
B_185_2142_0 ; _rtDW -> UnitDelay1_DSTATE_m [ 1 ] = _rtB -> B_185_2146_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_fi = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_er = _rtP -> P_1156 * _rtB -> B_185_2153_0 +
_rtB -> B_185_2154_0 ; _rtDW -> UnitDelay1_DSTATE_c [ 0 ] = _rtB ->
B_185_2171_0 ; _rtDW -> UnitDelay1_DSTATE_c [ 1 ] = _rtB -> B_185_2175_0 ;
_rtDW -> DiscreteTimeIntegrator_SYSTEM_ENABLE_hj = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_j2 = _rtP -> P_1162 * _rtB -> B_185_2182_0 +
_rtB -> B_185_2183_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * *
) & _rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 3 ] ; real_T simTime =
ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK . Head [ 0 ] = ( ( _rtDW ->
TransportDelay_IWORK . Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK .
CircularBufSize [ 0 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK . Head [ 0 ]
+ 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK . Head [ 0 ] == _rtDW ->
TransportDelay_IWORK . Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK . Head [
0 ] , & _rtDW -> TransportDelay_IWORK . Last [ 0 ] , simTime - _rtP -> P_1165
, tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK . Head [ 0 ] ] = _rtB -> B_185_2284_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK . Head
[ 1 ] < ( _rtDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK . Head [ 1 ] == _rtDW -> TransportDelay_IWORK . Tail [ 1
] ) { if ( ! normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( &
_rtDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK . Head [
1 ] , & _rtDW -> TransportDelay_IWORK . Last [ 1 ] , simTime - _rtP -> P_1165
, tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK . Head [ 1 ] ] = _rtB -> B_185_2284_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK . Head
[ 2 ] < ( _rtDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK . Head [ 2 ] == _rtDW -> TransportDelay_IWORK . Tail [ 2
] ) { if ( ! normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( &
_rtDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK . Head [
2 ] , & _rtDW -> TransportDelay_IWORK . Last [ 2 ] , simTime - _rtP -> P_1165
, tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK . Head [ 2 ] ] = _rtB -> B_185_2284_0 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_b [ 0 ] = _rtB -> B_185_2291_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_b [ 1 ] = _rtB -> B_185_2291_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_b [ 2 ] = _rtB -> B_185_2291_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_i . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_i .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_p . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_p .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_p . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_p . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_p . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_p .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_p . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_p . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_p .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_p . Last [ 0 ] , simTime - _rtP
-> P_1170 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_p . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_p . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_p . Head [ 0 ] ] = _rtB -> B_185_2292_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_p . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_p .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_p . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_p . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_p . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_p .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_p . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_p . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_p .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_p . Last [ 1 ] , simTime - _rtP
-> P_1170 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_p . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_p . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_p . Head [ 1 ] ] = _rtB -> B_185_2292_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_p . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_p .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_p . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_p . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_p . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_p .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_p . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_p . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_p .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_p . Last [ 2 ] , simTime - _rtP
-> P_1170 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_p . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_p . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_p . Head [ 2 ] ] = _rtB -> B_185_2292_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_br [ 0 ] = _rtB -> B_185_2299_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_br [ 1 ] = _rtB -> B_185_2299_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_br [ 2 ] = _rtB -> B_185_2299_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_p . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_p .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_i . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_i .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_i . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_i . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_i . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_i .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_i . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_i . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_i .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_i . Last [ 0 ] , simTime - _rtP
-> P_1175 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_i . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_i . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_i . Head [ 0 ] ] = _rtB -> B_185_2302_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_i . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_i .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_i . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_i . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_i . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_i .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_i . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_i . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_i .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_i . Last [ 1 ] , simTime - _rtP
-> P_1175 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_i . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_i . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_i . Head [ 1 ] ] = _rtB -> B_185_2302_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_i . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_i .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_i . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_i . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_i . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_i .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_i . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_i . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_i .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_i . Last [ 2 ] , simTime - _rtP
-> P_1175 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_i . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_i . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_i . Head [ 2 ] ] = _rtB -> B_185_2302_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_e [ 0 ] = _rtB -> B_185_2309_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_e [ 1 ] = _rtB -> B_185_2309_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_e [ 2 ] = _rtB -> B_185_2309_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_o . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_o .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_m . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_m .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_m . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_m . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_m . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_m .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_m . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_m . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_m .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_m . Last [ 0 ] , simTime - _rtP
-> P_1180 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_m . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_m . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_m . Head [ 0 ] ] = _rtB -> B_185_2310_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_m . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_m .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_m . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_m . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_m . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_m .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_m . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_m . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_m .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_m . Last [ 1 ] , simTime - _rtP
-> P_1180 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_m . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_m . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_m . Head [ 1 ] ] = _rtB -> B_185_2310_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_m . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_m .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_m . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_m . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_m . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_m .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_m . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_m . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_m .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_m . Last [ 2 ] , simTime - _rtP
-> P_1180 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_m . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_m . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_m . Head [ 2 ] ] = _rtB -> B_185_2310_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_j [ 0 ] = _rtB -> B_185_2317_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_j [ 1 ] = _rtB -> B_185_2317_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_j [ 2 ] = _rtB -> B_185_2317_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_a . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_a .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_a . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_a .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_a . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_a . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_a . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_a .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_a . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_a . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_a .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_a . Last [ 0 ] , simTime - _rtP
-> P_1209 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_a . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_a . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_a . Head [ 0 ] ] = _rtB -> B_185_2342_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_a . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_a .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_a . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_a . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_a . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_a .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_a . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_a . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_a .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_a . Last [ 1 ] , simTime - _rtP
-> P_1209 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_a . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_a . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_a . Head [ 1 ] ] = _rtB -> B_185_2342_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_a . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_a .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_a . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_a . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_a . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_a .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_a . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_a . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_a .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_a . Last [ 2 ] , simTime - _rtP
-> P_1209 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_a . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_a . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_a . Head [ 2 ] ] = _rtB -> B_185_2342_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_k [ 0 ] = _rtB -> B_185_2349_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_k [ 1 ] = _rtB -> B_185_2349_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_k [ 2 ] = _rtB -> B_185_2349_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_j . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_j .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_e . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_e .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_e . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_e . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_e . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_e .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_e . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_e . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_e .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_e . Last [ 0 ] , simTime - _rtP
-> P_1214 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_e . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_e . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_e . Head [ 0 ] ] = _rtB -> B_185_2350_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_e . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_e .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_e . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_e . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_e . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_e .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_e . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_e . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_e .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_e . Last [ 1 ] , simTime - _rtP
-> P_1214 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_e . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_e . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_e . Head [ 1 ] ] = _rtB -> B_185_2350_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_e . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_e .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_e . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_e . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_e . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_e .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_e . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_e . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_e .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_e . Last [ 2 ] , simTime - _rtP
-> P_1214 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_e . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_e . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_e . Head [ 2 ] ] = _rtB -> B_185_2350_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_a [ 0 ] = _rtB -> B_185_2357_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_a [ 1 ] = _rtB -> B_185_2357_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_a [ 2 ] = _rtB -> B_185_2357_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_f . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_f .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_l . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_l .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_l . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_l . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_l . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_l .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_l . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_l . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_l .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_l . Last [ 0 ] , simTime - _rtP
-> P_1219 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_l . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_l . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_l . Head [ 0 ] ] = _rtB -> B_185_2360_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_l . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_l .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_l . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_l . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_l . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_l .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_l . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_l . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_l .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_l . Last [ 1 ] , simTime - _rtP
-> P_1219 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_l . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_l . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_l . Head [ 1 ] ] = _rtB -> B_185_2360_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_l . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_l .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_l . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_l . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_l . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_l .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_l . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_l . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_l .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_l . Last [ 2 ] , simTime - _rtP
-> P_1219 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_l . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_l . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_l . Head [ 2 ] ] = _rtB -> B_185_2360_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_cv [ 0 ] = _rtB -> B_185_2367_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_cv [ 1 ] = _rtB -> B_185_2367_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_cv [ 2 ] = _rtB -> B_185_2367_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_n . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_n .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_ef . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_ef .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_ef . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_ef . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_ef . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_ef .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_ef . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_ef . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_ef .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_ef . Last [ 0 ] , simTime - _rtP
-> P_1224 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_ef . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_ef . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_ef . Head [ 0 ] ] = _rtB -> B_185_2368_0 [ 0 ] ;
_rtDW -> TransportDelay_IWORK_ef . Head [ 1 ] = ( ( _rtDW ->
TransportDelay_IWORK_ef . Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_ef .
CircularBufSize [ 1 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_ef . Head [ 1
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_ef . Head [ 1 ] == _rtDW
-> TransportDelay_IWORK_ef . Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_ef . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_ef . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_ef .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_ef . Last [ 1 ] , simTime - _rtP
-> P_1224 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_ef . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_ef . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_ef . Head [ 1 ] ] = _rtB -> B_185_2368_0 [ 1 ] ;
_rtDW -> TransportDelay_IWORK_ef . Head [ 2 ] = ( ( _rtDW ->
TransportDelay_IWORK_ef . Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_ef .
CircularBufSize [ 2 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_ef . Head [ 2
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_ef . Head [ 2 ] == _rtDW
-> TransportDelay_IWORK_ef . Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_ef . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_ef . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_ef .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_ef . Last [ 2 ] , simTime - _rtP
-> P_1224 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_ef . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_ef . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_ef . Head [ 2 ] ] = _rtB -> B_185_2368_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_p [ 0 ] = _rtB -> B_185_2375_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_p [ 1 ] = _rtB -> B_185_2375_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_p [ 2 ] = _rtB -> B_185_2375_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_ju . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_ju
. TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_as . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_as .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_as . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_as . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_as . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_as .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_as . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_as . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_as .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_as . Last [ 0 ] , simTime - _rtP
-> P_1253 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_as . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_as . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_as . Head [ 0 ] ] = _rtB -> B_185_2400_0 [ 0 ] ;
_rtDW -> TransportDelay_IWORK_as . Head [ 1 ] = ( ( _rtDW ->
TransportDelay_IWORK_as . Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_as .
CircularBufSize [ 1 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_as . Head [ 1
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_as . Head [ 1 ] == _rtDW
-> TransportDelay_IWORK_as . Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_as . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_as . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_as .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_as . Last [ 1 ] , simTime - _rtP
-> P_1253 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_as . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_as . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_as . Head [ 1 ] ] = _rtB -> B_185_2400_0 [ 1 ] ;
_rtDW -> TransportDelay_IWORK_as . Head [ 2 ] = ( ( _rtDW ->
TransportDelay_IWORK_as . Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_as .
CircularBufSize [ 2 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_as . Head [ 2
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_as . Head [ 2 ] == _rtDW
-> TransportDelay_IWORK_as . Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_as . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_as . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_as .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_as . Last [ 2 ] , simTime - _rtP
-> P_1253 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_as . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_as . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_as . Head [ 2 ] ] = _rtB -> B_185_2400_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_p1 [ 0 ] = _rtB -> B_185_2407_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_p1 [ 1 ] = _rtB -> B_185_2407_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_p1 [ 2 ] = _rtB -> B_185_2407_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_g . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_g .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_n . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_n .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_n . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_n . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_n . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_n .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_n . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_n . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_n .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_n . Last [ 0 ] , simTime - _rtP
-> P_1258 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_n . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 0 ] ] = _rtB -> B_185_2408_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_n . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_n .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_n . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_n . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_n . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_n .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_n . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_n . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_n .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_n . Last [ 1 ] , simTime - _rtP
-> P_1258 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_n . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 1 ] ] = _rtB -> B_185_2408_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_n . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_n .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_n . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_n . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_n . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_n .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_n . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_n . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_n .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_n . Last [ 2 ] , simTime - _rtP
-> P_1258 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_n . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 2 ] ] = _rtB -> B_185_2408_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_g [ 0 ] = _rtB -> B_185_2415_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_g [ 1 ] = _rtB -> B_185_2415_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_g [ 2 ] = _rtB -> B_185_2415_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_iy . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_iy
. TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_g . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_g .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_g . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_g . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_g . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_g .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_g . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_g . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_g .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_g . Last [ 0 ] , simTime - _rtP
-> P_1263 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_g . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_g . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_g . Head [ 0 ] ] = _rtB -> B_185_2418_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_g . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_g .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_g . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_g . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_g . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_g .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_g . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_g . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_g .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_g . Last [ 1 ] , simTime - _rtP
-> P_1263 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_g . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_g . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_g . Head [ 1 ] ] = _rtB -> B_185_2418_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_g . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_g .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_g . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_g . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_g . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_g .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_g . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_g . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_g .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_g . Last [ 2 ] , simTime - _rtP
-> P_1263 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_g . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_g . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_g . Head [ 2 ] ] = _rtB -> B_185_2418_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_dz [ 0 ] = _rtB -> B_185_2425_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_dz [ 1 ] = _rtB -> B_185_2425_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_dz [ 2 ] = _rtB -> B_185_2425_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_i5 . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_i5
. TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_lh . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_lh .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_lh . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_lh . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_lh . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_lh .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_lh . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_lh . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_lh .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_lh . Last [ 0 ] , simTime - _rtP
-> P_1268 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_lh . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_lh . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_lh . Head [ 0 ] ] = _rtB -> B_185_2426_0 [ 0 ] ;
_rtDW -> TransportDelay_IWORK_lh . Head [ 1 ] = ( ( _rtDW ->
TransportDelay_IWORK_lh . Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_lh .
CircularBufSize [ 1 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_lh . Head [ 1
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_lh . Head [ 1 ] == _rtDW
-> TransportDelay_IWORK_lh . Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_lh . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_lh . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_lh .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_lh . Last [ 1 ] , simTime - _rtP
-> P_1268 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_lh . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_lh . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_lh . Head [ 1 ] ] = _rtB -> B_185_2426_0 [ 1 ] ;
_rtDW -> TransportDelay_IWORK_lh . Head [ 2 ] = ( ( _rtDW ->
TransportDelay_IWORK_lh . Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_lh .
CircularBufSize [ 2 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_lh . Head [ 2
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_lh . Head [ 2 ] == _rtDW
-> TransportDelay_IWORK_lh . Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_lh . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_lh . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_lh .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_lh . Last [ 2 ] , simTime - _rtP
-> P_1268 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_lh . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_lh . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_lh . Head [ 2 ] ] = _rtB -> B_185_2426_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_fl [ 0 ] = _rtB -> B_185_2433_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_fl [ 1 ] = _rtB -> B_185_2433_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_fl [ 2 ] = _rtB -> B_185_2433_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_nm . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_nm
. TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_k . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_k .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_k . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_k . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_k . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_k .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_k . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_k . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_k .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_k . Last [ 0 ] , simTime - _rtP
-> P_1297 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_k . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_k . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_k . Head [ 0 ] ] = _rtB -> B_185_2458_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_k . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_k .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_k . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_k . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_k . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_k .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_k . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_k . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_k .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_k . Last [ 1 ] , simTime - _rtP
-> P_1297 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_k . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_k . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_k . Head [ 1 ] ] = _rtB -> B_185_2458_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_k . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_k .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_k . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_k . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_k . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_k .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_k . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_k . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_k .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_k . Last [ 2 ] , simTime - _rtP
-> P_1297 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_k . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_k . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_k . Head [ 2 ] ] = _rtB -> B_185_2458_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_gn [ 0 ] = _rtB -> B_185_2465_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_gn [ 1 ] = _rtB -> B_185_2465_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_gn [ 2 ] = _rtB -> B_185_2465_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_h . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_h .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_kd . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_kd .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_kd . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_kd . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_kd . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_kd .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_kd . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_kd . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_kd .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_kd . Last [ 0 ] , simTime - _rtP
-> P_1302 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_kd . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_kd . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_kd . Head [ 0 ] ] = _rtB -> B_185_2466_0 [ 0 ] ;
_rtDW -> TransportDelay_IWORK_kd . Head [ 1 ] = ( ( _rtDW ->
TransportDelay_IWORK_kd . Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_kd .
CircularBufSize [ 1 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_kd . Head [ 1
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_kd . Head [ 1 ] == _rtDW
-> TransportDelay_IWORK_kd . Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_kd . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_kd . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_kd .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_kd . Last [ 1 ] , simTime - _rtP
-> P_1302 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_kd . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_kd . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_kd . Head [ 1 ] ] = _rtB -> B_185_2466_0 [ 1 ] ;
_rtDW -> TransportDelay_IWORK_kd . Head [ 2 ] = ( ( _rtDW ->
TransportDelay_IWORK_kd . Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_kd .
CircularBufSize [ 2 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_kd . Head [ 2
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_kd . Head [ 2 ] == _rtDW
-> TransportDelay_IWORK_kd . Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_kd . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_kd . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_kd .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_kd . Last [ 2 ] , simTime - _rtP
-> P_1302 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_kd . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_kd . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_kd . Head [ 2 ] ] = _rtB -> B_185_2466_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_e2 [ 0 ] = _rtB -> B_185_2473_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_e2 [ 1 ] = _rtB -> B_185_2473_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_e2 [ 2 ] = _rtB -> B_185_2473_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_pw . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_pw
. TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_it . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_it .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_it . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_it . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_it . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_it .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_it . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_it . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_it .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_it . Last [ 0 ] , simTime - _rtP
-> P_1307 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_it . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_it . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_it . Head [ 0 ] ] = _rtB -> B_185_2476_0 [ 0 ] ;
_rtDW -> TransportDelay_IWORK_it . Head [ 1 ] = ( ( _rtDW ->
TransportDelay_IWORK_it . Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_it .
CircularBufSize [ 1 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_it . Head [ 1
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_it . Head [ 1 ] == _rtDW
-> TransportDelay_IWORK_it . Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_it . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_it . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_it .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_it . Last [ 1 ] , simTime - _rtP
-> P_1307 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_it . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_it . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_it . Head [ 1 ] ] = _rtB -> B_185_2476_0 [ 1 ] ;
_rtDW -> TransportDelay_IWORK_it . Head [ 2 ] = ( ( _rtDW ->
TransportDelay_IWORK_it . Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_it .
CircularBufSize [ 2 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_it . Head [ 2
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_it . Head [ 2 ] == _rtDW
-> TransportDelay_IWORK_it . Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_it . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_it . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_it .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_it . Last [ 2 ] , simTime - _rtP
-> P_1307 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_it . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_it . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_it . Head [ 2 ] ] = _rtB -> B_185_2476_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_n [ 0 ] = _rtB -> B_185_2483_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_n [ 1 ] = _rtB -> B_185_2483_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_n [ 2 ] = _rtB -> B_185_2483_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_d . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_d .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_c . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_c .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_c . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_c . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_c . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_c .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_c . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_c . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_c .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_c . Last [ 0 ] , simTime - _rtP
-> P_1312 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_c . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_c . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_c . Head [ 0 ] ] = _rtB -> B_185_2484_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_c . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_c .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_c . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_c . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_c . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_c .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_c . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_c . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_c .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_c . Last [ 1 ] , simTime - _rtP
-> P_1312 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_c . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_c . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_c . Head [ 1 ] ] = _rtB -> B_185_2484_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_c . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_c .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_c . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_c . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_c . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_c .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_c . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_c . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_c .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_c . Last [ 2 ] , simTime - _rtP
-> P_1312 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_c . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_c . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_c . Head [ 2 ] ] = _rtB -> B_185_2484_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_i [ 0 ] = _rtB -> B_185_2491_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_i [ 1 ] = _rtB -> B_185_2491_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_i [ 2 ] = _rtB -> B_185_2491_0 [ 2 ] ; }
normal_operation_no_faults_TrueRMS_Update ( S , & _rtB -> TrueRMS , & _rtDW
-> TrueRMS , & _rtP -> TrueRMS ) ; normal_operation_no_faults_RMS_Update ( S
, & _rtB -> RMS , & _rtDW -> RMS , & _rtP -> RMS ) ; { real_T * * uBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK_k . TUbufferPtrs [ 0 ] ; real_T
* * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_k . TUbufferPtrs
[ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_mv .
Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_mv . Head [ 0 ] < ( _rtDW ->
TransportDelay_IWORK_mv . CircularBufSize [ 0 ] - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK_mv . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_mv . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_mv .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_mv . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_mv . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_mv .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_mv . Last [ 0 ] , simTime - _rtP
-> P_1376 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_mv . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_mv . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_mv . Head [ 0 ] ] = _rtB -> B_185_2574_0 [ 0 ] ;
_rtDW -> TransportDelay_IWORK_mv . Head [ 1 ] = ( ( _rtDW ->
TransportDelay_IWORK_mv . Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_mv .
CircularBufSize [ 1 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_mv . Head [ 1
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_mv . Head [ 1 ] == _rtDW
-> TransportDelay_IWORK_mv . Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_mv . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_mv . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_mv .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_mv . Last [ 1 ] , simTime - _rtP
-> P_1376 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_mv . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_mv . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_mv . Head [ 1 ] ] = _rtB -> B_185_2574_0 [ 1 ] ;
_rtDW -> TransportDelay_IWORK_mv . Head [ 2 ] = ( ( _rtDW ->
TransportDelay_IWORK_mv . Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_mv .
CircularBufSize [ 2 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_mv . Head [ 2
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_mv . Head [ 2 ] == _rtDW
-> TransportDelay_IWORK_mv . Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_mv . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_mv . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_mv .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_mv . Last [ 2 ] , simTime - _rtP
-> P_1376 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_mv . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_mv . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_mv . Head [ 2 ] ] = _rtB -> B_185_2574_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_bk [ 0 ] = _rtB -> B_185_2581_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_bk [ 1 ] = _rtB -> B_185_2581_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_bk [ 2 ] = _rtB -> B_185_2581_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_hv . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_hv
. TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_a1 . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_a1 .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_a1 . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_a1 . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_a1 . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_a1 .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_a1 . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_a1 . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_a1 .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_a1 . Last [ 0 ] , simTime - _rtP
-> P_1381 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_a1 . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_a1 . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_a1 . Head [ 0 ] ] = _rtB -> B_185_2582_0 [ 0 ] ;
_rtDW -> TransportDelay_IWORK_a1 . Head [ 1 ] = ( ( _rtDW ->
TransportDelay_IWORK_a1 . Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_a1 .
CircularBufSize [ 1 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_a1 . Head [ 1
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_a1 . Head [ 1 ] == _rtDW
-> TransportDelay_IWORK_a1 . Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_a1 . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_a1 . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_a1 .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_a1 . Last [ 1 ] , simTime - _rtP
-> P_1381 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_a1 . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_a1 . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_a1 . Head [ 1 ] ] = _rtB -> B_185_2582_0 [ 1 ] ;
_rtDW -> TransportDelay_IWORK_a1 . Head [ 2 ] = ( ( _rtDW ->
TransportDelay_IWORK_a1 . Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_a1 .
CircularBufSize [ 2 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_a1 . Head [ 2
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_a1 . Head [ 2 ] == _rtDW
-> TransportDelay_IWORK_a1 . Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_a1 . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_a1 . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_a1 .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_a1 . Last [ 2 ] , simTime - _rtP
-> P_1381 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_a1 . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_a1 . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_a1 . Head [ 2 ] ] = _rtB -> B_185_2582_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_ab [ 0 ] = _rtB -> B_185_2589_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_ab [ 1 ] = _rtB -> B_185_2589_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_ab [ 2 ] = _rtB -> B_185_2589_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_e . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_e .
TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_l0 . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_l0 .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_l0 . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_l0 . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_l0 . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_l0 .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_l0 . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_l0 . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_l0 .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_l0 . Last [ 0 ] , simTime - _rtP
-> P_1386 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_l0 . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_l0 . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_l0 . Head [ 0 ] ] = _rtB -> B_185_2592_0 [ 0 ] ;
_rtDW -> TransportDelay_IWORK_l0 . Head [ 1 ] = ( ( _rtDW ->
TransportDelay_IWORK_l0 . Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_l0 .
CircularBufSize [ 1 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_l0 . Head [ 1
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_l0 . Head [ 1 ] == _rtDW
-> TransportDelay_IWORK_l0 . Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_l0 . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_l0 . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_l0 .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_l0 . Last [ 1 ] , simTime - _rtP
-> P_1386 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_l0 . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_l0 . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK_l0 . Head [ 1 ] ] = _rtB -> B_185_2592_0 [ 1 ] ;
_rtDW -> TransportDelay_IWORK_l0 . Head [ 2 ] = ( ( _rtDW ->
TransportDelay_IWORK_l0 . Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_l0 .
CircularBufSize [ 2 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_l0 . Head [ 2
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_l0 . Head [ 2 ] == _rtDW
-> TransportDelay_IWORK_l0 . Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_l0 . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_l0 . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_l0 .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_l0 . Last [ 2 ] , simTime - _rtP
-> P_1386 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_l0 . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_l0 . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_l0 . Head [ 2 ] ] = _rtB -> B_185_2592_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_jf [ 0 ] = _rtB -> B_185_2599_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_jf [ 1 ] = _rtB -> B_185_2599_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_jf [ 2 ] = _rtB -> B_185_2599_0 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_dm . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_dm
. TUbufferPtrs [ 3 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_b . Head [ 0 ] = ( ( _rtDW -> TransportDelay_IWORK_b .
Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_b . CircularBufSize [ 0 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_b . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_b . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_b .
Tail [ 0 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_b . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_b . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_b .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_b . Last [ 0 ] , simTime - _rtP
-> P_1391 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_b . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_b . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_b . Head [ 0 ] ] = _rtB -> B_185_2600_0 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_b . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_b .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_b . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_b . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_b . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_b .
Tail [ 1 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_b . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_b . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_b .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_b . Last [ 1 ] , simTime - _rtP
-> P_1391 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_b . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_b . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_b . Head [ 1 ] ] = _rtB -> B_185_2600_0 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_b . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_b .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_b . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_b . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_b . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_b .
Tail [ 2 ] ) { if ( !
normal_operation_no_faults_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_b . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_b . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_b .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_b . Last [ 2 ] , simTime - _rtP
-> P_1391 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & _rtDW
-> TransportDelay_IWORK_b . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
TransportDelay_IWORK_b . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_b . Head [ 2 ] ] = _rtB -> B_185_2600_0 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_og [ 0 ] = _rtB -> B_185_2607_0 [ 0 ] ; _rtDW ->
Memory_PreviousInput_og [ 1 ] = _rtB -> B_185_2607_0 [ 1 ] ; _rtDW ->
Memory_PreviousInput_og [ 2 ] = _rtB -> B_185_2607_0 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Integ4_SYSTEM_ENABLE = 0U ; _rtDW -> Integ4_DSTATE = _rtP -> P_1415 * _rtB ->
B_185_2631_0 + _rtB -> B_185_2632_0 ; ssCallAccelRunBlock ( S , 185 , 2634 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE = _rtB -> B_185_2631_0 ;
_rtDW -> Integ4_SYSTEM_ENABLE_e = 0U ; _rtDW -> Integ4_DSTATE_i = _rtP ->
P_1427 * _rtB -> B_185_2645_0 + _rtB -> B_185_2646_0 ; ssCallAccelRunBlock (
S , 185 , 2648 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_f = _rtB ->
B_185_2645_0 ; _rtDW -> Integ4_SYSTEM_ENABLE_b = 0U ; _rtDW ->
Integ4_DSTATE_m = _rtP -> P_1437 * _rtB -> B_185_2656_0 + _rtB ->
B_185_2657_0 ; ssCallAccelRunBlock ( S , 185 , 2659 , SS_CALL_MDL_UPDATE ) ;
_rtDW -> Integ4_SYSTEM_ENABLE_p = 0U ; _rtDW -> Integ4_DSTATE_k = _rtP ->
P_1440 * _rtB -> B_185_2661_0 + _rtB -> B_185_2662_0 ; ssCallAccelRunBlock (
S , 185 , 2664 , SS_CALL_MDL_UPDATE ) ; _rtDW -> Integ4_SYSTEM_ENABLE_o = 0U
; _rtDW -> Integ4_DSTATE_k4 = _rtP -> P_1443 * _rtB -> B_185_2666_0 + _rtB ->
B_185_2667_0 ; ssCallAccelRunBlock ( S , 185 , 2669 , SS_CALL_MDL_UPDATE ) ;
_rtDW -> Integ4_SYSTEM_ENABLE_b2 = 0U ; _rtDW -> Integ4_DSTATE_h = _rtP ->
P_1446 * _rtB -> B_185_2671_0 + _rtB -> B_185_2672_0 ; ssCallAccelRunBlock (
S , 185 , 2674 , SS_CALL_MDL_UPDATE ) ; _rtDW -> Integ4_SYSTEM_ENABLE_m = 0U
; _rtDW -> Integ4_DSTATE_n = _rtP -> P_1449 * _rtB -> B_185_2676_0 + _rtB ->
B_185_2677_0 ; ssCallAccelRunBlock ( S , 185 , 2679 , SS_CALL_MDL_UPDATE ) ;
_rtDW -> Integ4_SYSTEM_ENABLE_f = 0U ; _rtDW -> Integ4_DSTATE_ho = _rtP ->
P_1452 * _rtB -> B_185_2681_0 + _rtB -> B_185_2682_0 ; ssCallAccelRunBlock (
S , 185 , 2684 , SS_CALL_MDL_UPDATE ) ; } if ( _rtDW -> TimeStampA_k == (
rtInf ) ) { _rtDW -> TimeStampA_k = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_dx ; } else if ( _rtDW -> TimeStampB_o2 == ( rtInf ) ) { _rtDW
-> TimeStampB_o2 = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_b ; } else
if ( _rtDW -> TimeStampA_k < _rtDW -> TimeStampB_o2 ) { _rtDW -> TimeStampA_k
= ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_dx ; } else { _rtDW ->
TimeStampB_o2 = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_b ; } * lastU
= _rtB -> B_185_3189_0 ; if ( _rtDW -> TimeStampA_fd == ( rtInf ) ) { _rtDW
-> TimeStampA_fd = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_m ; } else
if ( _rtDW -> TimeStampB_aw == ( rtInf ) ) { _rtDW -> TimeStampB_aw = ssGetT
( S ) ; lastU = & _rtDW -> LastUAtTimeB_e0 ; } else if ( _rtDW ->
TimeStampA_fd < _rtDW -> TimeStampB_aw ) { _rtDW -> TimeStampA_fd = ssGetT (
S ) ; lastU = & _rtDW -> LastUAtTimeA_m ; } else { _rtDW -> TimeStampB_aw =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_e0 ; } * lastU = _rtB ->
B_185_3200_0 ; if ( _rtDW -> TimeStampA_kq == ( rtInf ) ) { _rtDW ->
TimeStampA_kq = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_nq ; } else if
( _rtDW -> TimeStampB_n == ( rtInf ) ) { _rtDW -> TimeStampB_n = ssGetT ( S )
; lastU = & _rtDW -> LastUAtTimeB_pd ; } else if ( _rtDW -> TimeStampA_kq <
_rtDW -> TimeStampB_n ) { _rtDW -> TimeStampA_kq = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeA_nq ; } else { _rtDW -> TimeStampB_n = ssGetT ( S ) ;
lastU = & _rtDW -> LastUAtTimeB_pd ; } * lastU = _rtB -> B_185_3211_0 ; if (
_rtDW -> TimeStampA_k4 == ( rtInf ) ) { _rtDW -> TimeStampA_k4 = ssGetT ( S )
; lastU = & _rtDW -> LastUAtTimeA_gx ; } else if ( _rtDW -> TimeStampB_a3 ==
( rtInf ) ) { _rtDW -> TimeStampB_a3 = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_ea ; } else if ( _rtDW -> TimeStampA_k4 < _rtDW -> TimeStampB_a3
) { _rtDW -> TimeStampA_k4 = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_gx ; } else { _rtDW -> TimeStampB_a3 = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeB_ea ; } * lastU = _rtB -> B_185_3241_0 ; if ( _rtDW ->
TimeStampA_jj == ( rtInf ) ) { _rtDW -> TimeStampA_jj = ssGetT ( S ) ; lastU
= & _rtDW -> LastUAtTimeA_ab ; } else if ( _rtDW -> TimeStampB_la == ( rtInf
) ) { _rtDW -> TimeStampB_la = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_n ; } else if ( _rtDW -> TimeStampA_jj < _rtDW -> TimeStampB_la
) { _rtDW -> TimeStampA_jj = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_ab ; } else { _rtDW -> TimeStampB_la = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeB_n ; } * lastU = _rtB -> B_185_3252_0 ; if ( _rtDW ->
TimeStampA_l == ( rtInf ) ) { _rtDW -> TimeStampA_l = ssGetT ( S ) ; lastU =
& _rtDW -> LastUAtTimeA_g0 ; } else if ( _rtDW -> TimeStampB_jy == ( rtInf )
) { _rtDW -> TimeStampB_jy = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_l
; } else if ( _rtDW -> TimeStampA_l < _rtDW -> TimeStampB_jy ) { _rtDW ->
TimeStampA_l = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_g0 ; } else {
_rtDW -> TimeStampB_jy = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_l ; }
* lastU = _rtB -> B_185_3263_0 ; if ( _rtDW -> TimeStampA_jo == ( rtInf ) ) {
_rtDW -> TimeStampA_jo = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_br ;
} else if ( _rtDW -> TimeStampB_fi == ( rtInf ) ) { _rtDW -> TimeStampB_fi =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_ei ; } else if ( _rtDW ->
TimeStampA_jo < _rtDW -> TimeStampB_fi ) { _rtDW -> TimeStampA_jo = ssGetT (
S ) ; lastU = & _rtDW -> LastUAtTimeA_br ; } else { _rtDW -> TimeStampB_fi =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_ei ; } * lastU = _rtB ->
B_185_3294_0 ; if ( _rtDW -> TimeStampA_a == ( rtInf ) ) { _rtDW ->
TimeStampA_a = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_g1 ; } else if
( _rtDW -> TimeStampB_b == ( rtInf ) ) { _rtDW -> TimeStampB_b = ssGetT ( S )
; lastU = & _rtDW -> LastUAtTimeB_m ; } else if ( _rtDW -> TimeStampA_a <
_rtDW -> TimeStampB_b ) { _rtDW -> TimeStampA_a = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeA_g1 ; } else { _rtDW -> TimeStampB_b = ssGetT ( S ) ;
lastU = & _rtDW -> LastUAtTimeB_m ; } * lastU = _rtB -> B_185_3305_0 ; if (
_rtDW -> TimeStampA_oa == ( rtInf ) ) { _rtDW -> TimeStampA_oa = ssGetT ( S )
; lastU = & _rtDW -> LastUAtTimeA_j ; } else if ( _rtDW -> TimeStampB_g == (
rtInf ) ) { _rtDW -> TimeStampB_g = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_gx ; } else if ( _rtDW -> TimeStampA_oa < _rtDW -> TimeStampB_g
) { _rtDW -> TimeStampA_oa = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_j
; } else { _rtDW -> TimeStampB_g = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_gx ; } * lastU = _rtB -> B_185_3316_0 ; if ( _rtDW ->
TimeStampA_h == ( rtInf ) ) { _rtDW -> TimeStampA_h = ssGetT ( S ) ; lastU =
& _rtDW -> LastUAtTimeA_gxl ; } else if ( _rtDW -> TimeStampB_jo == ( rtInf )
) { _rtDW -> TimeStampB_jo = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_pb ; } else if ( _rtDW -> TimeStampA_h < _rtDW -> TimeStampB_jo
) { _rtDW -> TimeStampA_h = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_gxl ; } else { _rtDW -> TimeStampB_jo = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeB_pb ; } * lastU = _rtB -> B_185_3347_0 ; if ( _rtDW ->
TimeStampA_b == ( rtInf ) ) { _rtDW -> TimeStampA_b = ssGetT ( S ) ; lastU =
& _rtDW -> LastUAtTimeA_ea ; } else if ( _rtDW -> TimeStampB_e == ( rtInf ) )
{ _rtDW -> TimeStampB_e = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_a ;
} else if ( _rtDW -> TimeStampA_b < _rtDW -> TimeStampB_e ) { _rtDW ->
TimeStampA_b = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_ea ; } else {
_rtDW -> TimeStampB_e = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_a ; }
* lastU = _rtB -> B_185_3358_0 ; if ( _rtDW -> TimeStampA_ns == ( rtInf ) ) {
_rtDW -> TimeStampA_ns = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_l ; }
else if ( _rtDW -> TimeStampB_ce == ( rtInf ) ) { _rtDW -> TimeStampB_ce =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_eq4 ; } else if ( _rtDW ->
TimeStampA_ns < _rtDW -> TimeStampB_ce ) { _rtDW -> TimeStampA_ns = ssGetT (
S ) ; lastU = & _rtDW -> LastUAtTimeA_l ; } else { _rtDW -> TimeStampB_ce =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_eq4 ; } * lastU = _rtB ->
B_185_3369_0 ; if ( _rtDW -> TimeStampA_kp == ( rtInf ) ) { _rtDW ->
TimeStampA_kp = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_i ; } else if
( _rtDW -> TimeStampB_d == ( rtInf ) ) { _rtDW -> TimeStampB_d = ssGetT ( S )
; lastU = & _rtDW -> LastUAtTimeB_o1 ; } else if ( _rtDW -> TimeStampA_kp <
_rtDW -> TimeStampB_d ) { _rtDW -> TimeStampA_kp = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeA_i ; } else { _rtDW -> TimeStampB_d = ssGetT ( S ) ;
lastU = & _rtDW -> LastUAtTimeB_o1 ; } * lastU = _rtB -> B_185_3401_0 ; if (
_rtDW -> TimeStampA_j5 == ( rtInf ) ) { _rtDW -> TimeStampA_j5 = ssGetT ( S )
; lastU = & _rtDW -> LastUAtTimeA_gw ; } else if ( _rtDW -> TimeStampB_av ==
( rtInf ) ) { _rtDW -> TimeStampB_av = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_ol ; } else if ( _rtDW -> TimeStampA_j5 < _rtDW -> TimeStampB_av
) { _rtDW -> TimeStampA_j5 = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_gw ; } else { _rtDW -> TimeStampB_av = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeB_ol ; } * lastU = _rtB -> B_185_3412_0 ; if ( _rtDW ->
TimeStampA_pp == ( rtInf ) ) { _rtDW -> TimeStampA_pp = ssGetT ( S ) ; lastU
= & _rtDW -> LastUAtTimeA_hk ; } else if ( _rtDW -> TimeStampB_ec == ( rtInf
) ) { _rtDW -> TimeStampB_ec = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_ej ; } else if ( _rtDW -> TimeStampA_pp < _rtDW -> TimeStampB_ec
) { _rtDW -> TimeStampA_pp = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_hk ; } else { _rtDW -> TimeStampB_ec = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeB_ej ; } * lastU = _rtB -> B_185_3423_0 ;
UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID4 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { B_normal_operation_no_faults_T
* _rtB ; P_normal_operation_no_faults_T * _rtP ;
X_normal_operation_no_faults_T * _rtX ; XDot_normal_operation_no_faults_T *
_rtXdot ; XDis_normal_operation_no_faults_T * _rtXdis ;
DW_normal_operation_no_faults_T * _rtDW ; _rtDW = ( (
DW_normal_operation_no_faults_T * ) ssGetRootDWork ( S ) ) ; _rtXdis = ( (
XDis_normal_operation_no_faults_T * ) ssGetContStateDisabled ( S ) ) ;
_rtXdot = ( ( XDot_normal_operation_no_faults_T * ) ssGetdX ( S ) ) ; _rtX =
( ( X_normal_operation_no_faults_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_normal_operation_no_faults_T * ) _ssGetModelBlockIO ( S ) ) ; if ( ( _rtDW
-> Integrator_MODE != 3 ) && ( _rtDW -> Integrator_MODE != 4 ) ) { _rtXdot ->
Integrator_CSTATE = _rtB -> B_185_1386_0 ; _rtXdis -> Integrator_CSTATE =
false ; } else { _rtXdot -> Integrator_CSTATE = 0.0 ; if ( ( _rtDW ->
Integrator_MODE == 3 ) || ( _rtDW -> Integrator_MODE == 4 ) ) { _rtXdis ->
Integrator_CSTATE = true ; } }
normal_operation_no_faults_ACVoltageController_Deriv ( S , & _rtB ->
ACVoltageController , & _rtDW -> ACVoltageController , & _rtXdot ->
ACVoltageController ) ; _rtXdot -> Integrator_CSTATE_k = _rtB -> B_185_1362_0
; normal_operation_no_faults_ZeroSequenceController_Deriv ( S , & _rtB ->
ZeroSequenceController2 , & _rtDW -> ZeroSequenceController2 , & _rtP ->
ZeroSequenceController2 , & _rtX -> ZeroSequenceController2 , & _rtXdot ->
ZeroSequenceController2 ) ;
normal_operation_no_faults_Circulatingcurrentsuppression_Deriv ( S , & _rtB
-> Circulatingcurrentsuppression , & _rtDW -> Circulatingcurrentsuppression ,
& _rtXdot -> Circulatingcurrentsuppression ) ; _rtXdot -> Integrator_CSTATE_j
= _rtB -> B_185_1115_0 ; _rtXdot -> Integrator_CSTATE_n = _rtB ->
B_185_1114_0 ; normal_operation_no_faults_ZeroSequenceController_Deriv ( S ,
& _rtB -> ZeroSequenceController , & _rtDW -> ZeroSequenceController , & _rtP
-> ZeroSequenceController , & _rtX -> ZeroSequenceController , & _rtXdot ->
ZeroSequenceController ) ; if ( _rtDW -> Circulatingcurrentsuppression1_MODE
) { _rtXdot -> Integrator_CSTATE_b5 = _rtB -> B_4_29_0 ; _rtXdot ->
Integrator_CSTATE_d = _rtB -> B_4_34_0 ; } else { { real_T * dx ; int_T i ;
dx = & ( ( ( XDot_normal_operation_no_faults_T * ) ssGetdX ( S ) ) ->
Integrator_CSTATE_b5 ) ; for ( i = 0 ; i < 2 ; i ++ ) { dx [ i ] = 0.0 ; } }
} if ( ( _rtDW -> Integrator_MODE_i != 3 ) && ( _rtDW -> Integrator_MODE_i !=
4 ) ) { _rtXdot -> Integrator_CSTATE_l = _rtB -> B_185_1682_0 ; _rtXdis ->
Integrator_CSTATE_l = false ; } else { _rtXdot -> Integrator_CSTATE_l = 0.0 ;
if ( ( _rtDW -> Integrator_MODE_i == 3 ) || ( _rtDW -> Integrator_MODE_i == 4
) ) { _rtXdis -> Integrator_CSTATE_l = true ; } }
normal_operation_no_faults_ACVoltageController_Deriv ( S , & _rtB ->
ACVoltageController_n , & _rtDW -> ACVoltageController_n , & _rtXdot ->
ACVoltageController_n ) ; _rtXdot -> Integrator_CSTATE_o = _rtB ->
B_185_1658_0 ; normal_operation_no_faults_ZeroSequenceController_Deriv ( S ,
& _rtB -> ZeroSequenceController2_f , & _rtDW -> ZeroSequenceController2_f ,
& _rtP -> ZeroSequenceController2_f , & _rtX -> ZeroSequenceController2_f , &
_rtXdot -> ZeroSequenceController2_f ) ;
normal_operation_no_faults_Circulatingcurrentsuppression_Deriv ( S , & _rtB
-> Circulatingcurrentsuppression_d , & _rtDW ->
Circulatingcurrentsuppression_d , & _rtXdot ->
Circulatingcurrentsuppression_d ) ; if ( ( _rtDW -> Integrator_MODE_h != 3 )
&& ( _rtDW -> Integrator_MODE_h != 4 ) ) { _rtXdot -> Integrator_CSTATE_b =
_rtB -> B_185_1978_0 ; _rtXdis -> Integrator_CSTATE_b = false ; } else {
_rtXdot -> Integrator_CSTATE_b = 0.0 ; if ( ( _rtDW -> Integrator_MODE_h == 3
) || ( _rtDW -> Integrator_MODE_h == 4 ) ) { _rtXdis -> Integrator_CSTATE_b =
true ; } } normal_operation_no_faults_ACVoltageController_Deriv ( S , & _rtB
-> ACVoltageController_l , & _rtDW -> ACVoltageController_l , & _rtXdot ->
ACVoltageController_l ) ; _rtXdot -> Integrator_CSTATE_m = _rtB ->
B_185_1954_0 ; normal_operation_no_faults_ZeroSequenceController_Deriv ( S ,
& _rtB -> ZeroSequenceController2_fb , & _rtDW -> ZeroSequenceController2_fb
, & _rtP -> ZeroSequenceController2_fb , & _rtX -> ZeroSequenceController2_fb
, & _rtXdot -> ZeroSequenceController2_fb ) ;
normal_operation_no_faults_Circulatingcurrentsuppression_Deriv ( S , & _rtB
-> Circulatingcurrentsuppression_k , & _rtDW ->
Circulatingcurrentsuppression_k , & _rtXdot ->
Circulatingcurrentsuppression_k ) ;
normal_operation_no_faults_AutomaticGainControl_Deriv ( S , & _rtB ->
AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl , & _rtXdot -> AutomaticGainControl ) ; if ( ( _rtDW ->
Integrator_MODE_n != 3 ) && ( _rtDW -> Integrator_MODE_n != 4 ) ) { _rtXdot
-> Integrator_CSTATE_ja = _rtB -> B_185_1382_0 ; _rtXdis ->
Integrator_CSTATE_ja = false ; } else { _rtXdot -> Integrator_CSTATE_ja = 0.0
; if ( ( _rtDW -> Integrator_MODE_n == 3 ) || ( _rtDW -> Integrator_MODE_n ==
4 ) ) { _rtXdis -> Integrator_CSTATE_ja = true ; } } { real_T instantDelay ;
instantDelay = _rtB -> B_185_1407_0 ; if ( instantDelay > _rtP -> P_878 ) {
instantDelay = _rtP -> P_878 ; } if ( instantDelay < 0.0 ) { ( (
XDot_normal_operation_no_faults_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE = 0 ; } else { ( (
XDot_normal_operation_no_faults_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE = 1.0 / instantDelay ; } } _rtXdot ->
integrator_CSTATE = _rtB -> B_185_1417_0 [ 1 ] ; _rtXdot ->
TransferFcn_CSTATE = 0.0 ; _rtXdot -> TransferFcn_CSTATE += _rtP -> P_886 *
_rtX -> TransferFcn_CSTATE ; _rtXdot -> TransferFcn_CSTATE += _rtB ->
B_185_1383_0 ; _rtXdot -> Integrator_x1_CSTATE = _rtB -> B_185_1398_0 ;
_rtXdot -> Integrator_x2_CSTATE = _rtB -> B_185_1400_0 ;
normal_operation_no_faults_AutomaticGainControl_Deriv ( S , & _rtB ->
AutomaticGainControl_d , & _rtDW -> AutomaticGainControl_d , & _rtP ->
AutomaticGainControl_d , & _rtXdot -> AutomaticGainControl_d ) ; if ( ( _rtDW
-> Integrator_MODE_j != 3 ) && ( _rtDW -> Integrator_MODE_j != 4 ) ) {
_rtXdot -> Integrator_CSTATE_lt = _rtB -> B_185_1678_0 ; _rtXdis ->
Integrator_CSTATE_lt = false ; } else { _rtXdot -> Integrator_CSTATE_lt = 0.0
; if ( ( _rtDW -> Integrator_MODE_j == 3 ) || ( _rtDW -> Integrator_MODE_j ==
4 ) ) { _rtXdis -> Integrator_CSTATE_lt = true ; } } { real_T instantDelay ;
instantDelay = _rtB -> B_185_1703_0 ; if ( instantDelay > _rtP -> P_983 ) {
instantDelay = _rtP -> P_983 ; } if ( instantDelay < 0.0 ) { ( (
XDot_normal_operation_no_faults_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE_i = 0 ; } else { ( (
XDot_normal_operation_no_faults_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE_i = 1.0 / instantDelay ; } } _rtXdot ->
integrator_CSTATE_m = _rtB -> B_185_1713_0 [ 1 ] ; _rtXdot ->
TransferFcn_CSTATE_a = 0.0 ; _rtXdot -> TransferFcn_CSTATE_a += _rtP -> P_991
* _rtX -> TransferFcn_CSTATE_a ; _rtXdot -> TransferFcn_CSTATE_a += _rtB ->
B_185_1679_0 ; _rtXdot -> Integrator_x1_CSTATE_n = _rtB -> B_185_1694_0 ;
_rtXdot -> Integrator_x2_CSTATE_c = _rtB -> B_185_1696_0 ;
normal_operation_no_faults_AutomaticGainControl_Deriv ( S , & _rtB ->
AutomaticGainControl_a , & _rtDW -> AutomaticGainControl_a , & _rtP ->
AutomaticGainControl_a , & _rtXdot -> AutomaticGainControl_a ) ; if ( ( _rtDW
-> Integrator_MODE_m != 3 ) && ( _rtDW -> Integrator_MODE_m != 4 ) ) {
_rtXdot -> Integrator_CSTATE_bf = _rtB -> B_185_1974_0 ; _rtXdis ->
Integrator_CSTATE_bf = false ; } else { _rtXdot -> Integrator_CSTATE_bf = 0.0
; if ( ( _rtDW -> Integrator_MODE_m == 3 ) || ( _rtDW -> Integrator_MODE_m ==
4 ) ) { _rtXdis -> Integrator_CSTATE_bf = true ; } } { real_T instantDelay ;
instantDelay = _rtB -> B_185_1999_0 ; if ( instantDelay > _rtP -> P_1088 ) {
instantDelay = _rtP -> P_1088 ; } if ( instantDelay < 0.0 ) { ( (
XDot_normal_operation_no_faults_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE_o = 0 ; } else { ( (
XDot_normal_operation_no_faults_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE_o = 1.0 / instantDelay ; } } _rtXdot ->
integrator_CSTATE_b = _rtB -> B_185_2009_0 [ 1 ] ; _rtXdot ->
TransferFcn_CSTATE_l = 0.0 ; _rtXdot -> TransferFcn_CSTATE_l += _rtP ->
P_1096 * _rtX -> TransferFcn_CSTATE_l ; _rtXdot -> TransferFcn_CSTATE_l +=
_rtB -> B_185_1975_0 ; _rtXdot -> Integrator_x1_CSTATE_o = _rtB ->
B_185_1990_0 ; _rtXdot -> Integrator_x2_CSTATE_e = _rtB -> B_185_1992_0 ;
_rtXdot -> integrator_CSTATE_b5 [ 0 ] = _rtB -> B_185_2331_0 [ 0 ] ; _rtXdot
-> integrator_CSTATE_l [ 0 ] = _rtB -> B_185_2333_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_n [ 0 ] = _rtB -> B_185_2339_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_i [ 0 ] = _rtB -> B_185_2341_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_k [ 0 ] = _rtB -> B_185_2389_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_lw [ 0 ] = _rtB -> B_185_2391_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_d [ 0 ] = _rtB -> B_185_2397_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_mn [ 0 ] = _rtB -> B_185_2399_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_p [ 0 ] = _rtB -> B_185_2447_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_a [ 0 ] = _rtB -> B_185_2449_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_j [ 0 ] = _rtB -> B_185_2455_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_jl [ 0 ] = _rtB -> B_185_2457_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_nm [ 0 ] = _rtB -> B_185_2509_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_c [ 0 ] = _rtB -> B_185_2511_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_e [ 0 ] = _rtB -> B_185_2517_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_f [ 0 ] = _rtB -> B_185_2519_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_b5 [ 1 ] = _rtB -> B_185_2331_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_l [ 1 ] = _rtB -> B_185_2333_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_n [ 1 ] = _rtB -> B_185_2339_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_i [ 1 ] = _rtB -> B_185_2341_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_k [ 1 ] = _rtB -> B_185_2389_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_lw [ 1 ] = _rtB -> B_185_2391_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_d [ 1 ] = _rtB -> B_185_2397_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_mn [ 1 ] = _rtB -> B_185_2399_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_p [ 1 ] = _rtB -> B_185_2447_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_a [ 1 ] = _rtB -> B_185_2449_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_j [ 1 ] = _rtB -> B_185_2455_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_jl [ 1 ] = _rtB -> B_185_2457_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_nm [ 1 ] = _rtB -> B_185_2509_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_c [ 1 ] = _rtB -> B_185_2511_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_e [ 1 ] = _rtB -> B_185_2517_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_f [ 1 ] = _rtB -> B_185_2519_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_b5 [ 2 ] = _rtB -> B_185_2331_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_l [ 2 ] = _rtB -> B_185_2333_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_n [ 2 ] = _rtB -> B_185_2339_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_i [ 2 ] = _rtB -> B_185_2341_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_k [ 2 ] = _rtB -> B_185_2389_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_lw [ 2 ] = _rtB -> B_185_2391_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_d [ 2 ] = _rtB -> B_185_2397_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_mn [ 2 ] = _rtB -> B_185_2399_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_p [ 2 ] = _rtB -> B_185_2447_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_a [ 2 ] = _rtB -> B_185_2449_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_j [ 2 ] = _rtB -> B_185_2455_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_jl [ 2 ] = _rtB -> B_185_2457_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_nm [ 2 ] = _rtB -> B_185_2509_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_c [ 2 ] = _rtB -> B_185_2511_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_e [ 2 ] = _rtB -> B_185_2517_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_f [ 2 ] = _rtB -> B_185_2519_0 [ 2 ] ;
normal_operation_no_faults_TrueRMS_Deriv ( S , & _rtB -> TrueRMS , & _rtDW ->
TrueRMS , & _rtXdot -> TrueRMS ) ; normal_operation_no_faults_RMS_Deriv ( S ,
& _rtB -> RMS , & _rtDW -> RMS , & _rtXdot -> RMS ) ; _rtXdot ->
integrator_CSTATE_kd [ 0 ] = _rtB -> B_185_2623_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_c1 [ 0 ] = _rtB -> B_185_2625_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_js [ 0 ] = _rtB -> B_185_2627_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_po [ 0 ] = _rtB -> B_185_2629_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_kd [ 1 ] = _rtB -> B_185_2623_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_c1 [ 1 ] = _rtB -> B_185_2625_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_js [ 1 ] = _rtB -> B_185_2627_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_po [ 1 ] = _rtB -> B_185_2629_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_kd [ 2 ] = _rtB -> B_185_2623_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_c1 [ 2 ] = _rtB -> B_185_2625_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_js [ 2 ] = _rtB -> B_185_2627_0 [ 2 ] ; _rtXdot ->
integrator_CSTATE_po [ 2 ] = _rtB -> B_185_2629_0 [ 2 ] ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { boolean_T anyStateSaturated
; B_normal_operation_no_faults_T * _rtB ; P_normal_operation_no_faults_T *
_rtP ; X_normal_operation_no_faults_T * _rtX ;
ZCV_normal_operation_no_faults_T * _rtZCSV ; DW_normal_operation_no_faults_T
* _rtDW ; _rtDW = ( ( DW_normal_operation_no_faults_T * ) ssGetRootDWork ( S
) ) ; _rtZCSV = ( ( ZCV_normal_operation_no_faults_T * )
ssGetSolverZcSignalVector ( S ) ) ; _rtX = ( ( X_normal_operation_no_faults_T
* ) ssGetContStates ( S ) ) ; _rtP = ( ( P_normal_operation_no_faults_T * )
ssGetModelRtp ( S ) ) ; _rtB = ( ( B_normal_operation_no_faults_T * )
_ssGetModelBlockIO ( S ) ) ; _rtZCSV -> RelationalOperator_RelopInput_ZC =
_rtX -> Integrator_CSTATE - _rtB -> B_185_8_0 ; if ( ( _rtDW ->
Integrator_MODE == 1 ) && ( _rtX -> Integrator_CSTATE >= _rtP -> P_118 ) ) {
_rtZCSV -> Integrator_IntgUpLimit_ZC = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC = _rtX -> Integrator_CSTATE - _rtP -> P_118 ; } if
( ( _rtDW -> Integrator_MODE == 2 ) && ( _rtX -> Integrator_CSTATE <= _rtP ->
P_119 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC = 0.0 ; } else { _rtZCSV ->
Integrator_IntgLoLimit_ZC = _rtX -> Integrator_CSTATE - _rtP -> P_119 ; }
anyStateSaturated = false ; if ( ( _rtDW -> Integrator_MODE == 3 ) || ( _rtDW
-> Integrator_MODE == 4 ) ) { anyStateSaturated = true ; } if (
anyStateSaturated ) { _rtZCSV -> Integrator_LeaveSaturate_ZC = _rtB ->
B_185_1386_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC = 0.0 ; }
_rtZCSV -> Step_StepTime_ZC = ssGetT ( S ) - _rtP -> P_129 ; _rtZCSV ->
DCregulatorTURNONidreferenceforrectifier_StepTime_ZC = ssGetT ( S ) - _rtP ->
P_134 ; _rtZCSV ->
GridVoltagestabilizerTURNONiqreferenceforrectifier_StepTime_ZC = ssGetT ( S )
- _rtP -> P_172 ; normal_operation_no_faults_ACVoltageController_ZC ( S , &
_rtB -> ACVoltageController , & _rtDW -> ACVoltageController , & _rtP ->
ACVoltageController , & _rtZCSV -> ACVoltageController ) ; _rtZCSV ->
Saturation_UprLim_ZC [ 0 ] = _rtB -> B_185_99_0 - _rtP -> P_182 ; _rtZCSV ->
Saturation_UprLim_ZC [ 1 ] = _rtB -> B_185_108_0 - _rtP -> P_182 ; _rtZCSV ->
Saturation_LwrLim_ZC [ 0 ] = _rtB -> B_185_99_0 - _rtP -> P_183 ; _rtZCSV ->
Saturation_LwrLim_ZC [ 1 ] = _rtB -> B_185_108_0 - _rtP -> P_183 ; _rtZCSV ->
EnableCirculatingCurrentSuppresion_StepTime_ZC = ssGetT ( S ) - _rtP -> P_190
; normal_operation_no_faults_Circulatingcurrentsuppression_ZC ( S , & _rtB ->
Circulatingcurrentsuppression , & _rtDW -> Circulatingcurrentsuppression , &
_rtP -> Circulatingcurrentsuppression , & _rtZCSV ->
Circulatingcurrentsuppression ) ; _rtZCSV ->
EnableACRMSregulatorTURNONidreferenceforinverter_StepTime_ZC = ssGetT ( S ) -
_rtP -> P_264 ; _rtZCSV ->
EnableReactivepowersetpointregulationiqreferenceforinverter_StepTime_ZC =
ssGetT ( S ) - _rtP -> P_289 ; _rtZCSV -> Saturation2_UprLim_ZC [ 0 ] = _rtB
-> B_185_277_0 - _rtP -> P_298 ; _rtZCSV -> Saturation2_UprLim_ZC [ 1 ] =
_rtB -> B_185_286_0 - _rtP -> P_298 ; _rtZCSV -> Saturation2_LwrLim_ZC [ 0 ]
= _rtB -> B_185_277_0 - _rtP -> P_299 ; _rtZCSV -> Saturation2_LwrLim_ZC [ 1
] = _rtB -> B_185_286_0 - _rtP -> P_299 ; if ( _rtDW ->
Circulatingcurrentsuppression1_MODE ) { _rtZCSV -> Saturation_UprLim_ZC_k [ 0
] = _rtB -> B_4_55_0 [ 0 ] - _rtP -> P_31 ; _rtZCSV ->
Saturation_LwrLim_ZC_pm [ 0 ] = _rtB -> B_4_55_0 [ 0 ] - _rtP -> P_32 ;
_rtZCSV -> Saturation_UprLim_ZC_k [ 1 ] = _rtB -> B_4_55_0 [ 1 ] - _rtP ->
P_31 ; _rtZCSV -> Saturation_LwrLim_ZC_pm [ 1 ] = _rtB -> B_4_55_0 [ 1 ] -
_rtP -> P_32 ; _rtZCSV -> Saturation_UprLim_ZC_k [ 2 ] = _rtB -> B_4_55_0 [ 2
] - _rtP -> P_31 ; _rtZCSV -> Saturation_LwrLim_ZC_pm [ 2 ] = _rtB ->
B_4_55_0 [ 2 ] - _rtP -> P_32 ; } else { { real_T * zcsv = & ( ( (
ZCV_normal_operation_no_faults_T * ) ssGetSolverZcSignalVector ( S ) ) ->
Saturation_UprLim_ZC_k [ 0 ] ) ; int_T i ; for ( i = 0 ; i < 6 ; i ++ ) {
zcsv [ i ] = 0.0 ; } } } _rtZCSV -> RelationalOperator_RelopInput_ZC_g = _rtX
-> Integrator_CSTATE_l - _rtB -> B_185_395_0 ; if ( ( _rtDW ->
Integrator_MODE_i == 1 ) && ( _rtX -> Integrator_CSTATE_l >= _rtP -> P_370 )
) { _rtZCSV -> Integrator_IntgUpLimit_ZC_h = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC_h = _rtX -> Integrator_CSTATE_l - _rtP -> P_370 ; }
if ( ( _rtDW -> Integrator_MODE_i == 2 ) && ( _rtX -> Integrator_CSTATE_l <=
_rtP -> P_371 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC_p = 0.0 ; } else {
_rtZCSV -> Integrator_IntgLoLimit_ZC_p = _rtX -> Integrator_CSTATE_l - _rtP
-> P_371 ; } anyStateSaturated = false ; if ( ( _rtDW -> Integrator_MODE_i ==
3 ) || ( _rtDW -> Integrator_MODE_i == 4 ) ) { anyStateSaturated = true ; }
if ( anyStateSaturated ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_m = _rtB ->
B_185_1682_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_m = 0.0 ; }
_rtZCSV -> Step_StepTime_ZC_o = ssGetT ( S ) - _rtP -> P_381 ;
normal_operation_no_faults_ACVoltageController_ZC ( S , & _rtB ->
ACVoltageController_n , & _rtDW -> ACVoltageController_n , & _rtP ->
ACVoltageController_n , & _rtZCSV -> ACVoltageController_n ) ; _rtZCSV ->
Saturation_UprLim_ZC_d [ 0 ] = _rtB -> B_185_484_0 - _rtP -> P_428 ; _rtZCSV
-> Saturation_UprLim_ZC_d [ 1 ] = _rtB -> B_185_493_0 - _rtP -> P_428 ;
_rtZCSV -> Saturation_LwrLim_ZC_h [ 0 ] = _rtB -> B_185_484_0 - _rtP -> P_429
; _rtZCSV -> Saturation_LwrLim_ZC_h [ 1 ] = _rtB -> B_185_493_0 - _rtP ->
P_429 ; normal_operation_no_faults_Circulatingcurrentsuppression_ZC ( S , &
_rtB -> Circulatingcurrentsuppression_d , & _rtDW ->
Circulatingcurrentsuppression_d , & _rtP -> Circulatingcurrentsuppression_d ,
& _rtZCSV -> Circulatingcurrentsuppression_d ) ; _rtZCSV ->
RelationalOperator_RelopInput_ZC_o = _rtX -> Integrator_CSTATE_b - _rtB ->
B_185_598_0 ; if ( ( _rtDW -> Integrator_MODE_h == 1 ) && ( _rtX ->
Integrator_CSTATE_b >= _rtP -> P_498 ) ) { _rtZCSV ->
Integrator_IntgUpLimit_ZC_l = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC_l = _rtX -> Integrator_CSTATE_b - _rtP -> P_498 ; }
if ( ( _rtDW -> Integrator_MODE_h == 2 ) && ( _rtX -> Integrator_CSTATE_b <=
_rtP -> P_499 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC_g = 0.0 ; } else {
_rtZCSV -> Integrator_IntgLoLimit_ZC_g = _rtX -> Integrator_CSTATE_b - _rtP
-> P_499 ; } anyStateSaturated = false ; if ( ( _rtDW -> Integrator_MODE_h ==
3 ) || ( _rtDW -> Integrator_MODE_h == 4 ) ) { anyStateSaturated = true ; }
if ( anyStateSaturated ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_j = _rtB ->
B_185_1978_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_j = 0.0 ; }
_rtZCSV -> Step_StepTime_ZC_k = ssGetT ( S ) - _rtP -> P_509 ;
normal_operation_no_faults_ACVoltageController_ZC ( S , & _rtB ->
ACVoltageController_l , & _rtDW -> ACVoltageController_l , & _rtP ->
ACVoltageController_l , & _rtZCSV -> ACVoltageController_l ) ; _rtZCSV ->
Saturation_UprLim_ZC_p [ 0 ] = _rtB -> B_185_687_0 - _rtP -> P_556 ; _rtZCSV
-> Saturation_UprLim_ZC_p [ 1 ] = _rtB -> B_185_696_0 - _rtP -> P_556 ;
_rtZCSV -> Saturation_LwrLim_ZC_p [ 0 ] = _rtB -> B_185_687_0 - _rtP -> P_557
; _rtZCSV -> Saturation_LwrLim_ZC_p [ 1 ] = _rtB -> B_185_696_0 - _rtP ->
P_557 ; normal_operation_no_faults_Circulatingcurrentsuppression_ZC ( S , &
_rtB -> Circulatingcurrentsuppression_k , & _rtDW ->
Circulatingcurrentsuppression_k , & _rtP -> Circulatingcurrentsuppression_k ,
& _rtZCSV -> Circulatingcurrentsuppression_k ) ; _rtZCSV ->
HitCrossing_HitNoOutput_ZC = _rtB -> B_185_939_0 - _rtP -> P_706 ; _rtZCSV ->
HitCrossing_HitNoOutput_ZC_c = _rtB -> B_185_950_0 - _rtP -> P_713 ; _rtZCSV
-> HitCrossing_HitNoOutput_ZC_e = _rtB -> B_185_961_0 - _rtP -> P_720 ;
_rtZCSV -> HitCrossing_HitNoOutput_ZC_cl = _rtB -> B_185_985_0 - _rtP ->
P_733 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_ci = _rtB -> B_185_996_0 - _rtP
-> P_740 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_p = _rtB -> B_185_1007_0 -
_rtP -> P_747 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_k = _rtB ->
B_185_1031_0 - _rtP -> P_760 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_kj =
_rtB -> B_185_1042_0 - _rtP -> P_767 ; _rtZCSV ->
HitCrossing_HitNoOutput_ZC_f = _rtB -> B_185_1053_0 - _rtP -> P_774 ; _rtZCSV
-> HitCrossing_HitNoOutput_ZC_n = _rtB -> B_185_1077_0 - _rtP -> P_787 ;
_rtZCSV -> HitCrossing_HitNoOutput_ZC_cj = _rtB -> B_185_1088_0 - _rtP ->
P_794 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_eb = _rtB -> B_185_1099_0 -
_rtP -> P_801 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_ec = _rtB -> B_185_12_0
- _rtP -> P_868 ; normal_operation_no_faults_AutomaticGainControl_ZC ( S , &
_rtB -> AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl , & _rtZCSV -> AutomaticGainControl ) ; if ( ( _rtDW ->
Integrator_MODE_n == 1 ) && ( _rtX -> Integrator_CSTATE_ja >= _rtP -> P_876 )
) { _rtZCSV -> Integrator_IntgUpLimit_ZC_g = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC_g = _rtX -> Integrator_CSTATE_ja - _rtP -> P_876 ;
} if ( ( _rtDW -> Integrator_MODE_n == 2 ) && ( _rtX -> Integrator_CSTATE_ja
<= _rtP -> P_877 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC_h = 0.0 ; } else
{ _rtZCSV -> Integrator_IntgLoLimit_ZC_h = _rtX -> Integrator_CSTATE_ja -
_rtP -> P_877 ; } anyStateSaturated = false ; if ( ( _rtDW ->
Integrator_MODE_n == 3 ) || ( _rtDW -> Integrator_MODE_n == 4 ) ) {
anyStateSaturated = true ; } if ( anyStateSaturated ) { _rtZCSV ->
Integrator_LeaveSaturate_ZC_mr = _rtB -> B_185_1382_0 ; } else { _rtZCSV ->
Integrator_LeaveSaturate_ZC_mr = 0.0 ; } _rtZCSV ->
RelationalOperator_RelopInput_ZC_oa = _rtB -> B_185_1374_0 - _rtB ->
B_185_1375_0 ; _rtZCSV -> Saturation2_UprLim_ZC_n = _rtB -> B_185_1385_0 -
_rtP -> P_889 ; _rtZCSV -> Saturation2_LwrLim_ZC_l = _rtB -> B_185_1385_0 -
_rtP -> P_890 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_ebe = _rtB ->
B_185_399_0 - _rtP -> P_973 ;
normal_operation_no_faults_AutomaticGainControl_ZC ( S , & _rtB ->
AutomaticGainControl_d , & _rtDW -> AutomaticGainControl_d , & _rtP ->
AutomaticGainControl_d , & _rtZCSV -> AutomaticGainControl_d ) ; if ( ( _rtDW
-> Integrator_MODE_j == 1 ) && ( _rtX -> Integrator_CSTATE_lt >= _rtP ->
P_981 ) ) { _rtZCSV -> Integrator_IntgUpLimit_ZC_m = 0.0 ; } else { _rtZCSV
-> Integrator_IntgUpLimit_ZC_m = _rtX -> Integrator_CSTATE_lt - _rtP -> P_981
; } if ( ( _rtDW -> Integrator_MODE_j == 2 ) && ( _rtX ->
Integrator_CSTATE_lt <= _rtP -> P_982 ) ) { _rtZCSV ->
Integrator_IntgLoLimit_ZC_k = 0.0 ; } else { _rtZCSV ->
Integrator_IntgLoLimit_ZC_k = _rtX -> Integrator_CSTATE_lt - _rtP -> P_982 ;
} anyStateSaturated = false ; if ( ( _rtDW -> Integrator_MODE_j == 3 ) || (
_rtDW -> Integrator_MODE_j == 4 ) ) { anyStateSaturated = true ; } if (
anyStateSaturated ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_k = _rtB ->
B_185_1678_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_k = 0.0 ; }
_rtZCSV -> RelationalOperator_RelopInput_ZC_c = _rtB -> B_185_1670_0 - _rtB
-> B_185_1671_0 ; _rtZCSV -> Saturation2_UprLim_ZC_p = _rtB -> B_185_1681_0 -
_rtP -> P_994 ; _rtZCSV -> Saturation2_LwrLim_ZC_i = _rtB -> B_185_1681_0 -
_rtP -> P_995 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_h = _rtB -> B_185_602_0
- _rtP -> P_1078 ; normal_operation_no_faults_AutomaticGainControl_ZC ( S , &
_rtB -> AutomaticGainControl_a , & _rtDW -> AutomaticGainControl_a , & _rtP
-> AutomaticGainControl_a , & _rtZCSV -> AutomaticGainControl_a ) ; if ( (
_rtDW -> Integrator_MODE_m == 1 ) && ( _rtX -> Integrator_CSTATE_bf >= _rtP
-> P_1086 ) ) { _rtZCSV -> Integrator_IntgUpLimit_ZC_f = 0.0 ; } else {
_rtZCSV -> Integrator_IntgUpLimit_ZC_f = _rtX -> Integrator_CSTATE_bf - _rtP
-> P_1086 ; } if ( ( _rtDW -> Integrator_MODE_m == 2 ) && ( _rtX ->
Integrator_CSTATE_bf <= _rtP -> P_1087 ) ) { _rtZCSV ->
Integrator_IntgLoLimit_ZC_b = 0.0 ; } else { _rtZCSV ->
Integrator_IntgLoLimit_ZC_b = _rtX -> Integrator_CSTATE_bf - _rtP -> P_1087 ;
} anyStateSaturated = false ; if ( ( _rtDW -> Integrator_MODE_m == 3 ) || (
_rtDW -> Integrator_MODE_m == 4 ) ) { anyStateSaturated = true ; } if (
anyStateSaturated ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_f = _rtB ->
B_185_1974_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_f = 0.0 ; }
_rtZCSV -> RelationalOperator_RelopInput_ZC_ob = _rtB -> B_185_1966_0 - _rtB
-> B_185_1967_0 ; _rtZCSV -> Saturation2_UprLim_ZC_h = _rtB -> B_185_1977_0 -
_rtP -> P_1099 ; _rtZCSV -> Saturation2_LwrLim_ZC_m = _rtB -> B_185_1977_0 -
_rtP -> P_1100 ; normal_operation_no_faults_TrueRMS_ZC ( S , & _rtB ->
TrueRMS , & _rtDW -> TrueRMS , & _rtP -> TrueRMS , & _rtZCSV -> TrueRMS ) ;
_rtZCSV -> EnableHarmonicFilters_StepTime_ZC = ssGetT ( S ) - _rtP -> P_1455
; _rtZCSV ->
EnergizeConverterswithstartupcurrentBypassesstartupresistors_StepTime_ZC =
ssGetT ( S ) - _rtP -> P_1458 ; _rtZCSV ->
EnergizegridHarmonicFilter_StepTime_ZC = ssGetT ( S ) - _rtP -> P_1461 ;
_rtZCSV -> HitCrossing_HitNoOutput_ZC_j = _rtB -> B_185_3191_0 - _rtP ->
P_1475 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_ju = _rtB -> B_185_3202_0 -
_rtP -> P_1482 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_a = _rtB ->
B_185_3213_0 - _rtP -> P_1489 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_jt =
_rtB -> B_185_3243_0 - _rtP -> P_1501 ; _rtZCSV ->
HitCrossing_HitNoOutput_ZC_p2 = _rtB -> B_185_3254_0 - _rtP -> P_1508 ;
_rtZCSV -> HitCrossing_HitNoOutput_ZC_k2 = _rtB -> B_185_3265_0 - _rtP ->
P_1515 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_i = _rtB -> B_185_3296_0 -
_rtP -> P_1527 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_pu = _rtB ->
B_185_3307_0 - _rtP -> P_1534 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_nn =
_rtB -> B_185_3318_0 - _rtP -> P_1541 ; _rtZCSV ->
HitCrossing_HitNoOutput_ZC_eh = _rtB -> B_185_3349_0 - _rtP -> P_1553 ;
_rtZCSV -> HitCrossing_HitNoOutput_ZC_ca = _rtB -> B_185_3360_0 - _rtP ->
P_1560 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_cih = _rtB -> B_185_3371_0 -
_rtP -> P_1567 ; _rtZCSV -> Switchonloadnetwork_StepTime_ZC = ssGetT ( S ) -
_rtP -> P_1569 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_hn = _rtB ->
B_185_3403_0 - _rtP -> P_1582 ; _rtZCSV -> HitCrossing_HitNoOutput_ZC_ft =
_rtB -> B_185_3414_0 - _rtP -> P_1589 ; _rtZCSV ->
HitCrossing_HitNoOutput_ZC_i4 = _rtB -> B_185_3425_0 - _rtP -> P_1596 ; }
static void mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 ,
1636544948U ) ; ssSetChecksumVal ( S , 1 , 1522054363U ) ; ssSetChecksumVal (
S , 2 , 4018321131U ) ; ssSetChecksumVal ( S , 3 , 1630921518U ) ; { mxArray
* slVerStructMat = NULL ; mxArray * slStrMat = mxCreateString ( "simulink" )
; char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat ,
1 , & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat =
mxGetField ( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == NULL ) {
status = 1 ; } else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; }
} mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "9.1" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != sizeof ( DW_normal_operation_no_faults_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( B_normal_operation_no_faults_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
P_normal_operation_no_faults_T ) ) { static char msg [ 256 ] ; sprintf ( msg
, "Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetModelRtp ( S , ( real_T *
) & normal_operation_no_faults_rtDefaultP ) ; _ssSetConstBlockIO ( S , &
normal_operation_no_faults_rtInvariant ) ; rt_InitInfAndNaN ( sizeof ( real_T
) ) ; ( ( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) -> P_118 =
rtInf ; ( ( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) -> P_119
= rtMinusInf ; ( ( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) )
-> P_370 = rtInf ; ( ( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S )
) -> P_371 = rtMinusInf ; ( ( P_normal_operation_no_faults_T * )
ssGetModelRtp ( S ) ) -> P_498 = rtInf ; ( ( P_normal_operation_no_faults_T *
) ssGetModelRtp ( S ) ) -> P_499 = rtMinusInf ; ( (
P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) -> P_876 = rtInf ; (
( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) -> P_889 = rtInf ;
( ( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) -> P_981 = rtInf
; ( ( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) -> P_994 =
rtInf ; ( ( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) ->
P_1086 = rtInf ; ( ( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) )
-> P_1099 = rtInf ; ( ( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S
) ) -> TrueRMS . P_7 = rtInf ; ( ( P_normal_operation_no_faults_T * )
ssGetModelRtp ( S ) ) -> AutomaticGainControl_a . P_11 = rtInf ; ( (
P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) ->
ACVoltageController_l . TrueRMS . P_7 = rtInf ; ( (
P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) ->
AutomaticGainControl_d . P_11 = rtInf ; ( ( P_normal_operation_no_faults_T *
) ssGetModelRtp ( S ) ) -> ACVoltageController_n . TrueRMS . P_7 = rtInf ; (
( P_normal_operation_no_faults_T * ) ssGetModelRtp ( S ) ) ->
AutomaticGainControl . P_11 = rtInf ; ( ( P_normal_operation_no_faults_T * )
ssGetModelRtp ( S ) ) -> ACVoltageController . TrueRMS . P_7 = rtInf ; }
static void mdlInitializeSampleTimes ( SimStruct * S ) { slAccRegPrmChangeFcn
( S , mdlOutputsTID4 ) ; } static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
