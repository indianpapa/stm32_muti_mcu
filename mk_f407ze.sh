export STM32_BUILD_DIR=build_f407ze
export STM32_CPU="-mcpu=cortex-m4"
export STM32_FPU="-mfpu=fpv4-sp-d16"
export STM32_FLOAT_ABI="-mfloat-abi=hard"
export STM32_MCU_FAMILY=STM32F407xx
export STM32_MCU_CLASS_NAME=STM32F4xx
export STM32_MCU_EXACT_NAME=STM32F407ZE
export STM32_START_UP_ASM_SOURCE=F407/startup_stm32f407xx.s
export STM32_LINKER_SCRIPT=F407/STM32F407VGTx_FLASH.ld
export STM32_HAL_DRIVER_SRC_PREFIX=stm32f4xx_hal
make clean
make