/**********************************************
 * Author:
 *	VISUAL LASER 10 NEW aka EMA3NTO
 *	CristianCrazyIT aka CRISTIAN
 * UNDER MIT LICENSE
 * Benchmark tools
 **********************************************/

//HEADERS

#include <iostream>
#include <numeric>
#include <string>
#include <functional>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>

#include "BenchMarkClass.cpp"


//MACRO

#define CONV_UNIT nano //Conversione tempo: milli, micro, nano
#define UNIT_STR "nano" //Stringa di "conversione tempo": milli, micro, nano



#ifdef _WIN32
#define CLS "cls"
#define PAUSE "pause"
#elif defined(__linux__)
#define CLS "clear"
#define PAUSE "read -n 1 -s -p \"Premi un tasto per continuare...\""
#endif


//NAMESPACE
using namespace std;
using namespace BMN;

//FUNCTIONS
/****************[UTILITY]**************/

void getFilesNames(string &fileName, string &extension, int &nFile)
{
	cout << "Inserire il nome dei file test: " << endl; //[nome_indice.estensione]
	getline(cin, fileName);
	cout << "Inserire estensione file test: " << endl;
	getline(cin, extension);
	cout << "Inserire il numero di file da leggere" << endl;
	cin >> nFile;
	clearCin();
}

[[deprecated("Replaced by a normal cout")]] void printTime(clock_t t, clock_t tt, string name, int summ)
{
	cout << "Somma: " << summ << " Tempo esecuzione " << name << ": " << setprecision(6) << fixed << (double)(tt - t) / CLOCKS_PER_SEC << " sec" << endl;
}

/****************[FUNZIONI PRINCIPALI]**************/

int somma(int a[], int i, int j)
{
	int somma = 0;
	for (int k = i; k <= j; k++)
	{
		somma += a[k];
	}
	return somma;
}
int maxSum_rec(int ar[], int inizio, int fine)
{

	if (inizio == fine)
		return ar[inizio];

	int meta = (inizio + fine) / 2;
	int maxSX = maxSum_rec(ar, inizio, meta);
	int maxDX = maxSum_rec(ar, meta + 1, fine);
	int maxim = max(maxSX, maxDX);
	int maxSDX = 0, somma = 0;
	for (int dx = meta; dx >= inizio; dx--)
	{
		somma += ar[dx];
		maxSDX = max(maxSDX, somma);
	}

	int maxDSX = 0;
	somma = 0;
	for (int sx = meta + 1; sx <= fine; sx++)
	{
		somma += ar[sx];
		maxDSX = max(maxDSX, somma);
	}
	return max(maxim, maxDSX + maxSDX);
}

execTime maxSum1(int *ar, int n) 
{
	auto t1 = chrono::high_resolution_clock::now();

	// Portion of code to be timed

	int maxSomma = ar[0];
	for (int inizio = 0; inizio < n; inizio++)
	{
		for (int fine = inizio; fine < n; fine++)
		{
			maxSomma = max(maxSomma, somma(ar, inizio, fine));
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	cout << "max s1 " << maxSomma << endl;
	chrono::duration<double, CONV_UNIT> elapsed = t2 - t1;
	return execTime{elapsed.count(), maxSomma};
}

execTime maxSum2(int *ar, int n)
{
	auto t1 = chrono::high_resolution_clock::now();
	// Portion of code to be timed

	int maxSomma = 0;
	for (int inizio = 0; inizio < n; inizio++)
	{
		for (int fine = inizio; fine <= n; fine++)
		{
			maxSomma = max(maxSomma, accumulate(ar + inizio, ar + fine, 0));
		}
	}

	// Record end time
	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, CONV_UNIT> elapsed = t2 - t1;
	cout << "max s2 " << maxSomma << endl;
	return execTime{elapsed.count(), maxSomma};
}

execTime maxSum3(int *ar, int n)
{
	auto t1 = chrono::high_resolution_clock::now();
	// Portion of code to be timed

	int maxSum = 0, sum = 0;
	for (int i = 0; i < n; ++i)
	{
		sum = 0;
		for (int j = i; j < n; ++j)
		{
			sum = sum + ar[j];
			maxSum = max(maxSum, sum);
		}
	}

	// Record end time
	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, CONV_UNIT> elapsed = t2 - t1;
	//printTime(t1, t2, "S3", maxSum);
	cout << "max s3 " << maxSum << endl;
	return execTime{elapsed.count(), maxSum};
}

execTime maxSum4(int *ar, int n)
{
	//Divide-et-impera
	auto t1 = chrono::high_resolution_clock::now();
	// Portion of code to be timed
	int maxSomma = maxSum_rec(ar, 0, n - 1);

	// Record end time
	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, CONV_UNIT> elapsed = t2 - t1;

	cout << "max s4 " << maxSomma << endl;
	//printTime(t1, t2, "S4", maxSomma);
	return execTime{elapsed.count(), maxSomma};
}

execTime maxSum5(int *ar, int n)
{
	//vl10new
	int maxSoFar = ar[0];
	int maxHere = maxSoFar;
	auto t1 = chrono::high_resolution_clock::now();

	// Portion of code to be timed
	int maxSomma = maxSum_rec(ar, 0, n - 1);

	// Record end time
	for (int i = 1; i < n; i++)
	{
		maxHere = maxHere + ar[i];
		maxSoFar = max(maxSoFar, maxHere);
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, CONV_UNIT> elapsed = t2 - t1;
	cout << "max s5 " << maxSomma << endl;
	return execTime{elapsed.count(), maxSomma};
}

[[deprecated("Not working")]]
execTime maxSum6(int *ar, int n)
{
	//il_navy
	int max=0, conta;
	max = ar[0], conta = ar[0];
	auto t1 = chrono::high_resolution_clock::now();

	for (int i = 0; i < n; i++)
	{
		conta += ar[i];
		if (conta > max)
		{
			max = conta;
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, std::micro> elapsed = t2 - t1;

	cout << "max s6 " << max << endl;
	return execTime{elapsed.count(), max};
}

//MAIN FUNCTION
int main()
{
	string fileName, extension, filetoSave;
	int nFile;
	
	/********[OTTENIMENTO NOME FILE]********/
	getFilesNames(fileName, extension, nFile); //ottengo i nome dei file e l'estensione

	BenchMark bm(fileName, extension, nFile); //istanzio la classe BenchMark

	/*******[INFORMAZIONI AGGIUNTIVE UTENTE]*******/
	system(CLS);
	cout << "Visual Laser 10 New\nEMA3NTO, CCRAZY_IT\n" << endl;
	cout << "Time unit: " << UNIT_STR <<"sec." << endl;
	
	
	/********[MENU SELEZIONE]********/
	unsigned char choice;

	do
	{
		cout << "Scegli una delle seguenti opzioni:\n"
			 << "1) Modalit\x85 1\n2) Modalit\x85 2\n3) Modalit\x85 3\n4) Modalit\x85 4\n5) Modalit\x85 5\n"
			 << "a) Tutte le modalit\x85\ne) Chiudi" << endl;
		cin >> choice;
		clearCin();

		if (choice != 'e')
		{
			cout << "Inserire il nome del file di output, senza estensione: ";
			cin >> filetoSave;
			clearCin();
		}
		switch (choice)
		{
		case 'e': //EXIT
			std::exit(EXIT_SUCCESS);
		case 'a': //ALL
		case '1':
			bm.writeFile(bm.calc(maxSum1), "S1", filetoSave);
			if (choice == '1')
				break;
		case '2':
			bm.writeFile(bm.calc(maxSum2), "S2", filetoSave);
			if (choice == '2')
				break;
		case '3':
			bm.writeFile(bm.calc(maxSum3), "S3", filetoSave);
			if (choice == '3')
				break;
		case '4':
			bm.writeFile(bm.calc(maxSum4), "S4", filetoSave);
			if (choice == '4')
				break;
		case '5':
			bm.writeFile(bm.calc(maxSum5), "S5", filetoSave);
			if (choice == '5')
				break;
		default:
			cout << "Scelta non valida" << endl;
			break;
		};
		pc();
	} while (choice != 'e');

	std::exit(EXIT_SUCCESS);
}
