################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/aleze/workspace_v13/Code_ESD_v1" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00" --include_path="C:/Users/aleze/workspace_v13/Code_ESD_v1/device" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28003x/driverlib" --include_path="C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=DEBUG --define=_FLASH --define=_LAUNCHXL_F280039C --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/aleze/workspace_v13/Code_ESD_v1/CPU1_LAUNCHXL_FLASH/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-2006585825: ../wtachdog_example.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1250/ccs/utils/sysconfig_1.18.0/sysconfig_cli.bat" -s "C:/ti/c2000/C2000Ware_5_01_00_00/.metadata/sdk.json" -d "F28003x" --script "C:/Users/aleze/workspace_v13/Code_ESD_v1/wtachdog_example.syscfg" -o "syscfg" --package 100PZ --part F28003x_100PZ --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-2006585825 ../wtachdog_example.syscfg
syscfg/board.h: build-2006585825
syscfg/board.cmd.genlibs: build-2006585825
syscfg/board.opt: build-2006585825
syscfg/pinmux.csv: build-2006585825
syscfg/adc.dot: build-2006585825
syscfg/c2000ware_libraries.cmd.genlibs: build-2006585825
syscfg/c2000ware_libraries.opt: build-2006585825
syscfg/c2000ware_libraries.c: build-2006585825
syscfg/c2000ware_libraries.h: build-2006585825
syscfg/clocktree.h: build-2006585825
syscfg/: build-2006585825

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/aleze/workspace_v13/Code_ESD_v1" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00" --include_path="C:/Users/aleze/workspace_v13/Code_ESD_v1/device" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28003x/driverlib" --include_path="C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=DEBUG --define=_FLASH --define=_LAUNCHXL_F280039C --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/aleze/workspace_v13/Code_ESD_v1/CPU1_LAUNCHXL_FLASH/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


