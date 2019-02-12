
# ICG - Implementação de um rasterizador de pontos e de linhas
Aqui temos uma breve explicação sobre a realização da primeira atividade da disciplina Introdução a computação gráfica, ministrada pelo professor Christan Azambuja Pagot. O objetivo da atividade era implementar algoritmos para a rasterização de primitivas como pontos e linhas, através da escrita direta na memória, como os sistemas operacionais atuais protegem a memória quanto ao acesso direito, utilizamos um framework fornecido pelo professor para simular este acesso à memória de vídeo. 

Foram implementadas 3 funções principais durante a atividade, a primeira se chama PutPixel e consiste em rasterizar um ponto na memória de vídeo, recebendo com parâmetros a psoição do píxel na tela (x,y) e sua cor (RGBA).
Diferente do sistema de coordenadas cartesiano, sistemas de computação gráfica utilizam um sistema de coordenadas onde a origem parte do píxel superior esquerdo, e a direção positiva do eixo y é para baixo.
![graphics3d_displaycoord](https://user-images.githubusercontent.com/31492509/52667904-e3c6dc00-2ef0-11e9-88ec-ca30da3772ac.png)
