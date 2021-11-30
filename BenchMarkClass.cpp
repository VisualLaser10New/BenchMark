//LIBS
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>


//NAMESPACES
using namespace std;


//MACRO
#ifdef _WIN32
	#define CLS "cls"
	#define PAUSE "pause"
#elif defined (__linux__)
	#define CLS "clear"
	#define PAUSE "read -n 1 -s -p \"Premi un tasto per continuare...\""
#endif

void pc() //pc non intende "partitito comunista", ma "pause cls"
{
	system(PAUSE);
	system(CLS);
}

void clearCin()
{
	cin.clear();
	cin.ignore(80, '\n');
}



namespace BMN
{
	struct execTime
	{
		double executionTime;
		int summ;
	};

	class BenchMark
	{
		private:
			int NFILE;
			vector<vector<int>> arr; //matrice che contiene tutti gli array

			void analizeBuffer(fstream &file, vector<int> &ar)
			{
				string buff;
				char delimiter;
				//Buffer analyze
				while(getline(file, buff))
				{
					stringstream ss(buff);
					string buf1;
					
					//Check delimiter
					if(buff.find(';') != -1)
					{
						delimiter = ';';
					}
					else if(buff.find(',') != -1)
					{
						delimiter = ',';
					}
					else
					{
						cout << "Formato file non valido\n";
						pc();
						exit(EXIT_FAILURE);
					}
					
					
					while(getline(ss, buf1, delimiter))
					{
						//cout << buf1<<endl;
						ar.push_back(stoi(buf1));
					}
				}
			}	
			void readFiles(string name, string extension)
			{
				for(int i = 1; i <= NFILE; ++i)
				{
					//Path gen
					string filetoRead = name + to_string(i) + extension; //composto da nome_file + numero_file + estensione
					
					//lettura del file
					fstream file(filetoRead, fstream::in); //apro il file
					string buff;
					vector<int> ar; //array temporaneo
					
					
					if(!file.good()) //File check
					{
						cout << "Errore nell'apertura del file" << endl;
						pc();
						exit(EXIT_FAILURE);
					}
					
					analizeBuffer(file, ar); //Analizzo il file
					file.close();

					arr.push_back(ar); //Aggiungo l'array alla matrice
				}
			}

			bool fileExist(string name)
			{
				ifstream file(name);
				if(file.good())
				{
					file.close();
					return true;
				}
				else
				{
					file.close();
					return false;
				}
			}

		public:
			BenchMark(string name, string extension, int nfile)
			{
				NFILE = nfile;
				readFiles(name, extension);
			}

			vector<execTime> calc(execTime (*maxsumFunc)(int * ar, int n))
			{
				//viene passato l'algoritmo per calcolare la somma massima
				//per ogni array, pertanto per ogni file dato che un file contiene un array, calcolo la somma massima 
				vector<execTime> execTimeVec; //vettore che contiene tutti i tempi di esecuzione
				
				for(int i = 0; i < NFILE; i++)
				{
					//mette nell'array execTimeVec i tempi di esecuzione dell'algoritmo per tutti gli array
					execTimeVec.push_back(maxsumFunc(arr[i].data(), arr[i].size())); 
				}
				//writeFile(execTimeVec, algName); //scrive su file i tempi di esecuzione

				return execTimeVec;
			}

			bool writeFile(vector<execTime> input, string algname, string filetoSave)
			{
				/*
				* template di scrittura del file
				+------+--------+--------+--------+--------+
				| Algo |  Arr1  |  arr2  |  Arr3  |  ArrN  |
				+------+--------+--------+--------+--------+
				| S1   | TimAr1 | TimAr2 | TimAr3 | TimArN |
				| S2   | TimAr1 | TimAr2 | TimAr3 | TimArN |
				| SX   | TimAr1 | TimAr2 | TimAr3 | TimArN |
				+------+--------+--------+--------+--------+

				Per ogni riga vi è un algoritmo e per ogni colonna i tempi che l'algoritmo ha impiegato per computare l'array
				*/
				char delimiter = ';';

				if(!fileExist(filetoSave+".csv")) //scrive nel file i titoli
				{
					fstream file(filetoSave+".csv", fstream::out);
					file << "Algo" << delimiter;

					for(int i = 0; i< input.size(); i++)
					{
						file << "Arr" << i+1 << delimiter;
					}
					file << endl;

					file.close();
				}


				fstream file(filetoSave+".csv", fstream::app); //scrivo nei file i tempi

				if(!file.good())
				{
					return false;
				}
				file << algname << delimiter;
				for(execTime time : input)
				{
					file << time.executionTime << delimiter;
				}
				file << endl;
				file.close();
				return true;
			}
	};

}
