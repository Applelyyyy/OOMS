// 1. Save Data TO CSV
// 2. List Data IN CSV {Print all in csv}
// 3. Add Data TO CSV
// 4. Search Data
// 5. Update Data {in temp file csv}
// 6. Delete Data
// 7. Back To Menu

// log to do
// DONE Menu v0.1
// DONE Menu v0.5
// To do read the file csv v0.1


//-----------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//-----------------
// SETUP FUNCTION prototypes
void cls();
void delay_q(int d_value);
void delay(int delay);
void menu();
void invalid();
void read_csv(char *filename);
void list();
void change_csv_path();
char *csv_name();
void ui_change_csv();
char *check_file();
//-----------------
// filename defult CSV
char *filename = "../data/draw_data.csv";
//-----------------
// main v1
int main() {
    char input[100];
    int choice;
    do {
        menu();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            invalid();
            continue;
        }
        choice = atoi(input);
        switch (choice)
        {
        case 1:
            break;
        case 2:
            list();
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            cls();
            break;
        case 8:
            // change_csv_path();
            break;
        case 9:
            cls();
            delay_q(3);
            exit(0);
        default:
            invalid();
        }
    }
    while(1);
    return 0;

}





// // ui cange file path CSV
// void ui_change_csv(){
//     cls();
//     printf("Change CSV File Path\n");
//     printf("==========================================\n");
//     printf("Current CSV File: %s\n", csv_name(););
//     printf("Enter new CSV file path: ");
// }


// //change file path CSV
// void change_csv_path(){
//     ui_change_csv();
//     char n_path[256];
//     if (fgets(n_path, sizeof(n_path), stdin) != NULL){
//         size_t len = strlen(n_path);
//         if (len > 0 && new_path[len - 1] == '\n'){
//             n_path[len - 1] = '\0';
//         }
//         filename = strdup(new_path);
//         printf("\nCSV file path updated to: %s\n", filename);
//     }
//     else {
//         printf("\n Failed to read input\n");
//         printf("CSV File Not Change ! use --> %s",csv_name());
//     }
//     delay(1.5);
// }




// real name file
char *csv_name() {
    static char r_name[256];
    const char *base_name = strrchr(filename, '/');
    if (!base_name) {
        base_name = filename;
    } else {
        base_name++;
    }
    strncpy(r_name, base_name, sizeof(r_name));
    char *dot = strrchr(r_name, '.'); 
    if (dot) {
        *dot = '\0';
    }
    printf("Current Read CSV --> %s\n\n",r_name);
    return 0;
}

//check file path
char *check_file(){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("Error: Could not open file %s\n", filename);
        return 0;
    }
    else{
        csv_name();
        return 0;
    }
}


void list(){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("--------------------------------------\n\n");
        printf("\t%s\n\n", check_file());
        printf("--------------------------------------\n");
        printf("Press Enter to go back to the menu...\n");
        getchar();
    }
    else{
        printf("\n--------------------------------------\n");
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line); // list all csv
        }
        fclose(file);
        printf("\n--------------------------------------\n");
        printf("\nPress Enter to go back to the menu...");
        getchar();
    }
}

// clear terminal
void cls(){
    system("cls||clear");
}


// delay to quit the program
// TO DO DO save or Not 
void delay_q(int d_value){
    printf("Exit Program...in");
    for(int i = d_value; i >= 1; i--){
        printf("...%d",i);
        delay(1);
    }
    printf("\n");
    printf("EXIT...");
}

// sleep to delay for me because i program robot LOL
void delay(int delay){
    sleep(delay);
}

// Menu v1.5
void menu(){
    cls();
    printf("==========================================\n");
    printf("Welcome To Online Order Management System \n");
    printf("==========================================\n\n");
    printf("\t     --|Menu Panel|--\n");
    printf("------------------------------------------\n");
    printf("1. Save Data TO CSV\n");
    printf("2. List Data IN CSV\n");
    printf("3. Add Data TO CSV\n");
    printf("4. Search Data\n");
    printf("5. Update Data\n");
    printf("6. Delete Data\n");
    printf("7. Back To Menu\n");
    printf("8. Change Path CSV\n");
    printf("9. !! Exit The Program !!\n");
    printf("------------------------------------------\n\n");
    printf("\t     Current Read CSV\n");
    printf("%s\n", check_file());
    printf("==========================================\n");
    printf(" --> Enter your choice (1-9): ");
}

// error input function
void invalid(){
    cls();
    printf("\n");
    printf("******************************************\n");
    printf("!!! Invalid choice. Please try again. !!!\n");
    printf("******************************************\n\n");
    printf("\nPress Enter to go back to the menu...");
    getchar();
}

