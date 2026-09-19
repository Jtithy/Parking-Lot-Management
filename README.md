# 🚗 Parking Lot Management System

A **C-based Parking Lot Management System** designed to simplify vehicle registration, parking management, owner management, parking fee calculation, and report generation through an admin-controlled console application.

The system provides a structured way to register administrators, manage vehicle information, allocate parking spaces, calculate parking fees, monitor parking status, and generate parking reports.

---

## 📌 Project Overview

The **Parking Lot Management System** is a console-based application developed using the **C programming language**.

It is designed to manage a parking facility with:

* 👨‍💼 Admin management
* 👤 Vehicle owner management
* 🚗 Vehicle registration
* 🅿️ Parking space allocation
* 🚘 Vehicle parking and unparking
* 💰 Automatic parking fee calculation
* 📊 Parking status monitoring
* 📄 Parking report generation
* 💾 File-based data persistence
* ✅ Input validation

The application supports up to **10 administrators, 100 owners, 200 vehicles, and 50 parking spaces**. These limits are defined within the program constants.

---

## ✨ Features

### 👨‍💼 Admin Management

* Register new administrators
* Admin login authentication
* Password validation
* Admin data persistence
* Admin panel with controlled system access

### 🚗 Vehicle Management

* Add/register vehicles
* Automatically generate Vehicle IDs
* Store license plate information
* Store vehicle type
* Link vehicles with their owners
* View all registered vehicles
* Delete vehicles
* Check parking status

### 👤 Owner Management

* Automatically generate Owner IDs
* Store owner name and contact information
* Link owners with registered vehicles
* View all registered owners

### 🅿️ Parking Management

* 50 parking spaces
* Automatically find the first available parking space
* Park vehicles using Vehicle ID
* Record vehicle entry time
* Prevent already parked vehicles from being parked again
* Detect when no parking space is available
* Unpark vehicles
* Automatically make released spaces available again

### 💰 Parking Fee Calculation

The system calculates parking fees based on the vehicle's parking duration.

**Current rate:**

```text
100 TK per hour
```

A minimum charge of **1 hour** is applied even if the vehicle stays for less than one hour.

### 📊 Parking Status

The system displays:

* Total parking spaces
* Occupied spaces
* Available spaces
* Occupancy rate
* Individual parking-space status
* Vehicle ID
* License plate
* Parking duration

### 📄 Report Generation

The system can generate a detailed parking report containing:

* Total parking spaces
* Occupied spaces
* Available spaces
* Occupancy rate
* Estimated current revenue
* Registered vehicles
* Registered administrators
* Currently parked vehicles
* Parking duration

Reports are automatically saved inside the `reports` directory.

---

## 🛠️ Technologies Used

| Technology          | Purpose                                     |
| ------------------- | ------------------------------------------- |
| **C**               | Core application development                |
| **File Handling**   | Persistent data storage                     |
| **Structures**      | Admin, Owner, Vehicle and Parking Spot data |
| **Time Library**    | Entry time and parking duration             |
| **String Handling** | Data processing and validation              |
| **Standard I/O**    | Console-based user interaction              |

The project uses standard C libraries including:

```c
stdio.h
stdlib.h
string.h
time.h
ctype.h
```

It also includes platform-specific directory creation support for Windows and Unix-like systems.

---

## 📂 Project Structure

```text
Parking-Lot-Management/
│
├── main.c
├── a.exe
│
├── admin/
│   └── data.txt
│
├── owners/
│   └── data.txt
│
├── vehicles/
│   └── data.txt
│
├── parking/
│   └── data.txt
│
└── reports/
    └── report_*.txt
```

The required folders are automatically created when the program starts.

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/Jtithy/Parking-Lot-Management.git
```

### 2. Navigate to the Project

```bash
cd Parking-Lot-Management
```

### 3. Compile the Program

If you are using GCC:

```bash
gcc main.c -o parking
```

### 4. Run the Program

On Windows:

```bash
parking.exe
```

On Linux/macOS:

```bash
./parking
```

---

## 🖥️ Application Flow

```text
                    ┌──────────────────────┐
                    │  Parking Lot System  │
                    └──────────┬───────────┘
                               │
                    ┌──────────▼───────────┐
                    │      Main Menu       │
                    └──────────┬───────────┘
                               │
              ┌────────────────┴────────────────┐
              │                                 │
      ┌───────▼────────┐                ┌──────▼───────┐
      │ Admin Register │                │  Admin Login │
      └────────────────┘                └──────┬───────┘
                                               │
                                      ┌────────▼────────┐
                                      │   Admin Panel   │
                                      └────────┬────────┘
                                               │
             ┌───────────────┬─────────────────┼────────────────┐
             │               │                 │                │
       ┌─────▼─────┐   ┌────▼─────┐    ┌─────▼─────┐    ┌────▼─────┐
       │ Vehicles  │   │  Status  │    │  Reports  │    │  Owners  │
       └─────┬─────┘   └──────────┘    └───────────┘    └──────────┘
             │
       ┌─────┴─────────────┐
       │                   │
  ┌────▼─────┐       ┌─────▼─────┐
  │   Park   │       │   Unpark  │
  └──────────┘       └───────────┘
```

---

## 📋 Main Menu

```text
========== PARKING LOT MANAGEMENT SYSTEM ==========

1. Admin Login
2. Register as Admin
3. Exit
```

After successful login, the administrator gets access to:

```text
========== ADMIN PANEL ==========

1. Manage Vehicles
2. View Parking Status
3. Generate Report
4. View All Owners
5. Logout
```

---

## 🚘 Vehicle Management Menu

```text
========== MANAGE VEHICLES ==========

1. View All Vehicles
2. Add Vehicles
3. Park Vehicle
4. Unpark Vehicle
5. Delete Vehicles
6. Back to Admin Menu
```

---

## 💳 Parking Process

### Step 1 — Register a Vehicle

The administrator provides:

* License plate
* Vehicle type
* Owner name
* Owner phone number

The system automatically generates:

```text
Owner ID: OWN0001
Vehicle ID: VH0001
```

### Step 2 — Park the Vehicle

The administrator enters the Vehicle ID.

The system:

1. Searches for the vehicle.
2. Checks whether it is already parked.
3. Finds an available parking spot.
4. Assigns the spot.
5. Records the entry time.
6. Saves the updated parking information.

### Step 3 — Unpark the Vehicle

When the vehicle leaves, the system:

1. Finds the vehicle.
2. Calculates the parking duration.
3. Calculates the parking fee.
4. Releases the parking spot.
5. Updates the stored data.
6. Displays the final fee.

---

## 💰 Fee Calculation

The current parking rate is:

```text
100 TK / hour
```

For example:

```text
Parking Duration = 3 hours
Rate             = 100 TK/hour

Total Fee        = 3 × 100
                 = 300 TK
```

If the vehicle stays for less than one hour, the minimum charge is one hour.

---

## 💾 Data Storage

The application uses **text files for persistent storage** instead of a database.

Data is organized into separate directories:

```text
admin/data.txt
owners/data.txt
vehicles/data.txt
parking/data.txt
reports/report_*.txt
```

When the application starts, previously stored information is loaded automatically.

> **Note:** The current implementation writes data using append-mode file operations. For production use, the storage layer could be improved by replacing this approach with a database or controlled file rewriting.

---

## 🔐 Input Validation

The system includes validation for several types of input:

* Admin name
* Email
* Phone number
* Password
* License plate
* Numeric menu selections

For example, administrator passwords must contain at least **8 characters**, while phone numbers are validated for an 11-digit format.

---

## 📊 System Capacity

| Resource       | Maximum |
| -------------- | ------: |
| Administrators |      10 |
| Owners         |     100 |
| Vehicles       |     200 |
| Parking Spaces |      50 |

These limits can be modified in `main.c` by changing the corresponding constants.

---

## 🧠 Concepts Demonstrated

This project demonstrates several fundamental C programming concepts:

* Structures (`struct`)
* Arrays
* Functions
* Function prototypes
* File handling
* String manipulation
* Input validation
* Searching
* Time/date handling
* Conditional statements
* Loops
* Modular program organization
* Persistent data management

---

## 🔮 Future Improvements

Possible future improvements include:

* 🗄️ MySQL/PostgreSQL database integration
* 🖥️ Graphical User Interface
* 🌐 Web-based parking management
* 📱 Mobile application
* 🔐 Password hashing and stronger authentication
* 🎫 Digital parking tickets
* 💳 Online payment integration
* 📈 Advanced revenue analytics
* 🔔 Parking availability notifications
* 📷 License plate recognition
* 👥 Multiple admin roles and permissions
* ☁️ Cloud-based data storage

---

## 👨‍💻 Authors

**Jeb-Un-Nesa Tithy**
**Nishat Biswas Pranto**

Developed as an academic C programming project.

---

## 📜 License

This project is available for educational and learning purposes.

---

## ⭐ Support

If you find this project useful, consider giving the repository a ⭐ on GitHub.

**Repository:**
[Parking-Lot-Management](https://github.com/Jtithy/Parking-Lot-Management?utm_source=chatgpt.com)
