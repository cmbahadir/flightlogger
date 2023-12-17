# test/CMakeLists.txt

# Find the Google Test package
find_package(GTest REQUIRED)

# Add the test executable
add_executable(FlightLoggerTest test_flightlogger.cpp)

# Link the test executable with the Google Test libraries and the necessary libraries
target_link_libraries(FlightLoggerTest GTest::GTest GTest::Main pthread ${LIBPQXX_LIBRARIES})

# Add the test to CTest
include(GoogleTest)
gtest_discover_tests(FlightLoggerTest)