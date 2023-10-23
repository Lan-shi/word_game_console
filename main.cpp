#include <iostream> //header library untuk menggunakan cin/cout dll.
#include <ctime> //header library untuk mengambil fungsi waktu
#include <cstdlib> //header library berisi fungsi-fungsi yg dipakai
#include <string> //header library untuk penggunaan string
#include <chrono> //header library untuk operasi yang berhubungan dengan waktu/jam, perhitungan waktu dll.
#include <vector> // header library untuk dynamic-array yang flexibel (bisa membesar/mengecil sesuai isi, contoh penggunaan di tingkat kesulitan
#include <thread> // header library untuk mengatur thread sebagai alur program berjalan

using namespace std; //mempermudah penulisan code fungsi yg membutuhkan awalan std::, contoh std::cout = cout;.
using namespace std::chrono; //mempermudah penulisan code fungsi yg membutuhkan awalan std::chrono, contoh std::chrono::high_resolution_clock = high_resolution_clock().

const int RACE_DURATION_SEC = 15; //variabel untuk durasi game nya, bisa diubah sesuai keinginan(default nya 15 detik)
const int TIME_PENALTY_SEC = 2; //variabel untuk penalti(pengurangan waktu) ketika salah ketik, maka waktunya berkurang 2 detik
const int SCORE_PENALTY = 1; //variabel untuk pengurangan nilai poin -1 ketika salah ketik.

struct Difficulty { //struct untuk difficulty(tingkat kesulitan level, (easy,medium,hard)
    string name; // berisikan string name, untuk nama kesulitan
    vector<string> words; //berisikan string words, untuk kata-kata yang akan digunakan
};

// Array kata-kata untuk gamenya, bisa di isi sesuai tingkat kesulitan. Bisa ditambahin lebih banyak lagi.
// Tingkat kesulitan Mudah (3-5 huruf)
Difficulty easy = {
    "Mudah",
    {"kucing", "anjing", "burung", "bunga", "pohon", "matahari", "bulan", "air", "rumah", "meja", "bola", "kaki", "makan", "tidur", "panas", "dingin", "hijau", "kuning", "merah", "biru", "besar", "kecil", "panjang", "pendek", "senang", "sedih", "cepat", "lambat", "cerah", "hujan"}
};
// Tingkat kesulitan medium (4-8 huruf, cukup mudah untuk di ketik)
Difficulty medium = {
    "Sedang",
    {"apel", "pisang", "jeruk", "anggur", "nanas", "lemon", "melon", "mangga", "semangka", "pepaya", "makanan", "minuman", "mobil", "sepeda", "pesawat", "kapal", "kereta", "buku", "pensil", "pensiun", "cinta", "marah", "lucu", "cantik", "tampan", "besar", "kecil", "bahagia", "benci", "kaget"}
};
//Tingkat kesulitan hard (5-10++ huruf, kompleks untuk di ketik)
Difficulty hard = {
    "Sulit",
    {"gajah", "harimau", "jerapah", "zebra", "badak", "singa", "serigala", "beruang", "monyet", "trenggiling", "berlari", "berenang", "terbang", "bermain", "berbicara", "menerbangkan", "mencium", "makan malam", "menggambar", "menulis", "menari", "menangis", "merangkak", "menggigit", "merusak", "membaca", "tertidur", "tersenyum", "merenung", "mengerjakan"}
};


// Fungsi mengambil kata secara acak dari array difficulty
string getRandomWord(const vector<string>& wordBank) {
    //variabel index berisikan nilai random(dengan fungsi rand())
    //sesuai dengan berapa banyak kata yang tersimpan pada wordbank(array kata di difficulty)
    int index = rand() % wordBank.size();
    // di return index nya untuk mengambil kata dengan nilai index yang sama
    return wordBank[index];
}

//Fungsi untuk membersihkan layar cmd
void clearScreen() {
/*NOTE : system("cls")  bisa digunakan di windows, sedangkan system("clear") digunakan untuk linux ataupun macOS dll*/
#ifdef _WIN32  //kondisi jika dijalankan pada MS WINDOWS
    system("cls");
#else //kondisi jika dijalankan selain di WINDOWS
    system("clear");
#endif
}
//fungsi untuk menyimpan instruksi game yang dibuat
void showInstructions() {
    cout << "Ketik kata yang diberikan dan tekan Enter secepat mungkin!!" << endl;
    cout << "Peraturan!!" << endl;
    cout << "Kata benar +1 poin" << endl;
    cout << "Kata salah -1 poin, -2 detik" << endl;
    cout << "=============================================================" << endl;
}
//fungsi main
int main() {
    //digunakan srand() untuk memunculkan nomor acak
    //srand() dipasangkan dengan (time(0)) agar sekuens angka yang muncul berbeda setiap kali program berjalan
    //time(0) bernilai integer, maka digunakan static_cast<unsigned> yang digunakan untuk mengconvert hasil time(0)
    // dari signed integer ke unsigned integer (Karena srand harus di isi unsigned integer)
    srand(static_cast<unsigned>(time(0)));

    //Intro saat program dijalankan
    cout << "Selamat Datang di Type Racer Game!" << endl;
    //Pemilihan tingkat kesulitan
    cout << "Pilih tingkat kesulitan:" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    //variabel choice untuk menyimpan pilihan dari user
    int choice;
    //meminta input dari user
    cin >> choice;
    // Mengabaikan baris baru saat memasukan input
    cin.ignore();

    //membuat variabel constant untuk pointer(mengarahkan vector string) wordBank.
    //di wordbank disimpan alamat dari vector string nya
    const vector<string>* wordBank;
    //membuat variabel string untuk menyimpan pilihan tingkat kesulitan
    string selectedDifficulty;
    //switch case(alternatif dari penggunaan if-else)
    switch (choice) { //menggunakan nilai inputan dari user yang diminta sebelumnya untuk tingkat kesulitan
         case 1: //jika pilihannya 1
            //wordBank yang menyimpan alamat vector
            //diarahkan untuk mengambil kata pada alamat yang sesuai dari tingkat kesulitan easy
            wordBank = &easy.words;
            selectedDifficulty = easy.name; //variabel selectedDifficulty berisikan string bernama "easy"
            break;
        case 2: //jika pilihannya 2
            wordBank = &medium.words;
            selectedDifficulty = medium.name;
            break;
        case 3: //jika pilihannya 3
            wordBank = &hard.words;
            selectedDifficulty = hard.name;
            break;
        default: //jika user melakukan input yang tidak adak pilihannya sama sekali
            //otomatis diarahkan ke tingkat kesulitan medium
            cout << "Pilihan tidak valid. Kesulitan default (medium) telah dipilih." << endl;
            wordBank = &medium.words;
            selectedDifficulty = medium.name;
            break;
    }

    //instruksi setelah tingkat kesulitan dipilih
    cout << "Ketik kata yang diberikan dan tekan Enter secepat mungkin!!" << endl;
    cout << "Dimulai dalam ";
    //melakukan print untuk countdown game dimulai
    for (int i = 3; i >= 1; i--) {
        cout << i << "... ";
        //memberikan jeda 1 detik setelah countdown agar pase nya teratur
        this_thread::sleep_for(seconds(1));
    }

    cout << "GO!" << endl; //memulai game
    clearScreen();  //clearScreen() dipanggil untuk menghapus instruksi intro sebelumnya
    this_thread::sleep_for(seconds(1));//memberikan jeda 1 detik setelah countdown agar pase nya teratur

    showInstructions(); //memanggil fungsi showInstructions() yang dibuat diluar main()
    string targetWord = getRandomWord(*wordBank); //variabel targetWord diisi kata yang diambil dari fungsi getRandomWord
    cout << "Type: " << targetWord << endl; //menampilkan kata yang harus diketikkan user

    // startTime digunakan untuk merekam timer untuk durasi game dimulai
    //fungsi high_resolution_clock::now() diambil dari library <chrono> untuk mengukur waktu secara presisi
    auto startTime = high_resolution_clock::now();
    //endTime digunakan untuk menambahkan waktu saat program berjalan ditambahkan durasi game yang ditentukan sebelumnya
    auto endTime = startTime + seconds(RACE_DURATION_SEC);

    int numCorrectWords = 0; //deklarasi numCorrectWords untuk berapa banyak kata yang benar
    int numWrongWords = 0; //deklarasi numWrongWords untuk berapa banyak kata yang salah
    int overallScore = 0; //deklarasi overallScore total poin yang didapat

    //looping yang dibuat untuk memunculkan kata terus menerus selama timer masih berjalan
    while (high_resolution_clock::now() < endTime) { //jika time saat ini masih belum melewati endTime yang ditentukan
        string userInput; //variabel userInput untuk menyimpan jawaban user
        getline(cin, userInput); // input yang diketikkan user diambil oleh getline() dan disimpan di userInput

        if (userInput == targetWord) { //kondisi jika user benar mengetikan kata
            numCorrectWords++; //jumlah kata yang benar bertambah +1
            overallScore++; //total score bertambah


        } else { //kondisi jika user mengetikkan kata yang salah
            numWrongWords++; //jumlah kata yang salah bertambah +1
            endTime += seconds(TIME_PENALTY_SEC); // timer terkena penalty waktu yang telah ditentukan
            overallScore = max(0, overallScore - SCORE_PENALTY); // total score berkurang

        }

        clearScreen(); // membersihkan layar console dari kata sebelumnya dan instruksi yang ditampilkan


        showInstructions(); // menampilkan kembali instruksi selama timer berjalan
        targetWord = getRandomWord(*wordBank); //mengambil lagi kata yang berbeda
        cout << "Type: " << targetWord << endl; //kembali memberikan kata untuk di input user

    } //looping berakhir ketika timer melewati endTime

    //elapsedTime menyimpan perhitungan waktu yang telah dilewati semenjak program berjalan
    //dengan duration_cast<seconds> untuk convert dengan bantuan fungsi library <chrono> agar tersimpan berapa detik waktu yang berjalan
    auto elapsedTime = duration_cast<seconds>(high_resolution_clock::now() - startTime);
    int totalWordsTyped = numCorrectWords + numWrongWords; //totalWordsTyped menyimpan berapa banyak kata yang muncul selama dimulai

    //menampilkan instruksi saat waktu telah habis
    cout << "\nWaktu telah habis!" << endl;
    //menampilkan jumlah kata yang benar dari keseluruhan kata yang muncul
    cout << "Kata yang benar: " << numCorrectWords << "/" << totalWordsTyped << endl;
    //menampilkan total skor
    cout << "Total skor mu: " << overallScore << endl;
    //Menampilkan tingkat kesulitan yang dipilih
    cout << "Tingkat kesulitan: " << selectedDifficulty << endl;
    // Pause agar console tidak langsung keluar
    cout << "Tekan Enter untuk keluar...";
    //Untuk mengabaikan Enter yang dipencet user terakhir kali pada kata terakhir yang dimasukkan
    cin.ignore();
    //Mengambil input user untuk keluar dari program
    cin.get();

    return 0;
}
