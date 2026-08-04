# ce-arm.cmake - для сборки под ARM
set(CMAKE_SYSTEM_NAME WindowsCE)
set(CMAKE_SYSTEM_VERSION 5.0)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Путь к компилятору
set(CMAKE_C_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/ce/bin/x86_arm/cl.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/ce/bin/x86_arm/cl.exe")

# Пути к SDK
set(SDK_ROOT "C:/Program Files (x86)/Windows CE Tools/SDKs/your-sdk")
set(CMAKE_FIND_ROOT_PATH ${SDK_ROOT})

# Настройки для поиска библиотек
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Флаги компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:ARMv4 /D_ARM_")
