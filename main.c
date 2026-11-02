#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATA_FILE "books.txt"
#define TITLE_LEN 100
#define AUTHOR_LEN 100
#define LINE_LEN 256

/*
 * Library Management System
 * DSA concepts:
 *   - Singly linked list for storing books dynamically
 *   - Linear search for finding a book by ID
 *
 * Data is saved to books.txt when the program exits and loaded at startup.
 */

typedef struct Book {
    int id;
    char title[TITLE_LEN];
    char author[AUTHOR_LEN];
    int is_issued;                 /* 0 = available, 1 = issued */
    struct Book *next;
} Book;

static Book *head = NULL;

/* Read a line safely and remove its trailing newline. */
static void read_line(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

/* Read an integer. Returns 0 if the input is not a valid integer. */
static int read_int(const char *prompt, int *value) {
    char line[LINE_LEN];
    char extra;

    printf("%s", prompt);

    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }

    return sscanf(line, " %d %c", value, &extra) == 1;
}

/* Find a book by its unique ID. */
static Book *find_book(int id) {
    Book *current = head;

    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

/* Add a new book at the end of the linked list. */
static void add_book(void) {
    int id;
    char title[TITLE_LEN];
    char author[AUTHOR_LEN];

    if (!read_int("Enter book ID: ", &id)) {
        printf("Invalid ID. Please enter a whole number.\n");
        return;
    }

    if (id <= 0) {
        printf("Book ID must be positive.\n");
        return;
    }

    if (find_book(id) != NULL) {
        printf("A book with this ID already exists.\n");
        return;
    }

    read_line("Enter book title: ", title, sizeof(title));
    read_line("Enter author name: ", author, sizeof(author));

    if (title[0] == '\0' || author[0] == '\0') {
        printf("Title and author cannot be empty.\n");
        return;
    }

    Book *new_book = (Book *)malloc(sizeof(Book));
    if (new_book == NULL) {
        printf("Memory allocation failed. Book was not added.\n");
        return;
    }

    new_book->id = id;
    snprintf(new_book->title, sizeof(new_book->title), "%s", title);
    snprintf(new_book->author, sizeof(new_book->author), "%s", author);
    new_book->is_issued = 0;
    new_book->next = NULL;

    if (head == NULL) {
        head = new_book;
    } else {
        Book *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_book;
    }

    printf("Book added successfully.\n");
}

/* Display all books. */
static void display_books(void) {
    Book *current = head;

    if (current == NULL) {
        printf("No books are currently in the library.\n");
        return;
    }

    printf("\n%-8s %-30s %-25s %-12s\n",
           "ID", "TITLE", "AUTHOR", "STATUS");
    printf("--------------------------------------------------------------------------\n");

    while (current != NULL) {
        printf("%-8d %-30.30s %-25.25s %-12s\n",
               current->id,
               current->title,
               current->author,
               current->is_issued ? "Issued" : "Available");
        current = current->next;
    }
}

/* Search for a book using its ID. */
static void search_book(void) {
    int id;

    if (!read_int("Enter book ID to search: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }

    Book *book = find_book(id);

    if (book == NULL) {
        printf("Book not found.\n");
        return;
    }

    printf("\nBook found:\n");
    printf("ID     : %d\n", book->id);
    printf("Title  : %s\n", book->title);
    printf("Author : %s\n", book->author);
    printf("Status : %s\n", book->is_issued ? "Issued" : "Available");
}

/* Mark a book as issued. */
static void issue_book(void) {
    int id;

    if (!read_int("Enter book ID to issue: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }

    Book *book = find_book(id);

    if (book == NULL) {
        printf("Book not found.\n");
    } else if (book->is_issued) {
        printf("This book is already issued.\n");
    } else {
        book->is_issued = 1;
        printf("Book issued successfully.\n");
    }
}

/* Mark a book as returned. */
static void return_book(void) {
    int id;

    if (!read_int("Enter book ID to return: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }

    Book *book = find_book(id);

    if (book == NULL) {
        printf("Book not found.\n");
    } else if (!book->is_issued) {
        printf("This book is already available.\n");
    } else {
        book->is_issued = 0;
        printf("Book returned successfully.\n");
    }
}

/*
 * File format per line:
 * id|issued|title|author
 * The program does not allow '|' in title or author.
 */
static void save_books(void) {
    FILE *file = fopen(DATA_FILE, "w");

    if (file == NULL) {
        printf("Warning: could not save data to %s.\n", DATA_FILE);
        return;
    }

    Book *current = head;
    while (current != NULL) {
        fprintf(file, "%d|%d|%s|%s\n",
                current->id,
                current->is_issued,
                current->title,
                current->author);
        current = current->next;
    }

    fclose(file);
}

/* Load previously saved books when the program starts. */
static void load_books(void) {
    FILE *file = fopen(DATA_FILE, "r");
    char line[LINE_LEN];

    if (file == NULL) {
        return; /* First run: no data file yet. */
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        int id, issued;
        char title[TITLE_LEN];
        char author[AUTHOR_LEN];

        if (sscanf(line, "%d|%d|%99[^|]|%99[^\n]",
                   &id, &issued, title, author) == 4) {
            if (id <= 0 || find_book(id) != NULL) {
                continue;
            }

            Book *book = (Book *)malloc(sizeof(Book));
            if (book == NULL) {
                printf("Warning: not enough memory to load all books.\n");
                break;
            }

            book->id = id;
            book->is_issued = (issued == 1);
            snprintf(book->title, sizeof(book->title), "%s", title);
            snprintf(book->author, sizeof(book->author), "%s", author);
            book->next = NULL;

            if (head == NULL) {
                head = book;
            } else {
                Book *tail = head;
                while (tail->next != NULL) {
                    tail = tail->next;
                }
                tail->next = book;
            }
        }
    }

    fclose(file);
}

/* Release all dynamically allocated nodes. */
static void free_books(void) {
    Book *current = head;

    while (current != NULL) {
        Book *next = current->next;
        free(current);
        current = next;
    }

    head = NULL;
}

int main(void) {
    int choice;

    load_books();

    printf("====================================\n");
    printf("       LIBRARY MANAGEMENT SYSTEM\n");
    printf("====================================\n");

    while (1) {
        printf("\n1. Add a book\n");
        printf("2. Display all books\n");
        printf("3. Search for a book\n");
        printf("4. Issue a book\n");
        printf("5. Return a book\n");
        printf("6. Save and exit\n");

        if (!read_int("Enter your choice: ", &choice)) {
            printf("Invalid choice. Please enter a number from 1 to 6.\n");
            continue;
        }

        switch (choice) {
            case 1:
                add_book();
                break;
            case 2:
                display_books();
                break;
            case 3:
                search_book();
                break;
            case 4:
                issue_book();
                break;
            case 5:
                return_book();
                break;
            case 6:
                save_books();
                free_books();
                printf("Data saved. Thank you for using the system!\n");
                return 0;
            default:
                printf("Invalid choice. Please select 1 to 6.\n");
        }
    }
}
