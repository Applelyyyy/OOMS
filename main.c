// 1. Save Data TO CSV
// 2. List Data IN CSV {Print all in csv}
// 3. Add Data TO CSV
// 4. Search Data
// 5. Update Data {in temp file csv}
// 6. Delete Data
// 7. Back To Menu


// DONE Menu v0.1
// TO DO Menu v0.5
//-----------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//-----------------
// SETUP FUNCTION
void cls();
void delay_q(int d_value);
void delay(int delay);
void menu();
void invalid();
//-----------------

// main v0.1
int main() {
    int choice;
    do {
        menu();
        if (scanf("%d",&choice) != 1) {
            invalid();
            while (getchar() != '\n');
            continue;
        }
        switch (choice)
        {
        case 1:
            break;
        case 2:
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

//load data in 


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

// Menu v0.5
void menu(){
    cls();
    printf("==========================================\n");
    printf("Welcome To Online Order Management System \n");
    printf("==========================================\n\n");
    printf("1. Save Data TO CSV\n");
    printf("2. List Data IN CSV\n");
    printf("3. Add Data TO CSV\n");
    printf("4. Search Data\n");
    printf("5. Update Data\n");
    printf("6. Delete Data\n");
    printf("7. Back To Menu\n");
    printf("8. !! Exit The Program !!\n\n");
    printf("==========================================\n");
    printf(" --> Enter your choice (1-8): ");
}

// error input function
void invalid(){
    cls();
    printf("\n");
    printf("******************************************\n");
    printf("!!! Invalid choice. Please try again. !!!\n");
    printf("******************************************\n");
    delay(1.5);
}

