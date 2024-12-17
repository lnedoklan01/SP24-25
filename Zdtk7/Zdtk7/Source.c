#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH (50)

// Struktura direktorija
struct _directory;
typedef struct _directory* PositionDirectory;
typedef struct _directory {
    char name[MAX_LENGTH];
    PositionDirectory subDirectories;  // Prvi poddirektorij
    PositionDirectory next;            // Sljedeći direktorij na istoj razini
    PositionDirectory parent;          // Roditeljski direktorij
} Directory;

// Funkcije za rad s direktorijima
PositionDirectory createDirectory(char name[MAX_LENGTH], PositionDirectory parent);
PositionDirectory createSubdirectory(char name[MAX_LENGTH], PositionDirectory currentDirectory);
PositionDirectory changeDirectory(char name[MAX_LENGTH], PositionDirectory currentDirectory);
int listDirectoryContents(PositionDirectory currentDirectory);

// Glavna funkcija
int main() {
    // Kreiranje root direktorija
    PositionDirectory rootDirectory = createDirectory("C:", NULL);
    PositionDirectory currentDirectory = rootDirectory;

    while (1) {
        printf("\nMenu:\n");
        printf("1 - md (Create Directory)\n");
        printf("2 - cd dir (Change Directory)\n");
        printf("3 - cd.. (Go Up)\n");
        printf("4 - dir (List Contents)\n");
        printf("5 - exit\n");

        char choice[10];
        printf("Enter your choice: ");
        scanf("%s", choice);

        if (strcmp(choice, "1") == 0) {
            char directoryName[MAX_LENGTH];
            printf("Enter directory name: ");
            scanf("%s", directoryName);
            createSubdirectory(directoryName, currentDirectory);
        }
        else if (strcmp(choice, "2") == 0) {
            char directoryName[MAX_LENGTH];
            printf("Enter directory name: ");
            scanf("%s", directoryName);
            PositionDirectory newDir = changeDirectory(directoryName, currentDirectory);
            if (newDir != currentDirectory) {
                currentDirectory = newDir;
            }
        }
        else if (strcmp(choice, "3") == 0) {
            if (currentDirectory->parent != NULL) {
                currentDirectory = currentDirectory->parent;
                printf("Moved up to '%s'.\n", currentDirectory->name);
            }
            else {
                printf("Already at root directory.\n");
            }
        }
        else if (strcmp(choice, "4") == 0) {
            listDirectoryContents(currentDirectory);
        }
        else if (strcmp(choice, "5") == 0) {
            printf("Exiting the program.\n");
            break;
        }
        else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

// Kreiranje novog direktorija
PositionDirectory createDirectory(char name[MAX_LENGTH], PositionDirectory parent) {
    PositionDirectory newDirectory = (PositionDirectory)malloc(sizeof(Directory));
    if (!newDirectory) {
        printf("Cannot allocate memory for directory.\n");
        return NULL;
    }
    strcpy(newDirectory->name, name);
    newDirectory->subDirectories = NULL;
    newDirectory->next = NULL;
    newDirectory->parent = parent;
    return newDirectory;
}

// Kreiranje poddirektorija
PositionDirectory createSubdirectory(char name[MAX_LENGTH], PositionDirectory currentDirectory) {
    PositionDirectory newDirectory = createDirectory(name, currentDirectory);
    if (!newDirectory) {
        return NULL;
    }
    newDirectory->next = currentDirectory->subDirectories;
    currentDirectory->subDirectories = newDirectory;
    printf("Directory '%s' created under '%s'.\n", name, currentDirectory->name);
    return newDirectory;
}

// Promjena direktorija
PositionDirectory changeDirectory(char name[MAX_LENGTH], PositionDirectory currentDirectory) {
    PositionDirectory temp = currentDirectory->subDirectories;
    while (temp) {
        if (strcmp(temp->name, name) == 0) {
            printf("Changed to directory '%s'.\n", temp->name);
            return temp;
        }
        temp = temp->next;
    }
    printf("Directory '%s' not found.\n", name);
    return currentDirectory;
}

// Ispis sadržaja direktorija
int listDirectoryContents(PositionDirectory currentDirectory) {
    printf("Contents of '%s':\n", currentDirectory->name);
    PositionDirectory temp = currentDirectory->subDirectories;
    if (!temp) {
        printf("   (empty)\n");
        return EXIT_SUCCESS;
    }
    while (temp) {
        printf("   - %s\n", temp->name);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}
