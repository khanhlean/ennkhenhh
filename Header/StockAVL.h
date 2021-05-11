#include<iostream>
#include<algorithm>
#include<cstdlib>
#include<fstream>
#include<cstring>

using namespace std;


struct Stock{
	Stock(){};
	Stock(const char* Code, const char* Name, const char* Unit,int Amount){
		strcpy(code,Code);
		strcpy(name,Name);
		strcpy(unit,Unit);
		amount=Amount;
	}
	
	void operator=(const Stock& stock){
		strcpy(code,stock.code);
		strcpy(name,stock.name);
		strcpy(unit,stock.unit);
		amount=stock.amount;
	}
	int operator>(const Stock& stock){
		return strcmp(code,stock.code)>0;
	}
	int operator<(const Stock& stock){
		return strcmp(code,stock.code)<0;
	}
	char code[7];
	char name[51];
	char unit[31];
	int amount;
};

struct Node{
	Stock stock;
	int height;
	Node *left,*right;
};

bool CompareByName(Node* a,Node* b){//Sap xep theo tang dan cua ten
	return strcmp(a->stock.name,b->stock.name)<=0;
}

Node* CreateNode(const Stock& stock){
	Node* node=new Node;
	node->stock=stock;
	node->height=1;
	node->left=NULL;
	node->right=NULL;
	return node;
}

int GetNodeHeight(Node* node){
	if(node==NULL) return 0;
	return node->height;
}

int GetNodeBalanceFactor(Node* node){
	if(node==NULL) return 0;
	return GetNodeHeight(node->left)-GetNodeHeight(node->right);
}

Node* RightRotate(Node* root){// lenh ve ben trai thi quay ben phai
	//Xoay
	Node* nextroot=root->left;
	root->left=nextroot->right;
	nextroot->right=root;
	
	//Cap nhat do cao
	root->height=max(GetNodeHeight(root->left),GetNodeHeight(root->right))+1;
	nextroot->height=max(GetNodeHeight(nextroot->left),GetNodeHeight(nextroot->right))+1;
	return nextroot;
}

Node* LeftRotate(Node* root){
	Node* nextroot=root->right;
	root->right=nextroot->left;
	nextroot->left=root;
	
	root->height=max(GetNodeHeight(root->left),GetNodeHeight(root->right))+1;
	nextroot->height=max(GetNodeHeight(nextroot->left),GetNodeHeight(nextroot->right))+1;
	return nextroot;
}

Node* InsertNode(Node* root, Stock& stock,Node* parr[],Node*& tonameorder,int& index){
	if(root==NULL) {
		Node* node=CreateNode(stock);
		parr[++index]=node;
		tonameorder=node;
		return node;
	}
	if(stock>root->stock) root->right=InsertNode(root->right,stock,parr,tonameorder,index);
	else root->left=InsertNode(root->left,stock,parr,tonameorder,index);
	//Khong co truong hop bang
	
	root->height=max(GetNodeHeight(root->left),GetNodeHeight(root->right))+1;
	int bf=GetNodeBalanceFactor(root);
	
	if(bf>1&&stock<root->left->stock) { //LEFT LEFT
		return RightRotate(root);
	}
	if(bf>1&&stock>root->left->stock){ //LEFT RIGHT
		root->left=LeftRotate(root->left);
		return RightRotate(root);
	}
	
	if(bf<-1&&stock>root->right->stock){//RIGHT RIGHT
		return LeftRotate(root);
	}
	if(bf<-1&&stock<root->right->stock){// RIGHT LEFT
		root->right=RightRotate(root->right);
		return LeftRotate(root);
	}
		
	return root;
}

Node* GetMinValueNode(Node *root){
	while(root->left!=NULL)
	 	root=root->left;
	return root;
}

Node* SearchNode(Node* root,const char* code){
	while(root!=NULL){
		if(stricmp(code,root->stock.code)<0) root=root->left;
		else if(stricmp(code,root->stock.code)>0) root=root->right;
		else break;
	}
	return root;
}

Node* DeleteNode(Node* root,Stock& stock,Node*& stockPointer){
	if(root==NULL) return root;
	
	if(stock<root->stock) root->left=DeleteNode(root->left,stock,stockPointer);
	else if(stock>root->stock) root->right=DeleteNode(root->right,stock,stockPointer);
	else{//Truong hop bang
		if(root->right==NULL||root->left==NULL){//nut nay chua mot nut la hoac khong chu nut la nao, truong hop khong can xoay cay
			Node* tmp=root->right?root->right:root->left;
			
			if(tmp==NULL){//Truong hop nay khong can cap nhat lai cay
				stockPointer=root;
				return tmp;
			}
			else{//Do cao giam co the anh huong toi do cao cua nut cha
				*root=*tmp;//xoa root, dua node len cho root
//				root->left=NULL;
//				root->right=NULL;
//				s
				//Xoa vung nho cua tmp
				stockPointer=tmp;
			}
			
		}
		else { //Lay nut cuc trai
			Node* tmp=GetMinValueNode(root->right);//Nut cuc trai cua cay con ben phai
			root->stock=tmp->stock;//Lay noi dung cu nut cuc trai
			
			root->right=DeleteNode(root->right,tmp->stock,stockPointer);//Xoa nut cuc trai;
			
		}
	}
	//Can bang lai cay
	root->height=max(GetNodeHeight(root->left),GetNodeHeight(root->right))+1;
	int bf=GetNodeBalanceFactor(root);	
		
	if(bf>1&&GetNodeBalanceFactor(root->left)>=0){//LEFT LEFT
		return RightRotate(root);
	}
	else if(bf>1&&GetNodeBalanceFactor(root->left)<0){//LEFT RIGHT
		root->left=LeftRotate(root->left);
		return RightRotate(root);
	}
	else if(bf<-1&&GetNodeBalanceFactor(root->right)<=0){//RIGHT RIGHT
		return LeftRotate(root);
	}
	else if(bf<-1&&GetNodeBalanceFactor(root->right)>0){//RIGHT LEFT
		root->right=RightRotate(root->right);
		return LeftRotate(root);
	}
	return root;
}

Node* DeleteNode(Node* pos,Node* &stockPointer){
	if(pos==NULL) return NULL;
	cout<<"Call"<<endl;
	if(pos->right==NULL||pos->left==NULL){//nut nay chua mot nut la hoac khong chu nut la nao, truong hop khong can xoay cay
			Node* tmp=pos->right!=NULL?pos->right:pos->left;
			
			if(tmp==NULL){//Truong hop nay khong can cap nhat lai cay
//				delete pos;

				stockPointer=pos;
				return NULL;
			}
			else{//Do cao giam co the anh huong toi do cao cua nut cha
				*pos=*tmp;//xoa pos, dua node len cho pos
				stockPointer=tmp;
				//Xoa vung nho cua tmp
//				delete tmp;
			}
			
		}
		else { //Lay nut cuc trai
			Node* tmp=GetMinValueNode(pos->right);//Nut cuc trai cua cay con ben phai
			pos->stock=tmp->stock;//Lay noi dung cu nut cuc trai
			
			pos->right=DeleteNode(tmp,stockPointer);//Xoa nut cuc trai;
			
	}
		pos->height=max(GetNodeHeight(pos->left),GetNodeHeight(pos->right))+1;
	int bf=GetNodeBalanceFactor(pos);	
		
	if(bf>1&&GetNodeBalanceFactor(pos->left)>=0){//LEFT LEFT
		return RightRotate(pos);
	}
	else if(bf>1&&GetNodeBalanceFactor(pos->left)<0){//LEFT RIGHT
		pos->left=LeftRotate(pos->left);
		return RightRotate(pos);
	}
	else if(bf<-1&&GetNodeBalanceFactor(pos->right)<=0){//RIGHT RIGHT
		return LeftRotate(pos);
	}
	else if(bf<-1&&GetNodeBalanceFactor(pos->right)>0){//RIGHT LEFT
		pos->right=RightRotate(pos->right);
		return LeftRotate(pos);
	}
	return pos;
}
void PrintOrder(Node* p){
	if(p!=NULL){
		cout<<p->stock.code<<' '<<p->stock.name<<' '<<p->stock.unit<<' '<<p->stock.amount<<endl;
		PrintOrder(p->left);
		PrintOrder(p->right);
	}
}

int partition ( Node * A[],int start ,int end) {
    int i = start + 1;
    char piv[51];
    strcpy(piv,A[start]->stock.name);         
    for(int j =start + 1; j <= end ; j++ )  {
          if (strcmp(A[j]->stock.name,piv)<0){
                 swap (A[ i ],A [ j ]);
            i += 1;
        }
   }
   swap ( A[ start ],A[ i-1 ]) ;  
   return i-1;                      
}

void Quick_sort ( Node* parr[] ,int start , int end ) {
	 cout<<"sfsdf"<<endl;
   if( start < end ) {
   	 cout<<"Vall"<<endl;
         int piv_pos = partition (parr,start , end ) ;     
         Quick_sort (parr,start , piv_pos -1);  
         Quick_sort ( parr,piv_pos +1 , end) ; 
   }
}


