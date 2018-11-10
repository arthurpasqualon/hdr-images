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
float c = 0.8;//por escala
float gama = 2.2;//gama
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

void forma(posI){
    if(modo==SCALE){

    float r = image[posI].r * exposure;
    float rt = r/(r+c);
    image8[posI].r = fmin(1.0, rt) * 255;

    r =  image[posI].g * exposure;
    rt = r/(r+c);
    image8[posI].g = fmin(1.0, rt) * 255;

    r =  image[posI].b * exposure;
    rt = r/(r+c);
    image8[posI].b = fmin(1.0, rt) * 255;

    }else{ // == gama
        float r = image[posI].r * exposure;
        image8[posI].r = fmin(1.0,pow(r,(1/gama))) * 255;

        float g = image[posI].g * exposure;
        image8[posI].g = fmin(1.0,pow(g,(1/gama))) * 255;

        float b = image[posI].b * exposure;
        image8[posI].b = fmin(1.0,pow(b,(1/gama))) * 255;
    }
}
void process()
{
    printf("Exposure: %.3f\n", exposure);

    int pos;
    for(pos=0; pos<sizeX*sizeY; pos++) {
        forma(pos);
    }

    // NÃO ALTERAR A PARTIR DAQUI!!!!
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
    }

    init(argc,argv);

    printf("%d x %d\n", sizeX, sizeY);

    exposure = 1.0f; // exposição inicial

    // Aplica processamento inicial
    process();

    // Não retorna... a partir daqui, interação via teclado e mouse apenas, na janela gráfica
    glutMainLoop();
    return 0;
}

