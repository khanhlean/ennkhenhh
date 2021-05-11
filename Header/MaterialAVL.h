#include<iostream>
#include<algorithm>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include"ALGORITHM.h"


#ifndef MATERIAL_H
#define MATERIAL_H



using namespace std;


struct Material{
	Material(){};
	Material(const char* Code, const char* Name, const char* Unit,int Quantity){
		strcpy(code,Code);
		strcpy(name,Name);
		strcpy(unit,Unit);
		quantity=Quantity;
	}
	
	void operator=(const Material& material){
		strcpy(code,material.code);
		strcpy(name,material.name);
		strcpy(unit,material.unit);
		quantity=material.quantity;
	}
	int operator>(const Material& material){
		return stricmp(code,material.code)>0;
	}
	int operator<(const Material& material){
		return stricmp(code,material.code)<0;
	}
	char code[10];
	char name[41];
	char unit[31];
	int quantity;
};


struct MaterialNode{
		Material material;
		int height;
		MaterialNode* left;
		MaterialNode* right;
	};
	
int GetNodeHeight(MaterialNode* node){
		if(node==NULL) return 0;
		return node->height;
}

MaterialNode* CreateNode(const Material& data){
		MaterialNode* node=new MaterialNode;
		node->material=data;
		node->left=NULL;
		node->right=NULL;
		node->height=1;
		return node;
}
	
int GetNodeBalanceFactor(MaterialNode* node){
		if(node==NULL) return 0;
		return GetNodeHeight(node->left)-GetNodeHeight(node->right);
}

	
MaterialNode* RightRotate(MaterialNode* root){// lenh ve ben trai thi quay ben phai
		//Xoay
		MaterialNode* nextroot=root->left;
		root->left=nextroot->right;
		nextroot->right=root;
		
		//Cap nhat do cao
		root->height=max(GetNodeHeight(root->left),GetNodeHeight(root->right))+1;
		nextroot->height=max(GetNodeHeight(nextroot->left),GetNodeHeight(nextroot->right))+1;
		return nextroot;
	}
	
MaterialNode* LeftRotate(MaterialNode* root){
		MaterialNode* nextroot=root->right;
		root->right=nextroot->left;
		nextroot->left=root;
		
		root->height=max(GetNodeHeight(root->left),GetNodeHeight(root->right))+1;
		nextroot->height=max(GetNodeHeight(nextroot->left),GetNodeHeight(nextroot->right))+1;
		return nextroot;
	}
	
MaterialNode* GetMinNode(MaterialNode* root){
		while(root->left!=NULL) root=root->left;
		return root;
	}
	
	
	
MaterialNode* InsertNode(MaterialNode* &root,Material& data){//Tra ve nut vua moi chen vao
		if(root==NULL) {
			root=CreateNode(data);
			return root;
		}
		MaterialNode* renode;//Nut mua voi chen vao
		
		MaterialNode* nextnode;
		bool isleftnode=true;
		if(data>root->material) nextnode=root->right,isleftnode=false;
		else if(data<root->material)nextnode=root->left;
		else return root;
		//Truong hop bang nhau tra ve vi tri cua node
		renode=InsertNode(nextnode,data);
		
		if(isleftnode) root->left=nextnode;
		else root->right=nextnode;
		
		root->height=max(GetNodeHeight(root->left),GetNodeHeight(root->right))+1;
		int bf=GetNodeBalanceFactor(root);
		
		if(bf>1&&data<root->material) { //LEFT LEFT
			root=RightRotate(root);
		}
		else if(bf>1&&data>root->material){ //LEFT RIGHT
			root->left=LeftRotate(root->left);
			root=RightRotate(root);
		}
		
		else if(bf<-1&&data>root->material){//RIGHT RIGHT
			root=LeftRotate(root);
		}
		else if(bf<-1&&data<root->material){// RIGHT LEFT
			root->right=RightRotate(root->right);
			root=LeftRotate(root);
		}
		
		
		return renode;
	}
	
MaterialNode* DeleteNode(MaterialNode* &root,Material& data){//Ham nay se khong xoa vung nho vung nho du lieu se duoc tra lai
		if(root==NULL) return root;
		
		MaterialNode* renode;
		MaterialNode* nextnode;
		
		if(data<root->material) {
			nextnode=root->left;
			renode=DeleteNode(nextnode,data);
			root->left=nextnode;
		}
		else if(data>root->material) {
			nextnode=root->right;
			renode=DeleteNode(nextnode,data);
			root->right=nextnode;
		}
		else{//Truong hop bang
			if(root->right==NULL||root->left==NULL){//nut nay chua mot nut la hoac khong chu nut la nao, truong hop khong can xoay cay
				MaterialNode* tmp=root->right?root->right:root->left;
				
				if(tmp==NULL){//Truong hop nay khong can cap nhat lai cay
					MaterialNode* tmp=root;
					root=NULL;
					return tmp;
				}
				else{//Do cao giam co the anh huong toi do cao cua nut cha
					*root=*tmp;//xoa root, dua node len cho root
	//				root->left=NULL;
	//				root->right=NULL;
	//				s
					//Xoa vung nho cua tmp
					renode=tmp;
				}
				
			}
			else { //Lay nut cuc trai
				MaterialNode* tmp=GetMinNode(root->right);
	//			Node* tmp=GetMinValueNode(root->right);//Nut cuc trai cua cay con ben phai
	//			root->material=tmp->material;//Lay noi dung cu nut cuc trai
				//Sap chep key  va data cua nut cuc trai  
				root->material=tmp->material;
				
				
				nextnode=root->right;
				renode=DeleteNode(nextnode,tmp->material);//Xoa nut cuc trai;
				root->right=nextnode;
			}
		}
		
		root->height=max(GetNodeHeight(root->left),GetNodeHeight(root->right))+1;
		int bf=GetNodeBalanceFactor(root);	
			
		if(bf>1&&GetNodeBalanceFactor(root->left)>=0){//LEFT LEFT
			root=RightRotate(root);
		}
		else if(bf>1&&GetNodeBalanceFactor(root->left)<0){//LEFT RIGHT
			root->left=LeftRotate(root->left);
			root=RightRotate(root);
		}
		else if(bf<-1&&GetNodeBalanceFactor(root->right)<=0){//RIGHT RIGHT
			root=LeftRotate(root);
		}
		else if(bf<-1&&GetNodeBalanceFactor(root->right)>0){//RIGHT LEFT
			root->right=RightRotate(root->right);
			root= LeftRotate(root);
		}
		
		
		return renode;
	}
	
MaterialNode* SearchNode(MaterialNode* root,const char* key){
		MaterialNode* re=NULL;
		while(root!=NULL){
			if(stricmp(key,root->material.code)<0) root=root->left;
			else if(stricmp(key,root->material.code)>0) root=root->right;
			else {
				re=root;
				break;
			}
		}
		return re;
};

void quickSortMaterial(MaterialNode* Data[], int l , int r)
{	
	if (l <= r)
	{
		
		char key[45];
 		strcpy(key,Data[(l+r)/2]->material.name);
		int i = l;
		int j = r;
 
		while (i <= j)
		{
			while (stricmp(Data[i]->material.name,key)<0)
				i++;
			while (stricmp(Data[j]->material.name,key)>0)
				j--;
 
			if (i <= j)
			{
				swap(Data[i],Data[j]);
				i++;
				j--;
			}
		}
 
	
		if (l < j)
			quickSortMaterial(Data, l, j);
		if (r > i)
			quickSortMaterial(Data, i, r);
	}
}


class nhanvien
{
	public:
	string sex,middle,first,last,manv;
	nhanvien(){
		billList=NULL;
	}
	~nhanvien()
	{
		for(BillNode* k=billList;k!=NULL;k=k->next)
			delete k;
	}
	nhanvien(string Ho,string Ten,string Dem,string Phai,string Manv)
	{
		//Tao mot nhan vien moi
		first=Ten;
		last=Ho;
		middle=Dem;
		sex=Phai;
		manv=Manv;
		billList=NULL;
	}
	BillNode* billList;
	void add(string Ho,string Ten,string Dem,string Phai,string Manv)
	{
		///Sua doi nhan vien
		first=Ten;
		last=Ho;
		middle=Dem;
		sex=Phai;
		manv=Manv;
	}
};
string chuanhoa(string s){
	char k=s[s.size()-1];
	if(s.size()==1) return s;
	else
	{
		while(s[0]==' ') s.erase(s.begin()+0);
		for(int i=1;i<s.size()-2;i++)
			if(s[i]==' '&&s[i+1]==' ') 
			{
				s.erase(s.begin()+i);
				i--;	
			}
		while(s[s.size()-1]==' '||s[s.size()-1]==k) s.erase(s.begin()+s.size()-1);
	}
	return s;
}


void quickSort(nhanvien* Data[], int l , int r)
{	
	if (l <= r)
	{
		
		string key = Data[(l+r)/2]->first+Data[(l+r)/2]->last+Data[(l+r)/2]->middle;
 		
 
		int i = l;
		int j = r;
 
		while (i <= j)
		{
			while (stricmp((Data[i]->first+Data[i]->last+Data[i]->middle).c_str(),key.c_str())<0)
				i++;
			while (stricmp((Data[j]->first+Data[j]->last+Data[j]->middle).c_str(),key.c_str())>0)
				j--;
 
			if (i <= j)
			{
				swap(Data[i], Data[j]);
				i++;
				j--;
			}
		}
 
	
		if (l < j)
			quickSort(Data, l, j);
		if (r > i)
			quickSort(Data, i, r);
	}
}
struct Node
{
	string data;
	long long sl;
	Material *material;
	
	struct Node *pLeft;
	struct Node *pRight;
};
typedef struct Node NODE;
typedef NODE* TREE;
void khoitao(TREE &t)
{
	t=NULL;
}
void themNODE(TREE &t,string x,int sl,Material* material)
{
	if(t==NULL)
	{
		NODE *p=new NODE;
		p->data=x;
		p->sl=sl;
		p->pLeft=NULL;
		p->pRight=NULL;
		p->material=material;
		t=p;
	}
	else
	{
		if(t->data>x)
			themNODE(t->pLeft,x,sl,material);
		else if(t->data<x)
			themNODE(t->pRight,x,sl,material);
		else
		{
			t->sl+=sl;
		}
	}
}

struct thongke
{
	int tien;
	string ten;
	string ma;

};
typedef struct thongke TK;



void heap(int r,int n,Node *a[])
{
	int k=2*r+1;
	Node *x=a[r];
	bool check=true;
	while(k<=n-1&&check)
	{
		if(k<n-1) // khong co nhanh phai
			if(a[k]->sl<a[k+1]->sl)
				k++;
		if(a[k]->sl<=x->sl) check=false;
		else
		{
			a[r]=a[k];
			r=k;
			k=2*r+1;
		}
		a[r]=x;
	}
}
void heap_sort(Node *a[],int n,int x)
{
	Node *temp;
	for(int i=n/2-1;i>=0;i--)	
		heap(i,n,a);
	for(int i=n-2,j=0;i>=0&&j<x;i--,j++)
	{
		temp=a[0];
		a[0]=a[i+1];
		a[i+1]=temp;
		heap(0,i+1,a);
	}
}


#endif


