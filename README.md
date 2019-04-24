
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



# ICG - Implementação de um pipeline gráfico com linguagem C++
Aqui temos uma breve explicação sobre a realização da segunda atividade da disciplina Introdução a computação gráfica, ministrada pelo professor Christan Azambuja Pagot. O objetivo da atividade era implementar um pipeline gráfico que converte os vértices desde o espaço do objeto, até o espaço de tela, e posteriormente podemos desenhar as primitivas utilizando as funções que construímos na primeira etapa.

### Entendendo o pipeline gráfico
O pipeline gráfico, é um modelo conceitual que descreve quais etapas um sistema gráfico precisa executar para renderizar uma cena 3D para uma tela 2D. Uma vez que um modelo 3D tenha sido criado, por exemplo, em um videogame ou qualquer outra animação 3D, o pipeline gráfico é o processo de transformar esse modelo 3D no que o computador exibe na tela. Ao todo são 5 transformações, como pode ser visto na figura abaixo. As transformações de um espaço para outro são feitas através de matrizes, que multiplicam os vértices do objeto, as matrizes podem ser combinadas para otimizar este processo. O trabalho foi feito em línguagem C++ e utilizamos a biblioteca glm para operações matemáticas envolvendo matrizes e vetores.

![p1](https://user-images.githubusercontent.com/31492509/56624879-1ab41f00-6611-11e9-8cbb-1f28a1e1dfdb.png)

### 1. Transformação: Espaço do Objeto → Espaço do Universo
A descrição do modelo é feita no sistema de coordenadas do objeto, ou simplismente espaço do objeto. O objeto é descrito no centro do espaço de coordenadas. No próximo espaço, o espaço de mundo, ou espaço de universo, os objetos estão organizados em uma cena com outros objetos e podem estar deslocados da origem. A matriz que realiza esta transformação é denominada matriz de modelagem (ou modeling matrix). A matriz de modelagem é composta por uma sequência de transformações geométricas que posicionam o modelo no universo. As transformações que podem compor a matriz de modelagem são a rotação, translação, escala e o cisalhamento (shear). No nosso pipeline a transformação que faremos é de uma translação no eixo y, a matriz de translação é multiplicada da matriz identidade formando a nossa model matrix.

![2](https://user-images.githubusercontent.com/31492509/56624875-1982f200-6611-11e9-8f46-03cbfd66981c.png)

Trecho do código que descreve a matriz de modelagem:
```
	// MODEL MATRIX (ESP. OBJETO -> ESP. UNIV):
	glm::mat4 i4 = glm::mat4( // matriz identsidade 4x4
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1));
	// Transformação aplicada -> rotação no eixo y
	glm::mat4 rotation = glm::mat4(
		glm::vec4(cos(rotation_angle), 0, -sin(rotation_angle), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(sin(rotation_angle), 0, cos(rotation_angle), 0),
		glm::vec4(0, 0, 0, 1));

	glm::mat4 modeling_matrix = i4 * rotation; // vamos aplicar uma rotação no objeto.
  ```
### 2. Transformação: Espaço do Universo → Espaço da Câmera
Esta etapa do pipeline é responsável por transformar vértices do espaço do universo para o espaço da câmera. A "camêra" é o ponto de vista da cena, a camêra pode ser posicionada em várias posições durante uma cena, para fazer esta transformação vamos realizar uma mudança de base, ou seja, transformar as coordenadas para um novo sistema. Este novo sistema é definido através de três informações:
- A posição da camêra, que indica onde a camera está posicionada no espaço de universo.
- A direção da camêra, que mostra para onde a camera está apontando.
- O vetor "up", que indica a parte de cima da camêra, nos dando a posição correta no eixo.
A matriz que realiza esta mudaná é chama de matriz de visualização, ou view matrix. Ela é composta por uma translação e uma rotação, seus valores podem ser definidos por operações matemáticas a partir dos três valores de entrada descritos acima.

![3](https://user-images.githubusercontent.com/31492509/56625247-d0cc3880-6612-11e9-9e02-d3712026b545.png)

Trecho do código que descreve a matriz de visualização:
```
	// VIEW MATRIX (ESP. UNIV -> ESP. CAMERA):
	glm::vec3 pos = glm::vec3(0, 0, 5);
	glm::vec3 lookat = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);

	//Base ortonormal da camera, coordenadas x y e z.
	glm::vec3 zc = -(lookat - pos) / glm::l1Norm(lookat - pos);
	glm::vec3 xc = glm::cross(up, zc) / glm::l1Norm(glm::cross(up, zc));
	glm::vec3 yc = glm::cross(zc, xc);

	glm::mat4 c_b = glm::mat4(
		glm::vec4(xc, 0),
		glm::vec4(yc, 0),
		glm::vec4(zc, 0),
		glm::vec4(0, 0, 0, 1));

	glm::mat4 c_t = glm::mat4(
		glm::vec4(1, 0, 0, -pos.x), // matriz T para formar a view matriz (fórmula no slide)
		glm::vec4(0, 1, 0, -pos.y),
		glm::vec4(0, 0, 1, -pos.z),
		glm::vec4(0, 0, 0, 1));

	glm::mat4 view_matrix = c_b * c_t;
```
### 3. Transformação: Espaço da Câmera → Espaço de Recorte
Esta etapa do pipeline é responsável por transformar vértices do espaço da câmera para o espaço de recorte (ou projetivo). Aqui podemos adicionar a distorção perpectiva, que faz com que objetos distantes da camêra sofram uma deformação aparentando serem menores e objetos próximos à camêra aparentam ser maiores. É o efeito que simula como nosso cérebro interpreta objetos distantes, você pode observar esta distorção em rodovias onde as pontas do acostamento parecem se tocar no "infinito".

![GO-118-trecho-Divisa-GO-DF-Campos-Belos](https://user-images.githubusercontent.com/31492509/56627143-f3624f80-661a-11e9-9f9b-98c8d22d480c.jpg)

A matriz que realiza esta transformação é chamada de matriz de projeção, ou projection matrix, ela é definida a partir da distância da camêra para o plano de visualização que chamamos geralmente de "d". Aqui nossa coordenada homogênea, que vou entrar em mais detalhes a frente, provavelmente terá seu valor alterado de 1.

![5](https://user-images.githubusercontent.com/31492509/56627139-f3624f80-661a-11e9-8d8d-3b32ce5ac447.png)

Trecho do código que descreve a matriz de projeção:
```
	// PROJECTION MATRIX ESP. CAMERA -> ESP. DE RECORTE (CLIPING SPACE)
	double d = 3; // distância do centro de projeção para o viewplane
	glm::mat4 projection_matrix = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, d),
		glm::vec4(0, 0, -1 / d, 0));
	// matriz model view proj, que leva do espaço de objeto para o esp. de recorte
	glm::mat4 mvp = modeling_matrix * view_matrix * projection_matrix;
```

### 4. Transformação: Espaço de Recorte → Espaço “Canônico"
Esta etapa do pipeline é responsável por transformar pontos do espaço de recorte para o espaço canônico. Isto é feito em duas etapas: 
Primeiro, vamos dividir as coordenadas dos vértices no espaço de recorte pela sua coordenada homogênea, ou seja, o quarto valor do vetor. Esta transformação gera uma alteração da geometria da cena, tornando menores os objetos que estiverem mais distantes da câmera.
```
	//Divide pela coordenada homogênea
	for (unsigned int i = 0; i < objeto.size(); i++) {
		objeto[i] = objeto[i] / objeto[i].w;
	}
```
E vamos aplicar a combinação das três matrizes definidas nos passos 1, 2 e 3 nos vértices, transformando-os para o espaço canônico, onde o hexahedro anterior se torna um cubo de coordenadas unitárias e centrado na origem.
``` 
	// aplicação da matriz mvp
	for (unsigned int i = 0; i < objeto.size(); i++) {
		objeto[i] = objeto[i] * mvp;
	}
```

![6](https://user-images.githubusercontent.com/31492509/56627141-f3624f80-661a-11e9-8994-30b211f12455.png)

### 5. Transformação: Espaço Canônico → Espaço de Tela"
Por fim, vamos transformar as coordenadas no espaço canônico para o espaço da tela, já falamos sobre o espaço na tela no primeiro trabalho da disciplina, mas sabemos que ele é bidimensional, ou seja, temos apenas coordenadas x e y. 

![7](https://user-images.githubusercontent.com/31492509/56627142-f3624f80-661a-11e9-908f-c80ea3f6b716.png)


A matriz que realiza esta transformação é chamada de ViewPort, e pode ser quebrada em três partes.
- Primeiro vamos inverter o eixo y de coordenadas, pois sabemos que na tela o eixo y parte de cima para baixo.
```
	// inverte o eixo y
	glm::mat4 s1 = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, -1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1));
```

- Depois vamos fazer uma escala nos eixos x e y para que se adequem as coordenadas da tela em questão, por isso recebe como entrada valores w (width) e h (heigth) que são os valores de largura e altura da tela respectivamente.
```
	// ajusta o tamanho para a tela (escala)
	glm::mat4 t = glm::mat4(
		glm::vec4(width / 2, 0, 0, 0),
		glm::vec4(0, height / 2, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(1, 1, 0, 1));
```
- E vamos por fim transladar o nosso bloco para que existam apenas valores "positivos" diferente do espaço canônico onde o centro de projeção era na origem.
```
	// translação para a parte "positiva"
	glm::mat4 s2 = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4((width - 1) / 2, (height - 1) / 2, 0, 1));
```
A matriz viewport de fato é a combinação destas três matrizes, e podemos aplicar nos vértices, encerrando o pipeline gráfico.
```
	glm::mat4 viewport_matrix = s2 * t * s1;

	// aplica a viewport nos vértices
	for (unsigned int i = 0; i < objeto.size(); i++)
	{
		objeto[i] = round(viewport_matrix * objeto[i]);
	}
```
Essas dimensões são configuráveis para cada tela, no nosso trabalho a janela possui dimensão 512x512, ou seja o valor de largura e altura eram iguais, ambos 512.

![8](https://user-images.githubusercontent.com/31492509/56627153-fd844e00-661a-11e9-9995-33a0ccadf593.png)

### 6. Rasterização
Esta etapa gera a rasterização dos modelos no espaço de tela, ou seja, "desenha" a cena na tela de fato. Nós já fizemos um algoritmo de rasterização na primeira etapa do trabalho, então reaproveitamos o código aqui, foi necessário apenas pegar os valores de coordenadas geradas pelo pipeline, criar um objeto vértice com tais coordenadas e rasterizar as primitivas (triângulos) na tela.
Uma coisa importante é limpar o buffer do objeto, se não as rasterizações iriam se sobrepor.
```
	// limpa o buffer
	memset(FBptr, 0, width*height * 4);

	// rasteriza na tela, usando a função do primeiro trabalho
	for (unsigned int i = 0; i < objeto.size(); i += 3) {

		Vertice v1(objeto[i][0], objeto[i][1], 255, 255, 255, 255);
		Vertice v2(objeto[i + 1][0], objeto[i + 1][1], 255, 255, 255, 255);
		Vertice v3(objeto[i + 2][0], objeto[i + 2][1], 255, 255, 255, 255);

		DrawTriangle(v1, v2, v3);
	}
```

### Coordenadas homogêneas
Você deve ter observado que durante nosso pipeline utilizamos matrizes de dimensão 4x4, mesmo trabalhando com objetos 3d (3x3) isso acontence pois utilizamos as chamadas coordenadas homogêneas, que nos dão diversas vantagens, por exemplo, não conseguimos representar a transformação de rotação 3d em uma matriz 3x3, pois não ficaria linear, mas se utilizarmos coordenadas homogêneas isso se torna possível, geralmente a coordenada homogênea tem valor 1, mas durante o pipeline esse valor pode ser alterado.

![ch](https://user-images.githubusercontent.com/31492509/56627154-fd844e00-661a-11e9-8b82-a252f6ff0342.png)

### Resultados
Para verificar os resultados do nosso pipeline, utilizamos uma biblioteca do professor que carrega modelos 3d, o modelo escolhido foi a cabeça de um macaco. Os valores foram lidos através de uma função e armazenados em um vetor de coordenadas, e após passar por nosso pipeline e rasterizar na tela temos o seguinte resultado:

![WhatsApp Image 2019-04-23 at 23 05 18](https://user-images.githubusercontent.com/31492509/56627530-936ca880-661c-11e9-860c-59fca392af34.jpeg)

Para comparar e verificar realmente o funcionamento do pipeline, fizemos a mesma rasterização do modelo 3 do macaco só que utilizando um código em OpenGL, e ambos sistemas geraram os mesmos resultados.

![WhatsApp Image 2019-04-23 at 12 59 04](https://user-images.githubusercontent.com/31492509/56627298-92874700-661b-11e9-83ce-38a77c32da0a.jpeg)

### Considerações:
Ficamos satisfeitos com o resultado final da atividade, é muito gratificante pegar todo esse conhecimento teórico aplicar na prática e ver que realmente funciona, as maiores dificuldades que tivemos no trabalho não foi a implementação do pipeline em si, mas sim nas estruturas de dados que iriamos utilizar para salvar os valores do objeto 3d e na utilização da biblioteca GLM.

### Referências:
Algumas fontes que nos auxiliaram durante o desenvolvimento do trabalho;

Fundamentals of Computer Graphics - Peter Shirley, Michael Ashikhmin, Steve Marschner

Notas de Aula do Prof. Christian

https://en.wikipedia.org/wiki/Graphics_pipeline







