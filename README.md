# PC de Bordo da Fênix

Esse repositório contém o projeto do Arduino com o código referente ao computador de bordo para a Fênix tal como todas as bibliotecas utilizadas. Também pode ser encontrado uma bancada de testes local para o PC de Bordo. Mais explicações a seguir.

## PC de Bordo

O PC de Bordo encontrado no arquivo ```PC_Bordo/PC_Bordo.ino``` as seguintes funções:

### Detectar apogeu

Para a detecção do apogeu utilizamos dois sensores em conjunto: o acelerômetro e o barômetro. O acelerômetro tem uma propriedade interessante que ao entrar em estado de queda livre, o valor apontado é ~0m/s^2. Isso é interessante pois quando sabemos que o módulo da velocidade é próximo de 0, podemos começar a prestar mais atenção ao apogeu. Também é utilizado um sistema de segurança que ativa o sistema de ejeção após um tempo específico desde o início da queda livre para garantir que mesmo que o apogeu não seja detectado, o foguete ainda sim sobreviva.

O barômetro é utilizado para detectar a altura, e quando o valor dele desce em vários loops seguidos, podemos assumir que passamos do apogeu.

Ambos sensores tem seu sinal tratado por meio de um filtro de Kalman.

### Ejeção

Para ejetar o paraquedas do foguete, a única coisa necessária é acionar um servo motor, o que é feito próximo ao apogeu.

### Armazenamento dos dados do vôo

Também é utilizado um módulo de cartão SD que guarda todas as leituras e ações que o foguete tomou durante o vôo para um estudo mais aprofundado do que deu certo e o que deu errado no lançamento.

### Links úteis:

##### Como um acelerômetro funciona (e porquê ele marca 0m/s2 em queda livre)
https://www.youtube.com/watch?v=-om0eTXsgnY

##### Exemplo de um PC de bordo
https://shanetully.com/2016/07/inside-the-construction-of-an-amateur-rocketry-flight-computer/

##### Implementação de um acelerômetro, giroscópio e módulo SD
https://makersportal.com/blog/2019/8/17/arduino-mpu6050-high-frequency-accelerometer-and-gyroscope-data-saver

##### Exemplo de um lançamento (útil para ver o gráfico do acelerômetro e seu ruído)
https://www.youtube.com/watch?v=7WYBVW2gnr8

## Bancada de Testes

Para não correr o risco de erros de lógica no PC de Bordo passarem despercebidos, testar rotinas de segurança caso o ruído seja alto ou testar o comportamento geral do PC de bordo foi criado uma bancada de simulação para testar o código. Basicamente existe arquivos (```Bancada_Testes/Arduino/```) que definem constantes do ambiente do Arduino (como o ```Serial.println()```) para se adaptar a uma execução local tal como implementações superfíciais das bibliotecas utilizadas pelos módulos conectados que substituem a leitura de dados do sensor pela leitura de um arquivo de texto com os dados simulados de um lançamento. As leituras e ações do foguete são mostradas em um gráfico.

### Requerimentos

- Um sistema Linux (existem comandos shell ```.sh``` para serem executados)
- Python (foi testado no Python 3.6)
  - Matplotlib
  - Numpy

### Preparação

O arquivo ```.ino``` do PC de Bordo deverá ser colocado na pasta ```PC_Bordo``` com o nome de ```PC_Bordo.ino```. As bibliotecas necessárias para usar os módulos ligados ao Arduino deverão ser criadas de forma mínima na pasta ```Bancada_Testes/Arduino/``` (como mostram alguns exemplos já criados). Os dados simulados de cada um dos sensores deverão ser colocados ```Bancada_Testes/Dados/```.

### Execução

Assim que tudo estiver configurado, o arquivo ```mostrar_teste.py``` deverá ser executado da forma

```python Bancada_Teste/mostrar_teste.py```