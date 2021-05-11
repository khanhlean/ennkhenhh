#include<iostream>
#include<vector>
#include<algorithm>
#include<cstring>
#include<string>
#include <cstddef>
#include<fstream>
#include<cctype>
#include<ctime>
#include<cmath>
#include"Windows.h"

#include"graphics.h"

#ifndef BASIC_H
#define BASIC_H



const int  MAX_QUANTITY=999999999;
const int  MAX_PAYMENT_VALUE=999999999;


using namespace std;


#define smallwindow_bg_color WHITE
#define bg_color COLOR(250,250,255)
#define bg_border_color COLOR(188,188,204)
#define title_bg_color COLOR(150,170,190)


#define invalid_color COLOR(247,100,64)
#define valid_color COLOR(130,234,70)

#define tab_basic_color COLOR(90,141,183)
#define tab_on_color  COLOR(128,200,253)

#define tab_width 400
#define tab_height 32


#define addbutton_basic_color COLOR(255,218,125)
#define addbutton_on_color COLOR(224,180,90)


#define pageup_basic_color COLOR(220,220,230)
#define pageup_on_color COLOR(180,180,200)
#define scroll_basic_color COLOR(150,150,224)
#define scroll_on_color COLOR(130,130,225)

#define close_on_color COLOR(237, 45, 42)
#define close_basic_color COLOR(250,250,250)

#define deletepayment_basic_color COLOR(252,82,66) 
#define deletepayment_on_color COLOR(176,56,46)

#define field_basic_color COLOR(240,240,240)
#define field_on_color WHITE
#define fieldborder_basic_color WHITE
#define fieldborder_on_color COLOR(100,150,230)

#define pink_basic_color COLOR(252, 131, 195)
#define pink_on_color COLOR(142, 74, 110)


//#define tab_style settextstyle(1,0,2)
#define tab_text_color WHITE

#define popup_text_color WHITE

//#define text_style settextstyle(0,0,0);
#define text_width 10
#define text_height 18
////////////////////////////////


const int screenWidth=1310;
const int screenHeight=700;
const int listWidth=1274;
const int listHeight=570;


Time currentTime;


void CatchTime(){
	time_t currenttime= time(0);
	tm* now=localtime(&currenttime);
	currentTime.date=now->tm_mday;
	currentTime.month=now->tm_mon+1;
	currentTime.year=now->tm_year+1900;
}

const int BORDEROFFSET=2;

const int MAXSTOCK=2e6;//2 trieu phan tu

bool isActive;
//Anouncement
float startTime=0.f;
char mess[81];

int desR=250,desG=250,desB=255;

int drawR=250,drawG=250,drawB=255;


float timer=0.f;
bool isappear=true;
void DrawTextPointer(int centerX,int centerY){
	if((float)clock()/CLOCKS_PER_SEC>=timer){
		timer=(float)clock()/CLOCKS_PER_SEC;
		if(isappear) timer+=0.7f;
		else timer+=0.7f;
		isappear=!isappear;
	}
	if(isappear){
		setcolor(BLACK);
		line(centerX,centerY-10,centerX,centerY+10);
	}
}


//Override
void outtextxy(int x,int y,long long value){
	char tmp[20];
	lltoa(value,tmp,10);
	outtextxy(x,y,tmp);
}
//////////////////////////////////Thong bao tren man hinh
int messageposX=1250,messageposY=53;
float dur;
void ChangeMessage(const char* message){
	drawR=247;
	drawG=100;
	drawB=64;
	
	startTime=(float)clock()/CLOCKS_PER_SEC;
	dur=5.f;
	strcpy(mess,"!");
	strcat(mess,message);
}

void PrintMessage(){
	float ratio=((float)clock()/CLOCKS_PER_SEC-startTime)/dur;
	if(ratio>1.f) ratio=1.f;
	setbkcolor(getpixel(messageposX,messageposY));
	if(ratio==1.f) setcolor(getpixel(messageposX,messageposY));
	else setcolor(COLOR(drawR+(desR-drawR)*ratio,drawG+(desG-drawG)*ratio,drawB+(desB-drawB)*ratio));
	outtextxy(messageposX-textwidth(mess),messageposY,mess);
	
}
////////////////////////////////////////////////////

void DrawTabBG(){
	setfillstyle(SOLID_FILL,tab_basic_color);
	bar(0,0,screenWidth,tab_height+BORDEROFFSET+3);
}
void DrawStatiscalWindow(){
	setfillstyle(SOLID_FILL,smallwindow_bg_color);
	bar(screenWidth/2-600,90,screenWidth/2+600,680);
	setcolor(bg_border_color);
	rectangle(screenWidth/2-600,90,screenWidth/2+600,680);
	

}
void DrawInputWindow(char* text){
		int width=700;
		//
		setfillstyle(SOLID_FILL,smallwindow_bg_color);
		setcolor(smallwindow_bg_color);
		bar(screenWidth/2-width/2,220,screenWidth/2+width/2,550);
		setcolor(bg_border_color);
		rectangle(screenWidth/2-width/2,220,screenWidth/2+width/2,550);
		
		setcolor(WHITE);
		setfillstyle(SOLID_FILL,title_bg_color);
		bar(400,180,900,220);
		setbkcolor(title_bg_color);	
		outtextxy(650-textwidth(text)/2,190,text);
}
void DrawBG(){
		setfillstyle(SOLID_FILL,bg_color);
		bar(0,0,screenWidth,screenHeight);
		DrawTabBG();
		
		setcolor(tab_on_color);
		for(int i=0;i<BORDEROFFSET;i++){
			rectangle(i,tab_height+BORDEROFFSET+i+3,screenWidth-i-1,screenHeight-i-1);
		}
		setcolor(bg_border_color);
		rectangle(15,43,screenWidth-15,80);
}

int GetInput(){
//
	int re=0;
	for(int i=65;i<=90;i++){
		if(GetAsyncKeyState((char)(i))& 1 ) {
			if(GetKeyState(VK_CAPITAL)&1)
			return i;
			else return i+32;
		}
	}
	for(int i=48;i<=57;i++){
		if(GetAsyncKeyState((char)i)& 1 ) {
			return i;
		}
	}
	if(GetAsyncKeyState(VK_BACK)& 1 ) return -1;
	if(GetAsyncKeyState(VK_SPACE)&1) return ' ';
	if(GetAsyncKeyState(VK_TAB)&1) return  (int)'\t';
	if(GetAsyncKeyState(VK_UP)&1) return 1;
	if(GetAsyncKeyState(VK_DOWN)&1) return 2;
	return re;
}



int mousePointX,mousePointY,oldMousePointX,oldMousePointY;
bool isLeftDown,isLeftUp;



void UpdateMouseFlag(){
	oldMousePointX=mousePointX;
	oldMousePointY=mousePointY;
	mousePointX=mousex();
	mousePointY=mousey();
	isLeftDown=GetAsyncKeyState(VK_LBUTTON)&1;
	isLeftUp=ismouseclick(WM_LBUTTONUP);
}

float Distant(int x1,int y1,int x2,int y2){
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

class UI{
	public: UI(int x,int y,int w, int h,int basiccolor,int oncolor,int textcolor=WHITE){
		basicColor=basiccolor;
		onColor=oncolor;
		curColor=basicColor;
		textColor=textcolor;
		
		centerX=x;
		centerY=y;
		width=w;
		height=h;
	}
	public: UI(){};
	public: virtual void DrawUI(){
		setbkcolor(curColor);
		setfillstyle(SOLID_FILL,curColor);
		bar(centerX-width/2,centerY-height/2,centerX+width/2,centerY+height/2);
	}
	protected: bool OnRange(){
		if(mousePointX>=centerX-width/2&&mousePointX<=centerX+width/2&&mousePointY>=centerY-height/2&&mousePointY<=centerY+height/2) return true;
		return false;
	}

	protected:	
		int centerX,centerY;
		int width,height;
	
		int curColor;
		int basicColor;
		int onColor;
		int textColor;
};

template<class T> 
class Trigger:public UI{//Trigger la nut co mau chu mau trang
typedef void(T::*Function)();
	typedef void(T::*Function2)(int);
	
	public: Trigger(){};
	public: Trigger(int x,int y,int w,int h,int basiccolor,int oncolor,const char* txt,Function toDo,int color=WHITE):UI(x,y,w,h,basiccolor,oncolor,color){
		strcpy(text,txt);
		todo=toDo;
		animation=false;
	}
	public: Trigger(int x,int y,int w,int h,int basiccolor,int oncolor,const char* txt, Function2 toDo2,int color=WHITE):UI(x,y,w,h,basiccolor,oncolor,color){
		strcpy(text,txt);
		todo2=toDo2;
		animation=false;
	}
	public: void Action(T* object){
		DrawUI();
		if(OnRange()&&isLeftDown){
			(object->*todo)();
		}
	}
	public: void Action(T* object,int index){
		
		DrawUI();
		if(OnRange()&&isLeftDown){
		(object->*todo2)(index);
		}
	}
	
	public: void DrawUI(){
		UI::DrawUI();
		if(OnRange()){
			if(!animation) 	startanimationtime=(float)clock()/CLOCKS_PER_SEC;
			float ratio=((float)clock()/CLOCKS_PER_SEC-startanimationtime)/animationtime;
			if(ratio>1.f) ratio=1.f;
			//Ve animation fill
			setbkcolor(onColor);
			setfillstyle(SOLID_FILL,onColor);
			bar(centerX-width/2,centerY-height/2,(centerX+width/2),centerY-height/2+height*ratio);
			
			animation=true;
		}
		else animation=false;
		
		setcolor(textColor);
		int w=textwidth(text);
		int h=textheight(text);
		outtextxy(centerX-w/2,centerY-h/2,text);
	}

	
	public: static const float animationtime=0.35f;
	
	private:
	bool animation;
	float startanimationtime;
	char text[21];
	Function todo;
	Function2 todo2;
};

//Input Field
class InputField:public UI{
	public: InputField(){};
	public: InputField(int x,int y,int h,int maxch,int basiccolor,int oncolor,const char* fieldname,char* infield,int textColor=BLACK):UI(x,y,(maxch+2)*text_width,h,basiccolor,oncolor,textColor){
		maxChar=maxch;
		strcpy(fieldName,fieldname);
		index=0; 
		field=infield;
		field[index]='\0';
		anouncement[index]='\0';
		
		invalidAnounceColor=invalid_color;
	    validAnounceColor=valid_color;
	    
		
	}
	public: void AddChar(char c){
		if(index==maxChar) return;
		field[index++]=c;
		field[index]='\0';
	}
	public: void DeleteChar(){
		if(index==0) return;
		field[--index]='\0';
	}
	private: void DrawUI(){
		UI::DrawUI();
		setcolor(textColor);		
		outtextxy(centerX-width/2+text_width,centerY-text_height/2,field);
		//Field Name
		setbkcolor(smallwindow_bg_color);
		outtextxy(centerX-width/2-(strlen(fieldName)+1)*text_width,centerY-text_height/2,fieldName);
		
		//border 
		setcolor(curBorderColor);
		rectangle(centerX-width/2 -1,centerY-height/2-1,centerX+width/2 +1,centerY+height/2 +1);
		rectangle(centerX-width/2 -2,centerY-height/2-2,centerX+width/2 +2,centerY+height/2 +2);
		
		//put announcement
		setcolor(curAnounceColor);
		outtextxy(centerX-textwidth(anouncement)/2,centerY+height/2+3,anouncement);
		
	}
	private: void OnClick(InputField* &fieldpointer){
		if((OnRange()&&isLeftDown)||fieldpointer==this){
			curColor=onColor;
			curBorderColor=fieldborder_on_color;
			fieldpointer=this;
			
		}
		else  {
			curColor=basicColor;
			curBorderColor=fieldborder_basic_color;
		}
	}
	public: void Action(InputField* &fieldpointer){
		OnClick(fieldpointer);
		DrawUI();
		if(fieldpointer==this) 	DrawTextPointer(centerX-width/2+(text_width*(index+1)),centerY);
	}
	public: void BlankField(){
		index=0;
		field[0]='\0';
		anouncement[0]='\0';
	}
	public: void CustomInit(const char* text){//Khoi tao field void doan text da co san
		strcpy(field,text);
		index=strlen(field);
		anouncement[0]='\0';//blank
	}
	public: void CustomInit(int val){
		char tmp[10];
		itoa(val,tmp,10);
		strcpy(field,tmp);
		index=strlen(field);
		anouncement[0]='\0';//blank
	}
public: bool CheckParseInt(){
		//Debug
		if(strlen(field)==0) {
				Anouncement("Khong the trong!",false);
				return false;	
		}
		for(int i=0;i<strlen(field);i++){
			if(!isdigit(field[i])){
				Anouncement("Chi co bao gom so",false);
				return false;
			}
		}
		Anouncement("OK",true);
		return true;	
	}
	public: int ParseInt(){
		return atoi(field);
	}
	public: bool CheckParseString(){
		if(strlen(field)==0) {
			Anouncement("Khong the trong!",false);
			return false;	
		}
		//Debug
		Anouncement("OK",true);
		return true;
	}
	public: void Anouncement(const char* anounce,bool isValid){
		strcpy(anouncement,anounce);
		if(isValid) curAnounceColor=validAnounceColor;
		else curAnounceColor=invalidAnounceColor;
	}
	public: void ParseString(char *to){
		strcpy(to,field);
		Standardized(to);////Chuan hoa chuoi
	}
	int index;
	int maxChar;
	char* field;
	char anouncement[61];
	char fieldName[41];
	int invalidAnounceColor;
	int validAnounceColor;
	int curAnounceColor;
	int curBorderColor;
};


	
class Scroller:public UI{
	public: Scroller(){};
	public: Scroller(int upx,int upy,int* i):UI(upx,upy,20,60,scroll_basic_color,scroll_on_color){
		len=460;
		
		flag=false;
		upX=upx;
		upY=upy;
		downY=upY+len;
		
		centerX=upX;
		centerY=upY;
		
		upbutton=Trigger<Scroller>(upx,upY-height/2-10,20,20,pageup_basic_color,pageup_on_color,"^",&Scroller::Up);
		downbutton=Trigger<Scroller>(upx,downY+height/2+10,20,20,pageup_basic_color,pageup_on_color,"v",&Scroller::Down);
		
		index=i;
		*index=0;
	}
	private: void OnClick(){
		if((OnRange()||flag)&&GetAsyncKeyState(VK_LBUTTON)&0x8000){
			curColor=onColor;
			//Tinh vector, di chuyen huong truc Y
			centerY=mousePointY;
			centerY=max(centerY,upY);
			centerY=min(downY,centerY);
			flag=true;
		}
		else curColor=basicColor,flag=false;
	}
	public: void Action(const int& base){
		maxindex=base;
		if(base<=0) {
			UI:DrawUI();
			*index=0;
		}
		else 	*index=round(base*(Distant(upX,upY,centerX,centerY)/len));//chi so tiep theo cho indice
	
		OnClick();
		upbutton.Action(this);
		downbutton.Action(this);
	
		
		setfillstyle(SOLID_FILL,pageup_basic_color);
		bar(upX-width/2,upY-height/2,upX+width/2,downY+height/2);
		UI::DrawUI();
	}
	
	private: void Up(){
		*index=max(0,*index-1);
		
		double lastcur=Distant(upX,upY,centerX,centerY);
		
		/////////////////////Nho sua cai ham nay
		double cur=(len*(*index))/maxindex;
		double deltaMoveY=cur-lastcur;
		centerY+=deltaMoveY;
	}
	private: void Down(){
		*index=min(maxindex,*index+1);
		
		double lastcur=Distant(upX,upY,centerX,centerY);
		
		/////////////////////Nho sua cai ham nay
		double cur=(len*(*index))/maxindex;
		double deltaMoveY=cur-lastcur;
		centerY+=deltaMoveY;
	}
	
	
	Trigger<Scroller> upbutton,downbutton;
	
	int* index;
	int maxindex;
	
	int len;
	int upX;
	int upY,downY;
	bool flag;
};
//
//Tab
class Tab: public UI{
	public: Tab(){};
	public: Tab(int x,int y,int w,int h,int basiccolor,int oncolor,const char* txt,int textColor=WHITE):UI(x,y,w,h,basiccolor,oncolor,textColor){//Goi constructor cua lop UI de khoi tao cho Tab
		strcpy(text,txt);
	}
	protected: virtual void DrawUI(){//Override cua UI 
		UI::DrawUI();
		setcolor(textColor);
		int w=textwidth(text);
		int h=textheight(text);
		outtextxy(centerX-w/2,centerY-h/2,text);//Can le test
	}
	public  : void OnClick(Tab* &tab){
		if((OnRange()&&isLeftDown)||tab==this){
			tab=this;
			curColor=onColor;
		}
		else if(tab!=this) curColor=basicColor; 
		DrawUI();
	}
	public: virtual void Action()=0;
	char text[30];
};



int CompareTime(const Time& a,const Time& b)
{
	if(a.year!=b.year)
	{
		if(a.year>b.year)return 1;
		else return -1;
	}
	else
	{
		if(a.month!=b.month)
		{
			if(a.month>b.month) return 1;
			else return -1;
		}
		else
		{
			if(a.date!=b.date)
			{
				if(a.date>b.date) return 1;
				else return -1;
			}
			else return 0;
		}
	}
}
bool CheckTime(InputField& datefield,InputField& monthfield,InputField& yearfield){
	if(!datefield.CheckParseInt()||!monthfield.CheckParseInt()||!yearfield.CheckParseInt()) return false;
	
		Time t=Time(datefield.ParseInt(),monthfield.ParseInt(),yearfield.ParseInt());
		if(CompareTime(currentTime,t)<0){
			datefield.Anouncement("Hoa don o tuong lai ???",false);
			return false;
		}
		
		bool flag=true;
		int date=datefield.ParseInt();
		int month=monthfield.ParseInt();
		int year=yearfield.ParseInt();
		if(month<=0||month>12){
			monthfield.Anouncement("Thang chi tu 1-12",false);
			flag=false;
		}
		if(date<=0||date>31){
			datefield.Anouncement("Ngay chi tu 1-31",false);
			flag=false;
		}
		if(!flag) return false;
		switch (month){
			case 2:
				{
					if(year%4==0&&date>29){//nam nhuan nhe
						datefield.Anouncement("Thang 2 co 29 ngay thoi",false);
						return false;
					}
					else if(year%4!=0&&date>28){
						datefield.Anouncement("Thang 2 co 28 ngay thoi",false);
						return false;
					}
					break;
				}
			case 4:{
				if(date>30){
					datefield.Anouncement("Thang 4 co 30 ngay thoi",false);
					return false;
				}
				break;
			}
			case 6:{
				if(date>30){
					datefield.Anouncement("Thang 6 co 30 ngay thoi",false);
					return false;
				}
				break;
			}
			case 9:{
				if(date>30){
					datefield.Anouncement("Thang 9 co 30 ngay thoi",false);
					return false;
				}
				break;
			}
			case 11:{
				if(date>30){
					datefield.Anouncement("Thang 11 co 30 ngay thoi",false);
					return false;
				}
				break;
			}
	}
	return true;
}


#endif

