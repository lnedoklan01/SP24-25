#define _CRT_SECURE_NO_WARNINGS

// 5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa rezultat.
// Stog je potrebno realizirati preko vezane liste.

#include<stdio.h>
#include<stdlib.h>

#define MAX_LENGTH (1024)
#define MAX_SIZE (50)

// Inicijalizacija strukture
struct _stackElement;
typedef struct _stackElement* Position;
typedef struct _stackElement {
    double number;
    Position next;
} StackElement;

// funckija calculatePostfixFromFile racuna napisani postfiks iz tekstualne datoteke "postfix.txt"
int calculatePostfixFromFile(Position head, char* fileName, double* result);

// funkcija readFile provjerava da li je moguce ucitati tekstualnu datoteku
// ako se tekstualna datoteka prodje provjeru onda je funckija cita
int readFile(char* fileName, char* buffer);

// funckija parseStringIntoPostfix ucitan string iz tekstualne datoteke pretvara u postfiks
int parseStringIntoPostfix(Position head, char* buffer, double* result);

// funkcija checkStackAndExtractResult provjerava stack i ako postfiks nije zadovoljavajuc govori nam da provjerimo tekstualnu datoteku
// ako je postfiks zadovoljavajuc izvlazi ga iz datoteke i prosljedjuje sljedecoj funkciji
int checkStackAndExtractResult(Position head, double* result);

Position createStackElement(double number);

// funkcija push dodaje novi element na pocetak stacka
int push(Position head, Position newStackElement);

// funckija printStack ispisuje stack
int printStack(Position first);

// fukcija pop izbacuje element sa pocetka stacka te nam vraca njegovu vrijednost
int pop(Position head, double* result);

// operacija popAndPerformOperation izbacuje element sa pocetka stacka dok ne dodje do kraja stacka
// nad svakim elementom provodi jednu od cetiri funkcije koje smo funckiji dodijelili
int popAndPerformOperation(Position head, char operation, double* result);

int main() {
    // inicijaliziranje heada vezane liste
    StackElement head = {
        .number = 0,
        .next = NULL };

    double result = 0;

    //racunanje i ispis postfiksa
    if (calculatePostfixFromFile(&head, "postfix.txt", &result) == EXIT_SUCCESS) {
        printf("Result is: %0.1lf\n", result);
    }

    return EXIT_SUCCESS;
}

int calculatePostfixFromFile(Position head, char* fileName, double* result) {
    char buffer[MAX_LENGTH] = { 0 };
    int status = 0;

    if (readFile(fileName, buffer) != EXIT_SUCCESS || !buffer) {
        return EXIT_FAILURE;
    }

    status = parseStringIntoPostfix(head, buffer, result);
    if (status != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int readFile(char* fileName, char* buffer) {
    FILE* filePointer = NULL;
    int numBytes = 0;

    filePointer = fopen(fileName, "r");
    if (!filePointer) {
        perror("Can't open file!\n");
        return EXIT_FAILURE;
    }

    // fread funkcija sprema sadrzaj tekstualne datoteke u buffer
    numBytes = fread(buffer, sizeof(char), MAX_LENGTH, filePointer);
    if (numBytes <= 0) {
        fclose(filePointer);
        return EXIT_FAILURE;
    }

    fclose(filePointer);

    return EXIT_SUCCESS;
}

int parseStringIntoPostfix(Position head, char* buffer, double* result)
{
    char* currentBuffer = buffer;
    int status = 0;
    int numBytes = 0;
    char operation = 0;
    double number = 0.0;
    Position newStackElement = NULL;

    while (strlen(currentBuffer) > 0) {
        status = sscanf(currentBuffer, " %lf %n", &number, &numBytes);
        if (status != 1) {
            sscanf(currentBuffer, " %c %n", &operation, &numBytes);
            status = popAndPerformOperation(head, operation, result);

            if (status != EXIT_SUCCESS) {
                return EXIT_FAILURE;
            }

            number = *result;
        }

        newStackElement = createStackElement(number);
        if (!newStackElement) {
            return EXIT_FAILURE;
        }

        currentBuffer += numBytes;
        printf("|%s| <-->", currentBuffer);
        push(head, newStackElement);
    }

    return checkStackAndExtractResult(head, result);
}

int checkStackAndExtractResult(Position head, double* result) {
    int status = EXIT_SUCCESS;

    status = pop(head, result);

    if (status != EXIT_SUCCESS) {
        return status;
    }

    if (head->next) {
        // brise sadrzaj sa ekrana konzole
        system("cls");
        printf("Invalid postfix, please check the file!\r\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

Position createStackElement(double number)
{
    Position newStackElement = NULL;

    newStackElement = (Position)malloc(sizeof(StackElement));
    if (!newStackElement) {
        perror("Can't allocate memory!\n");
        return NULL;
    }

    newStackElement->number = number;
    newStackElement->next = NULL;

    return newStackElement;
}

int push(Position head, Position newStackElement) {
    newStackElement->next = head->next;
    head->next = newStackElement;

    printStack(head->next);

    return EXIT_SUCCESS;
}

int printStack(Position first) {
    Position current = first;

    while (current) {
        printf(" %0.1lf", current->number);
        current = current->next;
    }
    printf("\n");

    return EXIT_SUCCESS;
}

int pop(Position head, double* result) {
    Position toDelete = NULL;

    // provjeravamo da li je nakon zadnjeg pop-a ostao element u stack-u
    toDelete = head->next;
    if (!toDelete) {
        printf("Stack is empty! Program has nothing to pop!\n");
        return -1;
    }

    head->next = toDelete->next;
    *result = toDelete->number;
    free(toDelete);

    return EXIT_SUCCESS;
}

int popAndPerformOperation(Position head, char operation, double* result) {
    double operand1 = 0;
    double operand2 = 0;
    int status1 = 0;
    int status2 = 0;

    status1 = pop(head, &operand1);
    if (status1 != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    status2 = pop(head, &operand2);
    if (status2 != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    // ovisno o znaku na koji program naleti, ta operacija ce se izvrsiti nad elementom koji smo pop-ali
    switch (operation) {
    case '+':
        *result = operand2 + operand1;
        break;
    case '-':
        *result = operand2 - operand1;
        break;
    case '*':
        *result = operand2 * operand1;
        break;
    case '/':
        *result = operand2 / operand1;
        break;
    default:
        printf("Operation %c not supported yet!\r\n", operation);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}