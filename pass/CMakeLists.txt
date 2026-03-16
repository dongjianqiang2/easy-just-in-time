add_llvm_library(EasyJitPass
  Easy.cpp
  Layout.cpp
  Utils.cpp
  MayAliasTracer.cpp
  RegisterPasses.cpp
)

install(TARGETS EasyJitPass
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
