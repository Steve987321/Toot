add_executable("Tests"
            "../Libs/googletest/googletest/src/gtest_main.cc"
        "../Toot/compiler/Compiler.cpp"
        "../Toot/compiler/Lexer.cpp"
        "../Toot/compiler/Parser.cpp"
        "../Toot/compiler/PreProcess.cpp"
        "../Toot/system/File.cpp"
          "../Toot/tvm/Lib/CPPBinding.cpp"
          "../Toot/tvm/Lib/IO.cpp"
        "../Toot/tvm/TVM.cpp"
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
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release
  )
endif()
target_include_directories("Tests" PRIVATE
  $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../Libs/googletest/googletest/include>
  $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../Toot>
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
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++23>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("Tests" PROPERTIES
    CXX_STANDARD 23
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
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug
  )
endif()
target_include_directories("Tests" PRIVATE
  $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../Libs/googletest/googletest/include>
  $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../Toot>
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
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++23>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("Tests" PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()