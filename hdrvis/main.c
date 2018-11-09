#include <math.h>
#include <string.h>		// para usar strings
// Rotinas para acesso da OpenGL
#include "opengl.h"
// Rotinas para leitura de arquivos .hdr
#include "rgbe.h"
// Variáveis globais a serem utilizadas:

// Dimensões da imagem de entrada
int sizeX, sizeY;
int width, height;

// Imagem de entrada
RGBf* image;

// Imagem de saída
RGB8* image8;

// Fator de exposição inicial
float exposure = 1.0;

// Modo de exibição atual
int modo;

// Função pow mais eficiente (cerca de 7x mais rápida)
float fastpow(float a, float b);
float fastpow(float a, float b) {
     union { float f; int i; }
      u = { a };
      u.i = (int)(b * (u.i - 1065307417) + 1065307417);
      return u.f;
}

// Função principal de processamento: ela deve chamar outras funções
// quando for necessário (ex: algoritmos de tone mapping, etc)
void process()
{
    //escole a correção com o T E G
    printf("Exposure: %.3f\n", exposure);
    //
    // EXEMPLO: preenche a imagem com pixels cor de laranja...
    //
    //
    // SUBSTITUA este código pelos algoritmos a serem implementados
    //
    if(modo==SCALE){

    }else{ // == gama

    }
    for(int posI=0; posI<sizeX*sizeY; posI++) {
         image8[posI].r = (unsigned char) fmin(1.0,image[posI].r)*255;
         image8[posI].g = (unsigned char) fmin(1.0,image[posI].g)*255;
         image8[posI].b = (unsigned char) fmin(1.0,image[posI].b)*255;
    }

    int pos;
    for(pos=0; pos<sizeX*sizeY; pos++) {
        image8[pos].r = (unsigned char) (image[pos].r * exposure);
        image8[pos].g = (unsigned char) (image[pos].g * exposure);
        image8[pos].b = (unsigned char) (image[pos].b * exposure);
    }

    //
    // NÃO ALTERAR A PARTIR DAQUI!!!!
    //
    buildTex();
}

int main(int argc, char** argv)
{
    FILE* arq = fopen(argv[1],"rb");
    RGBE_ReadHeader(arq, &sizeX, &sizeY, NULL);

    // Aloca imagem float
    image = (RGBf *)malloc(sizeof(RGBf) * sizeX * sizeY);
     // Aloca memória para imagem de 24 bits
    image8 = (RGB8*) malloc(sizeof(RGB8) * sizeX * sizeY);
    int result = RGBE_ReadPixels_RLE(arq, (float*)image, sizeX, sizeY);
    if (result == RGBE_RETURN_FAILURE) {
       printf("erro");
    }
    fclose(arq);
    if(argc==1) {
        printf("hdrvis [image file.hdr]\n");
        exit(1);
    }//tentativa de carregar a imagem, mas não rolou
    // Inicialização da janela gráfica
    init(argc,argv);

    //
    // INCLUA aqui o código para LER a imagem de entrada
    //
    // Siga as orientações no enunciado para:
    //
    // 1. Descobrir o tamanho da imagem (ler header)
    // 2. Ler os pixels
    //

    // TESTE: cria uma imagem de 800x600
   /* sizeX = 800;
    sizeY = 600;
*/
    printf("%d x %d\n", sizeX, sizeY);

    exposure = 1.0f; // exposição inicial

    // Aplica processamento inicial
    process();

    // Não retorna... a partir daqui, interação via teclado e mouse apenas, na janela gráfica
    glutMainLoop();
    return 0;
}

