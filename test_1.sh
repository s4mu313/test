# Move in directory
cd webserver/build/ 

# Launch the server
./webserver & export PID=$!

# Verify status of port 12345
ss -tulpn | grep 12345

# Verify the process is started
if ps -p $PID > /dev/null
then
   echo "$PID is running"
fi

# Kill process
kill $PID
echo "$PID shutting down..."

# Verify status of port 12345
ss -tulpn | grep 12345
