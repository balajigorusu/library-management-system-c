#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[100];
    char author[100];
    int quantity;
};

// Function prototypes
void addBook();
void displayBooks();
void searchBook();
void updateBook();
void deleteBook();

int main() {
    int choice;

    do {
        printf("\n--- Library Management System ---\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book by ID\n");
        printf("4. Update Book\n");
        printf("5. Delete Book\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: updateBook(); break;
            case 5: deleteBook(); break;
            case 6: printf("Exiting program.\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while(choice != 6);

    return 0;
}

void addBook() {
    FILE *fp = fopen("library.dat", "ab");
    struct Book b;

    printf("Enter book ID: ");
    scanf("%d", &b.id);
    getchar(); // clear newline
    printf("Enter title: ");
    fgets(b.title, sizeof(b.title), stdin);
    printf("Enter author: ");
    fgets(b.author, sizeof(b.author), stdin);
    printf("Enter quantity: ");
    scanf("%d", &b.quantity);

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("Book added successfully!\n");
}

void displayBooks() {
    FILE *fp = fopen("library.dat", "rb");
    struct Book b;

    if (fp == NULL) {
        printf("No books found.\n");
        return;
    }

    printf("\n%-5s %-30s %-30s %-10s\n", "ID", "Title", "Author", "Quantity");
    printf("---------------------------------------------------------------\n");

    while (fread(&b, sizeof(b), 1, fp)) {
        printf("%-5d %-30s %-30s %-10d", b.id, b.title, b.author, b.quantity);
    }

    fclose(fp);
}

void searchBook() {
    FILE *fp = fopen("library.dat", "rb");
    struct Book b;
    int id, found = 0;

    printf("Enter book ID to search: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == id) {
            printf("\nBook found!\n");
            printf("ID: %d\nTitle: %sAuthor: %sQuantity: %d\n", b.id, b.title, b.author, b.quantity);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book not found.\n");
    }

    fclose(fp);
}

void updateBook() {
    FILE *fp = fopen("library.dat", "rb+");
    struct Book b;
    int id, found = 0;

    printf("Enter book ID to update: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == id) {
            printf("Enter new title: ");
            getchar(); // clear newline
            fgets(b.title, sizeof(b.title), stdin);
            printf("Enter new author: ");
            fgets(b.author, sizeof(b.author), stdin);
            printf("Enter new quantity: ");
            scanf("%d", &b.quantity);

            fseek(fp, -sizeof(b), SEEK_CUR);
            fwrite(&b, sizeof(b), 1, fp);
            found = 1;
            printf("Book updated successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Book not found.\n");
    }

    fclose(fp);
}

void deleteBook() {
    FILE *fp = fopen("library.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    struct Book b;
    int id, found = 0;

    printf("Enter book ID to delete: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id != id) {
            fwrite(&b, sizeof(b), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (found) {
        printf("Book deleted successfully.\n");
    } else {
        printf("Book not found.\n");
    }
}

