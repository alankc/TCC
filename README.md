# Desenvolvimento de um sistema de logística para um robô móvel hospitalar utilizando mapas de grade
Dispositivos robóticos estão inseridos no cotidiano industrial a décadas, atuando como manipuladores fixados em seu local de trabalho controlado, onde são responsáveis por realizarem tarefas repetitivas e por vezes insalubres. Robôs móveis, no entanto, devem lidar com o dinamismo de um ambiente (e.g. pessoas, animais e objetos) para se deslocarem, tornando a execução das atividades muito mais complexa do que em um ambiente controlado. No intuito de facilitar o processo de navegação de um robô móvel, mapas podem ser utilizados para auxiliar na localização e no planejamento de rota. A navegação de um robô móvel em um ambiente envolve a realização de tarefas que, muitas vezes, podem ter prioridades. Assim, o sistema que planeja a rota a ser percorrida pelo robô deve levar em consideração não apenas encontrar o menor caminho, mas também o que permite que a prioridade na execução da tarefa seja atendida. O ambiente hospitalar oferece um nível elevado de dinamismo, o que aumenta a complexidade de navegação, além de requisitar que suas tarefas sejam executas de forma prioritária. Devido as características mencionadas tornarem as tarefas para robôs móveis desafiadoras, escolheu-se ambientes dinâmicos, como o hospitalar, para atuação do sistema desenvolvido, permitindo com que tarefas de logística sejam atendidas com base em suas prioridades em um ambiente previamente mapeado.

## Banco de Dados
Neste diretório você pode encontrar os arquivos para gerar o banco de dados, assim como preenche-lo com os dados utilizados para a realização dos experimentos.

Você pode utilizar o arquivo .cnf para utilizar as configurações idênticas as que utilizei, ou modificar dois parâmetros do seu arquivo de configuração do MySql:

`
[mysqldump]
quick
max_allowed_packet = 16M
`

`
key_buffer = 16M
`

`
innodb_log_file_size = 25MB
`

Estes parâmetros foram modificados para que fossem aceitas imagens com grandes dimensões no banco de dados.

## Dados dos Experimentos
Aqui estão presentes os resultados obtidos nos experimentos. Estes dados estão contidos também nos apêndices do texto.

Para compreender a organização da planilha recomendo a leitura, ao menos, da seção que descreve os experimentos no texto.

## Player e Stage
Neste diretório estão presentes os arquivos de configuração dos drivers para a simulação e para utilizar o Pionner P3-DX.

Recomendo a leitura deste [tutorial](http://player-stage-manual.readthedocs.io/en/v4.1.0/) caso você não for familiarizado com o Player e Stage.

### Dependências
 * Player 3.0.2
 * Stage 4.1.1
### Execução
Para utilizar o sistema em simulação:

`
player simulacao.cfg
`

Para utilizar o sistema com o robô:

`
player pionner.cfg
`

## Sistema
Este diretório contém os arquivos de código fonte do sistema para logística robótico utilizando mapas de grade.
### Dependências
* CXX11
* libplayer3.0.2
* libboost
* libmysqlcppconn
### Compilação
O makefile deste sistema é o gerado automaticamente pelo Netbeans 8.2. O comando para compilar o programa é:

`
make -f Makefile CONF=Release
`
## Texto
Contém o arquivo em PDF para o texto na íntegra do trabalho de conclusão de curso para o qual este sistema foi desenvolvido.

