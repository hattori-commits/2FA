#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <unistd.h> 

#define OTP_VALIDITY 30 
#define MAX_ATTEMPTS 3  
#define MAX_USERS 20    
#define DATA_FILE "users_data.txt" 


typedef struct {
    char user_name[50];
    char password[50];
} User;

User users[MAX_USERS];
int user_count = 0;


int generate_otp() {
    srand(time(NULL));
    return rand() % 900000 + 100000; 
}


int is_registered(const char *user_name) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].user_name, user_name) == 0) {
            return i; 
        }
    }
    return -1; 
}


void load_users() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        // File doesn't exist yet
        return;
    }

    user_count = 0;
    while (fscanf(file, "%s %s", users[user_count].user_name, users[user_count].password) == 2) {
        user_count++;
        if (user_count >= MAX_USERS) break; 
    }

    fclose(file);
}


void save_users() {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        printf("Error saving user data.\n");
    }

    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s %s\n", users[i].user_name, users[i].password);
    }

    fclose(file);
}


void register_user() {
    if (user_count >= MAX_USERS) {
        printf("User limit reached. Cannot register new users.\n");
    }

    printf("Register a new user:\n");
    printf("Enter username: ");
    scanf("%s", users[user_count].user_name);
    printf("Enter Password: ");
    scanf("%s", users[user_count].password);

    printf("User registered successfully!\n");
    user_count++;

    
    save_users();
}


int main() {
    char input_user_name[50], input_password[50];
    int otp, entered_otp, attempts = 0;
    time_t otp_generated_time;

   
    load_users();

    printf("Welcome! Are you a registered user? (yes/no): ");
    char choice[10];
    scanf("%s", choice);

    if (strcmp(choice, "no") == 0) {
        register_user();
        return 0; 
    }

    if (strcmp(choice, "yes") != 0) {
        printf("Invalid choice. Exiting...\n");
        return 1;
    }

    
    printf("Enter user_name: ");
    scanf("%s", input_user_name);

    int user_index = is_registered(input_user_name);
    if (user_index == -1) {
        printf("User not found. Please register first.\n");
        return 1;
    }

    printf("Enter Password: ");
    scanf("%s", input_password);

    if (strcmp(users[user_index].password, input_password) != 0) {
        printf("Invalid password. Exiting...\n");
        return 1;
    }

    printf("Login successful!\n");

   
    otp = generate_otp();
    otp_generated_time = time(NULL);

   
    printf("Your OTP is: %d\n", otp);

    
    while (attempts < MAX_ATTEMPTS) {
        printf("Enter OTP: ");
        scanf("%d", &entered_otp);

        
        if (difftime(time(NULL), otp_generated_time) > OTP_VALIDITY) {
            printf("OTP expired. Please request a new one.\n");
            return 1;
        }

        if (entered_otp == otp) {
            printf("Authentication successful!\n");
            return 0;
        } else {
            printf("Incorrect OTP. Try again.\n");
            attempts++;
        }
    }

    printf("Maximum attempts reached. Exiting...\n");
    return 1;
}
