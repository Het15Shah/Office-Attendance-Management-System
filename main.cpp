#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

// Forward declarations
class Employee;
class Admin;

// Base class for common functionalities
class General {
protected:
    string name;
    string userID;
    string password;
    
public:
    General() {}
    General(string n, string id, string pass) : name(n), userID(id), password(pass) {}
    
    // Pure virtual functions for polymorphism
    virtual void view_call() = 0;
    virtual void Calendar() = 0;
    virtual bool abs() = 0;
    
    // Common getters
    string getName() const { return name; }
    string getUserID() const { return userID; }
    string getPassword() const { return password; }
    
    // Virtual destructor for proper cleanup
    virtual ~General() {}
};

// Structure for leave request
struct LeaveRequest {
    string employeeID;
    string employeeName;
    string leaveDate;
    string reason;
    string status; // "Pending", "Approved", "Rejected"
    
    LeaveRequest(string id, string name, string date, string r) 
        : employeeID(id), employeeName(name), leaveDate(date), reason(r), status("Pending") {}
};

// Structure for attendance record
struct AttendanceRecord {
    string date;
    bool present;
    string timeIn;
    string timeOut;
    
    AttendanceRecord(string d, bool p, string tIn = "", string tOut = "") 
        : date(d), present(p), timeIn(tIn), timeOut(tOut) {}
};

// Global variables and data structures
class Global {
public:
    static vector<string> Mdays;
    static string time_t_now;
    static string tm_ltm;
    static string Today;
    static string TMonth;
    static string TYear;
    static string THour;
    static string cmd;
    static string rg;
    static vector<string> holi;
    static queue<LeaveRequest> Lreq;
    static unordered_map<string, vector<AttendanceRecord>> attendanceHash;
    static vector<Employee*> emp_array;
    static string login_user;
    
    // Initialize current date and time
    static void initializeDateTime() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        
        Today = to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);
        TMonth = to_string(1 + ltm->tm_mon);
        TYear = to_string(1900 + ltm->tm_year);
        THour = to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min);
        
        // Initialize month days
        Mdays = {"31", "28", "31", "30", "31", "30", "31", "31", "30", "31", "30", "31"};
        
        // Initialize holidays
        holi = {"1/1/2024", "26/1/2024", "15/8/2024", "2/10/2024", "25/12/2024"};
    }
    
    static bool isHoliday(const string& date) {
        return find(holi.begin(), holi.end(), date) != holi.end();
    }
};

// Initialize static members
vector<string> Global::Mdays;
string Global::time_t_now;
string Global::tm_ltm;
string Global::Today;
string Global::TMonth;
string Global::TYear;
string Global::THour;
string Global::cmd;
string Global::rg;
vector<string> Global::holi;
queue<LeaveRequest> Global::Lreq;
unordered_map<string, vector<AttendanceRecord>> Global::attendanceHash;
vector<Employee*> Global::emp_array;
string Global::login_user;

// Employee class
class Employee : public General {
private:
    vector<AttendanceRecord> personalAttendance;
    vector<LeaveRequest> leaveHistory;
    bool isPresent;
    
public:
    Employee() : isPresent(false) {}
    Employee(string n, string id, string pass) : General(n, id, pass), isPresent(false) {}
    
    // Override virtual functions
    void view_call() override {
        cout << "\n=== Employee Dashboard ===" << endl;
        cout << "Welcome, " << name << "!" << endl;
        cout << "User ID: " << userID << endl;
        cout << "Today's Date: " << Global::Today << endl;
        cout << "Current Time: " << Global::THour << endl;
    }
    
    void Calendar() override {
        cout << "\n=== Monthly Calendar ===" << endl;
        cout << "Month: " << Global::TMonth << "/" << Global::TYear << endl;
        cout << "Total Days: " << Global::Mdays[stoi(Global::TMonth) - 1] << endl;
        cout << "Holidays this month: ";
        for (const string& holiday : Global::holi) {
            if (holiday.find("/" + Global::TMonth + "/" + Global::TYear) != string::npos) {
                cout << holiday << " ";
            }
        }
        cout << endl;
    }
    
    bool abs() override {
        return !isPresent;
    }
    
    // Employee specific functions
    void ask_leave() {
        string leaveDate, reason;
        cout << "\nEnter leave date (DD/MM/YYYY): ";
        cin >> leaveDate;
        cout << "Enter reason for leave: ";
        cin.ignore();
        getline(cin, reason);
        
        LeaveRequest req(userID, name, leaveDate, reason);
        Global::Lreq.push(req);
        leaveHistory.push_back(req);
        
        cout << "Leave request submitted successfully!" << endl;
    }
    
    void lv_sts() {
        cout << "\n=== Leave Status ===" << endl;
        if (leaveHistory.empty()) {
            cout << "No leave requests found." << endl;
            return;
        }
        
        for (const auto& req : leaveHistory) {
            cout << "Date: " << req.leaveDate << endl;
            cout << "Reason: " << req.reason << endl;
            cout << "Status: " << req.status << endl;
            cout << "-------------------" << endl;
        }
    }
    
    void present() {
        if (isPresent) {
            cout << "Already marked present for today!" << endl;
            return;
        }
        
        if (Global::isHoliday(Global::Today)) {
            cout << "Today is a holiday. No need to mark attendance." << endl;
            return;
        }
        
        isPresent = true;
        AttendanceRecord record(Global::Today, true, Global::THour, "");
        personalAttendance.push_back(record);
        Global::attendanceHash[userID].push_back(record);
        
        cout << "Attendance marked successfully for " << Global::Today << endl;
        cout << "Time In: " << Global::THour << endl;
    }
    
    void viewPPAtd() {
        cout << "\n=== Personal Attendance Record ===" << endl;
        if (personalAttendance.empty()) {
            cout << "No attendance records found." << endl;
            return;
        }
        
        int presentDays = 0;
        cout << left << setw(12) << "Date" << setw(10) << "Status" << setw(10) << "Time In" << "Time Out" << endl;
        cout << string(40, '-') << endl;
        
        for (const auto& record : personalAttendance) {
            cout << left << setw(12) << record.date 
                 << setw(10) << (record.present ? "Present" : "Absent")
                 << setw(10) << record.timeIn 
                 << record.timeOut << endl;
            if (record.present) presentDays++;
        }
        
        cout << "\nTotal Present Days: " << presentDays << endl;
        cout << "Total Working Days: " << personalAttendance.size() << endl;
    }
    
    void profile() {
        cout << "\n=== Employee Profile ===" << endl;
        cout << "Name: " << name << endl;
        cout << "User ID: " << userID << endl;
        cout << "Current Status: " << (isPresent ? "Present" : "Absent") << endl;
        cout << "Total Attendance Records: " << personalAttendance.size() << endl;
        cout << "Total Leave Requests: " << leaveHistory.size() << endl;
    }
    
    void return_add() {
        cout << "\nReturning to Employee Dashboard..." << endl;
    }
    
    // Friend function to allow admin access
    friend class Admin;
    friend void displayEmployeeDetails(const Employee& emp);
};

// Admin class
class Admin : public General {
private:
    vector<Employee*> managedEmployees;
    
public:
    Admin() {}
    Admin(string n, string id, string pass) : General(n, id, pass) {}
    
    // Override virtual functions
    void view_call() override {
        cout << "\n=== Admin Dashboard ===" << endl;
        cout << "Welcome, Administrator " << name << "!" << endl;
        cout << "User ID: " << userID << endl;
        cout << "Today's Date: " << Global::Today << endl;
        cout << "Total Employees: " << Global::emp_array.size() << endl;
        cout << "Pending Leave Requests: " << Global::Lreq.size() << endl;
    }
    
    void Calendar() override {
        cout << "\n=== System Calendar ===" << endl;
        cout << "Month: " << Global::TMonth << "/" << Global::TYear << endl;
        cout << "Total Days: " << Global::Mdays[stoi(Global::TMonth) - 1] << endl;
        cout << "System Holidays: ";
        for (const string& holiday : Global::holi) {
            cout << holiday << " ";
        }
        cout << endl;
    }
    
    bool abs() override {
        return false; // Admin is always considered present
    }
    
    // Admin specific functions
    void Register() {
        string empName, empID, empPass;
        cout << "\n=== Register New Employee ===" << endl;
        cout << "Enter Employee Name: ";
        cin.ignore();
        getline(cin, empName);
        cout << "Enter Employee ID: ";
        cin >> empID;
        cout << "Enter Password: ";
        cin >> empPass;
        
        // Check if employee already exists
        for (Employee* emp : Global::emp_array) {
            if (emp->getUserID() == empID) {
                cout << "Employee with this ID already exists!" << endl;
                return;
            }
        }
        
        Employee* newEmp = new Employee(empName, empID, empPass);
        Global::emp_array.push_back(newEmp);
        managedEmployees.push_back(newEmp);
        
        cout << "Employee registered successfully!" << endl;
    }
    
    void approve() {
        if (Global::Lreq.empty()) {
            cout << "No pending leave requests." << endl;
            return;
        }
        
        cout << "\n=== Approve Leave Requests ===" << endl;
        queue<LeaveRequest> tempQueue;
        
        while (!Global::Lreq.empty()) {
            LeaveRequest req = Global::Lreq.front();
            Global::Lreq.pop();
            
            cout << "\nEmployee: " << req.employeeName << " (ID: " << req.employeeID << ")" << endl;
            cout << "Leave Date: " << req.leaveDate << endl;
            cout << "Reason: " << req.reason << endl;
            cout << "Approve this request? (y/n): ";
            
            char choice;
            cin >> choice;
            
            if (choice == 'y' || choice == 'Y') {
                req.status = "Approved";
                cout << "Leave request approved." << endl;
            } else {
                req.status = "Rejected";
                cout << "Leave request rejected." << endl;
            }
            
            // Update employee's leave history
            for (Employee* emp : Global::emp_array) {
                if (emp->getUserID() == req.employeeID) {
                    for (auto& leaveReq : emp->leaveHistory) {
                        if (leaveReq.leaveDate == req.leaveDate && leaveReq.reason == req.reason) {
                            leaveReq.status = req.status;
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    
    void viewAtd() {
        cout << "\n=== View All Attendance ===" << endl;
        if (Global::emp_array.empty()) {
            cout << "No employees registered." << endl;
            return;
        }
        
        cout << left << setw(15) << "Employee ID" << setw(20) << "Name" << setw(15) << "Status" << "Records" << endl;
        cout << string(60, '-') << endl;
        
        for (Employee* emp : Global::emp_array) {
            cout << left << setw(15) << emp->getUserID() 
                 << setw(20) << emp->getName() 
                 << setw(15) << (emp->isPresent ? "Present" : "Absent")
                 << emp->personalAttendance.size() << endl;
        }
    }
    
    void viewPAtd() {
        string empID;
        cout << "Enter Employee ID to view attendance: ";
        cin >> empID;
        
        Employee* targetEmp = nullptr;
        for (Employee* emp : Global::emp_array) {
            if (emp->getUserID() == empID) {
                targetEmp = emp;
                break;
            }
        }
        
        if (targetEmp == nullptr) {
            cout << "Employee not found!" << endl;
            return;
        }
        
        cout << "\n=== Attendance for " << targetEmp->getName() << " ===" << endl;
        if (targetEmp->personalAttendance.empty()) {
            cout << "No attendance records found." << endl;
            return;
        }
        
        cout << left << setw(12) << "Date" << setw(10) << "Status" << setw(10) << "Time In" << "Time Out" << endl;
        cout << string(40, '-') << endl;
        
        for (const auto& record : targetEmp->personalAttendance) {
            cout << left << setw(12) << record.date 
                 << setw(10) << (record.present ? "Present" : "Absent")
                 << setw(10) << record.timeIn 
                 << record.timeOut << endl;
        }
    }
    
    void profile() {
        cout << "\n=== Admin Profile ===" << endl;
        cout << "Name: " << name << endl;
        cout << "User ID: " << userID << endl;
        cout << "Role: Administrator" << endl;
        cout << "Total Employees Managed: " << Global::emp_array.size() << endl;
        cout << "Pending Leave Requests: " << Global::Lreq.size() << endl;
    }
    
    void return_add() {
        cout << "\nReturning to Admin Dashboard..." << endl;
    }
    
    // Function to remove employee (using friend concept)
    void removeEmployee() {
        string empID;
        cout << "Enter Employee ID to remove: ";
        cin >> empID;
        
        auto it = find_if(Global::emp_array.begin(), Global::emp_array.end(),
                         [empID](Employee* emp) { return emp->getUserID() == empID; });
        
        if (it != Global::emp_array.end()) {
            delete *it;
            Global::emp_array.erase(it);
            cout << "Employee removed successfully!" << endl;
        } else {
            cout << "Employee not found!" << endl;
        }
    }
};

// Friend function implementation
void displayEmployeeDetails(const Employee& emp) {
    cout << "\n=== Employee Details (Friend Function Access) ===" << endl;
    cout << "Name: " << emp.name << endl;
    cout << "User ID: " << emp.userID << endl;
    cout << "Present Status: " << (emp.isPresent ? "Present" : "Absent") << endl;
    cout << "Total Attendance Records: " << emp.personalAttendance.size() << endl;
}

// Function overloading examples
class Detail {
public:
    // Overloaded functions for different parameter types
    void show(string name) {
        cout << "Employee Name: " << name << endl;
    }
    
    void show(string name, string id) {
        cout << "Employee Name: " << name << ", ID: " << id << endl;
    }
    
    void show(string name, string id, int records) {
        cout << "Employee Name: " << name << ", ID: " << id << ", Records: " << records << endl;
    }
};

// Main system class
class AttendanceSystem {
private:
    Admin systemAdmin;
    
public:
    AttendanceSystem() {
        Global::initializeDateTime();
        systemAdmin = Admin("System Administrator", "admin", "admin123");
    }
    
    void displayMainMenu() {
        cout << "\n" << string(50, '=') << endl;
        cout << "    OFFICE ATTENDANCE MANAGEMENT SYSTEM" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. Employee Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
    }
    
    void displayAdminMenu() {
        cout << "\n=== Admin Menu ===" << endl;
        cout << "1. Register Employee" << endl;
        cout << "2. Approve Leave Requests" << endl;
        cout << "3. View All Attendance" << endl;
        cout << "4. View Specific Employee Attendance" << endl;
        cout << "5. Remove Employee" << endl;
        cout << "6. View Profile" << endl;
        cout << "7. View Calendar" << endl;
        cout << "8. Logout" << endl;
        cout << "Enter your choice: ";
    }
    
    void displayEmployeeMenu() {
        cout << "\n=== Employee Menu ===" << endl;
        cout << "1. Mark Attendance" << endl;
        cout << "2. Request Leave" << endl;
        cout << "3. Check Leave Status" << endl;
        cout << "4. View Personal Attendance" << endl;
        cout << "5. View Profile" << endl;
        cout << "6. View Calendar" << endl;
        cout << "7. Logout" << endl;
        cout << "Enter your choice: ";
    }
    
    bool adminLogin() {
        string id, pass;
        cout << "\n=== Admin Login ===" << endl;
        cout << "Enter Admin ID: ";
        cin >> id;
        cout << "Enter Password: ";
        cin >> pass;
        
        if (id == systemAdmin.getUserID() && pass == systemAdmin.getPassword()) {
            Global::login_user = id;
            return true;
        }
        cout << "Invalid credentials!" << endl;
        return false;
    }
    
    Employee* employeeLogin() {
        string id, pass;
        cout << "\n=== Employee Login ===" << endl;
        cout << "Enter Employee ID: ";
        cin >> id;
        cout << "Enter Password: ";
        cin >> pass;
        
        for (Employee* emp : Global::emp_array) {
            if (emp->getUserID() == id && emp->getPassword() == pass) {
                Global::login_user = id;
                return emp;
            }
        }
        cout << "Invalid credentials!" << endl;
        return nullptr;
    }
    
    void adminSession() {
        systemAdmin.view_call();
        
        int choice;
        do {
            displayAdminMenu();
            cin >> choice;
            
            switch (choice) {
                case 1:
                    systemAdmin.Register();
                    break;
                case 2:
                    systemAdmin.approve();
                    break;
                case 3:
                    systemAdmin.viewAtd();
                    break;
                case 4:
                    systemAdmin.viewPAtd();
                    break;
                case 5:
                    systemAdmin.removeEmployee();
                    break;
                case 6:
                    systemAdmin.profile();
                    break;
                case 7:
                    systemAdmin.Calendar();
                    break;
                case 8:
                    cout << "Logging out..." << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        } while (choice != 8);
    }
    
    void employeeSession(Employee* emp) {
        emp->view_call();
        
        int choice;
        do {
            displayEmployeeMenu();
            cin >> choice;
            
            switch (choice) {
                case 1:
                    emp->present();
                    break;
                case 2:
                    emp->ask_leave();
                    break;
                case 3:
                    emp->lv_sts();
                    break;
                case 4:
                    emp->viewPPAtd();
                    break;
                case 5:
                    emp->profile();
                    break;
                case 6:
                    emp->Calendar();
                    break;
                case 7:
                    cout << "Logging out..." << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        } while (choice != 7);
    }
    
    void run() {
        int choice;
        do {
            displayMainMenu();
            cin >> choice;
            
            switch (choice) {
                case 1:
                    if (adminLogin()) {
                        adminSession();
                    }
                    break;
                case 2: {
                    Employee* emp = employeeLogin();
                    if (emp != nullptr) {
                        employeeSession(emp);
                    }
                    break;
                }
                case 3:
                    cout << "Thank you for using the system!" << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        } while (choice != 3);
        
        // Cleanup
        for (Employee* emp : Global::emp_array) {
            delete emp;
        }
        Global::emp_array.clear();
    }
};

// Main function
int main() {
    AttendanceSystem system;
    system.run();
    return 0;
}