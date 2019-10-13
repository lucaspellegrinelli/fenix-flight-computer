make clean
cp ../PC_Bordo/PC_Bordo.ino pc_bordo.cpp
cp -a ../PC_Bordo/Bibliotecas\ Necessárias/Filtro_Kalman/. Arduino/lib
echo '#include "arduino_consts.h"' | cat - pc_bordo.cpp > temp && mv temp pc_bordo.cpp
echo 'int main(){try{setup();for(int i=0;i<750;i++){loop();}}catch(...){return 0;}}' >> pc_bordo.cpp
make
rm -f pc_bordo.cpp
rm -rf Arduino/lib
./bancada
make clean




