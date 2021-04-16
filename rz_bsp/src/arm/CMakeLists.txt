# FreeRTOS for Renesas RZ/A StreamIt Boards
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(FreeRTOS10)
SET(PROJECT_VERSION 10.0.0)

SET(ARM_INC
  "${PROJECT_SOURCE_DIR}/cmsis/inc/arm_common_tables.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/arm_const_structs.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/arm_math.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/cmsis_os.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_ca.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_caFunc.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_caInstr.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_ca_mmu.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_cm0.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_cm0plus.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_cm3.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_cm4.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_cm7.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_cmFunc.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_cmInstr.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_cmSimd.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_sc000.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/core_sc300.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/RTX_CM_lib.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/RTX_Config.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/rt_HAL_CA.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/rt_HAL_CM.h"
  "${PROJECT_SOURCE_DIR}/cmsis/inc/rt_TypeDef.h"
)

SET(HEADERS_LIST_ARM
    ${ARM_INC}
    PARENT_SCOPE
)

SOURCE_GROUP("Header Files\\ARM" FILES ${ARM_INC})

INCLUDE_DIRECTORIES(
  "${PROJECT_SOURCE_DIR}/cmsis"
  "${PROJECT_SOURCE_DIR}/cmsis/inc"
  "${PROJECT_SOURCE_DIR}/cmsis/src"
)

SET(ARM_SRC
  "${PROJECT_SOURCE_DIR}/cmsis/src/HAL_CA.c"
)

SET(SOURCES_LIST_ARM
    ${ARM_SRC}
    PARENT_SCOPE
)

SOURCE_GROUP("Source Files\\ARM" FILES ${ARM_SRC})
