#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

#define MAX_LINE (1024)
#define MAX_SIZE (50)

#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)
#define EMPTY_LISTS (-2)

//inicijaliziranje strukture element koja ce u sebi kao elemente strukture imat elemente polinoma sa kojima kasnije manipuliramo
struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
	int coefficient;
	int exponent;
	Position next;
} Element;

//funkcija readFile cita datoteku polynomes.txt i poziva funckiju parseStringIntoList koja stringove iz datoteke pretvara u vezane liste
int readFile(Position headPolynome1, Position headPolynome2, char* fileName);

//funckija printPolynome ispisuje sta???  
int printPolynome(char* polynomeName, Position first);

//fukcija addPolynomes zbraja polinome po pravilu o zbrajanju polinoma
int addPolynomes(Position resultHead, Position firstElementPolynome1, Position firstElementPolynome2);

//fukcija multiplyPolynomes mnozi polinome po pravilu o mnozenju polinoma
int multiplyPolynomes(Position resultHead, Position headPoly1, Position headPoly2);

//funckija freeMemory oslobadja iskoristenu memoriju koju su zauzeli inicijalizirani head-ovi vezanih lista
int freeMemory(Position head);

//funckija changeStringIntoList pretvara procitane strinogove iz datoteke u liste
int changeStringIntoList(Position head, char* buffer);

//funkcija createElement inicijalizira novu strukturu i dinamicki alocira potrebnu memoriju
Position createNewElement(int coefficient, int exponent);

//funkcija insertSorted provjerava da li su polinomi sortirani po pravilu o sortiranju polinoma pa ih ubacuje u funkciju multiplyPolynome
int insertSorted(Position head, Position newElement);

//funckija mergeAfter osigurava da polinomi u listi budu slozeni pravilnim redosljedom(od polinoma sa najmanjom potencijom do polinoma sa najvecom potencijom)
//tj., funckija provjerava da li sljedeci polinom u listi moze nalaziti na toj poziciji,
// te ako ne moze zadrzava ga da polinom koji treba doci na tu poziciju dodje na tu poziciju
int mergeAfter(Position current, Position newElement);

//funckija insertAfter ubacuje novi element nakon nekog elementa
int insertAfter(Position current, Position newElement);

//funckija deleteAfter brise sljedeci element u vezanoj listi
int deleteAfter(Position previous);

//funkcija crateAndInsertAfter kreira novi element u vezanoj listi i ubacuje ga na sljedece mjesto u listi
int createAndInsertAfter(int coefficient, int exponent, Position current);


int main()
{
	//inicijaliziramo vise head-ova kako bi kreirali nove nezavisne liste, sto nam omogucuje sljedece:
	//1. paralelno izvrsavanje - svaki head radi sam za sebe sto ubrzava određene operacije poput pretrage, sortiranja ili filtriranja
	//2. optimizacija podataka - optimalnije koristenje procesorske snage kod manipuliranja liste (izvodjenja operacija nad listom)
	//3. bolja organizacija i upravljanje podacima - mozemo organizirati podatke na razlicite nacine zbog broja head-ova
		//npr. možete imati headove koji sortiraju podatke, dok drugi vrše pretragu ili izračunavaju umnozak polinoma kao u nasem slucaju
	Element headPolynome1 = {
		.coefficient = 0,
		.exponent = 0,
		.next = NULL
	};

	Element headPolynome2 = {
		.coefficient = 0,
		.exponent = 0,
		.next = NULL
	};

	Element headPolynomeAdd = {
		.coefficient = 0,
		.exponent = 0,
		.next = NULL
	};

	Element headPolynomeMultiplication = {
		.coefficient = 0,
		.exponent = 0,
		.next = NULL
	};

	//ova sintaksa se moze zapisati na sljedeci nacin --> char filename[] = "polynomes.txt";
	char* fileName = "polynomes.txt";

	//ispis iscitanih stringova(polinoma) iz datoteke i funkcija koje smo proveli nad tim stringovima(polinomima)
	if (readFile(&headPolynome1, &headPolynome2, fileName) == EXIT_SUCCESS) {
		printPolynome("First polynome: ", headPolynome1.next);
		printPolynome("Second polynome: ", headPolynome2.next);

		addPolynomes(&headPolynomeAdd, headPolynome1.next, headPolynome2.next);
		multiplyPolynomes(&headPolynomeMultiplication, headPolynome1.next, headPolynome2.next);

		printPolynome("Added polynomes: ", headPolynomeAdd.next);
		printPolynome("Multiplied polynomes: ", headPolynomeMultiplication.next);

		freeMemory(&headPolynome1);
		freeMemory(&headPolynome2);
		freeMemory(&headPolynomeAdd);
		freeMemory(&headPolynomeMultiplication);

	}



	return EXIT_SUCCESS;
}

int readFile(Position headPolynome1, Position headPolynome2, char* fileName) {
	FILE* filePointer = NULL;
	char buffer[MAX_LINE] = { 0 };
	int status = EXIT_SUCCESS;

	filePointer = fopen(fileName, "r");
	if (!filePointer) {
		printf("Can't open file!\n");
		return FILE_NOT_OPEN;
	}

	fgets(buffer, MAX_LINE, filePointer);
	status = changeStringIntoList(headPolynome1, buffer);
	if (status != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	fgets(buffer, MAX_LINE, filePointer);
	status = changeStringIntoList(headPolynome2, buffer);
	if (status != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

int printPolynome(char* polynomeName, Position first) {
	printf(" %s = ", polynomeName);
	if (first) {
		if (first->exponent < 0) {
			if (first->coefficient == 1) {
				printf("x^(%d)", first->exponent);
			}
			else {
				printf("%dx^(%d)", first->coefficient, first->exponent);
			}
		}
		else {
			if (first->coefficient == 1) {
				printf("x^%d", first->exponent);
			}
			else {
				printf("%dx^%d", first->coefficient, first->exponent);
			}
		}

		first = first->next;
	}

	for (; first != NULL; first = first->next) {
		if (first->coefficient < 0) {
			if (first->exponent < 0) {
				printf(" - %dx^(%d)", abs(first->coefficient), first->exponent);
			}
			else {
				printf(" - %dx^%d", abs(first->coefficient), first->exponent);
			}
		}
		else {
			if (first->exponent < 0) {
				if (first->coefficient == 1) {
					printf(" + x^(%d)", first->exponent);
				}
				else {
					printf(" + %dx^(%d)", first->coefficient, first->exponent);
				}
			}
			else {
				if (first->coefficient == 1) {
					printf(" + x^%d", first->exponent);
				}
				else {
					printf(" + %dx^%d", first->coefficient, first->exponent);
				}
			}
		}
	}

	printf("\n");
	return EXIT_SUCCESS;
}

int addPolynomes(Position resultHead, Position firstElementPoly1, Position firstElementPoly2) {
	Position currentPoly1 = firstElementPoly1;
	Position currentPoly2 = firstElementPoly2;
	Position currentResult = resultHead;
	Position remainingPoly = NULL;

	while (currentPoly1 != NULL && currentPoly2 != NULL) {
		if (currentPoly1->exponent == currentPoly2->exponent) {
			createAndInsertAfter(currentPoly1->coefficient + currentPoly2->coefficient, currentPoly1->exponent, currentResult);
			currentPoly1 = currentPoly1->next;
			currentPoly2 = currentPoly2->next;
			currentResult = currentResult->next;
		}
		else if (currentPoly1->exponent < currentPoly2->exponent) {
			createAndInsertAfter(currentPoly1->coefficient, currentPoly1->exponent, currentResult);
			currentPoly1 = currentPoly1->next;
			currentResult = currentResult->next;
		}
		else {
			createAndInsertAfter(currentPoly2->coefficient, currentPoly2->exponent, currentResult);
			currentPoly2 = currentPoly2->next;
			currentResult = currentResult->next;
		}

	}
	if (currentPoly1 == NULL) {
		remainingPoly = currentPoly2;
	}
	else {
		remainingPoly = currentPoly1;
	}

	while (remainingPoly != NULL) {
		createAndInsertAfter(remainingPoly->coefficient, remainingPoly->exponent, currentResult);
		remainingPoly = remainingPoly->next;
		currentResult = currentResult->next;
	}

	return EXIT_SUCCESS;
}

int multiplyPolynomes(Position resultHead, Position firstElementPoly1, Position firstElementPoly2) {

	if (firstElementPoly1 == NULL || firstElementPoly2 == NULL)
		return EMPTY_LISTS;
	for (Position currentPoly1 = firstElementPoly1; currentPoly1 != NULL; currentPoly1 = currentPoly1->next) {
		for (Position currentPoly2 = firstElementPoly2; currentPoly2 != NULL; currentPoly2 = currentPoly2->next) {
			Position newElement = createNewElement(currentPoly1->coefficient * currentPoly2->coefficient, currentPoly1->exponent + currentPoly2->exponent);
			if (!newElement) {
				return EXIT_FAILURE;
			}

			insertSorted(resultHead, newElement);
		}
	}
	return EXIT_SUCCESS;

}

int freeMemory(Position head)
{
	Position current = head;

	while (current->next) {
		deleteAfter(current);
	}

	return EXIT_SUCCESS;
}

int changeStringIntoList(Position head, char* buffer)
{
	char* currentBuffer = buffer;
	int coefficient = 0;
	int exponent = 0;
	int numBytes = 0;
	int status = 0;
	Position newElement = NULL;

	while (strlen(currentBuffer) > 0) {
		status = sscanf(currentBuffer, " %dx^%d %n", &coefficient, &exponent, &numBytes);
		if (status != 2) {
			printf("This file is not good!\n");
			return EXIT_FAILURE;
		}

		newElement = createNewElement(coefficient, exponent);
		if (!newElement) {
			return EXIT_FAILURE;
		}

		insertSorted(head, newElement);
		currentBuffer += numBytes;
	}

	return EXIT_SUCCESS;
}

Position createNewElement(int coefficient, int exponent) {
	Position element = NULL;

	element = (Position)malloc(sizeof(Element));
	if (!element) {
		printf("Can't allocate memory!\n");
		return FAILED_MEMORY_ALLOCATION;
	}

	element->coefficient = coefficient;
	element->exponent = exponent;
	element->next = NULL;

	return element;
}

int insertSorted(Position head, Position newElement) {
	Position current = head;

	while (current->next != NULL && current->next->exponent > newElement->exponent) {
		current = current->next;
	}

	mergeAfter(current, newElement);

	return EXIT_SUCCESS;
}

int mergeAfter(Position current, Position newElement) {

	if (current->next == NULL || current->next->exponent != newElement->exponent) {
		insertAfter(current, newElement);
	}
	else
	{
		int resultCoefficient = current->next->coefficient + newElement->coefficient;
		if (resultCoefficient == 0)
		{
			deleteAfter(current);
		}
		else
		{
			current->next->coefficient = resultCoefficient;
		}
		free(newElement);
	}

	return EXIT_SUCCESS;
}

int insertAfter(Position current, Position newElement) {
	newElement->next = current->next;
	current->next = newElement;

	return EXIT_SUCCESS;
}

int deleteAfter(Position previous) {
	Position toDelete = NULL;

	toDelete = previous->next;
	previous->next = toDelete->next;
	free(toDelete);

	return EXIT_SUCCESS;
}

int createAndInsertAfter(int coefficient, int exponent, Position current) {
	Position newElement = createNewElement(coefficient, exponent);
	if (!newElement) {
		return EXIT_FAILURE;
	}

	insertAfter(current, newElement);

	return EXIT_SUCCESS;
}