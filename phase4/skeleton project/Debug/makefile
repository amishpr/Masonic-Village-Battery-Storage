################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /opt/ccstudio/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Example_2806xLEDBlink.obj" \
"./F2806x_Adc.obj" \
"./F2806x_CSMPasswords.obj" \
"./F2806x_CodeStartBranch.obj" \
"./F2806x_Comp.obj" \
"./F2806x_CpuTimers.obj" \
"./F2806x_DBGIER.obj" \
"./F2806x_DefaultIsr.obj" \
"./F2806x_DisInt.obj" \
"./F2806x_Dma.obj" \
"./F2806x_ECan.obj" \
"./F2806x_ECap.obj" \
"./F2806x_EPwm.obj" \
"./F2806x_EQep.obj" \
"./F2806x_GlobalVariableDefs.obj" \
"./F2806x_Gpio.obj" \
"./F2806x_HRCap.obj" \
"./F2806x_I2C.obj" \
"./F2806x_Mcbsp.obj" \
"./F2806x_OscComp.obj" \
"./F2806x_PieCtrl.obj" \
"./F2806x_PieVect.obj" \
"./F2806x_Sci.obj" \
"./F2806x_Spi.obj" \
"./F2806x_SysCtrl.obj" \
"./F2806x_TempSensorConv.obj" \
"./F2806x_usDelay.obj" \
"../F28069M.cmd" \
"../F2806x_Headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
cpu1.out \

EXE_OUTPUTS__QUOTED += \
"cpu1.out" \

BIN_OUTPUTS += \
cpu1.hex \

BIN_OUTPUTS__QUOTED += \
"cpu1.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "cpu1.out"

# Tool invocations
cpu1.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"/opt/ccstudio/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 --define=CPU1 --define=_LAUNCHXL_F28069M -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"cpu1.map" --stack_size=0x300 --warn_sections -i"/opt/ccstudio/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/lib" -i"/opt/ccstudio/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="cpu1_linkInfo.xml" --rom_model -o "cpu1.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

cpu1.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"/opt/ccstudio/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/hex2000"  -o "cpu1.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Example_2806xLEDBlink.obj" "F2806x_Adc.obj" "F2806x_CSMPasswords.obj" "F2806x_CodeStartBranch.obj" "F2806x_Comp.obj" "F2806x_CpuTimers.obj" "F2806x_DBGIER.obj" "F2806x_DefaultIsr.obj" "F2806x_DisInt.obj" "F2806x_Dma.obj" "F2806x_ECan.obj" "F2806x_ECap.obj" "F2806x_EPwm.obj" "F2806x_EQep.obj" "F2806x_GlobalVariableDefs.obj" "F2806x_Gpio.obj" "F2806x_HRCap.obj" "F2806x_I2C.obj" "F2806x_Mcbsp.obj" "F2806x_OscComp.obj" "F2806x_PieCtrl.obj" "F2806x_PieVect.obj" "F2806x_Sci.obj" "F2806x_Spi.obj" "F2806x_SysCtrl.obj" "F2806x_TempSensorConv.obj" "F2806x_usDelay.obj" 
	-$(RM) "Example_2806xLEDBlink.d" "F2806x_Adc.d" "F2806x_Comp.d" "F2806x_CpuTimers.d" "F2806x_DefaultIsr.d" "F2806x_Dma.d" "F2806x_ECan.d" "F2806x_ECap.d" "F2806x_EPwm.d" "F2806x_EQep.d" "F2806x_GlobalVariableDefs.d" "F2806x_Gpio.d" "F2806x_HRCap.d" "F2806x_I2C.d" "F2806x_Mcbsp.d" "F2806x_OscComp.d" "F2806x_PieCtrl.d" "F2806x_PieVect.d" "F2806x_Sci.d" "F2806x_Spi.d" "F2806x_SysCtrl.d" "F2806x_TempSensorConv.d" 
	-$(RM) "F2806x_CSMPasswords.d" "F2806x_CodeStartBranch.d" "F2806x_DBGIER.d" "F2806x_DisInt.d" "F2806x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

