/*
Dashboard for Bill payments and Ticket bookings
Project for CBSE Grade 12
Mehul Arora and Raghav Mathur
Class XII-A
*/
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <ctime>
#include <string.h>

using namespace std;

// Account Login
int valid_username(char username[17]);
int existing_username(char username[17]);
int existing_username_create(char username[17]);

// Dashboard
void check_amount();
void buy_tickets();
void pay_bills();
void deposit();
void check_balance();


// Account Management
void create_account();
void delete_account();
void change_password();

// God View
// Password: XINO
void account_list();
void sort_accounts();

// Account Class
class account
{
private:
    // Account Login
    char account_username[17];
    int account_password;
    // Owner Details
    char f_name[20];
    char l_name[20];
    // Balance
    float balance;


public:
    account();
    char* return_username();
    float return_balance();
    int check_password(int);
    void in();
    void out();
    void deposit(float amount);
    void withdraw(float amount);
    int sufficient_balance(float amount);
    void change_password(int new_password);
};

// Constructor
account::account()
{
    strcpy(account_username,"000000000000");
    account_password = 0;
    strcpy(f_name, "NULL");
    strcpy(l_name, "NULL");
    balance = 0;
}

// Returns username
char* account::return_username()
{
    return account_username;
}

// Returns Account Balance
float account::return_balance()
{
    return balance;
}

// Authorizes Account
int account::check_password(int password)
{
    if (account_password == password)
    {
        cout << "\nAuthorized\n";
        return 1;
    }
    else
    {
        cout << "\nIncorrect password\n";
        return 0;
    }
}

// Create New Account
void account::in()
{
    cout << "Enter your first name: ";
    cin >> f_name;
    cout << "Enter your last name: ";
    cin >> l_name;

    cout << "\nEnter a username (12 characters): ";
    cin >> account_username;

    // username Validity Check
    if (!valid_username(account_username))
    {
        exit(0);
    }

    if (existing_username_create(account_username))
    {
        exit(0);
    }

    cout << "\nEnter your 4 digit account password: ";
    cin >> account_password;
}

// Output Account Details
void account::out() 
{
    cout << account_username << endl;
    cout << "First name: " << f_name << endl;
    cout << "Last name: " << l_name << endl;
    cout << "password: " << account_password << endl;
    cout << "Balance: " << balance << endl << endl;
}

void account::deposit(float amount)
{
    balance += amount;
}

void account::withdraw(float amount)
{
    balance -= amount;
}

// Checks if Withdrawal is Allowed
int account::sufficient_balance(float amount)
{
    if (balance >= amount)
    {
        return 1;
    }
    cout << "\nERROR: Insufficient Funds\n";
    return 0;
}

void account::change_password(int new_password)
{
    account_password = new_password;
}

int main()
{
    //remove("accounts.dat");
    fflush(stdin);
    int choice;

    while (1)
    {
        // Menu
        cout << "\n\t1. Buy tickets"
             << "\n\t2. Deposit Money"
             << "\n\t3. Check Balance"
             << "\n\t4. Pay Bills"
             << "\n\t5. Create New Account"
             << "\n\t6. Delete Existing Account"
             << "\n\t7. Change Account Password"
             << "\n\t8. List All Accounts"
             << "\n\t9. Sort All Accounts"
             << "\n\t10. Exit";

        cout << "\n\nChoose an option: ";
        cin >> choice;

        if (choice == 1) buy_tickets();
        else if (choice == 2) deposit();
        else if (choice == 3) check_balance();
        else if (choice == 4) pay_bills();
        else if (choice == 5) create_account();
        else if (choice == 6) delete_account();
        else if (choice == 7) change_password();
        else if (choice == 8) account_list();
        else if (choice == 9) sort_accounts();
        // Exits Menu
        else if (choice == 10)
        {
            cout << "\nProgram successfully terminated\n";
            break;
        }
        else
            cout << "\nInvalid Choice\n";
    }

    return 0;
}

// Valid username Check
int valid_username(char username[17])
{
    if (strlen(username)!=12)
    {
        cout << "\nERROR: Only 12 Digits Allowed\n";
        return 0;
    }

    return 1;
}

// Existing username Check (During Account Authorization)
int existing_username(char username[17])
{
    ifstream f("accounts.dat", ios::binary);
    account a;

    while (f.read((char*)(&a),sizeof(account)))
    {
        if (!strcmp(a.return_username(),username))
        {
            f.close();
            return 1;
        }
    }

    cout << "\nERROR: Account Doesn't Exist.\n";
    f.close();
    return 0;
}

// Existing username Check (During Account Creation)
int existing_username_create(char username[17])
{
    ifstream f("accounts.dat", ios::binary);
    account a;

    while (f.read((char*)(&a),sizeof(account)))
    {
        if (strcmp(a.return_username(),username)==0)
        {
            cout << "\nERROR: username Taken.\n";
            f.close();
            return 1;
        }
    }

    f.close();
    return 0;
}

// Valid Withdrawal Amount (Not Negative, Not Zero)
int check_amount(float amount)
{
    if (amount > 0)
    {
        return 1;
    }
    if (amount == 0)
    {
        cout << "\nERROR: Amount Cannot Be Zero\n";
        return 0;
    }

    cout << "\nERROR: Amount Cannot Be Negative\n";
    return 0;
}

void deposit()
{
    fstream f("accounts.dat", ios::binary | ios::in | ios::out | ios::ate);
    f.seekg(0);

    account a;

    char account_username[17];
    int account_password;

    cout << "\nEnter your 12 digit username: ";
    cin >> account_username;

    // username Validity Check
    if (!valid_username(account_username))
    {
        f.close();
        return;
    }

    if (!existing_username(account_username))
    {
        f.close();
        return;
    }

    cout << "\nEnter your 4 digit account password: ";
    cin >> account_password;

    while (f.read((char*)(&a),sizeof(account)))
    {
        if (!strcmp(a.return_username(), account_username))
        {
            if(!a.check_password(account_password))
            {
                f.close();
                return;
            }
            break;
        }
    }

    float amount;
    int x = -1*sizeof(account);

    cout << "\nEnter amount to deposit: ";
    cin >> amount;

    if (!check_amount(amount))
    {
        f.close();
        return;
    }

    // Make Changes to Object
    a.deposit(amount);

    a.out();

    // Make Changes to File
    f.seekp(x,ios::cur);
    f.write((char*)(&a),sizeof(account));

    f.close();
}

void buy_tickets()
{
    fstream f("accounts.dat", ios::binary | ios::in | ios::out | ios::ate);
    f.seekg(0);

    account a;

    char account_username[17];
    int account_password;

    cout << "\nEnter your 12 digit username: ";
    cin >> account_username;

    // username Validity Check
    if (!valid_username(account_username))
    {
        f.close();
        return;
    }

    if (!existing_username(account_username))
    {
        f.close();
        return;
    }

    cout << "\nEnter your 4 digit account password: ";
    cin >> account_password;

    while (f.read((char*)(&a),sizeof(account)))
    {
        if (!strcmp(a.return_username(), account_username))
        {
            if(!a.check_password(account_password))
            {
                f.close();
                return;
            }
            break;
        }
    }

    float amount;
    int category,num;
    int x = -1*sizeof(account);

    cout<<"How many tickets? \n";
    cin>>num;

    cout << "\nEnter category of ticket you want to book \n 1. 250/- for front row seats \n 2. 500/- for upper row seats \n 3. 1000/- for balcony seats \n ";
    cin>>category;
    if(category==1)
        amount=250*num;
    if(category==2)
        amount=500*num;
    if(category==3)
        amount=1000*num;
    cout << endl;


    if (!a.sufficient_balance(amount))
    {
        f.close();
        return;
    }

    // Make Changes to Object
    a.withdraw(amount);

    a.out();

    // Make Changes to File
    f.seekp(x,ios::cur);
    f.write((char*)(&a),sizeof(account));

    f.close();
}

void pay_bills()
{
       fstream f("accounts.dat", ios::binary | ios::in | ios::out | ios::ate);
    f.seekg(0);

    account a;

    char account_username[17];
    int account_password;

    cout << "\nEnter your 12 digit username: ";
    cin >> account_username;

    // username Validity Check
    if (!valid_username(account_username))
    {
        f.close();
        return;
    }

    if (!existing_username(account_username))
    {
        f.close();
        return;
    }

    cout << "\nEnter your 4 digit account password: ";
    cin >> account_password;

    while (f.read((char*)(&a),sizeof(account)))
    {
        if (!strcmp(a.return_username(), account_username))
        {
            if(!a.check_password(account_password))
            {
                f.close();
                return;
            }
            break;
        }
    }

    float amount;
    int category;
    int x = -1*sizeof(account);



    cout << "\nEnter category of ticket you want to book \n 1. Credit Card Bill \n 2. Electricity Bill \n 3. Water bill \n ";
    cin>>category;

    cout<<"What is the amount of the bill?"<<endl;
    cin>>amount;
    cout << endl;


    if (!a.sufficient_balance(amount))
    {
        f.close();
        return;
    }

    // Make Changes to Object
    a.withdraw(amount);

    a.out();

    // Make Changes to File
    f.seekp(x,ios::cur);
    f.write((char*)(&a),sizeof(account));

    f.close();
}


void check_balance()
{
    fstream f("accounts.dat", ios::binary | ios::in);
    f.seekg(0);

    account a;

    char account_username[17];
    int account_password;

    cout << "\nEnter your 12 digit username: ";
    cin >> account_username;

    // username Validity Check
    if (!valid_username(account_username))
    {
        f.close();
        return;
    }

    if (!existing_username(account_username))
    {
        f.close();
        return;
    }

    cout << "\nEnter your 4 digit account password: ";
    cin >> account_password;

    while (f.read((char*)(&a),sizeof(account)))
    {
        if (!strcmp(a.return_username(), account_username))
        {
            if(!a.check_password(account_password))
            {
                f.close();
                return;
            }
            break;
        }
    }

    // Output Remaining Balance
    cout << "\nYour balance is " << a.return_balance() << " rupees.\n";

    f.close();
}

void create_account()
{
    fstream f("accounts.dat", ios::binary | ios::app | ios::out);
    account a;

    // Input New Account Details
    cout << endl;
    a.in();

    // Appends New Account Details to File
    f.write((char*)(&a),sizeof(account));
    f.close();
}

void delete_account()
{
    fstream f("accounts.dat", ios::binary | ios::in);
    f.seekg(0);

    account a;

    char account_username[17];
    int account_password;

    cout << "\nEnter your 12 digit username: ";
    cin >> account_username;

    // username Validity Check
    if (!valid_username(account_username))
    {
        f.close();
        return;
    }

    if (!existing_username(account_username))
    {
        f.close();
        return;
    }

    cout << "\nEnter your 4 digit account password: ";
    cin >> account_password;

    f.seekg(0);

    while (f.read((char*)(&a),sizeof(account)))
    {
        if (!strcmp(a.return_username(), account_username))
        {
            if(!a.check_password(account_password))
            {
                f.close();
                return;
            }
            break;
        }
    }

    char c;

    cout << "\nAre you sure you want to delete your account? All your balance will be lost.";
    cout << "\nEnter y to confirm: ";
    cin >> c;

    if (c != 'y')
    {
        f.close();
        return;
    }

    fstream g("temp.dat", ios::binary | ios::out);

    f.seekg(0);

    // Output All But Account to Delete to New File
    while (f.read((char*)(&a),sizeof(account)))
    {
        if (strcmp(a.return_username(), account_username))
        {
            g.write((char*)(&a),sizeof(account));
        }
    }

    f.close();
    g.close();

    cout << "\nAccount Deleted\n";

    remove("accounts.dat");
    rename("temp.dat","accounts.dat");
}

void change_password()
{
    fstream f("accounts.dat", ios::binary | ios::in | ios::out | ios::ate);
    f.seekg(0);

    account a;

    char account_username[17];
    int account_password;

    cout << "\nEnter your 12 digit username: ";
    cin >> account_username;

    // username Validity Check
    if (!valid_username(account_username))
    {
        f.close();
        return;
    }

    if (!existing_username(account_username))
    {
        f.close();
        return;
    }

    cout << "\nEnter your (current) 4 digit account password: ";
    cin >> account_password;

    while (f.read((char*)(&a),sizeof(account)))
    {
        if (!strcmp(a.return_username(), account_username))
        {
            if(!a.check_password(account_password))
            {
                f.close();
                return;
            }
            break;
        }
    }

    int new_password;
    int x = -1*sizeof(account);

    cout << "\nEnter your new 4 digit account password: ";
    cin >> new_password;

    // Modify Object
    a.change_password(new_password);

    // Write Modified Object to File
    f.seekp(x,ios::cur);
    f.write((char*)(&a),sizeof(account));

    cout << "\npassword Successfully Changed\n";

    f.close();
}

void account_list()
{
    char password[20];

    // Password: XINO
    cout << "\nEnter Administrator Password: ";
    cin >> password;

    if (strcmp(password, "XINO"))
    {
        cout << "\nERROR: Incorrect Administrator Password\n";
        return;
    }

    ifstream f("accounts.dat", ios::binary);
    account a;

    cout << endl;

    // Output
    while (f.read((char*)(&a),sizeof(account)))
    {
        a.out();
    }

    f.close();
}

void sort_accounts()
{
    char password[20];

    // Password: XINO
    cout << "\nEnter Administrator Password: ";
    cin >> password;

    if (strcmp(password, "XINO"))
    {
        cout << "\nERROR: Incorrect Administrator Password\n";
        return;
    }

    ifstream f("accounts.dat", ios::binary);
    int num = 0;
    account a[100];
    account temp;

    // Take File Data in Array of Objects
    while (f.read((char*)(&a[num++]),sizeof(account)))

    // Bubble Sort Array
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num - i - 1; j++)
        {
            if (a[j].return_balance() < a[j+1].return_balance())
            {
                temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
            }
        }
    }

    fstream g("temp.dat", ios::binary | ios::out);

    // Output Sorted Array to New File
    for (int i = 0; i < num - 1; i++)
    {
        g.write((char*)(&a[i]),sizeof(account));
    }

    f.close();
    g.close();

    remove("accounts.dat");
    rename("temp.dat","accounts.dat");
}
