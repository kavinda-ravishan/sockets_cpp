cd ./server
cl.exe ./main_tcp_win.cpp 
cd ../client
cl.exe ./main_tcp_win.cpp 

cd ../server
START ./main_tcp_win.exe
cd ../client
START ./main_tcp_win.exe