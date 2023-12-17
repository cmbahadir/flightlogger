#ifndef FLIGHTLOGGER_H
#define FLIGHTLOGGER_H

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <thread>
#include <pqxx/pqxx>
#include <boost/algorithm/string/replace.hpp>

// Using directives for convenience
using std::string;
using std::queue;
using std::mutex;
using std::condition_variable;

// Global variables
extern queue<string> dataQueue;
extern mutex mtx;
extern condition_variable cv;
extern condition_variable cvBuffer;
extern mutex mtxBuffer;
extern bool isTransactionFinished;

// Function declarations
pqxx::connection* connectToDatabase(const string& connectionString);
void processReceivedData(pqxx::connection* conn, const string& line);
void sendDataToPostgreSQL(pqxx::connection* conn);
void receiveDataFromSocket(int socketFd, pqxx::connection* conn);
void connectToServer();

#endif // FLIGHTLOGGER_H