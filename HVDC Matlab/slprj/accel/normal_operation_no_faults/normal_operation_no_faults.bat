@echo off
set MATLAB=D:\eslam\programs\MATLAB2018\R2018a
"%MATLAB%\bin\win64\gmake" -f normal_operation_no_faults.mk  COMBINE_OUTPUT_UPDATE_FCNS=0 INCLUDE_MDL_TERMINATE_FCN=1 MULTI_INSTANCE_CODE=0 ISPROTECTINGMODEL=NOTPROTECTING
