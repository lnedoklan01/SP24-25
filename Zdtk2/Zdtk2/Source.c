#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS (0)
#define EXIT_PROGRAM (-1)

#define MAX_LENGTH (50)


struct _Person;

//Position se koristi za pohranu adrese strukture _Person
typedef struct _Person* Position;

//deklaracija strukture Person
typedef struct _Person
{
	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	int birthYear;
	Position next;
} Person;

//funkcija appendList dodaje novu osobu na pocetak vezane liste
int addToList(Position head);

//funkcija addToEndOfList dodaje novu osobu na kraj vezane liste
int addToEndOfList(Position head);

//funckija printList ispisuje sve osobe u novonastaloj vezanoj listi
void printList(Position first);

//funckija findPerson pretrazuje osobu po prezimenu u vezanoj listi
int findPerson(Position head);

//funkcija deletePerson brise osobu iz vezane liste,
//tako sto upisujemo prezime osobe koju zelimo izbrisati
int deletePerson(Position head);

//funckija cratePerson inicijalizira novu strukturu Person i sva njezina polja
Position createPerson();

//
Position findLast(Position head);

// funkcija printPerson ispisuje podatke o zeljenoj osobi
//podaci koje ispisuje su: ime, prezime i adresa
int printPerson(Position person);

//funckija menu pruza korisniku izbornik iz kojeg moze birati sto zeli napraviti sa novonastalom listom
int menu(Position head);

//funkcija wishedSurname uzima prezime koje je korisnik upisao i koristi ga u funkcijama koje brisu ili pronalaze osobu iz vezane liste
char* wishedSurname();

void sort(Position headptr);

//funkcija main za argumente uzima argc(argument count) i argv(argument vector)
//argc predstavlja broj stvari koje smo unjeli u terminalu (u ovom slucaju to su osobe koje tvore vezanu listu)
//argv je niz koji sprema stvari koje smo unjeli kao string
int main(int argc, char** argv)
{
	//inicijaliziranje "dummy" elementa koji pokazuje na pocetak vezane liste 
	Person Head = {
		.next = NULL,
		.name = {0},
		.surname = {0},
		.birthYear = 0
	};

	//funkciji menu prosljedjujemo adresu Head-a
	menu(&Head);

	return EXIT_SUCCESS;
}

//Position nam predstavlja referencu na objekt tipa struct _Person
//to znaci kada pozovemo tu fukciju ona ce vratiti referencu na na novo kreiranu osobu
Position createPerson()
{
	Position newPerson = NULL;
	char name[MAX_LENGTH] = { 0 };
	char surname[MAX_LENGTH] = { 0 };
	int birthYear = 0;

	//dinamicka alokacija memorije za novu osobu
	newPerson = (Position)malloc(sizeof(Person));
	if (!newPerson)
	{
		//perror je naredba koja ispisuje poruku u stderr (standar error stream), cija je namjena da ispise error korisniku
		//printf ne bi koristili u ovom slucaju jer se on koristi za opcu izlaznu poruku korisniku
		//razlozi zbog kojeg koristimo u ovom slucaju perror, a ne printf je taj sto perror odma ispisuje pogresku korisniku,
		//a i nama kao programerima sto nam je korisno kod debuggiranja koda
		perror("Failed to allocate memory!\n");
		return NULL;
	}

	printf("Enter name:\n");
	scanf(" %s", name);
	printf("Enter surname:\n");
	scanf(" %s", surname);
	printf("Enter birth year:\n");
	scanf(" %d", &birthYear);

	//strcpy funkcija kopira sadrzaj jednog stringa u drugi string
	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;

	return newPerson;
}

int addToList(Position head)
{
	Position newPerson = NULL;

	newPerson = createPerson();

	//ako pokazivac newPerson != NULL stvara se novi cvor u vezanoj listi koji poprima vrijednost Head
	//cvor kojeg je prije toga okupirao Head sada postaje cvor koji zauzima newPerson 
	if (newPerson)
	{
		newPerson->next = head->next;
		head->next = newPerson;
	}

	return EXIT_SUCCESS;
}

//funkcija findLast prolazi po vezanoj listi dok ne dodje do cvora koji je jednak NULL
//kada naidje na takav cvor zna da je dosao do kraja liste
//to mozemo iskroisti kako bi identificirali zadnji element vezane liste
Position findLast(Position head)
{
	while (head->next != NULL)
	{
		head = head->next;
	}

	return head;
}


int addToEndOfList(Position head)
{
	Position newPerson = NULL;

	newPerson = createPerson();

	//head cvora postavljamo na zadnjeg clana vezane liste pomocu funkcije findLast
	//za postavljanje nove osobe kao head cvora koristimo isti postupak kao i kod addToList funckije
	if (newPerson)
	{
		head = findLast(head);
		newPerson->next = head->next;
		head->next = newPerson;
	}

	return EXIT_SUCCESS;
}

//funkcija koja ispisuje elemente vezane liste ukoliko ona postoji
void printList(Position first) {
	if (first == NULL) {
		printf("Empty list!\n");
		return;
	}

	while (first != NULL) {
		printf("Name: %s\t Surname: %s\t Birth year: %d\t\n", first->name, first->surname, first->birthYear);
		first = first->next;
	}
}

int findPerson(Position head)
{
	if (head->next)
	{
		//strcmp(string compare) usporedjuje postojece prezime sa onim koje je korisnik upisao
		//ovisno o tocnosti program izvrsava zadane naredbe
		while (head->next && strcmp(head->next->surname, wishedSurname()) != 0)
		{
			head = head->next;
		}
		if (head->next)
		{
			printPerson(head->next);
		}
		else
		{
			perror("Can't find person with that surname!\n");
			return EXIT_PROGRAM;
		}
	}
	else
	{
		perror("Empty list!\n");
	}

	return EXIT_SUCCESS;
}
int deletePerson(Position head)
{
	Position previous = NULL;
	char* target_surname = wishedSurname();
	while (head && strcmp(head->surname, target_surname) != 0)
	{
		previous = head;
		head = head->next;
	}
	if (head)
	{
		printPerson(head);
		if (previous != NULL)
		{
			previous->next = head->next;
		}
		free(head);
		printf("Person deleted!\n");
		return EXIT_SUCCESS;
	}
	else
	{
		perror("Can't find person with that surname!\n");
		return EXIT_FAILURE;
	}
}

int printPerson(Position person)
{
	printf("Name: %s, surname: %s, birth year: %d\n",person->name, person->surname, person->birthYear);

	return EXIT_SUCCESS;
}

int menu(Position head) {
	int choice = 0;

	while (1) {
		printf("Choose an option:\n");
		printf("1. Add new person to list\n");
		printf("2. Add new person to end of list\n");
		printf("3. Print list\n");
		printf("4. Search for person\n");
		printf("5. Delete person\n");
		printf("6. Exit\n");

		printf("Your choice is: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			addToList(head);
			break;
		case 2:
			addToEndOfList(head);
			break;
		case 3:
			printList(head->next);
			break;
		case 4:
			findPerson(head);
			break;
		case 5:
			deletePerson(head);
			break;
		case 6:
			return EXIT_SUCCESS;
		default:
			printf("Invalid choice!\n");
		}
	}
	return EXIT_SUCCESS;
}

char* wishedSurname()
{
	char surname[MAX_LENGTH] = { 0 };
	printf("Enter surname of the wanted person:\n");
	scanf(" %s", surname);

	return surname;
}
void sort(Position headptr) {
	Position current = headptr;

	if (!current->next || !current->next->next) {
		return;
	}
	while (1) {
		current = headptr;
		while (current->next->next) {
			if (strcmp(current->next->surname, current->next->next->surname) > 0) {
				Position first = current->next;
				Position second = first->next;
				first->next = second->next;
				second->next = first;
				current->next = second;
			}
			current = current->next;
		}
	}
}