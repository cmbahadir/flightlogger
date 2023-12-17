#include "flightlogger.hpp"

// Definitions for the extern variables
std::queue<std::string> dataQueue;
std::mutex mtx;
std::condition_variable cv;
std::condition_variable cvBuffer;
std::mutex mtxBuffer;
bool isTransactionFinished = true;

pqxx::connection* connectToDatabase(const std::string& connectionString)
{
    try
    {
        pqxx::connection* conn = new pqxx::connection(connectionString);
        if (conn->is_open()) {
            std::cout << "Opened database successfully: " << conn->dbname() << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
        }
        return conn;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception occurred in connectToDatabase: " << e.what() << std::endl;
        throw;
    }
}

void processReceivedData(pqxx::connection* conn, const std::string& line)
{
    // Wait until the previous transaction finishes
    {
        std::unique_lock<std::mutex> lockBuffer(mtxBuffer);
        cvBuffer.wait(lockBuffer, []{ return isTransactionFinished; });
    }

    // Push each line to the dataQueue
    {
        std::unique_lock<std::mutex> lock(mtx);
        dataQueue.push(line);
    }
    cv.notify_one();

    // Print the received line to stdout
    std::string quoted_line = "'" + boost::replace_all_copy(line, ",", "','") + "'";
    boost::replace_all(quoted_line, "\r", ""); // Remove "\r" in line ending

    // Write data to PostgreSQL
    if (conn && conn->is_open())
    {
        isTransactionFinished = false;

        pqxx::work W(*conn); // Create a new pqxx::work object for each transaction

        // Write received csv line with 22 fields to table LTFJ40 in flightlogger database
        W.exec("INSERT INTO LTFJ40 VALUES (" + quoted_line + ");");

        W.commit();

        isTransactionFinished = true;
        cvBuffer.notify_one();
    }
}

// Send data to postgresql
void sendDataToPostgreSQL(pqxx::connection* conn)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !dataQueue.empty(); });

        std::string line = dataQueue.front();
        dataQueue.pop();

        lock.unlock();

        // processReceivedData(conn, line);
    }
}


void receiveDataFromSocket(int socketFd, pqxx::connection* conn)
{
    while (true)
    {
        // Read from the connected socket
        char buffer[1024];
        ssize_t bytesRead = recv(socketFd, buffer, sizeof(buffer), 0);
        if (bytesRead > 0)
        {
            std::string receivedData(buffer, bytesRead);
            std::stringstream ss(receivedData);
            std::string line;
            while (std::getline(ss, line))
            {
                processReceivedData(conn, line);
            }
        }
    }
}


void connectToServer()
{
    // Create a socket
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1)
    {
        throw std::runtime_error("Failed to create socket");
    }

    // Set up the server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(30003);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set server address as localhost

    // Connect to the server
    if (connect(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        throw std::runtime_error("Failed to connect to port 30003");
    }

    std::string connectionString = "dbname = flightlogger user = postgres password = postgres hostaddr = 127.0.0.1 port = 5432";
    pqxx::connection* conn = connectToDatabase(connectionString);

    std::thread receiveThread(receiveDataFromSocket, socketFd, conn);
    std::thread sendThread(sendDataToPostgreSQL, conn);

    receiveThread.join();
    sendThread.join();

    // Close the socket
    close(socketFd);
    delete conn;
}

int main()
{
    try
    {
        connectToServer();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }
}
