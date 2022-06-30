#include <stdio.h>
#include <stdlib.h>
#define MODEL_ARR_LEN 256

struct Huffman//STRUKTURA KODU HUFFMANA
{
    int symbol;
    int freq;
    struct Huffman* parent;
    struct Huffman* left_child;
    struct Huffman* right_child;
};
typedef struct Huffman Huffman;

struct Tablica_Kodowa//STRUKTURA TABLICY KODOWEJ
{
    int symbol;
    char kod_binarny[20];
};
typedef struct Tablica_Kodowa Tablica_Kodowa;

struct TreeStruct//STRUKTURA DRZEWA
{
    int symbol;
    int freq;
    int left_child_symbol;
    int right_child_symbol;
    struct TreeStruct* parent;
    struct TreeStruct* left_child;
    struct TreeStruct* right_child;
};
typedef struct TreeStruct TreeStruct;

Huffman huffmanModelArray[MODEL_ARR_LEN];//TABLICA MODELU
Huffman huffmanTreeArray[MODEL_ARR_LEN];//TABLICA DRZEWA I JEGO WARTOSCI
Huffman huffmanTreeArray_for_freq[MODEL_ARR_LEN];//TABLICA DRZEWA DLA FREQ
Huffman Tree[MODEL_ARR_LEN];//TABLICA DRZEWO
Tablica_Kodowa huffmanCodelArray[MODEL_ARR_LEN];//TABLICA DLA TABELI KODOWEJ

int liczba_znakow = 0;
int liczba_dodanych_znakow = 0;
int liczba_dodanych_znakow_w_drzewie = 0;
int liczba_dodanych_znakow_po_odczytaniu_drzewa = 0;
char nazwa_pliku_poczatkowego[50];
char nazwa_pliku_koncowego[50];
char nazwa_pliku_koncowego2[50];
char nazwa_pliku_do_dekompresji[50];
char nazwa_pliku_drzewa[50];
char rozszerzenie[] = ".txt";
char model[] = ".model_not_sorted";
char model_sorted[] = ".model_sorted";
char tree[] = ".graf";
char code[] = ".code";
//char plik_comp[] = ".compressed";
char plik_decomp[] = ".decompressed";
int sorted = 0;
int is_there_a_file = 0;
int is_there_a_file2 = 0;
TreeStruct *root;

int CompareHuffmanNodes(const void *item1, const void *item2)//POROWNANIE DWOCH NODE I KTORY JEST WIEKSZY
{
    Huffman *node1 = (Huffman *)item1;
    Huffman *node2 = (Huffman *)item2;

    int compareRes = (node1->freq - node2->freq);

    if(compareRes == 0)//JEZELI FREQ JEST ROWNE TO PATRZY NA SYMBOL I KTORY JEST WIEKSZY
    {
        compareRes = (node1->symbol - node2->symbol);
    }

    return -compareRes;
}

void GenerateModelFromFile()
{
    for (int k=0; k<MODEL_ARR_LEN; ++k)//DEKLARACJA TABELI
    {
        huffmanModelArray[k].symbol = k;
        huffmanModelArray[k].freq = 0;
        huffmanModelArray[k].left_child = NULL;
        huffmanModelArray[k].right_child = NULL;
        huffmanModelArray[k].parent = NULL;
    }
    FILE *f_in;
    unsigned char buffer[1];
    int readCount = 0;
    int readBytesLength = 1;
    char j[1];
    gets(j);
    printf("Podaj nazwe pliku z ktorego pobrac dane [bez rozszerzenia]: ");
    gets(nazwa_pliku_poczatkowego);
    strcat(nazwa_pliku_poczatkowego, rozszerzenie);
    if((f_in = fopen(nazwa_pliku_poczatkowego, "rb")) == NULL)
    {
        printf("\n\n");
        printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!\n");
        is_there_a_file = 0;
    }
    else
    {
        printf("Podaj nazwe pliku do ktorego wpisac dane nieposortowane i posortowane [bez rozszerzenia]: ");
        gets(nazwa_pliku_koncowego);
        f_in = fopen(nazwa_pliku_poczatkowego, "rb");

        while(readCount = fread(buffer, sizeof(unsigned char), readBytesLength, f_in))//POBRANIE Z PLIKU SYMBOLI I ZAMIANA ICH NA ASCII I WPISANIE W TABLICE
        {
            huffmanModelArray[buffer[0]].freq++;
            liczba_znakow++;
        }
        printf("-----------------------------------------\n");
        printf("PRZED SORTOWANIEM\n");
        printf("-----------------------------------------\n");
        printf("Liczba znakow w pliku: %d\n\n", liczba_znakow);
        for (int i=0; i<MODEL_ARR_LEN; ++i)// WYPISANIE WARTOSCI
        {
            if(huffmanModelArray[i].freq != 0)
            {
                printf("%d: %d\n", huffmanModelArray[i].symbol, huffmanModelArray[i].freq);
                liczba_dodanych_znakow++;
            }
        }
        fclose(f_in);
        is_there_a_file = 1;
    }
}

void SortModel(Huffman *huffmanModelArray, int arr_len)
{
    printf("-----------------------------------------\n");
    printf("PO SORTOWANIU\n");
    printf("-----------------------------------------\n");
    if(is_there_a_file == 1)
    {
        printf("Liczba znakow w pliku: %d\n\n", liczba_znakow);
        qsort(huffmanModelArray, arr_len, sizeof(Huffman), CompareHuffmanNodes);//SORTOWANIE TABLICY
        for (int i=0; i<MODEL_ARR_LEN; ++i)//WYPISANIE WARTOSCI
        {
            if(huffmanModelArray[i].freq != 0)
            {
                printf("%d: %d\n", huffmanModelArray[i].symbol, huffmanModelArray[i].freq);

            }
        }
        sorted = 1;
    }
    else if(is_there_a_file == 0)
    {
        printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!\n");
    }

}

void WriteModelToFile()
{
    if(is_there_a_file == 1)
    {
        FILE *f_out;
        char nazwa_pliku2[50];
        if(sorted == 1)
        {
            strcpy(nazwa_pliku2, nazwa_pliku_koncowego);
            strcat(nazwa_pliku2, model_sorted);
        }
        else if(sorted == 0)
        {
            strcpy(nazwa_pliku2, nazwa_pliku_koncowego);
            strcat(nazwa_pliku2, model);
        }
        //strcat(nazwa_pliku2, rozszerzenie);
        f_out = fopen(nazwa_pliku2,"wb");
        fprintf(f_out, "Liczba znakow w pliku: %d\n\n", liczba_znakow);
        for (int i=0; i<MODEL_ARR_LEN; ++i)//WYPISANIE WARTOSCI
        {
            if(huffmanModelArray[i].freq != 0)
            {
                fprintf(f_out, "%d: %d\n", huffmanModelArray[i].symbol, huffmanModelArray[i].freq);
            }
        }
        fclose(f_out);
    }
    else if(is_there_a_file == 0)
    {
        //printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!\n");
    }

}



void GenerateHuffmanTreeFromModel()
{
    printf("-----------------------------------------\n");
    printf("DRZEWO\n");
    printf("-----------------------------------------\n");
    if(is_there_a_file == 1)
    {
        int symbolCounter = liczba_dodanych_znakow;
        int node_name = 501;
        for(int i=0; i<liczba_dodanych_znakow; i++)//DEKLARACJA TABLICY DRZEWA
        {
            huffmanTreeArray[i].symbol   = huffmanModelArray[i].symbol;
            huffmanTreeArray[i].freq = huffmanModelArray[i].freq;
        }

        for(int k=0; k<liczba_dodanych_znakow-1; k++)//TWORZENIE DRZEWA I JEGO POLACZEN Z MODELU ZRODLA
        {
            Tree[k].parent = node_name;
            Tree[k].left_child = huffmanTreeArray[liczba_dodanych_znakow-1-k].symbol;
            Tree[k].right_child = huffmanTreeArray[liczba_dodanych_znakow-2-k].symbol;

            huffmanTreeArray_for_freq[k].symbol = node_name;
            huffmanTreeArray_for_freq[k].freq = huffmanTreeArray[liczba_dodanych_znakow-1-k].freq + huffmanTreeArray[liczba_dodanych_znakow-2-k].freq;

            huffmanTreeArray[liczba_dodanych_znakow-2-k].symbol = node_name;
            huffmanTreeArray[liczba_dodanych_znakow-2-k].freq = huffmanTreeArray[liczba_dodanych_znakow-1-k].freq + huffmanTreeArray[liczba_dodanych_znakow-2-k].freq;

            symbolCounter--;
            qsort(huffmanTreeArray, symbolCounter, sizeof(Huffman), CompareHuffmanNodes);
            liczba_dodanych_znakow_w_drzewie++;
            node_name++;
        }
    }
    else if(is_there_a_file == 0)
    {
        //printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!\n");
    }


}

void string_changer(int znak, char string[20], int is_none)//ZAMIANA PEWNEGO STRINGU NA JAKAS INNA WARTOSC W ZALEZNOSCI OD WARUNKU SPRINTF - DO JEDNEGO STRINGU WPISUJE INNA WARTOSC
{
    if(is_none == 1)
    {
        sprintf(string, "none");
    }
    else if(is_none == 0)
    {
        if(znak > 500)
        {
            sprintf(string, "#%d", znak % 500);
        }
        else
        {
            sprintf(string, "%d", znak);
        }
    }

}

int find_your_parent(int parent)//SPRAWDZENIE DLA DANEJ WARTOSCI INT CZY JEST DZIECKIEM JAKIM KOLWIEK JEZELI TAK TO ZWRACA WARTOSC RODZICA DLA TEGO INTA
{
    for(int i = 0; i<liczba_dodanych_znakow_w_drzewie; i++)
    {
        if(parent == Tree[i].left_child || parent == Tree[i].right_child) return Tree[i].parent;
    }
    return 0;
}

void WriteHuffmanTreeToFile()
{
    if(is_there_a_file == 1)
    {
        FILE *f_out;
        char nazwa_pliku3[50];
        strcpy(nazwa_pliku3, nazwa_pliku_koncowego);
        strcat(nazwa_pliku3, tree);
        //strcat(nazwa_pliku3, rozszerzenie);
        char numer_wezla[20];
        char parent[20];
        char left_child[20];
        char right_child[20];
        f_out = fopen(nazwa_pliku3,"wb");
        for(int i = liczba_dodanych_znakow_w_drzewie-1; i >=0; i--)//DLA KAZDEJ WARTOSCI TWORZENIE LADNEGO DRZEWA Z ROZNYMI DODATKAMI NP ZMIANY STRING NA NODNE ALBO #
        {
            string_changer(Tree[i].parent, numer_wezla, 0);
            if(find_your_parent(Tree[i].parent) == 0) string_changer(parent, parent, 1);
            else string_changer(find_your_parent(Tree[i].parent), parent, 0);
            if(Tree[i].left_child == NULL) string_changer(left_child, left_child, 1);
            else string_changer(Tree[i].left_child, left_child, 0);
            if(Tree[i].right_child == NULL) string_changer(right_child, right_child, 1);
            else string_changer(Tree[i].right_child, right_child, 0);

            printf("%s: %d      Left Child: %s      Right Child: %s      Parent: %s\n", numer_wezla, huffmanTreeArray_for_freq[i].freq, left_child, right_child, parent);
            fprintf(f_out, "%s: %d      Left Child: %s      Right Child: %s      Parent: %s \n", numer_wezla, huffmanTreeArray_for_freq[i].freq, left_child, right_child, parent);
        }
        for(int i = 0; i < liczba_dodanych_znakow; i++)
        {
            string_changer(huffmanModelArray[i].symbol, numer_wezla, 0);
            if(find_your_parent(huffmanModelArray[i].symbol) == 0) string_changer(parent, parent, 1);
            else string_changer(find_your_parent(huffmanModelArray[i].symbol), parent, 0);
            if(huffmanModelArray[i].left_child == NULL) string_changer(left_child, left_child, 1);
            else string_changer(huffmanModelArray[i].left_child, left_child, 0);
            if(huffmanModelArray[i].right_child == NULL) string_changer(right_child, right_child, 1);
            else string_changer(huffmanModelArray[i].right_child, right_child, 0);
            printf("%s: %d      Left Child: %s      Right Child: %s      Parent: %s\n", numer_wezla, huffmanModelArray[i].freq, left_child, right_child, parent);
            fprintf(f_out, "%s: %d      Left Child: %s      Right Child: %s      Parent: %s \n", numer_wezla, huffmanModelArray[i].freq, left_child, right_child, parent);
        }
        fclose(f_out);
    }
    else if(is_there_a_file == 0)
    {
        printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!\n");
    }

}

void lewo_prawo(char znak, int wsk, int i, int index, int bit)//W ZALEZNOSCI OD WEZLA CZY LEWY CZY PRAWY TO PRZYPISANIE MU WARTOSCI 0 LUB 1 I WPISANIE DO TABLICY I JEZELI DOJDZIE DO KONCA DRZEWEA TO POWROT NA GORRE
{
    for(int j=0; j<liczba_dodanych_znakow_w_drzewie; j++)
    {
        if(wsk == Tree[j].right_child || wsk == Tree[j].left_child)
        {
            if(wsk == Tree[j].right_child) znak = '1';
            else if (wsk == Tree[j].left_child) znak = '0';
            wsk = Tree[j].parent;
            if(bit == 1)
            {
                huffmanCodelArray[i].kod_binarny[index] = znak;
                index++;
            }
        }

        if(huffmanCodelArray[i].symbol == Tree[j].right_child || huffmanCodelArray[i].symbol == Tree[j].left_child)
        {
            if(huffmanCodelArray[i].symbol == Tree[j].right_child) znak = '1';
            else if(huffmanCodelArray[i].symbol == Tree[j].left_child) znak = '0';
            bit = 1;
            huffmanCodelArray[i].kod_binarny[index] = znak;
            index++;
            wsk=Tree[j].parent;
        }
        else if(wsk == -1) wsk = Tree[j].parent;
    }
}

void GenerateCodeTableFromTree()
{
    printf("-----------------------------------------\n");
    printf("TABLICA KODOWA\n");
    printf("-----------------------------------------\n");
    if(is_there_a_file == 1)
    {
        if(liczba_dodanych_znakow == 1)//JEZELI LICZBA ZNAKOW JEST 1 TO PO PROSTU WPISUJE ROOTA
        {
            huffmanCodelArray[0].symbol = huffmanModelArray[0].symbol;
            huffmanCodelArray[0].kod_binarny[0] = '0';
        }
        else
        {
            for(int i=0; i<liczba_dodanych_znakow; i++)//ZAINICJALIZOWANIE TABLICY TABELI KODOWEJ I JEJ SYMBOLOW
            {
                huffmanCodelArray[i].symbol = huffmanModelArray[i].symbol;
            }
            int index;
            int bit;
            int wsk;
            for(int i=0; i<liczba_dodanych_znakow; i++)//DLA KAZDEGO ELEMENTU DRZEWA STWORZENIE POLACZEN I NA JEGO PODSTAWIE USTALENIE WARTOSCI
            {
                index = 0;
                wsk = -1;
                bit = 0;
                char zero_or_one;
                lewo_prawo(zero_or_one, wsk, i, index, bit);
            }
        }

    }
    else if(is_there_a_file == 0)
    {
        //printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!\n");
    }

}

void WriteCodeTableToFile()
{
    if(is_there_a_file == 1)
    {
        FILE *f_out;
        char nazwa_pliku4[50];
        strcpy(nazwa_pliku4, nazwa_pliku_koncowego);
        strcat(nazwa_pliku4, code);
        //strcat(nazwa_pliku4, rozszerzenie);
        f_out = fopen(nazwa_pliku4,"wb");
        printf("\n");
        printf("  Char  | Huffman code ");
        printf("\n--------------------\n");
        fprintf(f_out, " Char | Huffman code ");
        fprintf(f_out, "\n--------------------\n");
        for(int i=0; i<liczba_dodanych_znakow; i++)//WPISANIE W PLIK
        {
            printf(" %d\t| %s\t\n", huffmanCodelArray[i].symbol, strrev(huffmanCodelArray[i].kod_binarny));
            fprintf(f_out, " %d\t| %s\t\n", huffmanCodelArray[i].symbol, huffmanCodelArray[i].kod_binarny);
        }
        fclose(f_out);
    }
    else if(is_there_a_file == 0)
    {
        printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!");
    }

}

void WriteCompressedFile()
{
    printf("\n-----------------------------------------\n");
    printf("KOMPRESJA\n");
    printf("-----------------------------------------\n");
    if(is_there_a_file == 1)
    {
        FILE *f_out;
        FILE *f_in;
        char nazwa_pliku5[50];
        char content;
        strcpy(nazwa_pliku5, nazwa_pliku_koncowego);
        //strcat(nazwa_pliku5, plik_comp);
        //strcat(nazwa_pliku5, rozszerzenie);
        f_in = fopen(nazwa_pliku_poczatkowego, "rb");
        f_out = fopen(nazwa_pliku5,"wb");
        unsigned char buffer1[1];
        unsigned char buffer2[1];
        int readCount = 0;
        int readBytesLength = 1;
        unsigned char bitCounter = 0;
        unsigned char current = 0;
        while(readCount = fread(buffer1, sizeof(unsigned char), readBytesLength, f_in))//ZCZYTANIE Z PLIKU WEJSCIOWEGO I WPISANIE JAKO ASCII
        {
            int i = 0;
            while(buffer1[0]!= huffmanCodelArray[i].symbol)//ZNALEZIENIE INDEKSU SYMBOLU W TABLICY KODOWEJ
            {
                i++;
            }
            for(int j = 0; j < sizeof(huffmanCodelArray[i].kod_binarny); j++)//PRZESUNIECIA BITOWE DLA KAZDEGO SYMBOLU
            {
                if(huffmanCodelArray[i].kod_binarny[j] == 49)
                {
                    current = current << 1|1;
                    bitCounter++;
                }
                else if(huffmanCodelArray[i].kod_binarny[j] == 48)
                {
                    current = current << 1;
                    bitCounter++;
                }
                if (bitCounter == 8)
                {
                    buffer2[0] = current;
                    fwrite(buffer2,1,1,f_out);
                    current = 0;
                    bitCounter = 0;
                }
            }
        }
        if(bitCounter != 0)//OSTATNI BIT UZUPENIANY JEST JAKO 0
        {
            while(bitCounter != 8)
            {
                current = current << 1;
                bitCounter++;
            }
            buffer2[0] = current;
            fwrite(buffer2,1,1,f_out);
        }

        printf("\nPOMYSLNIE ZKOMPRESOWANO DO PLIKU O NAZWIE: %s\n\n", nazwa_pliku5);
        fclose(f_in);
        fclose(f_out);
        system("pause");
        system("cls");
    }
    else if(is_there_a_file == 0)
    {
        printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!\n\n\n");
        system("pause");
        system("cls");
    }
}

void ReadHuffmanTreeFromFile_ReadCodeTableFromFile(TreeStruct TreeArray[])
{
    FILE *f_in;
    char nazwa_pliku6[50];
    unsigned char buffer[1];
    int readCount = 0;
    int size = 0;
    int readBytesLength = 1;
    int Colon_index = 0;
    int Hashtag_symbol = 0;
    int temp = 0;
    int i = 0;
    int j = 0;
    //strcpy(nazwa_pliku6, nazwa_pliku_koncowego);
    //strcat(nazwa_pliku6, tree);
    //strcat(nazwa_pliku6, rozszerzenie);
    f_in = fopen(nazwa_pliku_drzewa,"rb");
    printf("-----------------------------------------\n");
    printf("CZYTANIE DRZEWA Z PLIKU\n");
    printf("-----------------------------------------\n");
    while(readCount = fread(buffer, sizeof(unsigned char), readBytesLength, f_in))//ZCZYTANIE Z PLIKU DRZEWA I JAKO ASCII
    {
        if(buffer[0] < 58 && buffer[0] > 47)//SPRAWDZANIE CZY JEST ASCII I NADPISANIE WARTOSCI TEMP
        {
            temp = temp * 10;
            temp = temp + buffer[0] - '0';
        }
        else if(buffer[0] == 10)//KTORA TO NOWA LINIA I JEZELI NOWA LINIA TO INDEX DWUKROPKA JEST ZEROWANY
        {
            Colon_index = 0;
            i++;
        }
        else if(buffer[0] == 35)//ZNALEZIENIE HASZTAGU I ZMIANA NA NAZWE WEZLA
        {
            Hashtag_symbol = Hashtag_symbol + 500;
        }
        else if(temp == 0 && buffer[0] == 58)//ZNALEZIENIE :
        {
            Colon_index++;
        }
        else
        {
            if(temp != 0)// W ZALEZNOSCI OD : JEST INNA AKCJA 0. ZMIANA SYMBOLU ZNAKU 1. ZMIANA FREQ 2. SYMBOL LEWEGO 3. SYMBOL PRAWEGO 4. STWORZENIE OJCA
            {
                if(Colon_index == 0)
                {
                    TreeArray[i].symbol = temp + Hashtag_symbol;
                    temp = 0;
                    Hashtag_symbol = 0;
                    Colon_index++;
                }
                if(Colon_index == 1)
                {
                    TreeArray[i].freq = temp;
                    temp = 0;
                    Hashtag_symbol = 0;
                }
                if(Colon_index == 2)
                {
                    TreeArray[i].left_child_symbol = temp + Hashtag_symbol;
                    temp = 0;
                    Hashtag_symbol = 0;
                }
                if(Colon_index == 3)
                {
                    TreeArray[i].right_child_symbol = temp + Hashtag_symbol;
                    temp = 0;
                    Hashtag_symbol = 0;
                }
                if(Colon_index == 4)
                {
                    temp = temp + Hashtag_symbol;
                    while(TreeArray[j].symbol != temp)//ZNALEZIENIE INDEKSU TEGO OJCA I POTEM PRZYPISANIE JEGMU WSZYSTKICH WSKAZNIKOW I ZMIENNYCH
                    {
                        j++;
                    }
                    TreeArray[i].parent = &TreeArray[j];
                    if(TreeArray[j].left_child_symbol == TreeArray[i].symbol)
                    {
                        TreeArray[j].left_child = &TreeArray[i];
                    }
                    else
                    {
                        TreeArray[j].right_child = &TreeArray[i];
                    }
                    j = 0;
                    temp = 0;
                    Hashtag_symbol = 0;
                }
            }
        }
    }
    printf("\nPOMYSLNIE ODCZYTANO DRZEWO Z PLIKU O NAZWIE: %s\n\n", nazwa_pliku_drzewa);
    liczba_dodanych_znakow_po_odczytaniu_drzewa = TreeArray[0].freq;
    root = &TreeArray[0];
    fclose(f_in);
}

void WriteDecompressedFile(TreeStruct TreeArray[])
{
    FILE *f_out;
    FILE *f_in;
    char nazwa_pliku8[50];
    f_in = fopen(nazwa_pliku_do_dekompresji,"rb");
    f_out = fopen(nazwa_pliku_koncowego2,"wb");
    int signCounter = liczba_dodanych_znakow_po_odczytaniu_drzewa;
    printf("-----------------------------------------\n");
    printf("DEKOMPRESJA\n");
    printf("-----------------------------------------\n");
    unsigned char buffer1[1];
    unsigned char buffer2[1];
    int readCount = 0;
    int readBytesLength = 1;
    unsigned char currentbit;
    while((readCount = fread(buffer1, sizeof(unsigned char), readBytesLength, f_in)) || signCounter !=0)//ZCZYTANIE Z PLIKU I DEKOMPRESJA POPRZEZ UZYWANIE WSKAZNIKOW I PRZESUNIEC BITOWYCH
    {
        for(int i = 7; i >= 0; i--)
        {
            currentbit = buffer1[0] >> i&1;
            if(signCounter > 0)
            {
                if(root->symbol >= 500 && currentbit == 0)
                {
                    root = root->left_child;
                }
                else if(root->symbol >= 500 && currentbit == 1)
                {
                    root = root->right_child;
                }
                else if(root->symbol < 500)//WPISANIE DO PLIKU I POWROT ROOTA DO POCZATKU
                {
                    buffer2[0] = root->symbol;
                    fwrite(buffer2,1,1,f_out);
                    signCounter--;
                    if(currentbit == 1)
                    {
                        root = TreeArray[0].right_child;
                    }
                    else
                    {
                        root = TreeArray[0].left_child;
                    }
                }
            }
        }
    }
    printf("\nPOMYSLNIE ZDEKOMPRESOWANO PLIK I WPISANO WYNIK DO PLIKU O NAZWIE: %s\n\n", nazwa_pliku_koncowego2);
    fclose(f_in);
    fclose(f_out);
    system("pause");
    system("cls");
}

//-----Zad 1-----
void MODEL()
{
    GenerateModelFromFile();
    WriteModelToFile();
    SortModel(huffmanModelArray, MODEL_ARR_LEN);
    WriteModelToFile();
}

//-----Zad 2-----
void TREE_CODE_TABLE()
{
    GenerateHuffmanTreeFromModel();
    WriteHuffmanTreeToFile();
    GenerateCodeTableFromTree();
    WriteCodeTableToFile();
}

//-----Zad 3-----
void KOMPRESUJ()
{
    WriteCompressedFile();
}

void DEKOMPRESUJ()
{
    FILE *f_in;
    char nazwa_pliku9[50];
    unsigned char buffer[1];
    int readCount = 0;
    int size = 0;
    int readBytesLength = 1;
    char j[1];
    gets(j);

    printf("Podaj nazwe pliku do dekompresji [bez rozszerzenia]: ");
    gets(nazwa_pliku_do_dekompresji);
    //strcat(nazwa_pliku_do_dekompresji, plik_comp);
    //strcat(nazwa_pliku_do_dekompresji, rozszerzenie);

    if((f_in = fopen(nazwa_pliku_do_dekompresji, "rb")) == NULL)
    {
        printf("\n\n");
        printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!\n\n\n");
        is_there_a_file2 = 0;
        system("pause");
        system("cls");
    }
    else
    {
        printf("Podaj nazwe pliku drzewa [bez rozszerzenia]: ");
        gets(nazwa_pliku_drzewa);
        if((f_in = fopen(nazwa_pliku_drzewa, "rb")) == NULL)
        {
            printf("\n\n");
            printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!\n\n\n");
            is_there_a_file2 = 0;
            system("pause");
            system("cls");
        }
        else
        {
            strcat(nazwa_pliku_drzewa, tree);
            //strcat(nazwa_pliku9, rozszerzenie);
            f_in = fopen(nazwa_pliku_drzewa,"rb");
            printf("Podaj nazwe pliku wyjsciowego [bez rozszerzenia - domyslne rozszerzenie to .decompressed]: ");
            gets(nazwa_pliku_koncowego2);
            strcat(nazwa_pliku_koncowego2, plik_decomp);
            //strcat(nazwa_pliku_koncowego2, rozszerzenie);
            while(readCount = fread(buffer, sizeof(unsigned char), readBytesLength, f_in))//ZCZYTANIE Z PLIKU I WPISANIE DO TABLICY DRZEWA I ZARAZERWOWANIE MIEJSCA I DEKLARACJA ILE MA BYC MIEJSCA
            {
                if(buffer[0] == 10)
                {
                    size++;
                }
            }
            TreeStruct TreeArray[size];
            fclose(f_in);

            ReadHuffmanTreeFromFile_ReadCodeTableFromFile(TreeArray);
            WriteDecompressedFile(TreeArray);
            is_there_a_file2 = 1;
        }

    }
}

int main()
{
    int komenda;
	while(1)
    {
        printf("HUFFMAN CODING\n");
        printf("----------------------------------------------\n");
        printf("[0] Wyjscie z programu\n");
        printf("[1] Kompresja\n");
        printf("[2] Dekompresja\n");
        printf("----------------------------------------------\n\n");
		printf("\nWybrana komenda: ");
		scanf("%d", &komenda);
		printf("\n");
		switch (komenda)
		{
			case 0: return 0; break;
			case 1: MODEL(); TREE_CODE_TABLE(); KOMPRESUJ(); break;
			case 2: DEKOMPRESUJ(); break;
		}
    }
    return 0;
}
