add_executable(easy-jit-opt main.cpp)

target_link_libraries(easy-jit-opt
  PRIVATE
  EasyJitPass
  LLVMCore
  LLVMIRReader
  LLVMBitWriter
  LLVMSupport
)

set(EASY_JIT_PASS_TOOL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/easy-jit-opt PARENT_SCOPE)

install(TARGETS easy-jit-opt
        RUNTIME DESTINATION bin)
