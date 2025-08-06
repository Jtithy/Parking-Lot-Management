#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Constants
#define MAX_ADMINS 10
#define MAX_USERS 100
#define MAX_VEHICLES 200
#define MAX_PARKING_SPOTS 50
#define NAME_LEN 50
#define EMAIL_LEN 30
#define CONTACT_LEN 12
#define PASSWORD_LEN 50
#define VEHICLE_ID_LEN 20
#define LICENSE_PLATE_LEN 15
#define VEHICLE_TYPE_LEN 20

// For Admin
typedef struct {          
    char name[NAME_LEN];
    char phoneNumber[CONTACT_LEN];
    char email[EMAIL_LEN];
    char password[PASSWORD_LEN];
} Admin;

// For User
typedef struct {          
    char name[NAME_LEN];
    char vehicleID[VEHICLE_ID_LEN];
    char phoneNumber[CONTACT_LEN];
    int userId;
} User;

// For Vehicle
typedef struct {           
    char vehicleId[VEHICLE_ID_LEN];
    char licensePlate[LICENSE_PLATE_LEN];
    char ownerName[NAME_LEN];
    char ownerPhoneNumber[CONTACT_LEN];
    char vehicleType[VEHICLE_TYPE_LEN];
    int isParked;
    int spotNumber;
    int ownerId;
    time_t entryTime;
} Vehicle;

// For Parking Spot
typedef struct {           
    int spotNumber;
    int isOccupied;
    char vehicleId[VEHICLE_ID_LEN];
    time_t entryTime;
    double parkingFee;
} ParkingSpot;

// Global variables
Admin admins[MAX_ADMINS];     
User users[MAX_USERS];
Vehicle vehicles[MAX_VEHICLES];
ParkingSpot spots[MAX_PARKING_SPOTS];
int numAdmins = 0;
int numUsers = 0;
int numVehicles = 0;
int nextUserId = 1001;
int currentLoggedUserId = -1;

// Function prototypes
void initializeSystem();     
void mainMenu();
void adminMenu();
int loginAdmin();
void registerAdmin();
void saveAdminData();
void saveUserData();
void saveVehicleData();
void saveParkingData();
void loadAdminData();
void loadUserData();
void loadVehicleData();
void loadParkingData();
void addVehicle();
void parkVehicle();
void unparkVehicle();
void displayParkingStatus();
void generateReport();
void manageUsers();
void manageVehicles();
void viewAllUsers();
void viewAllVehicles();
void deleteUser();
void deleteVehicle();
double calculateParkingFee(time_t entryTime);

// Validation functions
int isValidName(char *name);        
int isValidEmail(char *email);
int isValidPhoneNumber(char *phone);
int isValidPassword(char *password);
int isValidLicensePlate(char *plate);
void clearInputBuffer();
void createDirectories();
int findUserById(char *userId);
int findVehicleById(char *vehicleId);
int findAvailableSpot();
void generateVehicleId(char *vehicleId);

// Debugging function
void debugShowAllAdmins();         

int main() {
    initializeSystem();
    mainMenu();
    return 0;
}

// Initialize the system
void initializeSystem() {          
    for (int i = 0; i < MAX_PARKING_SPOTS; i++) {
        spots[i].spotNumber = i + 1;
        spots[i].isOccupied = 0;
        strcpy(spots[i].vehicleId, "");
        spots[i].entryTime = 0;
        spots[i].parkingFee = 0.0;
    }
    createDirectories();
    loadAdminData();
    loadUserData();
    loadVehicleData();
    loadParkingData();

    printf("=== Parking Lot Management System Initialized ===\n");
    printf("Total Parking Spots: %d\n", MAX_PARKING_SPOTS);
    printf("Registered Admins: %d\n", numAdmins);
    printf("Registered Vehicles: %d\n", numVehicles);
}

// Create necessary directories
void createDirectories() {       
    #ifdef _WIN32
        system("mkdir admin >nul 2>&1");
        system("mkdir users >nul 2>&1");
        system("mkdir vehicles >nul 2>&1");
        system("mkdir parking >nul 2>&1");
        system("mkdir reports >nul 2>&1");
    #else
        system("mkdir -p admin users vehicles parking reports 2>/dev/null");
    #endif
}

// Main menu for the system
void mainMenu() {          
    int choice;
    while (1) {
        printf("\n========== PARKING LOT MANAGEMENT SYSTEM ==========\n");
        printf("1. Admin Login\n");
        printf("2. Register as Admin\n");
        printf("3. Exit\n");
        printf("===================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                if (loginAdmin()) {
                    adminMenu();
                }
                break;
            case 2:
                registerAdmin();
                break;
            case 3:
                printf("Goodbye.!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Register new admin
void registerAdmin() {          
    if (numAdmins >= MAX_ADMINS) {
        printf("ERROR: Maximum number of admins reached.\n");
        return;
    }
    printf("\n------- Admin Registration -------\n");
    char temp_input[200];

    // Get admin name
    while (1) {            
        printf("Enter Admin Name: ");
        if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // Remove newline character
        temp_input[strcspn(temp_input, "\n")] = 0;        

        // Trim leading spaces
        char *start = temp_input;                         
        while (*start == ' ') start++;
        char *end = start + strlen(start) - 1;
        while (end > start && *end == ' ') end--;
        *(end + 1) = 0;

        if (isValidName(start)) {
            strcpy(admins[numAdmins].name, start);
            break;
        } else {
            printf("Invalid name. Please enter a valid name (letters and spaces only).\n");
        }
    }

    // Get phone number
    while (1) {             
        printf("Enter Phone Number (exactly 11 digits starting with '01'): ");
        if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        temp_input[strcspn(temp_input, "\n")] = 0;

        if (isValidPhoneNumber(temp_input)) {
            strcpy(admins[numAdmins].phoneNumber, temp_input);
            break;
        } else {
            printf("Invalid phone number. Must be exactly 11 digits starting with '01'.\n");
        }
    }

    // Get email
    while (1) {            
        printf("Enter Email: ");
        if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        temp_input[strcspn(temp_input, "\n")] = 0;

        if (isValidEmail(temp_input)) {
            strcpy(admins[numAdmins].email, temp_input);
            break;
        } else {
            printf("Invalid email format. Please enter a valid email.\n");
        }
    }

    // Get password
    while (1) {            
        printf("Enter Password (at least 8 characters): ");
        if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        temp_input[strcspn(temp_input, "\n")] = 0;

        if (isValidPassword(temp_input)) {
            strcpy(admins[numAdmins].password, temp_input);
            break;
        } else {
            printf("Invalid password. Must be at least 8 characters long.\n");
        }
    }
    numAdmins++;
    saveAdminData();

    printf("\n*** ADMIN REGISTRATION SUCCESSFUL! ***\n");
    printf("Admin registered: '%s'\n", admins[numAdmins-1].name);
    printf("You can now login.\n");
}

// Admin login function
int loginAdmin() {               
    if (numAdmins == 0) {
        printf("ERROR: No admins registered. Please register first.\n");
        return 0;
    }
    printf("\n------- Admin Login -------\n");
    
    char input_name[NAME_LEN];
    char input_password[PASSWORD_LEN];

    // Get admin name
    printf("Enter Admin Name: ");      
    if (fgets(input_name, sizeof(input_name), stdin) == NULL) {
        printf("Error reading name.\n");
        return 0;
    }
    input_name[strcspn(input_name, "\n")] = 0;

    // Trim leading spaces
    char *start = input_name;         
    while (*start == ' ') start++;
    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;
    *(end + 1) = 0;
    strcpy(input_name, start);

    // Get admin password
    printf("Enter Password: ");        
    if (fgets(input_password, sizeof(input_password), stdin) == NULL) {
        printf("Error reading password.\n");
        return 0;
    }
    input_password[strcspn(input_password, "\n")] = 0;

    for (int i = 0; i < numAdmins; i++) {           
        if (strcmp(admins[i].name, input_name) == 0 &&
            strcmp(admins[i].password, input_password) == 0) {
            printf("Login successful! Welcome, %s!\n", admins[i].name);
            return 1;
        }
    }
    printf("ERROR: No matching admin found.\n");
    printf("Please check your username and password.\n");
    return 0;
}

// Admin menu function
void adminMenu() {                  
    int choice;
    while (1) {
        printf("\n========== ADMIN PANEL ==========\n");
        printf("1. Manage Vehicles\n");
        printf("2. View Parking Status\n");
        printf("3. Generate Report\n");
        printf("4. View All Users\n");
        printf("5. Logout\n");
        printf("=================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        switch (choice) {
            case 1:
                manageVehicles();
                break;
            case 2:
                displayParkingStatus();
                break;
            case 3:
                generateReport();
                break;
            case 4:
                viewAllUsers();
                break;
            case 5:
                printf("Logged out successfully.\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Add new vehicle
void addVehicle() {          

    char vehicleId[VEHICLE_ID_LEN];
    char licensePlate[LICENSE_PLATE_LEN];
    char ownerName[NAME_LEN];
    char vehicleType[VEHICLE_TYPE_LEN];
    int ownerId;
    char temp_input[200];

    // Get license plate
    printf("Enter License Plate: ");       
    fgets(licensePlate, sizeof(licensePlate), stdin);
    licensePlate[strcspn(licensePlate, "\n")] = 0;

    if (!isValidLicensePlate(licensePlate)) {
        printf("Invalid license plate.\n");
        return;
    }

    // Get vehicle type
    printf("Enter Vehicle Type (Car/Bike/etc): ");       
    fgets(vehicleType, sizeof(vehicleType), stdin);
    vehicleType[strcspn(vehicleType, "\n")] = 0;

    // Get owner name
    while (1) {                     
        printf("Enter Owner Name: ");
        if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // Remove newline character
        temp_input[strcspn(temp_input, "\n")] = 0;       

        // Trim leading spaces
        char *start = temp_input;               
        while (*start == ' ') start++;
        char *end = start + strlen(start) - 1;
        while (end > start && *end == ' ') end--;
        *(end + 1) = 0;

        if (isValidName(start)) {
            strcpy(users[numUsers].name, start);
            break;
        } else {
            printf("Invalid name. Please enter a valid name (letters and spaces only).\n");
        }
    }
    users[numUsers].userId = nextUserId++;

    // Get owner phone number
    while (1) {                      
        printf("Enter Phone Number (exactly 11 digits starting with '01'): ");
        if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        temp_input[strcspn(temp_input, "\n")] = 0;

        if (isValidPhoneNumber(temp_input)) {
            strcpy(users[numUsers].phoneNumber, temp_input);
            break;
        } else {
            printf("Invalid phone number. Must be exactly 11 digits starting with '01'.\n");
        }
    }

    // Generate unique vehicle ID
    generateVehicleId(vehicleId);           
    strcpy(vehicles[numVehicles].vehicleId, vehicleId);
    strcpy(vehicles[numVehicles].licensePlate, licensePlate);
    strcpy(vehicles[numVehicles].ownerName, users[numUsers].name);
    strcpy(vehicles[numVehicles].vehicleType, vehicleType);
    strcpy(vehicles[numVehicles].ownerPhoneNumber, users[numUsers].phoneNumber);
    vehicles[numVehicles].isParked = 0;
    vehicles[numVehicles].spotNumber = 0;
    vehicles[numVehicles].entryTime = 0;
    

    numVehicles++;
    saveVehicleData();

    printf("Vehicle added successfully. ID: %s\n", vehicleId);
}

// Park a vehicle
void parkVehicle() {           
    if (numVehicles == 0) {
        printf("No vehicles registered.\n");
        return;
    }
    printf("\n------- Park Vehicle -------\n");

    char vehicleId[VEHICLE_ID_LEN];
    printf("Enter Vehicle ID to park: ");
    if (fgets(vehicleId, sizeof(vehicleId), stdin) == NULL) {
        printf("Error reading vehicle ID.\n");
        return;
    }
    vehicleId[strcspn(vehicleId, "\n")] = 0;

    int vehicleIndex = findVehicleById(vehicleId);
    if (vehicleIndex == -1) {
        printf("ERROR: Vehicle not found.\n");
        return;
    }

    if (vehicles[vehicleIndex].isParked) {
        printf("ERROR: Vehicle is already parked in spot %d.\n", vehicles[vehicleIndex].spotNumber);
        return;
    }

    int availableSpot = findAvailableSpot();
    if (availableSpot == -1) {
        printf("ERROR: No available parking spots.\n");
        return;
    }

    // Mark vehicle as parked
    vehicles[vehicleIndex].isParked = 1;             
    vehicles[vehicleIndex].spotNumber = availableSpot;
    vehicles[vehicleIndex].entryTime = time(NULL);

    spots[availableSpot - 1].isOccupied = 1;
    strcpy(spots[availableSpot - 1].vehicleId, vehicleId);
    spots[availableSpot - 1].entryTime = time(NULL);
    saveVehicleData();
    saveParkingData();

    printf("*** VEHICLE PARKED SUCCESSFULLY! ***\n");
    printf("Vehicle ID: %s\n", vehicleId);
    printf("License Plate: %s\n", vehicles[vehicleIndex].licensePlate);
    printf("Parking Spot: %d\n", availableSpot);
    printf("Entry Time: %s", ctime(&vehicles[vehicleIndex].entryTime));
}

// Unpark a vehicle
void unparkVehicle() {              
    printf("\n------- Unpark Vehicle -------\n");

    char vehicleId[VEHICLE_ID_LEN];
    printf("Enter Vehicle ID to unpark: ");
    if (fgets(vehicleId, sizeof(vehicleId), stdin) == NULL) {
        printf("Error reading vehicle ID.\n");
        return;
    }
    vehicleId[strcspn(vehicleId, "\n")] = 0;

    int vehicleIndex = findVehicleById(vehicleId);
    if (vehicleIndex == -1) {
        printf("ERROR: Vehicle not found.\n");
        return;
    }

    if (!vehicles[vehicleIndex].isParked) {
        printf("ERROR: Vehicle is not currently parked.\n");
        return;
    }
    int spotNumber = vehicles[vehicleIndex].spotNumber;
    double parkingFee = calculateParkingFee(vehicles[vehicleIndex].entryTime);

    // Mark vehicle as unparked
    vehicles[vehicleIndex].isParked = 0;               
    int oldSpotNumber = vehicles[vehicleIndex].spotNumber;
    vehicles[vehicleIndex].spotNumber = 0;
    vehicles[vehicleIndex].entryTime = 0;

    spots[oldSpotNumber - 1].isOccupied = 0;
    strcpy(spots[oldSpotNumber - 1].vehicleId, "");
    spots[oldSpotNumber - 1].entryTime = 0;
    spots[oldSpotNumber - 1].parkingFee = parkingFee;
    saveVehicleData();
    saveParkingData();

    printf("*** VEHICLE UNPARKED SUCCESSFULLY! ***\n");
    printf("Vehicle ID: %s\n", vehicleId);
    printf("License Plate: %s\n", vehicles[vehicleIndex].licensePlate);
    printf("Parking Spot: %d (now available)\n", spotNumber);
    printf("Parking Fee: $%.2f\n", parkingFee);
    printf("Thank you for using our parking service!\n");
}

// Display current parking status
void displayParkingStatus() {                  
    printf("\n========== PARKING STATUS ==========\n");

    int occupied = 0;
    for (int i = 0; i < MAX_PARKING_SPOTS; i++) {
        if (spots[i].isOccupied) occupied++;
    }
    printf("Total Spots: %d\n", MAX_PARKING_SPOTS);
    printf("Occupied: %d\n", occupied);
    printf("Available: %d\n", MAX_PARKING_SPOTS - occupied);
    printf("Occupancy Rate: %.1f%%\n", (float)occupied / MAX_PARKING_SPOTS * 100);
    printf("\n--- Spot Details ---\n");
    printf("%-4s %-10s %-15s %-20s %-15s\n", "Spot", "Status", "Vehicle ID", "License Plate", "Duration");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < MAX_PARKING_SPOTS; i++) {
        printf("%-4d %-10s", spots[i].spotNumber,
               spots[i].isOccupied ? "OCCUPIED" : "AVAILABLE");
        if (spots[i].isOccupied) {
            int vehicleIndex = findVehicleById(spots[i].vehicleId);
            if (vehicleIndex != -1) {
                time_t currentTime = time(NULL);
                double duration = difftime(currentTime, spots[i].entryTime) / 3600.0; // in hours
                printf(" %-15s %-20s %.1f hrs",
                       spots[i].vehicleId,
                       vehicles[vehicleIndex].licensePlate,
                       duration);
            }
        }
        printf("\n");
    }
}

// Generate parking report
void generateReport() {           
    printf("\n========== PARKING REPORT ==========\n");

    time_t currentTime = time(NULL);
    char filename[100];
    sprintf(filename, "reports/report_%ld.txt", currentTime);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error creating report file.\n");
        return;
    }

    fprintf(fp, "=== PARKING LOT MANAGEMENT REPORT ===\n");       
    fprintf(fp, "Generated on: %s\n", ctime(&currentTime));
    fprintf(fp, "==========================================\n\n");

    // Count occupied spots and calculate revenue
    int occupied = 0;             
    double totalRevenue = 0.0;

    for (int i = 0; i < MAX_PARKING_SPOTS; i++) {
        if (spots[i].isOccupied) {
            occupied++;
            totalRevenue += calculateParkingFee(spots[i].entryTime);
        }
    }
    fprintf(fp, "SYSTEM STATISTICS:\n");
    fprintf(fp, "Total Parking Spots: %d\n", MAX_PARKING_SPOTS);
    fprintf(fp, "Occupied Spots: %d\n", occupied);
    fprintf(fp, "Available Spots: %d\n", MAX_PARKING_SPOTS - occupied);
    fprintf(fp, "Occupancy Rate: %.1f%%\n", (float)occupied / MAX_PARKING_SPOTS * 100);
    fprintf(fp, "Estimated Current Revenue: TK- %.2f/=\n\n", totalRevenue);
    fprintf(fp, "USER STATISTICS:\n");
    fprintf(fp, "Registered Vehicles: %d\n", numVehicles);
    fprintf(fp, "Registered Admins: %d\n\n", numAdmins);
    // Write currently parked vehicles`
    fprintf(fp, "CURRENTLY PARKED VEHICLES:\n");              
    fprintf(fp, "%-4s %-15s %-15s %-20s %-10s %-15s\n",
            "Spot", "Vehicle ID", "License", "Owner", "Type", "Duration(hrs)");
    fprintf(fp, "--------------------------------------------------------------------------------\n");

    for (int i = 0; i < MAX_PARKING_SPOTS; i++) {
        if (spots[i].isOccupied) {
            int vehicleIndex = findVehicleById(spots[i].vehicleId);
            if (vehicleIndex != -1) {
                double duration = difftime(currentTime, spots[i].entryTime) / 3600.0;
                fprintf(fp, "%-4d %-15s %-15s %-20s %-10s %.1f\n",
                        spots[i].spotNumber,
                        spots[i].vehicleId,
                        vehicles[vehicleIndex].licensePlate,
                        vehicles[vehicleIndex].ownerName,
                        vehicles[vehicleIndex].vehicleType,
                        duration);
            }
        }
    }

    fclose(fp);

    printf("Report generated successfully!\n");
    printf("Report saved as: %s\n", filename);
    // Display summary on screen
    printf("\nREPORT SUMMARY:\n");                   
    printf("Total Spots: %d\n", MAX_PARKING_SPOTS);
    printf("Occupied: %d\n", occupied);
    printf("Available: %d\n", MAX_PARKING_SPOTS - occupied);
    printf("Occupancy Rate: %.1f%%\n", (float)occupied / MAX_PARKING_SPOTS * 100);
    printf("Current Revenue: TK- %.2f/=\n", totalRevenue);
}

// View all registered users
void viewAllUsers() {              
    printf("\n========== ALL USERS ==========\n");
    if (numUsers == 0)
    {
        printf("No users to display.\n");
        return;
    }

    printf("\n===== Registered Users =====\n");
    printf("%-5s %-15s %-25s %-15s %-15s\n", "No.", "User ID", "Name", "Contact", "Vehicle ID");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < numUsers; i++)
    {
        printf("%-5s %-15s %-25s %-15s %-15s\n",
               i + 1,
               users[i].userId,
               users[i].name,
               users[i].phoneNumber,
               users[i].vehicleID);
    }

}

void viewAllVehicles() {
    printf("\n========== ALL VEHICLES ==========\n");
    if (numVehicles == 0) {
        printf("No vehicles registered.\n");
        return;
    }

    printf("%-15s %-15s %-20s %-15s %-8s %-5s %-11s\n",
           "Vehicle ID", "License", "Owner", "Type", "Parked", "Spot", "Owner Phone Number");
    printf("-------------------------------------------------------------------------------------\n");

    for (int i = 0; i < numVehicles; i++) {
        printf("%-15s %-15s %-20s %-15s %-8s %-5d %-11s\n",
               vehicles[i].vehicleId,
               vehicles[i].licensePlate,
               vehicles[i].ownerName,
               vehicles[i].vehicleType,
               vehicles[i].isParked ? "Yes" : "No",
               vehicles[i].spotNumber,
               vehicles[i].ownerPhoneNumber);
    }
}

// Manage vehicles menu
void manageVehicles() {
    int choice;
    printf("\n========== MANAGE VEHICLES ==========\n");
    printf("1. View All Vehicles\n");
    printf("2. Add Vehicles\n");
    printf("3. Park Vehicle\n");
    printf("4. Unpark Vehicle\n");
    printf("5. Delete Vehicles\n");
    printf("6. Back to Admin Menu\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    switch (choice) {
        case 1:
            viewAllVehicles();
            break;
        case 2:
            addVehicle();
            break;
        case 3:
            parkVehicle();
            break;
        case 4:
            unparkVehicle();
            break;
        case 5:
            deleteVehicle();
            break;
        case 6:
            return;
        default:
            printf("Invalid choice.\n");
    }
}


// Delete Vehicle
void deleteVehicle() {
    if (numVehicles == 0) {
        printf("No vehicles to delete.\n");
        return;
    }

    viewAllVehicles();

    char vehicleId[VEHICLE_ID_LEN];
    printf("Enter Vehicle ID to delete: ");
    if (fgets(vehicleId, sizeof(vehicleId), stdin) == NULL) {
        printf("Error reading vehicle ID.\n");
        return;
    }
    vehicleId[strcspn(vehicleId, "\n")] = 0;

    int vehicleIndex = findVehicleById(vehicleId);
    if (vehicleIndex == -1) {
        printf("Vehicle not found.\n");
        return;
    }

    printf("Are you sure you want to delete vehicle %s? (y/n): ", vehicleId);
    char confirm;
    scanf(" %c", &confirm);
    clearInputBuffer();

    if (confirm == 'y' || confirm == 'Y') {
        if (vehicles[vehicleIndex].isParked) {
            spots[vehicles[vehicleIndex].spotNumber - 1].isOccupied = 0;
            strcpy(spots[vehicles[vehicleIndex].spotNumber - 1].vehicleId, "");
        }

        // Shift vehicles array
        for (int i = vehicleIndex; i < numVehicles - 1; i++) {
            vehicles[i] = vehicles[i + 1];
        }
        numVehicles--;

        saveVehicleData();
        saveParkingData();
        printf("Vehicle deleted successfully.\n");
    } else {
        printf("Vehicle deletion cancelled.\n");
    }
}

void debugShowAllAdmins() {
    printf("\n========== DEBUG: ALL ADMINS ==========\n");
    printf("Total admins: %d\n", numAdmins);

    if (numAdmins == 0) {
        printf("No admins registered.\n");
        return;
    }

    for (int i = 0; i < numAdmins; i++) {
        printf("Admin %d:\n", i+1);
        printf("  Name: '%s'\n", admins[i].name);
        printf("  Phone: '%s'\n", admins[i].phoneNumber);
        printf("  Email: '%s'\n", admins[i].email);
        printf("  Password: '%s'\n", admins[i].password);
        printf("---\n");
    }
}

// Data saving functions

// Admin Data
void saveAdminData() {
    FILE *fp = fopen("admin/data.txt", "w");
    if (fp == NULL) {
        printf("ERROR: Cannot create/open admin data file!\n");
        return;
    }

    fprintf(fp, "%d\n", numAdmins);
    for (int i = 0; i < numAdmins; i++) {
        fprintf(fp, "%s|%s|%s|%s\n",
                admins[i].name,
                admins[i].phoneNumber,
                admins[i].email,
                admins[i].password);
    }

    fclose(fp);
}

// User Data
void saveUserData() {
    FILE *fp = fopen("users/data.txt", "w");
    if (fp == NULL) {
        printf("ERROR: Cannot create/open user data file!\n");
        return;
    }
//fprintf(fp, "%s %s %s %s\n", userID, name, contact, vehicleID);
    fprintf(fp, "%d\n", numUsers);
    for (int i = 0; i < numUsers; i++) {
        fprintf(fp, "%s|%s|%s|%s\n",
                users[i].userId,
                users[i].name,
                users[i].phoneNumber,
                users[i].vehicleID);
    }

    fclose(fp);
}

// Vehicle Data
void saveVehicleData() {
    FILE *fp = fopen("vehicles/data.txt", "w");
    if (fp == NULL) {
        printf("ERROR: Cannot create/open vehicle data file!\n");
        return;
    }

    fprintf(fp, "%d\n", numVehicles);
    for (int i = 0; i < numVehicles; i++) {
        fprintf(fp, "%s|%s|%s|%s|%d|%d|%s|%ld\n",
                vehicles[i].vehicleId,
                vehicles[i].licensePlate,
                vehicles[i].ownerName,
                vehicles[i].vehicleType,
                vehicles[i].isParked,
                vehicles[i].spotNumber,
                vehicles[i].ownerPhoneNumber,
                vehicles[i].entryTime);
    }

    fclose(fp);
}

// Parking Data
void saveParkingData() {
    FILE *fp = fopen("parking/data.txt", "w");
    if (fp == NULL) {
        printf("ERROR: Cannot create/open parking data file!\n");
        return;
    }

    for (int i = 0; i < MAX_PARKING_SPOTS; i++) {
        fprintf(fp, "%d|%d|%s|%ld|%.2f\n",
                spots[i].spotNumber,
                spots[i].isOccupied,
                spots[i].vehicleId,
                spots[i].entryTime,
                spots[i].parkingFee);
    }

    fclose(fp);
}

// Data loading functions

// Admin Data
void loadAdminData() {
    FILE *fp = fopen("admin/data.txt", "r");
    if (fp == NULL) {
        numAdmins = 0;
        return;
    }

    if (fscanf(fp, "%d", &numAdmins) != 1) {
        numAdmins = 0;
        fclose(fp);
        return;
    }

    char line[500];
    fgets(line, sizeof(line), fp); // consume newline

    for (int i = 0; i < numAdmins; i++) {
        if (fgets(line, sizeof(line), fp) == NULL) break;

        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, "|");
        if (token) strcpy(admins[i].name, token);

        token = strtok(NULL, "|");
        if (token) strcpy(admins[i].phoneNumber, token);

        token = strtok(NULL, "|");
        if (token) strcpy(admins[i].email, token);

        token = strtok(NULL, "|");
        if (token) strcpy(admins[i].password, token);
    }

    fclose(fp);
}

// User Data
void loadUserData() {
    FILE *fp = fopen("users/data.txt", "r");
    if (fp == NULL) {
        numUsers = 0;
        return;
    }

    if (fscanf(fp, "%d", &numUsers) != 1) {
        numUsers = 0;
        fclose(fp);
        return;
    }

    char line[300];
    fgets(line, sizeof(line), fp); // consume newline

    for (int i = 0; i < numUsers; i++) {
        if (fgets(line, sizeof(line), fp) == NULL) break;

        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, "|");
        if (token) strcpy(users[i].name, token);

        token = strtok(NULL, "|");
        if (token) strcpy(users[i].phoneNumber, token);

        token = strtok(NULL, "|");
        if (token) strcpy(users[i].vehicleID, token);
    }

    fclose(fp);
}

// Vehicle Data
void loadVehicleData() {
    FILE *fp = fopen("vehicles/data.txt", "r");
    if (fp == NULL) {
        numVehicles = 0;
        return;
    }

    if (fscanf(fp, "%d", &numVehicles) != 1) {
        numVehicles = 0;
        fclose(fp);
        return;
    }

    char line[500];
    fgets(line, sizeof(line), fp); // consume newline

    for (int i = 0; i < numVehicles; i++) {
        if (fgets(line, sizeof(line), fp) == NULL) break;

        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, "|");
        if (token) strcpy(vehicles[i].vehicleId, token);

        token = strtok(NULL, "|");
        if (token) strcpy(vehicles[i].licensePlate, token);

        token = strtok(NULL, "|");
        if (token) strcpy(vehicles[i].ownerName, token);

        token = strtok(NULL, "|");
        if (token) strcpy(vehicles[i].vehicleType, token);

        token = strtok(NULL, "|");
        if (token) vehicles[i].isParked = atoi(token);

        token = strtok(NULL, "|");
        if (token) vehicles[i].spotNumber = atoi(token);

        token = strtok(NULL, "|");
        if (token) vehicles[i].ownerId = atoi(token);

        token = strtok(NULL, "|");
        if (token) vehicles[i].entryTime = atol(token);
    }

    fclose(fp);
}

void loadParkingData() {
    FILE *fp = fopen("parking/data.txt", "r");
    if (fp == NULL) {
        return;
    }

    char line[200];
    int i = 0;

    while (fgets(line, sizeof(line), fp) != NULL && i < MAX_PARKING_SPOTS) {
        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, "|");
        if (token) spots[i].spotNumber = atoi(token);

        token = strtok(NULL, "|");
        if (token) spots[i].isOccupied = atoi(token);

        token = strtok(NULL, "|");
        if (token) strcpy(spots[i].vehicleId, token);

        token = strtok(NULL, "|");
        if (token) spots[i].entryTime = atol(token);

        token = strtok(NULL, "|");
        if (token) spots[i].parkingFee = atof(token);

        i++;
    }

    fclose(fp);
}

// Utility functions
int isValidName(char *name) {
    int len = strlen(name);
    if (len < 2 || len >= NAME_LEN) return 0;

    for (int i = 0; i < len; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int isValidEmail(char *email) {
    int len = strlen(email);
    if (len < 5 || len >= EMAIL_LEN) return 0;

    int atCount = 0, atPos = -1, dotPos = -1;

    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            atCount++;
            atPos = i;
        } else if (email[i] == '.' && atPos != -1) {
            dotPos = i;
        }
    }

    return (atCount == 1 && atPos > 0 && dotPos > atPos + 1 && dotPos < len - 1);
}

int isValidPhoneNumber(char *phone) {
    int len = strlen(phone);
    if (len != 11) return 0;
    if (phone[0] != '0' || phone[1] != '1') return 0;

    for (int i = 0; i < 11; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

int isValidPassword(char *password) {
    int len = strlen(password);
    return (len >= 8 && len < PASSWORD_LEN);
}

int isValidLicensePlate(char *plate) {
    int len = strlen(plate);
    return (len >= 3 && len < LICENSE_PLATE_LEN);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int findUserById(char *userId) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].userId, userId) == 0) {
            return i;
        }
    }
    return -1;
}

int findVehicleById(char *vehicleId) {
    for (int i = 0; i < numVehicles; i++) {
        if (strcmp(vehicles[i].vehicleId, vehicleId) == 0) {
            return i;
        }
    }
    return -1;
}

int findAvailableSpot() {
    for (int i = 0; i < MAX_PARKING_SPOTS; i++) {
        if (!spots[i].isOccupied) {
            return spots[i].spotNumber;
        }
    }
    return -1;
}

void generateUserId(char *userId) {
    static int userCounter = 1;
    sprintf(userId, "USR%04d", userCounter++);
}

void generateVehicleId(char *vehicleId) {
    static int vehicleCounter = 1;
    sprintf(vehicleId, "VH%04d", vehicleCounter++);
}

double calculateParkingFee(time_t entryTime) {
    time_t currentTime = time(NULL);
    double hours = difftime(currentTime, entryTime) / 3600.0;

    // Parking fee structure: $2 per hour, minimum $2
    double fee = hours * 20.0;
    return (fee < 2.0) ? 20.0 : fee;
}
