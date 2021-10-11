set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR p2)
SET(CMAKE_CROSSCOMPILING 1)
set(CMAKE_C_COMPILER_WORKS 1)

set(llvm "/opt/p2llvm/bin")
# set(sysroot "/opt/l4t-gcc/gcc-aarch64-linux-gnu")

set(target_ops "-fno-exceptions -fno-jump-tables --target=p2 -Dprintf=__simple_printf")

set(CMAKE_C_FLAGS "${target_ops} -ffunction-sections -fdata-sections" CACHE STRING "" FORCE)
set(CMAKE_C_LINK_FLAGS "--target=p2")
set(CMAKE_CXX_FLAGS "${target_ops} -ffunction-sections -fdata-sections" CACHE STRING "" FORCE)
set(CMAKE_CXX_LINK_FLAGS "--target=p2")
set(CMAKE_C_COMPILER ${llvm}/clang)
set(CMAKE_CXX_COMPILER ${llvm}/clang++)
set(CMAKE_AR ${llvm}/llvm-ar)
set(CMAKE_RANLIB ${llvm}/llvm-ranlib)