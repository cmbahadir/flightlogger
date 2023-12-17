#include "gtest/gtest.h"
#include "../flightlogger.hpp"

// Mock database connection
pqxx::connection* conn = nullptr;

TEST(FlightLoggerTest, ProcessReceivedData) {
    std::string line = "Test data";
    processReceivedData(conn, line);
    // Add assertions here to verify the behavior of processReceivedData
}

TEST(FlightLoggerTest, SendDataToPostgreSQL) {
    sendDataToPostgreSQL(conn);
    // Add assertions here to verify the behavior of sendDataToPostgreSQL
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}