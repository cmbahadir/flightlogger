DB_USER="flightlogger_user"
DB_PASS="flightlogger_pass"
DB_NAME="flightlogger"
DB_TABLE="LTFJ40"

# Check if database exists and drop it
sudo -u postgres psql -c "DROP DATABASE IF EXISTS $DB_NAME;"

# Check if user exists and drop it
sudo -u postgres psql -c "DROP USER IF EXISTS $DB_USER;"

# Create a new user
sudo -u postgres createuser --interactive --password $DB_USER

# Create a new database
sudo -u postgres createdb -O $DB_USER $DB_NAME

# Log in to the new database, check if table exists and drop it, then create a new table
sudo -u postgres psql -d $DB_NAME -c "
DROP TABLE IF EXISTS $DB_TABLE;
CREATE TABLE $DB_TABLE (
    Message TEXT,
    Transmission TEXT,
    Session TEXT,
    AircraftID TEXT,
    HexIdent TEXT,
    FlightID TEXT,
    Date_message_generated TEXT,
    Time_message_generated TEXT,
    Date_message_logged TEXT,
    Time_message_logged TEXT,
    Callsign TEXT,
    Altitude TEXT,
    GroundSpeed TEXT,
    Track TEXT,
    Latitude TEXT,
    Longitude TEXT,
    VerticalRate TEXT,
    Squawk TEXT,
    Alert TEXT,
    Emergency TEXT,
    SPI TEXT,
    IsOnGround TEXT
);"