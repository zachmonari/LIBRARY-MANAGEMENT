#include <stdio.h>
#include <string.h>

typedef struct {
    char title[100];
    char author[100];
    char ISBN[20];
    int isAvailable; // 1 if available, 0 if borrowed
} Book;

void addBook() {
    FILE *file = fopen("library.dat", "ab");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }
    
    Book newBook;
    printf("Enter title: ");
    fgets(newBook.title, 100, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = '\0';
    
    printf("Enter author: ");
    fgets(newBook.author, 100, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0';
    
    printf("Enter ISBN: ");
    fgets(newBook.ISBN, 20, stdin);
    newBook.ISBN[strcspn(newBook.ISBN, "\n")] = '\0';
    
    newBook.isAvailable = 1;
    
    fwrite(&newBook, sizeof(Book), 1, file);
    fclose(file);
    
    printf("Book added successfully.\n");
}
void deleteBook() {
    char isbn[20];
    printf("Enter ISBN of the book to delete: ");
    fgets(isbn, 20, stdin);
    isbn[strcspn(isbn, "\n")] = '\0';

    FILE *file = fopen("library.dat", "rb");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    FILE *tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        printf("Error opening temporary file.\n");
        fclose(file);
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (strcmp(book.ISBN, isbn) != 0) {
            fwrite(&book, sizeof(Book), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("library.dat");
        rename("temp.dat", "library.dat");
        printf("Book deleted successfully.\n");
    } else {
        remove("temp.dat");
        printf("Book not found.\n");
    }
}

void searchBook() {
    char query[100];
    printf("Enter title, author, or ISBN to search: ");
    fgets(query, 100, stdin);
    query[strcspn(query, "\n")] = '\0';

    FILE *file = fopen("library.dat", "rb");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (strstr(book.title, query) || strstr(book.author, query) || strstr(book.ISBN, query)) {
            printf("Title: %s\n", book.title);
            printf("Author: %s\n", book.author);
            printf("ISBN: %s\n", book.ISBN);
            printf("Availability: %s\n", book.isAvailable ? "Available" : "Borrowed");
            found = 1;
        }
    }

    fclose(file);

    if (!found) {
        printf("No books found.\n");
    }
}

void viewBooks() {
    FILE *file = fopen("library.dat", "rb");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    Book book;
    while (fread(&book, sizeof(Book), 1, file)) {
        printf("Title: %s\n", book.title);
        printf("Author: %s\n", book.author);
        printf("ISBN: %s\n", book.ISBN);
        printf("Availability: %s\n", book.isAvailable ? "Available" : "Borrowed");
        printf("--------------------------\n");
    }

    fclose(file);
}

void borrowBook() {
    char isbn[20];
    printf("Enter ISBN of the book to borrow: ");
    fgets(isbn, 20, stdin);
    isbn[strcspn(isbn, "\n")] = '\0';

    FILE *file = fopen("library.dat", "rb+");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (strcmp(book.ISBN, isbn) == 0) {
            if (book.isAvailable) {
                book.isAvailable = 0;
                fseek(file, -sizeof(Book), SEEK_CUR);
                fwrite(&book, sizeof(Book), 1, file);
                printf("Book borrowed successfully.\n");
            } else {
                printf("Book is already borrowed.\n");
            }
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Book not found.\n");
    }
}

void returnBook() {
    char isbn[20];
    printf("Enter ISBN of the book to return: ");
    fgets(isbn, 20, stdin);
    isbn[strcspn(isbn, "\n")] = '\0';

    FILE *file = fopen("library.dat", "rb+");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (strcmp(book.ISBN, isbn) == 0) {
            if (!book.isAvailable) {
                book.isAvailable = 1;
                fseek(file, -sizeof(Book), SEEK_CUR);
                fwrite(&book, sizeof(Book), 1, file);
                printf("Book returned successfully.\n");
            } else {
                printf("Book is not borrowed.\n");
            }
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Book not found.\n");
    }
}


//main block
int main() {
    int choice;
    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Delete Book\n");
        printf("3. Search Book\n");
        printf("4. View All Books\n");
        printf("5. Borrow Book\n");
        printf("6. Return Book\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // to consume newline character

        switch (choice) {
            case 1: addBook(); break;
            /*case 2: deleteBook(); break;
            case 3: searchBook(); break;
            case 4: viewBooks(); break;
            case 5: borrowBook(); break;
            case 6: returnBook(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice, try again.\n");*/
            
        }
    } while (choice != 0);
    return 0;
}


