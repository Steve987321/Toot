add_library("TVM" STATIC
    "TVM/Source/Lib/CPPBinding.cpp"
    "TVM/Source/Lib/CPPBinding.h"
    "TVM/Source/Lib/IO.cpp"
    "TVM/Source/Lib/IO.h"
  "TVM/Source/TVM.cpp"
  "TVM/Source/TVM.h"
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("TVM" PROPERTIES
    OUTPUT_NAME "TVM"
    ARCHIVE_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Debug-macosx-x86_64"
    LIBRARY_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Debug-macosx-x86_64"
    RUNTIME_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Debug-macosx-x86_64"
  )
endif()
target_include_directories("TVM" PRIVATE
  $<$<CONFIG:Debug>:/Users/stefan/Developer/Steve987321/Toot/TVM/Source>
)
target_compile_definitions("TVM" PRIVATE
)
target_link_directories("TVM" PRIVATE
)
target_link_libraries("TVM"
)
target_compile_options("TVM" PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++20>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("TVM" PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("TVM" PROPERTIES
    OUTPUT_NAME "TVM"
    ARCHIVE_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Release-macosx-x86_64"
    LIBRARY_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Release-macosx-x86_64"
    RUNTIME_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Release-macosx-x86_64"
  )
endif()
target_include_directories("TVM" PRIVATE
  $<$<CONFIG:Release>:/Users/stefan/Developer/Steve987321/Toot/TVM/Source>
)
target_compile_definitions("TVM" PRIVATE
)
target_link_directories("TVM" PRIVATE
)
target_link_libraries("TVM"
)
target_compile_options("TVM" PRIVATE
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++20>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("TVM" PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()