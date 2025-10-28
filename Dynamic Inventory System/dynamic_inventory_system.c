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
        printf("Enter product name: ");
        fgets(productName, 50, stdin);

        size_t length = strlen(productName);
        if (length > 0 && productName[length - 1] == '\n')
            productName[length - 1] = '\0';

        int onlySpaces = 1;
        for (size_t index = 0; index < strlen(productName); index++)
        {
            if (!isspace(productName[index]))
            {
                onlySpaces = 0;
                break;
            }
        }

        if (strlen(productName) == 0 || onlySpaces)
        {
            printf("Product name cannot be empty or contain only spaces.\n");
            continue;
        }

        if (strlen(productName) > 50)
        {
            printf("Product name too long (max 50 characters).\n");
            continue;
        }

        break;
    }
}
void inventoryLengthValidation(int *inventoryLength)
{
    char extra;
    while (1)
    {
        printf("\nEnter number of products (1-100): ");
        if (scanf("%d%c", inventoryLength, &extra) != 2 || extra != '\n')
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
    while (1)
    {
        printf("Enter product price (0-100000): ");
        if (scanf("%f", productPrice) != 1)
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
    while (1)
    {
        printf("Enter minimum price: ");
        if (scanf("%f", minPrice) != 1)
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
    while (1)
    {
        printf("Enter maximum price: ");
        if (scanf("%f", maxPrice) != 1)
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
    char extra;
    while (1)
    {
        printf("Enter productId (1-10000): ");
        if (scanf("%d%c", productId, &extra) != 2 || extra != '\n')
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
    char extra;
    while (1)
    {
        printf("Enter productQuantity (0-1000000): ");
        if (scanf("%d%c", productQuantity, &extra) != 2 || extra != '\n')
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

    productIdValidation(&((*products + *productCount)->product_id));
    for (int i = 0; i < *productCount; i++)
    {
        if (((*products + *productCount)->product_id) == (*products + i)->product_id)
        {
            printf("Product Id should be uniqe, This is already present.\n");
            return;
        }
    }

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
    for (int i = 0; i < productCount; i++)
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", (products + i)->product_id, (products + i)->product_name, (products + i)->product_price, (products + i)->product_quantity);
    }
}
void updateProductQuantity(struct Product *products, int productCount)
{
    int productId;
    int found = 0;
    productIdValidation(&productId);

    for (int i = 0; i < productCount; i++)
    {
        if (productId == (products + i)->product_id)
        {
            found = 1;
            productQuantityValidation(&(products + i)->product_quantity);
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

    for (int i = 0; i < productCount; i++)
    {
        if (productId == (products + i)->product_id)
        {
            found = 1;
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d", (products + i)->product_id, (products + i)->product_name, (products + i)->product_price, (products + i)->product_quantity);
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
void searchProductByName(struct Product *products, int productCount)
{
    char name[30], found = 0;
    printf("Enter product name: ");
    scanf("%s", name);

    for (int i = 0; i < productCount; i++)
    {
        if (strstr((products + i)->product_name, name))
        {
            found = 1;
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d", (products + i)->product_id, (products + i)->product_name, (products + i)->product_price, (products + i)->product_quantity);
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
void searchProductByPrice(struct Product *products, int productCount)
{
    float minPrice, maxPrice;
    int found = 0;

    minPriceValidation(&minPrice);
    maxPriceValidation(&maxPrice, minPrice);

    for (int i = 0; i < productCount; i++)
    {
        if (minPrice <= (products + i)->product_price && maxPrice >= (products + i)->product_price)
        {
            found = 1;
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d", (products + i)->product_id, (products + i)->product_name, (products + i)->product_price, (products + i)->product_quantity);
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
void deleteProductById(struct Product *products, int *productCount)
{
    int productId, found = 0;
    productIdValidation(&productId);

    for (int i = 0; i < *productCount; i++)
    {
        if ((products + i)->product_id == productId)
        {
            found = 1;
            for (int j = i; j < *productCount - 1; j++)
            {
                *(products + j) = *(products + j + 1);
            }
            (*productCount)--;
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

    for (int i = 0; i < inventoryLength; i++)
    {
        printf("\nPlease enter the %dth product details-----\n", i + 1);

    retry_id:
        productIdValidation(&(products + i)->product_id);

        int duplicate = 0;
        for (int j = 0; j < i; j++)
        {
            if ((products + j)->product_id == (products + i)->product_id)
            {
                printf("Product ID already exists. Please enter a unique ID.\n");
                duplicate = 1;
                break;
            }
        }

        if (duplicate)
            goto retry_id;

        productNameValidation((products + i)->product_name);
        productPriceValidation(&(products + i)->product_price);
        productQuantityValidation(&(products + i)->product_quantity);
    }

    printf("\nAll initial products added successfully!\n");
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

    printf("========= INVENTORY MENU =========\n");
    printf("1. Add New Product \n");
    printf("2. Display All Products \n");
    printf("3. Update Product Quantity \n");
    printf("4. Search Product by ID \n");
    printf("5. Search Product by Name \n");
    printf("6. Search Product by Price Range \n");
    printf("7. Delete Product \n");
    printf("8. Exit \n");
    while (1)
    {
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input, Please enter a number between 1-8.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

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
            printf("\nMemory released successfully. Exiting program...\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
            continue;
        }
    }
    return 0;
}