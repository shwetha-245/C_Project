#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// Structure for items in the order
struct items {
    char item[20];
    float price;
    int qty;
};

// Structure for orders
struct orders {
    char customer[50];
    char date[50];
    int numOfItems;
    struct items itm[50];
};

// Function to generate bill header
void generateBillHeader(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t    ADV. Restaurant");
    printf("\n\t   -----------------");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n---------------------------------------\n");
}

// Function to generate bill body
void generateBillBody(char item[30], int qty, float price) {
    printf("%s\t\t", item); 
    printf("%d\t\t", qty); 
    printf("%.2f\t\t", qty * price); 
    printf("\n");
}

// Function to generate bill footer with tax and discount
void generateBillFooter(float total) {
    printf("\n");
    float discount = 0.1 * total;
    float netTotal = total - discount;
    float cgst = 0.09 * netTotal;
    float sgst = cgst;
    float grandTotal = netTotal + cgst + sgst; 

    printf("---------------------------------------\n");
    printf("Sub Total\t\t\t%.2f", total);
    printf("\nDiscount @10%%\t\t\t%.2f", discount);
    printf("\n\t\t\t\t-------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nCGST @9%%\t\t\t%.2f", cgst);
    printf("\nSGST @9%%\t\t\t%.2f", sgst);
    printf("\n---------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n---------------------------------------\n");
}

// Main function
int main() {
    int opt, n;
    struct orders ord;
    struct orders order;
    char saveBill = 'y', contFlag = 'y';
    char name[50];
    FILE *fp;

    while (contFlag == 'y') {
        system("clear");
        float total = 0;
        int invoiceFound = 0;

        printf("\t============ADV. RESTAURANT============");
        printf("\n\nPlease select your preferred operation");
        printf("\n\n1.Generate Invoice");
        printf("\n2.Show all Invoices");
        printf("\n3.Search Invoice");
        printf("\n4.Export Invoice to File");
        printf("\n5.Exit");

        printf("\n\nYour choice:\t");
        scanf("%d", &opt);
        fgetc(stdin); // clear input buffer

        switch (opt) {
            case 1:
                system("clear");
                printf("\nPlease enter the name of the customer:\t");
                fgets(ord.customer, 50, stdin);
                ord.customer[strlen(ord.customer) - 1] = 0; // remove trailing newline
                strcpy(ord.date, __DATE__);
                printf("\nPlease enter the number of items:\t");
                scanf("%d", &n);
                ord.numOfItems = n;

                // Input items
                for (int i = 0; i < n; i++) {
                    fgetc(stdin); // clear input buffer
                    printf("\n\n");
                    printf("Please enter the item %d:\t", i + 1);
                    fgets(ord.itm[i].item, 20, stdin);
                    ord.itm[i].item[strlen(ord.itm[i].item) - 1] = 0; // remove trailing newline
                    printf("Please enter the quantity:\t");
                    scanf("%d", &ord.itm[i].qty);
                    printf("Please enter the unit price:\t");
                    scanf("%f", &ord.itm[i].price);
                    total += ord.itm[i].qty * ord.itm[i].price;
                }

                // Generate bill
                generateBillHeader(ord.customer, ord.date);
                for (int i = 0; i < ord.numOfItems; i++) {
                    generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                }
                generateBillFooter(total);

                // Save the bill
                printf("\nDo you want to save the invoice [y/n]:\t");
                scanf("%s", &saveBill);
                if (saveBill == 'y') {
                    fp = fopen("RestaurantBill.dat", "a+");
                    fwrite(&ord, sizeof(struct orders), 1, fp);
                    if (fwrite != 0) {
                        printf("\nSuccessfully saved");
                    } else {
                        printf("\nError saving");
                    }
                    fclose(fp);
                }
                break;

            case 2:
                system("clear");
                fp = fopen("RestaurantBill.dat", "r");
                printf("\n  *****Your Previous Invoices*****\n");
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    generateBillHeader(order.customer, order.date);
                    for (int i = 0; i < order.numOfItems; i++) {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        tot += order.itm[i].qty * order.itm[i].price;
                    }
                    generateBillFooter(tot);
                }
                fclose(fp);
                break;

            case 3:
                printf("Enter the name of the customer:\t");
                fgets(name, 50, stdin);
                name[strlen(name) - 1] = 0; // remove trailing newline
                system("clear");
                fp = fopen("RestaurantBill.dat", "r");
                printf("\t*****Invoice of %s*****", name);
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    if (!strcmp(order.customer, name)) {
                        generateBillHeader(order.customer, order.date);
                        for (int i = 0; i < order.numOfItems; i++) {
                            generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                            tot += order.itm[i].qty * order.itm[i].price;
                        }
                        generateBillFooter(tot);
                        invoiceFound = 1;
                    }
                }
                if (!invoiceFound) {
                    printf("Sorry, the invoice for %s does not exist", name);
                }
                fclose(fp);
                break;

            case 4:
                printf("Enter the name of the customer to export:\t");
                fgets(name, 50, stdin);
                name[strlen(name) - 1] = 0; // remove trailing newline
                fp = fopen("RestaurantBill.dat", "r");
                FILE *fp_txt;
                fp_txt = fopen("ExportedInvoice.txt", "w");
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    if (!strcmp(order.customer, name)) {
                        fprintf(fp_txt, "Invoice of %s\n", order.customer);
                        fprintf(fp_txt, "Date: %s\n", order.date);
                        for (int i = 0; i < order.numOfItems; i++) {
                            fprintf(fp_txt, "Item: %s\tQty: %d\tTotal: %.2f\n", order.itm[i].item, order.itm[i].qty, order.itm[i].qty * order.itm[i].price);
                            tot += order.itm[i].qty * order.itm[i].price;
                        }
                        fprintf(fp_txt, "Sub Total: %.2f\n", tot);
                    }
                }
                fclose(fp_txt);
                printf("\nInvoice exported to ExportedInvoice.txt\n");
                fclose(fp);
                break;

            case 5:
                printf("\n\t\tBye Bye :)\n\n");
                exit(0);
                break;

            default:
                printf("Sorry, invalid option.");
                break;
        }
        printf("\nDo you want to perform another operation? [y/n]:\t");
        scanf("%s", &contFlag);
    }

    printf("\n\t\tBye Bye :)\n\n");
    return 0;
}
