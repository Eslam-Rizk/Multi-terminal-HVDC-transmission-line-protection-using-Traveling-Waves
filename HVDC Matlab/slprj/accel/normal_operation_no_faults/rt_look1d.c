#include "__cf_normal_operation_no_faults.h"
#include "rt_look1d.h"
real_T rt_Lookup ( const real_T * x , int_T xlen , real_T u , const real_T *
y ) { int_T idx = rt_GetLookupIndex ( x , xlen , u ) ; real_T num = y [ idx +
1 ] - y [ idx ] ; real_T den = x [ idx + 1 ] - x [ idx ] ; real_T m = num /
den ; return ( y [ idx ] + ( m * ( u - x [ idx ] ) ) ) ; }
