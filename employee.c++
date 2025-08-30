#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Employee {
    int id;
    string name;
    double hoursWorked;
    double hourlyRate;

public:
    Employee(int i = 0, string n = "", double h = 0, double r = 0)
        : id(i), name(n), hoursWorked(h), hourlyRate(r) {}

    int getID() const { return id; }
    string getName() const { return name; }
    double getHours() const { return hoursWorked; }
    double getRate() const { return hourlyRate; }
    double getSalary() const { return hoursWorked * hourlyRate; }

    void setName(const string &n) { name = n; }
    void setHours(double h) { hoursWorked = h; }
    void setRate(double r) { hourlyRate = r; }

    void display() const {
        cout << "ID: " << id
             << " | Name: " << name
             << " | Hours: " << hoursWorked
             << " | Rate: " << hourlyRate
             << " | Salary: " << getSalary() << endl;
    }

    // Save to file
    void saveToFile(ofstream &out) const {
        out << id << "," << name << "," << hoursWorked << "," << hourlyRate << endl;
    }

    // Load from file
    static Employee loadFromFile(const string &line) {
        int i;
        string n;
        double h, r;

        size_t p1 = line.find(",");
        size_t p2 = line.find(",", p1 + 1);
        size_t p3 = line.find(",", p2 + 1);

        i = stoi(line.substr(0, p1));
        n = line.substr(p1 + 1, p2 - p1 - 1);
        h = stod(line.substr(p2 + 1, p3 - p2 - 1));
        r = stod(line.substr(p3 + 1));

        return Employee(i, n, h, r);
    }
};

class Payroll {
    vector<Employee> employees;

public:
    void addEmployee() {
        int id;
        string name;
        double hours, rate;
        cout << "Enter Employee ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Hours Worked: ";
        cin >> hours;
        cout << "Enter Hourly Rate: ";
        cin >> rate;

        employees.push_back(Employee(id, name, hours, rate));
        cout << "Employee added successfully!\n";
    }

    void updateEmployee() {
        int id;
        cout << "Enter Employee ID to update: ";
        cin >> id;
        for (auto &e : employees) {
            if (e.getID() == id) {
                string name;
                double hours, rate;
                cin.ignore();
                cout << "Enter New Name: ";
                getline(cin, name);
                cout << "Enter New Hours Worked: ";
                cin >> hours;
                cout << "Enter New Hourly Rate: ";
                cin >> rate;
                e.setName(name);
                e.setHours(hours);
                e.setRate(rate);
                cout << "Employee updated successfully!\n";
                return;
            }
        }
        cout << "Employee not found.\n";
    }

    void deleteEmployee() {
        int id;
        cout << "Enter Employee ID to delete: ";
        cin >> id;
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->getID() == id) {
                employees.erase(it);
                cout << "Employee deleted successfully!\n";
                return;
            }
        }
        cout << "Employee not found.\n";
    }

    void searchEmployee() const {
        int id;
        cout << "Enter Employee ID to search: ";
        cin >> id;
        for (const auto &e : employees) {
            if (e.getID() == id) {
                e.display();
                return;
            }
        }
        cout << "Employee not found.\n";
    }

    void listEmployees() const {
        if (employees.empty()) {
            cout << "No employees available.\n";
            return;
        }
        for (const auto &e : employees) e.display();
    }

    void summaryReport() const {
        double total = 0;
        for (const auto &e : employees) total += e.getSalary();
        cout << "Total Payroll Amount: " << total << endl;
    }

    void saveAll() const {
        ofstream file("payroll.txt");
        if (!file) {
            cout << "Error saving data!\n";
            return;
        }
        for (const auto &e : employees) e.saveToFile(file);
        cout << "Data saved successfully.\n";
    }

    void loadAll() {
        ifstream file("payroll.txt");
        string line;
        if (!file) {
            cout << "No previous data found. Starting fresh.\n";
            return;
        }
        while (getline(file, line)) {
            if (!line.empty())
                employees.push_back(Employee::loadFromFile(line));
        }
    }
};

int main() {
    Payroll payroll;
    payroll.loadAll();

    int choice;
    do {
        cout << "\n===== Employee Payroll System =====\n";
        cout << "1. Add Employee\n2. Update Employee\n3. Delete Employee\n";
        cout << "4. Search Employee\n5. List Employees\n6. Summary Report\n";
        cout << "7. Save & Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: payroll.addEmployee(); break;
            case 2: payroll.updateEmployee(); break;
            case 3: payroll.deleteEmployee(); break;
            case 4: payroll.searchEmployee(); break;
            case 5: payroll.listEmployees(); break;
            case 6: payroll.summaryReport(); break;
            case 7: payroll.saveAll(); cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);

    return 0;
}
