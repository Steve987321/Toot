add_library("Toot" STATIC
    "Toot/compiler/Compiler.cpp"
    "Toot/compiler/Compiler.h"
    "Toot/compiler/Lexer.cpp"
    "Toot/compiler/Lexer.h"
    "Toot/compiler/Parser.cpp"
    "Toot/compiler/Parser.h"
    "Toot/compiler/PreProcess.cpp"
    "Toot/compiler/PreProcess.h"
    "Toot/system/File.cpp"
    "Toot/system/File.h"
      "Toot/tvm/Lib/CPPBinding.cpp"
      "Toot/tvm/Lib/CPPBinding.h"
      "Toot/tvm/Lib/IO.cpp"
      "Toot/tvm/Lib/IO.h"
    "Toot/tvm/TVM.cpp"
    "Toot/tvm/TVM.h"
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("Toot" PROPERTIES
    OUTPUT_NAME "Toot"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug-macosx-x86_64
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug-macosx-x86_64
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug-macosx-x86_64
  )
endif()
target_include_directories("Toot" PRIVATE
  $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/Toot>
)
target_compile_definitions("Toot" PRIVATE
)
target_link_directories("Toot" PRIVATE
)
target_link_libraries("Toot"
)
target_compile_options("Toot" PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-O0>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-O0>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++23>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("Toot" PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("Toot" PROPERTIES
    OUTPUT_NAME "Toot"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release-macosx-x86_64
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release-macosx-x86_64
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release-macosx-x86_64
  )
endif()
target_include_directories("Toot" PRIVATE
  $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/Toot>
)
target_compile_definitions("Toot" PRIVATE
)
target_link_directories("Toot" PRIVATE
)
target_link_libraries("Toot"
)
target_compile_options("Toot" PRIVATE
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++23>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("Toot" PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()