#include<stdio.h>
#include<Windows.h>
#include<MMSystem.h>
#pragma comment(lib, "Winmm.lib")

struct A{
	char read[100];
}d[30]; // 읽어들일 text 파일

void R(){
	int i;
	FILEW *f;
	f=fopen("read.txt","r");
	for(i=0;i<50;i++)
		fscanf(f,"%s", &d[i].read);
	fclode(f);
}// 파일 텍스트 배열에 저장

void S(char a){
	if(a=='1') PlaySound("one.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='2') PlaySound("two.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='3') PlaySound("three.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='4') PlaySound("four.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='5') PlaySound("five.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='6') PlaySound("six.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='7') PlaySound("seven.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='8') PlaySound("eight.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='9') PlaySound("nine.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='0') PlaySound("zero.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='+') PlaySound("plus.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='-') PlaySound("minus.mp3", 0, SND_FILENAME | SND_ASYNC)
	if(a=='=') PlaySound("equal.mp3", 0, SND_FILENAME | SND_ASYNC)
    if(a=='(') PlaySound("left(1).mp3", 0, SND_FILENAME | SND_ASYNC)
    if(a==')') PlaySound("right(1).mp3", 0, SND_FILENAME | SND_ASYNC)
    if(a=='{') PlaySound("left(2).mp3", 0, SND_FILENAME | SND_ASYNC)
    if(a=='}') PlaySound("right(2).mp3", 0, SND_FILENAME | SND_ASYNC)
    if(a=='/') PlaySound("eivide.mp3", 0, SND_FILENAME | SND_ASYNC)
    if(a=='*') PlaySound("times.mp3", 0, SND_FILENAME | SND_ASYNC)
} //음성 출력
 
int main(){
    int i=0, j;
    R();
    while(d[i].read!='\0'){
        S(d[i].read)
        i++;
        Sleep(1500); //음성이 출력되는 동안 시간지연
    }
}
