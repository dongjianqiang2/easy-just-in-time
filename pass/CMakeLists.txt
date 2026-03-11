if (COMMAND add_llvm_loadable_module)
  add_llvm_loadable_module(EasyJitPass
    RegisterPasses.cpp
    Easy.cpp
    Layout.cpp
    Utils.cpp
    MayAliasTracer.cpp
    PLUGIN_TOOL clang
  )
else()
  add_library(EasyJitPass MODULE
    RegisterPasses.cpp
    Easy.cpp
    Layout.cpp
    Utils.cpp
    MayAliasTracer.cpp
  )

  llvm_map_components_to_libnames(EASY_JIT_PASS_LLVM_LIBS
    core
    ipo
    irreader
    passes
    support
    transformutils
  )
  target_link_libraries(EasyJitPass PRIVATE ${EASY_JIT_PASS_LLVM_LIBS})
endif()

set(EASY_JIT_PASS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/EasyJitPass${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)

install(TARGETS EasyJitPass
        LIBRARY DESTINATION lib)

# Get proper shared-library behavior (where symbols are not necessarily
# resolved when the shared library is linked) on OS X.
if(APPLE)
    set_target_properties(EasyJitPass PROPERTIES
        LINK_FLAGS "-undefined dynamic_lookup"
    )
endif(APPLE)
