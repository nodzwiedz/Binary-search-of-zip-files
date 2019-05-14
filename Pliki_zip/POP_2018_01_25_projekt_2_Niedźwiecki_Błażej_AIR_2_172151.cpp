/// POP 2018-01-25 projekt made by Niedüwiecki B≥aøej CODE-BLOCKS 13.11 compilator - MinGW 32
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <cstdlib>
#include <iomanip>
#include <cmath>
using namespace std;

struct File{
    string name;
    long long int position;
    string Comment;
};

class ZIP {
private:
   fstream file;
public:
   stringstream ssfile; // strumien z plikiem
   long long int size;
   ZIP(string path) {
      this->file.open(path,ios::binary | fstream::in | fstream::out);
      this->file.seekg(0,this->file.end);
      this->size = this->file.tellg();
      this->file.seekg(0,this->file.beg);
   }
   ~ZIP() { this->file.close(); }

   long long int findCentralDirectoryFileHeader(long long int from=0) {

      stringstream code,s;
      unsigned char c;
      this->file.seekg(from);
      code << (unsigned char) 0x50 << (unsigned char) 0x4b << (unsigned char) 0x01 << (unsigned char) 0x02;
      for (long long int pos=this->file.tellg(); pos <= this->size-4; this->file.seekg(++pos)) {
         s.str("");
         int ile=4;
         while (this->file.tellg() != this->size && ile--) {
            this->file >> c;
            s << c;
         }
         if (code.str() == s.str()) { // jest !
            return pos;
         }
      }
      return -1;
   }
   static int convertCharToInt(char &c){
      if ((int)c<0) return 256 + (int)c;
      return (int)c;
   }
   long long int findEndOfCentralDirectoryRecord(long long int from=0) {
      stringstream code,s;
      unsigned char c;
      this->file.seekg(from);
      code << (unsigned char) 0x50 << (unsigned char) 0x4b << (unsigned char) 0x05 << (unsigned char) 0x06;
      for (long long int pos=this->file.tellg(); pos <= this->size-4; this->file.seekg(++pos)) {
         s.str("");
         int ile=4;
         while (this->file.tellg() != this->size && ile--) {
            this->file >> c;
            s << c;
         }
         if (code.str() == s.str()) { // jest !
            return pos;
         }
      }
      return -1;
   }

   void convertToData(long long int n){
        int year,month,day;
        year=1980; month=0; day=0;
        string wynik = "0000000000000000";
        int where = 15;
        while(n){ //na binarny
        wynik[where] = (n%2?'1':'0');
        n /= 2;
        where--;
        }
        int potega=0;
        for(int i=wynik.length()-1;i>=wynik.length()-5;i--){ //dzien
            day+=(wynik[i]-48)*pow(2,potega);
            potega++;
        }
        potega=0;
        for(int i=wynik.length()-6;i>=wynik.length()-9;i--){ //miesiac
            month+=(wynik[i]-48)*pow(2,potega);
            potega++;
        }
        potega=0;
        for(int i=wynik.length()-10;i>=0;i--){ //rok
            year+=(wynik[i]-48)*pow(2,potega);
            potega++;
        }
        cout    << year << "-"
                << setfill('0') << setw(2) << month << "-"
                << setfill('0') << setw(2) << day << endl;
   }

     void convertToTime(long long int n){
        int hour=0; int minute=0; int second=0;
        string wynik = "0000000000000000";
        int where = 15;
        while(n){
        wynik[where] = (n%2?'1':'0');
        n /= 2;
        where--;
        }
        int potega=0;
        for(int i=wynik.length()-1;i>=wynik.length()-5;i--){ //sekundy
            second+=(wynik[i]-48)*pow(2,potega);
            potega++;
        }
        second=second*2;
        potega=0;
        for(int i=wynik.length()-6;i>=wynik.length()-11;i--){ //minuty
            minute+=(wynik[i]-48)*pow(2,potega);
            potega++;
        }
        potega=0;
        for(int i=wynik.length()-12;i>=0;i--){ //godziny
            hour+=(wynik[i]-48)*pow(2,potega);
            potega++;
        }
        cout    << setfill('0') << setw(2) << hour << ":"
                << setfill('0') << setw(2) << minute << ":"
                << setfill('0') << setw(2) << second << endl;
   }

    void extractInfoFromCentralDirectoryFileHeaderPosition(long long int pos, File pliki[], int target) {
      cout << endl;
      cout << "=================================================CENTRAL REPO================================================\n";
      cout << endl;

      this->file.seekg(pos+4); // omijam 4 bajtowy kod
      char c1,c2,c3,c4;
      long long int n;
      stringstream s;
      // wersja 2
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Wersja: 0x" << s.str() << " > " << n << endl; s.str(""); s.clear();
      // wersja minimum
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Wersja minimalna wymagana do rozpakowania: 0x"
         << s.str() << " > " << n << endl; s.str(""); s.clear();
      // flagi
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Flagi: 0x"
         << s.str() << " > " << n << endl; s.str(""); s.clear();
      // metoda compresji
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Metoda kompresji: " << s.str() << " > " << n << endl; s.str(""); s.clear();
      // Czas
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Czas ostatniej modyfikacji: "
         << s.str() << " > " << n << " > "; convertToTime(n);
         s.str(""); s.clear();
      // Data
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Data ostatniej modyfikacji: "
         << s.str() << " > " << n << " > "; convertToData(n);
         s.str(""); s.clear();

      // CRC
      this->file.read(&c1,1); this->file.read(&c2,1); this->file.read(&c3,1); this->file.read(&c4,1);
      if ((int)c1<0)
      s  << setfill('0') << setw(2) << hex << this->convertCharToInt(c4)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c3)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c2)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "CRC: " << s.str() << " > " << n << endl; s.str(""); s.clear();
      // Compresed size
      this->file.read(&c1,1); this->file.read(&c2,1); this->file.read(&c3,1); this->file.read(&c4,1);
      s  << setfill('0') << setw(2) << hex << this->convertCharToInt(c4)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c3)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c2)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Plik skompresowany w bajtach: " << s.str() << " > " << n << endl; s.str(""); s.clear();
      // UnCompresed size
      this->file.read(&c1,1); this->file.read(&c2,1); this->file.read(&c3,1); this->file.read(&c4,1);
      s  << setfill('0') << setw(2) << hex << this->convertCharToInt(c4)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c3)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c2)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Plik nieskompresowany w bajtach: " << s.str() << " > " << n << endl; s.str(""); s.clear();
      // Nazwa pliku
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Dlugosc nazwy pliku: "
         << s.str() << " > " << n << endl; s.str(""); s.clear();
      long long int N = n;
      // Dodatkowe napisy
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Dlugosc dodatkowych wpisow: "
         << s.str() << " > " << n << endl; s.str(""); s.clear();
      long long int M = n;
      // Komentarz
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Dlugosc komentarza: "
         << s.str() << " > " << n << endl; s.str(""); s.clear();
      long long int K = n;
      // Numer dysku
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Numer dysku, gdzie sie plik zaczyna: "
         << s.str() << " > " << n << endl; s.str(""); s.clear();
      // Atrybuty wewnetrzne
      this->file.read(&c1,1); this->file.read(&c2,1);
      s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Atrybuty wewnetrzne: "
         << s.str() << " > " << n << endl; s.str(""); s.clear();
      // Atrybuty zewnetrzne
      this->file.read(&c1,1); this->file.read(&c2,1); this->file.read(&c3,1); this->file.read(&c4,1);
      s  << setfill('0') << setw(2) << hex << this->convertCharToInt(c4)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c3)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c2)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Atrybuty zewnetrzne: " << s.str() << " > " << n << endl; s.str(""); s.clear();
      // Relative offset
      this->file.read(&c1,1); this->file.read(&c2,1); this->file.read(&c3,1); this->file.read(&c4,1);
      s  << setfill('0') << setw(2) << hex << this->convertCharToInt(c4)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c3)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c2)
         << setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
      n = strtol(s.str().c_str(),NULL,16);
      cout << "Pozycja wewnatrz pliku: " << s.str() << " > " << n << endl; s.str(""); s.clear();
      long long int position=n;
      // Plik nazwa
      cout << "Nazwa pliku: ";
      for (int i=1; i<=N; i++) {
         this->file.read(&c1,1); cout << c1; pliki[target].name+=c1;
      }
      cout << endl;
      // Dodatek
      cout << "Dodatek:\n";
      for (int i=1; i<=M; i++) {
         this->file.read(&c1,1); cout << this->convertCharToInt(c1);
      }
      cout << endl;
      // Komentarz
      cout << "Komentarz do pliku: ";
      pliki[target].Comment = "";
      for (int i=1; i<=K; i++) {
         this->file.read(&c1,1); cout << c1;
         pliki[target].Comment+=c1;
      }
      cout << endl;
      cout << "=================================================CENTRAL REPO================================================\n";
      cout << endl;

      extractInfoFromHeaderPosition(position); //z tej pozycji co znalazlem w pliku, nie musze szukac
   }

    void extractInfoFromHeaderPosition(long long int pos){
        cout << endl;
        cout << "===================================================HEADER====================================================\n";
        cout << endl;
        this->file.seekg(pos+4);//omijam pierwsze 4 bajty
        char c1,c2,c3,c4;
        long long int n;
        stringstream s;
        // wersja 2
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Wersja programu: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Flagi: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Metoda kompresji: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Czas ostatniej modyfikacji pliku: " << s.str() << " > "; convertToTime(n); s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Data ostatniej modyfikacji pliku: " << s.str() << " > "; convertToData(n); s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1); this->file.read(&c3,1); this->file.read(&c4,1);
        s   << setfill('0') << setw(2) << hex << this->convertCharToInt(c4)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c3)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c2)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "CRC-32: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1); this->file.read(&c3,1); this->file.read(&c4,1);
        s   << setfill('0') << setw(2) << hex << this->convertCharToInt(c4)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c3)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c2)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Rozmiar skompresowanego pliku: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1); this->file.read(&c3,1); this->file.read(&c4,1);
        s   << setfill('0') << setw(2) << hex << this->convertCharToInt(c4)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c3)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c2)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Rozmiar rozpakowanego pliku: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Dlugosc nazwy pliku: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        long long int N=n;
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Dodatkowa dlugosc naglowka: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        long long int M=n;
        cout << "Nazwa pliku: ";
        for(int i=1;i<=N;i++){
        this->file.read(&c1,1); cout << c1;
        }
        cout << endl;
        cout << "Dodatek: ";
        for(int i=1;i<=M;i++){
        this->file.read(&c1,1); cout << c1;
        }


        cout << endl;
        cout << "===================================================HEADER====================================================\n";
        cout << endl;

    }

    void extractInfoFromEndOfDirectoryRecordPosition(long long int pos, File pliki[]){
        cout << endl;
        cout << "============================================END OF CENTRAL REPO==============================================\n";
        cout << endl;
        this->file.seekg(pos+4); // omijam 4 bajtowy kod
        char c1,c2,c3,c4;
        long long int n;
        stringstream s;
        // wersja 2
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Numer dysku przy kompresji z podzialem: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Numer dysku gdzie zaczyna sie centralne repozytorium: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Liczba wpisow plikow centralnego repozytorium na biezacym dysku: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Liczba wszystkich wpisow plikow centralnego repozytorium: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1); this->file.read(&c3,1); this->file.read(&c4,1);
        s   << setfill('0') << setw(2) << hex << this->convertCharToInt(c4)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c3)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c2)
            << setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Rozmiar wpisow plikow centralnego repozytorium: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1); this->file.read(&c3,1); this->file.read(&c4,1);
        s   <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c4)
            <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c3)
            <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2)
            <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Przesuniecie centralnego repozytorium: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16);
        cout << "Dlugosc komentarza: " << s.str() << " > " << n << endl; s.str(""); s.clear();
        long long int P=n;
        cout << "Komentarz do pliku: ";
        pliki[0].Comment = "";
        for(int i=1;i<=P;i++){
            this->file.read(&c1,1); cout << c1;
            pliki[0].Comment+=c1;
        }

        cout << endl;
        cout << "============================================END OF CENTRAL REPO==============================================\n";
        cout << endl;
    }

    void addCommentToFile(string comment, long long int how_long, long long int position){//do plikow w srodku ZIPa
        char c1,c2;
        long long int n;
        stringstream s;
        s.str(""); s.clear();
        this->file.seekg(position+28);//ide do miejsca gdzie jest dlugosc nazwy
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16); s.str(""); s.clear();
        long long int nameSize = n;
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16); s.str(""); s.clear();
        long long int addly = n;
        char first = how_long;
        char second = how_long>>8;//przesuniecie bitowe, zeby zapisac w little endian
        this->file.write( (char*)&first, 1 ); //wpisuje do dlugosci komentarza
        this->file.write( (char*)&second, 1 );
        position = this->file.tellg();
        this->file.seekg(position+12+nameSize+addly);//tam gdzie jest komentarz
        long long int second_position =  this->file.tellg() ;
        this->file.seekg(0, this->file.end );
        position = this->file.tellg(); //koniec pliku
        for(long long int i = position ; i > second_position ; i--){ //przesuwam wszystko to co jest za komentarzem
            this->file.seekg( i-1 );
            this->file.read( &c1, 1 );
            this->file.seekp( i+how_long-1 );
            this->file.write( &c1, 1);
            this->file.seekg( i-1 );
        }
        for(int i = 0 ; i < how_long ; i++){    //dodaje komentarz do pliku
            this->file.write( &comment[i] , 1 );
        }
    }

    void addCommentToZipFile(string comment, long long int how_long, long long int position){//do calego pliku .zip
        this->file.seekg(position+20); //ide do dlugosci komentarza
        char first = how_long;
        char second = how_long>>8;
        this->file.write( (char*)&first, 1 );
        this->file.write( (char*)&second, 1 );
        for(int i = 0 ; i < how_long ; i++){
            this->file.write( &comment[i] , 1 );
        }
    }

    long long int deleteCommentFromFile( long long int position ){
        char c1,c2;
        long long int n;
        stringstream s;
        s.str(""); s.clear();
        this->file.seekg(position+28);//ide do miejsca gdzie jest dlugosc nazwy
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16); s.str(""); s.clear();
        long long int nameSize = n;
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16); s.str(""); s.clear();
        long long int addly = n;
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16); s.str(""); s.clear();
        long long int how_long = n;
        if(how_long != 0){
            long long int current = this->file.tellg();
            this->file.seekg(current-2);
            int zero=0;
            char first = zero;
            char second = zero>>8;
            this->file.write( (char*)&first, 1 ); //ide do dlugosci kom i tam wstawiam 0
            this->file.write( (char*)&second, 1 );
            position = this->file.tellg();
            this->file.seekg(position+12+nameSize+addly);
            long long int second_position = this->file.tellg();
            this->file.seekg(0, this->file.end);
            position = this->file.tellg();
            this->file.seekg( second_position );
            for(long long int i = second_position; (i+how_long) < position ; i++ ){
                this->file.seekg(i+how_long);
                this->file.read(&c1,1);
                this->file.seekg(i);
                this->file.write(&c1,1);
            }
            return how_long;
        }
    }

    void deleteCommentFromEndOfFile(long long int position){
        char c1,c2;
        long long int n;
        stringstream s;
        s.str(""); s.clear();
        this->file.seekg(position+20);//tu gdzie jest dlugosc komentarz
        this->file.read(&c1,1); this->file.read(&c2,1);
        s <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c2) <<  setfill('0') << setw(2) << hex << this->convertCharToInt(c1);
        n = strtol(s.str().c_str(),NULL,16); s.str(""); s.clear();
        long long int how_long = n;
        this->file.seekg(position+20);//wracam na to miejsce
        int zero=0;
        char first = zero;
        char second = zero>>8;
        this->file.write( (char*)&first, 1 ); //ide do komentarza
        this->file.write( (char*)&second, 1 );
    }

};



int main() {
cout << "Podaj nazwe pliku zip wraz z rozszerzeniem: " ;
string file_name;  // nazwa pliku
cin >> file_name;

ZIP zip(file_name);
long long int pos = -1;
File* pliki;
pliki = new File[25];
pliki[0].name = file_name;
int target=1;

while ( (pos = zip.findCentralDirectoryFileHeader(pos+1)) != -1) {
    zip.extractInfoFromCentralDirectoryFileHeaderPosition(pos, pliki, target);
    pliki[target].position=pos;
    target++;
}
pos = -1;
while ( (pos = zip.findEndOfCentralDirectoryRecord(pos+1)) != -1) {
    zip.extractInfoFromEndOfDirectoryRecordPosition(pos, pliki);
    pliki[0].position=pos;
}
int maks_setw = 0;
for(int i=0;i < target; i++){
    if(pliki[i].name.length() > maks_setw) maks_setw = pliki[i].name.length();
}
cout << "PLIKI: " << endl;
for(int i=0;i< target ;i++){
    cout << i+1 << ". " << setfill(' ') << setw(maks_setw) << pliki[i].name << " -> "<< " Komentarz:  ";
    if(pliki[i].Comment == "") cout << "Brak komentarza." << endl;
    else cout << pliki[i].Comment << endl;
}
int number;
string comment;
while ( number!=4 ){
    cout << endl;
    cout << "1. Dodaj komentarz do pliku." << endl;
    cout << "2. Usun komentarz z pliku." << endl;
    cout << "3. Zmien komentarz w pliku." << endl;
    cout << "4. Zakoncz program." << endl << endl;
    cout << "Wybierz opcje numer... ";
    while(! (cin >> number) ){
        cout << "Zle dane! Podaj cyfre z zakresu od 1 do 4" << endl;
        cin.clear();
        cin.sync();
        cout << "Wybierz opcje numer... ";
    }
    if(number>=1 && number<=4){
        if(number!=4){
            cout << endl << "Wybierz plik... " << endl;
            int another_number;
            for(int i=0;i< (target) ;i++){
                cout << i+1 << ". " << setfill(' ') << setw(maks_setw) << pliki[i].name << " -> "<< " Komentarz:  ";
                if(pliki[i].Comment == "") cout << "Brak komentarza." << endl;
                else cout << pliki[i].Comment << endl;
            }
            cout << endl << "Numer pliku: ";
            while( !(cin >> another_number)){
                cout << "Zle dane!" << endl;
                cin.clear();
                cin.sync();
                cout << endl << "Numer pliku: ";
            }
            if(another_number < 1 || another_number > target ){
                cout << "Podaj cyfre z zakresu od 1 do " << target << endl;
                continue;
            }
            if(number==1){
                if( pliki[another_number-1].Comment != "" ){
                cout << "W tym pliku jest juz komentarz. Mozesz DODAC komentarz jedynie do pliku bez komentarza." << endl;
                continue;
                }
                cout << "Podaj komentarz, ktory ma zostac dodany do pliku: ";
                cin >> comment;
                long long int how_long = comment.length();
                if( (another_number-1)==0 ){
                    zip.addCommentToZipFile(comment, how_long, pliki[another_number-1].position);
                }
                else{
                    zip.addCommentToFile(comment, how_long, pliki[another_number-1].position);
                    for(int i = another_number; i< (target) ; i++) pliki[i].position += how_long;
                    pliki[0].position += how_long;
                    pliki[another_number-1].Comment = comment;
                }
                cout << "Dodalem komentarz do pliku " << pliki[another_number-1].name << endl << endl;
                system("PAUSE");
                if( (another_number-1)==0 ) zip.extractInfoFromEndOfDirectoryRecordPosition(pliki[another_number-1].position, pliki);
                else zip.extractInfoFromCentralDirectoryFileHeaderPosition(pliki[another_number-1].position , pliki , target);
            }
            else if(number==2){
                if( pliki[another_number-1].Comment == "" ){
                    cout << "W tym pliku nie ma komentarza. Mozesz USUNAC komentarz jedynie z pliku zawierajacego komentarz." << endl;
                    continue;
                }
                if( (another_number-1)==0 ){
                    zip.deleteCommentFromEndOfFile(pliki[another_number-1].position);
                    cout << "Usunalem komentarz z pliku " << pliki[another_number-1].name << endl;
                    system("PAUSE");
                    zip.extractInfoFromEndOfDirectoryRecordPosition(pliki[another_number-1].position, pliki);
                    pliki[0].Comment = "";
                }
                else{
                    long long int del = zip.deleteCommentFromFile(pliki[another_number-1].position);
                    cout << "Usunalem komentarz z pliku " << pliki[another_number-1].name << endl;
                    system("PAUSE");
                    zip.extractInfoFromCentralDirectoryFileHeaderPosition(pliki[another_number-1].position , pliki , target);
                    for(int i = another_number; i<10; i++) pliki[i].position -= del;
                    pliki[0].position -= del;
                    pliki[another_number-1].Comment = "";
                }
            }
            else if(number==3){
                if( pliki[another_number-1].Comment == "" ){
                cout << "W tym pliku nie ma komentarza. Mozesz ZMIENIC komentarz jedynie w pliku z komentarzem." << endl;
                continue;
                }
                cout << "Podaj komentarz, ktorym chcesz zamienic poprzedni: ";
                cin >> comment;
                //najpierw usuwam kom z pliku
                if( (another_number-1)==0 ){
                    zip.deleteCommentFromEndOfFile(pliki[another_number-1].position);
                    pliki[0].Comment = "";
                }
                else{
                    long long int del = zip.deleteCommentFromFile(pliki[another_number-1].position);
                    for(int i = another_number; i<10; i++) pliki[i].position -= del;
                    pliki[0].position -= del;
                    pliki[another_number-1].Comment = "";
                }
                //a teraz dodaje ten nowy, zmieniony
                long long int how_long = comment.length();
                if( (another_number-1)==0 ){
                    zip.addCommentToZipFile(comment, how_long, pliki[another_number-1].position);
                }
                else{
                    zip.addCommentToFile(comment, how_long, pliki[another_number-1].position);
                    for(int i = another_number; i< (target) ; i++) pliki[i].position += how_long;
                    pliki[0].position += how_long;
                    pliki[another_number-1].Comment = comment;
                }
                cout << "Zmienilem komentarz w pliku " << pliki[another_number-1].name << endl << endl;
                system("PAUSE");
                if( (another_number-1)==0 ) zip.extractInfoFromEndOfDirectoryRecordPosition(pliki[another_number-1].position, pliki);
                else zip.extractInfoFromCentralDirectoryFileHeaderPosition(pliki[another_number-1].position , pliki , target);
            }
        }
    }
    else cout << "Zle dane! Podaj cyfre od 1 do 4" << endl;
}
cout << "Koniec programu.";

   return 0;

}
