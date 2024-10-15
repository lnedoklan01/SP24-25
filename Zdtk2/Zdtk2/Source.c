#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define PROGRAM_SUCCESS (0)
#define EXIT_PROGRAM (-1)
#define FILE_DIDNT_OPEN (-1)
#define MAX_SIZE (50)
#define MAX_LINE (1024)
#define MAX_POINTS (50)

typedef struct _person position;
typedef struct _person {
	char name[32];
	char surname[32];
	int birth_year;
	position next;
}person;
position CreatePerson(position head, char* name, char* surname, int birth_year);
int preprend_list(position head, char* name, char* surname, int birth_year);
int print_list(position first);
int append_list(position head, char* name, char* surname, int birth_year);
position find_by_surname(position first, char* surname);




int main() {
	person head = { .name = "", .surname = "",.birth_year = 0, .next = NULL };

	return 0;
}


position CreatePerson(position head, char* name, char* surname, int birth_year) {
	position newPerson = NULL;
	newPerson = (position*)malloc(sizeof(person));
	if (!newPerson) {
		printf("Malloc failed\n");
		return NULL;
	}
	strcpy(newPerson->name = name);
	strcpy(newPerson->surname = surname);
	newPerson.birth_year = birth_year;
	newPerson.next = NULL;
}
int preprend_list(position head,char* name, char* surname, int birth_year) {
	position new_person = NULL;

	new_person = CreatePerson(name, surname, birth_year);
	if (!new_person) {
		return -1;
	}
	new_person = head->next;
	head->next = new_person;
	return EXIT_SUCCESS;
}
int print_list(position first) {
	position temp = NULL;

	while (temp) {
		printf("First name: %s\nLast name: %s\nBirth year: %d", temp->name, temp->surname,temp.birth_year);
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}
int append_list(position head,char* name, char* surname, int birth_year) {
	position new_person = NULL;

	new_person = CreatePerson(name, surname, birth_year);
	if (!new_person) {
		return -1;
	}
	new_person->next
}
position find_last(position head) {
	position temp = head;

	while (temp->next) {
		temp = temp->next;
	}
	return temp;
}
int insert_after(position previous, position to_insert) {

}
position find_by_surname(position first, char* surname) {
	position temp = first;
	while (temp) {
		if (strcmp(temp->surname, surname)==0) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}