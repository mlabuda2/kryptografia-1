#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

/**
Kamil Pek 231050
20.10.2016
*/

int klucz_a, klucz_b;
string alfad = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string alfam = "abcdefghijklmnopqrstuvwxyz";

class pliki{
public:

	void zapisz(string s, int tryb){				// ios::app -> dopisywanie do pliku
		ofstream crypto ("crypto.txt", ios::app);
		ofstream decrypt ("decrypt.txt", ios::app);
		if(tryb == 1){
			if(crypto.is_open()){
				crypto << s << "\n";
				crypto.close();	}
			else cout << "Nie mozna zapisac do pliku.\n";	}
		else if(tryb == 2){
			if(decrypt.is_open()){
				decrypt << s << "\n";
				decrypt.close(); }
			else cout << "Nie mozna zapisac do pliku.\n";	}
	}

	int odczytklucza(){
		ifstream key("key.txt");
		if(key.is_open()){
			key >> klucz_a >> klucz_b;
			if(klucz_a >= -26 && klucz_a <= 26) return klucz_a;
			else cout << "Nieprawidlowy klucz w pliku key.txt\n";
			key.close(); }
		else cout << "Nie mozna odczytac pliku key.txt\n";
		cout << klucz_a << "\t" << klucz_b;
		return klucz_a;
	}

	void czyscplik(int tryb){
		if(tryb == 1){
			ofstream crypto ("crypto.txt");
			if(crypto.is_open()){
				crypto << "";
				crypto.close();	}
			else cout << "Nie mozna przeprowadzic operacji na pliku.\n"; }
		if(tryb == 2){
			ofstream decrypt ("decrypt.txt");
			if(decrypt.is_open()){
				decrypt << "";
				decrypt.close();	}
			else cout << "Nie mozna przeprowadzic operacji na pliku.\n"; }
	}

	int wielkosc(char znak){
		if(znak >= 'a' && znak <= 'z') return 0;
		if(znak >= 'A' && znak <= 'Z') return 1;
		else return 2; // pozostale znaki
	}

};

class szyfrcezara{
public:
	pliki obsluga;

	szyfrcezara(int tryb) {
		int sc_klucz = obsluga.odczytklucza();
		string sc_tekst;
		string sc_zaszyfr;
		if(tryb == 1){
			obsluga.czyscplik(1);
			ifstream plain("plain.txt");
			if(plain.is_open()){
				while(getline(plain, sc_tekst)){
					string szyfr = szyfrowanie(sc_tekst, sc_klucz);
					obsluga.zapisz(szyfr, 1); }
				plain.close(); } }
		if(tryb == 2){
			obsluga.czyscplik(2);
			ifstream crypto("crypto.txt");
			if(crypto.is_open()){
				while(getline(crypto, sc_zaszyfr)){
					string tresc = deszyfrowanie(sc_zaszyfr, sc_klucz);
					obsluga.zapisz(tresc, 2); }
				crypto.close(); }	}
		if(tryb == 3){
			ifstream crypto("crypto.txt");
			ifstream extra("extra.txt");
			char cr_a, cr_b, pl_a, pl_b;
			if(crypto.is_open() && extra.is_open()){
				crypto >> cr_a >> cr_b;
				extra >> pl_a >> pl_b;
				kryptoanaliza(cr_a, cr_b, pl_a, pl_b);
				crypto.close(); } }
		if(tryb == 4){
			ifstream crypto("crypto.txt");
			string sc_tresc;
			if(crypto.is_open()){
				while(getline(crypto, sc_tresc)){
					kryptoanaliza2(sc_tresc);
					crypto.close();	}	}	}
}

		string szyfrowanie(string &text, int shift){
			int d = text.size();
			for(int i = 0; i < d; i++) {
	        if(text[i] >= 'A' && text[i] <= 'Z')
	            text[i] = (text[i] + shift > 'Z') ? (text[i] + shift) - 26 : (text[i] + shift);
	        else if(text[i] >= 'a' && text[i] <= 'z')
	            text[i] = (text[i] + shift > 'z') ? (text[i] + shift) - 26 : (text[i] + shift);
	    }
			return text;
		}

		string deszyfrowanie(string &text, int shift){
			int d = text.size();
			for(int i = 0; i < d; i++) {
			        if(text[i] >= 'A' && text[i] <= 'Z')
			            text[i] = (text[i] - shift < 'A') ? (text[i] - shift) + 26 : (text[i] - shift);
			        else if(text[i] >= 'a' && text[i] <= 'z')
			            text[i] = (text[i] - shift < 'a') ? (text[i] - shift) + 26 : (text[i] - shift);
			    }
				return text;
			}

			void kryptoanaliza(char cr_a, char cr_b, char pl_a, char pl_b){
				int r1 = (int)cr_a - (int)pl_a;
				int r2 = (int)cr_b - (int)pl_b;
				if(r1 == r2){
					ofstream keynew("key-new.txt");
					if(keynew.is_open()){
						keynew << r1;
						keynew.close();	}
					else cout << "Nie mozna otworzyc pliku key-new.txt\n"; }
				else cout << "Nie znaleziono klucza\n";
			}

			void kryptoanaliza2(string &t){
				string slowa;
				string tresc;
				for(int i = 0; i < 25; i++){
					tresc = deszyfrowanie(t, i);
					const char* tr = tresc.c_str();
					ifstream slownik("slowa1.txt");
					if(slownik.is_open()){
						while(getline(slownik, slowa)){
							const char* sl = slowa.c_str();
							if((strcmp(sl, tr)) == 0){
								cout << sl << " " << tr << " " << i << "\n";
								ofstream keynew("key-new.txt");
								if(keynew.is_open()){
									keynew << i;
									keynew.close(); }
								else cout << "Nie mozna obsluzyc pliku key-new.txt.\n"; }
 						} }
					else cout << "Nie mozna otworzyc slownika.\n"; }
			}
};

class szyfrafiniczny{ 												// (a*x+b)%26
public:
	pliki obsluga;

	szyfrafiniczny(int tryb){
		int sa_klucz = obsluga.odczytklucza();
		string sa_tekst;
		string sa_zaszyfr;
		if(tryb == 1){
			obsluga.czyscplik(1);
			ifstream plain("plain.txt");
			if(plain.is_open()){
				while(getline(plain, sa_tekst)){
					string sa_szyfr = szyfrowanie(sa_tekst, sa_klucz);
					obsluga.zapisz(sa_szyfr, 1); }
				plain.close(); }	}
		if(tryb == 2){
			obsluga.czyscplik(2);
			ifstream crypto("crypto.txt");
			if(crypto.is_open()){
				while(getline(crypto, sa_zaszyfr)){
					string sa_tresc = deszyfrowanie(sa_zaszyfr, sa_klucz);
					obsluga.zapisz(sa_tresc, 2); }
				crypto.close();	}	}
		if(tryb == 3){
			cout << "Szyfr Afiniczny - kryptoanaliza z tekstem jawnym.\n";
			cout << "Funkcja jeszcze nie gotowa.\n"; }
		if(tryb == 4){
			cout << "Szyfr Afiniczny - kryptoanaliza wylacznie w oparciu o kryptogram.\n";
			cout << "Funkcja jeszcze nie gotowa.\n"; }
	}

	string szyfrowanie(string &t, int k){
		int d = t.size();
		for(int i = 0; i < d; i++){
			int e = (int)t[i];
			int v = ((klucz_a*e)+klucz_b)%26;
			t[i] = alfam[v]; }
		return t;
	}

	string deszyfrowanie(string &t, int k){
		int d = t.size();
		int mv = mul_inv(klucz_a, klucz_b);
		for(int i = 0; i < d; i++){
			int e = (int)t[i];
			int v = (mv*(e - klucz_b))%26;
			t[i] = alfam[v]; }
		return t;
	}

	int mul_inv(int a, int b){					// odwrotnosc modulo
		int b0 = b, t, q;
		int x0 = 0, x1 = 1;
		if (b == 1) return 1;
		while (a > 1){
			q = a / b;
			t = b, b = a % b, a = t;
			t = x0, x0 = x1 - q * x0, x1 = t; }
		if (x1 < 0) x1 += b0;
		return x1;
	}

};

int main(int argc, char * argv[]){

	if ( !strcmp(argv[1], "-c") && !strcmp(argv[2], "-e") ) szyfrcezara sc(1);
	else if ( !strcmp(argv[1], "-c") && !strcmp(argv[2], "-d") ) szyfrcezara sc(2);
	else if ( !strcmp(argv[1], "-c") && !strcmp(argv[2], "-j") ) szyfrcezara sc(3);
	else if ( !strcmp(argv[1], "-c") && !strcmp(argv[2], "-k") ) szyfrcezara sc(4);

	if ( !strcmp(argv[1], "-a") && !strcmp(argv[2], "-e") ) szyfrafiniczny sa(1);
	else if ( !strcmp(argv[1], "-a") && !strcmp(argv[2], "-d")) szyfrafiniczny sa(2);
	else if ( !strcmp(argv[1], "-a") && !strcmp(argv[2], "-j")) szyfrafiniczny sa(3);
	else if ( !strcmp(argv[1], "-a") && !strcmp(argv[2], "-k")) szyfrafiniczny sa(4);

	return 0;
}
