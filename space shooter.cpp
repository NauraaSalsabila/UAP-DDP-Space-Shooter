#include <iostream>
//library untuk menggunakan cout dan cin
#include <dos.h> 
//library untuk menggunakan dapat menambahkan sound dan sleep
#include <windows.h>
//library untuk menggunakan pemberian warna pada kalimat dan background serta peletakan posisi
#include <time.h>
//library yang digunakan untuk mendefinisikan tanggal dan waktu
#include <conio.h>
//library untuk menggunakan getch


#define LEBAR_LAYAR 90
#define TINGGI_LAYAR 26
#define LEBAR_AREA 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define Enemy_DIF 45

using namespace std; 
 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD PosisiCursor;
//Di gunakan untuk menempatkan kursor di tempat yang tidak dapat dicapai bila
//hanya menggunakan sekuensi program saja


int musuhY[3];
// posisi musuh
int musuhX[3];
// posisi musuh
int enemyFlag[3];
// muncul dan menghilangnya musuh
char pesawat[3][5] = { ' ',' ','^',' ',' ',
					'|','^','^','^','|',
					'^','?','?','?','^' }; 
int posisiPesawat = LEBAR_AREA/2;
int skor = 0;
int peluru[20][4];
int bulletsLife[20];
int bIndex = 0;


void load(){

	system("Color 0B");
	cout<<"\n\n\n\n\n\n\n\n\n\n\n";
	cout<<"\t\t\t\t\tLoading... \n";
	cout<<endl;
	cout<<"\t\t\t  ";
	char x =219;
	
	for(int i=0; i<40; i++)
	{
		cout<<x;
		if(i<10)
		Sleep(350);
		if(i>=10 && i<20)
		Sleep(150);
		if(i>=10)
		Sleep(25);
	}
}

void aturan(){
	
	system("cls");
	cout<<"Instructions";
	cout<<"\n----------------";
	cout<<"\n Press spacebar to make bird fly";
	cout<<"\n\nPress any key to continue";
	getch();
}

void gotoxy(int x, int y){
	PosisiCursor.X = x;
	PosisiCursor.Y = y;
	SetConsoleCursorPosition(console, PosisiCursor);
}
//Di gunakan untuk menempatkan kursos di tempat yang tidak dapat dicapai bila
//hanya menggunakan sekuensi program saja

void setcursor(bool visible, DWORD size) {
	if(size == 0)
	size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	//untuk memodifikasi lokasi tinggi dan visibilitas	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}
void Border(){ 
	
	for(int i=0; i<LEBAR_LAYAR; i++){ 
		gotoxy(i,TINGGI_LAYAR); cout<<"_";
	}
	
	for(int i=0; i<TINGGI_LAYAR; i++){
		gotoxy(0,i); cout<<"|";
		gotoxy(LEBAR_LAYAR,i); cout<<"|";
	}
	for(int i=0; i<TINGGI_LAYAR; i++){
		gotoxy(LEBAR_AREA,i); cout<<"|";
	}
}
void genEnemy(int ind){
	musuhX[ind] = 5 + rand()%(LEBAR_AREA-10);  
}
void buatMusuh(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(musuhX[ind], musuhY[ind]);   cout<<".**.";  
		gotoxy(musuhX[ind], musuhY[ind]+1); cout<<"****"; 
		gotoxy(musuhX[ind], musuhY[ind]+2); cout<<"****"; 
		gotoxy(musuhX[ind], musuhY[ind]+3); cout<<".**."; 
		 
	} 
}
void hapusMusuh(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(musuhX[ind], musuhY[ind]); cout<<"    ";  
		gotoxy(musuhX[ind], musuhY[ind]+1); cout<<"    "; 
		gotoxy(musuhX[ind], musuhY[ind]+2); cout<<"    "; 
		gotoxy(musuhX[ind], musuhY[ind]+3); cout<<"    "; 
	} 
}
void resetMusuh(int ind){
	hapusMusuh(ind);
	musuhY[ind] = 4;
	genEnemy(ind);
}
void genBullet(){
	peluru[bIndex][0] = 22;
	peluru[bIndex][1] = posisiPesawat;
	peluru[bIndex][2] = 22; 
	peluru[bIndex][3] = posisiPesawat+4; 
	bIndex++;
	if( bIndex == 20)
		bIndex = 0;
}
void pindahPeluru(){
	for(int i=0; i<20; i++){
		if( peluru[i][0] > 2 )
			peluru[i][0]--;
		else
			peluru[i][0] = 0;
		
		if( peluru[i][2] > 2 )
			peluru[i][2]--;
		else
			peluru[i][2] = 0;
	} 
}
void buatPeluru(){
	for(int i=0; i<20; i++){
		if( peluru[i][0] > 1){
			gotoxy(peluru[i][1],peluru[i][0]); cout<<"."; 
			gotoxy(peluru[i][3],peluru[i][2]); cout<<".";
		}
	}
}
void hapus_Peluru(){
	for(int i=0; i<20; i++){
		if( peluru[i][0] >= 1 ){
			gotoxy(peluru[i][1],peluru[i][0]); cout<<" ";
			gotoxy(peluru[i][3],peluru[i][2]); cout<<" ";
		}
	}
}
void hapusPeluru(int i){ 
	gotoxy(peluru[i][1],peluru[i][0]); cout<<" ";
	gotoxy(peluru[i][3],peluru[i][2]); cout<<" "; 
}
void buatPesawat(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			gotoxy(j+posisiPesawat, i+22); cout<<pesawat[i][j];
		}
	}
}
void hapusPesawat(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			gotoxy(j+posisiPesawat, i+22); cout<<" ";
		}
	}
}
 
int tabrak(){
	if( musuhY[0]+4 >= 23 ){
		if( musuhX[0] + 4 - posisiPesawat >= 0 && musuhX[0] + 4 - posisiPesawat < 8  ){
			return 1;
		}
	}
	return 0;
}
int tembakanPeluru(){
	for(int i=0; i<20; i++){
		for(int j=0; j<4; j+=2){
			if( peluru[i][j] != 0 ){
				if( peluru[i][j] >= musuhY[0] && peluru[i][j] <= musuhY[0]+4 ){
					if( peluru[i][j+1] >= musuhX[0] && peluru[i][j+1] <= musuhX[0]+4 ){
						hapusPeluru(i);
						peluru[i][j] = 0;
						resetMusuh(0);
						return 1;
					}
				}
				if( peluru[i][j] >= musuhY[1] && peluru[i][j] <= musuhY[1]+4 ){
					if( peluru[i][j+1] >= musuhX[1] && peluru[i][j+1] <= musuhX[1]+4 ){
						hapusPeluru(i);
						resetMusuh(1); 
						peluru[i][j] = 0;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

void totalSkor(){
	gotoxy(LEBAR_AREA + 7, 5);cout<<"Skor: "<<skor<<endl;
}

bool play_again = false;

void mulai(){
	
	posisiPesawat = -1 + LEBAR_AREA/2;
	skor = 0;
	enemyFlag[0] = 1;  
	enemyFlag[1] = 1;
	musuhY[0] = musuhY[1] = 4;
	
	for(int i=0; i<20; i++){
		peluru[i][0] = peluru[i][1] = 0; 
	}
	
	system("cls"); 
	Border();
	genEnemy(0);
	genEnemy(1);
	totalSkor();
	
	gotoxy(LEBAR_AREA + 5, 2);cout<<"Space Shooter";
	gotoxy(LEBAR_AREA + 6, 4);cout<<"----------";
	gotoxy(LEBAR_AREA + 6, 6);cout<<"----------";
	gotoxy(LEBAR_AREA + 7, 12);cout<<"Control ";
	gotoxy(LEBAR_AREA + 7, 13);cout<<"-------- ";
	gotoxy(LEBAR_AREA + 2, 14);cout<<" A Key - Kiri";
	gotoxy(LEBAR_AREA + 2, 15);cout<<" D Key - Kanan";
	gotoxy(LEBAR_AREA + 2, 16);cout<<" Spacebar = Tembak";
	
	gotoxy(10, 5);cout<<"Press any key to start";
	getch();
	gotoxy(10, 5);cout<<"                      ";
	
	while(1){
		if(kbhit()){
			char ch = getch();
			if( ch=='a' || ch=='A' ){
				if( posisiPesawat > 2 )
					posisiPesawat -= 2;
			}
			if( ch=='d' || ch=='D' ){
				if( posisiPesawat < LEBAR_AREA-7 )
					posisiPesawat += 2;
			}
			if(ch==32){
				genBullet();
			} 
			if(ch==27){
				break;
			}
		}
		
		buatPesawat();
		buatMusuh(0);
		buatMusuh(1);
		buatPeluru();
		if( tabrak() == 1  ){
			system("cls");
			cout<<endl;
			cout<<"\t\t--------------------------"<<endl;
			cout<<"\t\t-------- Game Over -------"<<endl;
			cout<<"\t\t--------------------------"<<endl<<endl;
			cout<<"\t\t    yaah kamu kalah :("<<endl<<endl;
			getch();
			cout << "\n\t\t    Mau main lagi? y/t";
			cout<< "\n\t\t     Pilih Menu :";
			
		        char pil;
		        int counter=0;
		        cin >> pil;
		        while(((pil!='y')&&(pil!='t'))){
 				cout<<"\tSilahkan masukkan pilihan yang tertera!\n";
 				cout<< "\n\tPilih Menu :";
					cin>>pil;
					cout<<endl;
					counter++;
				}
		        if(pil=='t'||pil=='T'){
					system("cls");
					play_again = false;
		        	return;
		        }else if(pil=='y'||pil=='Y'){
					system("cls");
		        	play_again = true;
		        	return;
				}
		}
		if(  tembakanPeluru() == 1 ){
			skor++;
			totalSkor();
				if(skor==10){
					system("break");
					system("cls");
					cout<<endl;
					cout<<"\t\t--------------------------"<<endl;
					cout<<"\t\t-------- KAMU MENANG -------"<<endl;
					cout<<"\t\t--------------------------"<<endl<<endl;
					cout<<"\t\tPress any key to go back to menu.";
					getch();
					system("cls");
					play_again = false;
					return;
			}
		} 
		Sleep(200);
		hapusPesawat();
		hapusMusuh(0);
		hapusMusuh(1);
		hapus_Peluru();
		pindahPeluru();   
		
		if( enemyFlag[0] == 1 )
			musuhY[0] += 1;
		
		if( enemyFlag[1] == 1 )
			musuhY[1] += 1;
		 
		if( musuhY[0] > TINGGI_LAYAR-5 ){ 
			resetMusuh(0); 
		}
		if( musuhY[1] > TINGGI_LAYAR-5 ){ 
			resetMusuh(1); 
		}
	}
}

int main()
{

	setcursor(0,0); 
	srand( (unsigned)time(NULL)); 
	 
	do{
	    if (play_again) {
	        aturan();
	        system("cls");
	        mulai();
	    }
	    else {
		system("cls");
		system("Color 0A");
		gotoxy(10,5); cout<<" -------------------------- "; 
		gotoxy(10,6); cout<<" |     Space Shooter      | "; 
		gotoxy(10,7); cout<<" --------------------------";
		gotoxy(10,9); cout<<"1. Start Game";
		gotoxy(10,10); cout<<"2. Quit";	 
		gotoxy(10,12); cout<<"Select option: ";
		char op = getche();
		system("cls");
		
		if( op=='1'){
		aturan();
		system("cls");
		load();
		mulai();
		}else if( op=='2'){
		cout<<"Terimakasih sudah menggunakan program kami";
		cout<<"\n(0>.<0)";
		exit(0);
		}else{
		system("cls");
		}
	    }
		
	}while(1);
	
	return 0;
}

