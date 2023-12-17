# FlightLogger

FlightLogger is a C++ project that logs flight data. It can be used to process and store flight data from a running instance of dump1090 with SBS output at port 30003.

## Dependencies

The project has the following dependencies:

- CMake version 3.12.0 or higher
- C++17
- PkgConfig
- libpqxx
- A running instance of PostgreSQL at port 5432
- A running instance of dump1090 with SBS output at port 30003

## Building the Project

To build the project, navigate to the `build` directory and run `cmake ..` and `make`.

```bash
cd build
cmake ..
make
```

To build tests run cmake with "cmake -DBUILD_TESTS:BOOL=ON .." 

## Configuring the Database

To configure the database, run the `configure_db.sh` script.

```bash
./configure_db.sh
```

## Running the Project

To run the project, you need to have a running instance of dump1090 with SBS output at port 30003. Once you have that, you can run the `flightlogger` executable:

```bash
./flightlogger
```

This will start the flightlogger, which will process and store flight data from the running instance of dump1090.

## Contributing

Contributions are welcome! To contribute, please fork the repository, create a new branch for your changes, commit your changes to that branch, and create a pull request to the original repository.