add_executable("Tests"
        "../Compiler/Source/Compiler.cpp"
        "../Compiler/Source/Lexer.cpp"
        "../Compiler/Source/Parser.cpp"
        "../Compiler/Source/PreProcess.cpp"
          "../TVM/Source/Lib/CPPBinding.cpp"
          "../TVM/Source/Lib/IO.cpp"
        "../TVM/Source/TVM.cpp"
            "../libs/googletest/googletest/src/gtest_main.cc"
    "Compiler/test_lexer.cpp"
    "Compiler/test_parser.cpp"
    "TVM/test_vm.cpp"
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  add_dependencies("Tests"
    "gtest"
  )
  set_target_properties("Tests" PROPERTIES
    OUTPUT_NAME "Tests"
    ARCHIVE_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/Tests/bin/Release-macosx"
    LIBRARY_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/Tests/bin/Release-macosx"
    RUNTIME_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/Tests/bin/Release-macosx"
  )
endif()
target_include_directories("Tests" PRIVATE
  $<$<CONFIG:Release>:/Users/stefan/Developer/Steve987321/Toot/libs/googletest/googletest/include>
  $<$<CONFIG:Release>:/Users/stefan/Developer/Steve987321/Toot/Compiler/Source>
  $<$<CONFIG:Release>:/Users/stefan/Developer/Steve987321/Toot/TVM/Source>
)
target_compile_definitions("Tests" PRIVATE
)
target_link_directories("Tests" PRIVATE
)
target_link_libraries("Tests"
  $<$<CONFIG:Release>:gtest>
)
target_compile_options("Tests" PRIVATE
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++20>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("Tests" PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_dependencies("Tests"
    "gtest"
  )
  set_target_properties("Tests" PROPERTIES
    OUTPUT_NAME "Tests"
    ARCHIVE_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/Tests/bin/Debug-macosx"
    LIBRARY_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/Tests/bin/Debug-macosx"
    RUNTIME_OUTPUT_DIRECTORY "/Users/stefan/Developer/Steve987321/Toot/Tests/bin/Debug-macosx"
  )
endif()
target_include_directories("Tests" PRIVATE
  $<$<CONFIG:Debug>:/Users/stefan/Developer/Steve987321/Toot/libs/googletest/googletest/include>
  $<$<CONFIG:Debug>:/Users/stefan/Developer/Steve987321/Toot/Compiler/Source>
  $<$<CONFIG:Debug>:/Users/stefan/Developer/Steve987321/Toot/TVM/Source>
)
target_compile_definitions("Tests" PRIVATE
)
target_link_directories("Tests" PRIVATE
)
target_link_libraries("Tests"
  $<$<CONFIG:Debug>:gtest>
)
target_compile_options("Tests" PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-O0>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-O0>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++20>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("Tests" PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()