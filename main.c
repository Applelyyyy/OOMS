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
// Done read the file csv v1
// done change path file v-beta
// todo 


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
char *check_file();
//-----------------
// filename defult CSV
char *filename = "../data/raw_data.csv";
//  defult CSV config
char *csv_default = "OrderID,ProductName,Quantity,TotalPrice";
//-----------------



// main v2
int main() {
    char input[256];
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
            change_csv_path();
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

//todo make a ui v2
//change file path CSV
void change_csv_path(){
    cls();
    printf("--------------------------------------\n");
    printf("Change CSV File and Create\n");
    printf("--------------------------------------\n");
    printf("Enter new CSV file path (relative to 'data' folder): ");
    char n_path[256];
    if (fgets (n_path, sizeof(n_path), stdin) != NULL){
        size_t len = strlen(n_path);
        if (len > 0 && n_path[len - 1] == '\n'){
            n_path[len - 1] = '\0';
        }
        //create file
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "../data/%s.csv",n_path);
        FILE *file = fopen(full_path, "r");
        // if have file go else and loaded
        if (file == NULL){
            // create file and loaded and csv_default auto loaded
            file = fopen(full_path, "w");
            fprintf(file,"%s\n",csv_default);
            fclose(file);
            if (file == NULL){
                printf("--------------------------------------\n");
                printf("!!! Error : Could Not Create File\n");
                printf("--------------------------------------\n");
                printf("Press Enter to go back to the menu...\n");
                getchar();
                return;
            }
            printf("--------------------------------------\n");
            printf("New CSV File Create! --> %s\n", full_path);
            printf("--------------------------------------\n");
        }
        else {
            printf("--------------------------------------\n");
            printf("CSV file Found and Loaded: %s\n", full_path);
            printf("--------------------------------------\n");
        }
        fclose(file);
        filename = strdup(full_path);
    }
    else {
        printf("!!! Error !!!\n");
        printf("--------------------------------------\n");
        printf("Failed to read CSV Path not Change use -->%s\n",csv_name());
        printf("--------------------------------------\n");
    }
    printf("\nPress Enter to go back to the menu...");
    getchar();
}




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

// list all in csv file
void list(){
    cls();
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("--------------------------------------\n\n");
        printf("\t%s\n\n", check_file());
        printf("--------------------------------------\n");
        printf("Press Enter to go back to the menu...\n");
        getchar();
    }
    else{
        printf("--------------------------------------\n");
        printf("\t     LIST CSV\n");
        printf("--------------------------------------\n");
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("|%s", line); // list all csv
        }
        fclose(file);
        printf("--------------------------------------\n");
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
    printf("Exit Program..[");
    for(int i = d_value; i >= 1; i--){
        printf("===");
        delay(1);
    }
    printf("]");
    printf("\n");
    printf("Press Enter to EXIT...");
    getchar();
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
    printf("8. Change file CSV\n");
    printf("9. !! Exit The Program !!\n");
    printf("------------------------------------------\n\n");
    printf("\t     Current Read CSV\n\n");
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

