
#include<algorithm>
#include<sstream>
#include<iostream>
#include<string>
#include<cstring>


#ifndef BILL_H
#define BILL_H


using namespace std;

struct List
{
	List(){};
	public: void add(string maHD,string ngaylap,string lHD,string tenNV,string trigia)
	{
		maHoaDon=maHD;
		Ngaylap=ngaylap;
		LoaiHD=lHD;
		tenNv=tenNV;
		Trigia=trigia;
	}
	string maHoaDon,Ngaylap,LoaiHD,tenNv,Trigia;
};

struct Time{
	Time(){};
	Time(const int& Date,const int& Month, const int& Year){
		//Check;
		
		date=Date;
		month=Month;
		year=Year;
	}
	
	void ParseString(char * des){
		char tmp[10];
		itoa(date,tmp,10);
		strcpy(des,tmp);
		itoa(month,tmp,10);
		strcat(des,"/");
		strcat(des,tmp);
		strcat(des,"/");
		itoa(year,tmp,10);
		strcat(des,tmp);
	}
	
	void operator =(const Time& b){
		date=b.date;
		month=b.month;
		year=b.year;
	}
	int date,month,year;
};

struct BillDetail{
	BillDetail(){};
	BillDetail(const char* Code,const int& Quantity,const int& Price,const int& Vat){
		strcpy(code,Code);
		quantity=Quantity;
		price=Price;
		vat=Vat;
		total=(1LL*quantity*price)*((vat+100)/100.f);
	}

	char code[10];//Ma vat tu
	int quantity;
	int price;
	int vat;
	
	int total;//
	
	void operator=(const BillDetail& b){
		strcpy(code,b.code);
		quantity=b.quantity;
		price=b.price;
		vat=b.vat;
		total=b.total;

	}
};


enum BillType{N,X};
const int MAX_MATERIALS_PER_BILL=20;


struct DetailList{
	int n;
	BillDetail billDetail[MAX_MATERIALS_PER_BILL];
};
struct Bill{
	Bill(){
		code[0]='\0';
		total=0;
		detailList=new DetailList;
		detailList->n=-1;
	};
	~Bill(){
		delete detailList;
	}
	void DeleteBillDetail(int index){
		//Khong co truong hop index vuot qua gioi han
		total-=detailList->billDetail[index].total;
		for(int i=index;i<detailList->n;i++){
			detailList->billDetail[i]=detailList->billDetail[i+1];
		}
		--detailList->n;
	}
	void AddBillDetail(const char* Code,const int& Quantity,const int& Price, const int& Vat){
		detailList->billDetail[++detailList->n]=BillDetail(Code,Quantity,Price,Vat);
		total+=detailList->billDetail[detailList->n].total;
	}
	void ResetBill(){
		code[0]='\0';
		total=0;
		detailList->n=-1;
	}
	char code[20];
	Time time;
	BillType billType;//
	
	DetailList* detailList;
	
	long long total;
	
};


struct BillNode{
	Bill bill;
	BillNode* next;
};



Bill* InsertBill(BillNode* &list,const Bill* bill){// Tra ve vi tri bill vu moi chen vo
	BillNode* node=new BillNode;
	node->next=NULL;
	node->bill.time=bill->time;
	node->bill.total=bill->total;
	node->bill.billType=bill->billType;
	strcpy(node->bill.code,bill->code);	
//	node->bill.countBillDetail=bill->countBillDetail;

	
	node->bill.detailList=new DetailList;
	*node->bill.detailList=*(bill->detailList);
//	for(int i=0;i<=bill->countBillDetail;i++){
//		node->bill.billDetail[i]=bill->billDetail[i];
//	}
	node->next=list;
	list=node;
	
	return &node->bill;
}


BillNode* FindBill(BillNode* List,const char* BillCode){
	while(List!=NULL&&stricmp(List->bill.code,BillCode)!=0){
		List=List->next;
	}
	return List;
}



#endif







