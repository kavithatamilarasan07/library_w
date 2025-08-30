#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>   // for cin.ignore()
using namespace std;

class Book {
    int bookID;
    string title;
    string author;
    bool available;

public:
    Book(int id = 0, string t = "", string a = "", bool av = true)
        : bookID(id), title(t), author(a), available(av) {}

    int getID() { return bookID; }
    string getTitle() { return title; }
    string getAuthor() { return author; }
    bool isAvailable() { return available; }

    void issueBook() { available = false; }
    void returnBook() { available = true; }

    void display() {
        cout << "ID: " << bookID
             << " | Title: " << title
             << " | Author: " << author
             << " | Status: " << (available ? "Available" : "Issued") << endl;
    }

    // Save to file
    void saveToFile(ofstream &out) {
        out << bookID << "," << title << "," << author << "," << available << endl;
    }

    // Load from file
    static Book loadFromFile(string line) {
        int id, av;
        string t, a;
        size_t p1 = line.find(",");
        size_t p2 = line.find(",", p1 + 1);
        size_t p3 = line.find(",", p2 + 1);

        id = stoi(line.substr(0, p1));
        t = line.substr(p1 + 1, p2 - p1 - 1);
        a = line.substr(p2 + 1, p3 - p2 - 1);
        av = stoi(line.substr(p3 + 1));

        return Book(id, t, a, av);
    }
};

class Library {
    vector<Book> books;

public:
    void addBook() {
        int id;
        string t, a;
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear input buffer
        cout << "Enter Title: ";
        getline(cin, t);
        cout << "Enter Author: ";
        getline(cin, a);
        books.push_back(Book(id, t, a, true));
        cout << "Book added successfully!\n";
    }

    void searchBook() {
        cout << "Search by: 1) ID  2) Title: ";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice == 1) {
            int id;
            cout << "Enter Book ID: ";
            cin >> id;
            for (auto &b : books)
                if (b.getID() == id) { b.display(); return; }
            cout << "Book not found.\n";
        } else {
            string t;
            cout << "Enter Title: ";
            getline(cin, t);
            for (auto &b : books)
                if (b.getTitle() == t) { b.display(); return; }
            cout << "Book not found.\n";
        }
    }

    void issueBook() {
        int id;
        cout << "Enter Book ID to issue: ";
        cin >> id;
        for (auto &b : books) {
            if (b.getID() == id && b.isAvailable()) {
                b.issueBook();
                cout << "Book issued successfully!\n";
                return;
            }
        }
        cout << "Book not available.\n";
    }

    void returnBook() {
        int id;
        cout << "Enter Book ID to return: ";
        cin >> id;
        for (auto &b : books) {
            if (b.getID() == id && !b.isAvailable()) {
                b.returnBook();
                cout << "Book returned successfully!\n";
                return;
            }
        }
        cout << "Invalid return.\n";
    }

    void showStats() {
        int total = books.size(), issued = 0;
        for (auto &b : books) if (!b.isAvailable()) issued++;
        cout << "Total Books: " << total
             << " | Issued: " << issued
             << " | Available: " << total - issued << endl;
    }

    void saveAll() {
        ofstream file("library.txt");
        for (auto &b : books) b.saveToFile(file);
        cout << "Data saved successfully.\n";
    }

    void loadAll() {
        ifstream file("library.txt");
        if (!file) {
            cout << "No previous data found. Starting fresh.\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            if (!line.empty())
                books.push_back(Book::loadFromFile(line));
        }
    }
};

int main() {
    Library lib;
    lib.loadAll();  // Load existing data

    int choice;
    do {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Add Book\n2. Search Book\n3. Issue Book\n4. Return Book\n";
        cout << "5. Show Statistics\n6. Save & Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.searchBook(); break;
            case 3: lib.issueBook(); break;
            case 4: lib.returnBook(); break;
            case 5: lib.showStats(); break;
            case 6: lib.saveAll(); cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}
