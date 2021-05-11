#include"graphics.h"
#include"Header/Bill.h"
#include"Header/ALGORITHM.h"
#include"Header/MaterialAVL.h"
#include"Header/BASIC.h"

//

//////as
//

//
const int MAX_STAFF=500;
const int MAX_MATERIAL=1e7;
const int MAX_PRINT_BILL=1000;
const int MAX_PAYMENT=20;


//TextPointer 
class MaterialTab:public Tab{
	enum MATERIAL_TAB_STATES{ADD,EDIT,PRINT,EMPTY};
	
	public: MaterialTab(){};
	public: MaterialTab(int x,int y,int w,int h,int basiccolor,int oncolor,const char* txt):Tab(x,y,w,h,basiccolor,oncolor,txt){
		//Khoi tao data
		materialavl=NULL;
		
		//Khoi rao pointer
		state=EMPTY;

		fieldPointer=NULL;
		nodepointer=NULL;
		
		cnt=-1;//vi tri cuar parr
		
		parr=new MaterialNode*[MAX_MATERIAL];
		
		////Cua so ADD
		addButton=Trigger<MaterialTab>(95,62,140,30,addbutton_basic_color,addbutton_on_color,"Them vat tu",&MaterialTab::InitAddMaterial);
		editButton=Trigger<MaterialTab>(245,62,140,30,addbutton_basic_color,addbutton_on_color,"Hieu chinh",&MaterialTab::InitEdit);
		printButton=Trigger<MaterialTab>(395,62,140,30,addbutton_basic_color,addbutton_on_color,"In danh sach",&MaterialTab::InitPrint);
		
		
		//Nut chuc nang
		save=Trigger<MaterialTab>(screenWidth/2,520,680,40,tab_on_color,tab_basic_color,"Luu thay doi",&MaterialTab::Save);
		close=Trigger<MaterialTab>(983,243,40,40,close_basic_color,close_on_color,"X",&MaterialTab::Close,COLOR(100,100,100));
		
		//Input Field
		add[0]=InputField(700,280,30,sizeof(m_name)-1,field_basic_color,field_on_color,"Ten vat tu: ",m_name);
		add[1]=InputField(600,360,30,sizeof(m_unit)-1,field_basic_color,field_on_color,"Don vi tinh:",m_unit);
		add[2]=InputField(545,450,30,sizeof(m_quantity)-1,field_basic_color,field_on_color,"So luong:  ",m_quantity);
		add[3]=InputField(800,450,30,sizeof(m_materialcode)-1,field_basic_color,field_on_color,"Ma va tu: ",m_materialcode);
		
		//Cua so Edit
		code=InputField(655,370,30,sizeof(m_code)-1,field_basic_color,field_on_color,"Ma vat tu: ",m_code);
		findCode=Trigger<MaterialTab>(655,520,680,40,tab_on_color,tab_basic_color,"Tim vat tu",&MaterialTab::CheckCode);
		del=Trigger<MaterialTab>(655,450,680,40,tab_on_color,tab_basic_color,"Xoa vat tu",&MaterialTab::Delete);
		
		scroller=Scroller(1170,155,&i);
		
		ReadData();//Cho du lieu va
		
	};
	
	~MaterialTab(){
		for(int i=0;i<=cnt;i++) 
		delete parr[i];
		delete[] parr;
		//Giai phong cay
	}
	
	/// Cac chuc nang cua nut
	void InitAddMaterial(){
		Reset();
		state=ADD;
		fieldPointer=&add[0];
	}
	private: void InitPrint(){
		Reset();
//		sort(parr,parr+cnt+1,CompareByName);
		state=PRINT;
	}
	private: void InitEdit(){
		Reset();
		state=EDIT;
		fieldPointer=&code;
	}
	///////////////////////////////////////////////////////////////
	//Action cua tung chuc nang
	private: void AddMaterialAction(){
		//Ve Khung
		DrawInputWindow("Nhap thong tin vat tu");
		//
		add[0].Action(fieldPointer);
		add[1].Action(fieldPointer);
		add[2].Action(fieldPointer);
		add[3].Action(fieldPointer);
	
		save.Action(this);
		close.Action(this);
	}
	private: void PrintAction(){
		//
		int startX=screenWidth/2-830/2;
		int startY=95;//Do dich
		int height=28;//Do cao tung tung khoang cach in
		//MAVT:9 TENVT: 40 DONVI:30 SOLUONG:9 +5 khoang trang
		//BG
		setfillstyle(SOLID_FILL,smallwindow_bg_color);
		bar(startX,startY,startX+86*text_width,startY+height*21);
		//Title 
		setfillstyle(SOLID_FILL,title_bg_color);
		bar(startX,startY,startX+text_width*86,startY+height);
		setcolor(BLACK);
		setbkcolor(title_bg_color);
		outtextxy(startX+text_width*3.5f,startY+height/2-text_height/2,"Ma VT");
		outtextxy(startX+text_width*28,startY+height/2-text_height/2,"Ten VT");
		outtextxy(startX+text_width*61.5f,startY+height/2-text_height/2,"Don vi");
		outtextxy(startX+text_width*76.5f,startY+height/2-text_height/2,"So luong");
		
		//Ve danh sach
		setbkcolor(WHITE);
		for(int j=i;j<=cnt&&j-i+1<=20;j++){//In toi da 25 du lieu
			setcolor(BLACK);
			outtextxy(startX+text_width*1,startY+(j-i+1)*height+height/2-text_height/2,parr[j]->material.code);
			outtextxy(startX+text_width*12,startY+(j-i+1)*height+height/2-text_height/2,parr[j]->material.name);
			outtextxy(startX+text_width*54,startY+(j-i+1)*height+height/2-text_height/2,parr[j]->material.unit);
			char tmp[10];
			itoa(parr[j]->material.quantity,tmp,10);
			outtextxy(startX+text_width*76,startY+(j-i+1)*height+height/2-text_height/2,tmp);
		
		}

		
		setcolor(bg_border_color);
		rectangle(startX,startY,startX+text_width*86,startY+height*21);
		line(startX+text_width*11,startY,startX+text_width*11,startY+21*height);
		line(startX+text_width*53,startY,startX+text_width*53,startY+21*height);
		line(startX+text_width*75,startY,startX+text_width*75,startY+21*height);
		
		
		scroller.Action(cnt);
		
	}
	
	//Edit
	private: void EditCheck(){
		DrawInputWindow("Nhap vao ma vat tu");
		////////////////////
		code.Action(fieldPointer);
		close.Action(this);
		findCode.Action(this);	
	}
	private: void Edit(){//Sau khi tim code thanh cong
		DrawInputWindow("Sua doi vat tu");
		//
		add[0].Action(fieldPointer);
		add[1].Action(fieldPointer);
		
		
		setcolor(GREEN);
		int w=textwidth("--------------- OR ---------------");
		outtextxy(screenWidth/2-w/2,475,"--------------- OR ---------------");
		
		save.Action(this);
		del.Action(this);
		close.Action(this);
	
	}
	private: void CheckCode(){//Nut findCode
		if(!code.CheckParseString()) {
			return;
		}
		nodepointer=SearchNode(materialavl,m_code);
		
		if(nodepointer!=NULL){//Tim thay ma vat tu thao tac tren ma vattu
				//Khoi tao cho edit
				add[0].CustomInit(nodepointer->material.name);
				add[1].CustomInit(nodepointer->material.unit);
				fieldPointer=NULL;
		}
		else code.Anouncement("Khong tim thay ma vat tu nay!!!",false);
//		PrintOrder(materialavl);
			//
	}
	private: void Delete(){
		if(nodepointer==NULL){
			return ;
		}
		else{
			MaterialNode* deletepos=DeleteNode(materialavl,nodepointer->material);
			
			
			//dich qua trai
			int i=cnt;
			MaterialNode* last=NULL;
			while(parr[i]!=deletepos){
				MaterialNode* tmp=parr[i];
				parr[i]=last;
				last=tmp;
				--i;
			}
			parr[i]=last;
			--cnt;
			delete deletepos;
			ChangeMessage("Xoa thanh cong");
			Reset();
		}
	}
	
	public: MaterialNode* FindMaterialCode(const char* code){
		return SearchNode(materialavl,code);
	}
	
	//Dung chung
	private: void Save(){//them hoac sua mot material moi
	
		if(nodepointer==NULL){//Truong hop them
				if(!add[0].CheckParseString()||!add[1].CheckParseString()||!add[2].CheckParseInt()||!add[3].CheckParseString()) return;
				MaterialNode* dup=SearchNode(materialavl,m_materialcode);
				if(dup!=NULL){
					add[3].Anouncement("Da ton tai ma vat tu",false);
					return;
				}
				//Tao mot material moi
				Material newmaterial;
				add[3].ParseString(newmaterial.code);
				if(strlen(newmaterial.code)==0){
					add[3].Anouncement("Qua nhieu space!!!",false);
					return;
				}
				add[0].ParseString(newmaterial.name);
				add[1].ParseString(newmaterial.unit);
				newmaterial.quantity=add[2].ParseInt();
			
				MaterialNode* newnode=InsertNode(materialavl,newmaterial);
				
				//Chinh lai danh sach
				++cnt;		
				int i=cnt;//Ham InsertNode da tang cnt len roi nen cnt chinh la diem cuoi cua danh sach, va diem cuoi la diem rong khong co gi
				while(i-1>=0&&stricmp(parr[i-1]->material.name,newnode->material.name)>0) {
					parr[i]=parr[i-1];
					--i;
				}
				parr[i]=newnode;
				
				
			}
			else {//Truong hop Edit
				if(!add[0].CheckParseString()||!add[1].CheckParseString()) return;
				add[0].ParseString(nodepointer->material.name);
				add[1].ParseString(nodepointer->material.unit);
				quickSortMaterial(parr,0,cnt);
			}
		ChangeMessage("Luu du lieu thanh cong");
		Reset();		
	}
	private: void Close(){
		state=EMPTY;
	}
	private: void Reset(){
		fieldPointer=&add[0];
		nodepointer=NULL;
		
		add[0].BlankField();
		add[1].BlankField();
		add[2].BlankField();
		add[3].BlankField();
		code.BlankField();
		
		i=0;
	}
	//Thuc thi///////////////////
	public: void Action(){
//		DrawUI();
		
		addButton.Action(this);
		editButton.Action(this);
		printButton.Action(this);

		
		switch (state){
			case ADD:{
				AddMaterialAction();
				break;
			}
			case EDIT:{
				if(nodepointer==NULL){
					EditCheck();//Kiem tra ma nhap vao
				}
				else {
					Edit();
				}
				break;
			}
			case PRINT:{
				PrintAction();
				break;
			}
		}
		
		//Thuc thi field
		int c=GetInput();
		if(fieldPointer!=NULL) {
			if(c==-1) fieldPointer->DeleteChar();
			else if(c==1||c==2){
				if(state==ADD){
					int index=fieldPointer-add;
					if(c==1) --index;
					else ++index;
					index=max(0,index);
					index=min(index,3);
					fieldPointer=&add[index];
				}
				if(state==EDIT){
					if(nodepointer!=NULL){
					int index=fieldPointer-add;
					if(c==1) --index;
					else ++index;
					index=max(0,index);
					index=min(index,1);
					fieldPointer=&add[index];
				}
				}
			}
			else if(c!=0) fieldPointer->AddChar((char)c);	
		}
	}
	/////////////////////
	///Ve
	
	private: void DrawUI(){
		Tab::DrawUI();
	}
//
	//
	//Lay du lieu tu file///
	public : void ReadData(){
		ifstream data("data.txt",ios::binary);
		Material material;
		while(true){
			data.read(reinterpret_cast<char *>(&material), sizeof(Material));
			if(data.eof()) break;
			
			parr[++cnt]=InsertNode(materialavl,material);
		}
		data.close();//
		quickSortMaterial(parr,0,cnt);
	}
	//Quit
	public: void SaveAndQuit(){
		fstream data("data.txt",ios::out|ios::trunc|ios::binary);
		
		for(int i=0;i<=cnt;i++){
			data.write(reinterpret_cast<char *>(&parr[i]->material), sizeof(parr[i]->material));
		}
		data.close();
	}


	MATERIAL_TAB_STATES state;
	
	//Pointer
	InputField* fieldPointer;
	
	//Nut thuc thi cua so chuc nang
	Trigger<MaterialTab> addButton,editButton,printButton;
	
	////////Cua so ADD
	InputField add[4];//name, unit, quantity,material code
	
//	InputField name,unit,quantity;
	////////
	
	//Dung chung
	Trigger<MaterialTab> save,close;
	
	
	//Cua so EDIT
	Trigger<MaterialTab> del,findCode;
	InputField code;
	char m_code[10];
	
	MaterialNode* nodepointer;
	///////////
	
	//Cua so Print
	MaterialNode** parr;//dung de sap xep mang toi da 2 trieu phan tu
	int i;//index de in 
	Scroller scroller;
	
	
	//field temp data
	char m_name[41];
	char m_unit[21];
	char m_quantity[10];
	char m_materialcode[10];
	
	///DATA
	MaterialNode* materialavl;
	int cnt;
};
class StaffTab:public Tab
{
	public:
	~StaffTab(){
		for(int j=0;j<i;j++)
			delete NV[j];
	}
	enum Change{ADD,EDIT,PRINT,EMPTY,SEARCH};
	enum Phai{NAM,NU};
	public: StaffTab(int x,int y,int w,int h,int basiccolor,int oncolor,char *txt):Tab(x,y,w,h,basiccolor,oncolor,txt)
	{
		change=EMPTY;
		phai=NAM;
		
		
		editPos=-1;
		i=0;
		
		

		
		
		add	 =Trigger<StaffTab>(95,62,142,30,addbutton_basic_color,addbutton_on_color,"Them nhan vien",&StaffTab::addOnclick);
		edit =Trigger<StaffTab>(245,62,140,30,addbutton_basic_color,addbutton_on_color,"Hieu chinh",&StaffTab::editOnclick);
		print=Trigger<StaffTab>(395,62,140,30,addbutton_basic_color,addbutton_on_color,"In danh sach",&StaffTab::printOnclick);
		close=Trigger<StaffTab>(983,243,40,40,close_basic_color,close_on_color,"X",&StaffTab::Close,COLOR(100,100,100));
		nam	 =Trigger<StaffTab>(520,410,100,30,tab_on_color,tab_basic_color,"Nam",&StaffTab::namOnclick);
		nu	 =Trigger<StaffTab>(520,410,100,30,pink_basic_color,pink_on_color,"Nu",&StaffTab::nuOnclick);
		save =Trigger<StaffTab>(screenWidth/2,520,680,40,tab_on_color,tab_basic_color,"Save",&StaffTab::Save);
		Delete =Trigger<StaffTab>(screenWidth/2,470,680,40,tab_on_color,tab_basic_color,"Delete",&StaffTab::Xoa);
		search =Trigger<StaffTab>(screenWidth/2,520,680,40,tab_on_color,tab_basic_color,"Search",&StaffTab::Search);
		
		baokiin=NULL;
		text[0]  =InputField(550,270,30,sizeof(ho)-1,field_basic_color,field_on_color,"Ho:",ho);
		text[2]  =InputField(625,340,30,sizeof(ten)-1,field_basic_color,field_on_color,"Ten chinh:",ten);
		text[1]=InputField(800,270,30,sizeof(dem)-1,field_basic_color,field_on_color,"Ten dem:",dem);
		edtxtEdit   =InputField(700,350,30,sizeof(maNV)-1,field_basic_color,field_on_color,"Ma Nhan Vien:",maNV);
		
		scroller=Scroller(1080,150,&firstScoller);	
		// thanh scroller
		
		ReadNV();

		
	};
//================================================= ham xu li button =============================================================
	public:void Xoa()
	{
		if(NV[editPos]->billList!=NULL){
			ChangeMessage("Khong the xoa nhan vien nay");
			return;
		}
	
			
		for(int k=editPos;k<i-1;k++)
		{
				NV[k]=NV[k+1];
				
		}
		i--;
		////////////////////
		change=EMPTY;//
	}
	
	////////////////////////////////////////Ham bo tro duoc BillTab goi
	
	public: int Checkma(char *maNV)
	{
		string ma(maNV);
		for(int j=0;j<i;j++)
			if(stricmp(maNV,NV[j]->manv.c_str())==0) return  j;
		return -1;
	}
	public: BillNode* GetBillList(int staffpos){
		if(staffpos<0||staffpos>=i) return NULL;//LOI
		
		return NV[staffpos]->billList;
	}
	public:int getsize()
	{
		return i;
	}
	public: void InsertNewBill(int staffpos,Bill* newbill){
		InsertBill(NV[staffpos]->billList,newbill);
	}
	

	
	public: void Search(){
		if(!edtxtEdit.CheckParseString()) return;
		
		string ma(maNV);
		
		for(int j=0;j<i;j++)
		{
			if(strlwr((char *)NV[j]->manv.c_str())==ma) 
			{
				baokiin=NULL;
				
				
				editPos=j;

				text[2].CustomInit((char*)NV[j]->first.c_str());
				text[1].CustomInit((char*)NV[j]->middle.c_str());
				text[0].CustomInit((char*)NV[j]->last.c_str());
				change=SEARCH;
				return;
				///
			}
//		}
		}
		edtxtEdit.Anouncement("Khong tim thay ma nhan vien nay",false);
	}
	public: void addOnclick()																								  
	{
		
		change=ADD;
		editPos=-1;
		Reset();
			baokiin=&text[0];
			
	}
	public: void editOnclick()
	{
		change=EDIT;
		Reset();
		editPos=-1;
		baokiin=&edtxtEdit;
	}
	public: void printOnclick()
	{
		change=PRINT;
	
		
	}
	public: void Close()
	{
		change=EMPTY;
	}
	public: void namOnclick()
	{
		phai=NU;
	}
	public: void nuOnclick()
	{
		phai=NAM;
	}
	public: void Save()
	{
		//
			if(!text[0].CheckParseString()||!text[1].CheckParseString()||!text[2].CheckParseString()) return;
				
//			if(Ho==ChuanHoa(Ho)&&Ten==ChuanHoa(Ten)&&Dem==ChuanHoa(Dem)){
			string Ho,Ten,Dem,Phai;
			Ho.resize(50),Ten.resize(50),Dem.resize(50);
			text[0].ParseString((char*)Ho.c_str());
			text[1].ParseString((char*)Dem.c_str());
			text[2].ParseString((char*)Ten.c_str());

			//
			if(change==ADD) //trang thai them nhan vien 
			{
				NextString(curS,sizeof(curS));

				string ss(curS);
				if(phai==NU) Phai="Nu";
				else Phai="Nam";
				NV[i]=new nhanvien(Ho,Ten,Dem,Phai,ss);
				quickSort(NV,0,i);
				i++;
 			}
 			else if(change==SEARCH){//Thang thai edit
 				if(phai==NU)
 				NV[editPos]->add(Ho,Ten,Dem,"Nu",NV[editPos]->manv);
 				else NV[editPos]->add(Ho,Ten,Dem,"Nam",NV[editPos]->manv);
 				quickSort(NV,0,i-1);
			 }
			 change=EMPTY;
			 //
//
			////
	}
	/////////////Danh sach nhan vien co con tro toi hoa don
	//
	// nhap txt
	public:void infield()
	{
		//
			int c=GetInput();
			
			
			if(c==-1) baokiin->DeleteChar();
			else if(c==1||c==2){
				if(change==ADD){
					int index=baokiin-text;
					if(c==1) --index;
					else ++index;
					index=max(0,index);
					index=min(index,2);
					baokiin=&text[index];
				}

			}
			else if(c!=0) baokiin->AddChar((char)c);	
		
	}
	public: void thongtin()
	{
			
			text[0].Action(baokiin);
			text[1].Action(baokiin);
			text[2].Action(baokiin);

			setcolor(BLACK);
			outtextxy(360,400,"Gioi tinh:");
			if(phai==NAM) nam.Action(this);
			else nu.Action(this);
			save.Action(this);
			
		
		
			
			//=================== check text ===========================
			
		
	}
	
//----------------------------------------------------------------------------------------------------------------------------



//====================================================== MAIN ================================================================
	public: void Action()
	{
		add.Action(this);
		edit.Action(this);
		print.Action(this);
		
//
		if(change==ADD)
		{
			DrawInputWindow("Nhap vao thong tin nhan vien");
			DrawSmallWindow();
			thongtin();
		}
		else if (change==EDIT)
		{
			DrawInputWindow("Nhap vao ma nhan vien");
			DrawSmallWindow();
			edtxtEdit.Action(baokiin);
			search.Action(this);
		}
		else if(change==SEARCH)
		{
			DrawInputWindow("Sua doi nhan vien");
			thongtin();
			//
			Delete.Action(this);
		}
		else if (change==PRINT)
		{
			int x=270,y=90;
			int height=28;
			
				//30 15 10
				//================ ve khung ================================
				setfillstyle(SOLID_FILL,smallwindow_bg_color);
				bar(x,y,x+text_width*77,y+height*21);
				setfillstyle(SOLID_FILL,title_bg_color);
				bar(x,y,x+text_width*77,y+height);
				setcolor(BLACK);
				setbkcolor(title_bg_color);
				outtextxy(x+text_width,y+height/2-text_height/2,"Ma NV");
				outtextxy(x+text_width*14,y+height/2-text_height/2,"Ho");
				outtextxy(x+text_width*25,y+height/2-text_height/2,"Ten dem");
				outtextxy(x+text_width*48,y+height/2-text_height/2,"Ten");
				outtextxy(x+text_width*66.5f,y+height/2-text_height/2,"Gioi tinh");
				setcolor(bg_border_color);
				rectangle(x,y,x+text_width*77,y+height*21);
				line(x+text_width*7,y,x+text_width*7,y+21*height);
				line(x+text_width*23,y,x+text_width*23,y+21*height);
				line(x+text_width*34,y,x+text_width*34,y+21*height);
				line(x+text_width*65,y,x+text_width*65,y+21*height);
				//================== danh sach ==========================
				setcolor(BLACK);
				setbkcolor(WHITE);
				//
				for(int j=firstScoller;j<i&&j-firstScoller+1<=20;j++)
				{
				outtextxy(x+text_width,y+(j-firstScoller+1)*height+height/2-text_width/2,(char *)NV[j]->manv.c_str());
				outtextxy(x+text_width*8,y+(j-firstScoller+1)*height+height/2-text_width/2,(char *)NV[j]->last.c_str());
				outtextxy(x+text_width*24,y+(j-firstScoller+1)*height+height/2-text_width/2,(char *)NV[j]->middle.c_str());
				outtextxy(x+text_width*35,y+(j-firstScoller+1)*height+height/2-text_width/2,(char *)NV[j]->first.c_str());
				outtextxy(x+text_width*66,y+(j-firstScoller+1)*height+height/2-text_width/2,(char *)NV[j]->sex.c_str());
				}
				////
	

				pageup.Action(this);
				pagedown.Action(this);
				
				scroller.Action(i-1);
			
		}
		infield();
	}
//---------------------------------------------------------------------------------------------------------------------------

//================================================== Ham Phu ================================================================
	// cua so nho
	private: void DrawSmallWindow(){
		
		//
		close.Action(this);	
		//
		}
		//========================= reset edit text =================================
		private: void Reset(){
		baokiin=NULL;
		text[0].BlankField();
		text[1].BlankField();
		text[2].BlankField();
		edtxtEdit.BlankField();
		
		firstScoller=0;
		}
public:void ReadNV()
	{//

		fstream f;
		f.open("NV.txt", ios::in);
		string ten,ho,dem,gioitinh,ss;
		while(!f.eof()&&getline(f,ss))
		{	
				
			fflush(stdout); 
			getline(f,ho);
			getline(f,dem);
			getline(f,ten);
			getline(f,gioitinh);
			NV[i]=new nhanvien(ho,ten,dem,gioitinh,ss);
			
			i++;
		}
		f.close();
		quickSort(NV,0,i-1);
		
		
		for(int j=0;j<i;j++){
		Bill bill;
		string path=NV[j]->manv+".txt";
		FILE* file=fopen((char*)path.c_str(),"r");
		char type;
		
		char tmpcode[20];
		int tmpquantity;
		int tmpprice;
		int tmpvat;
		
		int n;
	//	//
		while(!feof(file)){
		fscanf(file,"%s %c %d/%d/%d %d \n",bill.code,&type,&bill.time.date,&bill.time.month,&bill.time.year,&n);
		if(feof(file)) break;
		for(int i=0;i<=n;i++){
			fscanf(file,"%s %d %d %d \n",tmpcode,&tmpquantity,&tmpprice,&tmpvat);
			bill.AddBillDetail(tmpcode,tmpquantity,tmpprice,tmpvat);
		}
		if(type=='X') bill.billType=X;
		else bill.billType=N;

		InsertBill(NV[j]->billList,&bill);
		bill.ResetBill();
		}
		fclose(file);
		}
//	
}
//
	public:void writeNV()
	{
		fstream f;
		f.open("NV.txt", ios::out);
		string ten,ho,dem,gioitinh,ss;
		for(int j=0;j<i;j++)
		{
			f<<NV[j]->manv<<endl;
			f<<NV[j]->last<<endl;
			f<<NV[j]->middle<<endl;
			f<<NV[j]->first<<endl;
			f<<NV[j]->sex<<endl;
		
		}
		f.close();
		////
	for(int j=0;j<i;j++)
	{
		string path=NV[j]->manv+".txt";
		FILE* file=fopen((char*)path.c_str(),"w");
		BillNode* k=NV[j]->billList;
		WriteData(k,file);///In cac so hoa don nho ra truoc khi doc vao thi so hoa don se giam dan
		fclose(file);
	}
	}
	void WriteData(BillNode* node,FILE* &file){//Ghi so hoa don nho truoc 
		if(node==NULL) return;
		WriteData(node->next,file);
		Bill& bill=node->bill;
		char type;
		if(bill.billType==X) type='X';
		else type='N';
		fprintf(file,"%s %c %d/%d/%d %d \n",bill.code,type,bill.time.date,bill.time.month,bill.time.year,bill.detailList->n);
		for(int i=0;i<=bill.detailList->n;i++){
			fprintf(file,"%s %d %d %d \n",bill.detailList->billDetail[i].code,bill.detailList->billDetail[i].quantity,bill.detailList->billDetail[i].price,bill.detailList->billDetail[i].vat);
		}
	}
	
	

//

	
	
	//
//---------------------------------------------------------------------------------------------------------------------------
public:
//================================================== EDITTEXT ===============================================================

	InputField *baokiin;
	InputField text[3];// edtxtLastName, edtxtMiddleName,edtxtFirstName,

	InputField edtxtEdit;
	
	char ten[30],ho[15], dem[10],maNV[6];

//---------------------------------------------------------------------------------------------------------------------------
//



//=================================================== BUTTON ================================================================
	Trigger<StaffTab> add;
	Trigger<StaffTab> edit;
	Trigger<StaffTab> print;
	Trigger<StaffTab> close;
	Trigger<StaffTab> nam;
	Trigger<StaffTab> nu;
	Trigger<StaffTab> save;
	Trigger<StaffTab> Delete;
	Trigger<StaffTab> search;
	Trigger<StaffTab> pageup,pagedown;
	
//---------------------------------------------------------------------------------------------------------------------------
	Change change;
	Phai phai;
	nhanvien* NV[500];
	int i,editPos;
	int firstScoller;
	Scroller scroller;
	
	
};


class BillTab:public Tab{
	enum BILL_TAB_STATES{IN_MATERIAL,OUT_MATERIAL,PRINT_BY_CODE,CHECK_PRINT_BY_TIME,PRINT_BY_TIME,PRINT_TOP,EMPTY};
	public: BillTab(){};
	~BillTab(){
//		DeleteTmpCache(billList);
	}

	public: BillTab(int x,int y,int w,int h,int basiccolor,int oncolor,const char* txt,MaterialTab* materialtab,StaffTab* stafftab):Tab(x,y,w,h,basiccolor,oncolor,txt){
		fieldPointer=NULL;
//		billList=NULL;
		billPointer=NULL;
		nv=NULL;
		state=EMPTY;
		t=NULL;
		indexBillInTime=-1;
		
		materialTab=materialtab;//d
		staffTab=stafftab;
		
		//Thanh cong cu 
		inButton=Trigger<BillTab>(95,62,140,30,addbutton_basic_color,addbutton_on_color,"Hoa don nhap",&BillTab::InitIn);
		outButton=Trigger<BillTab>(245,62,140,30,addbutton_basic_color,addbutton_on_color,"Hoa don xuat",&BillTab::InitOut);
		recentBillPrintButton=Trigger<BillTab>(395,62,140,30,addbutton_basic_color,addbutton_on_color,"In hoa don",&BillTab::InitPrintByCode);
		
		
		close=Trigger<BillTab>(1279,107,30,30,close_basic_color,close_on_color,"X",&BillTab::Close,COLOR(100,100,100));
		closeSmall=Trigger<BillTab>(990,236,30,30,close_basic_color,close_on_color,"X",&BillTab::Close,COLOR(100,100,100));
		
			
		//////////////////////////////////////////////NHAP XUAT/////////////////////////////////////////////////////////////////////////
		//Ngay hoa don
		int X=520;
		int Y=110;
		field[0]=InputField(X-10,Y,30,sizeof(m_staffCode)-1,field_basic_color,field_on_color,"Ma nhan vien: ",m_staffCode);
		field[1]=InputField(X+210,Y,30,sizeof(m_date)-1,field_basic_color,field_on_color,"Ngay:",m_date);
		field[2]=InputField(X+400,Y,30,sizeof(m_month)-1,field_basic_color,field_on_color,"Thang:",m_month);
		field[3]=InputField(X+620,Y,30,sizeof(m_year)-1,field_basic_color,field_on_color,"Nam:",m_year);
		
		//Pha them thanh toan
		int X2=250;
		field[4]=InputField(X2,175,30,sizeof(m_materialCode)-1,field_basic_color,field_on_color,"Ma vat tu:",m_materialCode);
		field[5]=InputField(X2+270,175,30,sizeof(m_quantity)-1,field_basic_color,field_on_color,"So luong:",m_quantity);
		field[6]=InputField(X2+540,175,30,sizeof(m_price)-1,field_basic_color,field_on_color,"Don gia:",m_price);
		field[7]=InputField(X2+750,175,30,sizeof(m_vat)-1,field_basic_color,field_on_color,"VAT(%):",m_vat);
		///sadf
			
		addPayment=Trigger<BillTab>(1180,175,170,30,addbutton_basic_color,addbutton_on_color,"Them thanh toan",&BillTab::AddPayment);
		
		makeInvoice=Trigger<BillTab>(655,663,200,30,addbutton_basic_color,addbutton_on_color,"Thanh toan hoa don",&BillTab::MakeInvoice);
		
		int startX=20;
		int startY=220;
		int height=20;
		for(int i=0;i<20;i++){
			deletePayment[i]=Trigger<BillTab>(1265,startY+height*(i+1)+height/2,18,18,deletepayment_basic_color,deletepayment_on_color,"x",&BillTab::DeletePayment);
		}
		
		//////////////////////////////////////////////////////IN HOA DON///////////////////////////////////////////////////////////////////////////
		enterBillCode=InputField(screenWidth/2+20,370,30,sizeof(m_billCode)-1,field_basic_color,field_on_color,"Nhap vao so hoa don",m_billCode);
		checkBillCode=Trigger<BillTab>(screenWidth/2,520,680,40,tab_on_color,tab_basic_color,"Tim kiem hoa don",&BillTab::CheckBillCode);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		////////////////////////////////////////////////////THONG KE HOA DON THEO THOI GIAN///////////////////////////////////////////////////////
		fromtofield[0]=InputField(540,310,30,sizeof(m_fromdate)-1,field_basic_color,field_on_color,"Tu:     ngay",m_fromdate);
		fromtofield[1]=InputField(680,310,30,sizeof(m_frommonth)-1,field_basic_color,field_on_color,"thang",m_frommonth);
		fromtofield[2]=InputField(820,310,30,sizeof(m_fromyear)-1,field_basic_color,field_on_color,"nam",m_fromyear);
		fromtofield[3]=InputField(540,410,30,sizeof(m_todate)-1,field_basic_color,field_on_color,"Den:     ngay",m_todate);
		fromtofield[4]=InputField(680,410,30,sizeof(m_tomonth)-1,field_basic_color,field_on_color,"thang",m_tomonth);
		fromtofield[5]=InputField(820,410,30,sizeof(m_toyear)-1,field_basic_color,field_on_color,"nam",m_toyear);
		
		// thanh scroller
		scroller=Scroller(1230,170,&firstScoller);
		
		
		
		//------------------------------------------ Thong Ke -----------------------------------------------------------------
		thongke=Trigger<BillTab>(545,62,140,30,addbutton_basic_color,addbutton_on_color,"Thong ke",&BillTab::ThongKe);
		TKHD=Trigger<BillTab>(screenWidth/2.5,500,200,40,addbutton_basic_color,addbutton_on_color,"THONG KE HOA DON",&BillTab::CheckBillTime);
		TKVT=Trigger<BillTab>(screenWidth/1.6,500,200,40,addbutton_basic_color,addbutton_on_color,"TOP 10 VAT TU ",&BillTab::check);
	

		
	}

	//////////////////////////////////////////////HOAT DONG NHAP,XUAT///////////////////////////////////////////////////////
	private: void InitIn(){
		state=IN_MATERIAL;
		
		
		
		currentBill.billType=N;
		
		field[0].BlankField();
		LoadTime(field[1],field[2],field[3]);
		
		ClearOldBill();
			fieldPointer=&field[0];
	}
	private: void InitOut(){
		state=OUT_MATERIAL;
		

		field[0].BlankField();
		LoadTime(field[1],field[2],field[3]);
		
		currentBill.billType=X;
		
		ClearOldBill();
			fieldPointer=&field[0];
	}
	/////////////////////
	private: void InOutAction(){
		DrawBillWindow();
		setcolor(bg_border_color);
		///date and time
		//
		rectangle(600,90,screenWidth-70,150);
		line(300,90,300,150);
		line(815,90,815,150);
		line(1030,90,1030,150);
		////
		rectangle(20,150,screenWidth-20,215);
		
			setcolor(BLACK);
		setbkcolor(smallwindow_bg_color);
		char type[30];
		if(state==OUT_MATERIAL) strcpy(type,"HOA DON XUAT");
		else strcpy(type,"HOA DON NHAP");
		outtextxy(160-textwidth(type)/2,120-text_height/2,type);
		
		
		//Ngay thang nam
		field[0].Action(fieldPointer);
		field[1].Action(fieldPointer);
		field[2].Action(fieldPointer);
		field[3].Action(fieldPointer);
		/////////////////
		field[4].Action(fieldPointer);//Ma vat tu
		//Thong tin con lai
		field[5].Action(fieldPointer);
		field[6].Action(fieldPointer);
		field[7].Action(fieldPointer);
		
		addPayment.Action(this);
		//
		makeInvoice.Action(this);//Thanh toan hoa don
		
		PrintPayment();
	}
	private: void AddPayment(){//Them thanh toan
		if(currentBill.detailList->n==19) {
			ChangeMessage("Hoa don day!!!");
			return;
		}
		if(!field[4].CheckParseString()) return ;
		
		MaterialNode* findresult=materialTab->FindMaterialCode(m_materialCode);
		if(findresult==NULL) field[4].Anouncement("Khong ton tai ma vat tu nay!!!",false);
		else{
				//Kiem tra gia ca va so luong nhap vao
			if(!field[5].CheckParseInt()||!field[6].CheckParseInt()||!field[7].CheckParseInt()) return;
				
			for(int i=0;i<=currentBill.detailList->n;i++){//Kiem tra xem da co ma vat tu do o trong hoa don chua
				if(stricmp(findresult->material.code,materialPointer[i]->code)==0){
					field[4].Anouncement("Da co trong hoa don",false);
					return ;
				}
			}
			
			int amount=field[5].ParseInt();
		
			if(state==OUT_MATERIAL){
				if(amount>findresult->material.quantity) {
				char anoun[61];
				strcpy(anoun,"So luong hien tai:");
				char tmp[10];
				itoa(findresult->material.quantity,tmp,10);
				strcat(anoun,tmp);
					
				field[5].Anouncement(anoun,false); return;}
			}
			else if(state==IN_MATERIAL){	
				if(amount+findresult->material.quantity>MAX_QUANTITY) {
				ChangeMessage("So luong trong kho toi da 999999999");
				return;
				}
				
			}
			
			///Kiem tra tri gia hoa don co vuot qua gioi han hay khong
			long long value=(1LL*field[5].ParseInt()*field[6].ParseInt());
			if(value>MAX_PAYMENT_VALUE/(((double)100+field[7].ParseInt())/100)){//Dinh sai so roi value khong chi 100 thi max*100 cho khoi sai so
				ChangeMessage("Tri gia cua hoa don toi da 999999999");
				return ;
			}
//9
			
				cout<<"Here"<<endl;
				//Toi day la ok them vao danh sach
			currentBill.AddBillDetail(findresult->material.code,field[5].ParseInt(),field[6].ParseInt(),field[7].ParseInt());
			materialPointer[currentBill.detailList->n]=&findresult->material;
			ChangeMessage("Them hoa don thang cong");
			
			field[4].BlankField();
			field[5].BlankField();
			field[6].BlankField();
			field[7].BlankField();
		}
	}
	private: void MakeInvoice(){//Thanh toan hoa don
		if(currentBill.detailList->n==-1) {
			ChangeMessage("Khong co thanh toan");
			return;
		}
		if(!field[0].CheckParseString()) return;
		///
		int staffPos=staffTab->Checkma(m_staffCode);
		if(staffPos==-1)  field[0].Anouncement("Khong ton tai",false);
		else {
			//Kiem tra ngay thang
			if(!CheckTime(field[1],field[2],field[3])) return;
			currentBill.time=Time(field[1].ParseInt(),field[2].ParseInt(),field[3].ParseInt());
			
			NextString(curB,sizeof(curB));
			strcpy(currentBill.code,curB);
			if(state==IN_MATERIAL){
				for(int i=0;i<=currentBill.detailList->n;i++){
					materialPointer[i]->quantity+=currentBill.detailList->billDetail[i].quantity;
				}
			}
			else{
				for(int i=0;i<=currentBill.detailList->n;i++){
					materialPointer[i]->quantity-=currentBill.detailList->billDetail[i].quantity;
				}
				
			}
			/////////////Dua Billl vao
			///////////////////////
			staffTab->InsertNewBill(staffPos,&currentBill);
			currentBill.ResetBill();
			LoadTime(field[1],field[2],field[3]);
			ClearOldBill();
			ChangeMessage("Thanh toan thanh cong");
		}
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	////////////////////////////////////////////IN HOA DON THEO SO HOA DON///////////////////////////////////////////////////
	private:
	void InitPrintByCode(){
		enterBillCode.BlankField();
		state=PRINT_BY_CODE;
				
		billPointer=NULL;
		fieldPointer=&enterBillCode;
	}
	void EnterBillCode(){
		DrawInputWindow("Nhap vao ma hoa don");
		enterBillCode.Action(fieldPointer);
		checkBillCode.Action(this);
	}
	void CheckBillCode(){
		if(!enterBillCode.CheckParseString()) return;
		///tim kieme ma hoa don
		if(tolower(m_billCode[0])!='b') {
			enterBillCode.Anouncement("Ki tu dau phai la ki tu b",false);
			return;
		}
		for(int i=1;i<sizeof(m_billCode)-1;i++){
			if(!isdigit(m_billCode[i])){
				enterBillCode.Anouncement("Qua nhieu chu",false);
				return;
			}
		}
	
		////////////////////////////////////////////Kiem traa hoa don co ton tai hay khong
		
			int staffcount=staffTab->getsize();
		for(int i=0;i<staffcount;i++){
			BillNode*k=staffTab->GetBillList(i);
			//
			while(k!=NULL&&stricmp(k->bill.code,m_billCode)>0) k=k->next;
			//
			if(k!=NULL&&stricmp(m_billCode,k->bill.code)==0)
				{
					billPointer=&k->bill;
					nv=staffTab->NV[i];
					return;
				}					
						
		}
		enterBillCode.Anouncement("Khong tim thay hoa don",false);
		return ;

	}
	void PrintBill(){///////////////////////////////Nhap vao ma hoa don va in ra
		DrawStatiscalWindow();
		int startX=400;
		int startY=200;
		int height=20;
		//
		setbkcolor(smallwindow_bg_color);
		setcolor(BLACK);
		char text[20];
		
		if(billPointer->billType==N) strcpy(text,"HOA DON NHAP");
		else strcpy(text,"HOA DON XUAT");
		outtextxy(screenWidth/2-textwidth(text)/2,100,text);
		
		//Thoi gian hoa hoa don
		char tmp[50];
		strcpy(tmp,"Ngay lap hoa don : ");
		char datestr[15];
		billPointer->time.ParseString(datestr);
		strcat(tmp,datestr);
		outtextxy(startX+110,130,tmp);
		strcpy(tmp,"Ma nhan vien thanh toan: ");
		
		
		if(nv==NULL) strcat(tmp,"Thong tin nhan vien da bi xoa !");
		else strcat(tmp,(nv->manv).c_str());
//		strcat(tmp,billPointer->staff);
		outtextxy(startX+110,150,tmp);
		
		//Title
		setbkcolor(title_bg_color);
		setcolor(BLACK);
		setfillstyle(SOLID_FILL,title_bg_color);
		bar(startX,startY,startX+text_width*48,startY+height);
		outtextxy(startX+text_width*3,startY,"MA VT");
		outtextxy(startX+text_width*12.5f,startY,"SO LUONG");
		outtextxy(startX+text_width*24,startY,"DON GIA");
		outtextxy(startX+text_width*33.5,startY,"VAT");
		outtextxy(startX+text_width*37.5,startY,"THANH TIEN");
		
		//Border
		//(1,9,61,93)
		setcolor(bg_border_color);
		line(startX+text_width*11,startY,startX+text_width*11,startY+21*height);
		line(startX+text_width*22,startY,startX+text_width*22,startY+21*height);
		line(startX+text_width*33,startY,startX+text_width*33,startY+21*height);
		line(startX+text_width*37,startY,startX+text_width*37,startY+21*height);
		
		rectangle(startX,startY,startX+text_width*48,startY+21*height);
		rectangle(startX+text_width*37,startY+21*height,startX+text_width*48,startY+22*height);
		//(1,9,61,93)
		setbkcolor(smallwindow_bg_color);
		setcolor(BLACK);
		for(int i=0;i<=billPointer->detailList->n;i++){
			outtextxy(startX+text_width,startY+(i+1)*height,billPointer->detailList->billDetail[i].code);//9
			outtextxy(startX+text_width*12,startY+(i+1)*height,billPointer->detailList->billDetail[i].quantity);//9
			outtextxy(startX+text_width*23,startY+(i+1)*height,billPointer->detailList->billDetail[i].price);//9
			outtextxy(startX+text_width*34,startY+(i+1)*height,billPointer->detailList->billDetail[i].vat);
			outtextxy(startX+text_width*38,startY+(i+1)*height,billPointer->detailList->billDetail[i].total);
		}
		
		//Total
		outtextxy(startX+text_width*29,startY+21*height+height/2-text_height/2,"Total :");
		outtextxy(startX+text_width*38,startY+21*height+height/2-text_height/2,billPointer->total);
	}
	


	//////////////////////////////////////////////////////////HOA DON THEO THOI GIAN//////////////////////////////////////////////////////////////////////////
	private:void DrawBillsmall()
	{
		int width=700;
		
		setfillstyle(SOLID_FILL,smallwindow_bg_color);
		setcolor(smallwindow_bg_color);
		bar(screenWidth/2-width/2,220,screenWidth/2+width/2,550);
		
		setcolor(bg_border_color);
		rectangle(screenWidth/2-width/2,220,screenWidth/2+width/2,550);
		setfillstyle(SOLID_FILL,tab_basic_color);
		bar(500,200,800,240);
		
		closeSmall.Action(this);
	}
	
	
	private: 
		void ThongKe(){
			
			state=CHECK_PRINT_BY_TIME;
			fieldPointer=NULL;
			for(int i=0;i<=indexBillInTime;i++){
				billInTime[i]=NULL;
			}
			indexBillInTime=-1;
			
			fromtofield[0].BlankField();
			fromtofield[1].BlankField();
			fromtofield[2].BlankField();
			fromtofield[3].BlankField();
			fromtofield[4].BlankField();
			fromtofield[5].BlankField();
		
		}
		
		void CheckBillTime(){
//			
			bool valid=true;
			valid&=CheckTime(fromtofield[0],fromtofield[1],fromtofield[2])&CheckTime(fromtofield[3],fromtofield[4],fromtofield[5]);
			if(!valid) return;
			
			Time from=Time(fromtofield[0].ParseInt(),fromtofield[1].ParseInt(),fromtofield[2].ParseInt());
			Time to=Time(fromtofield[3].ParseInt(),fromtofield[4].ParseInt(),fromtofield[5].ParseInt());
//			
//			//

//			}
		for(int j=0;j<staffTab->getsize();j++)
		{
			BillNode *k=staffTab->GetBillList(j);
		
			while(k!=NULL)
			{

				Bill*tmp=&k->bill;
				if(CompareTime(tmp->time,from)>=0&&CompareTime(tmp->time,to)<=0)
				{
					billInTime[++indexBillInTime]=tmp;
					staffPointer[indexBillInTime]=staffTab->NV[j];
					
					
				}
					k=k->next;
				//
			}
		}
			state=PRINT_BY_TIME;
		}
		void RNL(TREE t)
		{
			if(t!=NULL)
			{
				RNL(t->pRight);
				
				a[n]=t;
				n++;
				RNL(t->pLeft);
			}
		}
		void check()
		{	
			
			n=0;
		khoitao(t);
		bool valid=true;
		valid&=CheckTime(fromtofield[0],fromtofield[1],fromtofield[2])&CheckTime(fromtofield[3],fromtofield[4],fromtofield[5]);
		if(!valid) return;
		
		Time from=Time(fromtofield[0].ParseInt(),fromtofield[1].ParseInt(),fromtofield[2].ParseInt());
		Time to=Time(fromtofield[3].ParseInt(),fromtofield[4].ParseInt(),fromtofield[5].ParseInt());

		for(int j=0;j<staffTab->getsize();j++)
		{
			BillNode *k=staffTab->GetBillList(j);
		
			while(k!=NULL)
			{//

				Bill*tmp=&k->bill;
				if(CompareTime(tmp->time,from)>=0&&CompareTime(tmp->time,to)<=0&&tmp->billType==X)
				{
						for(int j=0;j<=tmp->detailList->n;j++)
						{
						MaterialNode* cc=materialTab->FindMaterialCode(tmp->detailList->billDetail[j].code);
						if(cc==NULL)
							themNODE(t,tmp->detailList->billDetail[j].code,tmp->detailList->billDetail[j].total,NULL);
						themNODE(t,tmp->detailList->billDetail[j].code,tmp->detailList->billDetail[j].total,&cc->material);
				}
					
				}
						k=k->next;
				}
				
				//
			}
		
//		//
			RNL(t);
			heap_sort(a,n,10);
//			
			state=PRINT_TOP;
		}
	//
		void InputTime(){
			DrawBillsmall();
			setbkcolor(tab_basic_color);
		   	setcolor(WHITE);

		   	
			outtextxy(550,210,"NHAP KHOANG THOI GIAN");
			
			fromtofield[0].Action(fieldPointer);
			fromtofield[1].Action(fieldPointer);
			fromtofield[2].Action(fieldPointer);
			fromtofield[3].Action(fieldPointer);
			fromtofield[4].Action(fieldPointer);
			fromtofield[5].Action(fieldPointer);
			//
			TKHD.Action(this);
			TKVT.Action(this);
		}
	
//
	
	///////////////////////////////////////////////////////////////////PRINT_BY_TIME//////////////////////////////////////////////////////////////
	void PrintBillByTime(){
			 DrawStatiscalWindow();
				int x=130,y=134;
				int height=26;
			
				//================ ve khung ================================
				setcolor(RED);
				setbkcolor(WHITE);
				string announ="BANG THONG KE HOA DON TU : ";
				string s(m_fromdate),ss(m_frommonth),sss(m_fromyear),ssss=s+'/'+ss+'/'+sss;
				announ+=ssss;
				announ+=" DEN NGAY: ";
				string f(m_todate),ff(m_tomonth),fff(m_toyear),ffff=f+'/'+ff+'/'+fff;
				announ+=ffff;
				outtextxy(450,100,(char*)announ.c_str());
				///////
				//
				///
					setfillstyle(SOLID_FILL,title_bg_color);
					bar(x,y,x+text_width*106,y+height);
					setcolor(BLACK);
					setbkcolor(title_bg_color);
					outtextxy(x+text_width*7.f,y+height/2-text_height/2,"So HD");
					outtextxy(x+text_width*22,y+height/2-text_height/2,"Ngay Lap");
					outtextxy(x+text_width*33,y+height/2-text_height/2,"Loai");
					outtextxy(x+text_width*52,y+height/2-text_height/2,"Ho ten nhan vien nhap hoa don");
					outtextxy(x+text_width*97,y+height/2-text_height/2,"Tri gia");
			
					
					setcolor(bg_border_color);
					rectangle(x,y,x+text_width*106,y+height*21);
					line(x+text_width*21,y,x+text_width*21,y+21*height);
					line(x+text_width*32,y,x+text_width*32,y+21*height);
					line(x+text_width*39,y,x+text_width*39,y+21*height);
					line(x+text_width*95,y,x+text_width*95,y+21*height);
					

					for(int i=firstScoller;i<=indexBillInTime&&i-firstScoller+1<=20;i++)
					{
						//	
						string k;
						if(billInTime[i]->billType==N) k='N';
						else k='X';
						string q;
						if(staffPointer[i]!=NULL)
							q=chuanhoa(staffPointer[i]->last)+" "+chuanhoa(staffPointer[i]->middle)+" "+chuanhoa(staffPointer[i]->first);
						else
						{
							q="NHAN VIEN DA BI XOA";
						}
						setbkcolor(WHITE);
						setcolor(BLACK);
						outtextxy(x+text_width,y+height*(i-firstScoller+1)+height/2-text_height/2,billInTime[i]->code);
						
						char t[10];
						billInTime[i]->time.ParseString(t);
						outtextxy(x+text_width*22,y+height*(i-firstScoller+1)+height/2-text_height/2,t);
						outtextxy(x+text_width*34,y+height*(i-firstScoller+1)+height/2-text_height/2,(char*)k.c_str());
						outtextxy(x+text_width*40,y+height*(i-firstScoller+1)+height/2-text_height/2,(char*)q.c_str());
						outtextxy(x+text_width*96,y+height*(i-firstScoller+1)+height/2-text_height/2,(char*)Tostring(billInTime[i]->total).c_str());
						
					}
					scroller.Action(indexBillInTime);
				
		}
	
		
		///////////////////////////////////////////////////////PRINT_TOP_TEN/////////////////////////////////////////////////////////////////////////////////////////////////
		void PrintTop()
		{
			 	DrawStatiscalWindow();
			 
				int x=screenWidth/2-(text_width*73)/2,y=200;
				setcolor(RED);
				setbkcolor(WHITE);
				outtextxy(460,120,"10 VAT TU CO DOANH THU CAO NHAT");
				string s(m_fromdate),ss(m_frommonth),sss(m_fromyear),ssss=s+'/'+ss+'/'+sss;
				outtextxy(430,150,"Tu Ngay: ");
				outtextxy(600,150,"--Den Ngay: ");
				string f(m_todate),ff(m_tomonth),fff(m_toyear),ffff=f+'/'+ff+'/'+fff;
				outtextxy(520,150,(char*)ssss.c_str());
				outtextxy(720,150,(char*)ffff.c_str());
				
				setfillstyle(SOLID_FILL,title_bg_color);
				bar(x,y,x+text_width*73,y+height);
			
				setcolor(BLACK);
				setbkcolor(title_bg_color);
				outtextxy(x+text_width*1,y+height/2-text_height/2,"Ma vat tu");
				outtextxy(x+text_width*27,y+height/2-text_height/2,"Ten vat tu");
				outtextxy(x+text_width*56.5f,y+height/2-text_height/2,"Tong doanh thu");
			
	
				setcolor(bg_border_color);
				rectangle(x,y,x+text_width*73,y+11*height);
				line(x+text_width*11,y,x+text_width*11,y+11*height);
				line(x+text_width*53,y,x+text_width*53,y+11*height);
			
				setcolor(BLACK);
				setbkcolor(smallwindow_bg_color);
				
				int Y=240;
				
					for(int i=0;i<n&&i<10;i++)
				{//
					outtextxy(300,Y,(char*)a[n-1-i]->data.c_str());
					if(a[n-1-i]->material==NULL) 
						outtextxy(410,Y,"VAT TU DA BI XOA");
					else
						outtextxy(410,Y,a[n-1-i]->material->name);
					outtextxy(850,Y,a[n-1-i]->sl);
					Y+=30;
				}
			
		}
		
	

		
	
	
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////HAM BO TRO///////////////////////////////////////////////////////////////////////////////////
	///Ham nay duoc goi boi nut x trong danh sach payment	
	private: void LoadTime(InputField& date,InputField& month,InputField& year){
		date.CustomInit(currentTime.date);
		month.CustomInit(currentTime.month);
		year.CustomInit(currentTime.year);
	}
	//Ham ve


	
	
	//////////////////////CAC HAM VE//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	private: void DrawBillWindow(){
		setfillstyle(SOLID_FILL,WHITE);
		bar(15,90,screenWidth-15,screenHeight-15);
		setcolor(bg_border_color);
		rectangle(15,90,screenWidth-15,screenHeight-15);
		
		close.Action(this);
	}
	private: void PrintPayment(){///In cac thanh toan
		int startX=20;
		int startY=220;
		int height=20;
		//Title
		setcolor(BLACK);
		setbkcolor(title_bg_color);
		setfillstyle(SOLID_FILL,title_bg_color);
		bar(startX,startY,startX+text_width*127,startY+height);
		outtextxy(startX+text_width*3,startY,"MA VT");
		outtextxy(startX+text_width*29,startY,"TEN VT");
		outtextxy(startX+text_width*61,startY,"DON VI");
		outtextxy(startX+text_width*76.5f,startY,"SO LUONG");
		outtextxy(startX+text_width*88,startY,"DON GIA");
		outtextxy(startX+text_width*97.5f,startY,"VAT");
		outtextxy(startX+text_width*109.5f,startY,"THANH TIEN");
		
		
		//Border
		//(1,9,61,93)
		setcolor(bg_border_color);
		line(startX+text_width*11,startY,startX+text_width*11,startY+21*height);
		line(startX+text_width*53,startY,startX+text_width*53,startY+21*height);
		line(startX+text_width*75,startY,startX+text_width*75,startY+21*height);
		line(startX+text_width*86,startY,startX+text_width*86,startY+21*height);
		line(startX+text_width*97,startY,startX+text_width*97,startY+21*height);
		line(startX+text_width*101,startY,startX+text_width*101,startY+21*height);
		
		rectangle(startX+text_width*101,startY+21*height,startX+text_width*127,startY+22*height);//Khung cua tong 
		setbkcolor(smallwindow_bg_color);
		setcolor(BLACK);
		for(int i=0;i<=currentBill.detailList->n;i++){
			outtextxy(startX+text_width,startY+(i+1)*height+height/2-text_height/2,materialPointer[i]->code);
			outtextxy(startX+text_width*12,startY+(i+1)*height+height/2-text_height/2,materialPointer[i]->name);
			outtextxy(startX+text_width*54,startY+(i+1)*height+height/2-text_height/2,materialPointer[i]->unit);
			outtextxy(startX+text_width*76,startY+(i+1)*height+height/2-text_height/2,currentBill.detailList->billDetail[i].quantity);
			outtextxy(startX+text_width*87,startY+(i+1)*height+height/2-text_height/2,currentBill.detailList->billDetail[i].price);
			outtextxy(startX+text_width*98,startY+(i+1)*height+height/2-text_height/2,currentBill.detailList->billDetail[i].vat);
			outtextxy(startX+text_width*102,startY+(i+1)*height+height/2-text_height/2,currentBill.detailList->billDetail[i].total);
		}
		//Phan ghi tong tri gia hoa don
		outtextxy(startX+text_width*95,startY+21*height+height/2-text_height/2,"Tong:");
		outtextxy(startX+text_width*102,startY+21*height+height/2-text_height/2,currentBill.total);
		
		///Cac nut close cho tung payment
		for(int i=0;i<=currentBill.detailList->n;i++){
				deletePayment[i].Action(this,i);
		}///////////////////////////////
		setcolor(bg_border_color);
		rectangle(startX,startY,startX+text_width*127,startY+21*height);
	
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	public: void Action(){//Hoad dong chinh
		
		inButton.Action(this);
		outButton.Action(this);
		recentBillPrintButton.Action(this);
		billByTimeButton.Action(this);
		thongke.Action(this);
		
		switch (state){
			case IN_MATERIAL:{
				InOutAction();
				break;
			}
			case OUT_MATERIAL:{
				InOutAction();
				break;
			}
			case PRINT_BY_CODE:{
				if(billPointer==NULL) EnterBillCode();
				else PrintBill();
				break;
			}
			case CHECK_PRINT_BY_TIME:{
				
				InputTime();
				break;
			}
			case PRINT_BY_TIME:{
				PrintBillByTime();
				break;
			}
			case PRINT_TOP:{
				PrintTop();
				break;
			}
		}
		//
		
		int c=GetInput();
		if(fieldPointer!=NULL) {
			if(c==-1) fieldPointer->DeleteChar();
			else if(c==1||c==2){
				if(state==IN_MATERIAL||state==OUT_MATERIAL){
					int index=fieldPointer-field;
					if(c==1) --index;
					else ++index;
					index=max(0,index);
					index=min(index,7);
					fieldPointer=&field[index];
				}
				else if(state==CHECK_PRINT_BY_TIME){
					int index=fieldPointer-fromtofield;
					if(c==1) --index;
					else ++index;
					index=max(0,index);
					index=min(index,5);
					fieldPointer=&fromtofield[index];
				}
			}
			else if(c!=0) fieldPointer->AddChar((char)c);	
		}
	}
	
	///////////////////////////////HAM BO TRO//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Close(){
		state=EMPTY;
	}
	private: void DeletePayment(int index){
		for(int i=index;i<currentBill.detailList->n;i++){
			materialPointer[i]=materialPointer[i+1];
		}
		materialPointer[currentBill.detailList->n]=NULL;
		currentBill.DeleteBillDetail(index);
	}
	
	private: void ClearOldBill(){
		field[0].BlankField();
		field[5].BlankField();
		field[6].BlankField();
		field[7].BlankField();
		
		field[4].BlankField();
		fieldPointer=NULL;
		currentBill.ResetBill();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	public: void SaveAndQuit(){
		staffTab->writeNV();
		materialTab->SaveAndQuit();
		WriteString();
		isActive=false;
	}
	
		
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Doc ghi file
		
	 Bill* billPointer;//Che do in theo so hoa don
	
	BILL_TAB_STATES state;
	
	Bill currentBill;
	
	
	Bill* billInTime[MAX_PRINT_BILL];//Dung cho ca top ten, billintime
	const nhanvien* staffPointer[MAX_PRINT_BILL];
	int indexBillInTime;
	
	
	
	Material* materialPointer[MAX_PAYMENT];//Thay doi du lieu cua Material Tab
	Trigger<BillTab> deletePayment[MAX_PAYMENT];
	Trigger<BillTab> close;
	Trigger<BillTab> closeSmall;
	Trigger<BillTab> makeInvoice;
	
	Trigger<BillTab> inButton;
	Trigger<BillTab> outButton;
	Trigger<BillTab> recentBillPrintButton;
	Trigger<BillTab> billByTimeButton;
	Trigger<BillTab> toptenButton;
	
	InputField* fieldPointer;
	
	
	InputField enterBillCode;
	

	Trigger<BillTab> checkBillCode;
	
	const nhanvien* nv;//Con tro co chuc nang xac dinh nhan vien nao lam ra hoa don do
	
	InputField field[8];//staffCode,date,month,year,materialCode,quantity,price,year
	InputField fromtofield[6];//from date/month/year to date/month/year
	Trigger<BillTab> checkMaterialCode;
	Trigger<BillTab> addPayment;
	
	int firstScoller;	
	Scroller scroller;
	
	
	char m_fromdate[3];
	char m_frommonth[3];
	char m_fromyear[5];
	char m_todate[3];
	char m_tomonth[3];
	char m_toyear[5];
	

	
	char m_quantity[10];
	char m_price[10];
	char m_vat[3];
	
	

	char m_materialCode[10];	
	char m_staffCode[6];
	
	char m_date[3];
	char m_month[3];
	char m_year[5];
	
	char m_billCode[20];
	
	MaterialTab* materialTab;//Truy cap du lieu vat tu
	StaffTab* staffTab;
	
	//------------------------------- thong ke -------------------
	Trigger<BillTab> thongke,TKVT,TKHD;
	TREE t;
	Node* a[MAX_PRINT_BILL];
	int n;
};

//
//

int main(){ 
	mousePointX=-1,mousePointY=-1;
	isActive=true;
	CaptureString();
	CatchTime();
	
	StaffTab staff=StaffTab(5+tab_width*1.5f,tab_height/2+5,tab_width,tab_height,tab_basic_color,tab_on_color,"NHAN VIEN");
	MaterialTab material=MaterialTab(5+tab_width/2,tab_height/2+5,tab_width,tab_height,tab_basic_color,tab_on_color,"VAT TU");
	BillTab bill=BillTab(5+tab_width*2.5f,tab_height/2+5,tab_width,tab_height,tab_basic_color,tab_on_color,"THANH TOAN VA THONG KE",&material,&staff);
	Trigger<BillTab> quit=Trigger<BillTab>(1252,tab_height/2+5,105,tab_height,close_on_color,COLOR(165, 43, 41),"QUIT",&BillTab::SaveAndQuit);	
	
	
	
	initwindow(screenWidth,screenHeight,"Quan ly vat tu",0,0,false,false);
	settextstyle(0,0,0);
	
	Tab* tab=&material;
	while(isActive){
		setactivepage(1 - getactivepage());
		DrawBG();
		UpdateMouseFlag();
		
		material.OnClick(tab);
		bill.OnClick(tab);
		staff.OnClick(tab);
		quit.Action(&bill);
//		a
		tab->Action();
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		PrintMessage();
		clearmouseclick(WM_LBUTTONDOWN);
		setvisualpage(getactivepage());
		delay(20);
	}
	return 0;
}
