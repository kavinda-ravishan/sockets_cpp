cd ./server
cl.exe ./main_win.cpp 
cd ../client
cl.exe ./main_win.cpp 

cd ../server
START ./main_win.exe
cd ../client
START ./main_win.exe