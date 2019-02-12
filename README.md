
# ICG - Implementação de um rasterizador de pontos e de linhas
Aqui temos uma breve explicação sobre a realização da primeira atividade da disciplina Introdução a computação gráfica, ministrada pelo professor Christan Azambuja Pagot. O objetivo da atividade era implementar algoritmos para a rasterização de primitivas como pontos e linhas, através da escrita direta na memória, como os sistemas operacionais atuais protegem a memória quanto ao acesso direito, utilizamos um framework fornecido pelo professor para simular este acesso à memória de vídeo. 

### PutPixel
Foram implementadas 3 funções principais durante a atividade, a primeira se chama PutPixel e consiste em rasterizar um ponto na memória de vídeo, recebendo com parâmetros a posição do píxel na tela (x,y) e sua cor (RGBA).
Diferente do sistema de coordenadas cartesiano,no sistemas de computação gráfica a origem parte do píxel superior esquerdo, e a direção positiva do eixo y é para baixo.

![graphics3d_displaycoord](https://user-images.githubusercontent.com/31492509/52668397-31901400-2ef2-11e9-9e67-d7e18b1d0a31.png)

Além da posição os pixels também recebem informações referentes a sua cor, temos, ao todo, 4 canais (RGBA), cada canal varia de 0 à 255, determinando a cor final e o nível de transparência daquele píxel.
Durante a atividade decidimos utilizar uma abordagem orientada a objetos para facilitar o entendimento e também a organização do nosso código, os píxels foram representados por uma classe chamada Vertice, que concatena todas essas informações referentes a posição e cor. O offset (deslocamento) é encontrado através de uma fórmula __R = 4*(X + Y*W)__ que associa as coordenadas informadas e o número de colunas da tela, com essa posição é só colorir cada canal com os valores do objeto.

```
posicao = 4 * (pixel.getPosX() + (pixel.getPosY()*512));

	FBptr[posicao] = pixel.getR(); // componente R
	FBptr[posicao+1] = pixel.getG(); // componente G
	FBptr[posicao+2] = pixel.getB(); // componente B
	FBptr[posicao+3] = pixel.getA(); // componente A

```
### Exemplo de funcionamento:
Como entrada vamos passar um píxel simples, que está localizado na posição (50,50) e possui a cor vermelha.
```
Vertice v(50,50,255,0,0,0);
```
```
PutPixel(v);
```
Teremos a seguinte saída:

![solitario](https://user-images.githubusercontent.com/31492509/52673288-0829b500-2eff-11e9-902f-d85fd69ac857.png)


