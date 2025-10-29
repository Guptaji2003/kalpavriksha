#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Product
{
    int product_id;
    char product_name[50];
    float product_price;
    int product_quantity;
};

void productNameValidation(char *productName)
{
    while (1)
    {
        printf("Enter product name (max 50 chars): ");

        if (fgets(productName, 51, stdin) == NULL)
        {
            printf("Input error! Try again.\n");
            continue;
        }

        size_t namelength = strlen(productName);

        if (namelength > 0 && productName[namelength - 1] != '\n')
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            printf("Product name too long! Max 50 characters allowed.\n");
            continue;
        }

        if (namelength > 0 && productName[namelength - 1] == '\n')
            productName[namelength - 1] = '\0';

        int onlySpaces = 1;
        for (size_t nameIndex = 0; nameIndex < strlen(productName); nameIndex++)
        {
            if (!isspace((unsigned char)productName[nameIndex]))
            {
                onlySpaces = 0;
                break;
            }
        }

        if (strlen(productName) == 0 || onlySpaces)
        {
            printf("Product name cannot be empty or only spaces.\n");
            continue;
        }

        break;
    }
}

void inventoryLengthValidation(int *inventoryLength)
{
    char extrachar;
    while (1)
    {
        printf("\nEnter number of products (1-100): ");
        if (scanf("%d%c", inventoryLength, &extrachar) != 2 || extrachar != '\n')
        {
            printf("\nInvalid input, Please try again.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else if (*(inventoryLength) < 1 || *(inventoryLength) > 100)
        {
            printf("InventoryLength out of range, Please enter between 1 and 100.\n");
            continue;
        }
        else
        {
            break;
        }
    }
}
void productPriceValidation(float *productPrice)
{
    char extrachar;
    while (1)
    {
        printf("Enter product price (0-100000): ");
        if (scanf("%f%c", productPrice, &extrachar) != 2 || extrachar != '\n')
        {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else if (*(productPrice) < 0 || *(productPrice) > 100000)
        {
            printf("Price out of range. Please enter between 0 and 100000.\n");
            continue;
        }
        else
        {
            break;
        }
    }
}
void minPriceValidation(float *minPrice)
{
    char extrachar;

    while (1)
    {

        printf("Enter minimum price: ");
        if (scanf("%f%c", minPrice, &extrachar) != 2 || extrachar != '\n')
        {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else if (*(minPrice) < 0 || *(minPrice) > 100000)
        {
            printf("Price out of range. Please enter between 0 and 100000.\n");
            continue;
        }
        else
        {
            break;
        }
    }
}
void maxPriceValidation(float *maxPrice, float minPrice)
{
    char extrachar;
    while (1)
    {
        printf("Enter maximum price: ");
        if (scanf("%f%c", maxPrice, &extrachar) != 2 || extrachar != '\n')
        {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else if (*maxPrice < 0 || *maxPrice > 100000)
        {
            printf(" Price out of range. Please enter between 0 and 100000.\n");
            continue;
        }
        else if (*maxPrice < minPrice)
        {
            printf("Maximum price cannot be less than minimum price.\n");
            continue;
        }
        else
        {
            break;
        }
    }
}
void productIdValidation(int *productId)
{
    char extrachar;
    while (1)
    {
        printf("Enter productId (1-10000): ");
        if (scanf("%d%c", productId, &extrachar) != 2 || extrachar != '\n')
        {
            printf("\nInvalid productId,Please try again.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else if (*(productId) < 1 || *(productId) > 10000)
        {
            printf("The input went out of range.\n");
            continue;
        }
        else
        {
            break;
        }
    }
}
void productQuantityValidation(int *productQuantity)
{
    char extrachar;
    while (1)
    {
        printf("Enter productQuantity (0-1000000): ");
        if (scanf("%d%c", productQuantity, &extrachar) != 2 || extrachar != '\n')
        {
            printf("\nInvalid productQuantity, Please try again.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else if (*(productQuantity) < 0 || *(productQuantity) > 1000000)
        {
            printf("The input went out of range.\n");
            continue;
        }
        else
        {
            break;
        }
    }
}
void addNewProduct(int *productCount, int *inventoryLength, struct Product **products)
{
    if (*productCount == *inventoryLength)
    {
        printf("\nInventory full. Increasing size dynamically using realloc...\n");
        *inventoryLength += 1;
        *products = realloc(*products, (*inventoryLength) * sizeof(struct Product));
        if (*products == NULL)
        {
            printf("Memory reallocation failed!\n");
            return;
        }
    }

    printf("\nPlease enter the %dth product details-----\n", *productCount + 1);

retry_id:
    productIdValidation(&((*products + *productCount)->product_id));

    int duplicate = 0;
    for (int productIndex = 0; productIndex < *productCount; productIndex++)
    {
        if (((*products + *productCount)->product_id) == (*products + productIndex)->product_id)
        {
            printf("Product ID already exists. Please enter a unique ID.\n");
            duplicate = 1;
            break;
        }
    }

    if (duplicate)
        goto retry_id;

    productNameValidation((*products + *productCount)->product_name);
    productPriceValidation(&((*products + *productCount)->product_price));
    productQuantityValidation(&((*products + *productCount)->product_quantity));

    (*productCount)++;
    printf("\nProduct added successfully!\n");
}
void showAllProducts(int productCount, struct Product *products)
{
    if (productCount == 0)
    {
        printf("\nNo products to display.\n");
        return;
    }
    printf("\nDisplay all products-----------\n");
    for (int productIndex = 0; productIndex < productCount; productIndex++)
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", (products + productIndex)->product_id, (products + productIndex)->product_name, (products + productIndex)->product_price, (products + productIndex)->product_quantity);
    }
}
void updateProductQuantity(struct Product *products, int productCount)
{
    int productId;
    int found = 0;
    productIdValidation(&productId);

    for (int productIndex = 0; productIndex < productCount; productIndex++)
    {
        if (productId == (products + productIndex)->product_id)
        {
            found = 1;
            productQuantityValidation(&(products + productIndex)->product_quantity);
            break;
        }
    }
    if (found)
    {
        printf("\nProduct updated successfully.\n");
    }
    else
    {
        printf("\nProduct not found.\n");
    }
}
void searchProductById(struct Product *products, int productCount)
{
    int productId, found = 0;

    productIdValidation(&productId);

    for (int productIndex = 0; productIndex < productCount; productIndex++)
    {
        if (productId == (products + productIndex)->product_id)
        {
            found = 1;
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d", (products + productIndex)->product_id, (products + productIndex)->product_name, (products + productIndex)->product_price, (products + productIndex)->product_quantity);
            break;
        }
    }
    if (found)
    {
        printf("\nProduct found successfully.\n");
    }
    else
    {
        printf("\nProduct not found.\n");
    }
}

static int containsIgnoreCase(const char *mainString, const char *searchSubstring)
{
    size_t mainLength = strlen(mainString);
    size_t searchLength = strlen(searchSubstring);

    if (searchLength == 0 || mainLength < searchLength)
        return 0;

    for (size_t mainIndex = 0; mainIndex <= mainLength - searchLength; ++mainIndex)
    {
        size_t matchIndex;
        for (matchIndex = 0; matchIndex < searchLength; ++matchIndex)
        {
            if (tolower((unsigned char)mainString[mainIndex + matchIndex]) !=
                tolower((unsigned char)searchSubstring[matchIndex]))
                break;
        }
        if (matchIndex == searchLength)
            return 1;
    }
    return 0;
}

static int isEmptyOrSpaces(const char *inputString)
{
    for (size_t charIndex = 0; inputString[charIndex] != '\0'; ++charIndex)
        if (!isspace((unsigned char)inputString[charIndex]))
            return 0;
    return 1;
}

void searchProductByName(struct Product *products, int totalProducts)
{
    char searchName[51];
    int isProductFound = 0;

    while (1)
    {
        printf("Enter product name to search: ");

        if (!fgets(searchName, sizeof(searchName), stdin))
            return;

        if (searchName[0] == '\n')
        {
            printf("Product name cannot be empty!\n");
            continue;
        }

        size_t inputLength = strlen(searchName);

        if (inputLength > 0 && searchName[inputLength - 1] != '\n')
        {
            int extraChar, inputTooLong = 0;
            while ((extraChar = getchar()) != '\n' && extraChar != EOF)
                inputTooLong = 1;
            if (inputTooLong)
            {
                printf("Input too long! Maximum 50 characters allowed.\n");
                continue;
            }
        }
        else if (inputLength > 0 && searchName[inputLength - 1] == '\n')
        {
            searchName[inputLength - 1] = '\0';
            inputLength--;
        }

        while (inputLength > 0 && isspace((unsigned char)searchName[inputLength - 1]))
        {
            searchName[inputLength - 1] = '\0';
            inputLength--;
        }

        if (inputLength == 0 || isEmptyOrSpaces(searchName))
        {
            printf("Product name cannot be empty or spaces only!\n");
            continue;
        }

        break;
    }

    for (int productIndex = 0; productIndex < totalProducts; ++productIndex)
    {
        if (containsIgnoreCase((products + productIndex)->product_name, searchName))
        {
            isProductFound = 1;
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   (products + productIndex)->product_id,
                   (products + productIndex)->product_name,
                   (products + productIndex)->product_price,
                   (products + productIndex)->product_quantity);
        }
    }

    if (!isProductFound)
        printf("No product found containing \"%s\".\n", searchName);
    else
        printf("Matching products displayed.\n");
}




void searchProductByPrice(struct Product *products, int productCount)
{
    float minPrice, maxPrice;
    int found = 0;

    minPriceValidation(&minPrice);
    maxPriceValidation(&maxPrice, minPrice);

    for (int productIndex = 0; productIndex < productCount; productIndex++)
    {
        if (minPrice <= (products + productIndex)->product_price && maxPrice >= (products + productIndex)->product_price)
        {
            found = 1;
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", (products + productIndex)->product_id, (products + productIndex)->product_name, (products + productIndex)->product_price, (products + productIndex)->product_quantity);
        }
    }
    if (found)
    {
        printf("\nProduct found successfully.\n");
    }
    else
    {
        printf("\nProduct not found.\n");
    }
}
void deleteProductById(struct Product *products, int *productCount)
{
    int productId, found = 0;
    productIdValidation(&productId);

    for (int productIndex = 0; productIndex < *productCount; productIndex++)
    {
        if ((products + productIndex)->product_id == productId)
        {
            found = 1;
            for (int shiftingIndex = productIndex; shiftingIndex < *productCount - 1; shiftingIndex++)
            {
                *(products + shiftingIndex) = *(products + shiftingIndex + 1);
            }
            (*productCount)--;

            struct Product *temp = realloc(products, (*productCount) * sizeof(struct Product));

            if (temp != NULL)
            {
                products = temp;
            }
            break;
        }
    }
    if (found)
    {
        printf("\nProduct deleted successfully.\n");
    }
    else
    {
        printf("\nProduct not found.\n");
    }
}

void addInitialProducts(struct Product *products, int inventoryLength)
{
    printf("\nEnter Initial %d Products ----\n", inventoryLength);

    for (int productIndex = 0; productIndex < inventoryLength; productIndex++)
    {
        printf("\nPlease enter the %dth product details-----\n", productIndex + 1);

    retry_id:
        productIdValidation(&(products + productIndex)->product_id);

        int duplicate = 0;
        for (int initialIndex = 0; initialIndex < productIndex; initialIndex++)
        {
            if ((products + initialIndex)->product_id == (products + productIndex)->product_id)
            {
                printf("Product ID already exists. Please enter a unique ID.\n");
                duplicate = 1;
                break;
            }
        }

        if (duplicate)
            goto retry_id;

        productNameValidation((products + productIndex)->product_name);
        productPriceValidation(&(products + productIndex)->product_price);
        productQuantityValidation(&(products + productIndex)->product_quantity);
    }

    printf("\nAll initial products added successfully!\n");
}

void printMenu()
{
    printf("\n========= INVENTORY MENU =========\n");
    printf("1. Add New Product \n");
    printf("2. Display All Products \n");
    printf("3. Update Product Quantity \n");
    printf("4. Search Product by ID \n");
    printf("5. Search Product by Name \n");
    printf("6. Search Product by Price Range \n");
    printf("7. Delete Product \n");
    printf("8. Exit \n");
}

int main()
{
    int choice;
    int inventoryLength;
    inventoryLengthValidation(&inventoryLength);

    struct Product *products = (struct Product *)calloc(inventoryLength, sizeof(struct Product));
    if (products == NULL)
    {
        printf("memory allocation failed\n");
        return 1;
    }

    int productIndex = 0;
    addInitialProducts(products, inventoryLength);
    productIndex = inventoryLength;

    while (1)
    {
        printMenu();
        char extra;
        printf("Enter your choice: ");
        if (scanf("%d%c", &choice, &extra) != 2 || extra != '\n')
        {
            printf("Invalid input, Please enter a number between 1-8.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else
        {

            switch (choice)
            {
            case 1:
                addNewProduct(&productIndex, &inventoryLength, &products);
                break;
            case 2:
                showAllProducts(productIndex, products);
                break;
            case 3:
                updateProductQuantity(products, productIndex);
                break;
            case 4:
                searchProductById(products, productIndex);
                break;
            case 5:
                searchProductByName(products, productIndex);
                break;
            case 6:
                searchProductByPrice(products, productIndex);
                break;
            case 7:
                deleteProductById(products, &productIndex);
                break;
            case 8:
                free(products);
                products = NULL;
                printf("\nMemory released successfully. Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                continue;
            }
        }
    }
    return 0;
}