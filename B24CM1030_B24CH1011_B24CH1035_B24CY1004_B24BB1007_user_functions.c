#include "B24CM1030_B24CH1011_B24CH1035_B24CY1004_B24BB1007_bus.h"

void loadUsers() {
    FILE *file = fopen(USER_DATABASE, "rb");
    if (file != NULL) {
        userCount = fread(users, sizeof(User), MAX_USERS, file);
        fclose(file);
    }
}

void saveUsers() {
    FILE *file = fopen(USER_DATABASE, "wb");
    if (file != NULL) {
        fwrite(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Helper function: validate phone number
int isValidPhoneNumber(const char *phone) {
    if (strlen(phone) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (phone[i] < '0' || phone[i] > '9') return 0;
    }
    return 1;
}

// Helper function: validate password strength
int isStrongPassword(const char *password) {
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    if (strlen(password) < 6) return 0; // Minimum length 6

    for (int i = 0; password[i]; i++) {
        if (password[i] >= 'A' && password[i] <= 'Z') hasUpper = 1;
        else if (password[i] >= 'a' && password[i] <= 'z') hasLower = 1;
        else if (password[i] >= '0' && password[i] <= '9') hasDigit = 1;
        else hasSpecial = 1;
    }

    return (hasUpper && hasLower && hasDigit && hasSpecial);
}

int signUp() {
    clearScreen();
    printf("\n+===================================+\n");
    printf("|          SIGN UP                 |\n");
    printf("+===================================+\n");

    if (userCount >= MAX_USERS) {
        printf("Maximum user limit reached!\n");
        printf("Press Enter to continue...");
        while(getchar() != '\n'); 
        return 0;
    }

    User newUser;

    // Username input with back option
    while (1) {
        printf("Enter username (or press 'b' to go back): ");
        if (scanf("%49s", newUser.username) != 1) {
            printf("Error reading username!\n");
            while(getchar() != '\n');
            return 0;
        }
        if (newUser.username[0] == 'b' || newUser.username[0] == 'B') {
            return 0; // Back to authentication menu
        }

        int usernameExists = 0;
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, newUser.username) == 0) {
                usernameExists = 1;
                break;
            }
        }
        if (usernameExists) {
            printf("Username not available. Try another.\n");
        } else {
            break;
        }
    }

    // Password input with back option
    while (1) {
        printf("Enter password (Min 6 chars, 1 uppercase, 1 lowercase, 1 digit, 1 special character, or press 'b' to go back): ");
        if (scanf("%49s", newUser.password) != 1) {
            printf("Error reading password!\n");
            while(getchar() != '\n');
            return 0;
        }
        if (newUser.password[0] == 'b' || newUser.password[0] == 'B') {
            return 0; // Back to authentication menu
        }
        if (!isStrongPassword(newUser.password)) {
            printf("Weak password! Please follow the password rules.\n");
        } else {
            break;
        }
    }

    // Full name input with back option
    printf("Enter full name (or press 'b' to go back): ");
    while(getchar() != '\n');  // To clean input buffer
    if (fgets(newUser.fullName, sizeof(newUser.fullName), stdin) == NULL) {
        printf("Error reading full name!\n");
        return 0;
    }
    if (newUser.fullName[0] == 'b' || newUser.fullName[0] == 'B') {
        return 0; // Back to authentication menu
    }
    newUser.fullName[strcspn(newUser.fullName, "\n")] = '\0';

    // Phone number input with back option
    while (1) {
        printf("Enter phone number (10 digits, or press 'b' to go back): ");
        if (scanf("%14s", newUser.phone) != 1) {
            printf("Error reading phone number!\n");
            while(getchar() != '\n');
            return 0;
        }
        if (newUser.phone[0] == 'b' || newUser.phone[0] == 'B') {
            return 0; // Back to authentication menu
        }
        if (!isValidPhoneNumber(newUser.phone)) {
            printf("Invalid phone number! Please enter exactly 10 digits.\n");
        } else {
            break;
        }
    }

    // Email input with back option
    printf("Enter email (or press 'b' to go back): ");
    if (scanf("%49s", newUser.email) != 1) {
        printf("Error reading email!\n");
        while(getchar() != '\n');
        return 0;
    }
    if (newUser.email[0] == 'b' || newUser.email[0] == 'B') {
        return 0; // Back to authentication menu
    }

    users[userCount++] = newUser;
    saveUsers();
    currentUser = newUser;
    loggedIn = 1;

    printf("\nRegistration successful! You are now logged in.\n");
    printf("Press Enter to continue...");
    while(getchar() != '\n');

    return 1;
}


int signIn() {
    clearScreen();
    printf("\n+===================================+\n");
    printf("|          SIGN IN                 |\n");
    printf("+===================================+\n");

    char username[50];
    char password[50];

    printf("Enter username: ");
    if (scanf("%49s", username) != 1) {
        printf("Error reading username!\n");
        // Clear any remaining input
        while (getchar() != '\n');
        return 0;
    }

    printf("Enter password: ");
    if (scanf("%49s", password) != 1) {
        printf("Error reading password!\n");
        // Clear any remaining input
        while (getchar() != '\n');
        return 0;
    }

    // Clear any remaining input (like newline characters)
    while (getchar() != '\n');

    int userFound = 0;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            userFound = 1;
            if (strcmp(users[i].password, password) == 0) {
                currentUser = users[i];
                loggedIn = 1;
                printf("\nWelcome, %s!\n", currentUser.fullName);
                printf("Press Enter to continue...");
                while(getchar() != '\n');  // Wait for Enter key press to continue
                return 1;
            } else {
                printf("\nIncorrect password!\n");
                printf("Press Enter to continue...");
                while(getchar() != '\n');
                return 0;
            }
        }
    }

    // If we get here, username wasn't found
    if (!userFound) {
        printf("\nUser not found!\n");
        printf("Press Enter to continue...");
        while(getchar() != '\n');
        return 0;
    }

    return 0; // Default return if something unexpected happens
}


void showAuthMenu() {
    int choice;
    
    while (1) {
        clearScreen();
        printf("\n+===================================+\n");
        printf("|    BUS RESERVATION SYSTEM        |\n");
        printf("+===================================+\n");
        printf("| 1. Sign In                       |\n");
        printf("| 2. Sign Up                       |\n");
        printf("| 3. Exit                          |\n");
        printf("+===================================+\n");
        
        printf("Enter your choice (1-3): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }
        
        while(getchar() != '\n');
        
        switch (choice) {
            case 1: // Sign In
                if (signIn()) {
                    return;
                }
                break;
                
            case 2: // Sign Up
                if (signUp()) {
                    return;
                }
                break;
                
            case 3: // Exit
                printf("\nExiting system...\n");
                exit(0);
                
            default:
                printf("Invalid choice! Please enter 1, 2, or 3.\n");
                printf("Press Enter to continue...");
                while(getchar() != '\n');
        }
    }
}
