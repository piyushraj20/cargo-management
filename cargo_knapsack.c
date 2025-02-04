#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100

typedef struct {
    char id[20];
    char description[50];
    int weight;
    int value;
    double ratio;  // Profit-to-weight ratio
} Item;

// Function to load data from a CSV file
void load_data(const char *filename, Item items[], int *n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        exit(1);
    }

    char line[100];
    fgets(line, sizeof(line), file);  // Skip header row

    *n = 0;
    while (fscanf(file, "%[^,],%[^,],%d,%d\n", 
                  items[*n].id, items[*n].description, 
                  &items[*n].weight, &items[*n].value) == 4) {
        items[*n].ratio = (double) items[*n].value / items[*n].weight; // Calculate ratio
        (*n)++;
    }

    fclose(file);
}

// Comparator function for sorting items by profit-to-weight ratio
int compare_items(const void *a, const void *b) {
    Item *itemA = (Item *) a;
    Item *itemB = (Item *) b;
    return (itemB->ratio > itemA->ratio) - (itemB->ratio < itemA->ratio); // Sort in descending order
}

// Greedy knapsack function
void greedy_knapsack(Item items[], int n, int max_weight) {
    // Sort items by profit-to-weight ratio
    qsort(items, n, sizeof(Item), compare_items);

    int total_weight = 0;
    int total_value = 0;

    printf("Selected items:\n");
    for (int i = 0; i < n; i++) {
        if (total_weight + items[i].weight <= max_weight) {
            printf("%s - %s - Weight: %d kg, Profit: $%d\n", 
                   items[i].id, items[i].description, 
                   items[i].weight, items[i].value);
            total_weight += items[i].weight;
            total_value += items[i].value;
        }
    }

    printf("\nTotal Weight: %d kg\n", total_weight);
    printf("Total Profit: $%d\n", total_value);
}

int main() {
    Item items[MAX_ITEMS];
    int n;
    int max_weight;
    char filename[50];

    // Ask user for the filename
    printf("Enter the CSV filename : ");
    scanf("%s", filename);

    // Load data from the file specified by the user
    load_data(filename, items, &n);

    // Ask user for the cargo capacity
    printf("Enter the maximum cargo weight capacity (kg): ");
    scanf("%d", &max_weight);

    // Run the greedy knapsack algorithm
    greedy_knapsack(items, n, max_weight);

    return 0;
}
