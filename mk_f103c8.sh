export STM32_BUILD_DIR=build_f103c8
export STM32_CPU="-mcpu=cortex-m3"
export STM32_FPU=""
export STM32_FLOAT_ABI=""
export STM32_MCU_FAMILY=STM32F103xB
export STM32_MCU_CLASS_NAME=STM32F1xx
export STM32_MCU_EXACT_NAME=STM32F103C8
export STM32_START_UP_ASM_SOURCE=F103/startup_stm32f103xb.s
export STM32_LINKER_SCRIPT=F103/STM32F103C8Tx_FLASH.ld
export STM32_HAL_DRIVER_SRC_PREFIX=stm32f1xx_hal
make clean
make