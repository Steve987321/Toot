add_library("gtest" STATIC
      "../Libs/googletest/googletest/include/gtest/gtest-assertion-result.h"
      "../Libs/googletest/googletest/include/gtest/gtest-death-test.h"
      "../Libs/googletest/googletest/include/gtest/gtest-matchers.h"
      "../Libs/googletest/googletest/include/gtest/gtest-message.h"
      "../Libs/googletest/googletest/include/gtest/gtest-param-test.h"
      "../Libs/googletest/googletest/include/gtest/gtest-printers.h"
      "../Libs/googletest/googletest/include/gtest/gtest-spi.h"
      "../Libs/googletest/googletest/include/gtest/gtest-test-part.h"
      "../Libs/googletest/googletest/include/gtest/gtest-typed-test.h"
      "../Libs/googletest/googletest/include/gtest/gtest.h"
      "../Libs/googletest/googletest/include/gtest/gtest_pred_impl.h"
      "../Libs/googletest/googletest/include/gtest/gtest_prod.h"
          "../Libs/googletest/googletest/include/gtest/internal/custom/gtest-port.h"
          "../Libs/googletest/googletest/include/gtest/internal/custom/gtest-printers.h"
          "../Libs/googletest/googletest/include/gtest/internal/custom/gtest.h"
        "../Libs/googletest/googletest/include/gtest/internal/gtest-death-test-internal.h"
        "../Libs/googletest/googletest/include/gtest/internal/gtest-filepath.h"
        "../Libs/googletest/googletest/include/gtest/internal/gtest-internal.h"
        "../Libs/googletest/googletest/include/gtest/internal/gtest-param-util.h"
        "../Libs/googletest/googletest/include/gtest/internal/gtest-port-arch.h"
        "../Libs/googletest/googletest/include/gtest/internal/gtest-port.h"
        "../Libs/googletest/googletest/include/gtest/internal/gtest-string.h"
        "../Libs/googletest/googletest/include/gtest/internal/gtest-type-util.h"
    "../Libs/googletest/googletest/samples/prime_tables.h"
    "../Libs/googletest/googletest/samples/sample1.h"
    "../Libs/googletest/googletest/samples/sample2.h"
    "../Libs/googletest/googletest/samples/sample3-inl.h"
    "../Libs/googletest/googletest/samples/sample4.h"
    "../Libs/googletest/googletest/src/gtest-all.cc"
    "../Libs/googletest/googletest/src/gtest-assertion-result.cc"
    "../Libs/googletest/googletest/src/gtest-death-test.cc"
    "../Libs/googletest/googletest/src/gtest-filepath.cc"
    "../Libs/googletest/googletest/src/gtest-internal-inl.h"
    "../Libs/googletest/googletest/src/gtest-matchers.cc"
    "../Libs/googletest/googletest/src/gtest-port.cc"
    "../Libs/googletest/googletest/src/gtest-printers.cc"
    "../Libs/googletest/googletest/src/gtest-test-part.cc"
    "../Libs/googletest/googletest/src/gtest-typed-test.cc"
    "../Libs/googletest/googletest/src/gtest.cc"
    "../Libs/googletest/googletest/test/googletest-param-test-test.h"
    "../Libs/googletest/googletest/test/gtest-typed-test_test.h"
    "../Libs/googletest/googletest/test/production.h"
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("gtest" PROPERTIES
    OUTPUT_NAME "gtest"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release/gtest
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release/gtest
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release/gtest
  )
endif()
target_include_directories("gtest" PRIVATE
  $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../Libs/googletest/googletest>
  $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/../Libs/googletest/googletest/include>
)
target_compile_definitions("gtest" PRIVATE
)
target_link_directories("gtest" PRIVATE
)
target_link_libraries("gtest"
)
target_compile_options("gtest" PRIVATE
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++23>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("gtest" PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("gtest" PROPERTIES
    OUTPUT_NAME "gtest"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug/gtest
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug/gtest
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug/gtest
  )
endif()
target_include_directories("gtest" PRIVATE
  $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../Libs/googletest/googletest>
  $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/../Libs/googletest/googletest/include>
)
target_compile_definitions("gtest" PRIVATE
)
target_link_directories("gtest" PRIVATE
)
target_link_libraries("gtest"
)
target_compile_options("gtest" PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-O0>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-O0>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++23>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("gtest" PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()