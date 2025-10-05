#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// External variables and functions from main.c
extern char *filename;
extern char *csv_default;
extern struct data_csv_st {
    char OrderID[50];
    char ProductName[100];
    int Quantitiy;
    int TotalPrice;
} *products;
extern int product_count;
extern int product_capacity;

extern int read_data();
extern int save_file();
extern void cls();
extern void enter_to_back();

// ANSI colors
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define BOLD    "\x1b[1m"

typedef struct {
    int test_number;
    char test_name[100];
    int passed;
    char error_message[200];
    double execution_time;
} E2ETestResult;

E2ETestResult e2e_results[10];
int e2e_test_count = 0;

// Helper function to create test CSV file
void create_e2e_test_file() {
    char *original_filename = filename;
    char test_filename[] = "../data/e2e_test.csv";
    
    // Create the file
    FILE *file = fopen(test_filename, "w");
    if (file == NULL) {
        printf(RED"Error: Could not create test file %s\n"RESET, test_filename);
        return;
    }
    
    fprintf(file, "%s\n", csv_default);
    fprintf(file, "E2E001,Initial Product,5,500\n");
    fprintf(file, "E2E002,Second Product,3,300\n");
    fclose(file);
    
    // Temporarily change filename and reload data
    filename = test_filename;
    
    // Free existing products
    if (products != NULL) {
        free(products);
        products = NULL;
    }
    product_count = 0;
    product_capacity = 0;
    
    // Read the test data
    read_data();
    
    // Don't restore filename yet - let tests use the test file
}

// Helper function to simulate add operation
int simulate_add_data(const char* orderid, const char* productname, int quantity, int totalprice) {
    clock_t start = clock();
    
    // Validate inputs
    if (!orderid || !productname || quantity <= 0 || totalprice <= 0) {
        clock_t end = clock();
        e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        strcpy(e2e_results[e2e_test_count].error_message, "Invalid input parameters");
        return -1;
    }
    
    // Store original product count
    int original_count = product_count;
    
    // Check for duplicate OrderID
    for(int i = 0; i < product_count; i++){
        if(strcmp(products[i].OrderID, orderid) == 0){
            clock_t end = clock();
            e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
            strcpy(e2e_results[e2e_test_count].error_message, "Duplicate OrderID detected correctly");
            return 1; // Duplicate found - this is expected behavior
        }
    }
    
    // Expand capacity if needed
    if (product_capacity <= product_count){
        int new_capacity = (product_capacity == 0) ? 10 : product_capacity * 2;
        struct data_csv_st *new_products = realloc(products, new_capacity * sizeof(struct data_csv_st));
        if(!new_products){
            clock_t end = clock();
            e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
            strcpy(e2e_results[e2e_test_count].error_message, "Memory allocation failed");
            return -1;
        }
        products = new_products;
        product_capacity = new_capacity;
    }
    
    // Add the data
    strncpy(products[product_count].OrderID, orderid, sizeof(products[product_count].OrderID) - 1);
    products[product_count].OrderID[sizeof(products[product_count].OrderID) - 1] = '\0';
    
    strncpy(products[product_count].ProductName, productname, sizeof(products[product_count].ProductName) - 1);
    products[product_count].ProductName[sizeof(products[product_count].ProductName) - 1] = '\0';
    
    products[product_count].Quantitiy = quantity;
    products[product_count].TotalPrice = totalprice;
    product_count++;
    
    clock_t end = clock();
    e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Verify the data was added correctly
    if (product_count == original_count + 1 &&
        strcmp(products[product_count-1].OrderID, orderid) == 0 &&
        strcmp(products[product_count-1].ProductName, productname) == 0 &&
        products[product_count-1].Quantitiy == quantity &&
        products[product_count-1].TotalPrice == totalprice) {
        strcpy(e2e_results[e2e_test_count].error_message, "Data added successfully");
        return 0;
    } else {
        strcpy(e2e_results[e2e_test_count].error_message, "Data verification failed");
        return -1;
    }
}

// Helper function to simulate delete operation
int simulate_delete_data(const char* orderid_to_delete) {
    clock_t start = clock();
    
    if (!orderid_to_delete) {
        clock_t end = clock();
        e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        strcpy(e2e_results[e2e_test_count].error_message, "Invalid OrderID parameter");
        return -1;
    }
    
    int original_count = product_count;
    int found_index = -1;
    
    // Find the item to delete
    for (int i = 0; i < product_count; i++) {
        if (strcmp(products[i].OrderID, orderid_to_delete) == 0) {
            found_index = i;
            break;
        }
    }
    
    if (found_index == -1) {
        clock_t end = clock();
        e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        strcpy(e2e_results[e2e_test_count].error_message, "OrderID not found");
        return 1; // Not found
    }
    
    // Shift elements to remove the item
    for (int j = found_index; j < product_count - 1; j++) {
        products[j] = products[j + 1];
    }
    product_count--;
    
    // Fixed capacity management - don't change product_capacity to product_count
    if (product_count == 0) {
        if (products != NULL) {
            free(products);
            products = NULL;
        }
        product_capacity = 0;
    }
    // If product_count > 0, keep the original capacity
    
    clock_t end = clock();
    e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Verify deletion
    if (product_count == original_count - 1) {
        strcpy(e2e_results[e2e_test_count].error_message, "Data deleted successfully");
        return 0;
    } else {
        strcpy(e2e_results[e2e_test_count].error_message, "Deletion verification failed");
        return -1;
    }
}

// E2E Test 1: Add single valid record
void e2e_test_add_single_record() {
    e2e_results[e2e_test_count].test_number = e2e_test_count + 1;
    strcpy(e2e_results[e2e_test_count].test_name, "Add Single Valid Record");
    
    create_e2e_test_file();
    
    int result = simulate_add_data("E2E003", "New Test Product", 10, 1000);
    
    if (result == 0) {
        e2e_results[e2e_test_count].passed = 1;
    } else {
        e2e_results[e2e_test_count].passed = 0;
    }
    
    e2e_test_count++;
}

// E2E Test 2: Add duplicate OrderID (should fail)
void e2e_test_add_duplicate_orderid() {
    e2e_results[e2e_test_count].test_number = e2e_test_count + 1;
    strcpy(e2e_results[e2e_test_count].test_name, "Add Duplicate OrderID");
    
    int result = simulate_add_data("E2E001", "Duplicate Product", 5, 500);
    
    if (result == 1) { // Should return 1 for duplicate
        e2e_results[e2e_test_count].passed = 1;
    } else {
        e2e_results[e2e_test_count].passed = 0;
    }
    
    e2e_test_count++;
}

// E2E Test 3: Add multiple records
void e2e_test_add_multiple_records() {
    e2e_results[e2e_test_count].test_number = e2e_test_count + 1;
    strcpy(e2e_results[e2e_test_count].test_name, "Add Multiple Records");
    
    clock_t start = clock();
    int original_count = product_count;
    int success_count = 0;
    
    // Reset test count temporarily to avoid interfering with timing
    int temp_test_count = e2e_test_count;
    
    // Add 3 different records
    e2e_test_count = temp_test_count; // Reset for each call
    if (simulate_add_data("E2E004", "Product Four", 4, 400) == 0) success_count++;
    e2e_test_count = temp_test_count;
    if (simulate_add_data("E2E005", "Product Five", 5, 500) == 0) success_count++;
    e2e_test_count = temp_test_count;
    if (simulate_add_data("E2E006", "Product Six", 6, 600) == 0) success_count++;
    
    e2e_test_count = temp_test_count; // Restore
    
    clock_t end = clock();
    e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    if (success_count == 3 && product_count == original_count + 3) {
        e2e_results[e2e_test_count].passed = 1;
        strcpy(e2e_results[e2e_test_count].error_message, "All 3 records added successfully");
    } else {
        e2e_results[e2e_test_count].passed = 0;
        snprintf(e2e_results[e2e_test_count].error_message, sizeof(e2e_results[e2e_test_count].error_message), 
                "Only %d/3 records added successfully", success_count);
    }
    
    e2e_test_count++;
}

// E2E Test 4: Delete existing record
void e2e_test_delete_existing_record() {
    e2e_results[e2e_test_count].test_number = e2e_test_count + 1;
    strcpy(e2e_results[e2e_test_count].test_name, "Delete Existing Record");
    
    int result = simulate_delete_data("E2E002");
    
    if (result == 0) {
        e2e_results[e2e_test_count].passed = 1;
    } else {
        e2e_results[e2e_test_count].passed = 0;
    }
    
    e2e_test_count++;
}

// E2E Test 5: Delete non-existing record
void e2e_test_delete_nonexisting_record() {
    e2e_results[e2e_test_count].test_number = e2e_test_count + 1;
    strcpy(e2e_results[e2e_test_count].test_name, "Delete Non-existing Record");
    
    int result = simulate_delete_data("NONEXIST");
    
    if (result == 1) { // Should return 1 for not found
        e2e_results[e2e_test_count].passed = 1;
    } else {
        e2e_results[e2e_test_count].passed = 0;
    }
    
    e2e_test_count++;
}

// E2E Test 6: Add then Delete workflow
void e2e_test_add_then_delete_workflow() {
    e2e_results[e2e_test_count].test_number = e2e_test_count + 1;
    strcpy(e2e_results[e2e_test_count].test_name, "Add-Then-Delete Workflow");
    
    clock_t start = clock();
    int original_count = product_count;
    
    // Temporarily save test count
    int temp_test_count = e2e_test_count;
    
    // Add a record
    e2e_test_count = temp_test_count;
    int add_result = simulate_add_data("WORKFLOW", "Workflow Test", 1, 100);
    
    if (add_result != 0) {
        e2e_test_count = temp_test_count; // Restore
        clock_t end = clock();
        e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        e2e_results[e2e_test_count].passed = 0;
        strcpy(e2e_results[e2e_test_count].error_message, "Failed to add record");
        e2e_test_count++;
        return;
    }
    
    // Delete the same record
    e2e_test_count = temp_test_count;
    int delete_result = simulate_delete_data("WORKFLOW");
    
    e2e_test_count = temp_test_count; // Restore
    
    clock_t end = clock();
    e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    if (delete_result == 0 && product_count == original_count) {
        e2e_results[e2e_test_count].passed = 1;
        strcpy(e2e_results[e2e_test_count].error_message, "Add-Delete workflow completed successfully");
    } else {
        e2e_results[e2e_test_count].passed = 0;
        strcpy(e2e_results[e2e_test_count].error_message, "Workflow verification failed");
    }
    
    e2e_test_count++;
}

// E2E Test 7: Stress test - Add and Delete multiple records
void e2e_test_stress_add_delete() {
    e2e_results[e2e_test_count].test_number = e2e_test_count + 1;
    strcpy(e2e_results[e2e_test_count].test_name, "Stress Test - Multiple Add/Delete");
    
    clock_t start = clock();
    int original_count = product_count;
    int add_operations_success = 0;
    int delete_operations_success = 0;
    int total_operations = 15;  // 10 adds + 5 deletes
    
    // Temporarily save test count
    int temp_test_count = e2e_test_count;
    
    // Keep track of successfully added OrderIDs
    char successful_adds[10][20];
    int successful_add_count = 0;
    
    // Add 10 records
    for (int i = 1; i <= 10; i++) {
        char orderid[20];
        char productname[50];
        snprintf(orderid, sizeof(orderid), "STRESS%02d", i);
        snprintf(productname, sizeof(productname), "Stress Product %d", i);
        
        e2e_test_count = temp_test_count;
        if (simulate_add_data(orderid, productname, i, i * 100) == 0) {
            add_operations_success++;
            strcpy(successful_adds[successful_add_count], orderid);
            successful_add_count++;
        }
    }
    
    // Only delete the first 5 successfully added records
    int delete_attempts = (successful_add_count >= 5) ? 5 : successful_add_count;
    for (int i = 0; i < delete_attempts; i++) {
        e2e_test_count = temp_test_count;
        if (simulate_delete_data(successful_adds[i]) == 0) {
            delete_operations_success++;
        }
    }
    
    e2e_test_count = temp_test_count; // Restore
    
    clock_t end = clock();
    e2e_results[e2e_test_count].execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Calculate expected results
    int expected_operations = add_operations_success + delete_operations_success;
    int expected_count = original_count + add_operations_success - delete_operations_success;
    
    if (add_operations_success == 10 && delete_operations_success == delete_attempts && 
        product_count == expected_count) {
        e2e_results[e2e_test_count].passed = 1;
        snprintf(e2e_results[e2e_test_count].error_message, sizeof(e2e_results[e2e_test_count].error_message),
                "All operations successful: %d adds, %d deletes, final count: %d", 
                add_operations_success, delete_operations_success, product_count);
    } else {
        e2e_results[e2e_test_count].passed = 0;
        snprintf(e2e_results[e2e_test_count].error_message, sizeof(e2e_results[e2e_test_count].error_message),
                "Adds: %d/10, Deletes: %d/%d, Expected count: %d, Actual: %d", 
                add_operations_success, delete_operations_success, delete_attempts, expected_count, product_count);
    }
    
    e2e_test_count++;
}

// Main E2E test runner
void run_e2e_tests() {
    cls();
    printf(MAGENTA BOLD "\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                     E2E TEST SUITE                            ║\n");
    printf("║            End-to-End Tests for Add/Delete Functions          ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    printf(RESET);
    
    // Store original filename to restore later
    char *original_filename = filename;
    
    // Initialize test environment
    e2e_test_count = 0;
    
    printf(CYAN "\n🚀 Starting E2E Tests...\n\n" RESET);
    
    // Run all E2E tests
    e2e_test_add_single_record();
    e2e_test_add_duplicate_orderid();
    e2e_test_add_multiple_records();
    e2e_test_delete_existing_record();
    e2e_test_delete_nonexisting_record();
    e2e_test_add_then_delete_workflow();
    e2e_test_stress_add_delete();
    
    // Display results
    printf(BLUE BOLD "\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                        E2E TEST RESULTS                        ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n" RESET);
    
    int passed_tests = 0;
    double total_time = 0;
    
    for (int i = 0; i < e2e_test_count; i++) {
        if (e2e_results[i].passed) {
            printf(GREEN "✓ " RESET);
            passed_tests++;
        } else {
            printf(RED "✗ " RESET);
        }
        
        printf("Test %2d: %-30s (%.3fs)\n", 
               e2e_results[i].test_number, 
               e2e_results[i].test_name, 
               e2e_results[i].execution_time);
        
        if (strlen(e2e_results[i].error_message) > 0) {
            printf("         " YELLOW "%s" RESET "\n", e2e_results[i].error_message);
        }
        
        total_time += e2e_results[i].execution_time;
    }
    
    printf("\n" BLUE "═══════════════════════════════════════════════════════════════\n" RESET);
    printf(BOLD "E2E TEST SUMMARY:\n" RESET);
    printf("Total Tests: %d\n", e2e_test_count);
    printf("Tests Passed: " GREEN "%d" RESET "\n", passed_tests);
    printf("Tests Failed: " RED "%d" RESET "\n", e2e_test_count - passed_tests);
    printf("Success Rate: %.1f%%\n", e2e_test_count > 0 ? (double)passed_tests / e2e_test_count * 100 : 0);
    printf("Total Execution Time: %.3f seconds\n", total_time);
    printf("Current Product Count: %d\n", product_count);
    
    if (passed_tests == e2e_test_count) {
        printf("\n" GREEN BOLD "🎉 ALL E2E TESTS PASSED! 🎉\n" RESET);
    } else {
        printf("\n" RED BOLD "⚠️  %d E2E TEST(S) FAILED\n" RESET, e2e_test_count - passed_tests);
    }
    
    printf(BLUE "═══════════════════════════════════════════════════════════════\n" RESET);
    
    // Clean up test file
    remove("../data/e2e_test.csv");
    
    // Restore original filename and reload data
    filename = original_filename;
    if (products != NULL) {
        free(products);
        products = NULL;
    }
    product_count = 0;
    product_capacity = 0;
    read_data();
    
    enter_to_back();
}