#include<fstream>
#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<algorithm>
#include<chrono>
#include<ctime>
using namespace std;
using namespace std::chrono;
ifstream f("algsort.in");
ifstream f2("manual.in");
int w[100000000];
int v[100000000];
int vect[100000000];
int aux[100000000];
int ordonare(int left, int right, int pos)
{
    int j=0,i;
    for(i=left; i<right; i++)
        if(v[i]<=v[pos]&&i!=pos)
        {
            aux[j]=v[i];///punem in auxiliar daca elementul<=pivot
            j++;
        }

    aux[j]=v[pos];///pivotul il punem in vectorul auxiliar
    int poz_pivot2=j+left;
    j++;

    for(i=left; i<right; i++)
        if(v[i]>v[pos])
        {
            aux[j]=v[i];///daca nr>pivot, il punem in auxiliar
            j++;
        }

    for(i=left; i<right; i++)
        v[i]=aux[i-left];///copiem elementele sortate in vechiul vector

    return poz_pivot2;///returnam pozitia elementului pivot
}

void quicksort(int left, int right)
{
    if(left<right)
    {
        srand(time(0));
        int pos=left+rand()%(right-left);///pozitie random pt pivot
        pos=ordonare(left, right, pos);///noua pozitie

        quicksort(left, pos);
        quicksort(pos+1, right);
        ///sortam recursiv dreapta si stanga
    }
}

void shellsort(int v[], int n)
{
    ///worst case: shell sort=insertion sort
    for(int gap=n/2; gap>0; gap--)///micsoram gap ul dintre numere
    {
        for(int i=gap; i<n; i+=1)
        {
            int aux=v[i];

            int j;
            for(j=i; j>=gap && v[j-gap]>aux; j=j-gap)
                v[j]=v[j-gap];
            v[j]=aux;///pun elementul in pozitia buna
        }
    }
}

void mergesort(int left,int right)
{
    ///Complexitate totala O(nlogn)
    if (left<right)
    {
        int mid=(left+right)/2;

        mergesort(left,mid);
        mergesort(mid+1,right);
        ///DEI

        int k=0,i=left,j=mid+1;
        while(i<=mid && j<=right)
        {
            if(v[i]<v[j])
                w[++k]=v[i++];
            else
                w[++k]=v[j++];
            ///merge-uim 2 subvecti si cream noul vector
            ///cu elementele sortate din cei 2 subvecti

        }
        while(i<=mid)
            w[++k]=v[i++];
        ///copiem ce ramane in stanga
        while(j<=right)
            w[++k]=v[j++];
        ///si in dreapta
        for(i=left,j=1; i<=right; i++,j++)
            v[i]=w[j];///punem tot in vectorul v.
        ///mergem de la stanga la dreapta pentru a lua toate elementele
    }
}

void countsort(int v[], int n) {
    int max_element = v[0];
    for (int i = 1; i < n; i++) {
        if (v[i] > max_element) {
            max_element = v[i];
        }
    }
    ///am gasit elementul maxim

    ///facem un vector nou de frecvente
    int contor[max_element + 1];
    for (int i = 0; i <= max_element; i++) {
        contor[i] = 0;
    }

    ///vector de frecvente
    for (int i = 0; i < n; i++) {
        contor[v[i]]++;
    }

    ///in contor avem nr de numere mai mici sau egale decat fiecare element
    for (int i = 1; i <= max_element; i++) {
        contor[i] += contor[i - 1];
    }

    int result[n];///aici stocam vectorul sortat
    for (int i = n - 1; i >= 0; i--) { ///iteram de la dreapta la stanga si punem nr unde trebuie in result
        result[contor[v[i]] - 1] = v[i];
        contor[v[i]]--;///scadem pe masura ce punem cate un nr
    }
    ///copiem din rezultat in vectorul vechi
    for (int i = 0; i < n; i++) {
        v[i] = result[i];
    }
}

///functia countingsort necesara pentru radixsort
void countingsort(int v[], int n, int e)
{
    int w[n],i, count[10]={0};

    for(i=0;i<n;i++)
        count[(v[i]/e)%10]++;///frecventa

    for(i=1;i<10;i++)
        count[i]+=count[i-1];///adaugam ce e in stanga

    for(i=n-1;i>=0;i--)
    {
        w[count[(v[i]/e)%10]-1]=v[i];///punem numarul in pozitia lui in care trebuie sa apara
        count[(v[i]/e)%10]--;///scadem contorul lui pe masura ce punem numarul
    }

    for(i=0;i<n;i++)
        v[i]=w[i];///le punem in vectorul vechi

}

void radixsort(int v[], int n){
    int m=v[0];
    for(int i=1;i<n;i++)
        if (v[i]>m)
            m=v[i];
    ///maximul din vector
    for(int e=1;m/e>0;e*=10)
        countingsort(v,n,e);
}

///verific daca vectorul a fost sortat corect
bool check(int v[], int n)
{
    for(int i=1;i<n;i++)
        if(v[i]<v[i-1])
        return false;
    return true;
}

///verific daca vectorul are valori distincte
bool egale(int v[], int n)
    {
        for(int i=1;i<n;i++)
        if (v[i]!=v[i-1])
            return 0;
        return 1;
    }
int main()
{
    int n;
    f2>>n;
    for(int i=0;i<n;i++)
    f2>>v[i];

    
    auto start = high_resolution_clock::now();

    ///quicksort(0,n);
    ///radixsort(v,n);
    ///mergesort(0,n-1);
    ///shellsort(v,n);
    countsort(v,n);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);

    cout<<"Timp de executie pentru test"<<" este "<<duration.count() << endl;

    /*
    srand(time(0)); // initialize random seed

    int i,n,nrteste,nmaxi;
    f>>nrteste;
    int nrr=0;
    while(nrr<5*nrteste)
    {
    f>>n;
    if(n>1000000000)
    {cout<<"Nu putem testa algoritmii, sunt prea multe numere de input";
    break;}
    f>>nmaxi;


    for (i = 0; i < n; i++)
        vect[i] = rand() % nmaxi;
        v[i]=vect[i];

    auto start = high_resolution_clock::now();

    radixsort(v,n);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);

    if(check(v,n)==1)
        cout<<"Timp de executie pentru testul"<<" de la radixsort este "<<duration.count() << endl;

    else
        cout<<"Vectorul nu a fost sortat corect."<<endl;

    nrr++;

///------------------------------------------------------------------------
    for (i = 0; i < n; i++)
        v[i]=vect[i];
    if(egale(v,n)==1)
        cout<<"Quicksort nu este deloc eficient pentru acest input.";
    else
    {
        start = high_resolution_clock::now();
    quicksort(0,n);
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);

    if(check(v,n)==1)
        cout<<"Timp de executie pentru testul"<<" de la quicksort este "<<duration.count() << endl;
    else
        cout<<"Vectorul nu a fost sortat corect."<<endl;
    }

    nrr++;

///------------------------------------------------------------------------
    for (i = 0; i < n; i++)
        v[i]=vect[i];
    
    start = high_resolution_clock::now();
    countsort(v,n);
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);

    if(check(v,n)==1)
        cout<<"Timp de executie pentru testul"<<" de la countsort este "<<duration.count() << endl;
    else
        cout<<"Vectorul nu a fost sortat corect."<<endl;

    nrr++;

///------------------------------------------------------------------------
    for (i = 0; i < n; i++)
        v[i]=vect[i];

    start = high_resolution_clock::now();
    shellsort(v,n);
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);

    if(check(v,n)==1)
        cout<<"Timp de executie pentru testul"<<" de la shellsort este "<<duration.count() << endl;
    else
        cout<<"Vectorul nu a fost sortat corect."<<endl;

    nrr++;

///------------------------------------------------------------------------
    for (i = 0; i < n; i++)
        v[i]=vect[i];

    start = high_resolution_clock::now();
    mergesort(0,n-1);
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);

    if(check(v,n)==1)
        cout<<"Timp de executie pentru testul"<<" de la mergesort este "<<duration.count() << endl;

    else
        cout<<"Vectorul nu a fost sortat corect."<<endl;

    nrr++;

    }
    */
    f.close();
    f2.close();
    return 0;
}