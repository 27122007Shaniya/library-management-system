# Library Management System (C)

A beginner-friendly, menu-driven Library Management System built in C. It demonstrates core Data Structures and Algorithms concepts using a **singly linked list** and **linear search**.

## Features

- Add a book with a unique ID, title, and author
- Display all books and their availability
- Search for a book by ID
- Issue an available book
- Return an issued book
- Save book records to `books.txt` and load them on the next run
- Dynamically allocate and free linked-list nodes

## DSA concepts used

| Concept | Where it is used |
|---|---|
| Singly linked list | Store the collection of books |
| Dynamic memory allocation | Create a node when a book is added |
| Linear search | Find a book by its ID |
| Linked-list traversal | Display, search, save, and free records |
| File handling | Persist records between runs |

## Project structure

```text
library-management-system/
├── main.c
├── README.md
├── .gitignore
└── books.txt          # Created automatically when the program saves
```

## Requirements

- GCC or another C compiler
- Terminal / command prompt

## Compile and run

### Linux / macOS

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c -o library
./library
```

### Windows (MinGW)

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c -o library.exe
library.exe
```

The program creates `books.txt` in the current working directory when you choose **Save and exit**. Keep this file in the same directory when running the program again.

## Sample run

```text
====================================
       LIBRARY MANAGEMENT SYSTEM
====================================

1. Add a book
2. Display all books
3. Search for a book
4. Issue a book
5. Return a book
6. Save and exit
Enter your choice: 1
Enter book ID: 101
Enter book title: The Alchemist
Enter author name: Paulo Coelho
Book added successfully.

1. Add a book
2. Display all books
3. Search for a book
4. Issue a book
5. Return a book
6. Save and exit
Enter your choice: 2

ID       TITLE                          AUTHOR                    STATUS
--------------------------------------------------------------------------
101      The Alchemist                  Paulo Coelho              Available

1. Add a book
2. Display all books
3. Search for a book
4. Issue a book
5. Return a book
6. Save and exit
Enter your choice: 4
Enter book ID to issue: 101
Book issued successfully.

1. Add a book
2. Display all books
3. Search for a book
4. Issue a book
5. Return a book
6. Save and exit
Enter your choice: 6
Data saved. Thank you for using the system!
```

*The sample is illustrative; the menu is repeated after each operation.*

## Time complexity

Let `n` be the number of books currently stored.

| Operation | Time complexity | Reason |
|---|---:|---|
| Add book | O(n) | Traverse to the end of the list; duplicate-ID check is also O(n) |
| Display books | O(n) | Visit every node |
| Search by ID | O(n) | Linear search |
| Issue / Return | O(n) | Find the book, then update its status |
| Save / Load | O(n) for traversal, excluding file I/O costs | Visit each record |

Each book node uses **O(1)** extra memory. The full linked list uses **O(n)** memory.

## Data file format

The program saves one book per line in `books.txt`:

```text
101|1|The Alchemist|Paulo Coelho
```

The fields are `book_id|issued_status|title|author`. A status of `0` means available; `1` means issued. Avoid using the pipe character (`|`) in titles or author names because it separates fields in the data file.

## Possible improvements

- Delete a book
- Update book details
- Search by title or author
- Sort books by title or ID
- Add member records and due dates
- Replace linear search with a binary search tree or hash table

## License

This project is provided for learning and educational use. You may modify it and use it in your own portfolio.
