include(FetchContent)

set(DOWNLOAD_EXTRACT_TIMESTAMP True)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(COMMON_SRCS
../network/Message.hpp
../GameState.hpp
)

add_executable(snake_test 
  serialisation_tests.cpp
  ${COMMON_SRCS})

target_link_libraries(snake_test GTest::gtest_main gmock)

target_compile_features(snake_test PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(snake_test)


