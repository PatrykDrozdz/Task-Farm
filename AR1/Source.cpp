#include<mpi.h>
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<Windows.h>

#define N 10
#define s 4 //ziaron psudolosowoœci

using namespace std;

////////////////////////////////////////////
bool Randed(int number, int tab[], int count) {
	
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
//silnia
double fact(int n) {
	double res;
	for (int i = 1; i <=n; i++) {
		res = res*i;
	}
	return res;
}

//suma
double Sum(int a) {
	double sum;
	for (int i = 0; i <= a; i++) {
		sum += i;
	}
	return sum;
}

//////////////////////////////////////////////////
int main(int argc, char* argv[]) {

	MPI::Init();
	int rank, size; 
	int tag = 0;
	int proc = 1;
	int Tmax = 100;//maksymalny rozmiar zadania
	int randMin = 1;//dolna granica rand
	int randMax = Tmax;//górna granica rand
	rank = MPI::COMM_WORLD.Get_rank();
	size = MPI::COMM_WORLD.Get_size();
	int randNumber = 0;
	int randed = 0;
	MPI_Status status;

	//tablice
	/////////////////////////////////////////
	int* Rands = new int[N];//wylosowane liczby
	int* Ttab = new int[N];//odebrane
	int* Tags = new int[N];
	int* Ranks = new int[N];

	double* Relse = new double[N];
	double* RM = new double[N];

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
	for (int i = 0; i < N; i++) {
		MPI_Send(&Rands[i], 1, MPI_INT, proc, tag, MPI_COMM_WORLD);
		Tags[i] = tag;
		Ranks[i] = proc;
		proc++;
		tag++;
		if (proc == size) {
			proc = 0;
		}
	}
	tag = 0;
	proc = 0;

	//odbieranie
	for (int i = 0; i < N; i++) {
		MPI_Recv(&Ttab[i], 1, MPI_INT, 0,tag, MPI_COMM_WORLD, &status);
		tag++;
	}

	if (rank != 0) {

		for (int i = 0; i < N; i++) {
			Relse[i] = (double)fact(Ttab[i]) / (double)Sum(Ttab[i]);
			Sleep(Ttab[i]);
			if (Relse[i] != NULL) {
				MPI_Send(&Relse[i], 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
			}
		}

	}

	cout << endl;
	cout << endl;

	MPI::Finalize();

	system("pause");
	return 0;
}