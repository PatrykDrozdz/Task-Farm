#include<mpi.h>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<sstream>

#define N 10

using namespace std;

////////////////////////////////////////////
bool Randed(int number, double tab[], int count) {
	
	int i = 0;

	if (count <= 0) {
		return false;
	}
	else {
		while (i < count) {
			if (tab[i] == number) {
				return true;
			}
			i++;
		}
		return false;
	}
}
//////////////////////////////////////////////////
int main(int argc, char* argv[]) {

	MPI::Init();
	int rank, size, cycles, rest;
	int tag = 0;
	int proc = 1;
	int Tmax = 100;//maksymalny rozmiar zadania
	int s;//ziaron psudolosowoœci
	int randMin = 1;//dolna granica rand
	int randMax = Tmax;//górna granica rand
	rank = MPI::COMM_WORLD.Get_rank();
	size = MPI::COMM_WORLD.Get_size();
	int randNumber = 0;
	int randed = 0;
	stringstream zs;
	//MPI_Status status;

	zs << argv[0];

	zs >> s;


	//dekompozycja
	///////////////////////////////////////
	cycles = N / (size - 1);//cykle do wykonania(-1, bo bez procesu 0-master)
	rest = N%(size - 1);
	/////////////////////////////////////

	//tablice
	/////////////////////////////////////////
	double* Rands = new double[N];//wylosowane liczby
	double* Ttab = new double[N];//odebrane
	int* Tags = new int[N];

	////////////////////////////////////////

	//nadzorca
	if (rank == 0) {

		srand(s);

		while (randed < N) {
			randNumber = (rand() % randMax) + randMin;
			if (Randed(randNumber, Rands, randed) == false) {
				Rands[randed] = randNumber;
				randed++;
			}

		}

		cout << "Wylosowane liczby" << endl;
		for (int i = 0; i < N; i++) {
			cout << Rands[i] << "  ";
		}

	} 

	//rozdzielanie wysy³anie
	//for (int i = 0; i < N; i++) {
	//	MPI_Send(&Rands[i], 1, MPI_DOUBLE, proc, tag, MPI_COMM_WORLD);
	//	Tags[i] = proc;
	//	proc++;
	//	tag++;
	//	if (proc == size) {
	//		proc = 0;
	//	}
	//}
	//tag = 0;
	//proc = 0;

	////odbieranie
	//for (int i = 0; i < N; i++) {
	//	MPI_Recv(&Ttab[i], 1, MPI_DOUBLE, 0,tag, MPI_COMM_WORLD, &status);
	//	tag++;
	//}

	cout << endl;
	cout << endl;

	MPI::Finalize();

	system("pause");
	return 0;
}