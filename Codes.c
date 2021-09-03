
/*[1] ALGORITHM FOR CHANGING INFORMATION STORED IN FILE */
/* Suppose we have to change information about file1. Now we will store  all information of file1 in some temporary file except the information which we have to change. Then we will delete all information  of file1 and copy all information from temporary file into it along with the changed information*/
/* For deleting all existing information of file we will open file in  writing mode in order to truncate it */
/* Maximum two books can be borrowed */

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

struct Admin
{
    char firstname[200]; //Admin
    char lastname[200];  //Admin
    char password[200];  //password
    char username[200];  //admin
    char phoneno[30];    //1234567890
    char email[50];      //abc
};

struct Book_Borrowed_by_user
{
    char date[100];
    char NameOfBook[200];
    char BookStatus[100];
    char Author[100];
};
struct Books_InLib
{
    char NameOfBook[1000];
    char BookStatus[20];
    //char availability[30];
    char Author[100];
};

struct User
{
    char firstname[200];
    char lastname[200];
    char password[20];
    char username[200];
    char userid[20];
    int check; /* will be used to change the password created by the admin when user logins for the first time */
    int NoOfBookBorrowed;
    char phoneno[30];
    char email[50];
    struct Book_Borrowed_by_user booksBorrowed[2];
    int securityQuestionIndex;
    char securityQusetionAns[100];
};

struct borrow
{
    char Nameofbook[1000];
    char Author[100];
    int index;
};

struct PrescribedBooks
{
    char Name[1000];
    char type[100];
};

struct Subscription
{
    char Name[1000];
    char lang[100];
    char type[100];
};

struct Security
{
    char Question[500];
    int Index;
};

struct availablebook
{
    char Nameofbook[1000];
    char Author[100];
    int index;
};
struct Feedback
{

    char feed[10000];
    char date[50];
};
/* Global variables.\n Name of files */
char userInformationFile[500] = "UserInformation.txt\0";
char booksInLibraryfile[500] = "BooksInLibrary.txt\0";
char DetailsofAdminfile[500] = "DetailsOfAdmin.txt\0";
char tempFile[500] = "temporaryfile.txt\0";
char feedbackfile[100] = "feedback.txt\0";
char borrowbookfile[100] = "borrowbook.txt\0";
char Newsletter[100] = "subscription.txt\0";
char BookPrescription[100] = "BookPrescription.txt\0";
char SecurityQuestions[100] = "SecurityQuestion.txt\0";

/* This function is used to print statement at any coordinate on computer screen */
void gotoxy(int x, int y)
{
    COORD A;
    A.X = x;
    A.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), A);
}

void box(int n)
{
    /* x= 49,y=7,65,4,(60,4) (70,16 (80,23),(85,23) (100,27) */
    system("cls");

    int i = 45, j = 5;
    int k = i - 1;

    for (i; i < 120; i++)
    {
        gotoxy(i, j);
        printf("_");
    }
    int l = i;
    for (j = j + 1; j < n; j++)
    {
        gotoxy(i, j);
        printf("|");
        gotoxy(k, j);
        printf("|");
    }

    for (k = k + 1; k < 120; k++)
    {
        gotoxy(k, j - 1);
        printf("_");
    }
}

int captcha(int x, int y)
{

    srand(time(0));
    char a[6], c[6];

    for (int i = 0; i < 4; i++)
    {
        int t;
        while (1)
        {
            t = rand() % 91;
            if (t > 64)
            {
                a[i] = (char)t;
                gotoxy(x + 27 + i, y);
                printf("%c", a[i]);
                break;
            }
        }
    }
    gotoxy(x, y + 2);
    printf("Please Enter The Text Shown  ");
    gotoxy(x + 29, y + 2);
    printf("                      ");
    gotoxy(x + 28, y + 2);
    printf(": ");
    /*    char *b = malloc(1);
    char ch;
    int i = 0;
    while ((ch = getchar()) != '\n')
    {
        b[i++] = ch;

        b = realloc(b, i + 1);
    }

    b[i] = '\0'; */
    char b[60];
    scanf("%s", b);
    int i = strlen(b) - 1;
    int check = 0;

    for (int j = 0; j < 4; j++)
    {
        if (a[j] != b[j])
        {
            check = 1;
            break;
        }
    }

    return check;
}

void box1(int n)
{
    system("cls");
    //Box

    // int x = 43, y = 7;
    int i = 40, j = 5;
    int k = i - 1;

    for (i; i < 140; i++)
    {
        gotoxy(i, j);
        printf("_");
    }
    int l = i;
    for (j = j + 1; j < n; j++)
    {
        gotoxy(i, j);
        printf("|");
        gotoxy(k, j);
        printf("|");
    }

    for (k = k + 1; k < 140; k++)
    {
        gotoxy(k, j - 1);
        printf("_");
    }
}

/* Only used once to add security question. Never called from anywhere  */
void addque()
{

    system("cls");
    FILE *fp;
    struct Security s;
    fp = fopen(SecurityQuestions, "ab+");
    if (fp == NULL)
    {
        printf("Fl");
        exit(1);
    }
    printf("\nENter name ::");
    char q[2];
    fgets(q, 2, stdin);
    gets(s.Question);
    printf("Entr Ind ");
    scanf("%d", &s.Index);
    fwrite(&s, sizeof(struct Security), 1, fp);
    fclose(fp);
    printf("\n\nwant to add mor");
    int key;
    scanf("%d", &key);
    if (key == 1)
    {
        addque();
    }
    else if (key == 2)
    {
        return;
    }
}

/* This function is used for the user in order to change the password at the time of first login because user account and password is created
by Admin. So changing password is good for security reason */
void FirstTimeUserLogin(struct User user)
{

    FILE *fp1;
    struct User user1;
    system("cls"); //Clear screan
    box(22);
    gotoxy(60, 4);
    printf("Please Create New Password on your first login.");

    fp1 = fopen(userInformationFile, "rb+");
    /*This type of if statement is used to exit from program if file does not exist.
    This type of statement will be used multiple time throught the program.
    */
    if (fp1 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    FILE *fp2;
    fp2 = fopen(tempFile, "wb+");
    if (fp2 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    /* All these steps of changing information is mentioned on starting of code */

    /* fread function is used to read struct information from file and copy it to mentioned struct */
    /* This while loop will read every struct from file one by one  */
    while (fread(&user1, sizeof(struct User), 1, fp1))
    {
        /* This if statement will match username and ID if it matches it will perform statements accordingly */
        if (strcmp(user.username, user1.username) == 0 && strcmp(user.userid, user1.userid) == 0)
        {
            gotoxy(49, 7);
            printf("Enter new password : ");
            char p[20];
            scanf("%s", p);
            strcpy(user1.password, p);
            user1.check = 1; //it will ensure that user has changed his password for the first time
        }
        /* fwrite function is used to write struct in file */
        fwrite(&user1, sizeof(struct User), 1, fp2);
    }

    fclose(fp1);
    fclose(fp2);

    /* Now we will copy information from temporary file to original one */
    FILE *fp3, *fp4;
    struct User U;

    fp3 = fopen(userInformationFile, "wb+");
    if (fp3 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    fp4 = fopen(tempFile, "rb+");
    if (fp4 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    while (fread(&U, sizeof(struct User), 1, fp4))
    {
        fwrite(&U, sizeof(struct User), 1, fp3);
    }

    fclose(fp3);
    fclose(fp4);

    gotoxy(70, 16);
    printf("Password Changed Successfully...");
    gotoxy(85, 23); //Fixing coordinates of print statement of next line
    printf("Press any key to continue...");
    _getch();
    system("cls");
}

void DisplayNewsletter()
{
    struct Subscription S;
    system("cls");
    FILE *fp;
    gotoxy(55, 2);
    printf("SUBSCRIPTIONS");
    fp = fopen(Newsletter, "rb+");
    if (fp == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }

    int i = 1, j = 1;    /* For Indexing Newspaper and Magazine respectively*/
    int x1 = 20, y1 = 6; /* Coordintes For displaying Newspaper */
    int x2 = 70, y2 = 6; /* Coordintes For displaying Magazine */
    int x3 = 60, y3 = 6; /* Coordinate for Column divider */
    gotoxy(x1 + 4, y1 - 2);
    printf("Newspaper");
    gotoxy(x2 + 4, y2 - 2);
    printf("Magazine");

    while (fread(&S, sizeof(struct Subscription), 1, fp))
    {
        gotoxy(x3, y1);
        printf("|");
        gotoxy(x3, y2);
        printf("|");
        if (strcmp(S.type, "Newspaper\0") == 0)
        {
            gotoxy(x1, y1++);
            printf("[%d]  %s(%s)", i++, S.Name, S.lang);
        }
        else if (strcmp(S.type, "Magazine\0") == 0)
        {
            gotoxy(x2, y2++);
            printf("[%d]  %s(%s)", j++, S.Name, S.lang);
        }
        /*  gotoxy(x3, y3++);
        printf("|"); */
    }
    fclose(fp);
    y3 = (y1 > y2) ? y1 : y2;
    gotoxy(x2 + 5, y3 + 3);
    printf("Press any key to continue...");
    _getch();
}

/* Reference function for AddNonBorrowableItems() function */
void AddNewspaperMagzine(char *key, int x, int y)
{
    int temp = 0;
    struct Books_InLib Addbook;
    struct Subscription S;
    char t[2];
    while (temp == 0)
    {                                /* key =1 for Newspaper */
        if (strcmp(key, "1\0") == 0) ////
        {
            gotoxy(x, y + 10);
            printf("Enter Name Of Newspaper: ");
            strcpy(S.type, "Newspaper\0");
        }                                 /* Key = 2 Magazine */
        else if (strcmp(key, "2\0") == 0) ////
        {
            gotoxy(x, y + 10);
            printf("Enter Name Of Magazine: ");
            strcpy(S.type, "Magazine\0");
        }

        fgets(t, 2, stdin);
        gets(S.Name);
        gotoxy(x, y + 12);
        printf("Enter Language: ");
        scanf("%s", S.lang);

        FILE *f;
        f = fopen(Newsletter, "ab+");
        if (f == NULL)
        {
            printf("File Does Not Exist");
            exit(1);
        }

        /* Writing Subscription in file */
        fwrite(&S, sizeof(struct Subscription), 1, f);
        fclose(f);

        gotoxy(70, y + 16);
        printf("Subscription Added Successfully...");

        /*  */
        for (int i = 45; i < 120; i++)
        {
            gotoxy(i, y + 17);
            printf("_");
        }

        /* Giving an option to add more Newspaper/magazine */
        int temp1 = 0;
        while (temp1 == 0)
        {
            /* key = 1 Newspaper */
            if (strcmp(key, "1\0") == 0) //////
            {
                gotoxy(x + 10, y + 19);
                printf("Do You Want to add more Newspaper [Y/n]: ");
            }                                 /* key = 2 Magazine */
            else if (strcmp(key, "2\0") == 0) //////
            {
                gotoxy(x + 10, y + 19);
                printf("Do You Want to add more Magazines [Y/n]: ");
            }

            char key2[10];
            scanf("%s", key2);

            /* Checking enterd key */
            if (strcmp(key2, "y\0") == 0 || strcmp(key2, "Y\0") == 0)
            {
                temp = 0;
                temp1++;
                break;
            }
            else if (strcmp(key2, "n\0") == 0 || strcmp(key2, "N\0") == 0)
            {
                temp++;
                temp1++;
                break;
            }
            else
            {
                temp = 0;
                gotoxy(x + 10, y + 21);
                printf("Please Enter Valid Key.");
                _getch();
                gotoxy(x + 10, y + 19);
                printf("                                                             ");
                gotoxy(x + 10, y + 21);
                printf("                                                             ");
            }
        }

        /* Erasing Rough material */
        int j = y + 10;
        for (j; j < y + 22; j++)
        {
            for (int i = 45; i < 120; i++)
            {
                gotoxy(i, j);
                printf(" ");
            }
        }
    }
}
void AddNonBorrowableItems()
{

    char key[10];
    system("cls");
    box(30);
    int x = 49, y = 7;
    gotoxy(65, 4);
    printf("ADD SUBSCRIPTIONS");

    gotoxy(x, y);
    printf("[1] : Add Newspaper");
    gotoxy(x, y + 2);
    printf("[2] : Add Magazine");

    gotoxy(x, y + 4);
    printf("[3] : Exit");

    /* Asking for option to select */
    int u = 0;
    while (u == 0)
    {
        gotoxy(x, y + 6);
        printf("Key: ");
        scanf("%s", key);
        if (strcmp(key, "1\0") == 0)
        {
            u++;
            gotoxy(65, y + 7);
            printf("ADD NEWSPAPER");
            break;
        }
        else if (strcmp(key, "2\0") == 0)
        {
            u++;
            gotoxy(65, y + 7);
            printf("ADD MAGAZINE");
            break;
        }
        else if (strcmp(key, "3\0") == 0)
        {
            return;
        }
        else
        {
            gotoxy(x, y + 8);
            printf("Please Enter Valid Key");
            _getch();
            gotoxy(x, y + 6);
            printf("                          ");
            gotoxy(x, y + 8);
            printf("                                        ");
        }
    }

    /* Will Create Rectangular Box Within box */
    for (int i = 45; i < 120; i++)
    {
        gotoxy(i, y + 6);
        printf("_");
    }
    for (int i = 45; i < 120; i++)
    {
        gotoxy(i, y + 8);
        printf("_");
    }
    /*  */

    /* Adding Newspaper/magzine as selected by admin */
    AddNewspaperMagzine(key, x, y);

    /* Checking to Add more subscription */
    while (1)
    {
        gotoxy(x + 10, 19);
        printf("Do You Want to add more Subscription [Y/n]: ");

        char key1[10];

        scanf("%s", key1);
        if (strcmp(key1, "y\0") == 0 || strcmp(key1, "Y\0") == 0)
        {
            AddNonBorrowableItems();
            break;
        }
        else if (strcmp(key1, "n\0") == 0 || strcmp(key1, "N\0") == 0)
        {
            return;
        }
        else
        {
            gotoxy(x + 10, 21);
            printf("Please Enter Valid Key.");
            _getch();
            gotoxy(x + 10, 21);
            printf("                           ");
            gotoxy(x + 10, 19);
            printf("                                                     ");
        }
    }

    system("cls");
}

void feedback()
{
    system("cls");
    FILE *fp;
    struct Feedback f;
    fp = fopen(feedbackfile, "ab+");
    if (fp == NULL)
    {
        printf("File Does Not Exist.");
        exit(1);
    }
    gotoxy(50, 3);
    printf("Give Your Valuable Feedback");
    printf("\n\n");
    printf("\t\tNOTE: User Information will not be recorded.\n\n");

    char t[2];

    time_t curtime;
    time(&curtime);

    strcpy(f.date, ctime(&curtime));
    fgets(t, 2, stdin);
    fgets(f.feed, 10000, stdin);

    fwrite(&f, sizeof(struct Feedback), 1, fp);
    fclose(fp);
    printf("\n\n\n\n\t\t\t\t\t\t\t\tThank You For Your Valuable Feedback\n\n\n\t\t\t\t\t\t\t\t\t\t\tPress any key to continue...");
    _getch();
}

void Readfeedback()
{
    system("cls");
    FILE *fp;
    struct Feedback f;
    fp = fopen(feedbackfile, "rb+");
    if (fp == NULL)
    {
        printf("File Does Not Exist.");
        exit(1);
    }
    gotoxy(50, 3);
    int i = 1;
    printf("Feedback\n\n");
    while (fread(&f, sizeof(struct Feedback), 1, fp))
    {
        printf("[%d]\t%s\n%s\n\n", i++, f.date, f.feed);
        printf("------------------------------------------------------------\n");
    }
    fclose(fp);
    printf("\n\n\n\t\t\t\t\t\t\t\tPress any key to continue...");
    _getch();
}

void referencefunctionforDisplayPrescription(char *key)
{
    system("cls");
    struct PrescribedBooks B;
    FILE *fp;
    int x = 10, y = 4;
    /*  */
    fp = fopen(BookPrescription, "ab+");
    if (fp == NULL)
    {
        printf("File does Not Exist");
        exit(1);
    }
    if (strcmp(key, "1\0") == 0)
    {
        int i = 1;
        gotoxy(70, 2);
        printf("Recommended Books");
        while (fread(&B, sizeof(struct PrescribedBooks), 1, fp))
        {
            if (strcmp(B.type, "Book\0") == 0)
            {
                gotoxy(x, y);
                printf("[%d]  %s", i++, B.Name);
                y = y + 2;
            }
        }
    }
    else if (strcmp(key, "2\0") == 0)
    {
        int i = 1;
        gotoxy(70, 2);
        printf("Recommended Subscription");
        while (fread(&B, sizeof(struct PrescribedBooks), 1, fp))
        {
            if (strcmp(B.type, "Subscription\0") == 0)
            {
                gotoxy(x, y);
                printf("[%d]  %s", i++, B.Name);
                y = y + 2;
            }
        }
    }
    fclose(fp);

    gotoxy(75, y + 4);
    printf("Press any key to continue...");
    _getch();
    system("cls");
    DisplayPrescription();
}
void DisplayPrescription()
{
    system("cls");
    box(20);
    int x = 55, y = 7;
    char key[10];
    /*  */
    gotoxy(65, 4);
    printf("Recommendations");
    gotoxy(x, y);
    printf("[1] : Books");
    gotoxy(x, y + 2);
    printf("[2] : Subscriptions");
    gotoxy(x, y + 4);
    printf("[3] : Exit");

    while (1)
    {
        gotoxy(x, y + 6);
        printf("Key: ");
        scanf("%s", key);

        /*  */
        if (strcmp(key, "1\0") == 0)
        {

            gotoxy(70, y + 7);
            printf("RECOMMENDED BOOKS");
            break;
        }
        else if (strcmp(key, "2\0") == 0)
        {
            gotoxy(70, y + 7);
            printf("RECOMMENDED SUBSCRIPTIONS");
            break;
        }
        else if (strcmp(key, "3\0") == 0)
        {
            return;
        }
        else
        {
            gotoxy(x, y + 8);
            printf("Please Enter Valid Key.");
            _getch();
            gotoxy(x, y + 8);
            printf("                               ");
            gotoxy(x, y + 6);
            printf("                            ");
        }
    }
    /*  */
    for (int i = 45; i < 120; i++)
    {

        gotoxy(i, y + 6);
        printf("_");
        gotoxy(i, y + 8);
        printf("_");
    }

    referencefunctionforDisplayPrescription(key);
}

void Referenceforprescribedbook(char *key, int x, int y)
{

    struct PrescribedBooks B;
    FILE *fp;
    char t[2];
    if (strcmp(key, "1\0") == 0)
    {
        gotoxy(x, y + 10);
        printf("Name of Book: ");
        strcpy(B.type, "Book\0");
    }
    else if (strcmp(key, "2\0") == 0)
    {
        gotoxy(x, y + 10);
        printf("Name of Subscription: ");
        strcpy(B.type, "Subscription\0");
    }
    fgets(t, 2, stdin);
    gets(B.Name);

    /*  */
    fp = fopen(BookPrescription, "ab+");
    if (fp == NULL)
    {
        printf("File does Not Exist");
        exit(1);
    }

    fwrite(&B, sizeof(struct PrescribedBooks), 1, fp);
    fclose(fp);

    gotoxy(70, y + 13);
    printf("Thank You for Your Recommendation...");

    gotoxy(80, y + 16);
    printf("Press any key to continue...");
    _getch();
    /* Erasing unwanted material */
    for (int j = y + 7; j < y + 17; j++)
    {
        for (int i = 45; i < 120; i++)
        {
            gotoxy(i, j);
            printf(" ");
        }
    }
}
void Prescribebooks()
{
    system("cls");
    box(25);
    gotoxy(65, 4);
    printf("Recommend Books/Subscriptions");
    int x = 49, y = 7;
    char key[10];
    /*  */
    gotoxy(x, y);
    printf("[1] : Books");
    gotoxy(x, y + 2);
    printf("[2] : Subscriptions");
    gotoxy(x, y + 4);
    printf("[3] : Exit");
    while (1)
    {
        gotoxy(x, y + 6);
        printf("Key: ");
        scanf("%s", key);

        /*  */
        if (strcmp(key, "1\0") == 0)
        {
            gotoxy(70, y + 7);
            printf("RECOMMEND BOOKS");
            break;
        }
        else if (strcmp(key, "2\0") == 0)
        {
            gotoxy(70, y + 7);
            printf("RECOMMEND SUBSCRIPTION");
            break;
        }
        else if (strcmp(key, "3\0") == 0)
        {
            return;
        }
        else
        {
            gotoxy(x, y + 8);
            printf("Please Enter Valid Key.");
            _getch();
            gotoxy(x, y + 8);
            printf("                               ");
            gotoxy(x, y + 6);
            printf("                            ");
        }
    }
    /*  */
    for (int i = 45; i < 120; i++)
    {
        gotoxy(i, y + 6);
        printf("_");
        gotoxy(i, y + 8);
        printf("_");
    }

    /*  */
    Referenceforprescribedbook(key, x, y);

    /*  */
    while (1)
    {
        gotoxy(x + 10, y + 8);
        printf("Do you want to Recommend more [Y/n]: ");
        char key1[10];
        scanf("%s", key1);
        if (strcmp(key1, "y\0") == 0 || strcmp(key1, "Y\0") == 0)
        {
            Prescribebooks();
            break;
        }
        else if (strcmp(key1, "n\0") == 0 || strcmp(key1, "N\0") == 0)
        {
            gotoxy(80, y + 16);
            printf("Press any key to continue...");
            _getch();
            return;
        }
        else
        {
            gotoxy(x + 10, y + 10);
            printf("Please Enter Valid Key.");
            _getch();
            gotoxy(x + 10, y + 10);
            printf("                               ");
            gotoxy(x + 10, y + 8);
            printf("                                                ");
        }
    }
}

void ForDeletingSub(char *key11, int x, int y)
{
    struct Books_InLib book;
    struct Subscription S;
    char nameOfBook[1000];
    char nameofsub[1000];
    struct availablebook availablebooks;
    char author[100];

    FILE *fp;
    char t[2];
    int c = 0;
    gotoxy(x, y + 10);
    printf("Name of Subscription: ");
    fgets(t, 2, stdin);
    gets(nameofsub);
    fp = fopen(Newsletter, "rb+");
    if (fp == NULL)
    {
        printf("File Does Not exist");
        exit(1);
    }
    while (fread(&S, sizeof(struct Subscription), 1, fp))
    {
        if (strcmp(S.Name, nameofsub) == 0)
        {
            c++;
            break;
        }
    }
    fclose(fp);
    if (c == 0)
    {
        gotoxy(70, y + 13);
        printf("Subscription is not available.");
        gotoxy(80, y + 16);
        printf("Press any key to continue...");
        _getch();
        gotoxy(x, y + 10);
        printf("                                                 ");
        gotoxy(70, y + 13);
        printf("                                       ");
        gotoxy(80, y + 16);
        printf("                                        ");
        return;
    }
    else
    {
        gotoxy(70, y + 13);
        printf("Subscription is available in library.");

        char key[10];
        for (int i = 45; i < 120; i++)
        {
            gotoxy(i, y + 14);
            printf("_");
        }
        while (1)
        {
            gotoxy(60, y + 16);
            printf("Would you like to delete all of its instance [Y/n]: ");
            scanf("%s", key);
            if (strcmp(key, "Y\0") == 0 || strcmp(key, "y\0") == 0)
            {
                FILE *fp1;
                fp1 = fopen(tempFile, "wb+");
                if (fp1 == NULL)
                {
                    printf("File Does Not Exist");
                    exit(1);
                }
                fp = fopen(Newsletter, "rb+");
                if (fp == NULL)
                {
                    printf("File does not exist");
                    exit(1);
                }
                while (fread(&S, sizeof(struct Subscription), 1, fp))
                {
                    if (strcmp(S.Name, nameofsub) == 0)
                    {
                        continue;
                    }
                    fwrite(&S, sizeof(struct Subscription), 1, fp1);
                }

                fclose(fp);
                fclose(fp1);

                fp1 = fopen(tempFile, "rb+");
                if (fp1 == NULL)
                {
                    printf("File Does Not Exist");
                    exit(1);
                }
                fp = fopen(Newsletter, "wb+");
                if (fp == NULL)
                {
                    printf("File does not exist");
                    exit(1);
                }
                while (fread(&S, sizeof(struct Subscription), 1, fp1))
                {

                    fwrite(&S, sizeof(struct Subscription), 1, fp);
                }

                fclose(fp);
                fclose(fp1);
                /* successfull msg bls bls */
                gotoxy(70, y + 19);
                printf("Subscription Deleted Successfully...");
                gotoxy(80, y + 22);
                printf("Press any key to continue...");
                _getch();
                for (int j = y + 10; j < y + 23; j++)
                {
                    for (int i = 45; i < 120; i++)
                    {
                        gotoxy(i, j);
                        printf(" ");
                    }
                }
                return;
            }
            else if (strcmp(key, "n\0") == 0 || strcmp(key, "N\0") == 0)
            {
                gotoxy(75, y + 19);
                printf("Press any key to continue...");
                _getch();
                for (int j = y + 10; j < y + 20; j++)
                {
                    for (int i = 45; i < 120; i++)
                    {
                        gotoxy(i, j);
                        printf(" ");
                    }
                }
                return;
            }
            else
            {
                /* bls bls bla */
                gotoxy(60, y + 18);
                printf("Please Enter Valid Key.");
                _getch();
                for (int i = 45; i < 120; i++)
                {
                    gotoxy(i, y + 16);
                    printf(" ");
                    gotoxy(i, y + 18);
                    printf(" ");
                }
            }
        }
    }
}
void forDeletingBooks(char *key11, int x, int y)
{
    struct Books_InLib book;
    struct Subscription S;
    char nameOfBook[1000];
    char nameofsub[1000];
    struct availablebook availablebooks;
    char author[100];

    FILE *fp;
    char t[2];

    int kk = y + 10;
    gotoxy(x, y + 10);
    printf("Name of Book: ");
    fgets(t, 2, stdin);
    gets(nameOfBook);

    fp = fopen(booksInLibraryfile, "rb+");
    if (fp == NULL)
    {
        printf("\nFile Does not Exist\n");
        exit(1);
    }

    FILE *f1;
    f1 = fopen(borrowbookfile, "wb+");
    if (f1 == NULL)
    {
        printf("\nFile Does not Exist\n");
        exit(1);
    }

    int check = 0, check11 = 0;
    int tem1 = 1;
    while (fread(&book, sizeof(struct Books_InLib), 1, fp))
    {
        if (strcmp(book.NameOfBook, nameOfBook) == 0)
        {
            check11++;
            if (strcmp(book.BookStatus, "Available\0") == 0)
            { //strcpy(status, book.BookStatus);
                // strcpy(author, book.Author);
                check++;
                strcpy(availablebooks.Author, book.Author);
                strcpy(availablebooks.Nameofbook, book.NameOfBook);
                availablebooks.index = tem1++;
                fwrite(&availablebooks, sizeof(struct availablebook), 1, f1);
            }
        }
    }

    if (check11 == 0)
    {
        gotoxy(70, y + 13);
        printf("Book is not available in Library.\n");
        gotoxy(70, y + 16);
        printf("Press any key to continue...");
        _getch();
        gotoxy(70, y + 16);
        printf("                                          ");
        gotoxy(70, y + 13);
        printf("                                           ");
        gotoxy(x, y + 10);
        printf("                                                                  ");
    }
    if (check11 != 0 && check == 0)
    {
        gotoxy(60, y + 13);
        printf("Book has been borrowed.Please wait till it is returned.");
        gotoxy(70, y + 16);
        printf("Press any key to continue...");
        _getch();
        for (int j = y + 10; j < y + 17; j++)
        {
            for (int i = 45; i < 120; i++)
            {
                gotoxy(i, j);
                printf(" ");
            }
        }
        /* gotoxy(70, y + 16);
            printf("                                         ");
            gotoxy(60, y + 13);
            printf("                                                             ");
            gotoxy(x, y + 10);
            printf("                                                                  "); */
    }
    fclose(f1);
    fclose(fp);

    if (check != 0 && check11 != 0)
    {
        /*  for (int j = 36; j < 38; j++)
            {
                for (int i = 45; i < 120; i++)
                {
                    gotoxy(i, j);
                    printf(" ");
                }
            }
            for (int j = 36; j <= (check * 3 + 20); j++)
            {
                gotoxy(45, j);
                printf("|");
                gotoxy(120, j);
                printf("|");
            }
            for (int i = 45; i < 120; i++)
            {
                gotoxy(i, y + 11);
                printf("_");
                gotoxy(i,check*3+20);
            } */
        system("cls");
        int tem = y;
        f1 = fopen(borrowbookfile, "rb+");
        if (f1 == NULL)
        {
            printf("\nFile Does not Exist\n");
            exit(1);
        }

        gotoxy(x, tem++);
        printf("Books Available");
        tem++;
        int count;
        while (fread(&availablebooks, sizeof(struct borrow), 1, f1))
        {
            tem++;
            gotoxy(x, tem++);
            printf("[%d]   %s", availablebooks.index, availablebooks.Nameofbook);
            gotoxy(x + 10, tem++);
            printf("By  %s", availablebooks.Author);
            count = availablebooks.index;
        }
        tem = tem + 2;
        count++;
        gotoxy(x, tem++);
        printf("[%d] : Exit", count);
        fclose(f1);

        int key;
        ////////////////////////
        int tem2 = 0;
        y = ++tem;
        while (tem2 == 0)
        {
            f1 = fopen(borrowbookfile, "rb+");
            if (f1 == NULL)
            {
                printf("\nFile Does not Exist\n");
                exit(1);
            }
            gotoxy(x, y);
            printf("Key: ");   //////////
            scanf("%d", &key); ///////////
            while (fread(&availablebooks, sizeof(struct availablebook), 1, f1))
            {
                if (availablebooks.index == key)
                {
                    strcpy(nameOfBook, availablebooks.Nameofbook);
                    strcpy(author, availablebooks.Author);
                    tem2++;
                    break;
                }
            }
            if (tem2 == 0)
            {
                if (key == count)
                {
                    tem2++;
                    system("cls");
                    box(25);
                    return;
                }
                else
                {
                    gotoxy(x, y + 2);
                    printf("Please Enter Valid Key.");
                    _getch();
                    gotoxy(x, y + 2);
                    printf("                                 ");
                    gotoxy(x, y);
                    printf("                       ");
                }
            }
            fclose(f1);
        }
        /*                 */
        f1 = fopen(tempFile, "wb+");
        if (f1 == NULL)
        {
            printf("File does not exist.");
            exit(1);
        }
        fp = fopen(booksInLibraryfile, "rb+");
        if (fp == NULL)
        {
            printf("File does not exist.");
            exit(1);
        }
        while (fread(&book, sizeof(struct Books_InLib), 1, fp))
        {
            if (strcmp(book.NameOfBook, nameOfBook) == 0 && strcmp(book.Author, author) == 0)
            {
                continue;
            }
            fwrite(&book, sizeof(struct Books_InLib), 1, f1);
        }
        fclose(fp);
        fclose(f1);

        f1 = fopen(tempFile, "rb+");
        if (f1 == NULL)
        {
            printf("File does not exist.");
            exit(1);
        }
        fp = fopen(booksInLibraryfile, "wb+");
        if (fp == NULL)
        {
            printf("File does not exist.");
            exit(1);
        }
        while (fread(&book, sizeof(struct Books_InLib), 1, f1))
        {

            fwrite(&book, sizeof(struct Books_InLib), 1, fp);
        }
        fclose(fp);
        fclose(f1);
        gotoxy(60, y + 4);
        printf("Book has been deleted successfully.");
        gotoxy(70, y + 7);
        printf("Press any key to continue...");

        _getch();
        /*  for (int j = kk; j < y + 8; j++)
                {
                    for (int i = 45; i < 120; i++)
                    {
                        gotoxy(i, j);
                        printf(" ");
                    }
                } */
        system("cls");
        box(25);
        ///
    }
}
void ReferencefunctionforDeleteMaterial(char *key11, int x, int y)
{

    if (strcmp(key11, "1\0") == 0)
    {
        forDeletingBooks(key11, x, y);
    }
    else if (strcmp(key11, "2\0") == 0)
    {
        ForDeletingSub(key11, x, y);
    }
}
void Deletematerials()
{
    system("cls");
    box(32);
    gotoxy(65, 4);
    printf("Delete Books/Subscriptions");
    int x = 49, y = 7;
    char key[10];
    /*  */
    gotoxy(x, y);
    printf("[1] : Books");
    gotoxy(x, y + 2);
    printf("[2] : Subscriptions");
    gotoxy(x, y + 4);
    printf("[3] : Exit");
    while (1)
    {
        gotoxy(x, y + 6);
        printf("Key: ");
        scanf("%s", key);

        /*  */
        if (strcmp(key, "1\0") == 0)
        {
            gotoxy(70, y + 7);
            printf("DELETE BOOKS");
            break;
        }
        else if (strcmp(key, "2\0") == 0)
        {
            gotoxy(70, y + 7);
            printf("DELETE SUBSCRIPTION");
            break;
        }
        else if (strcmp(key, "3\0") == 0)
        {
            return;
        }
        else
        {
            gotoxy(x, y + 8);
            printf("Please Enter Valid Key.");
            _getch();
            gotoxy(x, y + 8);
            printf("                               ");
            gotoxy(x, y + 6);
            printf("                            ");
        }
    }
    /*  */
    for (int i = 45; i < 120; i++)
    {
        gotoxy(i, y + 6);
        printf("_");
        gotoxy(i, y + 8);
        printf("_");
    }

    /*  */
    ReferencefunctionforDeleteMaterial(key, x, y);

    /*  */
    while (1)
    {
        for (int j = y + 7; j < y + 11; j++)
        {
            for (int i = 45; i < 120; i++)
            {
                gotoxy(i, j);
                printf(" ");
            }
        }
        gotoxy(x + 10, y + 10);
        printf("Do you want to Delete more Books/Subscription [Y/n]: ");
        char key1[10];
        scanf("%s", key1);
        if (strcmp(key1, "y\0") == 0 || strcmp(key1, "Y\0") == 0)
        {
            Deletematerials();
            break;
        }
        else if (strcmp(key1, "n\0") == 0 || strcmp(key1, "N\0") == 0)
        {
            gotoxy(80, y + 16);
            printf("Press any key to continue...");
            _getch();
            return;
        }
        else
        {
            gotoxy(x + 10, y + 12);
            printf("Please Enter Valid Key.");
            _getch();
            gotoxy(x + 10, y + 12);
            printf("                               ");
            gotoxy(x + 10, y + 10);
            printf("                                                            ");
        }
    }
}

/* Refrence function for DeleteUser(). It is used to delete books if user has issued it at time of deleting his/her account. Never called independently*/
void DeleteBooksofUser(struct User U)
{
    struct Books_InLib book1, book2, book;
    if (U.NoOfBookBorrowed == 1 || U.NoOfBookBorrowed == 2)
    {
        strcpy(book1.NameOfBook, U.booksBorrowed[0].NameOfBook);
        strcpy(book1.Author, U.booksBorrowed[0].Author);
        strcpy(book1.BookStatus, "Available\0");
    }
    if (U.NoOfBookBorrowed == 2)
    {
        strcpy(book2.NameOfBook, U.booksBorrowed[1].NameOfBook);
        strcpy(book2.Author, U.booksBorrowed[1].Author);
        strcpy(book2.BookStatus, "Available\0");
    }

    FILE *fp, *fp1;
    fp = fopen(booksInLibraryfile, "rb+");
    if (fp == NULL)
    {
        printf("File Does Not Exist.");
        exit(1);
    }
    fp1 = fopen(tempFile, "wb+");
    if (fp1 == NULL)
    {
        printf("File Does Not Exist.");
        exit(1);
    }
    int i = 0, j = 0;
    while (fread(&book, sizeof(struct Books_InLib), 1, fp))
    {
        if (strcmp(book1.NameOfBook, book.NameOfBook) == 0 && strcmp(book1.Author, book.Author) == 0 && strcmp(book.BookStatus, "Borrowed\0") == 0)
        {
            if (i == 0)
            {
                strcpy(book.BookStatus, "Available\0");
                i++;
            }
        }
        if (strcmp(book2.NameOfBook, book.NameOfBook) == 0 && strcmp(book2.Author, book.Author) == 0 && strcmp(book.BookStatus, "Borrowed\0") == 0)
        {
            if (j == 0)
            {
                strcpy(book.BookStatus, "Available\0");
                j++;
            }
        }
        fwrite(&book, sizeof(struct Books_InLib), 1, fp1);
    }

    fclose(fp);
    fclose(fp1);

    fp = fopen(booksInLibraryfile, "wb+");
    if (fp == NULL)
    {
        printf("File Does Not Exist.");
        exit(1);
    }
    fp1 = fopen(tempFile, "rb+");
    if (fp1 == NULL)
    {
        printf("File Does Not Exist.");
        exit(1);
    }
    while (fread(&book, sizeof(struct Books_InLib), 1, fp1))
    {
        fwrite(&book, sizeof(struct Books_InLib), 1, fp);
    }
    fclose(fp);
    fclose(fp1);
}
/* This function is used to delete account of inactive users by admin */
/* It is also based on algorithm 1 */
void DeleteUser()
{

    FILE *fp1;
    struct User U1;
    struct User U;

    system("cls");

    box(20);
    int x = 49, y = 7;

    gotoxy(x + 25, y - 3);
    printf("DELETE USER");

    gotoxy(x, y);
    printf("Enter Username : ");

    char user[20];
    scanf("%s", user);

    gotoxy(x, y + 2);
    printf("Enter User ID: ");
    char userid[20];
    scanf("%s", userid);

    fp1 = fopen(userInformationFile, "rb+");
    if (fp1 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    FILE *fp2;
    fp2 = fopen(tempFile, "wb+");
    if (fp2 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    int check1 = 0; //It is used to check if user exist and is deleted

    while (fread(&U, sizeof(struct User), 1, fp1))
    {
        if (strcmp(user, U.username) == 0 && strcmp(userid, U.userid) == 0)
        {
            gotoxy(x + 20, 16);
            printf("User Deleted Succesfully...");
            // DeleteBooksofUser(user1);
            //    copying information of user
            strcpy(U1.booksBorrowed[0].Author, U.booksBorrowed[0].Author);
            strcpy(U1.booksBorrowed[0].BookStatus, U.booksBorrowed[0].BookStatus);
            strcpy(U1.booksBorrowed[0].date, U.booksBorrowed[0].date);
            strcpy(U1.booksBorrowed[0].NameOfBook, U.booksBorrowed[0].NameOfBook);

            strcpy(U1.booksBorrowed[1].Author, U.booksBorrowed[1].Author);
            strcpy(U1.booksBorrowed[1].BookStatus, U.booksBorrowed[1].BookStatus);
            strcpy(U1.booksBorrowed[1].date, U.booksBorrowed[1].date);
            strcpy(U1.booksBorrowed[1].NameOfBook, U.booksBorrowed[1].NameOfBook);

            strcpy(U1.email, U.email);
            U1.check = U.check;
            strcpy(U1.firstname, U.firstname);
            strcpy(U1.lastname, U.lastname);
            strcpy(U1.password, U.password);
            U1.NoOfBookBorrowed = U.NoOfBookBorrowed;
            strcpy(U1.phoneno, U.phoneno);
            strcpy(U1.securityQusetionAns, U.securityQusetionAns);
            strcpy(U1.userid, U.userid);
            strcpy(U1.username, U.username);
            U1.securityQuestionIndex = U.securityQuestionIndex;

            check1++;
        }
        else
        {
            fwrite(&U, sizeof(struct User), 1, fp2);
        }
    }

    // if (check1 == 0)
    // {
    //     gotoxy(x + 20, 16);
    //     printf("User Not Recognized...");
    // }

    fclose(fp1);
    fclose(fp2);
    FILE *fp3, *fp4;

    fp3 = fopen(userInformationFile, "wb+");
    if (fp3 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    fp4 = fopen(tempFile, "rb+");
    if (fp4 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    while (fread(&U, sizeof(struct User), 1, fp4))
    {
        fwrite(&U, sizeof(struct User), 1, fp3);
    }
    fclose(fp3);
    fclose(fp4);

    if (check1 == 0)
    {
        gotoxy(x + 20, 16);
        printf("User Not Recognized...");
    }
    else
    {
        DeleteBooksofUser(U1);
    }

    gotoxy(85, 23);
    printf("Press any key to continue...");
    _getch();
    system("cls");
}

/*  */
/* Reference function for Return books never called indpendently */
void ReturnBothbooks(struct User U1)
{
    while (1)
    {
        system("cls");

        struct Books_InLib book, book1;

        char key[10];

        FILE *fp1;
        struct User user1;
        //Box
        int x = 49, y = 7;
        int i = 45, j = 5;
        int k = i - 1;

        for (i; i < 120; i++)
        {
            gotoxy(i, j);
            printf("_");
        }
        int l = i;
        for (j = j + 1; j < 25; j++)
        {
            gotoxy(i, j);
            printf("|");
            gotoxy(k, j);
            printf("|");
        }

        for (k = k + 1; k < 120; k++)
        {
            gotoxy(k, j - 1);
            printf("_");
        }

        gotoxy(85, 4);
        printf("RETURN BOOKS");
        gotoxy(x, y);
        printf("Do You Want To Return Both Books [Y/n] : ");
        //     gotoxy(x, y + 2);
        //     printf("[1] : Yes");
        //     gotoxy(x, y + 4);
        //     printf("[2] : No");
        //     gotoxy(x, y + 6);
        //     printf("Key: ");
        scanf("%s", key);
        // printf("\n");

        if (strcmp(key, "Y\0") == 0 || strcmp(key, "y\0") == 0)
        {
            fp1 = fopen(userInformationFile, "rb+");
            if (fp1 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            FILE *fp2;
            fp2 = fopen(tempFile, "wb+");
            if (fp2 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            while (fread(&user1, sizeof(struct User), 1, fp1))
            {
                if (strcmp(U1.username, user1.username) == 0 && strcmp(U1.userid, user1.userid) == 0)
                {
                    strcpy(book.BookStatus, "Available\0");                     //this information will be used to add books again to library
                    strcpy(book.NameOfBook, user1.booksBorrowed[0].NameOfBook); //this information will be used to add books again to library
                    strcpy(book.Author, user1.booksBorrowed[0].Author);         //this information will be used to add books again to library

                    strcpy(book1.BookStatus, "Available\0");                     //this information will be used to add books again to library
                    strcpy(book1.NameOfBook, user1.booksBorrowed[1].NameOfBook); //this information will be used to add books again to library
                    strcpy(book1.Author, user1.booksBorrowed[1].Author);         //this information will be used to add books again to library

                    // strcpy(user1.booksBorrowed[0].BookStatus, "\0");
                    strcpy(user1.booksBorrowed[0].NameOfBook, "\0");
                    strcpy(user1.booksBorrowed[0].date, "\0");
                    strcpy(user1.booksBorrowed[0].Author, "\0");

                    //strcpy(user1.booksBorrowed[1].BookID, "\0");
                    strcpy(user1.booksBorrowed[1].NameOfBook, "\0");
                    strcpy(user1.booksBorrowed[1].date, "\0");
                    strcpy(user1.booksBorrowed[1].Author, "\0");

                    user1.NoOfBookBorrowed = 0;
                }
                fwrite(&user1, sizeof(struct User), 1, fp2);
            }

            fclose(fp1);
            fclose(fp2);

            FILE *fp3, *fp4;
            struct User U;

            fp3 = fopen(userInformationFile, "wb+");
            if (fp3 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            fp4 = fopen(tempFile, "rb+");
            if (fp4 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            while (fread(&U, sizeof(struct User), 1, fp4))
            {
                fwrite(&U, sizeof(struct User), 1, fp3);
            }

            fclose(fp3);
            fclose(fp4);
            gotoxy(70, 15);
            printf("Books Returned successfully...\n");
            struct Books_InLib book2;
            FILE *fp5;
            fp5 = fopen(booksInLibraryfile, "rb+");
            if (fp5 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }

            fp3 = fopen(tempFile, "wb+");
            if (fp3 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }
            int c11 = 0, c12 = 0;
            while (fread(&book2, sizeof(struct Books_InLib), 1, fp5))
            {
                if (strcmp(book2.NameOfBook, book.NameOfBook) == 0 && strcmp(book2.Author, book.Author) == 0 && strcmp(book2.BookStatus, "Borrowed\0") == 0)
                {
                    if (c11 == 0)
                        ;
                    strcpy(book2.BookStatus, "Available\0");
                    c11++;
                }
                if (strcmp(book2.NameOfBook, book1.NameOfBook) == 0 && strcmp(book2.Author, book1.Author) == 0 && strcmp(book2.BookStatus, "Borrowed\0") == 0)
                {
                    if (c12 == 0)
                        strcpy(book2.BookStatus, "Available\0");
                    c12++;
                }
                fwrite(&book2, sizeof(struct Books_InLib), 1, fp3);
            }

            fclose(fp5);
            fclose(fp3);

            fp5 = fopen(booksInLibraryfile, "wb+");
            if (fp5 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }

            fp3 = fopen(tempFile, "rb+");
            if (fp3 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }
            while (fread(&book2, sizeof(struct Books_InLib), 1, fp3))
            {

                fwrite(&book2, sizeof(struct Books_InLib), 1, fp5);
            }
            fclose(fp3);
            fclose(fp5);
            return; ////////////////////////
        }
        else if (strcmp(key, "N\0") == 0 || strcmp(key, "n\0") == 0)
        {
            return;
        }
        else
        {
            gotoxy(x, y + 2);
            printf("Please Enter Valid Key.");
            _getch();
        }
    }
}
/* Reference function for Return books never called indpendently */
void ReturnsecondBook(struct User U1)
{
    while (1)
    {
        system("cls");

        struct Books_InLib book;

        //Box
        int x = 43, y = 7;
        int i = 40, j = 5;
        int k = i - 1;

        for (i; i < 140; i++)
        {
            gotoxy(i, j);
            printf("_");
        }
        int l = i;
        for (j = j + 1; j < 25; j++)
        {
            gotoxy(i, j);
            printf("|");
            gotoxy(k, j);
            printf("|");
        }

        for (k = k + 1; k < 140; k++)
        {
            gotoxy(k, j - 1);
            printf("_");
        }
        gotoxy(85, 4);
        printf("RETURN BOOKS");

        char key[10];

        FILE *fp1;
        struct User user1;
        gotoxy(x + 15, y);
        printf("Book to be Returned");
        gotoxy(x, y + 2);
        printf("[1] : %s", U1.booksBorrowed[1].NameOfBook);
        y += 2;
        gotoxy(x, y + 1);
        printf("      By %s", U1.booksBorrowed[1].Author);
        gotoxy(x, y + 2);
        printf("      Date of Issue: %s", U1.booksBorrowed[1].date);
        // y+=2;
        // gotoxy(x, y + 2);
        // printf("[1] : Confirm");
        gotoxy(x, y + 4);
        printf("[2] : None");
        gotoxy(x, y + 6);
        printf("Key: ");
        scanf("%s", key);
        printf("\n");

        if (strcmp(key, "1\0") == 0)
        {
            fp1 = fopen(userInformationFile, "rb+");
            if (fp1 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            FILE *fp2;
            fp2 = fopen(tempFile, "wb+");
            if (fp2 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            while (fread(&user1, sizeof(struct User), 1, fp1))
            {
                if (strcmp(U1.username, user1.username) == 0 && strcmp(U1.userid, user1.userid) == 0)
                {
                    strcpy(book.BookStatus, "Available\0");                     //this information will be used to add books again to library
                    strcpy(book.NameOfBook, user1.booksBorrowed[1].NameOfBook); //this information will be used to add books again to library
                    strcpy(book.Author, user1.booksBorrowed[1].Author);         //this information will be used to add books again to library

                    // strcpy(user1.booksBorrowed[1].BookID, "\0");
                    strcpy(user1.booksBorrowed[1].NameOfBook, "\0");
                    strcpy(user1.booksBorrowed[1].date, "\0");
                    strcpy(user1.booksBorrowed[1].Author, "\0");

                    user1.NoOfBookBorrowed = 1;
                }
                fwrite(&user1, sizeof(struct User), 1, fp2);
            }

            fclose(fp1);
            fclose(fp2);

            FILE *fp3, *fp4;
            struct User U;

            fp3 = fopen(userInformationFile, "wb+");
            if (fp3 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            fp4 = fopen(tempFile, "rb+");
            if (fp4 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            while (fread(&U, sizeof(struct User), 1, fp4))
            {
                fwrite(&U, sizeof(struct User), 1, fp3);
            }

            fclose(fp3);
            fclose(fp4);
            gotoxy(80, 19);
            printf("Book Returned successfully...");
            struct Books_InLib book1;
            FILE *fp5;
            fp5 = fopen(booksInLibraryfile, "rb+");
            if (fp5 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }

            fp3 = fopen(tempFile, "wb+");
            if (fp3 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }
            int c11 = 0;
            while (fread(&book1, sizeof(struct Books_InLib), 1, fp5))
            {
                if (strcmp(book1.NameOfBook, book.NameOfBook) == 0 && strcmp(book1.Author, book.Author) == 0 && strcmp(book1.BookStatus, "Borrowed\0") == 0)
                {
                    if (c11 == 0)
                        ;
                    strcpy(book1.BookStatus, "Available\0");
                    c11++;
                }
                fwrite(&book1, sizeof(struct Books_InLib), 1, fp3);
            }

            fclose(fp5);
            fclose(fp3);

            fp5 = fopen(booksInLibraryfile, "wb+");
            if (fp5 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }

            fp3 = fopen(tempFile, "rb+");
            if (fp3 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }
            while (fread(&book1, sizeof(struct Books_InLib), 1, fp3))
            {

                fwrite(&book1, sizeof(struct Books_InLib), 1, fp5);
            }
            fclose(fp3);
            fclose(fp5);
            return;
        }
        else if (strcmp(key, "2\0") == 0)
        {
            return;
        }
        else
        {
            gotoxy(x, y + 8);
            printf("Please Enter Valid Key.");
            _getch();
        }
    }
}
/* Reference function for Return books never called indpendently */
void Returnfirstbook(struct User U1)
{
    while (1)
    {
        system("cls");

        struct Books_InLib book;
        //Box
        int x = 43, y = 7;
        int i = 40, j = 5;
        int k = i - 1;

        for (i; i < 140; i++)
        {
            gotoxy(i, j);
            printf("_");
        }
        int l = i;
        for (j = j + 1; j < 25; j++)
        {
            gotoxy(i, j);
            printf("|");
            gotoxy(k, j);
            printf("|");
        }

        for (k = k + 1; k < 140; k++)
        {
            gotoxy(k, j - 1);
            printf("_");
        }

        gotoxy(85, 4);
        printf("RETURN BOOKS");

        char key[10];

        FILE *fp1;
        struct User user1;
        gotoxy(x, y);
        printf("Book to be Returned");
        gotoxy(x, y + 2);
        printf("[1] : %s", U1.booksBorrowed[0].NameOfBook);
        y += 2;
        gotoxy(x, y + 1);
        printf("      By %s", U1.booksBorrowed[0].Author);
        gotoxy(x, y + 2);
        printf("      Date of Issue: %s", U1.booksBorrowed[0].date);
        // y++;
        // gotoxy(x, y + 2);
        // printf("[1] : Confirm ");
        gotoxy(x, y + 4);
        printf("[2] : None");
        gotoxy(x, y + 6);
        printf("Key: ");
        scanf("%s", key);

        if (strcmp(key, "1\0") == 0)
        {
            fp1 = fopen(userInformationFile, "rb+");
            if (fp1 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            FILE *fp2;
            fp2 = fopen(tempFile, "wb+");
            if (fp2 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            while (fread(&user1, sizeof(struct User), 1, fp1))
            {
                if (strcmp(U1.username, user1.username) == 0 && strcmp(U1.userid, user1.userid) == 0)
                {
                    strcpy(book.BookStatus, "Available\0");                     //this information will be used to add books again to library
                    strcpy(book.NameOfBook, user1.booksBorrowed[0].NameOfBook); //this information will be used to add books again to library
                    strcpy(book.Author, user1.booksBorrowed[0].Author);         //this information will be used to add books again to library

                    /* Copying information from index[1] to [0] */
                    //  strcpy(user1.booksBorrowed[0].BookID, user1.booksBorrowed[1].BookID);
                    strcpy(user1.booksBorrowed[0].NameOfBook, user1.booksBorrowed[1].NameOfBook);
                    strcpy(user1.booksBorrowed[0].date, user1.booksBorrowed[1].date);
                    strcpy(user1.booksBorrowed[0].Author, user1.booksBorrowed[1].Author);

                    // strcpy(user1.booksBorrowed[1].BookID, "\0");
                    strcpy(user1.booksBorrowed[1].NameOfBook, "\0");
                    strcpy(user1.booksBorrowed[1].date, "\0");
                    strcpy(user1.booksBorrowed[1].Author, "\0");

                    user1.NoOfBookBorrowed = 1;
                }
                fwrite(&user1, sizeof(struct User), 1, fp2);
            }

            fclose(fp1);
            fclose(fp2);

            FILE *fp3, *fp4;
            struct User U;

            fp3 = fopen(userInformationFile, "wb+");
            if (fp3 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            fp4 = fopen(tempFile, "rb+");
            if (fp4 == NULL)
            {
                printf("File not found\n");
                exit(1);
            }

            while (fread(&U, sizeof(struct User), 1, fp4))
            {
                fwrite(&U, sizeof(struct User), 1, fp3);
            }

            fclose(fp3);
            fclose(fp4);

            gotoxy(80, 19);
            printf("Book Returned successfully...");

            struct Books_InLib book1;
            FILE *fp5;
            fp5 = fopen(booksInLibraryfile, "rb+");
            if (fp5 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }

            fp3 = fopen(tempFile, "wb+");
            if (fp3 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }
            int c11 = 0;
            while (fread(&book1, sizeof(struct Books_InLib), 1, fp5))
            {
                if (strcmp(book1.NameOfBook, book.NameOfBook) == 0 && strcmp(book1.Author, book.Author) == 0 && strcmp(book1.BookStatus, "Borrowed\0") == 0)
                {
                    if (c11 == 0)
                        strcpy(book1.BookStatus, "Available\0");
                    c11++;
                }
                fwrite(&book1, sizeof(struct Books_InLib), 1, fp3);
            }

            fclose(fp5);
            fclose(fp3);

            fp5 = fopen(booksInLibraryfile, "wb+");
            if (fp5 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }

            fp3 = fopen(tempFile, "rb+");
            if (fp3 == NULL)
            {
                printf("\nFile Does Not Exist\n");
                exit(1);
            }
            while (fread(&book1, sizeof(struct Books_InLib), 1, fp3))
            {

                fwrite(&book1, sizeof(struct Books_InLib), 1, fp5);
            }
            fclose(fp3);
            fclose(fp5);
            return;
        }
        else if (strcmp(key, "2\0") == 0)
        {
            return;
        }
        else
        {
            gotoxy(x, y + 8);
            printf("Please Enter Valid Key.");
            _getch();
        }
    }
}
/* This function is used to return book if issued  */
/* it is also based on algorithm 1 */
void Returnbook(struct User U1)

{
    system("cls");
    //Box

    int x = 43, y = 7;
    int i = 40, j = 5;
    int k = i - 1;

    for (i; i < 140; i++)
    {
        gotoxy(i, j);
        printf("_");
    }
    int l = i;
    for (j = j + 1; j < 25; j++)
    {
        gotoxy(i, j);
        printf("|");
        gotoxy(k, j);
        printf("|");
    }

    for (k = k + 1; k < 140; k++)
    {
        gotoxy(k, j - 1);
        printf("_");
    }

    gotoxy(85, 4);
    printf("RETURN BOOKS");
    if (U1.NoOfBookBorrowed == 1 || U1.NoOfBookBorrowed == 2)
    {

        struct Books_InLib book;

        char key[10];

        FILE *fp1;
        struct User user1;

        if (U1.NoOfBookBorrowed == 1)
        {
            while (1)
            {
                box1(25);
                int x = 43, y = 7;
                gotoxy(85, 4);
                printf("RETURN BOOKS");
                gotoxy(x, y);
                printf("Book to be Returned");
                gotoxy(x, y + 2);
                printf("[1] : %s", U1.booksBorrowed[0].NameOfBook);
                gotoxy(x, y + 3);
                printf("      By %s", U1.booksBorrowed[0].Author);
                gotoxy(x, y + 4);
                printf("      Date of Issue: %s", U1.booksBorrowed[0].date);

                y += 2;
                gotoxy(x, y + 4);
                printf("[2] : None");
                gotoxy(x, y + 6);
                printf("Key: ");
                scanf("%s", key);
                printf("\n");

                if (strcmp(key, "1\0") == 0)
                {
                    fp1 = fopen(userInformationFile, "rb+");
                    if (fp1 == NULL)
                    {
                        printf("File not found\n");
                        exit(1);
                    }

                    FILE *fp2;
                    fp2 = fopen(tempFile, "wb+");
                    if (fp2 == NULL)
                    {
                        printf("File not found\n");
                        exit(1);
                    }

                    while (fread(&user1, sizeof(struct User), 1, fp1))
                    {
                        if (strcmp(U1.username, user1.username) == 0 && strcmp(U1.userid, user1.userid) == 0)
                        {
                            strcpy(book.BookStatus, "Available\0");                     //this information will be used to add books again to library
                            strcpy(book.NameOfBook, user1.booksBorrowed[0].NameOfBook); //this information will be used to add books again to library
                            strcpy(book.Author, user1.booksBorrowed[0].Author);         //this information will be used to add books again to library

                            //  strcpy(user1.booksBorrowed[0].BookStatus, "\0");
                            strcpy(user1.booksBorrowed[0].NameOfBook, "\0");
                            strcpy(user1.booksBorrowed[0].date, "\0");
                            strcpy(user1.booksBorrowed[0].Author, "\0");
                            user1.NoOfBookBorrowed = 0;
                        }
                        fwrite(&user1, sizeof(struct User), 1, fp2);
                    }

                    fclose(fp1);
                    fclose(fp2);

                    FILE *fp3, *fp4;
                    struct User U;

                    fp3 = fopen(userInformationFile, "wb+");
                    if (fp3 == NULL)
                    {
                        printf("File not found\n");
                        exit(1);
                    }

                    fp4 = fopen(tempFile, "rb+");
                    if (fp4 == NULL)
                    {
                        printf("File not found\n");
                        exit(1);
                    }

                    while (fread(&U, sizeof(struct User), 1, fp4))
                    {
                        fwrite(&U, sizeof(struct User), 1, fp3);
                    }

                    fclose(fp3);
                    fclose(fp4);
                    gotoxy(80, 19);
                    printf("Book Returned successfully...");
                    struct Books_InLib book1;
                    FILE *fp5;
                    fp5 = fopen(booksInLibraryfile, "rb+");
                    if (fp5 == NULL)
                    {
                        printf("\nFile Does Not Exist\n");
                        exit(1);
                    }

                    fp3 = fopen(tempFile, "wb+");
                    if (fp3 == NULL)
                    {
                        printf("\nFile Does Not Exist\n");
                        exit(1);
                    }
                    int c11 = 0;
                    while (fread(&book1, sizeof(struct Books_InLib), 1, fp5))
                    {
                        if (strcmp(book1.NameOfBook, book.NameOfBook) == 0 && strcmp(book1.Author, book.Author) == 0 && strcmp(book1.BookStatus, "Borrowed\0") == 0)
                        {
                            if (c11 == 0)
                                strcpy(book1.BookStatus, "Available\0");
                            c11++;
                        }
                        fwrite(&book1, sizeof(struct Books_InLib), 1, fp3);
                    }

                    fclose(fp5);
                    fclose(fp3);

                    fp5 = fopen(booksInLibraryfile, "wb+");
                    if (fp5 == NULL)
                    {
                        printf("\nFile Does Not Exist\n");
                        exit(1);
                    }

                    fp3 = fopen(tempFile, "rb+");
                    if (fp3 == NULL)
                    {
                        printf("\nFile Does Not Exist\n");
                        exit(1);
                    }
                    while (fread(&book1, sizeof(struct Books_InLib), 1, fp3))
                    {

                        fwrite(&book1, sizeof(struct Books_InLib), 1, fp5);
                    }
                    fclose(fp3);
                    fclose(fp5);
                    break;
                }
                else if (strcmp(key, "1\0") == 0)
                {
                    return;
                }
                else
                {
                    gotoxy(x, y + 8);
                    printf("Please Enter Valid Key.");
                    _getch();
                }
            }
        }
        else if (U1.NoOfBookBorrowed == 2)
        {
            while (1)
            {
                box1(35);
                int x = 43, y = 7;
                char ab[10];
                gotoxy(85, 4);
                printf("RETURN BOOKS");
                gotoxy(x, y);
                printf("Books to be returned");
                gotoxy(x, y + 2);
                printf("[1] : %s", U1.booksBorrowed[0].NameOfBook);
                gotoxy(x, y + 3);
                printf("      By %s", U1.booksBorrowed[0].Author);
                gotoxy(x, y + 4);
                printf("      Date of Issue: %s", U1.booksBorrowed[0].date);
                y++;
                gotoxy(x, y + 5);
                printf("[2] : %s", U1.booksBorrowed[1].NameOfBook);
                gotoxy(x, y + 6);
                printf("      By %s", U1.booksBorrowed[1].Author);
                gotoxy(x, y + 7);
                printf("      Date of Issue: %s", U1.booksBorrowed[1].date);
                y++;
                gotoxy(x, y + 8);
                printf("[3] : Both");
                gotoxy(x, y + 10);
                printf("[4] : None");
                y += 2;
                gotoxy(x, y + 10);
                printf("Key : ");
                scanf("%s", ab);
                printf("\n");

                if (strcmp(ab, "1\0") == 0)
                {
                    /* Return 1st book */
                    Returnfirstbook(U1);
                    break;
                }
                else if (strcmp(ab, "2\0") == 0)
                {
                    /* Return 2nd book */
                    ReturnsecondBook(U1);
                    break;
                }
                else if (strcmp(ab, "3\0") == 0)
                {
                    /* Return both book */
                    ReturnBothbooks(U1);
                    break;
                }
                else if (strcmp(ab, "4\0") == 0)
                {
                    return;
                }
                else
                {
                    gotoxy(x, y + 12);
                    printf("Please Enter Valid Key.");
                    _getch();
                }
            }
        }
    }
    else
    {
        gotoxy(75, y + 8);
        printf("You don't have any book to return.");
    }
    gotoxy(85, 23);
    printf("Press any key to continue...");
    _getch();
    system("cls");
}

/* It will update Userinformation and books in library accordingly. It is a refrence funtion used for BorrowbookManually() fun */
void ReferenceforBorrowBookManually(struct User U1, struct borrow B1)
{
    struct User U;
    struct Books_InLib book;
    FILE *fp, *fp1;
    fp1 = fopen(userInformationFile, "rb+");
    if (fp1 == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }
    fp = fopen(tempFile, "wb+");
    if (fp == NULL)
    {
        printf("Files Does Not Exist");
        exit(1);
    }
    while (fread(&U, sizeof(struct User), 1, fp1))
    {
        if (strcmp(U1.userid, U.userid) == 0)
        {
            if (U1.NoOfBookBorrowed == 0)
            {
                strcpy(U.booksBorrowed[0].NameOfBook, B1.Nameofbook);
                strcpy(U.booksBorrowed[0].Author, B1.Author);
                time_t curtime;
                time(&curtime);
                strcpy(U.booksBorrowed[0].date, ctime(&curtime));
                U.NoOfBookBorrowed = 1;
            }
            else if (U1.NoOfBookBorrowed == 1)
            {
                strcpy(U.booksBorrowed[1].NameOfBook, B1.Nameofbook);
                strcpy(U.booksBorrowed[1].Author, B1.Author);
                time_t curtime;
                time(&curtime);
                strcpy(U.booksBorrowed[1].date, ctime(&curtime));
                U.NoOfBookBorrowed = 2;
            }
        }
        fwrite(&U, sizeof(struct User), 1, fp);
    }
    fclose(fp);
    fclose(fp1);
    fp1 = fopen(userInformationFile, "wb+");
    if (fp1 == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }
    fp = fopen(tempFile, "rb+");
    if (fp == NULL)
    {
        printf("Files Does Not Exist");
        exit(1);
    }
    while (fread(&U, sizeof(struct User), 1, fp))
    {
        fwrite(&U, sizeof(struct User), 1, fp1);
    }
    fclose(fp);
    fclose(fp1);

    /*  */

    fp1 = fopen(booksInLibraryfile, "rb+");
    if (fp1 == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }
    fp = fopen(tempFile, "wb+");
    if (fp == NULL)
    {
        printf("Files Does Not Exist");
        exit(1);
    }
    int check = 0;
    while (fread(&book, sizeof(struct Books_InLib), 1, fp1))
    {
        if (check == 0)
        {
            if (strcmp(book.NameOfBook, B1.Nameofbook) == 0 && strcmp(book.Author, B1.Author) == 0 && strcmp(book.BookStatus, "Available\0") == 0)
            {
                strcpy(book.BookStatus, "Borrowed\0");
                check++;
            }
        }
        fwrite(&book, sizeof(struct Books_InLib), 1, fp);
    }

    fclose(fp);
    fclose(fp1);
    fp1 = fopen(booksInLibraryfile, "wb+");
    if (fp1 == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }
    fp = fopen(tempFile, "rb+");
    if (fp == NULL)
    {
        printf("Files Does Not Exist");
        exit(1);
    }
    while (fread(&book, sizeof(struct Books_InLib), 1, fp))
    {
        fwrite(&book, sizeof(struct Books_InLib), 1, fp1);
    }
    fclose(fp);
    fclose(fp1);
}
void BorrowbookManually(struct User U1)
{
    system("cls");
    struct borrow B;
    struct Books_InLib book;
    FILE *fp, *fp1;
    fp = fopen(booksInLibraryfile, "rb+");
    if (fp == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }
    fp1 = fopen(tempFile, "wb+");
    if (fp1 == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }
    /* Printing Books and store it in temprary file/borrow struct */
    int i = 1; //will used for indexing
    int x = 5, y = 3;
    gotoxy(65, 1);
    printf("Books to be borrowed\n");
    /*   gotoxy(63, 2);
    printf("------------------------------\n"); */
    gotoxy(x, y);
    printf("========================================================================================================================================");
    gotoxy(x + 2, y + 1);
    printf("KEY");
    /*   gotoxy(x + 12, y + 1);
    printf("Status"); */
    gotoxy(x + 12, y + 1);
    printf("Author");
    gotoxy(x + 70, y + 1);
    printf("Name of Book");
    gotoxy(x, y + 2);
    printf("========================================================================================================================================");

    while (fread(&book, sizeof(struct Books_InLib), 1, fp))
    {
        if (strcmp(book.BookStatus, "Available\0") == 0)
        {
            B.index = i;
            gotoxy(x + 3, y + 3);
            printf("[%d]", i++);
            // gotoxy(x + 10, y + 3);
            // printf("%s", book.BookStatus);
            gotoxy(x + 10, y + 3);
            printf("%s", book.Author);
            gotoxy(x + 62, y + 3);
            printf("%s", book.NameOfBook);
            y++;

            strcpy(B.Author, book.Author);
            strcpy(B.Nameofbook, book.NameOfBook);
            fwrite(&B, sizeof(struct borrow), 1, fp1);
            // printf("   %d   |\t%s\t\t|\t%s\t\t\t\t\t\t|\t\t%s\n", i++, book.Author, book.NameOfBook, book.BookStatus);
        }
    }

    fclose(fp);
    fclose(fp1);

    gotoxy(x + 3, y + 5);
    printf("[%d] : Exit", i);

    for (int i = 45; i < 120; i++)
    {
        gotoxy(i, y + 8);
        printf("_");
        gotoxy(i, y + 24);
        printf("_");
    }
    for (int j = y + 9; j < y + 25; j++)
    {
        gotoxy(45, j);
        printf("|");
        gotoxy(120, j);
        printf("|");
    }

    /*  */
    while (1)
    {
        int key;
        gotoxy(50, y + 10);
        printf("Enter Key: ");
        scanf("%d", &key);
        int check = 0, check1 = 0;
        fp1 = fopen(tempFile, "rb+");
        if (fp1 == NULL)
        {
            printf("File Does Not Exist");
            exit(1);
        }
        struct borrow B1;
        while (fread(&B, sizeof(struct borrow), 1, fp1))
        {
            if (key == B.index)
            {
                check++;
                strcpy(B1.Author, B.Author);
                strcpy(B1.Nameofbook, B.Nameofbook);
                B1.index = B.index;
                break;
            }
        }
        fclose(fp1);
        if (check == 0)
        {
            if (key == i)
            {
                return;
            }
            else
            {
                gotoxy(50, y + 12);
                printf("Enter Valid Key.");
                _getch();
                gotoxy(50, y + 12);
                printf("                                  ");
                gotoxy(50, y + 10);
                printf("                                        ");
            }
        }
        else
        {
            /* U1,boorow B1 */
            //////////borrow selected books
            ReferenceforBorrowBookManually(U1, B1);
            gotoxy(70, y + 17);
            printf("Congratulations Book Issued...");
            gotoxy(80, y + 23);
            printf("Press any key to continue...");
            _getch();
            return;
            system("cls");
        }
    }
}
/* This function will be used to issue book to perticular user */
/* It is also based on algorithm 1 */
void BorrowBook(struct User U1)
{
    system("cls");

    int x = 44, y = 7;

    gotoxy(80, 4);
    printf("BORROW BOOKS");
    gotoxy(77, 5);
    printf("------------------");
    if (U1.NoOfBookBorrowed == 0 || U1.NoOfBookBorrowed == 1)
    {

        struct User user1;
        char t[2];

        struct Books_InLib book;

        char nameOfBook[1000];
        char status[30];
        char author[100];

        /* Here we are searching for the book if it is available or not */
        // system("cls");

        gotoxy(x, y);
        printf("Enter Name of book : ");
        fgets(t, 2, stdin);
        //scanf("%s", &nameOfBook);
        gets(nameOfBook);

        FILE *fp;
        fp = fopen(booksInLibraryfile, "rb+");
        if (fp == NULL)
        {
            printf("\nFile Does not Exist\n");
            exit(1);
        }

        FILE *f1;
        f1 = fopen(borrowbookfile, "wb+");
        if (f1 == NULL)
        {
            printf("\nFile Does not Exist\n");
            exit(1);
        }
        struct borrow availablebooks;

        int check = 0;
        int tem1 = 1;
        while (fread(&book, sizeof(struct Books_InLib), 1, fp))
        {
            if (strcmp(book.NameOfBook, nameOfBook) == 0 && strcmp(book.BookStatus, "Available\0") == 0)
            {
                //strcpy(status, book.BookStatus);
                // strcpy(author, book.Author);
                check++;
                strcpy(availablebooks.Author, book.Author);
                strcpy(availablebooks.Nameofbook, book.NameOfBook);
                availablebooks.index = tem1++;
                fwrite(&availablebooks, sizeof(struct borrow), 1, f1);
            }
        }

        if (check == 0)
        {
            gotoxy(75, 10);
            printf("Book is not available in Library\n");
        }
        fclose(f1);
        fclose(fp);

        if (check != 0)
        {
            int tem = y + 2;
            f1 = fopen(borrowbookfile, "rb+");
            if (f1 == NULL)
            {
                printf("\nFile Does not Exist\n");
                exit(1);
            }

            gotoxy(x, tem++);
            printf("Books Available");
            tem++;
            int count;
            while (fread(&availablebooks, sizeof(struct borrow), 1, f1))
            {
                tem++;
                gotoxy(x, tem++);
                printf("[%d]   %s", availablebooks.index, availablebooks.Nameofbook);
                gotoxy(x + 10, tem++);
                printf("By  %s", availablebooks.Author);
                count = availablebooks.index;
            }
            tem = tem + 2;
            count++;
            gotoxy(x, tem++);
            printf("[%d] : Exit", count);
            fclose(f1);

            int key;

            int tem2 = 0;
            y = ++tem;
            while (tem2 == 0)
            {
                f1 = fopen(borrowbookfile, "rb+");
                if (f1 == NULL)
                {
                    printf("\nFile Does not Exist\n");
                    exit(1);
                }
                gotoxy(x, y);
                printf("Key: ");
                scanf("%d", &key);
                while (fread(&availablebooks, sizeof(struct borrow), 1, f1))
                {
                    if (availablebooks.index == key)
                    {
                        strcpy(nameOfBook, availablebooks.Nameofbook);
                        strcpy(author, availablebooks.Author);
                        tem2++;
                        break;
                    }
                }
                if (tem2 == 0)
                {
                    if (key == count)
                    {
                        tem2++;
                        return;
                    }
                    else
                    {
                        gotoxy(x, y + 2);
                        printf("Please Enter Valid Key.");
                        _getch();
                        gotoxy(x, y + 2);
                        printf("                                 ");
                        gotoxy(x, y);
                        printf("                       ");
                    }
                }
                fclose(f1);
            }

            if (tem2 == 1)
            {
                FILE *fp1;
                fp1 = fopen(userInformationFile, "rb+");
                if (fp1 == NULL)
                {
                    printf("File not found\n");
                    exit(1);
                }

                FILE *fp2;

                fp2 = fopen(tempFile, "wb+");
                if (fp2 == NULL)
                {
                    printf("File not found\n");
                    exit(1);
                }

                // printf("\nEnter date: ");
                char date[100];
                time_t curtime;
                time(&curtime);
                strcpy(date, ctime(&curtime));
                // scanf("%s", date);
                printf("\n");

                while (fread(&user1, sizeof(struct User), 1, fp1))
                {
                    if (strcmp(U1.username, user1.username) == 0 && strcmp(U1.userid, user1.userid) == 0)
                    {
                        if (user1.NoOfBookBorrowed == 0)
                        {
                            // strcpy(user1.booksBorrowed[0].BookStatus, status);
                            strcpy(user1.booksBorrowed[0].NameOfBook, nameOfBook);
                            strcpy(user1.booksBorrowed[0].date, date);
                            strcpy(user1.booksBorrowed[0].Author, author);
                            user1.NoOfBookBorrowed = 1;
                        }
                        else
                        {

                            // strcpy(user1.booksBorrowed[1].BookStatus, status);
                            strcpy(user1.booksBorrowed[1].NameOfBook, nameOfBook);
                            strcpy(user1.booksBorrowed[1].date, date);
                            strcpy(user1.booksBorrowed[1].Author, author);
                            user1.NoOfBookBorrowed = 2;
                        }
                    }

                    fwrite(&user1, sizeof(struct User), 1, fp2);
                }

                fclose(fp1);
                fclose(fp2);

                FILE *fp3, *fp4;
                FILE *fp6, *fp7;
                struct User U;

                fp6 = fopen(userInformationFile, "wb+");
                if (fp6 == NULL)
                {
                    printf("File not found\n");
                    exit(1);
                }

                fp7 = fopen(tempFile, "rb+");
                if (fp7 == NULL)
                {
                    printf("File not found\n");
                    exit(1);
                }
                /* Copying information from temporary file to userinformation(original file) */
                while (fread(&U, sizeof(struct User), 1, fp7))
                {
                    fwrite(&U, sizeof(struct User), 1, fp6);
                }

                fclose(fp6);
                fclose(fp7);
                gotoxy(70, y + 5);
                printf("Congratulations Book Issued ...");

                /* Now onwards removing book from available list of books */

                fp3 = fopen(booksInLibraryfile, "rb+");
                if (fp3 == NULL)
                {
                    printf("\nFile Does Not Exist.\n");
                    exit(1);
                }
                fp4 = fopen(tempFile, "wb+");
                if (fp4 == NULL)
                {
                    printf("\nFile Does Not Exist.\n");

                    exit(1);
                }
                struct Books_InLib book1;
                int c11 = 0;
                while (fread(&book1, sizeof(struct Books_InLib), 1, fp3))
                {
                    if (strcmp(book1.NameOfBook, nameOfBook) == 0 && strcmp(book1.Author, author) == 0 && strcmp(book1.BookStatus, "Available\0") == 0)
                    {
                        if (c11 == 0)
                        {
                            strcpy(book1.BookStatus, "Borrowed\0");
                            c11++;
                        }
                        // continue;
                    }
                    fwrite(&book1, sizeof(struct Books_InLib), 1, fp4);
                }
                fclose(fp3);
                fclose(fp4);
                fp3 = fopen(booksInLibraryfile, "wb+");
                if (fp3 == NULL)
                {
                    exit(1);
                }
                fp4 = fopen(tempFile, "rb+");
                if (fp4 == NULL)
                {
                    exit(1);
                }
                while (fread(&book1, sizeof(struct Books_InLib), 1, fp4))
                {
                    fwrite(&book1, sizeof(struct Books_InLib), 1, fp3);
                }
                fclose(fp3);
                fclose(fp4);
            }
            /*  else if (key == 2)
            {
            }
            else
            {
                gotoxy(x, y + 12);
                printf("Please Enter valid key.");
            } */
        }
        /* Maybe Here we can code for searching another book */
    }
    else
    {
        gotoxy(x, 15);
        printf("You have reached the maximum limit of books that can be borrowed.");
    }

    gotoxy(90, y + 11);
    printf("Press any key to continue...");
    _getch();
    system("cls");
}

void beforeborrowingbook(struct User U1)
{
    system("cls");
    box(25);
    if (U1.NoOfBookBorrowed == 0 || U1.NoOfBookBorrowed == 1)
    {
        while (1)
        {
            int x = 49, y = 7;
            gotoxy(70, 4);
            printf("BORROW BOOKS");
            gotoxy(x, y);
            printf("[1] : Search by Name");
            gotoxy(x, y + 2);
            printf("[2] : Search Manually");
            gotoxy(x, y + 4);
            printf("[3] : Exit");
            char key[10];
            gotoxy(x, y + 6);
            printf("Key: ");
            scanf("%s", key);
            if (strcmp(key, "1\0") == 0)
            {
                BorrowBook(U1);
                return;
            }
            else if (strcmp(key, "2\0") == 0)
            {
                BorrowbookManually(U1);
                return;
            }
            else if (strcmp(key, "3\0") == 0)
            {
                system("cls");
                return;
            }
            else
            {
                gotoxy(x, y + 8);
                printf("Please Enter Valid Key.");
                _getch();
                gotoxy(x, y + 6);
                printf("                           ");
                gotoxy(x, y + 8);
                printf("                                       ");
            }
        }
    }
    else
    {
        gotoxy(49, 15);
        printf("You have reached the maximum limit of books that can be borrowed.");
        gotoxy(95, 30);
        printf("Press any key to continue...");
        _getch();
        system("cls");
    }
}
/* This function will print details of admin */
/* In this function we will first open 'DetailsOfAdmin' file in reading mode then read and print it accordingly */
void printDetailsofAdmin()
{
    FILE *fp1;
    struct Admin admin;

    system("cls");
    gotoxy(55, 8);
    printf(" Details of Admin \n\n");

    fp1 = fopen(DetailsofAdminfile, "rb+");
    if (fp1 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    while (fread(&admin, sizeof(struct Admin), 1, fp1))
    {
        printf("\n\t\t\t\t\t\t----------------");

        printf("\n\t\t\t\t\t\tName: \t%s %s \n", admin.firstname, admin.lastname);
        printf("\n\t\t\t\t\t\tUsername: %s\n", admin.username);
        printf("\n\t\t\t\t\t\tPhone No.: %s\n", admin.phoneno);
        printf("\n\t\t\t\t\t\tEmail Id : %s\n", admin.email);
        printf("\t\t\t\t\t\t------------------------------------");
    }

    fclose(fp1);
    gotoxy(80, 25);

    printf("Press any key to continue...");
    _getch();
    system("cls");
}

void printdetailsofallusers()
{
    FILE *fp1;
    struct User user1;

    system("cls");
    gotoxy(52, 3);
    printf(" Details of All Users are ...\n\n");

    fp1 = fopen(userInformationFile, "rb+");
    if (fp1 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    int i = 1; //will be used for indexing

    int y = 6, j = 6;

    while (fread(&user1, sizeof(struct User), 1, fp1))
    {
        if (i % 2 != 0)
        {
            gotoxy(6, y);
            printf("[%d]", i++);

            gotoxy(11, y++);
            printf("Name:\t%s %s", user1.firstname, user1.lastname);
            gotoxy(11, y++);
            printf("Username:\t%s", user1.username);
            gotoxy(11, y++);
            printf("Registration No.:\t%s", user1.userid);
            gotoxy(11, y++);
            printf("Phone No.:\t%s", user1.phoneno);
            gotoxy(11, y++);
            printf("Email ID:\t%s", user1.email);
            gotoxy(11, y++);
            printf("------------------------------------------------");
        }
        else
        {
            gotoxy(70, j);
            printf("[%d]", i++);

            gotoxy(75, j++);
            printf("Name:\t%s %s", user1.firstname, user1.lastname);
            gotoxy(75, j++);
            printf("Username:\t%s", user1.username);
            gotoxy(75, j++);
            printf("Registration No.:\t%s", user1.userid);
            gotoxy(75, j++);
            printf("Phone No.:\t%s", user1.phoneno);
            gotoxy(75, j++);
            printf("Email ID:\t%s", user1.email);
            gotoxy(75, j++);
            printf("------------------------------------------------");
        }
    }
    fclose(fp1);

    printf("\n\n\n\t\t\t\t\t\t\t\t\tPress any key to continue...");
    _getch();
    system("cls");
}

/* This function will be used for changing deatails of user like password and contact details if he/she wants to do */
/* It is also based on Algorithm 1 */
void ChangeAccountDetails(struct User user)
{
    FILE *fp1;
    struct User user1;
    system("cls");

    fp1 = fopen(userInformationFile, "rb+");
    if (fp1 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    FILE *fp2;
    fp2 = fopen(tempFile, "wb+");
    if (fp2 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    while (fread(&user1, sizeof(struct User), 1, fp1))
    {
        if (strcmp(user.username, user1.username) == 0 && strcmp(user.userid, user1.userid) == 0)
        {

            int u = 0;
            while (u == 0)
            {
                int ab;
                system("cls");
                box(25);
                int x = 49, y = 7;
                gotoxy(x + 20, y - 3);
                printf("Change Your Account Details");
                gotoxy(x, y);
                printf("[1] : Change password");
                gotoxy(x, y + 2);
                printf("[2] : Change Email Id");
                gotoxy(x, y + 4);
                printf("[3] : Change Mobile No.");
                gotoxy(x, y + 6);
                printf("[4] : Exit");
                gotoxy(x, y + 8);
                printf("Key :");
                scanf("%d", &ab);

                switch (ab)
                {
                case 1:
                    for (int i = x - 3; i < 120; i++)
                    {
                        gotoxy(i, y + 9);
                        printf("_");
                    }
                    gotoxy(x, y + 11);
                    printf("Enter New Password : ");
                    char p[20];
                    scanf("%s", p);
                    strcpy(user1.password, p);
                    gotoxy(80, 23);
                    printf("Password Changed Successfully...");
                    _getch();
                    break;

                case 2:
                    for (int i = x - 3; i < 120; i++)
                    {
                        gotoxy(i, y + 9);
                        printf("_");
                    }
                    gotoxy(x, y + 11);
                    printf("Enter New Email Address : ");
                    char e[30];
                    scanf("%s", e);
                    strcpy(user1.email, e);
                    gotoxy(80, 23);
                    printf("Email ID Changed Successfully...");
                    _getch();
                    break;

                case 3:
                    for (int i = x - 3; i < 120; i++)
                    {
                        gotoxy(i, y + 9);
                        printf("_");
                    }
                    gotoxy(x, y + 11);
                    printf("Enter New Mobile Number : ");
                    char ph[14];
                    scanf("%s", ph);
                    strcpy(user1.phoneno, ph);
                    gotoxy(80, 23);
                    printf("Mobile Number Changed Successfully...");
                    _getch();
                    break;

                case 4:
                    u++;

                    break;
                default:
                    gotoxy(x, y + 10);
                    printf("Please Enter Valid Key.");
                    _getch();
                    break;
                }
            }
        }

        fwrite(&user1, sizeof(struct User), 1, fp2);
    }

    fclose(fp1);
    fclose(fp2);

    FILE *fp3, *fp4;

    struct User U;
    fp3 = fopen(userInformationFile, "wb+");
    if (fp3 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    fp4 = fopen(tempFile, "rb+");
    if (fp4 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    while (fread(&U, sizeof(struct User), 1, fp4))
    {
        fwrite(&U, sizeof(struct User), 1, fp3);
    }

    fclose(fp3);
    fclose(fp4);

    gotoxy(100, 27);
    printf("Press any key to continue...");
    _getch();
    system("cls");
}

/* This will be used to print details of user from user panel */
void VerifyAccount(char userName[], char Userid[])
{
    FILE *fp1;
    struct User user1;

    system("cls");
    gotoxy(65, 10);
    printf(" Your Details are ...\n\n");

    fp1 = fopen(userInformationFile, "rb+");
    if (fp1 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    while (fread(&user1, sizeof(struct User), 1, fp1))
    {
        if (strcmp(userName, user1.username) == 0 && strcmp(Userid, user1.userid) == 0)
        {

            printf("\t\t\t\t\t\t\t--------------\n");
            printf("\t\t\t\t\t\t\tName:\t\t%s %s\n", user1.firstname, user1.lastname);
            printf("\t\t\t\t\t\t\tUsername:\t%s\n", user1.username);
            printf("\t\t\t\t\t\t\tRegistration No.:\t%s\n", user1.userid);
            printf("\t\t\t\t\t\t\tPassword:\t%s\n", user1.password);
            printf("\t\t\t\t\t\t\tMobile No.:\t%s\n", user1.phoneno);
            printf("\t\t\t\t\t\t\tEmail ID:\t%s\n", user1.email);

            printf("\t\t\t\t\t\t\tNo. of books borrowed:\t%d\n", user1.NoOfBookBorrowed);

            if (user1.NoOfBookBorrowed == 1 || user1.NoOfBookBorrowed == 2)
            {
                printf("\n\t\t\t\t\t\t\t [1]\tName of book:\t%s\n", user1.booksBorrowed[0].NameOfBook);
                printf("\t\t\t\t\t\t\t\tName of Author:\t%s\n", user1.booksBorrowed[0].Author);
                printf("\t\t\t\t\t\t\t\tDate of Issue:\t%s\n", user1.booksBorrowed[0].date);
            }
            if (user1.NoOfBookBorrowed == 2)
            {
                printf("\n\t\t\t\t\t\t\t [2]\tName of book:\t%s\n", user1.booksBorrowed[1].NameOfBook);
                printf("\t\t\t\t\t\t\t\tName of Author:\t%s\n", user1.booksBorrowed[1].Author);
                printf("\t\t\t\t\t\t\t\tDate of Issue:\t%s\n", user1.booksBorrowed[1].date);
            }
            printf("\t\t\t\t\t\t\t------------------------------------\n");

            break;
        }
    }

    fclose(fp1);

    printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\t\tPress any key to continue...");
    _getch();
    system("cls");
}

/* This is a Admin function used to print details of perticular user */
void printdetailsofperticuleruser()
{
    struct User user;
    FILE *fp;

    char username[20];
    char userid[20];
    system("cls");
    printf("\t\t\t\t\t\t\t\t\t\t\t\tSEARCH USER");
    gotoxy(20, 2);
    printf("Enter Username : ");
    scanf("%s", &username);
    gotoxy(20, 4);
    printf("Enter Registration No.: ");
    scanf("%s", &userid);
    printf("\n\n\n\n\n\n\n");

    /*  */
    fp = fopen(userInformationFile, "rb+");
    if (fp == NULL)
    {
        printf("\nFile does not exist\n");
        exit(1);
    }

    int check = 0; //Used to check if enterd username and password is correct
    while (fread(&user, sizeof(struct User), 1, fp))
    {
        if (strcmp(user.userid, userid) == 0 && strcmp(user.username, username) == 0)
        {
            printf("\t\t\t\t\t\t\t******* Details of '%s %s' *******\n\n", user.firstname, user.lastname);
            printf("\t\t\t\t\t\t--------------\n");
            printf("\t\t\t\t\t\tName:\t\t%s %s\n", user.firstname, user.lastname);
            printf("\t\t\t\t\t\tRegistration No.:\t%s\n", user.userid);
            printf("\t\t\t\t\t\tMobile No.:\t%s\n", user.phoneno);
            printf("\t\t\t\t\t\tEmail ID:\t%s\n", user.email);
            ////////////////////////////////////////////////////////////////////////

            printf("\t\t\t\t\t\tNo. of books borrowed:\t%d\n", user.NoOfBookBorrowed);
            check++;
            if (user.NoOfBookBorrowed == 1 || user.NoOfBookBorrowed == 2)
            {
                printf("\n\t\t\t\t\t\t [1]\tName of book:\t%s\n", user.booksBorrowed[0].NameOfBook);
                printf("\t\t\t\t\t\t\tName of Author:\t%s\n", user.booksBorrowed[0].Author);
                printf("\t\t\t\t\t\t\tDate of Issue:\t%s\n", user.booksBorrowed[0].date);
            }
            if (user.NoOfBookBorrowed == 2)
            {
                printf("\n\t\t\t\t\t\t [2]\tName of book:\t%s\n", user.booksBorrowed[1].NameOfBook);
                printf("\t\t\t\t\t\t\tName of Author:\t%s\n", user.booksBorrowed[1].Author);
                printf("\t\t\t\t\t\t\tDate of Issue:\t%s\n", user.booksBorrowed[1].date);
            }
            printf("\t\t\t\t\t\t------------------------------------\n");
        }
    }

    if (check == 0)
    {
        printf("\n\n\t\t\t\t\t\t\t\tUser did not Recoganised...");
    }

    fclose(fp);
    printf("\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\tPress any key to continue...");
    _getch();
    system("cls");
}

/* Both User and Admin function */
void printallbooks()
{
    struct Books_InLib book;
    FILE *fp;
    system("cls");

    fp = fopen(booksInLibraryfile, "rb+");
    if (fp == NULL)
    {
        printf("\nFile does not exist\n");
        exit(1);
    }

    int i = 1; //will used for indexing
    int x = 5, y = 3;
    gotoxy(65, 1);
    printf("All Books In Library\n");
    /*   gotoxy(63, 2);
    printf("------------------------------\n"); */
    gotoxy(x, y);
    printf("========================================================================================================================================");
    gotoxy(x + 2, y + 1);
    printf("Sr.No.");
    gotoxy(x + 12, y + 1);
    printf("Status");
    gotoxy(x + 35, y + 1);
    printf("Author");
    gotoxy(x + 85, y + 1);
    printf("Name of Book");
    gotoxy(x, y + 2);
    printf("========================================================================================================================================");

    while (fread(&book, sizeof(struct Books_InLib), 1, fp))
    {
        gotoxy(x + 3, y + 3);
        printf("%d", i++);
        gotoxy(x + 10, y + 3);
        printf("%s", book.BookStatus);
        gotoxy(x + 30, y + 3);
        printf("%s", book.Author);
        gotoxy(x + 82, y + 3);
        printf("%s", book.NameOfBook);
        y++;

        // printf("   %d   |\t%s\t\t|\t%s\t\t\t\t\t\t|\t\t%s\n", i++, book.Author, book.NameOfBook, book.BookStatus);
    }

    fclose(fp);
    printf("\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\tPress any key to continue...");
    _getch();
    system("cls");
}

/* Admin function */
void AddBook()
{
    struct Books_InLib Addbook;
    char t[2];
    //box
    system("cls");
    int i = 40, j = 5;
    int k = i - 1;

    for (i; i < 140; i++)
    {
        gotoxy(i, j);
        printf("_");
    }
    int l = i;
    for (j = j + 1; j < 25; j++)
    {
        gotoxy(i, j);
        printf("|");
        gotoxy(k, j);
        printf("|");
    }

    for (k = k + 1; k < 140; k++)
    {
        gotoxy(k, j - 1);
        printf("_");
    }
    int x = 44, y = 7;
    gotoxy(70, 4);
    printf("ADD BOOKS IN LIBRARY");
    gotoxy(x, y);
    printf("Enter Name Of Book: ");
    fgets(t, 2, stdin);
    gets(Addbook.NameOfBook);
    // scanf("%s", &Addbook.NameOfBook);

    gotoxy(x, y + 2);
    printf("Enter Name of Author: ");
    gets(Addbook.Author);

    strcpy(Addbook.BookStatus, "Available\0");

    FILE *fp;
    fp = fopen(booksInLibraryfile, "ab+");
    if (fp == NULL)
    {
        printf("\nFile does not exist\n");
        exit(1);
    }

    fwrite(&Addbook, sizeof(struct Books_InLib), 1, fp);
    fclose(fp);

    gotoxy(x + 25, y + 5);
    printf("Book Added Successfully...");
    int u = 0;
    while (u == 0)
    {
        for (int i = 40; i < 140; i++)
        {
            gotoxy(i, y + 6);
            printf("_");
        }
        gotoxy(x + 25, y + 8);
        printf("Do You want to Add more Books [Y/n]: ");

        char key[10];
        /* gotoxy(x + 25, y + 10);
        printf("Key: "); */
        scanf("%s", key);
        if (strcmp(key, "Y\0") == 0 || strcmp(key, "y\0") == 0)
        {
            u++;
            AddBook();
        }
        else if (strcmp(key, "N\0") == 0 || strcmp(key, "n\0") == 0)
        {
            u++;
            return;
        }
        else
        {
            gotoxy(x + 25, y + 10);
            printf("Please Enter Valid Key.");
            _getch();
            for (int j = y + 8; j < y + 13; j++)
            {
                for (int i = 40; i < 140; i++)
                {
                    gotoxy(i, j);
                    printf(" ");
                }
            }
            /*   gotoxy(x + 25, y + 12);
            printf("                              ");
            gotoxy(x + 25, y + 8);
            printf("                                                ");
            gotoxy(x + 25, y + 10);
            printf("                                 "); */
        }
    }
    system("cls");
}

/* Both user and admin function */
void SearchBooks()
{
    struct Books_InLib book;
    FILE *fp;
    char t[2];
    char nameOfBook[100];
    //box
    system("cls");
    int i = 40, j = 5;
    int k = i - 1;

    for (i; i < 140; i++)
    {
        gotoxy(i, j);
        printf("_");
    }
    int l = i;
    for (j = j + 1; j < 25; j++)
    {
        gotoxy(i, j);
        printf("|");
        gotoxy(k, j);
        printf("|");
    }

    for (k = k + 1; k < 140; k++)
    {
        gotoxy(k, j - 1);
        printf("_");
    }
    int x = 44, y = 7;
    gotoxy(70, 4);
    printf("Search For Book ");
    gotoxy(x, y);
    printf("Enter Name of book : ");
    fgets(t, 2, stdin);
    gets(nameOfBook);
    // scanf("%s", &nameOfBook);

    fp = fopen(booksInLibraryfile, "rb+");
    if (fp == NULL)
    {
        printf("\nFile Does not Exist\n");
        exit(1);
    }

    int check = 0; //Used to check if book is available
    while (fread(&book, sizeof(struct Books_InLib), 1, fp))
    {
        if (strcmp(book.NameOfBook, nameOfBook) == 0)
        {
            check++;

            if (strcmp(book.BookStatus, "Available\0") == 0)
            {
                gotoxy(x + 20, y + 5);
                printf("Book is available in library.");
            }
            else
            {
                gotoxy(x + 20, y + 5);
                printf("Book has been Borrowed.");
            }
        }
    }

    if (check == 0)
    {
        gotoxy(x + 20, y + 5);
        printf("Book is not available in Library.");
    }

    fclose(fp);
    int u = 0;
    while (u == 0)
    {
        for (int i = 40; i < 140; i++)
        {
            gotoxy(i, y + 6);
            printf("_");
        }
        gotoxy(x + 25, y + 8);
        printf("Do You Want to Search More Books [Y/n]: ");
        char key[10];
        /* gotoxy(x + 25, y + 10);
        printf("Key : "); */
        scanf("%s", key);
        if (strcmp(key, "Y\0") == 0 || strcmp(key, "y\0") == 0)
        {
            u++;
            SearchBooks();
        }
        else if (strcmp(key, "N\0") == 0 || strcmp(key, "n\0") == 0)
        {
            u++;
            return;
        }
        else
        {
            gotoxy(x + 25, y + 10);
            printf("Please Enter Valid Key.");
            _getch();
            for (int j = y + 8; j < y + 13; j++)
            {
                for (int i = 40; i < 140; i++)
                {
                    gotoxy(i, j);
                    printf(" ");
                }
            } /*
            gotoxy(x + 25, y + 12);
            printf("                            ");
            gotoxy(x + 25, y + 8);
            printf("                                            ");
            gotoxy(x + 25, y + 10);
            printf("                                   "); */
        }
    }
    system("cls");
}

void SeacrchAndDisplayBooks()
{
    while (1)
    {
        system("cls");
        box(25);
        char key[10];
        gotoxy(70, 4);
        printf("Search For Book");
        int x = 49, y = 7;
        gotoxy(x, y);
        printf("[1] : Search by Name");
        gotoxy(x, y + 2);
        printf("[2] : Search Manually");
        gotoxy(x, y + 4);
        printf("[3] : Exit");
        gotoxy(x, y + 6);
        printf("Key: ");
        scanf("%s", key);
        if (strcmp(key, "1\0") == 0)
        {
            SearchBooks();
            // return;
        }
        else if (strcmp(key, "2\0") == 0)
        {
            printallbooks();
            // return;
        }
        else if (strcmp(key, "3\0") == 0)
        {
            return;
        }
        else
        {
            gotoxy(x, y + 8);
            printf("Please Enter Valid Key.");
            _getch();
        }
    }
}

void Addsecurityans(struct User U)
{
    FILE *fp, *fp1;
    struct User U1;
    fp = fopen(tempFile, "wb+");
    if (fp == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }
    fp1 = fopen(userInformationFile, "rb+");
    if (fp1 == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }
    while (fread(&U1, sizeof(struct User), 1, fp1))
    {
        if (strcmp(U1.userid, U.userid) == 0)
        {
            U1.securityQuestionIndex = U.securityQuestionIndex;
            strcpy(U1.securityQusetionAns, U.securityQusetionAns);
        }
        fwrite(&U1, sizeof(struct User), 1, fp);
    }
    fclose(fp);
    fclose(fp1);
    fp = fopen(tempFile, "rb+");
    if (fp == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }
    fp1 = fopen(userInformationFile, "wb+");
    if (fp1 == NULL)
    {
        printf("File Does Not Exist");
        exit(1);
    }
    while (fread(&U1, sizeof(struct User), 1, fp))
    {
        fwrite(&U1, sizeof(struct User), 1, fp1);
    }
    fclose(fp);
    fclose(fp1);
}
void createsecuritycodeforuser(struct User U)
{
    struct Security security;

    FILE *fp1, *fp2;
    char username[100], userid[20], securityque[100], securityAns[100];
    int securityindex;
    int maincheck = 0;

    system("cls");
    system("cls");
    box(30);
    fp1 = fopen(SecurityQuestions, "rb+");
    if (fp1 == NULL)
    {
        printf("File does not exist");
        exit(1);
    }
    int x = 49, y = 7;
    gotoxy(65, 4);
    printf("Please Select Correct Question");
    /* int count = 1; */

    while (fread(&security, sizeof(struct Security), 1, fp1))
    {
        gotoxy(x, y++);
        printf("  [%d] %s", security.Index, security.Question);
        y++;
        /*  count++; */
    }
    fclose(fp1);
    /* printf("  [%d] : None\n\n", count); */
    y++;
    int key1;
    int u = 0;
    while (u == 0)
    {
        gotoxy(x, y);
        printf("  Index: ");
        scanf("%d", &key1);

        fp1 = fopen(SecurityQuestions, "rb+");
        if (fp1 == NULL)
        {
            printf("File does not exist");
            exit(1);
        }
        int check = 0;
        while (fread(&security, sizeof(struct Security), 1, fp1))
        {
            if (key1 == security.Index)
            {
                securityindex = security.Index;
                strcpy(securityque, security.Question);
                check++;
                break;
            }
        }
        fclose(fp1);
        if (check == 1)
        {
            while (1)
            {
                system("cls");
                box(25);
                int x = 49, y = 7;
                gotoxy(x, y);
                printf("Selected Question:");
                gotoxy(x, y + 2);
                printf("%s", securityque);
                y++;
                for (int i = 45; i < 120; i++)
                {
                    gotoxy(i, y + 2);
                    printf("_");
                }
                gotoxy(x, y + 4);
                printf("[1] : Give Answer");
                gotoxy(x, y + 6);
                printf("[2] : Back");
                int key;
                gotoxy(x, y + 8);
                printf("Key: ");
                scanf("%d", &key);

                if (key == 1)
                {
                    for (int i = 45; i < 120; i++)
                    {
                        gotoxy(i, y + 9);
                        printf("_");
                    }
                    gotoxy(x, y + 11);
                    printf("Ans: ");
                    char t[2];
                    fgets(t, 2, stdin);
                    gets(securityAns);
                    strcpy(U.securityQusetionAns, securityAns);
                    U.securityQuestionIndex = securityindex;
                    /* write it in user  */
                    Addsecurityans(U);
                    return;
                }
                else if (key == 2)
                {
                    createsecuritycodeforuser(U);
                    return;
                }
                else
                {
                    gotoxy(x, y + 10);
                    printf("Please Enter Valid Key.");
                    _getch();
                    for (int j = y + 8; j < y + 11; j++)
                    {
                        for (int i = 45; i < 120; i++)
                        {
                            gotoxy(i, j);
                            printf("_");
                        }
                    }
                }
            }
        }
        else if (check == 0)
        {
            gotoxy(x, y + 2);
            printf("Please Enter Valid Key.");
            _getch();
            gotoxy(x, y + 2);
            printf("                                             ");
            gotoxy(x, y);
            printf("                                              ");
        }
    }
}
void AddNewUser()
{
    struct User U;

    FILE *fp;
    system("cls");

    //Box
    int i, j = 5;
    int x = 59, y = 7;
    for (i = 56; i < 130; i++)
    {
        gotoxy(i, j);
        printf("_");
    }
    int k = 55, l = i;
    for (j = j + 1; j < 25; j++)
    {
        gotoxy(i, j);
        printf("|");
        gotoxy(k, j);
        printf("|");
    }

    for (int t = 56; t < 130; t++)
    {
        gotoxy(t, j - 1);
        printf("_");
    }

    gotoxy(85, 4);
    printf("Add New User");
    int u = 0, u2 = 0;
    char id[20];
    struct User U2;
    while (u == 0)
    {
        fp = fopen(userInformationFile, "rb+");
        if (fp == NULL)
        {
            printf("\nError opened file\n");
            exit(1);
        }
        u2 = 0;
        gotoxy(x, y);
        printf("Create Registration No.: ");
        scanf("%s", id);
        u2 = 0;
        while (fread(&U2, sizeof(struct User), 1, fp))
        {
            if (strcmp(U2.userid, id) == 0)
            {
                u2 = 1;
                break;
            }
        }
        fclose(fp);
        if (u2 == 1)
        {
            u = 0;
            u2 = 0;

            gotoxy(x, y + 1);
            printf("Registration No. Is Not Available.");
            gotoxy(x, y + 2);
            printf("Press any key to Enter New Registration No.");
            _getch();
            gotoxy(x, y);
            printf("                                              ");
            gotoxy(x, y + 1);
            printf("                                        ");
            gotoxy(x, y + 2);
            printf("                                                 ");
        }
        else
        {
            u++;
            strcpy(U.userid, id);
        }
    }
    gotoxy(x, y + 2);
    printf("Enter First Name: ");
    scanf("%s", &U.firstname);

    gotoxy(x, y + 4);
    printf("Enter Last Name: ");
    scanf("%s", &U.lastname);
    u = 0;
    while (u == 0)
    {
        char usernme[200];
        gotoxy(x, y + 6);
        printf("Enter Username: ");
        scanf("%s", usernme);
        fp = fopen(userInformationFile, "rb+");
        if (fp == NULL)
        {
            printf("File Does Not Exist");
            exit(1);
        }
        int u2 = 0;
        while (fread(&U2, sizeof(struct User), 1, fp))
        {
            if (strcmp(U2.username, usernme) == 0)
            {
                u2 = 1;
                break;
            }
        }
        fclose(fp);
        if (u2 == 1)
        {
            u = 0;
            u2 = 0;

            gotoxy(x, y + 7);
            printf("Username Is Not Available.");
            gotoxy(x, y + 8);
            printf("Press any key to Enter New Username");
            _getch();
            gotoxy(x, y + 6);
            printf("                                                   ");
            gotoxy(x, y + 8);
            printf("                                         ");
            gotoxy(x, y + 7);
            printf("                                                 ");
        }
        else
        {
            u++;
            strcpy(U.username, usernme);
            break;
        }
    }

    // gotoxy(x, y + 6);
    // printf("Enter Username: ");
    // scanf("%s", &U.username);

    gotoxy(x, y + 8);
    printf("Enter Password: ");
    scanf("%s", &U.password);

    gotoxy(x, y + 10);
    printf("Enter Mobile No.: ");
    scanf("%s", &U.phoneno);
    gotoxy(x, y + 12);
    printf("Enter Email Address: ");
    scanf("%s", &U.email);
    U.check = 0;
    U.NoOfBookBorrowed = 0;
    strcpy(U.booksBorrowed[0].BookStatus, " \0");
    strcpy(U.booksBorrowed[0].date, " \0");
    strcpy(U.booksBorrowed[0].NameOfBook, " \0");
    strcpy(U.booksBorrowed[1].BookStatus, " \0");
    strcpy(U.booksBorrowed[1].date, " \0");
    strcpy(U.booksBorrowed[1].NameOfBook, " \0");
    strcpy(U.securityQusetionAns, "\0");
    U.securityQuestionIndex = 0;
    //  fclose(fp);
    fp = fopen(userInformationFile, "ab+");
    if (fp == NULL)
    {
        printf("\nError opened file\n");
        exit(1);
    }
    fwrite(&U, sizeof(struct User), 1, fp);
    fclose(fp);

    gotoxy(80, 23);
    printf("Press any Key to add Security Question...");
    _getch();
    createsecuritycodeforuser(U);
    gotoxy(40, 2);
    printf("Please Remember Your Security Question and Answer to change Password in case you forget it.");
    gotoxy(80, 23);
    printf("Account Created Successfully...");
    gotoxy(95, 28);
    printf("Press any key to continue...");
    _getch();
    system("cls");
}

void CreateAccount()
{

    struct User U;

    FILE *fp;
    system("cls");

    //Box
    int i, j = 5;
    int x = 59, y = 7;
    for (i = 56; i < 130; i++)
    {
        gotoxy(i, j);
        printf("_");
    }
    int k = 55, l = i;
    for (j = j + 1; j < 25; j++)
    {
        gotoxy(i, j);
        printf("|");
        gotoxy(k, j);
        printf("|");
    }

    for (int t = 56; t < 130; t++)
    {
        gotoxy(t, j - 1);
        printf("_");
    }

    gotoxy(85, 4);
    printf("Create Account");
    int u = 0, u2 = 0;
    char id[20];
    struct User U2;
    while (u == 0)
    {
        fp = fopen(userInformationFile, "rb+");
        if (fp == NULL)
        {
            printf("\nError opened file\n");
            exit(1);
        }
        u2 = 0;
        gotoxy(x, y);
        printf("Create Registration No.: ");
        scanf("%s", id);
        u2 = 0;
        while (fread(&U2, sizeof(struct User), 1, fp))
        {
            if (strcmp(U2.userid, id) == 0)
            {
                u2 = 1;
                break;
            }
        }
        fclose(fp);
        if (u2 == 1)
        {
            u = 0;
            u2 = 0;

            gotoxy(x, y + 1);
            printf("Registration No. Is Not Available.");
            gotoxy(x, y + 2);
            printf("Press any key to Enter New Registration No.");
            _getch();
            gotoxy(x, y);
            printf("                                              ");
            gotoxy(x, y + 1);
            printf("                                        ");
            gotoxy(x, y + 2);
            printf("                                                 ");
        }
        else
        {
            u++;
            strcpy(U.userid, id);
        }
    }
    gotoxy(x, y + 2);
    printf("Enter First Name: ");
    scanf("%s", &U.firstname);

    gotoxy(x, y + 4);
    printf("Enter Last Name: ");
    scanf("%s", &U.lastname);

    u = 0;
    while (u == 0)
    {
        char usernme[200];
        gotoxy(x, y + 6);
        printf("Enter Username: ");
        scanf("%s", usernme);
        fp = fopen(userInformationFile, "rb+");
        if (fp == NULL)
        {
            printf("File Does Not Exist");
            exit(1);
        }
        int u2 = 0;
        while (fread(&U2, sizeof(struct User), 1, fp))
        {
            if (strcmp(U2.username, usernme) == 0)
            {
                u2 = 1;
                break;
            }
        }
        fclose(fp);
        if (u2 == 1)
        {
            u = 0;
            u2 = 0;

            gotoxy(x, y + 7);
            printf("Username Is Not Available.");
            gotoxy(x, y + 8);
            printf("Press any key to Enter New Username");
            _getch();
            gotoxy(x, y + 6);
            printf("                                                   ");
            gotoxy(x, y + 8);
            printf("                                         ");
            gotoxy(x, y + 7);
            printf("                                                 ");
        }
        else
        {
            u++;
            strcpy(U.username, usernme);
            break;
        }
    }

    // gotoxy(x, y + 6);
    // printf("Enter Username: ");
    // scanf("%s", &U.username);

    gotoxy(x, y + 8);
    printf("Enter Password: ");
    scanf("%s", &U.password);

    gotoxy(x, y + 10);
    printf("Enter Mobile No.: ");
    scanf("%s", &U.phoneno);
    gotoxy(x, y + 12);
    printf("Enter Email Address: ");
    scanf("%s", &U.email);
    U.check = 1;
    U.NoOfBookBorrowed = 0;
    strcpy(U.booksBorrowed[0].BookStatus, " \0");
    strcpy(U.booksBorrowed[0].date, " \0");
    strcpy(U.booksBorrowed[0].NameOfBook, " \0");
    strcpy(U.booksBorrowed[1].BookStatus, " \0");
    strcpy(U.booksBorrowed[1].date, " \0");
    strcpy(U.booksBorrowed[1].NameOfBook, " \0");
    strcpy(U.securityQusetionAns, "\0");
    U.securityQuestionIndex = 0;
    //  fclose(fp);
    fp = fopen(userInformationFile, "ab+");
    if (fp == NULL)
    {
        printf("\nError opened file\n");
        exit(1);
    }
    fwrite(&U, sizeof(struct User), 1, fp);
    fclose(fp);

    gotoxy(80, 23);
    printf("Press any Key to add Security Question...");
    _getch();
    createsecuritycodeforuser(U);
    gotoxy(40, 2);
    printf("Please Remember Your Security Question and Answer to change Password in case you forget it.");
    gotoxy(80, 23);
    printf("Account Created Successfully...");
    gotoxy(95, 28);
    printf("Press any key to continue...");
    _getch();
    system("cls");
}
/* This function will be called from AdminLogin */
void AdminPanel()
{
    int check = 0;

    while (check == 0)
    {
        int x = 6;
        system("cls");
        gotoxy(50, x);
        printf("             *************ADMIN PANEL****************");

        gotoxy(70, x + 2);
        printf("[0] : Add New User");
        // x = x - 2;
        gotoxy(70, x + 4);
        printf("[1] : Add Book");
        /* Add subscription */
        gotoxy(70, x + 6);
        printf("[2] : Add Subscription");
        gotoxy(70, x + 8);
        printf("[3] : Delete User");
        gotoxy(70, x + 10);
        printf("[4] : Delete Books/Subscriptions");
        /* Delete material */
        gotoxy(70, x + 12);
        printf("[5] : Search Book");
        //  printf("[4] : information of borrowed book\n"); /* Need to code later */
        /* gotoxy(70, x + 14);
        printf("[6] : Display All Books"); */
        x = x - 2;
        gotoxy(70, x + 16);
        printf("[6] : Subscriptions");
        /* Display subscription */
        /* Display prescription */
        gotoxy(70, x + 18);
        printf("[7] : Recommendations");
        gotoxy(70, x + 20);
        printf("[8] : Details Of Particular User");

        gotoxy(70, x + 22);
        printf("[9] : Details Of All User");

        gotoxy(70, x + 24);
        printf("[10] : Feedback");

        gotoxy(70, x + 26);
        printf("[11] : Exit");
        char key[10];

        gotoxy(70, x + 28);
        printf("Key : ");
        scanf("%s", key);
        if (strcmp(key, "0\0") == 0)
        {
            AddNewUser();
        }
        else if (strcmp(key, "1\0") == 0)
        {
            AddBook();
        }
        else if (strcmp(key, "2\0") == 0)
        {
            AddNonBorrowableItems();
        }
        else if (strcmp(key, "3\0") == 0)
        {
            DeleteUser();
        }
        else if (strcmp(key, "4\0") == 0)
        {
            Deletematerials();
        }
        else if (strcmp(key, "5\0") == 0)
        {
            // SearchBooks();
            SeacrchAndDisplayBooks();
        }
        else if (strcmp(key, "6\0") == 0)
        {
            printallbooks();
        }
        else if (strcmp(key, "6\0") == 0)
        {
            DisplayNewsletter();
        }
        else if (strcmp(key, "7\0") == 0)
        {
            DisplayPrescription();
        }
        else if (strcmp(key, "8\0") == 0)
        {
            printdetailsofperticuleruser();
        }
        else if (strcmp(key, "9\0") == 0)
        {
            printdetailsofallusers();
        }
        else if (strcmp(key, "10\0") == 0)
        {
            Readfeedback();
        }
        else if (strcmp(key, "11\0") == 0)
        {
            check++;
            gotoxy(95, x + 34);
            printf("Press any key to continue...");
            _getch();
            return;
        }
        else
        {
            gotoxy(70, x + 32);
            printf("Enter Valid key.");

            _getch();
        }
        system("cls");
    }
}
/* AdminPanel will be called inside it */
void AdminLogin()
{
    char usrName[20], pass[20];
    FILE *fp;
    struct Admin U1;
    system("cls");

    int i = 45, j = 5;
    int k = i - 1;
    int x = 49, y = 7;
    for (i; i < 120; i++)
    {
        gotoxy(i, j);
        printf("_");
    }
    int l = i;
    for (j = j + 1; j < 25; j++)
    {
        gotoxy(i, j);
        printf("|");
        gotoxy(k, j);
        printf("|");
    }

    for (k = k + 1; k < 120; k++)
    {
        gotoxy(k, j - 1);
        printf("_");
    }

    gotoxy(72, 4);
    printf("LOGIN TO YOUR ACCOUNT");

    gotoxy(x, y);
    printf("Enter Username: ");
    scanf("%s", usrName);

    gotoxy(x, y + 2);
    printf("Enter Password: ");
    scanf("%s", pass);

    //captcha
    for (int t = 45; t < 120; t++)
    {
        gotoxy(t, y + 4);
        printf("_");
    }
    gotoxy(74, y + 5);
    printf("SECURITY TEST");
    for (int t = 45; t < 120; t++)
    {
        gotoxy(t, y + 6);
        printf("_");
    }

    /* Reading data from file */
    fp = fopen("DetailsOfAdmin.txt", "rb+");
    if (fp == NULL)
    {
        printf("\"File Not Found\"");
        exit(1);
    }

    /* using while loop to check username and login */

    while (fread(&U1, sizeof(struct Admin), 1, fp))
    {
        if (strcmp(usrName, U1.username) == 0 && strcmp(pass, U1.password) == 0)
        {
            int _securitycode = 1;
            int uu2 = 0;
            while (uu2 == 0)
            {
                _securitycode = captcha(x, y + 8);
                if (_securitycode == 1)
                {
                    gotoxy(59, 19);
                    printf("Incorrect Code.");

                    char ch[10];
                    int uu1 = 0;
                    do
                    {
                        /* gotoxy(x, 21);
                        printf("                                      "); */
                        gotoxy(x, 21);
                        printf("Do you want to try again [Y/n]: ");
                        scanf("%s", ch);

                        if (strcmp(ch, "y\0") == 0 || strcmp(ch, "Y\0 ") == 0)
                        {
                            gotoxy(x, 21);                                      ////////////////////////////////////
                            printf("                                        "); //////////////////////////////
                            uu1 = 1;
                        }
                        else if (strcmp(ch, "n\0") == 0 || strcmp(ch, "N\0") == 0)
                        {
                            return;
                        }
                        else
                        {

                            gotoxy(x + 10, 23);
                            printf("                                       ");
                            gotoxy(x + 10, 23);
                            printf("Please Enter Valid Key.");
                            _getch();
                            gotoxy(x + 10, 23);
                            printf("                         ");
                            gotoxy(x, 21);
                            printf("                                      ");
                        }
                    } while (uu1 == 0);
                }
                else
                {
                    uu2 = 1;
                }

                gotoxy(59, 19);
                printf("                         ");
            }
            gotoxy(59, 23);
            printf("                             ");
            if (_securitycode == 0)
            {
                gotoxy(80, 23);
                printf("  Username And Password is Correct.");
            }

            gotoxy(90, j + 5);
            printf(" Press any key to continue...");
            _getch();
            system("cls");
            gotoxy(70, 12);
            printf(" Welcome %s  ", U1.firstname);
            fclose(fp);
            _getch();
            AdminPanel();
        }
        else
        {
            int _securitycode = 1;
            int uu2 = 0;
            while (uu2 == 0)
            {
                _securitycode = captcha(x, y + 8);
                if (_securitycode == 1)
                {
                    gotoxy(59, 19);
                    printf("Incorrect Code.");

                    char ch[10];
                    int uu1 = 0;
                    do
                    {
                        /*  gotoxy(x, 21);
                        printf("                                      "); */
                        gotoxy(x, 21);
                        printf("Do you want to try again [Y/n]: ");
                        scanf("%s", ch);

                        if (strcmp(ch, "y\0") == 0 || strcmp(ch, "Y\0 ") == 0)
                        {
                            gotoxy(x, 21);
                            printf("                                      ");
                            uu1 = 1;
                        }
                        else if (strcmp(ch, "n\0") == 0 || strcmp(ch, "N\0") == 0)
                        {
                            return;
                        }
                        else
                        {

                            gotoxy(x + 10, 23);
                            printf("                                       ");
                            gotoxy(x + 10, 23);
                            printf("Please Enter Valid Key.");
                            _getch();
                            gotoxy(x + 10, 23);
                            printf("                          ");
                            gotoxy(x, 21);
                            printf("                                      ");
                        }
                    } while (uu1 == 0);
                }
                else
                {
                    uu2 = 1;
                }

                gotoxy(59, 19);
                printf("                              ");
            }
            gotoxy(59, 23);
            printf("                               ");

            gotoxy(80, 23);
            printf(" Username And Password is Incorrect.");

            gotoxy(90, j + 5);
            printf(" Press any key to continue...");
            _getch();
            fclose(fp);
            system("cls");
        }
    }
}
/* UserPanel will be called inside userlogin */
void UserPanel(struct User user)
{
    system("cls");
    printf("\n\t\t\t\t*************** USER PANEL ****************\n\n");

    if (user.check == 0)
    {
        /* change password */
        FirstTimeUserLogin(user);
    }

    int check = 0;
    while (check == 0)
    {
        struct User user1;
        FILE *fp;
        fp = fopen(userInformationFile, "rb+");
        if (fp == NULL)
        {
            printf("\nFile Does Not Exist.\n");
            exit(1);
        }
        while (fread(&user1, sizeof(struct User), 1, fp))
        {
            if (strcmp(user.userid, user1.userid) == 0 && strcmp(user.username, user1.username) == 0)
            {
                break;
            }
        }
        system("cls");
        printf("\n\n\n\n\n\n\n\n\t\t\t\t\t\t*************** USER PANEL ****************\n\n");
        // printf("\t\t\t\t\t \t\t[0] : User Profile\n\n");
        printf("\t\t\t\t\t \t\t[1] : User Profile\n\n");
        printf("\t\t\t\t\t \t\t[2] : Search Books\n\n");
        printf("\t\t\t\t\t \t\t[3] : Subscriptions\n\n");
        printf("\t\t\t\t\t \t\t[4] : Borrow Book\n\n");
        printf("\t\t\t\t\t \t\t[5] : Return Book\n\n");
        printf("\t\t\t\t\t \t\t[6] : Change Account Details\n\n");
        printf("\t\t\t\t\t \t\t[7] : Admin Details\n\n");
        printf("\t\t\t\t\t \t\t[8] : Give Feedback\n\n");
        printf("\t\t\t\t\t \t\t[9] : Recommend Books\n\n");
        printf("\t\t\t\t\t \t\t[10] : Exit\n\n");

        char key[10];
        printf("\n\n\t\t\t\t\t\t\tKey : ");
        scanf("%s", key);
        printf("\n");
        /*  if (strcmp(key, "0\0") == 0)
        {
             //may be improvise later
        }
        else  */
        if (strcmp(key, "1\0") == 0)
        {
            // SearchBooks();
            VerifyAccount(user1.username, user1.userid);
        }
        else if (strcmp(key, "2\0") == 0)
        {
            // printallbooks();
            SeacrchAndDisplayBooks();
        }
        else if (strcmp(key, "3\0") == 0)
        {
            DisplayNewsletter();
        }
        else if (strcmp(key, "4\0") == 0)
        {
            // BorrowBook(user1);
            beforeborrowingbook(user1);
        }
        else if (strcmp(key, "5\0") == 0)
        {
            Returnbook(user1);
        }
        else if (strcmp(key, "6\0") == 0)
        {
            ChangeAccountDetails(user1);
        }
        else if (strcmp(key, "7\0") == 0)
        {
            printDetailsofAdmin();
        }
        else if (strcmp(key, "8\0") == 0)
        {
            feedback();
        }
        else if (strcmp(key, "9\0") == 0)
        {
            Prescribebooks();
        }
        else if (strcmp(key, "10\0") == 0)
        {
            check++; //exit
        }
        else
        {
            printf("Enter Valid key\n");
            printf("\t\t\t\tPress any key to continue...\n");
            _getch();
        }
    }
}

/* Userpanel will be called inside it */
void UserLogin()
{
    char usrName[20], pass[20];
    FILE *fp;
    struct User U1;
    system("cls");

    int i = 45, j = 5;
    int k = i - 1;
    int x = 49, y = 7;
    for (i; i < 120; i++)
    {
        gotoxy(i, j);
        printf("_");
    }
    int l = i;
    for (j = j + 1; j < 25; j++)
    {
        gotoxy(i, j);
        printf("|");
        gotoxy(k, j);
        printf("|");
    }

    for (k = k + 1; k < 120; k++)
    {
        gotoxy(k, j - 1);
        printf("_");
    }

    gotoxy(72, 4);
    printf("LOGIN TO YOUR ACCOUNT");

    gotoxy(x, y);
    printf("Enter Username: ");
    scanf("%s", usrName);

    gotoxy(x, y + 2);
    printf("Enter Password: ");
    scanf("%s", pass);

    //captcha
    for (int t = 45; t < 120; t++)
    {
        gotoxy(t, y + 4);
        printf("_");
    }
    gotoxy(74, y + 5);
    printf("SECURITY TEST");
    for (int t = 45; t < 120; t++)
    {
        gotoxy(t, y + 6);
        printf("_");
    }

    /* Reading data from file */
    fp = fopen(userInformationFile, "rb+");
    if (fp == NULL)
    {
        printf("\"File Not Found\"");
        exit(1);
    }
    int check1 = 0;
    /* Using while loop to check username and login */

    while (fread(&U1, sizeof(struct User), 1, fp))
    {
        if (strcmp(usrName, U1.username) == 0 && strcmp(pass, U1.password) == 0)
        {
            int _securitycode = 1;
            int uu2 = 0;
            while (uu2 == 0)
            {
                _securitycode = captcha(x, y + 8);
                if (_securitycode == 1)
                {
                    gotoxy(59, 19);
                    printf("Incorrect Code.");

                    char ch[10];
                    int uu1 = 0;
                    do
                    {
                        /*  gotoxy(x, 21);
                        printf("                                      "); */
                        gotoxy(x, 21);
                        printf("Do you want to try again [Y/n]: ");
                        scanf("%s", ch);

                        if (strcmp(ch, "y\0") == 0 || strcmp(ch, "Y\0 ") == 0)
                        {
                            gotoxy(x, 21);
                            printf("                                          ");
                            uu1 = 1;
                        }
                        else if (strcmp(ch, "n\0") == 0 || strcmp(ch, "N\0") == 0)
                        {
                            return;
                        }
                        else
                        {

                            gotoxy(x + 10, 23);
                            printf("                                       ");
                            gotoxy(x + 10, 23);
                            printf("Please Enter Valid Key.");
                            _getch();
                            gotoxy(x + 10, 23);
                            printf("                            ");
                            gotoxy(x, 21);
                            printf("                                      ");
                        }
                    } while (uu1 == 0);
                }
                else
                {
                    uu2 = 1;
                }

                gotoxy(59, 19);
                printf("                         ");
            }
            gotoxy(59, 23);
            printf("                              ");
            if (_securitycode == 0)
            {
                gotoxy(80, 23);
                printf("  Username And Password is Correct.\n");
                check1 = 1;
            }

            gotoxy(90, j + 5);
            printf(" Press any key to continue...");
            _getch();
            system("cls");
            gotoxy(70, 12);
            printf(" Welcome %s %s", U1.firstname, U1.lastname);
            fclose(fp);
            _getch();
            UserPanel(U1);
        }
    }
    if (check1 == 0)
    {
        int _securitycode = 1;
        int uu2 = 0;
        while (uu2 == 0)
        {
            _securitycode = captcha(x, y + 8);
            if (_securitycode == 1)
            {
                gotoxy(59, 19);
                printf("Incorrect Code.");

                char ch[10];
                int uu1 = 0;
                do
                {
                    /*  gotoxy(x, 21);
                    printf("                                      "); */
                    gotoxy(x, 21);
                    printf("Do you want to try again [Y/n]: ");
                    scanf("%s", ch);

                    if (strcmp(ch, "y\0") == 0 || strcmp(ch, "Y\0 ") == 0)
                    {
                        gotoxy(x, 21);
                        printf("                                           ");
                        uu1 = 1;
                    }
                    else if (strcmp(ch, "n\0") == 0 || strcmp(ch, "N\0") == 0)
                    {
                        return;
                    }
                    else
                    {

                        gotoxy(x + 10, 23);
                        printf("                                       ");
                        gotoxy(x + 10, 23);
                        printf("Please Enter Valid Key.");
                        _getch();
                        gotoxy(x + 10, 23);
                        printf("                            ");
                        gotoxy(x, 21);
                        printf("                                      ");
                    }
                } while (uu1 == 0);
            }
            else
            {
                uu2 = 1;
            }

            gotoxy(59, 19);
            printf("                         ");
        }
        gotoxy(59, 23);
        printf("                             ");

        gotoxy(80, 23);
        printf(" Username And Password is Incorrect.");

        gotoxy(90, j + 5);
        printf(" Press any key to continue...");
        _getch();
        fclose(fp);
        system("cls");
    }
}

void Changingforgotpassword(struct User user)
{

    FILE *fp1;
    struct User user1;
    system("cls"); //Clear screan
    box(22);
    gotoxy(60, 4);
    printf("Please Create New Password ");

    fp1 = fopen(userInformationFile, "rb+");
    /*This type of if statement is used to exit from program if file does not exist.
    This type of statement will be used multiple time throught the program.
    */
    if (fp1 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    FILE *fp2;
    fp2 = fopen(tempFile, "wb+");
    if (fp2 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    /* All these steps of changing information is mentioned on starting of code */

    /* fread function is used to read struct information from file and copy it to mentioned struct */
    /* This while loop will read every struct from file one by one  */
    while (fread(&user1, sizeof(struct User), 1, fp1))
    {
        /* This if statement will match username and ID if it matches it will perform statements accordingly */
        if (strcmp(user.username, user1.username) == 0 && strcmp(user.userid, user1.userid) == 0)
        {
            gotoxy(49, 7);
            printf("Enter new password : ");
            char p[20];
            scanf("%s", p);
            strcpy(user1.password, p);
            user1.check = 1; //it will insure that user has changed his password for the first time
        }
        /* fwrite function is used to write struct in file */
        fwrite(&user1, sizeof(struct User), 1, fp2);
    }

    fclose(fp1);
    fclose(fp2);

    /* Now we will copy information from temporary file to original one */
    FILE *fp3, *fp4;
    struct User U;

    fp3 = fopen(userInformationFile, "wb+");
    if (fp3 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    fp4 = fopen(tempFile, "rb+");
    if (fp4 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    while (fread(&U, sizeof(struct User), 1, fp4))
    {
        fwrite(&U, sizeof(struct User), 1, fp3);
    }

    fclose(fp3);
    fclose(fp4);

    gotoxy(70, 16);
    printf("Password Changed Successfully...");
    gotoxy(85, 23); //Fixing coordinates of print statement of next line
    printf("Press any key to continue...");
    _getch();
    system("cls");
}

void ForgotPassword()
{
    struct Security security;
    struct User U;
    FILE *fp1, *fp2;
    char username[100], userid[20];
    char securityque[100];
    int securityindex;
    int maincheck = 0;
    char securityAns[100];

    system("cls");
    box(25);
    gotoxy(65, 4);
    printf("Change Password");
    int x = 49, y = 7;
    gotoxy(x, y);
    printf("Enter Username: ");
    scanf("%s", username);

    gotoxy(x, y + 2);
    printf("Enter Registration No.: "); /////////////////////////////////
    scanf("%s", userid);
    for (int i = 45; i < 120; i++)
    {
        gotoxy(i, y + 3);
        printf("_");
    }

    char key[10];
    int um = 0;
    while (um == 0)
    {
        gotoxy(x, y + 5);
        printf("[1] : Verify Security Question");
        gotoxy(x, y + 7);
        printf("[2] : Exit");
        gotoxy(x, y + 9);
        printf("Key: ");
        scanf("%s", key);

        if (strcmp(key, "1\0") == 0)
        {
            system("cls");
            fp1 = fopen(SecurityQuestions, "rb+");
            if (fp1 == NULL)
            {
                printf("File does not exist");
                exit(1);
            }
            box(30);
            gotoxy(60, 4);
            printf("Please Select Correct Security Question");

            int x1 = 49, y1 = 7;
            int count = 1;
            while (fread(&security, sizeof(struct Security), 1, fp1))
            {
                gotoxy(x1, y1);
                printf("[%d] : %s", security.Index, security.Question);
                y1 += 2;
                count++;
            }
            fclose(fp1);
            gotoxy(x1, y1);
            printf("[%d] : None", count);

            int key1;
            int u = 0;
            while (u == 0)
            {
                gotoxy(x1, y1 + 2);
                printf("Index: ");
                scanf("%d", &key1);

                fp1 = fopen(SecurityQuestions, "rb+");
                if (fp1 == NULL)
                {
                    printf("File does not exist");
                    exit(1);
                }
                int check = 0;
                while (fread(&security, sizeof(struct Security), 1, fp1))
                {
                    if (key1 == security.Index)
                    {
                        securityindex = security.Index;
                        strcpy(securityque, security.Question);
                        check++;
                        break;
                    }
                }
                fclose(fp1);
                if (check == 0)
                {
                    if (key1 == count)
                    {
                        system("cls");
                        /* Print Remaining */
                        box(25);
                        gotoxy(65, 4);
                        printf("Change Password");
                        int x = 49, y = 7;
                        gotoxy(x, y);
                        printf("Enter Username: %s", username);
                        /* scanf("%s", username); */

                        gotoxy(x, y + 2);
                        printf("Enter Registration No.: %s", userid);
                        /* scanf("%s", userid); */
                        for (int i = 45; i < 120; i++)
                        {
                            gotoxy(i, y + 3);
                            printf("_");
                        }
                        u++;
                        break;
                    }
                    else
                    {
                        gotoxy(x1, y1 + 4);
                        printf("Please Enter Valid Key.");
                        _getch();
                        gotoxy(x1, y1 + 2);
                        printf("                            ");
                        gotoxy(x1, y1 + 4);
                        printf("                             ");
                    }
                }
                else /* Correct key */
                {
                    maincheck = 1;
                    system("cls");
                    /* print remaining */
                    system("cls");
                    /* Print Remaining */
                    box(27);
                    gotoxy(65, 4);
                    printf("Change Password");
                    int x = 49, y = 7;
                    gotoxy(x, y);
                    printf("Enter Username: %s", username);
                    /* scanf("%s", username); */

                    gotoxy(x, y + 2);
                    printf("Enter Registration No.: %s", userid);
                    /* scanf("%s", userid); */
                    for (int i = 45; i < 120; i++)
                    {
                        gotoxy(i, y + 3);
                        printf("_");
                    }
                    u++;
                    um++;
                    break;
                }
            } //while
        }
        else if (strcmp(key, "2\0") == 0)
        {
            return;
        }
        else
        {
            /* For invalid key */
            gotoxy(x, y + 11);
            printf("Please Enter Valid key.");
            _getch();
            for (int j = y + 9; j < y + 12; j++)
            {
                for (int i = 45; i < 120; i++)
                {
                    gotoxy(i, j);
                    printf(" ");
                }
            }
        }
    } //main while
    if (maincheck == 1)
    {
        struct User U1;
        char t[2];
        gotoxy(x, y + 4);
        printf("Security Question:");

        gotoxy(x, y + 6);
        printf("%s", securityque);
        for (int i = 45; i < 120; i++)
        {
            gotoxy(i, y + 7);
            printf("_");
        }
        y += 2;
        gotoxy(x, y + 7);
        printf("Ans: ");
        fgets(t, 2, stdin);
        gets(securityAns);

        fp2 = fopen(userInformationFile, "rb+");
        if (fp2 == NULL)
        {
            printf("File Does Not Exist");
            exit(1);
        }
        int correctcheck = 0;
        while (fread(&U, sizeof(struct User), 1, fp2))
        {
            if (strcmp(U.userid, userid) == 0 && strcmp(U.username, username) == 0 && strcmp(U.securityQusetionAns, securityAns) == 0 && U.securityQuestionIndex == securityindex)
            {
                correctcheck = 1;

                /* Copying All Information */
                strcpy(U1.booksBorrowed[0].Author, U.booksBorrowed[0].Author);
                strcpy(U1.booksBorrowed[0].BookStatus, U.booksBorrowed[0].BookStatus);
                strcpy(U1.booksBorrowed[0].date, U.booksBorrowed[0].date);
                strcpy(U1.booksBorrowed[0].NameOfBook, U.booksBorrowed[0].NameOfBook);

                strcpy(U1.booksBorrowed[1].Author, U.booksBorrowed[1].Author);
                strcpy(U1.booksBorrowed[1].BookStatus, U.booksBorrowed[1].BookStatus);
                strcpy(U1.booksBorrowed[1].date, U.booksBorrowed[1].date);
                strcpy(U1.booksBorrowed[1].NameOfBook, U.booksBorrowed[1].NameOfBook);

                strcpy(U1.email, U.email);
                U1.check = U.check;
                strcpy(U1.firstname, U.firstname);
                strcpy(U1.lastname, U.lastname);
                strcpy(U1.password, U.password);
                U1.NoOfBookBorrowed = U.NoOfBookBorrowed;
                strcpy(U1.phoneno, U.phoneno);
                strcpy(U1.securityQusetionAns, U.securityQusetionAns);
                strcpy(U1.userid, U.userid);
                strcpy(U1.username, U.username);
                U1.securityQuestionIndex = U.securityQuestionIndex;
                break;
            }
        }
        fclose(fp2);
        if (correctcheck == 1)
        {
            /* Successfull */
            gotoxy(65, y + 11);
            printf("Correct Input...");
            gotoxy(75, y + 15);
            printf("Press any key to Change Password...");
            _getch();
            /* Change first user login change password */
            Changingforgotpassword(U1);
        }
        else
        {
            char key22[10];
            /* wnat to give ans again */
            gotoxy(50, y + 10);
            printf("Incorrect Input. Unable to Proceed.");
            for (int i = 45; i < 120; i++)
            {
                gotoxy(i, y + 11);
                printf("_");
            }
            while (1)
            {

                gotoxy(50, y + 13);

                printf("Do You Want to Try Again [Y/n]: ");
                scanf("%s", key22);
                if (strcmp(key22, "y\0") == 0 || strcmp(key22, "Y\0") == 0)
                {
                    ForgotPassword();
                    break;
                }
                else if (strcmp(key22, "n\0") == 0 || strcmp(key22, "N\0") == 0)
                {
                    gotoxy(80, 29);
                    printf("Press any key to continue...");
                    _getch();
                    system("cls");
                    return;
                }
                else
                {
                    gotoxy(50, y + 15);
                    printf("Please Enter Valid Key");
                    _getch();
                    for (int j = y + 13; j < y + 16; j++)
                    {
                        for (int i = 45; i < 120; i++)
                        {
                            gotoxy(i, j);
                            printf(" ");
                        }
                    }
                }
            }
        }
    }
}
void beforelogin()
{
    while (1)
    {
        system("cls");
        box(25);
        int x = 49, y = 7;
        gotoxy(70, y - 3);
        printf("Sign In/Sign Up");
        gotoxy(x, y);
        printf("[1] : Sign in");
        gotoxy(x, y + 2);
        printf("[2] : Create Account");
        y += 2;
        gotoxy(x, y + 2);
        printf("[3] : Forgot Password");
        gotoxy(x, y + 4);
        printf("[4] : Back");
        gotoxy(x, y + 6);
        printf("Key: ");
        char key[10];
        scanf("%s", key);
        if (strcmp(key, "1\0") == 0)
        {
            /* Create account page */
            UserLogin();
            // return;
        }
        else if (strcmp(key, "2\0") == 0)
        {
            // AddNewUser();
            CreateAccount();
        }
        else if (strcmp(key, "3\0") == 0)
        {
            ForgotPassword();
        }
        else if (strcmp(key, "4\0") == 0)
        {
            return;
        }
        else
        {
            gotoxy(x, y + 8);
            printf("Please Enter Valid Key.");
            _getch();
            system("cls");
        }
    }
}

void GuestPanel()
{
    while (1)
    {
        system("cls");
        gotoxy(50, 10);
        printf("             *************GUEST PANEL****************");

        gotoxy(70, 12);
        printf("[0] : Search Books");
        gotoxy(70, 14);
        printf("[1] : Subscriptions");
        gotoxy(70, 16);
        printf("[2] : Give Feedback");
        gotoxy(70, 18);
        printf("[3] : Recommend Books");
        gotoxy(70, 20);
        printf("[4] : Exit");

        char key[10];
        gotoxy(70, 22);
        printf("Key: ");
        scanf("%s", key);
        if (strcmp(key, "0\0") == 0)
        {
            // printallbooks();
            SeacrchAndDisplayBooks();
        }
        else if (strcmp(key, "1\0") == 0)
        {
            DisplayNewsletter();
        }
        else if (strcmp(key, "2\0") == 0)
        {
            feedback();
        }
        else if (strcmp(key, "3\0") == 0)
        {
            Prescribebooks();
        }
        else if (strcmp(key, "4\0") == 0)
        {
            return;
        }
        else
        {
            printf("Please Enter Valid key");
        }
    }
}

int main()
{

    //This commented lines of code is used to Fill Details of Admin since we do not need it again that's why we comment these lines.

    /*   struct Admin U;

    FILE *fp;
    system("cls");
    gotoxy(57, 3);
    printf("Create Account\n\n");
    printf("Enter First Name: ");
    scanf("%s", &U.firstname);
    // printf("\nEnter Last Name: ");
    // scanf("%s", &U.lastname);
    strcpy(U.lastname,"\0");
    printf("\nEnter Username: ");
    scanf("%s", &U.username);
    printf("\nEnter Password: ");
    scanf("%s", &U.password);
    printf("\nEnter Phone No.: ");
    scanf("%s",&U.phoneno);
    printf("\n Enter email:");
    scanf("%s",&U.email);
    fp = fopen("DetailsOfAdmin.txt", "wb+");
    if (fp == NULL)
    {
        printf("\nError opened file\n");
        exit(1);
    }
    fwrite(&U, sizeof(struct Admin), 1, fp);
    fclose(fp);
    system("cls");
    gotoxy(55, 20);
    printf("Account Created SuccessfullY.");
    gotoxy(75, 25);
    printf("Press any key to continue....");
    _getch();
    system("cls"); */
    system("cls");
    // addque();
    /*  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t*****            *****     **********      ***********          *****         ***********      ***         ***\n");
    printf("\t\t\t\t*****            *****     **********      ************        **   **        ************     ***         ***\n");
    printf("\t\t\t\t*****            *****     ****    ***     ****      ***      **     **       ****      ***    ***         ***\n");
    printf("\t\t\t\t*****            *****     ****    ***     ****    ****      **       **      ****    ****     ***         ***\n");
    printf("\t\t\t\t*****            *****     **********      **** ***          ************     **** ***          ***       *** \n");
    printf("\t\t\t\t*****            *****     **********      ****   ****       ************     ****   ****         *** * ***   \n");
    printf("\t\t\t\t*****            *****     ****    ***     ****    ****      ***      ***     ****    ****           ****      \n");
    printf("\t\t\t\t************     *****     ****    ***     ****     ***      ***      ***     ****     ***           ****      \n");
    printf("\t\t\t\t************     *****     **********      ****     ***      ***      ***     ****     ***           ****      \n");
    printf("\t\t\t\t************     *****     **********      ****     ***      ***      ***     ****     ***           ****      \n"); */
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t*       *     ******   *           ****     *****      *       *   ******       **********      *****                       \n");
    printf("\t\t\t\t*       *     *        *         *         *     *     * *   * *   *                 *         *     *                    \n");
    printf("\t\t\t\t*       *     *        *        *         *       *    *  * *  *   *                 *        *       *                   \n");
    printf("\t\t\t\t*       *     *        *        *         *       *    *   *   *   *                 *        *       *                     \n");
    printf("\t\t\t\t*   *   *     ******   *        *         *       *    *       *   ******            *        *       *                      \n");
    printf("\t\t\t\t*  * *  *     *        *        *         *       *    *       *   *                 *        *       *                      \n");
    printf("\t\t\t\t* *   * *     *        *         *         *     *     *       *   *                 *         *     *                       \n");
    printf("\t\t\t\t*       *     ******   ******      ****     *****      *       *   ******            *          *****                        \n\n\n\n");
    printf("");
    printf("     \t\t\t\t\t    *         *     ******     ******         ***        ******     *         *                           \n");
    printf("     \t\t\t\t\t    *         *     *     *    *     *       *   *       *     *     *       *           \n");
    printf("     \t\t\t\t\t    *         *     *     *    *     *      *     *      *     *      *     *         \n");
    printf("     \t\t\t\t\t    *         *     ******     *****       * * * * *     *****          * *               \n");
    printf("     \t\t\t\t\t    *         *     *     *    *    *      *       *     *    *          *           \n");
    printf("     \t\t\t\t\t    *         *     *     *    *     *     *       *     *     *         *         \n");
    printf("     \t\t\t\t\t    *******   *     ******     *     *     *       *     *     *         *                 \n");
    printf("                                                                          \n");
    printf("");
    printf("");

    _getch();

    while (1)
    {
        system("cls");
        gotoxy(50, 10);
        printf("            *************MAIN MENU*************");
        gotoxy(70, 12);
        printf("[0] : Admin");
        gotoxy(70, 14);
        printf("[1] : User");
        gotoxy(70, 16);
        printf("[2] : Guest");
        gotoxy(70, 18);
        printf("[3] : Exit");
        char key[10];
        gotoxy(70, 20);
        printf("Key: ");
        scanf("%s", key);
        if (strcmp(key, "0\0") == 0)
        {
            /* Admin */
            AdminLogin();
        }
        else if (strcmp(key, "1\0") == 0)
        {
            /* User */
            // UserLogin();
            beforelogin();
        }
        else if (strcmp(key, "2\0") == 0)
        {
            GuestPanel();
        }
        else if (strcmp(key, "3\0") == 0)
        {
            return 0; //exit
        }
        else
        {
            gotoxy(75, 22);
            printf("Please Enter Valid Key");
            _getch();
        }
    }
    return 0;
}
