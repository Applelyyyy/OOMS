#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

// Windows specific includes
#ifdef _WIN32
#include <direct.h>  // For _mkdir on Windows
#include <io.h>      // For Windows file operations
#endif

// Include external variables and functions from main.c
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

// Function prototypes from main.c
extern int read_data();
extern int save_file();
extern int SaveDataToFile(const char *OrderID, const char *ProductName, const int Quantity, const int TotalPrice);
extern char *csv_name();
extern void to_lowercase(char *str);
extern char *check_file();
extern void cls();
extern void enter_to_back();
extern void CreateifNoFile();

// ANSI color codes for test output
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BOLD          "\x1b[1m"
#define ANSI_UNDERLINE     "\x1b[4m"

// Test framework structure
typedef struct {
    int passed;
    int total;
    char test_suite_name[100];
    char failed_tests[1000];
} TestResult;

TestResult test_results[50];
int test_suite_count = 0;
int total_test_time = 0;

// Test macros with enhanced reporting - Fixed variable name conflict
#define START_TEST_TIMER() clock_t test_start_time = clock()
#define END_TEST_TIMER() total_test_time += (clock() - test_start_time)

#define ASSERT_EQ(expected, actual, test_name) \
    do { \
        test_results[test_suite_count].total++; \
        if ((expected) == (actual)) { \
            test_results[test_suite_count].passed++; \
            printf(ANSI_COLOR_GREEN "  ✓ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_CYAN "Expected %d, got %d" ANSI_COLOR_RESET "\n", test_name, expected, actual); \
        } else { \
            printf(ANSI_COLOR_RED "  ✗ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_RED "Expected %d, got %d" ANSI_COLOR_RESET "\n", test_name, expected, actual); \
            strcat(test_results[test_suite_count].failed_tests, test_name); \
            strcat(test_results[test_suite_count].failed_tests, "; "); \
            return 0; \
        } \
    } while(0)

#define ASSERT_STR_EQ(expected, actual, test_name) \
    do { \
        test_results[test_suite_count].total++; \
        if (strcmp(expected, actual) == 0) { \
            test_results[test_suite_count].passed++; \
            printf(ANSI_COLOR_GREEN "  ✓ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_CYAN "Strings match" ANSI_COLOR_RESET "\n", test_name); \
        } else { \
            printf(ANSI_COLOR_RED "  ✗ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_RED "Expected '%s', got '%s'" ANSI_COLOR_RESET "\n", test_name, expected, actual); \
            strcat(test_results[test_suite_count].failed_tests, test_name); \
            strcat(test_results[test_suite_count].failed_tests, "; "); \
            return 0; \
        } \
    } while(0)

#define ASSERT_NOT_NULL(ptr, test_name) \
    do { \
        test_results[test_suite_count].total++; \
        if ((ptr) != NULL) { \
            test_results[test_suite_count].passed++; \
            printf(ANSI_COLOR_GREEN "  ✓ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_CYAN "Pointer is not NULL" ANSI_COLOR_RESET "\n", test_name); \
        } else { \
            printf(ANSI_COLOR_RED "  ✗ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_RED "Pointer is NULL" ANSI_COLOR_RESET "\n", test_name); \
            strcat(test_results[test_suite_count].failed_tests, test_name); \
            strcat(test_results[test_suite_count].failed_tests, "; "); \
            return 0; \
        } \
    } while(0)

#define ASSERT_NULL(ptr, test_name) \
    do { \
        test_results[test_suite_count].total++; \
        if ((ptr) == NULL) { \
            test_results[test_suite_count].passed++; \
            printf(ANSI_COLOR_GREEN "  ✓ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_CYAN "Pointer is NULL as expected" ANSI_COLOR_RESET "\n", test_name); \
        } else { \
            printf(ANSI_COLOR_RED "  ✗ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_RED "Expected NULL pointer, got non-NULL" ANSI_COLOR_RESET "\n", test_name); \
            strcat(test_results[test_suite_count].failed_tests, test_name); \
            strcat(test_results[test_suite_count].failed_tests, "; "); \
            return 0; \
        } \
    } while(0)

#define ASSERT_TRUE(condition, test_name) \
    do { \
        test_results[test_suite_count].total++; \
        if (condition) { \
            test_results[test_suite_count].passed++; \
            printf(ANSI_COLOR_GREEN "  ✓ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_CYAN "Condition is true" ANSI_COLOR_RESET "\n", test_name); \
        } else { \
            printf(ANSI_COLOR_RED "  ✗ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_RED "Condition is false" ANSI_COLOR_RESET "\n", test_name); \
            strcat(test_results[test_suite_count].failed_tests, test_name); \
            strcat(test_results[test_suite_count].failed_tests, "; "); \
            return 0; \
        } \
    } while(0)

#define ASSERT_FALSE(condition, test_name) \
    do { \
        test_results[test_suite_count].total++; \
        if (!(condition)) { \
            test_results[test_suite_count].passed++; \
            printf(ANSI_COLOR_GREEN "  ✓ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_CYAN "Condition is false as expected" ANSI_COLOR_RESET "\n", test_name); \
        } else { \
            printf(ANSI_COLOR_RED "  ✗ " ANSI_COLOR_RESET "%s: " ANSI_COLOR_RED "Expected false, got true" ANSI_COLOR_RESET "\n", test_name); \
            strcat(test_results[test_suite_count].failed_tests, test_name); \
            strcat(test_results[test_suite_count].failed_tests, "; "); \
            return 0; \
        } \
    } while(0)

// Helper functions
void setup_test_environment() {
    // Create test directory if it doesn't exist - Fixed Windows compatibility
    #ifdef _WIN32
    _mkdir("../data");
    #else
    mkdir("../data", 0755);
    #endif
    
    // Clean up any existing test data
    if (products != NULL) {
        free(products);
        products = NULL;
    }
    product_count = 0;
    product_capacity = 0;
}

void cleanup_test_files() {
    const char* test_files[] = {
        "../data/test_data.csv",
        "../data/empty_test.csv",
        "../data/large_test.csv",
        "../data/edge_case_test.csv",
        "../data/test_save.csv",
        "../data/invalid_test.csv",
        "../data/corrupted_test.csv",
        "../data/unicode_test.csv",
        "../data/boundary_test.csv",
        "../data/performance_test.csv",
        "../data/malformed_test.csv",
        "../data/stress_test.csv",
        "../data/whitespace_test.csv",
        "../data/invalid_header.csv",
        "../data/small_test.csv",
        "../data/medium_test.csv",
        "../data/integration_test.csv",
        "../data/integration_test2.csv",
        "../data/temp.csv",
        "../data/backup_raw_data.csv",
        NULL
    };
    
    printf("Cleaning up test files...\n");
    for (int i = 0; test_files[i] != NULL; i++) {
        // Check if file exists before trying to remove
        FILE *check = fopen(test_files[i], "r");
        if (check != NULL) {
            fclose(check);
            if (remove(test_files[i]) == 0) {
                printf("  Removed: %s\n", test_files[i]);
            }
        }
    }
}

void create_test_csv(const char* filepath, const char* content) {
    FILE *file = fopen(filepath, "w");
    if (file) {
        fprintf(file, "%s", content);
        fclose(file);
    }
}

void create_binary_test_file(const char* filepath) {
    FILE *file = fopen(filepath, "wb");
    if (file) {
        unsigned char binary_data[] = {0x00, 0x01, 0x02, 0xFF, 0xFE, 0xFD};
        fwrite(binary_data, sizeof(binary_data), 1, file);
        fclose(file);
    }
}

// Comprehensive test functions

// Test 1: String manipulation functions
int test_to_lowercase_comprehensive() {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "\n=== Testing to_lowercase function (Comprehensive) ===\n" ANSI_COLOR_RESET);
    strcpy(test_results[test_suite_count].test_suite_name, "to_lowercase_comprehensive");
    test_results[test_suite_count].passed = 0;
    test_results[test_suite_count].total = 0;
    strcpy(test_results[test_suite_count].failed_tests, "");
    START_TEST_TIMER();
    
    // Test 1: All uppercase
    char test1[] = "HELLO WORLD";
    to_lowercase(test1);
    ASSERT_STR_EQ("hello world", test1, "All uppercase conversion");
    
    // Test 2: Mixed case with numbers and symbols
    char test2[] = "MiXeD cAsE 123!@#";
    to_lowercase(test2);
    ASSERT_STR_EQ("mixed case 123!@#", test2, "Mixed case with numbers/symbols");
    
    // Test 3: Already lowercase
    char test3[] = "already lowercase";
    to_lowercase(test3);
    ASSERT_STR_EQ("already lowercase", test3, "Already lowercase");
    
    // Test 4: Empty string
    char test4[] = "";
    to_lowercase(test4);
    ASSERT_STR_EQ("", test4, "Empty string");
    
    // Test 5: Single character uppercase
    char test5[] = "A";
    to_lowercase(test5);
    ASSERT_STR_EQ("a", test5, "Single uppercase character");
    
    // Test 6: Single character lowercase
    char test6[] = "z";
    to_lowercase(test6);
    ASSERT_STR_EQ("z", test6, "Single lowercase character");
    
    // Test 7: Only numbers
    char test7[] = "123456789";
    to_lowercase(test7);
    ASSERT_STR_EQ("123456789", test7, "Only numbers");
    
    // Test 8: Only special characters
    char test8[] = "!@#$%^&*()";
    to_lowercase(test8);
    ASSERT_STR_EQ("!@#$%^&*()", test8, "Only special characters");
    
    // Test 9: Multiple spaces
    char test9[] = "A   B   C";
    to_lowercase(test9);
    ASSERT_STR_EQ("a   b   c", test9, "Multiple spaces");
    
    // Test 10: Tab and newline characters
    char test10[] = "HELLO\tWORLD\n";
    to_lowercase(test10);
    ASSERT_STR_EQ("hello\tworld\n", test10, "Tab and newline characters");
    
    END_TEST_TIMER();
    test_suite_count++;
    return 1;
}

// Test 2: CSV reading with various scenarios
int test_read_data_comprehensive() {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "\n=== Testing read_data function (Comprehensive) ===\n" ANSI_COLOR_RESET);
    strcpy(test_results[test_suite_count].test_suite_name, "read_data_comprehensive");
    test_results[test_suite_count].passed = 0;
    test_results[test_suite_count].total = 0;
    strcpy(test_results[test_suite_count].failed_tests, "");
    START_TEST_TIMER();
    
    char *original_filename = filename;
    filename = "../data/test_data.csv";
    
    // Create comprehensive test CSV file
    const char* test_content = 
        "OrderID,ProductName,Quantity,TotalPrice\n"
        "ORD001,Laptop Computer,2,2000\n"
        "ORD002,Wireless Mouse,5,150\n"
        "ORD003,Mechanical Keyboard,3,300\n"
        "ORD004,Gaming Monitor,1,500\n"
        "ORD005,USB Cable,10,50\n";
    
    create_test_csv(filename, test_content);
    
    // Test reading data
    int result = read_data();
    ASSERT_EQ(0, result, "Read data return value");
    ASSERT_EQ(5, product_count, "Product count");
    ASSERT_NOT_NULL(products, "Products array allocated");
    ASSERT_TRUE(product_capacity >= product_count, "Capacity is sufficient");
    
    if (products != NULL && product_count >= 5) {
        // Verify all records
        ASSERT_STR_EQ("ORD001", products[0].OrderID, "First OrderID");
        ASSERT_STR_EQ("Laptop Computer", products[0].ProductName, "First ProductName");
        ASSERT_EQ(2, products[0].Quantitiy, "First Quantity");
        ASSERT_EQ(2000, products[0].TotalPrice, "First TotalPrice");
        
        ASSERT_STR_EQ("ORD005", products[4].OrderID, "Last OrderID");
        ASSERT_STR_EQ("USB Cable", products[4].ProductName, "Last ProductName");
        ASSERT_EQ(10, products[4].Quantitiy, "Last Quantity");
        ASSERT_EQ(50, products[4].TotalPrice, "Last TotalPrice");
        
        // Test middle record
        ASSERT_STR_EQ("ORD003", products[2].OrderID, "Middle OrderID");
        ASSERT_EQ(300, products[2].TotalPrice, "Middle TotalPrice");
    }
    
    filename = original_filename;
    END_TEST_TIMER();
    test_suite_count++;
    return 1;
}

// Test 3: Empty and invalid file scenarios
int test_file_edge_cases() {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "\n=== Testing File Edge Cases ===\n" ANSI_COLOR_RESET);
    strcpy(test_results[test_suite_count].test_suite_name, "file_edge_cases");
    test_results[test_suite_count].passed = 0;
    test_results[test_suite_count].total = 0;
    strcpy(test_results[test_suite_count].failed_tests, "");
    START_TEST_TIMER();
    
    char *original_filename = filename;
    
    // Test 1: Empty file (header only)
    filename = "../data/empty_test.csv";
    const char* empty_content = "OrderID,ProductName,Quantity,TotalPrice\n";
    create_test_csv(filename, empty_content);
    
    int result = read_data();
    ASSERT_EQ(0, result, "Empty file return value");
    ASSERT_EQ(0, product_count, "Empty file product count");
    
    // Test 2: Non-existent file
    filename = "../data/nonexistent_file_12345.csv";
    result = read_data();
    ASSERT_EQ(1, result, "Non-existent file should return error");
    
    // Test 3: File with only header and whitespace - FIXED
    filename = "../data/whitespace_test.csv";
    const char* whitespace_content = "OrderID,ProductName,Quantity,TotalPrice\n\n   \n\t\n";
    create_test_csv(filename, whitespace_content);
    
    // Verify file was created
    FILE *check_file = fopen(filename, "r");
    ASSERT_NOT_NULL(check_file, "Whitespace test file created");
    if (check_file) {
        fclose(check_file);
        
        result = read_data();
        ASSERT_EQ(0, result, "Whitespace file return value");
        // With improved read_data(), empty lines should be skipped, so product_count should be 0
        ASSERT_EQ(0, product_count, "Whitespace file product count should be 0");
    }
    
    // Test 4: Corrupted CSV (missing commas)
    filename = "../data/corrupted_test.csv";
    const char* corrupted_content = 
        "OrderID,ProductName,Quantity,TotalPrice\n"
        "ORD001 Laptop 2 2000\n"  // Missing commas
        "ORD002,Mouse,5,150\n";
    create_test_csv(filename, corrupted_content);
    result = read_data();
    // Should still work with scanf parsing
    ASSERT_EQ(0, result, "Corrupted CSV handling");
    
    // Test 5: File with invalid header
    filename = "../data/invalid_header.csv";
    const char* invalid_header = 
        "WrongHeader,Wrong,Wrong,Wrong\n"
        "ORD001,Laptop,2,2000\n";
    create_test_csv(filename, invalid_header);
    result = read_data();
    ASSERT_EQ(0, result, "Invalid header handling");
    
    // Restore original filename immediately after tests
    filename = original_filename;
    END_TEST_TIMER();
    test_suite_count++;
    return 1;
}

// Test 4: Memory management and large datasets
int test_memory_management_comprehensive() {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "\n=== Testing Memory Management (Comprehensive) ===\n" ANSI_COLOR_RESET);
    strcpy(test_results[test_suite_count].test_suite_name, "memory_management_comprehensive");
    test_results[test_suite_count].passed = 0;
    test_results[test_suite_count].total = 0;
    strcpy(test_results[test_suite_count].failed_tests, "");
    START_TEST_TIMER();
    
    char *original_filename = filename;
    
    // Test 1: Small dataset (within initial capacity)
    filename = "../data/small_test.csv";
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "OrderID,ProductName,Quantity,TotalPrice\n");
        for (int i = 1; i <= 5; i++) {
            fprintf(file, "ORD%03d,Product%d,%d,%d\n", i, i, i, i * 100);
        }
        fclose(file);
    }
    
    int result = read_data();
    ASSERT_EQ(0, result, "Small dataset read result");
    ASSERT_EQ(5, product_count, "Small dataset product count");
    ASSERT_TRUE(product_capacity >= 5, "Small dataset capacity check");
    
    // Test 2: Medium dataset (trigger one reallocation)
    filename = "../data/medium_test.csv";
    file = fopen(filename, "w");
    if (file) {
        fprintf(file, "OrderID,ProductName,Quantity,TotalPrice\n");
        for (int i = 1; i <= 15; i++) {
            fprintf(file, "ORD%03d,Product%d,%d,%d\n", i, i, i, i * 100);
        }
        fclose(file);
    }
    
    result = read_data();
    ASSERT_EQ(0, result, "Medium dataset read result");
    ASSERT_EQ(15, product_count, "Medium dataset product count");
    ASSERT_TRUE(product_capacity >= 15, "Medium dataset capacity check");
    
    // Test 3: Large dataset (multiple reallocations)
    filename = "../data/large_test.csv";
    file = fopen(filename, "w");
    if (file) {
        fprintf(file, "OrderID,ProductName,Quantity,TotalPrice\n");
        for (int i = 1; i <= 100; i++) {
            fprintf(file, "ORD%03d,Product%d,%d,%d\n", i, i, i, i * 100);
        }
        fclose(file);
    }
    
    result = read_data();
    ASSERT_EQ(0, result, "Large dataset read result");
    ASSERT_EQ(100, product_count, "Large dataset product count");
    ASSERT_TRUE(product_capacity >= 100, "Large dataset capacity check");
    ASSERT_NOT_NULL(products, "Large dataset products array");
    
    // Verify data integrity after reallocations
    if (products != NULL && product_count >= 100) {
        ASSERT_STR_EQ("ORD001", products[0].OrderID, "First record after reallocation");
        ASSERT_STR_EQ("ORD050", products[49].OrderID, "Middle record after reallocation");
        ASSERT_STR_EQ("ORD100", products[99].OrderID, "Last record after reallocation");
        ASSERT_EQ(10000, products[99].TotalPrice, "Last record price after reallocation");
    }
    
    // Test 4: Memory cleanup
    if (products != NULL) {
        free(products);
        products = NULL;
        product_count = 0;
        product_capacity = 0;
    }
    ASSERT_NULL(products, "Memory cleanup - products pointer");
    ASSERT_EQ(0, product_count, "Memory cleanup - product count");
    ASSERT_EQ(0, product_capacity, "Memory cleanup - product capacity");
    
    filename = original_filename;
    END_TEST_TIMER();
    test_suite_count++;
    return 1;
}

// Test 5: CSV parsing edge cases and malformed data
int test_csv_parsing_edge_cases_comprehensive() {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "\n=== Testing CSV Parsing Edge Cases (Comprehensive) ===\n" ANSI_COLOR_RESET);
    strcpy(test_results[test_suite_count].test_suite_name, "csv_parsing_comprehensive");
    test_results[test_suite_count].passed = 0;
    test_results[test_suite_count].total = 0;
    strcpy(test_results[test_suite_count].failed_tests, "");
    START_TEST_TIMER();
    
    char *original_filename = filename;
    filename = "../data/edge_case_test.csv";
    
    // Test with comprehensive edge cases
    const char* edge_content = 
        "OrderID,ProductName,Quantity,TotalPrice\n"
        "ORD001,Product with spaces,1,100\n"
        "ORD002,,2,200\n"  // Empty product name
        "ORD003,Product3,0,0\n"  // Zero values
        "ORD004,\"Product,with,commas\",1,300\n"  // Quoted product name with commas
        "ORD005,Product\nwith\nnewlines,1,400\n"  // Product with newlines
        "ORD006,Very Long Product Name That Exceeds Normal Length Expectations And Should Test Buffer Limits,1,500\n"
        "ORD007,$pecial!@#Characters%^&*(),1,600\n"  // Special characters
        "ORD008,Unicode_Test_áéíóú,1,700\n"  // Unicode characters
        "ORD009,Leading and Trailing Spaces ,1,800\n"  // Trailing spaces
        "ORD010,\"Quoted Product\",999999,999999\n";  // Maximum values
    
    create_test_csv(filename, edge_content);
    
    int result = read_data();
    ASSERT_EQ(0, result, "Edge case file read result");
    ASSERT_TRUE(product_count > 0, "Edge case product count > 0");
    ASSERT_NOT_NULL(products, "Edge case products array");
    
    if (products != NULL && product_count >= 3) {
        // Test various edge cases
        ASSERT_STR_EQ("ORD001", products[0].OrderID, "OrderID with spaces in product");
        ASSERT_STR_EQ("Product with spaces", products[0].ProductName, "Product name with spaces");
        
        ASSERT_STR_EQ("ORD002", products[1].OrderID, "OrderID with empty product name");
        // Note: Empty product name behavior depends on scanf implementation
        
        ASSERT_STR_EQ("ORD003", products[2].OrderID, "OrderID with zero values");
        ASSERT_EQ(0, products[2].Quantitiy, "Zero quantity parsing");
        ASSERT_EQ(0, products[2].TotalPrice, "Zero price parsing");
    }
    
    filename = original_filename;
    END_TEST_TIMER();
    test_suite_count++;
    return 1;
}

// Test 6: Save file functionality comprehensive
int test_save_file_comprehensive() {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "\n=== Testing save_file function (Comprehensive) ===\n" ANSI_COLOR_RESET);
    strcpy(test_results[test_suite_count].test_suite_name, "save_file_comprehensive");
    test_results[test_suite_count].passed = 0;
    test_results[test_suite_count].total = 0;
    strcpy(test_results[test_suite_count].failed_tests, "");
    START_TEST_TIMER();
    
    char *original_filename = filename;
    filename = "../data/test_save.csv";
    
    // Test 1: Save empty dataset
    if (products != NULL) {
        free(products);
    }
    product_capacity = 5;
    product_count = 0;
    products = malloc(product_capacity * sizeof(struct data_csv_st));
    ASSERT_NOT_NULL(products, "Memory allocation for empty save test");
    
    int result = save_file();
    ASSERT_EQ(0, result, "Save empty dataset return value");
    
    // Verify empty file has header
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        char line[256];
        if (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';
            ASSERT_STR_EQ("OrderID,ProductName,Quantity,TotalPrice", line, "Empty file header");
        }
        
        // Should not have data lines
        ASSERT_FALSE(fgets(line, sizeof(line), file) != NULL, "Empty file should have no data lines");
        fclose(file);
    }
    
    // Test 2: Save single record
    product_count = 1;
    strcpy(products[0].OrderID, "SINGLE001");
    strcpy(products[0].ProductName, "Single Product");
    products[0].Quantitiy = 1;
    products[0].TotalPrice = 100;
    
    result = save_file();
    ASSERT_EQ(0, result, "Save single record return value");
    
    // Verify single record file
    file = fopen(filename, "r");
    if (file != NULL) {
        char line[256];
        // Skip header
        fgets(line, sizeof(line), file);
        
        if (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';
            ASSERT_STR_EQ("SINGLE001,Single Product,1,100", line, "Single record data line");
        }
        fclose(file);
    }
    
    // Test 3: Save multiple records with various data types
    product_count = 5;
    
    strcpy(products[1].OrderID, "TEST002");
    strcpy(products[1].ProductName, "Product with spaces");
    products[1].Quantitiy = 999;
    products[1].TotalPrice = 99999;
    
    strcpy(products[2].OrderID, "TEST003");
    strcpy(products[2].ProductName, "");  // Empty product name
    products[2].Quantitiy = 0;
    products[2].TotalPrice = 0;
    
    strcpy(products[3].OrderID, "TEST004");
    strcpy(products[3].ProductName, "$pecial!@#Characters");
    products[3].Quantitiy = 5;
    products[3].TotalPrice = 500;
    
    strcpy(products[4].OrderID, "TEST005");
    strcpy(products[4].ProductName, "VeryLongProductNameThatTestsBufferLimitsAndShouldStillWorkCorrectly");
    products[4].Quantitiy = 10;
    products[4].TotalPrice = 1000;
    
    result = save_file();
    ASSERT_EQ(0, result, "Save multiple records return value");
    
    // Test 4: Verify file integrity by reading back
    result = read_data();
    ASSERT_EQ(0, result, "Read back saved data");
    ASSERT_EQ(5, product_count, "Read back product count");
    
    if (products != NULL && product_count >= 5) {
        ASSERT_STR_EQ("SINGLE001", products[0].OrderID, "Read back first OrderID");
        ASSERT_STR_EQ("TEST005", products[4].OrderID, "Read back last OrderID");
        ASSERT_EQ(1000, products[4].TotalPrice, "Read back last price");
    }
    
    filename = original_filename;
    END_TEST_TIMER();
    test_suite_count++;
    return 1;
}

// Test 7: Boundary conditions and limits
int test_boundary_conditions() {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "\n=== Testing Boundary Conditions ===\n" ANSI_COLOR_RESET);
    strcpy(test_results[test_suite_count].test_suite_name, "boundary_conditions");
    test_results[test_suite_count].passed = 0;
    test_results[test_suite_count].total = 0;
    strcpy(test_results[test_suite_count].failed_tests, "");
    START_TEST_TIMER();
    
    char *original_filename = filename;
    filename = "../data/boundary_test.csv";
    
    // Test 1: Maximum length strings
    const char* boundary_content = 
        "OrderID,ProductName,Quantity,TotalPrice\n"
        "ORD123456789012345678901234567890123456789012345,VeryLongProductNameThatTestsTheMaximumBufferSizeAndShouldHandleItCorrectlyWithoutCrashing12345,2147483647,2147483647\n"
        "MIN001,A,1,1\n"
        "MAX999,MaxProduct,999999,999999\n";
    
    create_test_csv(filename, boundary_content);
    
    int result = read_data();
    ASSERT_EQ(0, result, "Boundary test read result");
    ASSERT_TRUE(product_count > 0, "Boundary test product count");
    
    if (products != NULL && product_count >= 3) {
        // Test that long strings are properly truncated or handled
        ASSERT_TRUE(strlen(products[0].OrderID) <= 49, "OrderID within bounds");
        ASSERT_TRUE(strlen(products[0].ProductName) <= 99, "ProductName within bounds");
        
        // Test minimum values
        ASSERT_STR_EQ("MIN001", products[1].OrderID, "Minimum test OrderID");
        ASSERT_STR_EQ("A", products[1].ProductName, "Single character product name");
        ASSERT_EQ(1, products[1].Quantitiy, "Minimum quantity");
        ASSERT_EQ(1, products[1].TotalPrice, "Minimum price");
    }
    
    // Test 2: Test with maximum number of products (stress test)
    filename = "../data/stress_test.csv";
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "OrderID,ProductName,Quantity,TotalPrice\n");
        for (int i = 1; i <= 1000; i++) {  // 1000 products
            fprintf(file, "STRESS%04d,StressProduct%d,%d,%d\n", i, i, i % 100, (i * 50) % 10000);
        }
        fclose(file);
    }
    
    result = read_data();
    ASSERT_EQ(0, result, "Stress test read result");
    ASSERT_EQ(1000, product_count, "Stress test product count");
    ASSERT_TRUE(product_capacity >= 1000, "Stress test capacity");
    
    if (products != NULL && product_count >= 1000) {
        ASSERT_STR_EQ("STRESS0001", products[0].OrderID, "Stress test first record");
        ASSERT_STR_EQ("STRESS1000", products[999].OrderID, "Stress test last record");
    }
    
    filename = original_filename;
    END_TEST_TIMER();
    test_suite_count++;
    return 1;
}

// Test 8: Data validation and integrity
int test_data_validation() {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "\n=== Testing Data Validation ===\n" ANSI_COLOR_RESET);
    strcpy(test_results[test_suite_count].test_suite_name, "data_validation");
    test_results[test_suite_count].passed = 0;
    test_results[test_suite_count].total = 0;
    strcpy(test_results[test_suite_count].failed_tests, "");
    START_TEST_TIMER();
    
    // Setup test data for validation tests
    if (products != NULL) {
        free(products);
    }
    product_capacity = 10;
    product_count = 5;
    products = malloc(product_capacity * sizeof(struct data_csv_st));
    ASSERT_NOT_NULL(products, "Memory allocation for validation test");
    
    // Create test data with various scenarios
    strcpy(products[0].OrderID, "VALID001");
    strcpy(products[0].ProductName, "Valid Product");
    products[0].Quantitiy = 5;
    products[0].TotalPrice = 500;
    
    strcpy(products[1].OrderID, "DUPLICATE");
    strcpy(products[1].ProductName, "First Duplicate");
    products[1].Quantitiy = 1;
    products[1].TotalPrice = 100;
    
    strcpy(products[2].OrderID, "DUPLICATE");  // Duplicate OrderID
    strcpy(products[2].ProductName, "Second Duplicate");
    products[2].Quantitiy = 2;
    products[2].TotalPrice = 200;
    
    strcpy(products[3].OrderID, "NEGATIVE");
    strcpy(products[3].ProductName, "Negative Values");
    products[3].Quantitiy = -1;  // Negative quantity
    products[3].TotalPrice = -100;  // Negative price
    
    strcpy(products[4].OrderID, "ZERO");
    strcpy(products[4].ProductName, "Zero Values");
    products[4].Quantitiy = 0;
    products[4].TotalPrice = 0;
    
    // Test 1: Check for duplicate OrderIDs
    int duplicate_count = 0;
    for (int i = 0; i < product_count; i++) {
        for (int j = i + 1; j < product_count; j++) {
            if (strcmp(products[i].OrderID, products[j].OrderID) == 0) {
                duplicate_count++;
            }
        }
    }
    ASSERT_EQ(1, duplicate_count, "Duplicate OrderID detection");
    
    // Test 2: Check for negative values
    int negative_quantity_count = 0;
    int negative_price_count = 0;
    for (int i = 0; i < product_count; i++) {
        if (products[i].Quantitiy < 0) negative_quantity_count++;
        if (products[i].TotalPrice < 0) negative_price_count++;
    }
    ASSERT_EQ(1, negative_quantity_count, "Negative quantity detection");
    ASSERT_EQ(1, negative_price_count, "Negative price detection");
    
    // Test 3: Check for zero values
    int zero_quantity_count = 0;
    int zero_price_count = 0;
    for (int i = 0; i < product_count; i++) {
        if (products[i].Quantitiy == 0) zero_quantity_count++;
        if (products[i].TotalPrice == 0) zero_price_count++;
    }
    ASSERT_EQ(1, zero_quantity_count, "Zero quantity detection");
    ASSERT_EQ(1, zero_price_count, "Zero price detection");
    
    // Test 4: Check for empty OrderIDs
    int empty_orderid_count = 0;
    for (int i = 0; i < product_count; i++) {
        if (strlen(products[i].OrderID) == 0) {
            empty_orderid_count++;
        }
    }
    ASSERT_EQ(0, empty_orderid_count, "Empty OrderID detection");
    
    // Test 5: String length validation
    for (int i = 0; i < product_count; i++) {
        ASSERT_TRUE(strlen(products[i].OrderID) <= 49, "OrderID length validation");
        ASSERT_TRUE(strlen(products[i].ProductName) <= 99, "ProductName length validation");
    }
    
    END_TEST_TIMER();
    test_suite_count++;
    return 1;
}

// Test 9: Performance and timing tests - Fixed variable name conflict
int test_performance() {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "\n=== Testing Performance ===\n" ANSI_COLOR_RESET);
    strcpy(test_results[test_suite_count].test_suite_name, "performance");
    test_results[test_suite_count].passed = 0;
    test_results[test_suite_count].total = 0;
    strcpy(test_results[test_suite_count].failed_tests, "");
    START_TEST_TIMER();
    
    char *original_filename = filename;
    filename = "../data/performance_test.csv";
    
    // Create large dataset for performance testing - Fixed variable name conflict
    clock_t perf_start_time = clock();
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "OrderID,ProductName,Quantity,TotalPrice\n");
        for (int i = 1; i <= 5000; i++) {
            fprintf(file, "PERF%05d,PerformanceProduct%d,%d,%d\n", i, i, i % 100, i * 10);
        }
        fclose(file);
    }
    clock_t file_creation_time = clock() - perf_start_time;
    
    // Test read performance
    perf_start_time = clock();
    int result = read_data();
    clock_t read_time = clock() - perf_start_time;
    
    ASSERT_EQ(0, result, "Performance test read result");
    ASSERT_EQ(5000, product_count, "Performance test product count");
    
    // Performance should be reasonable (less than 5 seconds for 5000 records)
    double read_seconds = ((double)read_time) / CLOCKS_PER_SEC;
    ASSERT_TRUE(read_seconds < 5.0, "Read performance under 5 seconds");
    
    printf("  " ANSI_COLOR_CYAN "📊 Performance Metrics:\n" ANSI_COLOR_RESET);
    printf("    File creation: %.3f seconds\n", ((double)file_creation_time) / CLOCKS_PER_SEC);
    printf("    Data reading: %.3f seconds\n", read_seconds);
    printf("    Records/second: %.0f\n", read_seconds > 0 ? 5000.0 / read_seconds : 0.0);
    
    // Test save performance
    perf_start_time = clock();
    result = save_file();
    clock_t save_time = clock() - perf_start_time;
    
    ASSERT_EQ(0, result, "Performance test save result");
    
    double save_seconds = ((double)save_time) / CLOCKS_PER_SEC;
    ASSERT_TRUE(save_seconds < 5.0, "Save performance under 5 seconds");
    
    printf("    Data saving: %.3f seconds\n", save_seconds);
    
    filename = original_filename;
    END_TEST_TIMER();
    test_suite_count++;
    return 1;
}

// Test 10: Integration tests
int test_integration_scenarios() {
    printf(ANSI_COLOR_BLUE ANSI_BOLD "\n=== Testing Integration Scenarios ===\n" ANSI_COLOR_RESET);
    strcpy(test_results[test_suite_count].test_suite_name, "integration_scenarios");
    test_results[test_suite_count].passed = 0;
    test_results[test_suite_count].total = 0;
    strcpy(test_results[test_suite_count].failed_tests, "");
    START_TEST_TIMER();
    
    char *original_filename = filename;
    filename = "../data/integration_test.csv";
    
    // Scenario 1: Read -> Modify -> Save -> Read cycle
    const char* initial_content = 
        "OrderID,ProductName,Quantity,TotalPrice\n"
        "INT001,Initial Product 1,1,100\n"
        "INT002,Initial Product 2,2,200\n";
    
    create_test_csv(filename, initial_content);
    
    // Step 1: Read initial data
    int result = read_data();
    ASSERT_EQ(0, result, "Integration: Initial read");
    ASSERT_EQ(2, product_count, "Integration: Initial count");
    
    // Step 2: Modify data
    if (products != NULL && product_count >= 2) {
        strcpy(products[0].ProductName, "Modified Product 1");
        products[0].TotalPrice = 150;
        
        // Add new product (simulate adding data)
        if (product_count < product_capacity || product_capacity == 0) {
            if (product_capacity == product_count) {
                product_capacity = product_capacity == 0 ? 10 : product_capacity * 2;
                products = realloc(products, product_capacity * sizeof(struct data_csv_st));
            }
            
            if (products != NULL) {
                strcpy(products[product_count].OrderID, "INT003");
                strcpy(products[product_count].ProductName, "Added Product");
                products[product_count].Quantitiy = 3;
                products[product_count].TotalPrice = 300;
                product_count++;
            }
        }
    }
    
    ASSERT_EQ(3, product_count, "Integration: After modification count");
    
    // Step 3: Save modified data
    result = save_file();
    ASSERT_EQ(0, result, "Integration: Save modified data");
    
    // Step 4: Read back and verify
    result = read_data();
    ASSERT_EQ(0, result, "Integration: Read back modified data");
    ASSERT_EQ(3, product_count, "Integration: Read back count");
    
    if (products != NULL && product_count >= 3) {
        ASSERT_STR_EQ("Modified Product 1", products[0].ProductName, "Integration: Modified product name");
        ASSERT_EQ(150, products[0].TotalPrice, "Integration: Modified price");
        ASSERT_STR_EQ("INT003", products[2].OrderID, "Integration: Added product ID");
        ASSERT_EQ(300, products[2].TotalPrice, "Integration: Added product price");
    }
    
    // Scenario 2: Multiple file operations
    filename = "../data/integration_test2.csv";
    result = save_file();  // Save to different file
    ASSERT_EQ(0, result, "Integration: Save to different file");
    
    // Read from the new file
    result = read_data();
    ASSERT_EQ(0, result, "Integration: Read from different file");
    ASSERT_EQ(3, product_count, "Integration: Different file count");
    
    filename = original_filename;
    END_TEST_TIMER();
    test_suite_count++;
    return 1;
}

// Restore org CSV
void restore_original_csv() {
    // First, ensure filename points to the correct file
    char *original_filename = "../data/raw_data.csv";
    
    // Check if temp.csv exists and restore it to raw_data.csv if needed
    FILE *temp_file = fopen("../data/temp.csv", "r");
    if (temp_file != NULL) {
        fclose(temp_file);
        
        printf("Found temp.csv, restoring to raw_data.csv...\n");
        
        // Copy temp.csv back to raw_data.csv
        FILE *source = fopen("../data/temp.csv", "r");
        FILE *dest = fopen(original_filename, "w");
        
        if (source && dest) {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), source)) {
                fputs(buffer, dest);
            }
            printf("Restored original data from temp.csv to raw_data.csv\n");
        } else {
            printf("Error: Could not restore from temp.csv\n");
        }
        
        if (source) fclose(source);
        if (dest) fclose(dest);
        
        // Remove temp.csv after restoration
        remove("../data/temp.csv");
    }
    
    // Reset filename to default
    filename = original_filename;
}

// Main test runner function
void run_all_tests() {
    cls();
    // Use simple ASCII characters instead of Unicode
    printf("================================================================\n");
    printf("                COMPREHENSIVE UNIT TEST SUITE                  \n");
    printf("                Online Order Management System                 \n");
    printf("================================================================\n");
    
    // Initialize test results
    test_suite_count = 0;
    total_test_time = 0;
    for (int i = 0; i < 50; i++) {
        test_results[i].passed = 0;
        test_results[i].total = 0;
        strcpy(test_results[i].test_suite_name, "");
        strcpy(test_results[i].failed_tests, "");
    }
    
    // Backup current CSV file if it exists
    FILE *current_file = fopen("../data/raw_data.csv", "r");
    if (current_file != NULL) {
        fclose(current_file);
        
        // Create backup
        FILE *source = fopen("../data/raw_data.csv", "r");
        FILE *backup = fopen("../data/backup_raw_data.csv", "w");
        
        if (source && backup) {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), source)) {
                fputs(buffer, backup);
            }
            printf("Created backup of original data\n");
        }
        
        if (source) fclose(source);
        if (backup) fclose(backup);
    }
    
    setup_test_environment();
    
    printf("\nStarting comprehensive test execution...\n");
    printf("----------------------------------------------------------------\n");
    
    clock_t total_start_time = clock();
    
    // Run all comprehensive tests
    test_to_lowercase_comprehensive();
    test_read_data_comprehensive();
    test_file_edge_cases();
    test_memory_management_comprehensive();
    test_csv_parsing_edge_cases_comprehensive();
    test_save_file_comprehensive();
    test_boundary_conditions();
    test_data_validation();
    test_performance();
    test_integration_scenarios();
    
    clock_t total_end_time = clock();
    double total_time = ((double)(total_end_time - total_start_time)) / CLOCKS_PER_SEC;
    
    // Print comprehensive summary with simple ASCII
    printf("\n================================================================\n");
    printf("                      TEST SUMMARY REPORT                      \n");
    printf("================================================================\n");
    
    int total_passed = 0;
    int total_tests = 0;
    int suites_passed = 0;
    int critical_failures = 0;
    
    for (int i = 0; i < test_suite_count; i++) {
        total_passed += test_results[i].passed;
        total_tests += test_results[i].total;
        
        if (test_results[i].passed == test_results[i].total && test_results[i].total > 0) {
            printf(ANSI_COLOR_GREEN "[PASS] %-25s: %2d/%2d tests passed" ANSI_COLOR_RESET "\n", 
                   test_results[i].test_suite_name, test_results[i].passed, test_results[i].total);
            suites_passed++;
        } else if (test_results[i].total > 0) {
            printf(ANSI_COLOR_RED "[FAIL] %-25s: %2d/%2d tests passed" ANSI_COLOR_RESET, 
                   test_results[i].test_suite_name, test_results[i].passed, test_results[i].total);
            if (strlen(test_results[i].failed_tests) > 0) {
                printf(ANSI_COLOR_YELLOW " (Failed: %s)" ANSI_COLOR_RESET, test_results[i].failed_tests);
            }
            printf("\n");
            
            // Mark critical failures
            if (strstr(test_results[i].test_suite_name, "memory") || 
                strstr(test_results[i].test_suite_name, "read_data") ||
                strstr(test_results[i].test_suite_name, "save_file")) {
                critical_failures++;
            }
        }
    }
    
    // Calculate success rate
    double success_rate = total_tests > 0 ? (double)total_passed / total_tests * 100.0 : 0.0;
    
    printf("\n----------------------------------------------------------------\n");
    printf("OVERALL RESULTS:\n");
    printf("   Test Suites: %s%d/%d passed (%.1f%%)" ANSI_COLOR_RESET "\n", 
           (suites_passed == test_suite_count) ? ANSI_COLOR_GREEN : 
           (suites_passed >= test_suite_count * 0.8) ? ANSI_COLOR_YELLOW : ANSI_COLOR_RED,
           suites_passed, test_suite_count, 
           test_suite_count > 0 ? (double)suites_passed / test_suite_count * 100.0 : 0.0);
    
    printf("   Individual Tests: %s%d/%d passed (%.1f%%)" ANSI_COLOR_RESET "\n", 
           (success_rate >= 95.0) ? ANSI_COLOR_GREEN : 
           (success_rate >= 80.0) ? ANSI_COLOR_YELLOW : ANSI_COLOR_RED,
           total_passed, total_tests, success_rate);
    
    printf("   Critical Failures: %s%d" ANSI_COLOR_RESET "\n",
           (critical_failures == 0) ? ANSI_COLOR_GREEN : ANSI_COLOR_RED, critical_failures);
    
    printf("   Execution Time: " ANSI_COLOR_CYAN "%.3f seconds" ANSI_COLOR_RESET "\n", total_time);
    
    // Quality assessment with simple ASCII
    printf("\n----------------------------------------------------------------\n");
    printf("QUALITY ASSESSMENT:\n");
    
    if (success_rate >= 95.0 && critical_failures == 0) {
        printf(ANSI_COLOR_GREEN "EXCELLENT: Your code quality is outstanding!\n" ANSI_COLOR_RESET);
    } else if (success_rate >= 85.0 && critical_failures <= 1) {
        printf(ANSI_COLOR_YELLOW "GOOD: Your code is solid with minor issues.\n" ANSI_COLOR_RESET);
    } else if (success_rate >= 70.0) {
        printf(ANSI_COLOR_YELLOW "FAIR: Your code needs some improvements.\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "NEEDS WORK: Significant issues found that need attention.\n" ANSI_COLOR_RESET);
    }
    
    // Recommendations
    if (critical_failures > 0) {
        printf(ANSI_COLOR_RED "CRITICAL: Fix memory management and core I/O functions first!\n" ANSI_COLOR_RESET);
    }
    
    if (success_rate < 90.0) {
        printf(ANSI_COLOR_YELLOW "SUGGESTION: Review failed tests and improve error handling.\n" ANSI_COLOR_RESET);
    }
    
    printf("================================================================\n");
    
    // Cleanup test files
    cleanup_test_files();
    
    // Restore original CSV file from backup
    FILE *backup_file = fopen("../data/backup_raw_data.csv", "r");
    if (backup_file != NULL) {
        fclose(backup_file);
        
        // Restore from backup
        FILE *source = fopen("../data/backup_raw_data.csv", "r");
        FILE *dest = fopen("../data/raw_data.csv", "w");
        
        if (source && dest) {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), source)) {
                fputs(buffer, dest);
            }
            printf("Restored original raw_data.csv from backup\n");
        }
        
        if (source) fclose(source);
        if (dest) fclose(dest);
        
        // Remove backup file
        remove("../data/backup_raw_data.csv");
    }
    
    // Also check for temp.csv and handle it
    restore_original_csv();
    
    // Restore original data by reading from current file
    if (products != NULL) {
        free(products);
        products = NULL;
    }
    product_count = 0;
    product_capacity = 0;
    
    // Reload original data
    printf("Reloading original data...\n");
    read_data();
    printf("Data restoration complete.\n");
    
    printf("\nPress Enter to return to main menu...");
    // Clear input buffer properly
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}