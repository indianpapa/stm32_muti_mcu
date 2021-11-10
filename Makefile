TARGET = daze

OPT = -Og

BUILD_DIR = $(STM32_BUILD_DIR)

MCU_RES_PATH = Res

DRIVER_ROOT_PATH = Drivers

CORE_ROOT_PATH = Core

CORE_PLATFORM_PATH = $(CORE_ROOT_PATH)/$(STM32_MCU_CLASS_NAME)

HAL_DRIVER_ROOT_PATH = $(DRIVER_ROOT_PATH)/$(STM32_MCU_CLASS_NAME)_HAL_Driver

IMPL_ROOT_PATH = Impl

ifeq ($(STM32_MCU_EXACT_NAME), STM32F103C8)
CORE_EXACT_PLATFORM_IMPL_PATH = $(CORE_ROOT_PATH)/Src/f103c8
else ifeq ($(STM32_MCU_EXACT_NAME), STM32F407VG)
CORE_EXACT_PLATFORM_IMPL_PATH = $(CORE_ROOT_PATH)/Src/f407vg
else ifeq ($(STM32_MCU_EXACT_NAME), STM32F407ZE)
CORE_EXACT_PLATFORM_IMPL_PATH = $(CORE_ROOT_PATH)/Src/f407ze
else ifeq ($(STM32_MCU_EXACT_NAME), STM32F411CE)
CORE_EXACT_PLATFORM_IMPL_PATH = $(CORE_ROOT_PATH)/Src/f411ce
endif

C_SOURCES =  \
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX).c				\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_exti.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_tim.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_tim_ex.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_rcc.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_dma.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_cortex.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_gpio.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_uart.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_pwr.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_rcc_ex.c		 	\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_flash.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_flash_ex.c		\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_usart.c

C_SOURCES += $(wildcard $(CORE_PLATFORM_PATH)/Src/*.c)

C_SOURCES += $(wildcard $(CORE_ROOT_PATH)/Src/*.c)

C_SOURCES += $(wildcard $(CORE_EXACT_PLATFORM_IMPL_PATH)/*.c)

C_SOURCES += $(wildcard $(CORE_ROOT_PATH)/Src/peripherals/*.c)

C_SOURCES += $(wildcard $(CORE_ROOT_PATH)/Src/base/*.c)

C_SOURCES += $(wildcard $(IMPL_ROOT_PATH)/Src/*.c)

ifeq ($(STM32_MCU_CLASS_NAME), STM32F4xx)
C_SOURCES +=	\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_pwr_ex.c			\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_flash_ramfunc.c	\
$(HAL_DRIVER_ROOT_PATH)/Src/$(STM32_HAL_DRIVER_SRC_PREFIX)_dma_ex.c

else ifeq ($(STM32_MCU_CLASS_NAME), STM32F1xx)
endif

# ASM sources
ASM_SOURCES = $(MCU_RES_PATH)/$(STM32_START_UP_ASM_SOURCE)

#######################################
# binaries
#######################################
ARM_GCC_BIN_PATH = $(ARM_GCC_PATH)/bin
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
CC = $(ARM_GCC_BIN_PATH)/$(PREFIX)gcc
AS = $(ARM_GCC_BIN_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(ARM_GCC_BIN_PATH)/$(PREFIX)objcopy
SZ = $(ARM_GCC_BIN_PATH)/$(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = $(STM32_CPU)

# fpu
FPU = $(STM32_FPU)

# float-abi
FLOAT-ABI = $(STM32_FLOAT_ABI)

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-D$(STM32_MCU_FAMILY)

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-I$(CORE_ROOT_PATH)/Inc													\
-I$(CORE_PLATFORM_PATH)/Inc												\
-I$(HAL_DRIVER_ROOT_PATH)/Inc											\
-I$(HAL_DRIVER_ROOT_PATH)/Inc/Legacy									\
-I$(DRIVER_ROOT_PATH)/CMSIS/Device/ST/$(STM32_MCU_CLASS_NAME)/Include	\
-I$(DRIVER_ROOT_PATH)/CMSIS/Include										\
-I$(IMPL_ROOT_PATH)/Inc

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = $(MCU_RES_PATH)/$(STM32_LINKER_SCRIPT)

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***