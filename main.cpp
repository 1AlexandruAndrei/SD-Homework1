#include <fstream>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <ctime>
using namespace std;
using namespace std::chrono;
ifstream f("algsort.in");
ifstream f2("manual.in");
unsigned int w[100000000];
unsigned int v[100000000];
unsigned int vect[100000000];
unsigned int aux[100000000];
int ordonare(unsigned int left, unsigned int right, unsigned int pos)
{
    unsigned int j = 0;
    for (unsigned int i = left; i < right; i++)
        if (v[i] <= v[pos] && i != pos) /// daca e mai mic sau egal decat pivotul (si nu e pivot)
        {
            aux[j] = v[i]; /// punem in auxiliar daca elementul<=pivot
            j++;           /// incrementam indexul
        }

    aux[j] = v[pos];                    /// pivotul il punem in vectorul auxiliar
    unsigned int poz_pivot2 = j + left; /// pozitia pivot nou
    j++;

    for (unsigned int i = left; i < right; i++)
        if (v[i] > v[pos])
        {
            aux[j] = v[i]; /// daca nr>pivot, il punem in auxiliar
            j++;           /// incrementam indexul
        }

    for (unsigned int i = left; i < right; i++)
        v[i] = aux[i - left]; /// copiem elementele sortate in vechiul vector

    return poz_pivot2; /// returnam pozitia elementului pivot nou
}

void quicksort(unsigned int left, unsigned int right)
{
    if (left < right)
    {
        srand(time(0));
        unsigned int pos = left + rand() % (right - left); /// pozitie random pt pivot
        pos = ordonare(left, right, pos);                  /// noua pozitie

        quicksort(left, pos);
        quicksort(pos + 1, right);
        /// sortam recursiv dreapta si stanga
    }
}

void shellsort(unsigned int v[], unsigned int n)
{
    /// worst case: shell sort=insertion sort
    for (unsigned int gap = n / 2; gap > 0; gap--) /// micsoram gap ul dintre numere
    {
        for (unsigned int i = gap; i < n; i += 1)
        {
            unsigned int aux = v[i];

            unsigned int j;
            for (j = i; j >= gap && v[j - gap] > aux; j = j - gap)
                v[j] = v[j - gap];
            v[j] = aux; /// pun elementul in pozitia buna
        }
    }
}

void mergesort(unsigned int left, unsigned int right)
{
    /// Complexitate totala O(nlogn)
    if (left < right)
    {
        unsigned int mid = (left + right) / 2;

        mergesort(left, mid);
        mergesort(mid + 1, right);
        /// DEI

        unsigned int k = 0, i = left, j = mid + 1;
        while (i <= mid && j <= right)
        {
            if (v[i] < v[j])
                w[++k] = v[i++];
            else
                w[++k] = v[j++];
            /// merge-uim 2 subvectori si cream noul vector
            /// cu elementele sortate din cei 2 subvectori
        }
        while (i <= mid)
            w[++k] = v[i++];
        /// copiem ce ramane in stanga
        while (j <= right)
            w[++k] = v[j++];
        /// si in dreapta
        for (i = left, j = 1; i <= right; i++, j++)
            v[i] = w[j]; /// punem tot in vectorul v
        /// mergem de la stanga la dreapta pentru a lua toate elementele
    }
}

void countsort(unsigned int v[], unsigned int n)
{
    unsigned int max_element = v[0];
    for (unsigned int i = 1; i < n; i++)
    {
        if (v[i] > max_element)
        {
            max_element = v[i];
        }
    }
    /// am gasit elementul maxim

    /// facem un vector nou de frecvente
    unsigned int contor[max_element + 1];
    for (int i = 0; i <= max_element; i++)
    {
        contor[i] = 0;
    }

    /// vector de frecvente
    for (unsigned int i = 0; i < n; i++)
    {
        contor[v[i]]++;
    }

    /// in contor avem nr de numere mai mici sau egale decat fiecare element
    for (int i = 1; i <= max_element; i++)
    {
        contor[i] += contor[i - 1];
    }

    unsigned int result[n]; /// aici stocam vectorul sortat
    for (int i = n - 1; i >= 0; i--)
    { /// iteram de la dreapta la stanga si punem nr unde trebuie in vectorul result
        result[contor[v[i]] - 1] = v[i];
        contor[v[i]]--; /// scadem pe masura ce punem cate un nr
    }
    /// copiem din rezultat in vectorul vechi
    for (int i = 0; i < n; i++)
    {
        v[i] = result[i];
    }
}

/// functia countingsort necesara pentru radixsort
void countingsort(unsigned int v[], unsigned int n, int e)
{
    int i, count[10] = {0};
    unsigned int w[n];
    for (i = 0; i < n; i++)
        count[(v[i] / e) % 10]++; /// frecventa

    for (i = 1; i < 10; i++)
        count[i] += count[i - 1]; /// adaugam ce e in stanga

    for (i = n - 1; i >= 0; i--)
    {
        w[count[(v[i] / e) % 10] - 1] = v[i]; /// punem numarul in pozitia lui in care trebuie sa apara
        count[(v[i] / e) % 10]--;             /// scadem contorul lui pe masura ce punem numarul
    }

    for (i = 0; i < n; i++)
        v[i] = w[i]; /// copiem din rezultat in vectorul vechi
}

void radixsort(unsigned int v[], unsigned int n)
{
    unsigned int m = v[0];
    for (unsigned int i = 1; i < n; i++)
        if (v[i] > m)
            m = v[i];
    /// maximul din vector
    for (unsigned int e = 1; m / e > 0; e *= 10)
        countingsort(v, n, e);
}

///algoritm de sortare nativ
void sortArray(unsigned int v[], int n) {
    sort(v, v + n);
}


/// verific daca vectorul a fost sortat corect
bool check(unsigned int v[], unsigned int n)
{
    for (unsigned int i = 1; i < n; i++)
        if (v[i] < v[i - 1])
            return false;
    return true;
}

/// verific daca vectorul are valori distincte sau nu
bool egale(unsigned int v[], unsigned int n)
{
    for (unsigned int i = 1; i < n; i++)
        if (v[i] != v[i - 1])
            return 0;
    return 1;
}
int main()
{
    /// pentru citire manuala, din fisierul manual.in
    /// pentru ambele moduri de citire a datelor de intrare, timpul este masurat in nanosecunde

    /*unsigned int n;
    srand(time(0));
    f2>>n;
    if(n>100000000)
    {
        cout<<"Sunt prea multe numere.";
        return 0;
    }

    for(int i=0;i<n;i++)
        {
            f2>>v[i];
                if(v[i]>=4294967295)
                {
                    cout<<"Unul dintre numerele introduse este prea mare si nu se poate sorta vectorul.";
                    return 0;
                }
        }

    auto start = high_resolution_clock::now();


    ///quicksort(0,n);
    ///radixsort(v,n);
    mergesort(0,n-1);
    ///shellsort(v,n);
    ///countsort(v,n);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);

    cout<<"Timpul de executie pentru test"<<" este "<<duration.count() << endl;
    */

    /// pentru input automat din algsort.in
    srand(time(0)); // initialize random seed

    unsigned int i, nrteste;
    unsigned int n, nmaxi;
    f >> nrteste;
    int nrtestcurent = 0;
    while (nrtestcurent < 5 * nrteste)
    {
        f >> n;

        if (n > 1000000000)
        {
            cout << "Nu putem testa algoritmii, sunt prea multe numere de input";
            return 0;
        }

        f >> nmaxi;

        for (i = 0; i < n; i++)
        {
            vect[i] = rand() % nmaxi;
            v[i] = vect[i];
        }

        auto start = high_resolution_clock::now();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);

        if (nmaxi >= 100000000)
            cout << "RadixSort nu poate sorta, deoarece nmaxi este prea mare" << endl;
        else
        {
            start = high_resolution_clock::now();

            radixsort(v, n);

            stop = high_resolution_clock::now();
            duration = duration_cast<nanoseconds>(stop - start);

            if (check(v, n) == 1)
                cout << "Timp de executie pentru testul"
                     << " de la radixsort este " << duration.count() << endl;

            else
                cout << "Vectorul nu a fost sortat corect." << endl;
        }
       

        nrtestcurent++;

        ///------------------------------------------------------------------------

        for (i = 0; i < n; i++)
            v[i] = vect[i];
        if (n >= 100000000)
            cout << "Qucicksort nu poate sorta, deoarece nmaxi este prea mare" << endl;
        else
        {
            if (egale(v, n) == 1)
                cout << "Quicksort nu este deloc eficient pentru acest input, deoarece toate numerele sunt egale.";
            else
            {
                start = high_resolution_clock::now();
                quicksort(0, n);
                stop = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(stop - start);

                if (check(v, n) == 1)
                    cout << "Timp de executie pentru testul"
                         << " de la quicksort este " << duration.count() << endl;
                else
                    cout << "Vectorul nu a fost sortat corect." << endl;
            }
        }

        nrtestcurent++;

        ///------------------------------------------------------------------------

        for (i = 0; i < n; i++)
            v[i] = vect[i];

        if (nmaxi > 1000000)
            cout << "CountSort nu poate sorta, deoarece nmaxi este prea mare" << endl;
        else if (n >= 100000000)
            cout << "CountSort nu poate sorta, deoarece sunt prea multe numere" << endl;
        else
        {

            start = high_resolution_clock::now();
            countsort(v, n);
            stop = high_resolution_clock::now();
            duration = duration_cast<nanoseconds>(stop - start);

            if (check(v, n) == 1)
                cout << "Timp de executie pentru testul"
                     << " de la countsort este " << duration.count() << endl;
            else
                cout << "Vectorul nu a fost sortat corect." << endl;
        }

        nrtestcurent++;

        ///------------------------------------------------------------------------

        for (i = 0; i < n; i++)
            v[i] = vect[i];
        if(egale(v,n)==1)
        cout<<"Algorimtul nu va efectua sortarea eficient deoarece numerele sunt egale. "<<endl;
        else{
            start = high_resolution_clock::now();
        shellsort(v, n);
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);

        if (check(v, n) == 1)
            cout << "Timp de executie pentru testul"
                << " de la shellsort este " << duration.count() << endl;
        else
            cout << "Vectorul nu a fost sortat corect." << endl;

        }
        

        nrtestcurent++;

        ///------------------------------------------------------------------------

        for (i = 0; i < n; i++)
            v[i] = vect[i];

        if (n > 10000000)
            cout << "MergeSort nu poate sorta, deoarece sunt prea multe numere" << endl;
        else
        {
            start = high_resolution_clock::now();
            mergesort(0, n - 1);
            stop = high_resolution_clock::now();
            duration = duration_cast<nanoseconds>(stop - start);

            if (check(v, n) == 1)
                cout << "Timp de executie pentru testul"
                     << " de la mergesort este " << duration.count() << endl;

            else
                cout << "Vectorul nu a fost sortat corect." << endl;
        }

        nrtestcurent++;

        ///---------------------------------------------------------------------------------

        for (i = 0; i < n; i++)
            v[i] = vect[i];

        if (n > 10000000)
            cout << "Algoritmul de sortare nativ nu poate sorta, deoarece sunt prea multe numere" << endl;
        else
        {
            start = high_resolution_clock::now();
            sortArray(v, n);
            stop = high_resolution_clock::now();
            duration = duration_cast<nanoseconds>(stop - start);

            if (check(v, n) == 1)
                cout << "Timp de executie pentru testul"
                     << " de la algoritmul de sortare nativ este " << duration.count() << endl;

            else
                cout << "Vectorul nu a fost sortat corect." << endl;
        }

        nrtestcurent++;

        cout << endl;
    }
    cout << "THE END";
    f.close();
    f2.close();
    return 0;
}