# Lucas Pellegrinelli
# Código que adiciona no "pc_bordo.cpp" o .h necessário para as funções nativas
# do arduino serem reconhecidas e o int main()... que executa o loop do arduino.
# Após adicionar, o código é compilado utilizando as bibliotecas fakes, e depois
# as linhas adicionadas são removidas.

# Limpa o binário de outras possíveis execuções
make clean

# Copia o arquivo .ino para um .cpp nessa pasta para a compilação
cp ../PC_Bordo/PC_Bordo.ino pc_bordo.cpp

# Adiciona o import das constantes modificadas do Arduino no início do arquivo
echo '#include "arduino_consts.h"' | cat - pc_bordo.cpp > temp && mv temp pc_bordo.cpp

# Adiciona o main no final
echo 'int main(){try{setup();while(true){loop();}}catch(...){return 0;}}' >> pc_bordo.cpp

# Compila o programa
make

# Deleta a cópia .cpp que criamos do arquivo .ino
rm -f pc_bordo.cpp

# Executa os testes
./bancada
