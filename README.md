# 🏢 Office Attendance Management System

This project is a console-based Office Attendance Management System built using Object-Oriented Programming (OOP) principles in C++. It is designed to help streamline attendance tracking, leave management, and administrative tasks for employees and administrators.

## 🚀 Features

### 👨‍💼 Employee Interface

* View personal profile
* Mark daily attendance
* Request for leave
* Check leave approval status
* View monthly attendance summary

### 🛠 Admin Interface

* Add or remove employee records
* View attendance reports for all employees
* Approve or reject leave requests
* View employee profiles and leave history

## 🧠 Concepts & Technologies Used

* 💻 Language: C++
* 🧱 OOP Concepts:

  * Inheritance
  * Abstraction
  * Function overloading
  * Friend functions
* 📊 Data Structures:

  * Queue: Used for managing leave request queues
  * Hash Table (unordered\_map): Used for efficient access to daily attendance data
* 🗃 File Handling (optional): Can be extended to support persistent storage

## 🔒 Security & Access Control

* Proper abstraction is used to ensure employees cannot access admin-only data.
* Separate classes handle access control between Employee and Admin functionalities.

## 🔧 Future Improvements

* Add persistent storage using file handling or databases
* GUI using frameworks like Qt
* Generate attendance reports in CSV or PDF
* Email notifications for leave approvals

## Team Member

1. Tathya Prajapati - 202201170
2. Het Shah - 202201515
