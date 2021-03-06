
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
A saída é apresentada em uma tela que possui dimensão 512x512, ou seja, ao todo são 262144 pixels.
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

Para identificar as retas presentes nestes octantes utilizamos uma verificação baseada no eixo de coordenadas, as retas presentes no primeiro octante tinham um deslocamento x (Δx) maior que o deslocamento no eixo y (Δy), e o módulo do seu Δx é maior que o módulo do Δy.
```
	if(dx > 0 && dy > 0 && abs(dx) > abs(dy)){
```

Rasterizar as retas no quinto octante foi simples, partindo do algoritmo que renderiza no primeiro octante, apenas era necessário inverter qual seria o vértice inicial e o vértice final, para isso, foi criada uma condição que verifica se o Δx é negativo, e se necessário, inverte os pixels. 
```
	if (pixel2.getPosX() < pixel1.getPosX())
	{
		InverteVertice(pixel1, pixel2);
		cout << "INVERTIDO" << endl;
	}
``` 
###### Exemplo de funcionamento:
Exemplo da função DrawLine com os vértices nas seguintes coordenadas:
```
	Vertice v1(50, 100, 255, 0, 0, 255);
	Vertice v2(400, 200, 0, 255, 0, 255);
```

![whatsapp image 2019-02-13 at 10 40 41](https://user-images.githubusercontent.com/31492509/52716645-ff2bf880-2f7d-11e9-9417-b899d5a266f3.jpeg)

**2° e 6° OCTANTE**

Para conseguir rasterizar retas no segundo octante foi necessário inverter os eixos x e y no momento de rasterizar, pois, se observamos o eixo de coordenadas, a reta vai crescendo mais paralela ao eixo y, então uma das mudanças foi incrementar a coordenada y, diferente do passo anterior, onde incrementamos a coordenada x. Para renderizar retas no 6° octante não foi necessário criar nenhuma condição especial, pois a condição de inversão já garante este caso também.
```
	if (d <= 0){
		d += incr_e;
		aux.setPosY(aux.getPosY()+1);
	} else{
		d += incr_ne;
		aux.setPosY(aux.getPosY()+1);
		aux.setPosX(aux.getPosX()+1);
	}
```
###### Exemplo de funcionamento:
Exemplo da função DrawLine com os vértices nas seguintes coordenadas:
```
	Vertice v1(100, 50, 255, 0, 0, 255);
	Vertice v2(200, 400, 0, 255, 0, 255);
```

![2oct](https://user-images.githubusercontent.com/31492509/52719463-749ac780-2f84-11e9-867e-a5e5b8277711.jpeg)


**3° e 7° OCTANTE**

Para renderizar retas nestes octantes a abordagem foi semelhante ao 2° e 6° octante, porém com algumas diferenças, uma delas foi que agora nós vamos decrementar a posição y no momento de construir a reta.
``` 
	if (d <= 0){
		d += incr_e;
		aux.setPosY(aux.getPosY()-1);
	} else{
		d += incr_ne;
		aux.setPosX(aux.getPosX()+1);
		aux.setPosY(aux.getPosY()-1);
	}
```
###### Exemplo de funcionamento:
Exemplo da função DrawLine com os vértices nas seguintes coordenadas:
```
	Vertice v1(100, 400, 255, 0, 0, 255);
	Vertice v2(200, 50, 0, 255, 0, 255);
```

![7oct](https://user-images.githubusercontent.com/31492509/52719472-7795b800-2f84-11e9-922c-a7e1a906f8e6.jpeg)


**4° e 8° OCTANTE**

Para renderizar retas nestes octantes a abordagem foi semelhante ao 1° e 5° octante, porém com algumas diferenças, uma delas foi que agora nós vamos decrementar a posição y no momento de construir a reta, pois a reta está crescendo em direção ao eixo y negativo.
```
	if (d >= 0){
		d += incr_e;
		aux.setPosX(aux.getPosX()+1);
	} else{
		d += incr_ne;
		aux.setPosX(aux.getPosX()+1);
		aux.setPosY(aux.getPosY()-1);
	}
```
###### Exemplo de funcionamento:
Exemplo da função DrawLine com os vértices nas seguintes coordenadas:
```
	Vertice v1(50, 200, 255, 0, 0, 255);
	Vertice v2(400, 100, 0, 255, 0, 255);
```

![8oct](https://user-images.githubusercontent.com/31492509/52719948-6ef1b180-2f85-11e9-96d8-52577ddb29fd.jpeg)

**Δx = 0**

Alguns outros casos que não foram considerados até agora devem ser tratados, como por exemplo quando o deslocamento no eixo x (Δx) for igual a zero, ou seja, a reta está fixa e vai variar apenas no eixo y, devemos verificar o deslocamento em y para ver em qual direção devemos rasterizar.
```
	if(dy > 0){
		while(aux.getPosY() < pixel2.getPosY()){
			Interpolacao(aux, pixel1, pixel2);
			aux.setPosY(aux.getPosY()+1);
		}	
	} else if(dy < 0){
		while(aux.getPosY() > pixel2.getPosY()){
			Interpolacao(aux, pixel1, pixel2);
			aux.setPosY(aux.getPosY()-1);
		}
	}
```
###### Exemplo de funcionamento:
Exemplo da função DrawLine com os vértices nas seguintes coordenadas:
```
	Vertice v1(400, 100, 255, 0, 0, 255);
	Vertice v2(400, 200, 0, 255, 0, 255);
```

![dx0](https://user-images.githubusercontent.com/31492509/52742037-9bbdbd00-2fb5-11e9-9c37-3587c833e1e7.jpeg)

**Δy = 0**

Como somente o eixo x vai variar, e já existe a condição para inverter os vértices no eixo x, essa condição específica foi bem simples de implementar.
```
	while(aux.getPosX() < pixel2.getPosX()){
		Interpolacao(aux, pixel1, pixel2);
		aux.setPosX(aux.getPosX()+1);
	}
```
###### Exemplo de funcionamento:
Exemplo da função DrawLine com os vértices nas seguintes coordenadas:
```
	Vertice v1(400, 100, 255, 0, 0, 255);
	Vertice v2(400, 200, 0, 255, 0, 255);
```
![dy0](https://user-images.githubusercontent.com/31492509/52742040-9ceeea00-2fb5-11e9-9ff3-b17b27135d6c.jpeg)

**Δy = Δx**

Quando ambos os deslocamentos em x e em y são iguais significa que o crescimento de ambos será semelhante, logo sempre vamos colorir o píxel na diagonal, ou seja, crescer em ambos os eixos.
```
	while(aux.getPosX() < pixel2.getPosX()){
		Interpolacao(aux, pixel1, pixel2);
		aux.setPosX(aux.getPosX()+1);
		aux.setPosY(aux.getPosY()+1);
	}
``` 
###### Exemplo de funcionamento:
Exemplo da função DrawLine com os vértices nas seguintes coordenadas:
```
	Vertice v1(100, 300, 255, 0, 0, 255);
	Vertice v2(200, 400, 0, 255, 0, 255);
```
![dx dy](https://user-images.githubusercontent.com/31492509/52742046-a11b0780-2fb5-11e9-8d8c-d13331a62ad8.jpeg)

**Interpolação**

A interpolação linear das cores faz com que a rasterização de uma linha ela comece com cor do pixel inicial, e termine com a cor do pixel final. Fazendo um degradê bem suavizado. Para realizar o degradê é necessário mudar o valor do pixel a ser pintado. Essa mudança é realizada através de um cálculo simples: Primeiramente pegamos o valor da diferença da cor do pixel final e do inicial (I). No segundo passo pegamos a diferença do ponto atual ao ponto inicial (II). O terceiro passo calculamos o valor a ser incrementado a cor, esse cálculo é feito através da divisão do resultado do passo I com comprimento da linha, esse comprimento pode ser dx ou dy (III). E o quarto e último passo, é feito o cálculo para saber qual a cor que será pintada no pixel atual (IV). Todos os quatro passos são feitos para as três cores (RED, GREEN, BLUE) e o alfa. 

```
	dCor[0] = pixel2.getR() - pixel1.getR();	//    
	dCor[1] = pixel2.getG() - pixel1.getG();	//      (I)	
	dCor[2] = pixel2.getB() - pixel1.getB();	//
	dCor[3] = pixel2.getA() - pixel1.getA();	//

	if(dx == 0 || (abs(dy) > abs(dx))){
		variacao = aux.getPosY() - pixel1.getPosY();  // (II)
		for (int i = 0; i < 4; i++)
		{
			incCor[i] = dCor[i]/dy;		//	(III)
		}
	} else if((abs(dx) == abs(dy) ) || (abs(dx) > abs(dy)) ){
		variacao = aux.getPosX() - pixel1.getPosX();  // (II)
		for (int i = 0; i < 4; i++)
		{
			incCor[i] = dCor[i]/dx;		//	(III)
		}
	}

	inter.setR(pixel1.getR() + incCor[0]*variacao);	//
	inter.setG(pixel1.getG() + incCor[1]*variacao);	//	(IV)
	inter.setB(pixel1.getB() + incCor[2]*variacao);	//
	inter.setA(pixel1.getA() + incCor[3]*variacao);	//


```
Inicialmente utilizamos as variáveis dCor e incCor como sendo do tipo **_int_**, e tivemos o problema que não havia a realização do degradê, como visto na imagem a seguir:

![int](https://user-images.githubusercontent.com/31492509/52742165-e7706680-2fb5-11e9-9795-9ea6b91ac81e.jpeg)


Para consertar esse problema foi bem simples, apenas trocamos o tipo dessas variáveis para o tipo **_float_**, com isso obtivemos um degradê bem suavizado, conforme a imagem a seguir: 

![float](https://user-images.githubusercontent.com/31492509/52742030-99f3f980-2fb5-11e9-8a84-f06665daaee5.jpeg)

### DrawTriangle
Esta função desenha as arestas de um triângulo, recebendo como parâmetro três vértices, essa função consiste em chamar a função DrawLine três vezes, assim formando o desenho do triângulo desejado.

```
	void DrawTriangle(Vertice pixel1, Vertice pixel2, Vertice pixel3){
		DrawLine(pixel1, pixel2);
		DrawLine(pixel2, pixel3);
		DrawLine(pixel1, pixel3);
	}
```
###### Exemplo de funcionamento:
Exemplo da função DrawTriangle com os vértices nas seguintes coordenadas:
```
	Vertice v1(IMAGE_WIDTH/2 -150, IMAGE_HEIGHT/2 + 150, 255, 0, 0, 255);
	Vertice v2(IMAGE_WIDTH/2, IMAGE_HEIGHT/2 -150, 0, 255, 0, 255);
	Vertice v3(IMAGE_WIDTH/2 +150, IMAGE_HEIGHT/2 + 150, 0, 0, 255, 255);
```

![triangulo](https://user-images.githubusercontent.com/31492509/52742056-a5dfbb80-2fb5-11e9-9b0b-76341f8141de.jpeg)

### Considerações:
Ficamos satisfeitos com o resultado final da atividade, foi muito interessante trabalhar com computação gráfica pois a saída pode ser facilmente observada e contemplada, e também entender um pouco de como nossas telas funcionam, como cada bit é colorido de modo a gerar uma cena maior.
As maiores dificuldades foram em converter o algoritmo de Bresenham do primeiro octante para todos os outros, foi necessário muita pesquisa teórica antes de partir para programação.
O código com certeza pode ser refinado, acreditamos que com o tempo podemos otimizar mais o nosso código, fazendo algumas funções para
reduzir linhas de código e remover trechos redundantes.

### Referências:
Algumas fontes que nos auxiliaram durante o desenvolvimento do trabalho;
https://www.ntu.edu.sg/home/ehchua/programming/opengl/cg_basicstheory.html

https://johannesca.github.io/cg_t1/

http://letslearnbits.blogspot.com/2014/10/icgt1-interpolacao-de-cores.html

http://fleigfleig.blogspot.com/2016/08/interpolacao-de-cores-e-triangulos.html

