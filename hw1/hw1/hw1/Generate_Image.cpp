#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<Windows.h>


#define MAX_W 512
#define MAX_H 512

void main() {
    unsigned char Image[MAX_W][MAX_H] = { 0 };
    FILE* rawfile;
    rawfile = fopen("512x512.raw", "wb");

    //=================================================
    //=======================1번=======================
    for (int i = 0; i < MAX_W; i++) // 배열 시작은 0부터
    {
        for (int j = 0; j < 100; j++)
        {
            Image[i][j] = (char)120;
        }// 100까지

        for (int j = 100; j < 200; j++)
        {
            Image[i][j] = (char)((0.15) * j + 105);
        }//200까지

        for (int j = 200; j < 280; j++)
        {
            Image[i][j] = (char)((1.125) * j -90);
        }//280까지

        for (int j = 280; j < 300; j++)
        {
            Image[i][j] = (char)((0.75) * j + 15);
        }//300까지

        for (int j = 300; j < 512; j++)
        {
            Image[i][j] = (char)240;
        }//512까지
    }


    fwrite(Image, sizeof(char), 512 * 512, rawfile);
    fclose(rawfile);


    //=================================================
    //=======================2번=======================

    FILE* infile;
    FILE* rawfile2;
    FILE* outfile1;
    FILE* outfile2;

    infile = fopen("lena_bmp_512x512_new.bmp", "rb");   //헤더 정보 읽어올 bmp 파일 
    rawfile2 = fopen("512x512.raw", "rb");  //1번 결과 row파일
    outfile1 = fopen("512x512_not_rotate.bmp", "wb");   //뒤집기 전
    outfile2 = fopen("512x512_rotate.bmp", "wb");   //뒤집은 후

    BITMAPFILEHEADER hFile; 
    BITMAPINFOHEADER hInfo; 
    RGBQUAD hRGB[256];

    fread(&hFile, sizeof(BITMAPFILEHEADER), 1, infile);
    fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);
    fread(hRGB, sizeof(RGBQUAD), 256, infile);



    BYTE* lpImg = new BYTE[hInfo.biSizeImage];
    fread(lpImg, sizeof(char), hInfo.biSizeImage, rawfile2);

    fwrite(&hFile, sizeof(char), sizeof(BITMAPFILEHEADER), outfile1); // 파일 정보
    fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile1); // 영상 정보
    fwrite(hRGB, sizeof(RGBQUAD), 256, outfile1); // 팔레트 RGBQUAD size x 256가지
    fwrite(lpImg, sizeof(char), hInfo.biSizeImage, outfile1); //영상 Raw 데이터

    //=================================================
    //=======================3번=======================

    unsigned char temp = 0;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 512; j++)
        {
            temp = lpImg[(i * 512) + j];
            lpImg[(i * 512) + j] = lpImg[((511 * 512) - (i * 512)) + j];
            lpImg[((511 * 512) - (i * 512)) + j] = temp;
        }
    }

    fwrite(&hFile, sizeof(char), sizeof(BITMAPFILEHEADER), outfile2); // 파일 정보
    fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile2); // 영상 정보
    fwrite(hRGB, sizeof(RGBQUAD), 256, outfile2); // 팔레트 RGBQUAD size x 256가지
    fwrite(lpImg, sizeof(char), hInfo.biSizeImage, outfile2); //영상 Raw 데이터


}