#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// zakomentiras sa ctrl + K + C
//odkomentiras sa ctrl + K + U

//digresija profesora: "Steknite naviku definirati return-ove jer ce te u vecim programima imati puno koda u kojem morate znati sto vracate korisniku"
#define PROGRAM_SUCCESS (0)
#define EXIT_PROGRAM (-1)
#define FILE_DIDNT_OPEN (-1)


#define MAX_SIZE (50)
#define MAX_LINE (1024)
#define MAX_POINTS (50)

//inicijaliziranje strukture podataka _student
typedef struct _student {
    char name[MAX_SIZE];
    char surname[MAX_SIZE];
    double points;
} Student;

//funckija koja broji redove u tekstualnoj datoteci
//broj redova odgovara broju studenata u toj datoteci
int rowCounter(char* fileName);

//funckija koja cita podatke o studentima iz tekstualne datoteke
void readStudents(Student* stud, int noStud, char* fileName);

//funckija koja ispisuje studente sa njihovim ostvarenim apsolutnim i relativnim brojem bodova
void printStudents(Student* stud, int noStud);

//dinamicka alokacija memorije za studente
Student* allocateMemory(int noRows);

int main() {

    Student* stud = NULL;

    //stvaramo pokazivac na string "students.txt" u memoriji
    char* filePointer = "studenti.txt";
    int noStudents = rowCounter(filePointer);


    //ako u datoteci nema studenata program se nece izvrsiti
    if (noStudents < 0) {
        return EXIT_PROGRAM;
    }

    if (noStudents > 0)
    {
        stud = allocateMemory(noStudents);
        readStudents(stud, noStudents, filePointer);
        printStudents(stud, noStudents);

        //oslobadja prethodno dinamicki alociranu memoriju
        free(stud);
    }

    return PROGRAM_SUCCESS;
}

//no = number of
//inicializiraj sve varijable koje napises u kodu
//funckija koja sluzi za brojanje redova u file-u "students.txt"
int rowCounter(char* fileName) {
    int noRows = 0;

    //char buffer pohranjuje niz char-ova
    //u ovom slucaju on moze maksimalno primit 1024 char-a jer je to maksimalni broj znakova u jednom redu
    //inicijaliziraj svaku varijablu
    char buffer[MAX_LINE] = { 0 };

    //ako se file ne nalazi u folderu ka i solution moras napisat cijeli path od txt file-a
    // r - read file,
    // w - write(ako ne nadje file stvori ga, ako ima file izbrise sve i pocne pisat),
    // a - append

    FILE* filePointer = NULL;
    filePointer = fopen(fileName, "r");

    //!filePointer, tj. negacijom filePointera ako je datoteka neuspjesno otvorena izbacujemo poruku koju smo napisali,
    //a ako je uspjesno otvorena datoteka program nastavlja sa radom
    // return 0 = exit success
    // return -1 = exit not successful
    if (!filePointer) {
        perror("Failed to open file!\n");
        return FILE_DIDNT_OPEN;
    }

    while (fgets(buffer, MAX_LINE, filePointer)) {
        noRows++;
    }

    fclose(filePointer);

    printf("There is %d rows in this document (number of rows = number of students).\n", noRows);
    return noRows;
}

//funkcija koja iscitava informacije o studentima iz datotete "students.txt" i pohranjuje ih u strukturu Student
void readStudents(Student* stud, int noStud, char* fileName) {
    int i = 0;
    FILE* filePointer = NULL;
    filePointer = fopen(fileName, "r");

    if (!filePointer) {
        perror("File is not open!");
        return;
    }

    for (i = 0; i < noStud; i++) {
        fscanf(filePointer, "%s %s %lf\n", (stud + i)->name, (stud + i)->surname, &((stud + i)->points));
    }

    fclose(filePointer);
}

//funckija koja ispisuje studente sa izracunatim postotkom(relativni broj bodova)
void printStudents(Student* stud, int noStud) {
    for (int i = 0; i < noStud; i++) {
        double average = ((stud + i)->points / MAX_POINTS) * 100;
        printf("Student %s %s scored %.2lf points, which is equal to %.2lf percentage.\n",
            (stud + i)->name, (stud + i)->surname, (stud + i)->points, average);
    }
}

//dinamicki alocira memoriju
Student* allocateMemory(int noRows)
{
    FILE* filePointer = NULL;
    Student* stud = NULL;
    stud = (Student*)malloc(noRows * sizeof(Student));

    if (!stud) {
        perror("Can't allocate memory!\n");
        return NULL;
    }
}