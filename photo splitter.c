#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>    // fopen, fread, fseek, fprintf, fclose 함수가 선언된 헤더 파일
#include <stdlib.h>   // malloc, free 함수가 선언된 헤더 파일

#pragma pack(push, 1)                // 구조체를 1바이트 크기로 정렬

typedef struct _BITMAPFILEHEADER     // BMP 비트맵 파일 헤더 구조체
{
    unsigned short bfType;           // BMP 파일 매직 넘버
    unsigned int   bfSize;           // 파일 크기
    unsigned short bfReserved1;      // 예약
    unsigned short bfReserved2;      // 예약
    unsigned int   bfOffBits;        // 비트맵 데이터의 시작 위치
} BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER     // BMP 비트맵 정보 헤더 구조체(DIB 헤더)
{
    unsigned int   biSize;           // 현재 구조체의 크기
    int            biWidth;          // 비트맵 이미지의 가로 크기
    int            biHeight;         // 비트맵 이미지의 세로 크기
    unsigned short biPlanes;         // 사용하는 색상판의 수
    unsigned short biBitCount;       // 픽셀 하나를 표현하는 비트 수
    unsigned int   biCompression;    // 압축 방식
    unsigned int   biSizeImage;      // 비트맵 이미지의 픽셀 데이터 크기
    int            biXPelsPerMeter;  // 그림의 가로 해상도(미터당 픽셀)
    int            biYPelsPerMeter;  // 그림의 세로 해상도(미터당 픽셀)
    unsigned int   biClrUsed;        // 색상 테이블에서 실제 사용되는 색상 수
    unsigned int   biClrImportant;   // 비트맵을 표현하기 위해 필요한 색상 인덱스 수
} BITMAPINFOHEADER;

typedef struct _RGBTRIPLE            // 24비트 비트맵 이미지의 픽셀 구조체
{
    unsigned char rgbtBlue;          // 파랑
    unsigned char rgbtGreen;         // 초록
    unsigned char rgbtRed;           // 빨강
} RGBTRIPLE;

void WriteBitmapFile(BITMAPFILEHEADER fileheader, BITMAPINFOHEADER infoheader, char* output, int imgSize, char* filename)
{
	FILE* fp = fopen(filename, "wb");//파일을 이진쓰기모드로 열기

	fwrite(&fileheader, sizeof(BITMAPFILEHEADER), 1, fp);//비트맵파일헤더쓰기
	fwrite(&infoheader, sizeof(BITMAPINFOHEADER), 1, fp);//비트맵인포헤더쓰기
	fwrite(output, sizeof(char), imgSize, fp);//이미지인덱스정보 쓰기
	fclose(fp);
}

#define PIXEL_SIZE   3    // 픽셀 한 개의 크기 3바이트(24비트)
#define PIXEL_ALIGN  4    // 픽셀 데이터 가로 한 줄은 4의 배수 크기로 저장됨

int main()
{
    FILE *fpBmp;                    // 비트맵 파일 포인터
    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;

    unsigned char *image;    // 픽셀 데이터 포인터
    int size;                // 픽셀 데이터 크기
    int width, height;       // 비트맵 이미지의 가로, 세로 크기
    int padding;             // 픽셀 데이터의 가로 크기가 4의 배수가 아닐 때 남는 공간의 크기

    fpBmp = fopen("methematical notation.bmp", "rb");    // 비트맵 파일을 바이너리 모드로 열기
    if (fpBmp == NULL)    // 파일 열기에 실패하면
        return 0;         // 프로그램 종료

    // 비트맵 파일 헤더 읽기. 읽기에 실패하면 파일 포인터를 닫고 프로그램 종료
    if (fread(&fileheader, sizeof(BITMAPFILEHEADER), 1, fpBmp) < 1)
    {
        fclose(fpBmp);
        return 0;
    }

    // 매직 넘버가 MB가 맞는지 확인(2바이트 크기의 BM을 리틀 엔디언으로 읽었으므로 MB가 됨)
    // 매직 넘버가 맞지 않으면 프로그램 종료
    if (fileheader.bfType != 'MB')
    {
        fclose(fpBmp);
        return 0;
    }

    // 비트맵 정보 헤더 읽기. 읽기에 실패하면 파일 포인터를 닫고 프로그램 종료
    if (fread(&infoheader, sizeof(BITMAPINFOHEADER), 1, fpBmp) < 1)
    {
        fclose(fpBmp);
        return 0;
    }

    // 24비트 비트맵이 아니면 프로그램 종료
    if (infoheader.biBitCount != 24)
    {
        fclose(fpBmp);
        return 0;
    }

    size = infoheader.biSizeImage;    // 픽셀 데이터 크기
    width = infoheader.biWidth;       // 비트맵 이미지의 가로 크기
    height = infoheader.biHeight;     // 비트맵 이미지의 세로 크기

    // 이미지의 가로 크기에 픽셀 크기를 곱하여 가로 한 줄의 크기를 구하고 4로 나머지를 구함
    // 그리고 4에서 나머지를 빼주면 남는 공간을 구할 수 있음.
    // 만약 남는 공간이 0이라면 최종 결과가 4가 되므로 여기서 다시 4로 나머지를 구함
    padding = (PIXEL_ALIGN - ((width * PIXEL_SIZE) % PIXEL_ALIGN)) % PIXEL_ALIGN;

    if (size == 0)    // 픽셀 데이터 크기가 0이라면
    {
        // 이미지의 가로 크기 * 픽셀 크기에 남는 공간을 더해주면 완전한 가로 한 줄 크기가 나옴
        // 여기에 이미지의 세로 크기를 곱해주면 픽셀 데이터의 크기를 구할 수 있음
        size = (width * PIXEL_SIZE + padding) * height;
    }

    image = malloc(size);    // 픽셀 데이터의 크기만큼 동적 메모리 할당

    // 파일 포인터를 픽셀 데이터의 시작 위치로 이동
    fseek(fpBmp, fileheader.bfOffBits, SEEK_SET);

    // 파일에서 픽셀 데이터 크기만큼 읽음. 읽기에 실패하면 파일 포인터를 닫고 프로그램 종료
    if (fread(image, size, 1, fpBmp) < 1)
    {
        fclose(fpBmp);
        return 0;
    }

    fclose(fpBmp);    // 비트맵 파일 닫기
	
	char* output = (char *)malloc(sizeof(char) * size);
    // 픽셀 데이터는 아래 위가 뒤집혀서 저장되므로 아래쪽부터 반복
    // 세로 크기만큼 반복
    int x, y, tfx=0, tbx=0, blank, cnt=1;
    
    while(1) {
	    blank=1;
	    for(x=tbx; x<width; x++) {
	    	int flag=0;  //0이면 수학 기호가 포함되어 있다 
	    	for(y=height-1; y>=0; y--) {
	    		int index = (x * PIXEL_SIZE) + (y * (width * PIXEL_SIZE)) + (padding * y);  //픽셀 위치 
	    		
	    		RGBTRIPLE *pixel = (RGBTRIPLE *)&image[index];  //픽셀 위치 주소 
	    		unsigned char blue = pixel->rgbtBlue;  //픽셀 위치 주소에 있는 rgb값 불러오기 
	    		unsigned char green = pixel->rgbtGreen;
	    		unsigned char red = pixel->rgbtRed;
	    		
	    		unsigned char gray = (red + green + blue) / PIXEL_SIZE;  //검은색 정도 계산 
	    		
	    		if(gray<=100)  //100이하이면 검은색으로 판 
	    			flag=1;
			}
			if(blank && flag) {  //여백이 있으면서 기호가 나오면 여백 blank=0으로 설정 
				blank=0;
				continue;
			}
			if(!blank && !flag) {  //여백이 없으면서 기호가 사라지면 기호 좌표 tfx, tbx 계산 
				tfx=tbx;
				tbx=x;
				break;
			}
		}
		if(x>=width)
			break;
		char name[15]="math sign1.bmp";  //사진 이름 
		name[9]=cnt+'0';
		for(x=0; x<width; x++) {  //사진 자르 
			for(y=height-1; y>=0; y--) {
				if(tfx<=x && x<=tbx) {
					output[x*PIXEL_SIZE + y*width*PIXEL_SIZE + padding*y] = image[x*PIXEL_SIZE + y*width*PIXEL_SIZE + padding*y];
					output[x*PIXEL_SIZE + y*width*PIXEL_SIZE + padding*y + 1] = image[x*PIXEL_SIZE + y*width*PIXEL_SIZE + padding*y + 1];
					output[x*PIXEL_SIZE + y*width*PIXEL_SIZE + padding*y + 2] = image[x*PIXEL_SIZE + y*width*PIXEL_SIZE + padding*y + 2];
				}
				else {
					output[x*PIXEL_SIZE + y*width*PIXEL_SIZE + padding*y] = 255;
					output[x*PIXEL_SIZE + y*width*PIXEL_SIZE + padding*y + 1] = 255;
					output[x*PIXEL_SIZE + y*width*PIXEL_SIZE + padding*y + 2] = 255;
				}
			}
		}
		WriteBitmapFile(fileheader, infoheader, output, size, name); cnt++;  //사진 생성 
	}
    free(image);      // 픽셀 데이터를 저장한 동적 메모리 해제생성 

    return 0;
}
