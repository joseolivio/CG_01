
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
###### Exemplo de funcionamento:
Como entrada vamos passar um píxel simples, que está localizado na posição (50,50) e possui a cor vermelha.
```
Vertice v(50,50,255,0,0,0);
```
```
PutPixel(v);
```
Teremos a seguinte saída:

![solitario](https://user-images.githubusercontent.com/31492509/52673288-0829b500-2eff-11e9-902f-d85fd69ac857.png)

### DrawLine

Esta função rasteriza uma linha na tela, recebendo como parâmetros dois vértices (inicial e final), o algoritmo implementado para rasterização foi o de Bresenham, que permite determinar quais os pontos numa matriz de base quadriculada que devem ser destacados para atender o grau de inclinação de um ângulo. Também é feita uma interpolação linear entre os vértices, causando o efeito degradê.
###### Algoritmo de Bresenham
O interessante deste algoritmo é que utiliza apenas adição, subtração de inteiros e deslocamento de bits, operações muito simples e baratas em termos de processamento, o algoritmo determina os pontos que serão coloridos a fim de realizar uma aproximação de uma linha reta entre dois pontos. Partimos do algoritmo de Bresenham para o primeiro octante, e a partir daí modificamos o algoritmo para contemplar todos os casos.

![definitivo](https://user-images.githubusercontent.com/31492509/52677156-8429fa00-2f0b-11e9-88e5-f67ecd8e152a.png)

**1° e 5° OCTANTE**

Para rasterizar retas no quinto octante foi simples, apenas era necessário inverter qual seria o vértice inicial e o vértice final, para isso, foi criada uma condição que verifica em qual octante está a reta, e se necessário inverte os pixels.
```
	if (pixel2.getPosX() < pixel1.getPosX())
	{
		InverteVertice(pixel1, pixel2);
		cout << "INVERTIDO" << endl;
	}
``` 








