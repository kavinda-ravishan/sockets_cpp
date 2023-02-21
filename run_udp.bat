cd ./server
cl.exe ./main_udp_win.cpp 
cd ../client
cl.exe ./main_udp_win.cpp 

cd ../server
START ./main_udp_win.exe
cd ../client
START ./main_udp_win.exe