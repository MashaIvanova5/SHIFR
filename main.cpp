
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>

using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::ofstream;
using std::ifstream;
using std::string;
using std::vector;
using std::ios;
using std::bitset;

// функция шифрования для вывода зашифрованного текста в консоль
// ее параметры text - текст, который хотим зашифровать, 
//size_sdvig - величина сдвига,
//direction_sdvig - направление сдвига, 
//key - ключ шифрования
void crypto(string text, int size_sdvig, int key, char direction_sdvig) {
    
    srand(key);

    int len; // длина текста
    int gamma; // переменная равная значению функции rand()
    int chr_1; // код нечетного символа введенного текста
    int chr_2; // код четного символа введенного текста
    int chr; // код блока в 2 байта (код двух символов)
    int xor_chr; // значение, равное chr^gamma
    int sdvig; // значение сдвига в каждом блоке

    // считаем длину введенного текста
    len = text.length();

    // Если нужно добовляем лишний символ, чтобы длина делилась на 2
    if (len % 2 == 1) {
        text += "=";
        len++;
    }

    // если сдвиг вправо
    if (direction_sdvig == 'R') {
        for (int i = 0; i < len; i += 2) {

            // устанавливаем значение переменной gamma
            gamma = rand();

            // устанавливаем значение chr_1, равное коду символа text[i], и значение chr_2, равное коду символа text[i+1]
            chr_1 = int(text[i]);
            chr_2 = int(text[i + 1]);

            // склеиваем chr_1 и chr_2
            chr = chr_1 * 256 + chr_2;

            // считаем xor chr и gamma
            xor_chr = chr ^ gamma;

            // sdvig равен size_sdvig первым битам числа xor_sdvig
            sdvig = xor_chr % int(pow(2, size_sdvig));

            // xor_sdvig сдвигаем вправо на size_sdvig битов, удалив при этом первые size_sdvig биты 
            xor_chr = xor_chr / int(pow(2, size_sdvig));

            // передвигаем sdvig на (16-size_sdvig) битов. 16 - потому что блок равен 2 байтам
            xor_chr = sdvig * (pow(2, 16 - size_sdvig)) + xor_chr;

            // chr_1 равен последним 8 битам, а chr_2 равен первым 8 битам xor_chr
            chr_1 = xor_chr / 256;
            chr_2 = xor_chr % 256;

            // выводим двоичную запись получившихся символов
            cout << bitset<8>(chr_1) << bitset<8>(chr_2);
        }
    }
    // делаем то же самое только сдвиг теперь не влево, а вправо
    else if (direction_sdvig == 'L') {
        for (int i = 0; i < len; i += 2) {

            gamma = rand();

            chr_1 = int(text[i]);
            chr_2 = int(text[i + 1]);

            chr = chr_1 * 256 + chr_2;

            xor_chr = chr ^ gamma;

            sdvig = xor_chr / int(pow(2, (16 - size_sdvig)));

            xor_chr = xor_chr % int(pow(2, (16 - size_sdvig)));

            xor_chr = xor_chr * (pow(2, size_sdvig)) + sdvig;

            chr_1 = xor_chr / 256;
            chr_2 = xor_chr % 256;

            cout << bitset<8>(chr_1) << bitset<8>(chr_2);
        }
    }
    
}


// функция шифрования, которая выводит зашифрованный текст в файл
// ее параметры text - текст, который хотим зашифровать, 
//size_sdvig - величина сдвига,
//direction_sdvig - направление сдвига, 
//key - ключ шифрования
void crypto_file(string text, int size_sdvig, int key, char direction_sdvig) {

    // создаем объект outfile класса ofstream
    ofstream outfile;
    // открываем (создаем) файл D:\\crypto_text.bin
    outfile.open("D:\\crypto_text.bin", ios::binary);
    srand(key);

    int len;
    int gamma;
    int chr_1;
    int chr_2;
    int chr;
    int xor_chr;
    int sdvig;

    len = text.length();

    if (len % 2 == 1) {
        text += "=";
        len++;
    }

    // фактически делаем тоже самое, что и в функции cryptо, но теперь выводим не в консоль, а в файл
    if (direction_sdvig == 'R') {
        for (int i = 0; i < len; i += 2) {

            gamma = rand();

            chr_1 = int(text[i]);
            chr_2 = int(text[i + 1]);

            chr = chr_1 * 256 + chr_2;

            xor_chr = chr ^ gamma;

            sdvig = xor_chr % int(pow(2, size_sdvig));

            xor_chr = xor_chr / int(pow(2, size_sdvig));

            xor_chr = sdvig * (pow(2, 16 - size_sdvig)) + xor_chr;

            chr_1 = xor_chr / 256;
            chr_2 = xor_chr % 256;

            // записываем chr_1 и chr_2 в двоичном коде
            outfile.write((char*)&chr_1, sizeof(int));
            outfile.write((char*)&chr_2, sizeof(int));
        }
    }

    else if (direction_sdvig == 'L') {
        for (int i = 0; i < len; i += 2) {

            gamma = rand();

            chr_1 = int(text[i]);
            chr_2 = int(text[i + 1]);

            chr = chr_1 * 256 + chr_2;

            xor_chr = chr ^ gamma;

            sdvig = xor_chr / int(pow(2, (16 - size_sdvig)));

            xor_chr = xor_chr % int(pow(2, (16 - size_sdvig)));

            xor_chr = xor_chr * (pow(2, size_sdvig)) + sdvig;

            chr_1 = xor_chr / 256;
            chr_2 = xor_chr % 256;

            outfile.write((char*)&chr_1, sizeof(int));
            outfile.write((char*)&chr_2, sizeof(int));
        }
    }

    //закрываем файл
    outfile.close();
}


// функция дешифрования, отвечающая за дешифрования введенного текста и вывод его в консоль
// ее параметры text - набор битвов,
//size_sdvig - величина сдвига,
//direction_sdvig - направление сдвига, 
//key - ключ дешифрования
void decrypto(string text, int size_sdvig, int key, char direction_sdvig) {
    srand(key);

    int len;
    int gamma;
    int chr;
    int chr_1;
    int chr_2;
    int xor_chr;
    int sdvig;
    string new_text = ""; // дешифрованный тект

    vector <int> vec; // вектор, в котором будет записан код каждого символа

    len = text.length();

    // c помощью преобразования типов записываем в вектор vec код, состоящий из 8 битов 
    for (int i = 0; i < len; i += 8) {
        chr = 128 * ((int(text[i]) - 48)) + 64 * ((int(text[i + 1]) - 48)) + 32 * (int(text[i + 2]) - 48) + 16 * (int(text[i + 3]) - 48) + 8 * (int(text[i + 4]) - 48) + 4 * (int(text[i + 5]) - 48) + 2 * (int(text[i + 6]) - 48) + (int(text[i + 7]) - 48);
        vec.push_back(chr);
    }

    len = vec.size();

    // делаем почти то же самое что и при шифрование, только теперь если пользователь выбрал правый сдвиг, совершаем сдвиг влево и наоборот
    if (direction_sdvig == 'R') {
        for (int i = 0; i < len; i += 2) {
            gamma = rand();
            chr = vec[i] * 256 + vec[i + 1];

            sdvig = chr / int((pow(2, 16 - size_sdvig)));

            chr = chr - sdvig * ((pow(2, 16 - size_sdvig)));

            chr = chr * (pow(2, size_sdvig)) + sdvig;

            xor_chr = chr ^ gamma;

            chr_1 = xor_chr / 256;
            chr_2 = xor_chr % 256;

            // записываем кажый символ в new_text
            new_text += char(chr_1);
            new_text += char(chr_2);
        }
    }
    
    else if (direction_sdvig == 'L') {
        for (int i = 0; i < len; i += 2) {
            gamma = rand();
            chr = vec[i] * 256 + vec[i + 1];

            sdvig = chr % int((pow(2, size_sdvig)));

            chr = chr / int((pow(2, size_sdvig)));

            chr = chr + sdvig * (pow(2, 16 - size_sdvig));

            xor_chr = chr ^ gamma;

            chr_1 = xor_chr / 256;
            chr_2 = xor_chr % 256;

            new_text += char(chr_1);
            new_text += char(chr_2);
        }
    }

    len = new_text.length();
    
    // удаляем последний символ, если он равен "="
    if (new_text[len - 1] == '=') {
        new_text.erase(len - 1);
    }

    cout << new_text;
}


// функция дешифрования, отвечающая за дешифрование файла и вывод дешифрованного текста в консоль
// ее параметры name_file - название файла,
//size_sdvig - величина сдвига,
//direction_sdvig - направление сдвига, 
//key - ключ дешифрования
void decrypto_file(string name_file, int size_sdvig, int key, char direction_sdvig) {
    srand(key);

    // создаем объект infile класса ifstream
    ifstream infile;
    // открываем файл с названием name_file
    infile.open(name_file, ios::binary || ios::in);

    int len;
    int gamma;
    int chr;
    int chr_1;
    int chr_2;
    int xor_chr;
    int sdvig;
    string new_text = "";

    vector <int> vec;

    // записываем код каждого символа в вектор
    while (infile.read((char*)&chr, sizeof(int))) {
        vec.push_back(chr);
    }

    infile.close();

    len = vec.size();

    // делаем то же самое, что и в decrypto
    if (direction_sdvig == 'R') {
        for (int i = 0; i < len; i += 2) {
            gamma = rand();

            chr = vec[i] * 256 + vec[i + 1];

            sdvig = chr / int((pow(2, 16 - size_sdvig)));

            chr = chr - sdvig * ((pow(2, 16 - size_sdvig)));

            chr = chr * (pow(2, size_sdvig)) + sdvig;

            xor_chr = chr ^ gamma;

            chr_1 = xor_chr / 256;
            chr_2 = xor_chr % 256;

            new_text += char(chr_1);
            new_text += char(chr_2);
        }
    }

    else if (direction_sdvig == 'L') {
        for (int i = 0; i < len; i += 2) {
            gamma = rand();

            chr = vec[i] * 256 + vec[i + 1];

            sdvig = chr % int((pow(2, size_sdvig)));

            chr = chr / int((pow(2, size_sdvig)));

            chr = chr + sdvig * (pow(2, 16 - size_sdvig));

            xor_chr = chr ^ gamma;

            chr_1 = xor_chr / 256;
            chr_2 = xor_chr % 256;

            new_text += char(chr_1);
            new_text += char(chr_2);
        }
    }

    len = new_text.length();

    if (new_text[len - 1] == '=') {
        new_text.erase(len - 1);
    }

    cout << new_text;
}


int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Выберете режим работы: " << endl;
    cout << "Шифрование - 1" << endl << "Расшифрование - 2" << endl;
    int mode_work;
    cin >> mode_work;
    while ((mode_work < 1) || (mode_work > 2))
    {
        cout << "Неверный ввод, повторите попытку." << endl;
        cout << "Шифрование - 1" << endl << "Расшифрование - 2" << endl;
        cin >> mode_work;
    }


    cout << "Выберите режим чтения:" << endl;
    cout << "1 - с консоли" << endl;
    cout << "2 - с файла" << endl;
    int reading_mode;
    cin >> reading_mode;
    while ((reading_mode < 1) || (reading_mode > 2))
    {
        cout << "Выберите режим чтения:" << endl;
        cout << "1 - с консоли" << endl;
        cout << "2 - с файла" << endl;
        cin >> reading_mode;
    }


    if (mode_work == 1)
    {
        if (reading_mode == 1) {

            cout << "Введите текст: " << endl;
            string text;
            cin.ignore();
            getline(cin, text);

            cout << "Введите ключ: " << endl;
            int key;
            cin >> key;

            cout << "Введите размер побитового сдвига: " << endl;
            int size_sdvig;
            cin >> size_sdvig;

            cout << "Введите направление побитового сдвига (L - влево, R-вправо): " << endl;
            char direction_sdvig;
            cin >> direction_sdvig;

            crypto(text, size_sdvig, key, direction_sdvig);
        }
        else if (reading_mode == 2) {

            cout << "Введите текст: " << endl;
            string text;
            cin.ignore();
            getline(cin, text);

            cout << "Введите ключ: " << endl;
            int key;
            cin >> key;

            cout << "Введите размер побитового сдвига: " << endl;
            int size_sdvig;
            cin >> size_sdvig;

            cout << "Введите направление побитового сдвига (L - влево, R-вправо): " << endl;
            char direction_sdvig;
            cin >> direction_sdvig;

            crypto_file(text, size_sdvig, key, direction_sdvig);
        }
    }
    else if (mode_work == 2)
    {
        if (reading_mode == 1) {

            cout << "Введите текст: " << endl;
            string text;
            cin.ignore();
            getline(cin, text);

            cout << "Введите ключ: " << endl;
            int key;
            cin >> key;

            cout << "Введите размер побитового сдвига: " << endl;
            int size_sdvig;
            cin >> size_sdvig;

            cout << "Введите направление побитового сдвига (L - влево, R-вправо): " << endl;
            char direction_sdvig;
            cin >> direction_sdvig;

            decrypto(text, size_sdvig, key, direction_sdvig);
        }
        else if (reading_mode == 2) {

            cout << "Введите имя файла: " << endl;
            string name_file;
            cin >> name_file;

            cout << "Введите ключ: " << endl;
            int key;
            cin >> key;

            cout << "Введите размер побитового сдвига: " << endl;
            int size_sdvig;
            cin >> size_sdvig;

            cout << "Введите направление побитового сдвига (L - влево, R-вправо): " << endl;
            char direction_sdvig;
            cin >> direction_sdvig;

            decrypto_file(name_file, size_sdvig, key, direction_sdvig);
        }
    }
}
