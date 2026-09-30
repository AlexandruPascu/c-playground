#include <stdio.h>
#include <string.h>

typedef struct {
	char name[50];
	int age;
	float avg;
} Student;

int main(void) {
	Student s1, s2;

	strcpy(s1.name, "Alex");
	s1.age = 21;
	s1.avg = 8.88f;

	strcpy(s2.name, "Paul");
	s2.age = 24;
	s2.avg = 9.12f;

	printf("Studentul %s, in varsta de %d ani, are media %.2f.\n", s1.name, s1.age, s1.avg);
	printf("Studentul %s, in varsta de %d ani, are media %.2f.\n", s2.name, s2.age, s2.avg);
	return 0;
}
