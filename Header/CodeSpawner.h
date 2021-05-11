#include<iostream>
#include<cstring>

using namespace std;

char curM[10];
char curS[6];

char rootB[20];
char curB[20];//Su dung nhu bo nho tam
void NextString(char* cur,int len){
	int add=1;
	int i=len-2;//Ki tu cuoi la ki tu ket thuc
	while(add!=0){
		int val=(cur[i]-'0')+add;
		cur[i]=(val%10)+'0';
		if(val>9) add=1;
		else add=0;
		--i;
		if(i==-1){
			cout<<"He thong da vuot qua gioi han ma vat tu"<<endl;
			break;
		}
	}
}
void CaptureString(){
	FILE* f=fopen("C://Users//Quoc Bao//Desktop//DA1.0//file.txt","r");
	fscanf(f,"%s %s %s",curM,curS,rootB);
	strcpy(curB,rootB);
	fclose(f);
}
void WriteString(){
	FILE* f=fopen("D://DA1.0//file.txt","w");
	fprintf(f,"%s %s %s",curM,curS,curB);
	fclose(f);
	
}






