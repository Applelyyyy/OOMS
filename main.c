//plan v2
// 1. list
// 2. change path
// 3. go back to menu
// 4. update/Search csv -> 1.listv2 2.Search 3.add data -> if yes add 4.delete if yes -> delete 
// 5 -> 3,6,save

// log to do
// DONE Menu v0.1
// DONE Menu v0.5
// Done read the file csv v1
// done change path file v-beta
// done load file csv and change path
// todo make plan 4


//-----------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "github_sync.h" //sync file CSV
//-----------------
// SETUP FUNCTION prototypes
void cls();
void delay(int delay);
void menu();
void invalid();
void list();
void change_csv_path();
char *csv_name();
char *check_file();
void enter_to_back();
void c_no_file_CSV();
void start();
int read_data();
//-----------------
// filename defult CSV
char *filename = "../data/raw_data.csv";
//  defult CSV config
char *csv_default = "OrderID,ProductName,Quantity,TotalPrice";
//-----------------
//structures csv for r w search
struct data_csv_st {
    char OrderID[50];
    char ProductName[100];
    int Quantitiy;
    int TotalPrice;
};
//-----------------
// Set var
int product_count = 0;
int product_capacity = 0;
struct data_csv_st *products = NULL;
//-----------------
// ANSI color
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

// main v5
int main() {
    char input[256];
    int choice;
    c_no_file_CSV();
    do {
        start();
        read_data();
        menu();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            invalid();
            continue;
        }
        choice = atoi(input);
        switch (choice)
        {
        case 1:
            list();
            break;
        case 2:
            change_csv_path();
            break;
        case 3:
        //Larg-update
            break;
        case 9:
            printf(RESET);
            cls();
            printf(RED"Press Enter to EXIT...");
            getchar();
            exit(0);
        default:
            invalid();
        }
    }
    while(1);
    return 0;
}

//change file path CSV and create file if not have 
void change_csv_path(){
    cls();
    printf("-------------------------------------------\n\n");
    printf(YELLOW"Change CSV File(Path) "RESET"and "GREEN"Create(NewFile)"RESET"\n\n");
    printf("-------------------------------------------\n\n");
    printf("\t  |- File in Folder -|\n\n");
    printf("-------------------------------------------\n\n"YELLOW);
    #ifdef _WIN32
    system("dir /b ..\\data\\*.csv");
    #else
    system("ls ../data/*.csv");
    #endif
    printf(RESET"\n-------------------------------------------\n\n");
    printf("You want to"YELLOW" continue ?\n"RESET"");
    printf(GREEN"1. Continue\n"RESET);
    printf(RED"2. Go Back\n\n"RESET);
    printf("-------------------------------------------\n");
    printf(MAGENTA"---> "YELLOW"");
    char input[256];
    int choice;
    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        invalid();
        change_csv_path();
    }
        choice = atoi(input);
            if (choice == 2){
                cls();
                start();
            }
            else if (choice == 1){
                printf(RESET"Enter new CSV file path (relative to 'data' folder): "YELLOW);
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
                            enter_to_back();
                            return;
                        }
                        cls();
                        printf(RESET"--------------------------------------\n");
                        printf("Change CSV File and "YELLOW"Create\n"RESET);
                        printf("--------------------------------------\n");
                        printf("--------------------------------------\n");
                        printf("New CSV File Create! --> %s\n", full_path);
                        printf("--------------------------------------\n");
                        }
                    else {
                        printf(RESET"--------------------------------------\n");
                        printf("CSV file Found! and "GREEN"Loaded: %s\n"RESET, full_path);
                        printf("--------------------------------------\n");
                        fclose(file);
                        }
                    // Free previous filename if it was dynamically allocated
                    if (filename != NULL && filename != csv_default && filename != "../data/raw_data.csv") {
                    free(filename);
                    }
                    filename = strdup(full_path);
                    // Reset product data
                    product_count = 0;
                    product_capacity = 0;
                    free(products);
                    products = NULL;
                    read_data();
                }
                else {
                    printf("--------------------------------------\n");
                    printf(RED"Failed to read CSV Path not Change use -->"GREEN"%s\n"RESET"",csv_name());
                    printf("--------------------------------------\n");
                }
                enter_to_back();
            }
    else{
        invalid();
        change_csv_path();
    }
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
    printf("Current Read CSV --> "YELLOW"%s\n\n"RESET"",r_name);
    return 0;
}

//check file path
char *check_file(){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf(RED"Error"RESET" : Could not open file "YELLOW"%s\n"RESET"", filename);
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
        printf(RED"\t%s\n\n", check_file());
        printf(RESET"--------------------------------------\n");
        enter_to_back();
    }
    else{
        free(products);
        products = NULL; 
        product_count = 0;
        product_capacity = 0;
        read_data();
        printf("------------------------------------------------\n");
        printf(GREEN"\t     LIST CSV\n"RESET);
        printf("------------------------------------------------\n");
        printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");
        for (int i = 0; i < product_count; i++){
        printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",products[i].OrderID, products[i].ProductName, products[i].Quantitiy, products[i].TotalPrice);
        }
        printf("------------------------------------------------\n");
        enter_to_back();
    }
}

// struct list v2
int read_data(){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("Error To open File : %s", filename);
        perror("fopen");
        return 1;
    }
    char line[1024];
    fgets(line, sizeof(line), file); //  skip frist line
    while (fgets(line, sizeof(line), file)){
        if (product_capacity == product_count){
            if (product_capacity == 0){
                product_capacity = 10;
            }
            else{
                product_capacity = product_capacity * 2;
            }
            products = realloc(products, product_capacity * sizeof(struct data_csv_st));
            if(!products){
                perror("realloc");
                fclose(file);
                return 1;
            }
        }

        line[strcspn(line,"\n\r")] = '\0';
        char *token = strtok(line,",");
        int index = 0;
        while (token)
        {
            switch (index)
            {
            case 0:
                strcpy(products[product_count].OrderID ,token);
                break;
            case 1:
                strcpy(products[product_count].ProductName, token);
                break;
            case 2:
                products[product_count].Quantitiy = atoi(token);
                break;
            case 3:
                products[product_count].TotalPrice = atoi(token);
            default:
                break;
            }
            token = strtok(NULL,",");
            index++;
        }
        product_count++;
    }
    fclose(file);
    return 0;
}

// Do in background at the startup Program
// create the file if not have raw_data.cvs File
void c_no_file_CSV(){
    // printf("Checking File...\n");
    FILE *file = fopen(filename, "r");
    if (file != NULL){
        cls();
        return;
    }
    else{
        sync_github_file();//auto sync file from github
        FILE *file = fopen(filename, "r");
        if (file != NULL){
            fclose(file);
            filename = strdup(filename);
            read_data();
        }
        else{
            char ac_file[256];
            snprintf(ac_file,sizeof(ac_file),"../data/new_file.csv");// create new file if no file
            FILE *file = fopen(ac_file, "w");
            if (file != NULL){
                fprintf(file,"%s\n",csv_default);
                fclose(file);
                filename = strdup(ac_file);
            }
            else{
                exit(0);
            }
        }
    }
}

// clear terminal
void cls(){
    system("cls||clear");
}

// sleep to delay for me because i program robot LOL
void delay(int delay){
    sleep(delay);
}

// printf("3. Add Data TO CSV\n");
// printf("4. Search Data\n");
// printf("6. Delete Data\n");
// Menu v6
void menu(){
    cls();
    printf(RESET);
    printf(GREEN"==========================================\n"RESET);
    printf(BLUE"Welcome To Online Order Management System \n"RESET);
    printf(GREEN"==========================================\n\n"RESET);
    printf(RESET"\t     --|Menu Panel|--\n\n");
    printf(GREEN"------------------------------------------\n"RESET);
    printf(YELLOW"1."RESET" List Data IN CSV\n");
    printf(YELLOW"2."RESET" Change file CSV\n");
    printf(YELLOW"3."RESET" Update Data / Search Data\n");
    printf(RED"9."RESET" !! Exit The Program !!\n");
    printf(GREEN"------------------------------------------\n\n"RESET);
    printf("\t     Current Read CSV\n\n");
    printf("%s\n", check_file());
    printf(GREEN"==========================================\n"RESET);
    printf(" --> Enter your choice (1-3,9): ");
}

// invalid input function enter to back
void invalid(){
    cls();
    printf("\n");
    printf("******************************************\n");
    printf(RED"!!! Invalid choice. Please try again. !!!\n");
    printf(RESET"******************************************\n\n");
    printf("Press "RED"Enter"RESET" to go back...\n");
    getchar();
}
//function enter to back
void enter_to_back(){
    printf("\nPress "RED"Enter"RESET" to go back to the menu...");
    getchar();
}

//for set loop not all in main
void start(){
    cls();
}