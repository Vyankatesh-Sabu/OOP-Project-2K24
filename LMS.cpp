#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <ctime>
#include <cctype>
// #include <bits/stdc++.h>
using namespace std;

const int LATE_FINE = 10;
const int MAX_ISSUED_BOOKS = 2;

class LibraryItem {
public:
    virtual void display_info() const = 0;
};

class Book : public LibraryItem {
public:
    string id;
    string author;
    string title;
    string publisher;
    int copies_available;

    Book() {}

    Book(string id, string author, string title, string publisher, int copies_available)
        : id(id), author(author), title(title), publisher(publisher), copies_available(copies_available) {}

    void display_info() const override {
        cout << "ID: " << id 
             << ", Title: " << title 
             << ", Author: " << author 
             << ", Publisher: " << publisher 
             << ", Copies Available: " << copies_available << endl;
    }
};

class Student {
public:
    string student_id;
    string name;
    int books_issued;

    Student() {}

    Student(string student_id, string name, int books_issued)
        : student_id(student_id), name(name), books_issued(books_issued) {}

    void display_student_info() const {
        cout << "Student ID: " << student_id 
             << ", Name: " << name 
             << ", Books Issued: " << books_issued << endl;
    }
};

class Issue {
public:
    string student_id;
    string book_id;
    string issue_date;
    string return_date;

    Issue(string student_id, string book_id, string issue_date, string return_date)
        : student_id(student_id), book_id(book_id), issue_date(issue_date), return_date(return_date) {}
};

class Library {
private:
    vector<Book> books;
    vector<Student> students;
    vector<Issue> issued_books;

public:
    void load_books() {
        ifstream file("book_data.csv");
        string line;

        if (!file.is_open()) {
            cout << "Error opening book data file." << endl;
            return;
        }

        while (getline(file, line)) {
            stringstream ss(line);
            string id, author, title, publisher, copies;
            getline(ss, id, ',');
            getline(ss, author, ',');
            getline(ss, title, ',');
            getline(ss, publisher, ',');
            getline(ss, copies, ',');

            int copies_available = convert_string_to_int(copies);
            Book b(id, author, title, publisher, copies_available);
            books.push_back(b);
        }

        file.close();
    }

    void load_students() {
        ifstream file("stud_data.csv");
        string line;

        if (!file.is_open()) {
            cout << "Error opening student data file." << endl;
            return;
        }

        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, books_issued;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, books_issued, ',');

            int issued_books_count = convert_string_to_int(books_issued);
            Student s(id, name, issued_books_count);
            students.push_back(s);
        }

        file.close();
    }

    void load_issue_data() {
        ifstream file("issue_data.csv");
        string line;

        if (!file.is_open()) {
            cout << "Error opening issue data file." << endl;
            return;
        }

        while (getline(file, line)) {
            stringstream ss(line);
            string student_id, book_id, issue_date, return_date;
            getline(ss, student_id, ',');
            getline(ss, book_id, ',');
            getline(ss, issue_date, ',');
            getline(ss, return_date, ',');

            Issue i(student_id, book_id, issue_date, return_date);
            issued_books.push_back(i);
        }

        file.close();
    }

    bool load_admin_data(const string& username, const string& password) {
        ifstream file("admin_data.csv");
        string line;

        if (!file.is_open()) {
            cout << "Error opening admin data file." << endl;
            return false;
        }

        while (getline(file, line)) {
            stringstream ss(line);
            string user, pass;
            getline(ss, user, ',');
            getline(ss, pass, ',');

            if (user == username && pass == password) {
                return true;
            }
        }

        file.close();
        return false;
    }

    int convert_string_to_int(const string& str) {
        int num = 0;
        for (char c : str) {
            if (isdigit(c)) {
                num = num * 10 + (c - '0');
            }
        }
        return num;
    }

    bool is_book_id_unique(const string& id) const {
        for (const auto& book : books) {
            if (book.id == id) {
                return false;
            }
        }
        return true;
    }

    void add_book() {
        string id, author, title, publisher;
        int copies;

        cout << "Enter Book ID: ";
        cin >> id;

        if (!is_book_id_unique(id)) {
            cout << "Error: Book ID must be unique." << endl;
            return;
        }

        cout << "Enter Book Author: ";
        cin.ignore();
        getline(cin, author);
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Book Publisher: ";
        getline(cin, publisher);
        cout << "Enter Number of Copies Available: ";
        cin >> copies;

        Book b(id, author, title, publisher, copies);
        books.push_back(b);

        ofstream file("book_data.csv", ios::app);
        if (file.is_open()) {
            file << id << "," << author << "," << title << "," << publisher << "," << copies << "\n";
            file.close();
        } else {
            cout << "Error opening book data file." << endl;
        }

        cout << "Book added successfully!" << endl;
    }

    void view_books() const {
        cout << "Available Books:" << endl;
        for (const auto& book : books) {
            book.display_info();
        }
    }

    void search_book() const {
        string query;
        cout << "Enter title or author to search: ";
        cin.ignore();
        getline(cin, query);
        bool found = false;

        for (const auto& book : books) {
            if (book.title.find(query) != string::npos || book.author.find(query) != string::npos) {
                book.display_info();
                found = true;
            }
        }

        if (!found) {
            cout << "No books found matching the query." << endl;
        }
    }

    string get_current_date() {
        time_t now = time(0);
        char* dt = ctime(&now);
        string date = dt;
        date.pop_back();
        return date;
    }

    int calculate_days_difference(const string& date1, const string& date2) {
        tm tm1 = {};
        tm tm2 = {};
        strptime(date1.c_str(), "%c", &tm1);
        strptime(date2.c_str(), "%c", &tm2);
        time_t time1 = mktime(&tm1);
        time_t time2 = mktime(&tm2);
        return difftime(time2, time1) / (60 * 60 * 24);
    }

    void issue_book() {
        string student_id, book_id;
        cout << "Enter Student ID: ";
        cin >> student_id;
        cout << "Enter Book ID: ";
        cin >> book_id;

        Student* student = nullptr;
        for (int i = 0; i < students.size(); i++) {
            if (students[i].student_id == student_id) {
                student = &students[i];
                break;
            }
        }

        if (student == nullptr) {
            cout << "Student not found!" << endl;
            return;
        }

        if (student->books_issued >= MAX_ISSUED_BOOKS) {
            cout << "Error: Student cannot issue more than " << MAX_ISSUED_BOOKS << " books." << endl;
            return;
        }

        Book* book = nullptr;
        for (int i = 0; i < books.size(); i++) {
            if (books[i].id == book_id && books[i].copies_available > 0) {
                book = &books[i];
                break;
            }
        }

        if (book == nullptr) {
            cout << "Book not available!" << endl;
            return;
        }

        student->books_issued++;
        book->copies_available--;
        string issue_date = get_current_date();
        issued_books.push_back(Issue(student_id, book_id, issue_date, ""));
        
        ofstream file("issue_data.csv", ios::app);
        if (file.is_open()) {
            file << student_id << "," << book_id << "," << issue_date << "," << "" << "\n";
            file.close();
        } else {
            cout << "Error opening issue data file." << endl;
        }

        cout << "Book issued successfully!" << endl;
    }

    void return_book() {
        string student_id, book_id;
        cout << "Enter Student ID: ";
        cin >> student_id;
        cout << "Enter Book ID: ";
        cin >> book_id;

        bool issue_found = false;
        string return_date = get_current_date();

        for (int i = 0; i < issued_books.size(); i++) {
            if (issued_books[i].student_id == student_id && issued_books[i].book_id == book_id && issued_books[i].return_date.empty()) {
                issue_found = true;
                issued_books[i].return_date = return_date;

                int days_difference = calculate_days_difference(issued_books[i].issue_date, return_date);
                if (days_difference > 15) {
                    int fine = (days_difference - 15) * LATE_FINE;
                    cout << "Book returned late! Fine amount: $" << fine << endl;
                }

                for (int j = 0; j < books.size(); j++) {
                    if (books[j].id == book_id) {
                        books[j].copies_available++;
                        break;
                    }
                }

                for (int j = 0; j < students.size(); j++) {
                    if (students[j].student_id == student_id) {
                        students[j].books_issued--;
                        break;
                    }
                }
                break;
            }
        }

        if (!issue_found) {
            cout << "No issue record found for the given student and book ID." << endl;
            return;
        }

        ofstream file("issue_data.csv");
    if (file.is_open()) {
        for (const auto& issue : issued_books) {
            file << issue.student_id << "," 
                 << issue.book_id << "," 
                 << issue.issue_date << "," 
                 << issue.return_date << "\n"; // Ensure return_date is updated
        }
        file.close();
        cout << "Book return information updated successfully!" << endl;
    } else {
        cout << "Error opening issue data file for writing." << endl;
    }
}
    

    void view_issued_books() const {
        cout << "Issued Books:" << endl;
        for (const auto& issue : issued_books) {
            cout << "Student ID: " << issue.student_id 
                 << ", Book ID: " << issue.book_id 
                 << ", Issue Date: " << issue.issue_date 
                 << ", Return Date: " << issue.return_date << endl;
        }
    }
};

bool admin_login(Library& library) {
    string username, password;
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Admin Password: ";
    cin >> password;

    if (library.load_admin_data(username, password)) {
        cout << "Login successful!" << endl;
        return true;
    } else {
        cout << "Invalid username or password!" << endl;
        return false;
    }
}

int main() {
    Library library;
    library.load_books();
    library.load_students();
    library.load_issue_data();

    if (!admin_login(library)) {
        return 0;
    }

    int choice;
    while (1) {
        system("clear");

        cout << "\nLibrary Management System\n";
        cout << "1. View Books\n";
        cout << "2. Add Book\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. View Issued Books\n";
        cout << "6. Search Book\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                library.view_books();
                break;
            case 2:
                library.add_book();
                break;
            case 3:
                library.issue_book();
                break;
            case 4:
                library.return_book();
                break;
            case 5:
                library.view_issued_books();
                break;
            case 6:
                library.search_book();
                break;
            case 7:
                cout << "Exiting the program." << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}
