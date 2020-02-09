# Write file ini 
echo "[section]" >> /tmp/ini
echo "foo.bar = some value" >> /tmp/ini

# Move in server directory
cd webserver/build/ 

# Launch the server
xterm -hold -e ./webserver & export PID=$!

sleep 5

# Move in client directory
cd ..
cd ..
cd webclient/build

# Launch client and load file 
./webclient --load /tmp/ini

sleep 2

# Launch client and get existent value
./webclient --get section.foo.bar

sleep 2

# Launch client and get non existent value
./webclient --get section.other

sleep 2

# Launch client and get non existent value
./webclient --set section.other data_from_test_2

# Verify the process is started
if ps -p $PID > /dev/null
then
   echo "$PID is running"
fi

# Kill process
kill $PID
echo "$PID shutting down..."
