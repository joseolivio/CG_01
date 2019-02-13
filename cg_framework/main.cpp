#include "main.h"


Vertice v1(IMAGE_WIDTH/2 -150, IMAGE_HEIGHT/2 + 150, 255, 0, 0, 255);
Vertice v2(IMAGE_WIDTH/2, IMAGE_HEIGHT/2 -150, 0, 255, 0, 255);
Vertice v3(IMAGE_WIDTH/2 +150, IMAGE_HEIGHT/2 + 150, 0, 0, 255, 255);

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
//*************************************************************************
// Chame aqui as funções do mygl.h
//*************************************************************************

	/*for (unsigned int i=0; i<250; i++)
	{
		teste.setPosX(i);
		PutPixel(teste);
	} */
	//DrawLine(v1, v2);
	DrawTriangle(v1, v2, v3);
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;	

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}

