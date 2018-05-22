################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../origen.cpp 

OBJS += \
./origen.o 

CPP_DEPS += \
./origen.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_TM_CDIR='"/home/nxa26780/devices/origen_std_lib/v93k/src/origen"' -I/opt/hp93000/soc/prod_com/include -I"/home/nxa26780/devices/origen_std_lib/v93k/src/origen" -I/opt/hp93000/soc/prod_com/include/MAPI -I/opt/hp93000/soc/testmethod/include -I/opt/hp93000/soc/com/include -I/opt/hp93000/soc/pws/lib -I/opt/hp93000/soc/segments/tml/include -I/opt/hp93000/soc/segments/tml/include/ext_include -I/opt/hp93000/soc/prod_com/rdi/include -O0 -g3 -Wall -c -fmessage-length=0 -fpic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


