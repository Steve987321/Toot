add_library("Language" STATIC
    "Source/compiler/Compiler.cpp"
    "Source/compiler/Compiler.h"
    "Source/compiler/Lexer.cpp"
    "Source/compiler/Lexer.h"
    "Source/compiler/Parser.cpp"
    "Source/compiler/Parser.h"
    "Source/compiler/PreProcess.cpp"
    "Source/compiler/PreProcess.h"
    "Source/system/File.cpp"
    "Source/system/File.h"
    "Source/tvm/Lib/CPPBinding.cpp"
    "Source/tvm/Lib/CPPBinding.h"
    "Source/tvm/Lib/IO.cpp"
    "Source/tvm/Lib/IO.h"
    "Source/tvm/TVM.cpp"
    "Source/tvm/TVM.h"
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("Language" PROPERTIES
    OUTPUT_NAME "Language"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Debug-macosx-x86_64
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Debug-macosx-x86_64
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Debug-macosx-x86_64
  )
endif()
target_include_directories("Language" PRIVATE
  $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/Source>
)
target_compile_definitions("Language" PRIVATE
)
target_link_directories("Language" PRIVATE
)
target_link_libraries("Language"
)
target_compile_options("Language" PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-O0>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-O0>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++23>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("Language" PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("Language" PROPERTIES
    OUTPUT_NAME "Language"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Release-macosx-x86_64
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Release-macosx-x86_64
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Release-macosx-x86_64
  )
endif()
target_include_directories("Language" PRIVATE
  $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/Source>
)
target_compile_definitions("Language" PRIVATE
)
target_link_directories("Language" PRIVATE
)
target_link_libraries("Language"
)
target_compile_options("Language" PRIVATE
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++23>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("Language" PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()