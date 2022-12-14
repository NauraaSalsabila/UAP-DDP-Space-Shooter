
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


#define LEBAR_LAYAR 91
//Mendefinisikan bahwa lebar layar 91
#define TINGGI_LAYAR 26
//Mendefinisikan bahwa tinggi layar 26
#define LEBAR_AREA 70
//Mendefinisikan bahwa lebar layar game 70

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

char pesawat[3][5] = {' ',' ','^',' ',' ', //baris-kolom
				  	  '|','^','^','^','|',
					  '^','?','?','?','^' }; 
						
int posisiPesawat = LEBAR_AREA/2;
int skor = 0;
int peluru[20][4];
int bulletsLife[20];
int bIndex = 0;

void load(){
// fungsi untuk loading

	system("Color B"); // warna untuk tampilan loading dan saat bermain
	cout<<"\n\n\n\n\n\n\n\n\n\n\n";
	cout<<"\t\t\t\t\t   Loading \n";
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
	cout<<"\n Instruksi :";
	cout<<"\n----------------------------------------------------";
	cout<<"\n Hindari tertabrak oleh musuh";
	cout<<"\n a dan d untuk bergerak ke kiri dan ke kanan";
	cout<<"\n Gunakan spasi untuk menembak musuh di depan";
	cout<<"\n Capai target skor 10 untuk memenangkan pertandingan";
	cout<<"\n\n\n Tekan apa saja untuk melanjutkan...";
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
	size = 20; //default cursor size		
	
	CONSOLE_CURSOR_INFO lpCursor;	
	//untuk memodifikasi lokasi tinggi dan visibilitas	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
	
}

void Border(){ 
// untuk membuat bingkai/border pada game
	
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

void Border2(){
		for(int i=0; i<LEBAR_LAYAR; i++){ 
		gotoxy(i,TINGGI_LAYAR); cout<<"_";
	}
	for(int i=0; i<TINGGI_LAYAR; i++){
		gotoxy(0,i); cout<<"|";
		gotoxy(LEBAR_LAYAR,i); cout<<"|";
	}
}

void genEnemy(int ind){
	musuhX[ind] = 5 + rand()%(LEBAR_AREA-10); 
	//menentukan random arah kemunculan musuh 
}

void buatMusuh(int ind){
// membuat bentuk musuh
	if( enemyFlag[ind] == true ){
		gotoxy(musuhX[ind], musuhY[ind]);   cout<<".**.";  
		gotoxy(musuhX[ind], musuhY[ind]+1); cout<<"****"; 
		gotoxy(musuhX[ind], musuhY[ind]+2); cout<<"****"; 
		gotoxy(musuhX[ind], musuhY[ind]+3); cout<<".**."; 
		 
	} 
}

void hapusMusuh(int ind){
// menghapus musuh jika terkena peluru
	if( enemyFlag[ind] == true ){
		gotoxy(musuhX[ind], musuhY[ind]);   cout<<"    ";  
		gotoxy(musuhX[ind], musuhY[ind]+1); cout<<"    "; 
		gotoxy(musuhX[ind], musuhY[ind]+2); cout<<"    "; 
		gotoxy(musuhX[ind], musuhY[ind]+3); cout<<"    "; 
	} 
}

void resetMusuh(int ind){
// mereset/mengeluarkan musuh
	hapusMusuh(ind);
	musuhY[ind] = 4;
	genEnemy(ind);
}

void genBullet(){
// mengeluarkan peluru dari pesawat 
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
// membuat peluru 
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
			return 1;//agar program dapat membaca fungsi 
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
	
	posisiPesawat = -1 + LEBAR_AREA/2; //agar posisi awal pesawat tepat di tengah area
	skor = 0;
	enemyFlag[0] = 1;  
	enemyFlag[1] = 1;
	musuhY[0] = musuhY[1] = 4;
	
	for(int i=0; i<20; i++){
		peluru[i][0] = peluru[i][1] = 0; 
	}
	
	system("cls"); //sistem yang dapat menghapus layar screen
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
	
	gotoxy(10, 5);cout<<"Tekan apapun untuk memulai.";
	getch();
	gotoxy(10, 5);cout<<"                           ";
	
	while(1){
		if(kbhit()){ //untuk dapat menentukan apakah tombol telah ditekan atau tidak (conio.h)
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
			Sleep(400);
			cout<<"\t\t--------+--------*--------------*--------"<<endl;
			cout<<"\t\t-----+-------    KALAH    ---------+-----"<<endl;
			cout<<"\t\t*-------*-------------------*----------+-"<<endl<<endl;
			cout<<"\t\t            yaah kamu kalah :(           "<<endl<<endl;
			getch();
			system("cls");
			cout << "\n\t\t     Mau main lagi? y/t";
			cout<< "\n\t\t     y--> bermain kembali\n\t\t     t--> kembali ke menu :";
			cout<< "\n\n\t\t     Pilihan Opsi :";
			
		        char pil;
		        int counter=0;
		        cin >> pil;
		        while(((pil!='y' && pil!='Y'))&&((pil!='t' && pil!='T'))){
 				cout<<"\n\t\t     Opsi yang dipilih tidak tersedia.\n";
 				cout<< "\n\n\t\t     Pilihan Opsi :";
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
					Sleep(400);
					cout<<endl;
					cout<<"\t\t-+--------*---------------*--------+----"<<endl;
					cout<<"\t\t----+--------   MENANG    --------+-----"<<endl;
					cout<<"\t\t+----*-----+----------------*---------+-"<<endl<<endl;
					cout<<"\t\t      Ihiww menang, selamat yaa :)     "<<endl<<endl;
					cout<<"\n\t\t  Tekan apa saja untuk kembali ke menu.";
					getch();
					//Menjeda ketika mengakhiri program
					system("cls");
					play_again = false;
					return;
			}
		} 
		Sleep(50);
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
	//syarat agar dapat menggunakan tools srand/rand 
	 
	system ("color E");
	//Sistem untuk menambahkan warna
	gotoxy(27,10); cout<<"==============================================================";
    gotoxy(27,11); cout<<"   .      *           +                                  .    ";
    gotoxy(27,12); cout<<" +  _____       .            _____ _     +     _    *         ";
    gotoxy(27,13); cout<<"   |   __|___ __ ___ ___    |   __| |_ ___ ___| |_ ___ ___    ";
    gotoxy(27,14); cout<<"   |__   | . | .'|  _| -_|  |__   |   | . | . |  _| -_|  _|   ";
    gotoxy(27,15); cout<<"   |_____|  _|__,|___|___|  |_____|_|_|___|___|_| |___|_|     ";
    gotoxy(27,16); cout<<"  +      |_|      +                          .             +   ";
    gotoxy(27,17); cout<< "=============================================================";
    Sleep(4000);
    system("cls");
	do{
	    if (play_again) {
	        aturan();
	        system("cls");
	        mulai();
	    }
	    else {
		system("cls");
		system("Color A");
		Border2();
		gotoxy(34,4); cout<<" -------------------------- "; 
		gotoxy(34,5); cout<<" |          MENU          | "; 
		gotoxy(34,6); cout<<" -------------------------- ";
		gotoxy(34,10); cout<<" --------------------------";
		gotoxy(34,11);cout<<" | 1. Mulai Permainan     |";
		gotoxy(34,12);cout<<" | 2. Keluar              |";
		gotoxy(34,13); cout<<" --------------------------";	 
		gotoxy(34,15);cout<<"   Pilih Menu: ";
		char op = getche();
		//getche fungsinya sama dengan getch bedanya dapat menampilkan karakter yang diinput
		system("cls");
		
		if( op=='1'){
		aturan();
		system("cls");
		load();
		mulai();
		}else if( op=='2'){
		cout << "\n\n\n\t______________________________________________________\n\n\n";
		cout << "\t~ ~ ~ Terimakasih telah menggunakan program kami ~ ~ ~\n\n";
		cout<<  "\t                        (0>.<0)                         \n";
		cout << "\t______________________________________________________\n\n\n";
		exit(0);
		//exit atau memberhentikan program secara paksa/manual
		}else{
		system("cls");
		}
	    }
		
	}while(1);
	
	return 0;
}
