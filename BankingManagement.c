#include <stdio.h>
#include <string.h>
#define MAX_ACCOUNTS 100
#define ACCOUNT_FILE "accounts.txt"
#define TRANSACTION_FILE "transactions.txt"

struct Account { int accountNumber; char name[50]; float balance; };
struct Account accounts[MAX_ACCOUNTS];
int accountCount=0;

int readInt(void) {
    char line[100], extra; int value;
    if (!fgets(line,sizeof(line),stdin)) return -1;
    if (sscanf(line," %d %c",&value,&extra)!=1) return -1;
    return value;
}
float readAmount(void) {
    char line[100], extra; float value;
    if (!fgets(line,sizeof(line),stdin)) return -1.0f;
    if (sscanf(line," %f %c",&value,&extra)!=1) return -1.0f;
    return value;
}
int findAccount(int number) {
    for(int i=0;i<accountCount;i++)
        if(accounts[i].accountNumber==number) return i;
    return -1;
}
void saveAccounts(void) {
    FILE *fp=fopen(ACCOUNT_FILE,"w");
    if(!fp){ printf("Could not save account records.\n"); return; }
    for(int i=0;i<accountCount;i++)
        fprintf(fp,"%d|%s|%.2f\n",accounts[i].accountNumber,accounts[i].name,accounts[i].balance);
    fclose(fp);
}
void recordTransaction(int number,const char type[],float amount) {
    FILE *fp=fopen(TRANSACTION_FILE,"a");
    if(!fp){ printf("Transaction history could not be saved.\n"); return; }
    fprintf(fp,"%d|%s|%.2f\n",number,type,amount); fclose(fp);
}
void loadAccounts(void) {
    FILE *fp=fopen(ACCOUNT_FILE,"r");
    if(!fp) {
        struct Account sample[]={{1001,"Rahul Sharma",25000},{1002,"Aman Verma",40000},
        {1003,"Priya Singh",55000},{1004,"Rohit Kumar",18000},{1005,"Neha Gupta",75000}};
        accountCount=5;
        for(int i=0;i<accountCount;i++) accounts[i]=sample[i];
        saveAccounts(); return;
    }
    accountCount=0;
    while(accountCount<MAX_ACCOUNTS &&
      fscanf(fp,"%d|%49[^|]|%f",&accounts[accountCount].accountNumber,
      accounts[accountCount].name,&accounts[accountCount].balance)==3) accountCount++;
    fclose(fp);
}
void addAccount(void) {
    if(accountCount>=MAX_ACCOUNTS){ puts("Account limit reached."); return; }
    struct Account a;
    printf("Enter new account number: "); a.accountNumber=readInt();
    if(a.accountNumber<=0 || findAccount(a.accountNumber)!=-1){ puts("Invalid or duplicate account number."); return; }
    printf("Enter account holder name: ");
    if(!fgets(a.name,sizeof(a.name),stdin)) return;
    a.name[strcspn(a.name,"\n")]='\0';
    if(strlen(a.name)==0 || strchr(a.name,'|')){ puts("Invalid name."); return; }
    printf("Enter opening balance (Rs.): "); a.balance=readAmount();
    if(a.balance<0){ puts("Opening balance cannot be negative."); return; }
    accounts[accountCount++]=a; saveAccounts(); recordTransaction(a.accountNumber,"ACCOUNT_OPENED",a.balance);
    puts("Account created successfully.");
}
void viewAccounts(void) {
    if(!accountCount){ puts("No account records found."); return; }
    printf("\n%-12s %-25s %-15s\n","ACCOUNT NO.","ACCOUNT HOLDER","BALANCE (Rs.)");
    puts("--------------------------------------------------------------");
    for(int i=0;i<accountCount;i++)
      printf("%-12d %-25s Rs. %-12.2f\n",accounts[i].accountNumber,accounts[i].name,accounts[i].balance);
}
void searchAccount(void) {
    printf("Enter account number: "); int n=readInt(),i=findAccount(n);
    if(i<0){ puts("Account not found."); return; }
    printf("Account Number : %d\nAccount Holder : %s\nCurrent Balance: Rs. %.2f\n",
      accounts[i].accountNumber,accounts[i].name,accounts[i].balance);
}
void depositMoney(void) {
    printf("Enter account number: "); int n=readInt(),i=findAccount(n);
    if(i<0){ puts("Account not found."); return; }
    printf("Enter amount to deposit (Rs.): "); float amount=readAmount();
    if(amount<=0){ puts("Amount must be greater than zero."); return; }
    accounts[i].balance+=amount; saveAccounts(); recordTransaction(n,"DEPOSIT",amount);
    printf("Deposit successful. Updated balance: Rs. %.2f\n",accounts[i].balance);
}
void withdrawMoney(void) {
    printf("Enter account number: "); int n=readInt(),i=findAccount(n);
    if(i<0){ puts("Account not found."); return; }
    printf("Enter amount to withdraw (Rs.): "); float amount=readAmount();
    if(amount<=0){ puts("Amount must be greater than zero."); return; }
    if(amount>accounts[i].balance){ puts("Insufficient balance. Transaction cancelled."); return; }
    accounts[i].balance-=amount; saveAccounts(); recordTransaction(n,"WITHDRAWAL",amount);
    printf("Withdrawal successful. Remaining balance: Rs. %.2f\n",accounts[i].balance);
}
void checkBalance(void) {
    printf("Enter account number: "); int n=readInt(),i=findAccount(n);
    if(i<0){ puts("Account not found."); return; }
    printf("Account holder: %s\nAvailable balance: Rs. %.2f\n",accounts[i].name,accounts[i].balance);
}
void showTransactions(void) {
    printf("Enter account number: "); int n=readInt();
    if(findAccount(n)<0){ puts("Account not found."); return; }
    FILE *fp=fopen(TRANSACTION_FILE,"r");
    if(!fp){ puts("No transaction history available."); return; }
    int saved,found=0; char type[30]; float amount;
    printf("\nTransaction history for account %d\n",n);
    while(fscanf(fp,"%d|%29[^|]|%f",&saved,type,&amount)==3)
      if(saved==n){ printf("%-20s Rs. %.2f\n",type,amount); found=1; }
    fclose(fp); if(!found) puts("No transactions found.");
}
int main(void) {
    int choice; loadAccounts();
    puts("\n===============================================");
    puts("          BANKING MANAGEMENT SYSTEM");
    puts("===============================================");
    do {
      puts("\n1. Create New Account\n2. View All Accounts\n3. Search Account");
      puts("4. Deposit Money\n5. Withdraw Money\n6. Check Account Balance");
      puts("7. View Transaction History\n8. Exit");
      printf("Enter your choice: "); choice=readInt();
      switch(choice) {
        case 1:addAccount();break; case 2:viewAccounts();break;
        case 3:searchAccount();break; case 4:depositMoney();break;
        case 5:withdrawMoney();break; case 6:checkBalance();break;
        case 7:showTransactions();break;
        case 8:saveAccounts();puts("Records saved. Thank you.");break;
        default:puts("Invalid choice. Enter 1 to 8.");
      }
    } while(choice!=8);
    return 0;
}
