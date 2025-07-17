add_executable("Compiler"
  "Compiler/Source/Compiler.cpp"
  "Compiler/Source/Compiler.h"
  "Compiler/Source/Lexer.cpp"
  "Compiler/Source/Lexer.h"
  "Compiler/Source/Main.cpp"
  "Compiler/Source/Parser.cpp"
  "Compiler/Source/Parser.h"
  "Compiler/Source/PreProcess.cpp"
  "Compiler/Source/PreProcess.h"
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_dependencies("Compiler"
    "TVM"
  )
  set_target_properties("Compiler" PROPERTIES
    OUTPUT_NAME "Compiler"
    ARCHIVE_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Debug-macosx-x86_64"
    LIBRARY_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Debug-macosx-x86_64"
    RUNTIME_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Debug-macosx-x86_64"
  )
endif()
target_include_directories("Compiler" PRIVATE
  $<$<CONFIG:Debug>:/Users/stefan/Developer/Steve987321/Toot/TVM/Source>
)
target_compile_definitions("Compiler" PRIVATE
)
target_link_directories("Compiler" PRIVATE
)
target_link_libraries("Compiler"
  $<$<CONFIG:Debug>:TVM>
)
target_compile_options("Compiler" PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-O0>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-O0>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++20>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("Compiler" PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
  add_dependencies("Compiler"
    "TVM"
  )
  set_target_properties("Compiler" PROPERTIES
    OUTPUT_NAME "Compiler"
    ARCHIVE_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Release-macosx-x86_64"
    LIBRARY_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Release-macosx-x86_64"
    RUNTIME_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/bin/Release-macosx-x86_64"
  )
endif()
target_include_directories("Compiler" PRIVATE
  $<$<CONFIG:Release>:/Users/stefan/Developer/Steve987321/Toot/TVM/Source>
)
target_compile_definitions("Compiler" PRIVATE
)
target_link_directories("Compiler" PRIVATE
)
target_link_libraries("Compiler"
  $<$<CONFIG:Release>:TVM>
)
target_compile_options("Compiler" PRIVATE
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++20>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("Compiler" PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()