//-----------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "github_sync.h" //sync file CSV
#include <ctype.h>
//-----------------

//lang windows
#ifdef _WIN32
    #include <windows.h>
#else
    #include <locale.h>
#endif


// Add these definitions after the ANSI color definitions
#define CHECKMARK   "\xFB"     // √ symbol for Windows CP437
#define CROSS       "\xFE"     // × symbol for Windows CP437
#define BULLET      "\x07"     // • symbol for Windows CP437

// Alternative ASCII-safe versions
#define CHECK_ASCII  "[OK]"
#define CROSS_ASCII  "[X]"
#define ARROW_ASCII  " -> "


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
void CreateifNoFile();
void start();
void loop2();
int read_data();
void p_quit();
void ListFileFolder();
void WDUMenu();
int WDUChoice(char *input);
void add_data();
int save_file();
int SaveDataToFile(const char *OrderID, const char *ProductName, const int Quantitiy, const int TotalPrice);
void search_data();
void to_lowercase(char *str);
void delete_data();
void remove_file();
void update_data();
void run_all_tests();
void setup_console();
void run_e2e_tests();
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
#define BOLD    "\x1b[1m"



//---------------------------------------------------------//



// main v6
int main() {
    setup_console();
    char input[256];
    int choice;
    CreateifNoFile();
    read_data();
    do {
        start();
        menu();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            invalid();
            continue;
        }
        choice = atoi(input);
        switch (choice)
        {
        case 1:
            cls();
            list();
            break;
        case 2:
            cls();
            change_csv_path();
            break;
        case 3:
            WDUMenu();
            break;
        case 4:
            cls();
            ListFileFolder();
            enter_to_back();
            break;
        case 5:
            cls();
            run_all_tests();
            break;
        case 6:
            cls();
            run_e2e_tests();
            break;
        case 9:
            p_quit();
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
    printf("===================================================\n\n");
    printf(YELLOW"Change CSV File(Path) "RESET"and "GREEN"Create(NewFile)"RESET"\n\n");
    printf("===================================================\n\n");
    printf("You want to"YELLOW" continue ?\n"RESET"");
    printf(GREEN"1. Continue\n"RESET);
    printf(RED"2. Go Back\n\n"RESET);
    printf("-------------------------------------------\n");
    printf(" --> Enter your choice (1-2): "YELLOW);
    char input[256];
    int choice;
    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        invalid();
        change_csv_path();
    }
    printf(RESET);
        choice = atoi(input);
            if (choice == 2){
                cls();
                start();
            }
            else if (choice == 1){
                cls();
                printf(YELLOW"     Change CSV File(Path) "RESET"and "GREEN"Create(NewFile)"RESET"\n\n");
                ListFileFolder();
                printf(RESET"Enter new CSV file path (relative to 'data' folder ["RED"!q to Exit"RESET"]): "YELLOW);
                char n_path[256];
                if (fgets (n_path, sizeof(n_path), stdin) != NULL){
                    size_t len = strlen(n_path);
                    if (len > 0 && n_path[len - 1] == '\n'){
                        n_path[len - 1] = '\0';
                    }
                    if(strcmp(n_path, "!q") == 0){
                        enter_to_back();
                        return;
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
                            printf("--------------------------------------------------\n");
                            printf("!!! Error : Could Not Create File\n");
                            printf("--------------------------------------------------\n");
                            enter_to_back();
                            return;
                        }
                        cls();
                        printf("\n\n");
                        printf(YELLOW"Change CSV File(Path) "RESET"and "GREEN"Create(NewFile)"RESET"\n\n");
                        ListFileFolder();
                        printf(RESET"--------------------------------------------------\n\n");
                        printf("\t          CSV File "YELLOW"Created\n\n"RESET);
                        printf("\t"GREEN"New"RESET" CSV File --> "GREEN"%s\n\n"RESET"", full_path);
                        printf("--------------------------------------------------\n");
                        }
                        // if have file
                    else {
                        cls();
                        printf("\n\n");
                        printf(YELLOW"Change CSV File(Path) "RESET"and "GREEN"Create(NewFile)"RESET"\n\n");
                        ListFileFolder();
                        printf("\n");
                        printf(RESET"--------------------------------------------------\n\n");
                        printf("CSV file "GREEN"Found!"RESET" and "GREEN"Loaded: %s\n\n"RESET, full_path);
                        printf("--------------------------------------------------\n");
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

// Get real name file csv
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

//check file path if file Deleat or change name
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
        printf("----------------------------------------------------------\n");
        printf(GREEN"\t           LIST CSV\n"RESET);
        printf("----------------------------------------------------------\n");
        printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");
        int grand_total = 0;
        for (int i = 0; i < product_count; i++){
            printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",
                products[i].OrderID, products[i].ProductName, products[i].Quantitiy, products[i].TotalPrice);
            grand_total += products[i].TotalPrice;
        }
        
        printf("----------------------------------------------------------\n");
        printf(GREEN"%-10s %-20s %-10s "YELLOW"%-10d\n"RESET, "", "", "TOTAL:", grand_total);
        printf("----------------------------------------------------------\n");
        printf(YELLOW"Total Records: %d | Grand Total Price: %d\n"RESET, product_count, grand_total);
        printf("----------------------------------------------------------\n");
        enter_to_back();
    }
}

// struct list read data v2
int read_data(){
    if (products != NULL) {
        free(products);
        products = NULL;
    }
    product_count = 0;
    product_capacity = 0;

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
        if (strlen(line) == 0 || strspn(line, " \t\r\n") == strlen(line)) {
        continue;  // Skip this line completely
        }
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
void CreateifNoFile(){
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

//Save file Function
int save_file(){
    FILE *file = fopen(filename, "w");
    if (!file){
        perror("fopen");
        return 1;
    }
    fprintf(file, "%s\n", csv_default);
    for (int i = 0; i < product_count; i++){
        fprintf(file,"%s,%s,%d,%d\n",
        products[i].OrderID,
        products[i].ProductName,
        products[i].Quantitiy,
        products[i].TotalPrice
        );
    }
    fclose(file);
    return 0;
}

//Save data to File Function
int SaveDataToFile(const char *OrderID, const char *ProductName, const int Quantitiy, const int TotalPrice){

    for(int i = 0; i<product_count; i++){
        if(strcmp(products[i].OrderID,OrderID) == 0){
            return 1;
        }

    }
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
            return 1;
            }
    }
    // start add
    strcpy(products[product_count].OrderID, OrderID);
    strcpy(products[product_count].ProductName, ProductName);
    products[product_count].Quantitiy = Quantitiy;
    products[product_count].TotalPrice = TotalPrice;
    product_count++;
    //want to save ?
    char input[16];
    cls();
    printf(CYAN"\n\n");
    printf(BLUE"|- Save Data ? -| \n\n"RESET);
    printf(CYAN"\n\n"RESET);
    printf(YELLOW"Save before exit?\n"RESET);
    printf(GREEN"1. Save and go back\n"RESET);
    printf(RED"2. Discard and go back\n"RESET);
    printf(" --> Enter your choice (1-2): "YELLOW);
    while(1){
        fgets(input, sizeof(input), stdin);
        int choice = atoi(input);
        if (choice == 1) {
                printf(RESET);
                save_file();
                cls();
                printf(CYAN"\n\n");
                printf(BLUE"|- Save Data ? -| \n\n"RESET);
                printf(CYAN"\n\n"RESET);
                printf(GREEN"Saved Data.\n"RESET);
            } 
        else if (choice == 2) {
                printf(RESET);
                free(products);
                product_count = 0;
                product_capacity = 0;
                products = NULL;
                cls();
                printf(CYAN"\n\n");
                printf(BLUE"|- Save Data ? -| \n\n"RESET);
                printf(CYAN"\n\n"RESET);
                printf(RED"Discarded to save.\n"RESET);
                return 0;
            }
            else{
                printf(RED"!!! Invalid choice. Please try again. !!!\n"RESET);
                printf(" --> Enter your choice (1-2): "YELLOW);
                continue;
            }
        break;
    }
}


//Add data to csv current
void add_data(){
    cls();
    char OrderID[50];
    char ProductName[100];
    int Quantitiy;
    int TotalPrice;

    printf("\n\n");
    printf(GREEN"- Add Data To CSV -"RESET);
    printf("\n\n");
    printf(CYAN"------------------------------------------\n"RESET);
    //input product ID
    while (1) {
    printf("Enter Product ID ["RED"!q"RESET" to cancel]: "YELLOW"");
    fgets(OrderID, sizeof(OrderID), stdin);
    OrderID[strcspn(OrderID, "\r\n")] = '\0';

    //cancel
    if(strcmp(OrderID, "!q") == 0){
        enter_to_back();
        return;
    }
    // check for blank OrderID
    if (OrderID[0] == '\0' || strspn(OrderID, " ") == strlen(OrderID)) {
        printf(RED"OrderID cannot be blank! Please try again.\n"RESET);
        continue;
    }
    // check if have duplicate OrderID
    int is_duplicate = 0;
    for(int i = 0; i < product_count; i++){
        if(strcmp(products[i].OrderID,OrderID) == 0){
            printf(YELLOW"OrderID Duplicate "RED"can't be This OrderID. Please try again.\n"RESET);
            is_duplicate = 1;
            break;
        }
    }
    if (is_duplicate) continue;
    break; // valid OrderID
    }
    while(1){
        printf(RESET"Enter Product Name: "YELLOW"");
        fgets(ProductName,sizeof(ProductName), stdin);
        ProductName[strcspn(ProductName, "\r\n")] = '\0';
        if (ProductName[0] == '\0' || strspn(ProductName, " ") == strlen(ProductName)) {
        printf(RED"Product Name cannot be blank! Please try again.\n"RESET);
        continue;
        }
        break;
    }

    printf(RESET"Enter Quantity: "YELLOW"");
    char qty_str[16];
    int valid = 0;
    while (!valid) {
    fgets(qty_str, sizeof(qty_str), stdin);
    Quantitiy = atoi(qty_str);
    if (Quantitiy > 0) {
        valid = 1;
    } 
    else {
        printf(RED"Invalid input. Please enter a positive integer for Quantity \n"RESET);
        printf(RED"!!! Invalid choice. Please try again. !!!\n");
        printf(RESET"Enter Quantity: "YELLOW"");
    }  
    }
    printf(RESET"Enter TotalPrice: "YELLOW"");
    char price_str[16];
    valid = 0;
    while (!valid)
    {
        fgets(price_str, sizeof(price_str), stdin);
        TotalPrice = atoi(price_str);
        if (TotalPrice > 0){
            valid = 1;
        }
        else {
            printf(RED"Invalid input. Please enter a positive integer for Quantity \n"RESET);
            printf(RED"!!! Invalid choice. Please try again. !!!\n");
            printf(RESET"Enter TotalPrice: "YELLOW"");
        }
    }
    int result = SaveDataToFile(OrderID, ProductName, Quantitiy, TotalPrice);
    if (result == 1) {
        printf(RED"Failed to add data.\n"RESET);
    }
    enter_to_back();
}

// sleep to delay for me because i program robot LOL
void delay(int delay){
    sleep(delay);
}

// WDU Menu choice

int WDUChoice(char *input) {
    int choice = atoi(input);
    switch (choice) {
        case 1:
            cls();
            add_data();
            break;
        case 2:
            cls();
            delete_data();
            break;
        case 3:
            cls();
            search_data();
            break;
        case 4:
            update_data();
            break;
        case 5:
            read_data();
            list();
            break;
        case 9:
            cls();
            main();
        default:
            invalid();
    }
    return 0;
}

// Add this function to convert a string to lowercase
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Updated search_data function
void search_data() {
    cls();
    char search_term[100];
    int found;

    printf("\n\n");
    printf(GREEN"- Search Data in CSV -"RESET);
    printf("\n\n");
    printf(CYAN"------------------------------------------\n"RESET);

    while (1) {
        found = 0; // Reset the found flag for each search attempt

        
        printf("Enter "YELLOW"OrderID"RESET" or "YELLOW"ProductName"RESET" to search ["RED"!q"RESET" to cancel]: "YELLOW"");
        fgets(search_term, sizeof(search_term), stdin);
        search_term[strcspn(search_term, "\r\n")] = '\0';


        if (strcmp(search_term, "!q") == 0) {
            enter_to_back();
            return;
        }

        
        to_lowercase(search_term);

        cls();
        printf("----------------------------------------------------------\n");
        printf(GREEN"\t           SEARCH RESULTS\n"RESET);
        printf("----------------------------------------------------------\n");
        printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");

        // Search through the products array
        for (int i = 0; i < product_count; i++) {
            char lower_order_id[50];
            char lower_product_name[100];

            // Convert OrderID and ProductName to lowercase for comparison
            strcpy(lower_order_id, products[i].OrderID);
            strcpy(lower_product_name, products[i].ProductName);
            to_lowercase(lower_order_id);
            to_lowercase(lower_product_name);

            if (strstr(lower_order_id, search_term) || strstr(lower_product_name, search_term)) {
                printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",
                        products[i].OrderID, products[i].ProductName, products[i].Quantitiy, products[i].TotalPrice);
                found = 1;
            }
        }

        if (!found) {
            printf(RED"No matching data found for: %s\n"RESET, search_term);
            printf("----------------------------------------------------------\n");
            printf(YELLOW"Please try again or type '!q' to quit.\n"RESET);
        } else {
            printf("----------------------------------------------------------\n");
            enter_to_back();
            return;
        }
    }
}

// Delete data forn line
void delete_data() {
    cls();
    
    //how all data in the CSV
    printf("\n\n");
    printf(RED"- Delete Data from CSV -"RESET);
    printf("\n\n");
    printf(CYAN"----------------------------------------------------------\n"RESET);
    printf(YELLOW"Current data in CSV file:\n"RESET);
    printf("----------------------------------------------------------\n");
    printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");
    printf("----------------------------------------------------------\n");
    

    if (product_count == 0) {
        printf(RED"No data available in the CSV file.\n"RESET);
        printf("----------------------------------------------------------\n");
        enter_to_back();
        return;
    }
    
    for (int i = 0; i < product_count; i++) {
        printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",
                products[i].OrderID, products[i].ProductName, products[i].Quantitiy, products[i].TotalPrice);
    }
    printf("----------------------------------------------------------\n");
    printf(YELLOW"Total records: %d\n\n"RESET, product_count);
    
    char search_term[100];
    int found_indices[1000]; 
    int found_count = 0;

    while (1) {
        found_count = 0; 

       
        printf("Enter "YELLOW"OrderID"RESET" to search for deletion ["RED"!q"RESET" to cancel],or type "GREEN"'!list'"RESET" to list data: "YELLOW"");
        fgets(search_term, sizeof(search_term), stdin);
        search_term[strcspn(search_term, "\r\n")] = '\0';

       
        if (strcmp(search_term, "!q") == 0) {
            enter_to_back();
            return;
        }
        if (strcmp(search_term, "!list") == 0){
            cls();
            printf(RESET"----------------------------------------------------------\n");
            printf(RED"\t           LIST SEARCH RESULTS\n"RESET);
            printf("----------------------------------------------------------\n");
            printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");
            for (int i = 0; i < product_count; i++) {
                printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",
                    products[i].OrderID, products[i].ProductName, products[i].Quantitiy, products[i].TotalPrice);
                }
            printf("----------------------------------------------------------\n");
            enter_to_back();
            cls();
            continue;
        }

        
        if (search_term[0] == '\0' || strspn(search_term, " ") == strlen(search_term)) {
            printf(RED"OrderID cannot be blank! Please try again.\n"RESET);
            continue;
        }

        to_lowercase(search_term);

        cls();
        printf("----------------------------------------------------------\n");
        printf(RED"\t           DELETE SEARCH RESULTS\n"RESET);
        printf("----------------------------------------------------------\n");
        printf(YELLOW"Search OrderID: '%s'\n"RESET, search_term);
        printf("----------------------------------------------------------\n");
        printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");

        
        for (int i = 0; i < product_count; i++) {
            char lower_order_id[50];

            
            strcpy(lower_order_id, products[i].OrderID);
            to_lowercase(lower_order_id);

            if (strstr(lower_order_id, search_term)) {
                printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",
                        products[i].OrderID, products[i].ProductName, products[i].Quantitiy, products[i].TotalPrice);
                found_indices[found_count] = i;
                found_count++;
            }
        }

        if (found_count == 0) {
            printf("----------------------------------------------------------\n\n");
            printf(RED"No matching OrderID found for: %s\n\n"RESET, search_term);
            printf("----------------------------------------------------------\n");
            printf(YELLOW"Please try again or type '!q' to quit, or type '!list' to list data.\n"RESET);
        } 
        else {
            printf("----------------------------------------------------------\n");
            printf(YELLOW"Found %d item(s) with matching OrderID.\n"RESET, found_count);
            printf(RED"Do you want to DELETE these items? (y/N): "YELLOW"");
            
            char confirm[10];
            fgets(confirm, sizeof(confirm), stdin);
            confirm[strcspn(confirm, "\r\n")] = '\0';
            
            if (strcmp(confirm, "y") == 0 || strcmp(confirm, "Y") == 0) {

                for (int i = found_count - 1; i >= 0; i--) {
                    int index_to_delete = found_indices[i];
                    

                    for (int j = index_to_delete; j < product_count - 1; j++) {
                        products[j] = products[j + 1];
                    }
                    product_count--;
                }
                
                if (product_count == 0) {
                    if (products != NULL) {
                        free(products);
                        products = NULL;
                    }
                    product_capacity = 0;
                }
                
                char save_input[16];
                cls();
                printf(CYAN"\n\n");
                printf(RED"|- Delete Confirmation -| \n\n"RESET);
                printf(CYAN"\n\n"RESET);
                printf(RED"Successfully deleted %d item(s) from memory.\n\n"RESET, found_count);
                printf(YELLOW"Do you want to save changes to CSV file?\n"RESET);
                printf(GREEN"1. Save to CSV file and go back\n"RESET);
                printf(RED"2. Don't save to CSV (changes will be lost) and go back\n"RESET);
                printf(" --> Enter your choice (1-2): "YELLOW"");
                
                while(1) {
                    fgets(save_input, sizeof(save_input), stdin);
                    int save_choice = atoi(save_input);
                    
                    if (save_choice == 1) {
                        printf(RESET);
                        int save_result = save_file();
                        cls();
                        printf(CYAN"\n\n");
                        printf(RED"|- Delete Confirmation -| \n\n"RESET);
                        printf(CYAN"\n\n"RESET);
                        if (save_result == 0) {
                            printf(GREEN"Changes saved to CSV file successfully.\n"RESET);
                            printf(GREEN"Deleted %d item(s) permanently.\n"RESET, found_count);
                        } else {
                            printf(RED"Error saving to CSV file!\n"RESET);
                        }
                        enter_to_back();
                        return;
                    } 
                    else if (save_choice == 2) {
                        printf(RESET);
                        if (products != NULL) {
                            free(products);
                            products = NULL;
                        }
                        product_count = 0;
                        product_capacity = 0;
                        read_data();
                        cls();
                        printf(CYAN"\n\n");
                        printf(RED"|- Delete Confirmation -| \n\n"RESET);
                        printf(CYAN"\n\n"RESET);
                        printf(YELLOW"Changes NOT saved to CSV file.\n"RESET);
                        printf(YELLOW"Data restored from file. Nothing was permanently deleted.\n"RESET);
                        enter_to_back();
                        return;
                    }
                    else {
                        printf(RED"Invalid choice. Please enter 1 or 2: "YELLOW"");
                    }
                }
            } else {
                printf(RESET);
                printf(GREEN"Deletion cancelled. No data was removed.\n"RESET);
                enter_to_back();
                return;
            }
        }
    }
}

// remove file in data folder Permission Error Not USe
void remove_file() {
    cls();
    printf("\n\n");
    printf(RED"- Remove CSV Files from Data Folder -"RESET);
    printf("\n\n");
    printf(CYAN"------------------------------------------\n"RESET);
    
    printf(YELLOW"Available CSV files in data folder:\n"RESET);
    printf("===============================================\n");
    
    #ifdef _WIN32
    system("dir /b ..\\data\\*.csv 2>nul");
    #else
    system("ls ../data/*.csv 2>/dev/null");
    #endif
    
    printf("===============================================\n\n");
    
    char file_to_remove[256];
    char full_path[512];
    char confirm[10];
    
    while (1) {
        printf("Enter CSV filename to remove (without .csv extension) ["RED"!q"RESET" to cancel]: "YELLOW"");
        fgets(file_to_remove, sizeof(file_to_remove), stdin);
        file_to_remove[strcspn(file_to_remove, "\r\n")] = '\0';
        
        if (strcmp(file_to_remove, "!q") == 0) {
            enter_to_back();
            return;
        }
        
        if (file_to_remove[0] == '\0' || strspn(file_to_remove, " ") == strlen(file_to_remove)) {
            printf(RED"Filename cannot be blank! Please try again.\n"RESET);
            continue;
        }
        
        snprintf(full_path, sizeof(full_path), "../data/%s.csv", file_to_remove);
        
        FILE *file = fopen(full_path, "r");
        if (file == NULL) {
            printf(RED"File '%s.csv' not found in data folder! Please try again.\n"RESET, file_to_remove);
            continue;
        }
        fclose(file);
        
        char current_file[256];
        const char *base_name = strrchr(filename, '/');
        if (!base_name) {
            base_name = filename;
        } else {
            base_name++;
        }
        strncpy(current_file, base_name, sizeof(current_file));
        
        char target_file[256];
        snprintf(target_file, sizeof(target_file), "%s.csv", file_to_remove);
        
        printf(RESET"\n");
        printf(YELLOW"Target file: %s\n"RESET, full_path);
        printf(YELLOW"Current file: %s\n\n"RESET, filename);
        
        if (strcmp(current_file, target_file) == 0) {
            printf(RED"WARNING: You are trying to remove the currently loaded CSV file!\n"RESET);
            printf(RED"This will cause the program to reload with default settings.\n\n"RESET);
        }
        
        if (strcmp(file_to_remove, "raw_data") == 0) {
            printf(RED"WARNING: You are trying to remove 'raw_data.csv' (default file)!\n"RESET);
            printf(RED"This will cause the program to reset to initial state.\n\n"RESET);
        }
        
        printf(RED"Are you sure you want to DELETE '%s.csv'? This action cannot be undone!\n"RESET, file_to_remove);
        printf(YELLOW"Type 'y' to confirm deletion, or anything else to cancel: "YELLOW"");
        
        fgets(confirm, sizeof(confirm), stdin);
        confirm[strcspn(confirm, "\r\n")] = '\0';
        
        if (strcmp(confirm, "y") == 0 || strcmp(confirm, "Y") == 0) {
            if (remove(full_path) == 0) {
                cls();
                printf(CYAN"\n\n");
                printf(RED"|- File Removal Confirmation -|\n\n"RESET);
                printf(CYAN"\n"RESET);
                printf(GREEN"Successfully deleted: %s\n\n"RESET, full_path);
                
                if (strcmp(current_file, target_file) == 0 || strcmp(file_to_remove, "raw_data") == 0) {
                    printf(YELLOW"Program will now reload due to file removal...\n\n"RESET);
                    printf(RED"Press Enter to restart the program..."RESET);
                    getchar();
                    
                    if (products != NULL) {
                        free(products);
                        products = NULL;
                    }
                    product_count = 0;
                    product_capacity = 0;
                    
                    if (filename != NULL && filename != "../data/raw_data.csv") {
                        free(filename);
                    }
                    filename = "../data/raw_data.csv";
                    
                    // Restart the program
                    cls();
                    main();
                    return;
                } else {
                    printf(GREEN"File removed successfully. Current program state unchanged.\n"RESET);
                }
            } else {
                cls();
                printf(CYAN"\n\n");
                printf(RED"|- File Removal Error -|\n\n"RESET);
                printf(CYAN"\n"RESET);
                printf(RED"Error: Could not delete file '%s'\n"RESET, full_path);
                printf(RED"The file may be in use or you may not have permission.\n"RESET);
            }
        } else {
            cls();
            printf(CYAN"\n\n");
            printf(GREEN"|- File Removal Cancelled -|\n\n"RESET);
            printf(CYAN"\n"RESET);
            printf(GREEN"File removal cancelled. No files were deleted.\n"RESET);
        }
        
        enter_to_back();
        return;
    }
}

//Update data
void update_data() {
    cls();
    
    // Show all data in the CSV
    printf("\n\n");
    printf(GREEN"- Update Data in CSV -"RESET);
    printf("\n\n");
    printf(CYAN"------------------------------------------\n"RESET);
    printf(YELLOW"Current data in CSV file:\n"RESET);
    printf("----------------------------------------------------------\n");
    printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");
    printf("----------------------------------------------------------\n");
    
    if (product_count == 0) {
        printf(RED"No data available in the CSV file.\n"RESET);
        printf("----------------------------------------------------------\n");
        enter_to_back();
        return;
    }
    
    for (int i = 0; i < product_count; i++) {
        printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",
                products[i].OrderID, products[i].ProductName, products[i].Quantitiy, products[i].TotalPrice);
    }
    printf("----------------------------------------------------------\n");
    printf(YELLOW"Total records: %d\n\n"RESET, product_count);
    
    char search_orderid[50];
    int found_index = -1;

    while (1) {
        // Get OrderID to update
        printf("Enter "YELLOW"OrderID"RESET" to update ["RED"!q"RESET" to cancel]: "YELLOW"");
        fgets(search_orderid, sizeof(search_orderid), stdin);
        search_orderid[strcspn(search_orderid, "\r\n")] = '\0';

        // Cancel option
        if (strcmp(search_orderid, "!q") == 0) {
            enter_to_back();
            return;
        }

        // Check for blank OrderID
        if (search_orderid[0] == '\0' || strspn(search_orderid, " ") == strlen(search_orderid)) {
            printf(RED"OrderID cannot be blank! Please try again.\n"RESET);
            continue;
        }

        // Find the OrderID
        for (int i = 0; i < product_count; i++) {
            if (strcmp(products[i].OrderID, search_orderid) == 0) {
                found_index = i;
                break;
            }
        }

        if (found_index == -1) {
            printf(RED"OrderID '%s' not found! Please try again.\n"RESET, search_orderid);
            continue;
        }

        break; // OrderID found, exit loop
    }

    // Display current data for the found OrderID
    cls();
    printf("----------------------------------------------------------\n");
    printf(GREEN"\t           UPDATE DATA\n"RESET);
    printf("----------------------------------------------------------\n");
    printf(YELLOW"Current data for OrderID: %s\n"RESET, search_orderid);
    printf("----------------------------------------------------------\n");
    printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");
    printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",
            products[found_index].OrderID, products[found_index].ProductName, 
            products[found_index].Quantitiy, products[found_index].TotalPrice);
    printf("----------------------------------------------------------\n\n");

    // Get new data from user
    char new_OrderID[50];
    char new_ProductName[100];
    int new_Quantity;
    int new_TotalPrice;

    printf(GREEN"Enter new data (press Enter to keep current value):\n\n"RESET);

    // Update OrderID
    while (1) {
        printf("Current OrderID: "YELLOW"%s"RESET"\n", products[found_index].OrderID);
        printf("Enter new OrderID ["RED"!q"RESET" to cancel]: "YELLOW"");
        fgets(new_OrderID, sizeof(new_OrderID), stdin);
        new_OrderID[strcspn(new_OrderID, "\r\n")] = '\0';

        // Cancel option
        if (strcmp(new_OrderID, "!q") == 0) {
            enter_to_back();
            return;
        }

        // Keep current value if empty
        if (new_OrderID[0] == '\0' || strspn(new_OrderID, " ") == strlen(new_OrderID)) {
            strcpy(new_OrderID, products[found_index].OrderID);
            break;
        }

        // Check for duplicate OrderID (but allow same OrderID if it's the current one)
        int is_duplicate = 0;
        for (int i = 0; i < product_count; i++) {
            if (i != found_index && strcmp(products[i].OrderID, new_OrderID) == 0) {
                printf(YELLOW"OrderID '%s' already exists! Please try again.\n"RESET, new_OrderID);
                is_duplicate = 1;
                break;
            }
        }
        if (is_duplicate) continue;
        break; // Valid OrderID
    }

    // Update ProductName
    printf("\nCurrent ProductName: "YELLOW"%s"RESET"\n", products[found_index].ProductName);
    printf("Enter new ProductName: "YELLOW"");
    fgets(new_ProductName, sizeof(new_ProductName), stdin);
    new_ProductName[strcspn(new_ProductName, "\r\n")] = '\0';
    
    // Keep current value if empty
    if (new_ProductName[0] == '\0' || strspn(new_ProductName, " ") == strlen(new_ProductName)) {
        strcpy(new_ProductName, products[found_index].ProductName);
    }

    // Update Quantity
    printf(RESET"\nCurrent Quantity: "YELLOW"%d"RESET"\n", products[found_index].Quantitiy);
    printf("Enter new Quantity: "YELLOW"");
    char qty_str[16];
    fgets(qty_str, sizeof(qty_str), stdin);
    qty_str[strcspn(qty_str, "\r\n")] = '\0';
    
    // Keep current value if empty or invalid
    if (qty_str[0] == '\0' || atoi(qty_str) <= 0) {
        new_Quantity = products[found_index].Quantitiy;
    } else {
        new_Quantity = atoi(qty_str);
    }

    // Update TotalPrice
    printf(RESET"\nCurrent TotalPrice: "YELLOW"%d"RESET"\n", products[found_index].TotalPrice);
    printf("Enter new TotalPrice: "YELLOW"");
    char price_str[16];
    fgets(price_str, sizeof(price_str), stdin);
    price_str[strcspn(price_str, "\r\n")] = '\0';
    
    // Keep current value if empty or invalid
    if (price_str[0] == '\0' || atoi(price_str) <= 0) {
        new_TotalPrice = products[found_index].TotalPrice;
    } else {
        new_TotalPrice = atoi(price_str);
    }

    // Show updated data preview
    cls();
    printf("----------------------------------------------------------\n");
    printf(BLUE"\t           UPDATE PREVIEW\n"RESET);
    printf("----------------------------------------------------------\n");
    printf(YELLOW"Old Data:\n"RESET);
    printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");
    printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",
            products[found_index].OrderID, products[found_index].ProductName, 
            products[found_index].Quantitiy, products[found_index].TotalPrice);
    
    printf("\n"GREEN"New Data:\n"RESET);
    printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");
    printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",
            new_OrderID, new_ProductName, new_Quantity, new_TotalPrice);
    printf("----------------------------------------------------------\n");

    // Confirm update
    char confirm[10];
    printf(YELLOW"Do you want to update this data? (y/N): "YELLOW"");
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\r\n")] = '\0';

    if (strcmp(confirm, "y") == 0 || strcmp(confirm, "Y") == 0) {
        // Update the data
        strcpy(products[found_index].OrderID, new_OrderID);
        strcpy(products[found_index].ProductName, new_ProductName);
        products[found_index].Quantitiy = new_Quantity;
        products[found_index].TotalPrice = new_TotalPrice;

        // Ask to save
        char save_input[16];
        cls();
        printf(CYAN"\n\n");
        printf(BLUE"|- Update Data Confirmation -| \n\n"RESET);
        printf(CYAN"\n\n"RESET);
        printf(GREEN"Data updated successfully in memory.\n\n"RESET);
        printf(YELLOW"Do you want to save changes to CSV file?\n"RESET);
        printf(GREEN"1. Save to CSV file and go back\n"RESET);
        printf(RED"2. Don't save to CSV (changes will be lost) and go back\n"RESET);
        printf(" --> Enter your choice (1-2): "YELLOW"");

        while (1) {
            fgets(save_input, sizeof(save_input), stdin);
            int save_choice = atoi(save_input);

            if (save_choice == 1) {
                printf(RESET);
                int save_result = save_file();
                cls();
                printf(CYAN"\n\n");
                printf(BLUE"|- Update Data Confirmation -| \n\n"RESET);
                printf(CYAN"\n\n"RESET);
                if (save_result == 0) {
                    printf(GREEN"Changes saved to CSV file successfully.\n"RESET);
                    printf(GREEN"Data updated permanently.\n"RESET);
                } else {
                    printf(RED"Error saving to CSV file!\n"RESET);
                }
                enter_to_back();
                return;
            } 
            else if (save_choice == 2) {
                printf(RESET);
                // Restore original data
                free(products);
                products = NULL;
                product_count = 0;
                product_capacity = 0;
                read_data();
                cls();
                printf(CYAN"\n\n");
                printf(BLUE"|- Update Data Confirmation -| \n\n"RESET);
                printf(CYAN"\n\n"RESET);
                printf(YELLOW"Changes NOT saved to CSV file.\n"RESET);
                printf(YELLOW"Data restored from file. Nothing was permanently updated.\n"RESET);
                enter_to_back();
                return;
            }
            else {
                printf(RED"Invalid choice. Please enter 1 or 2: "YELLOW"");
            }
        }
    } else {
        printf(RESET);
        printf(GREEN"Update cancelled. No data was modified.\n"RESET);
        enter_to_back();
        return;
    }
}

// Summary the data in csv
void list_with_total(){
    cls();
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("--------------------------------------\n\n");
        printf(RED"\t%s\n\n", check_file());
        printf(RESET"--------------------------------------\n");
        enter_to_back();
        return;
    }
    
    printf("----------------------------------------------------------\n");
    printf(GREEN"\t      LIST CSV WITH TOTAL SUMMARY\n"RESET);
    printf("----------------------------------------------------------\n");
    printf(YELLOW"%-10s %-20s %-10s %-10s\n"RESET, "OrderID", "ProductName", "Quantity", "TotalPrice");
    printf("----------------------------------------------------------\n");
    
    int grand_total = 0;
    int total_quantity = 0;
    
    for (int i = 0; i < product_count; i++){
        printf(""RESET"%-10s "MAGENTA"%-20s "BLUE"%-10d "CYAN"%-10d\n"RESET"",
                products[i].OrderID, products[i].ProductName, products[i].Quantitiy, products[i].TotalPrice);
        grand_total += products[i].TotalPrice;
        total_quantity += products[i].Quantitiy;
    }
    
    printf("----------------------------------------------------------\n");
    printf(GREEN"SUMMARY:\n"RESET);
    printf("----------------------------------------------------------\n");
    printf(YELLOW"Total Records:    %-10d\n"RESET, product_count);
    printf(BLUE"Total Quantity:   %-10d\n"RESET, total_quantity);
    printf(CYAN"Grand Total Price: %-10d\n"RESET, grand_total);
    printf("----------------------------------------------------------\n");
    
    if (product_count > 0) {
        int average_price = grand_total / product_count;
        printf(MAGENTA"Average Price per Order: %-10d\n"RESET, average_price);
        printf("----------------------------------------------------------\n");
    }
    
    enter_to_back();
}



// UI Program Function




void WDUMenu(){
    char input[256];
    while (1)
    {
        cls();
        printf(RESET);
        printf(CYAN"\n\n"RESET);
        printf(BLUE" |- Update Data / Search Data -|\n\n"RESET);
        printf(CYAN"\n"RESET);
        printf(CYAN"------------------------------------------\n"RESET);
        printf(YELLOW"1."RESET" ADD Data\n");
        printf(YELLOW"2."RESET" Delete Data\n");
        printf(YELLOW"3."RESET" Search Data\n");
        printf(YELLOW"4."RESET" Update data\n");
        printf(YELLOW"5."RESET" List Data IN CSV\n");
        printf(RED"9."RESET" !! Go Back !!\n");
        printf(CYAN"------------------------------------------\n\n"RESET);
        printf("\t     [Current Edit CSV]\n\n");
        printf("%s\n", check_file());
        printf(CYAN"==========================================\n"RESET);
        printf(" --> Enter your choice (1-5,9): ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
                WDUChoice(input);
            } else {
                invalid();
                continue;
            }
    }
}


// Base Menu v6
void menu(){
    cls();
    printf(RESET);
    printf(CYAN"\n\n");
    printf(BLUE" |- Welcome To Online Order Management System -| \n\n"RESET);
    printf(CYAN"\n"RESET);
    printf(RESET"\t     --|Menu Panel|--\n\n");
    printf(CYAN"------------------------------------------\n"RESET);
    printf(YELLOW"1."RESET" List Data IN CSV\n");
    printf(YELLOW"2."RESET" Change file CSV\n");
    printf(YELLOW"3."RESET" Update Data / Search Data\n");
    printf(YELLOW"4."RESET" List File in Folder\n");
    printf(YELLOW"5."RESET" Run Unit Tests\n");
    printf(YELLOW"6."RESET" Run E2E Tests\n");
    printf(RED"9."RESET" !! Exit The Program !!\n");
    printf(CYAN"------------------------------------------\n\n"RESET);
    printf("\t     [Current Read CSV]\n\n");
    printf("%s\n", check_file());
    printf(CYAN"==========================================\n"RESET);
    printf(" --> Enter your choice (1-6,9): ");
}

// invalid input function enter to back
void invalid(){
    cls();
    printf("\n");
    printf(RED"!!! Invalid choice. Please try again. !!!\\n");
    printf("Press "RED"Enter"RESET" to go back...\n");
    getchar();
}

// exit the program
void p_quit(){
    char input[1024];
    int choice;
    cls();
    printf("\n");
    printf(YELLOW"\t      Exit The Program \n\n"RESET);
    printf(RESET"\n-------------------------------------------\n\n");
    printf("You want to"YELLOW" continue ?\n"RESET"");
    printf(GREEN"1. Continue\n"RESET);
    printf(RED"2. Go Back\n\n"RESET);
    printf("-------------------------------------------\n");
    printf(" --> Enter your choice (1-2): "YELLOW);
    if (fgets(input,sizeof(input), stdin) != NULL ){
        printf(RESET);
        choice = atoi(input);
        if(choice == 1){
            exit(0);
        }
        else if(choice == 2){
            main();
        }
        else{
            invalid();
            p_quit();
        }
    }
    else{
        invalid();
        p_quit();
    }
}

//function enter to back
void enter_to_back(){
    printf("\nPress "RED"Enter"RESET" to go back...");
    getchar();
}

//List all file in folder data
void ListFileFolder(){
    printf(RESET);
    printf("===============================================\n\n"YELLOW);
    printf(RESET"\t  |- "YELLOW"File in Folder"RESET" -|\n\n");
    printf("===============================================\n\n"GREEN);
    #ifdef _WIN32
    system("dir /b ..\\data\\*.csv");
    #else
    system("ls ../data/*.csv");
    #endif
    printf(RESET"\n===============================================\n\n");
}

//for set loop not all in main
void start(){
    cls();
}


//loop the function HARDCODE
void loop2(){
    cls();
}

// clear terminal
void cls(){
    system("cls||clear");
}


//startup
void setup_console() {
#ifdef _WIN32
    // Enable UTF-8 output for Windows
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // Alternative method for older Windows versions
    system("chcp 65001 > nul");
#else
    // Enable UTF-8 for Linux/Unix systems
    setlocale(LC_ALL, "");
    setlocale(LC_CTYPE, "C.UTF-8");
#endif
}