#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_INGREDIENTS 20
#define MAX_DESCRIPTION_LENGTH 200
#define HASH_TABLE_SIZE 100

typedef struct Ingredient {
    char name[MAX_NAME_LENGTH];
    double quantity;
    char unit[MAX_NAME_LENGTH];
} Ingredient;

typedef struct Recipe {
    char name[MAX_NAME_LENGTH];
    Ingredient ingredients[MAX_INGREDIENTS];
    int ingredient_count;
    char description[MAX_DESCRIPTION_LENGTH];
    struct Recipe* next; // For linked list or hash table collision handling
} Recipe;

typedef struct HashTable {
    Recipe* table[HASH_TABLE_SIZE];
} HashTable;

unsigned int hash_function(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key++;
    }
    return hash % HASH_TABLE_SIZE;
}

HashTable* create_hash_table() {
    HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable));
    if (!hash_table) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hash_table->table[i] = NULL;
    }
    return hash_table;
}

Recipe* create_recipe(const char* name, const char* description) {
    Recipe* new_recipe = (Recipe*)malloc(sizeof(Recipe));
    if (!new_recipe) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strncpy(new_recipe->name, name, MAX_NAME_LENGTH);
    strncpy(new_recipe->description, description, MAX_DESCRIPTION_LENGTH);
    new_recipe->ingredient_count = 0;
    new_recipe->next = NULL;
    return new_recipe;
}

void add_ingredient(Recipe* recipe, const char* name, double quantity, const char* unit) {
    if (recipe->ingredient_count >= MAX_INGREDIENTS) {
        printf("Maximum number of ingredients reached for this recipe.\n");
        return;
    }
    Ingredient* ingredient = &recipe->ingredients[recipe->ingredient_count++];
    strncpy(ingredient->name, name, MAX_NAME_LENGTH);
    ingredient->quantity = quantity;
    strncpy(ingredient->unit, unit, MAX_NAME_LENGTH);
}

void add_recipe(HashTable* hash_table, Recipe* recipe) {
    unsigned int index = hash_function(recipe->name);
    if (hash_table->table[index] == NULL) {
        hash_table->table[index] = recipe;
    }
    else {
        Recipe* current = hash_table->table[index];
        while (current->next) {
            current = current->next;
        }
        current->next = recipe;
    }
}

Recipe* find_recipe(HashTable* hash_table, const char* name) {
    unsigned int index = hash_function(name);
    Recipe* current = hash_table->table[index];
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void display_recipe(const Recipe* recipe) {
    if (!recipe) {
        printf("Recipe not found.\n");
        return;
    }
    printf("Recipe: %s\n", recipe->name);
    printf("Description: %s\n", recipe->description);
    printf("Ingredients:\n");
    for (int i = 0; i < recipe->ingredient_count; i++) {
        printf("  - %s: %.2f %s\n", recipe->ingredients[i].name, recipe->ingredients[i].quantity, recipe->ingredients[i].unit);
    }
}

void list_all_recipes(HashTable* hash_table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Recipe* current = hash_table->table[i];
        while (current) {
            printf("%s\n", current->name);
            current = current->next;
        }
    }
}

void delete_recipe(HashTable* hash_table, const char* name) {
    unsigned int index = hash_function(name);
    Recipe* current = hash_table->table[index];
    Recipe* prev = NULL;

    while (current) {
        if (strcmp(current->name, name) == 0) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                hash_table->table[index] = current->next;
            }
            free(current);
            printf("Recipe '%s' deleted.\n", name);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Recipe '%s' not found.\n", name);
}

void free_hash_table(HashTable* hash_table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Recipe* current = hash_table->table[i];
        while (current) {
            Recipe* to_free = current;
            current = current->next;
            free(to_free);
        }
    }
    free(hash_table);
}

void user_add_recipe(HashTable* hash_table) {
    char name[MAX_NAME_LENGTH], description[MAX_DESCRIPTION_LENGTH];
    printf("Enter recipe name: ");
    fgets(name, MAX_NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter recipe description: ");
    fgets(description, MAX_DESCRIPTION_LENGTH, stdin);
    description[strcspn(description, "\n")] = '\0';

    Recipe* new_recipe = create_recipe(name, description);

    int ingredient_count;
    printf("Enter number of ingredients: ");
    scanf("%d", &ingredient_count);
    getchar(); // Consume newline

    for (int i = 0; i < ingredient_count; i++) {
        char ingredient_name[MAX_NAME_LENGTH], unit[MAX_NAME_LENGTH];
        double quantity;

        printf("Enter ingredient %d name: ", i + 1);
        fgets(ingredient_name, MAX_NAME_LENGTH, stdin);
        ingredient_name[strcspn(ingredient_name, "\n")] = '\0';

        printf("Enter quantity: ");
        scanf("%lf", &quantity);
        getchar(); // Consume newline

        printf("Enter unit: ");
        fgets(unit, MAX_NAME_LENGTH, stdin);
        unit[strcspn(unit, "\n")] = '\0';

        add_ingredient(new_recipe, ingredient_name, quantity, unit);
    }

    add_recipe(hash_table, new_recipe);
    printf("Recipe '%s' added successfully.\n", name);
}

void user_delete_recipe(HashTable* hash_table) {
    char name[MAX_NAME_LENGTH];
    printf("Enter the name of the recipe to delete: ");
    fgets(name, MAX_NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = '\0';

    delete_recipe(hash_table, name);
}

int main() {
    HashTable* cookbook = create_hash_table();

    int choice;
    do {
        printf("\nCookbook Menu:\n");
        printf("1. Add Recipe\n");
        printf("2. Delete Recipe\n");
        printf("3. List All Recipes\n");
        printf("4. Find Recipe\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
        case 1:
            user_add_recipe(cookbook);
            break;
        case 2:
            user_delete_recipe(cookbook);
            break;
        case 3:
            printf("All recipes:\n");
            list_all_recipes(cookbook);
            break;
        case 4: {
            char name[MAX_NAME_LENGTH];
            printf("Enter the name of the recipe to find: ");
            fgets(name, MAX_NAME_LENGTH, stdin);
            name[strcspn(name, "\n")] = '\0';
            display_recipe(find_recipe(cookbook, name));
            break;
        }
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.");
        }
    }
}