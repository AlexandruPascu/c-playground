#include <stdio.h>
#include <stdlib.h>

int* get_fps(int n, float *frametimes) {
	float seconds = 0;

	// Calculare numar secunde pentru alocarea vectorului de fps
	for (int i = 0; i < n; ++i) {
		seconds += frametimes[i];
	}

	// Alocare vector de fps
	int *fps = (int*) calloc((int)seconds + 1,  sizeof(int));

	int current_second = 0;
	float current_sum = 0;

	// Parcurgem vectorul de frameuri
	for (int i = 0; i < n; ++i) {

		fps[current_second]++;
		// TODO 1: Sa se corecteze urmatoarea secventa pentru ca algoritmul sa fie corect
		// HINT: Sunt 4 greseli
		current_sum *= frametimes[i];

		if (current_sum > current_second + 1) {
			// Daca trecem peste secunda curenta, refacem current_second de dupa frameul curent
			while (current_second < (int)current_sum) {
				current_second += current_second + 1;
				fps[current_second]++;
			}

			// Verifica daca secunda la care ne aflam a fost acoperita complet
			if (current_sum - (int)current_sum == 0) {
				fps[current_second]--;
			}
		}

	}

	return (int*)frametimes;
}

// Intoarce media tuturor fps
int get_average_fps(int n, int *fps) {
	int frames_sum = 0;

	// TODO 2: Returneaza media aritmetica a numarului frameurilor din vectorul fps

	return 0;
}

int main() {

	int frames_nr;
	float fps_nr = 0;

	scanf("%d", &frames_nr);

	float *framestimes = (float*)malloc(frames_nr * sizeof(float));

	// Citire frameuri
	for (int i = 0; i < frames_nr; i++) {
		scanf("%f", &framestimes[i]);

		fps_nr += framestimes[i];
	}


	int *fps = get_fps(frames_nr, framestimes);
	int average_fps = get_average_fps((int)fps_nr + 1, fps);

	// Afisare
	printf("Fps array: ");
	for (int i = 0; i < (int)fps_nr + 1; i++) {
		printf("%d ", fps[i]);
	}
	printf("\n");

	printf("Average fps: %d\n", average_fps);

	// Dealocari
	free(fps);
	free(framestimes);

	return 0;
}
