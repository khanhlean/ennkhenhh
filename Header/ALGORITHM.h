#include<iostream>
#include<cstring>
#include<sstream>
#ifndef ALGORITHM_H
#define ALGORITHM_H




using namespace std;

void Standardized(char* s){//Chuan hoa chuoi
	int len=strlen(s);
	int i=0,j=len-1;
	while(i<len&&s[i]==' ') ++i;
	while(j>=0&&j>i&&s[j]==' ')--j;
	int cnt=0,index=-1;
	for(int k=i;k<=j;k++){
		if(s[k]==' '){
			if(cnt==0){
				s[++index]=s[k];
			}
			++cnt;
		}
		else{
			cnt=0;
			s[++index]=s[k];
		}
	}
	s[++index]='\0';
	return;
}


//////////////////////////////////////////////////////////////////Sinh ma chuoi
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
	FILE* f=fopen("file.txt","r");
	fscanf(f,"%s %s %s",curM,curS,rootB);
	strcpy(curB,rootB);
	fclose(f);
}
void WriteString(){
	FILE* f=fopen("file.txt","w");
	fprintf(f,"%s %s %s",curM,curS,curB);
	fclose(f);
	
}
////////////////////////////////////////////////////////////////////////////////









/////////////////////////////////////////////////////Bao kiin.....


////////////////////////////////////////////////////////////////////////////Cac ham ve chuoi
string Tostring(int a)
{
	string k;
	stringstream s;
	s<<a;
	s>>k;
	return k;
}




////////////////////////////////////////////////////////Cac ham so sanh thoi gian

#endif
