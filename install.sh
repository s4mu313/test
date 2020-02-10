# ini_parser directory
cd ini_parser/ && mkdir build && cd build

cmake ..
make
sudo make install

cd .. && cd ..

# ini_parser directory
cd webserver/ && mkdir build && cd build

cmake ..
make

cd .. && cd ..

# ini_parser directory
cd webclient/ && mkdir build && cd build

cmake ..
make
