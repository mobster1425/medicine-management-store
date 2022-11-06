
#include <stdio.h>
#include <string.h>
#include <time.h>
#include<stdlib.h>
#include<ctype.h>

#define MAX_YR 1999
#define MIN_YR 1900
#define MAX_SIZE_USER_NAME 30
#define MAX_SIZE_PASSWORD  20

#define FILE_NAME "medicinestoremanagement.bin"
#define FILE_HEADER_SIZE  sizeof(sFileHeader)

/* related to medicine info*/
#define MAX_MEDICINE_NAME 50
#define MAX_DATE_EXPIRY 50
#define MAX_USE_OF_MEDICINE 300

/* we are creating a STRUCTURE TO STORE DATE and we would call the structure in
 * another structure*/

typedef struct {
  int yyyy;
  int mm;
  int dd;
} Date;

typedef struct
{
    char username[MAX_SIZE_USER_NAME];
    char password[MAX_SIZE_PASSWORD];
} sFileHeader;

/*to store medicine info*/

typedef struct {
  unsigned int medicine_id;
  char medicinename[MAX_MEDICINE_NAME];
  char medicineuse[MAX_USE_OF_MEDICINE];
  Date expirydate;
} medicineinfo;

void printmessagecenter(const char *message) {
  int len = 0;
  int pos = 0;

  /* to calculate how many space needed to print*/

  len = (78 - strlen(message)) / 2;
  printf("\t\t\t");
  for (pos = 0; pos < len; pos++) {
    /* TO print space*/
    printf(" ");
  }
  /* print message*/
  printf("%s", message);
}

/* here we create function that prints the head meesage given and can be called
 * anywhere in the program*/

void headmessage(const char *message) {
  system("cls");
  printf("\t\t\t##########################################################");
  printf("\n\t\t\t##### MEDICINE STORE MANAGEMENT ########################");
  printf("\n\t\t###### BY ADIGUN FEHINTOLA AND OPU DEBNAT################");
  printf("\n\t\t\t####                                    ################");
  printf("\n\t\t\t########################################################");

  printmessagecenter(message);
  printf("\n\t\t_________________________________");
}

void welcomemessage() {
  headmessage("made by feyin and opu debnat");
  printf("\n\n\n\n");

  printf("\n\t\t\t*******************************************************");
  printf("\n\t\t\t=========================================================");
  printf("\n\t\t\t==== WELCOME TO OUR MEDICAL STORE MANAGEMENT SYSTEM=======");
  printf("\n\t\t\t==========================================================");
  printf("\n\n\n\t\t\t Enter any Key to Advance");
  getchar();
}

/* so we are creating a function to check if given medicine  name or other user
 * input name is valid i mean correct that it is an alphaber*/
int isNameValid(const char *name)
{
    int validName = 1;
    int len = 0;
    int index = 0;
    len = strlen(name);
    for(index =0; index <len ; ++index)
    {
        if(!(isalpha(name[index])) && (name[index] != '\n') && (name[index] != ' '))
        {
            validName = 0;
            break;
        }
    }
    return validName;
}


// function to check leap year
// function returns 1 if leap year
int  IsLeapYear(int year)
{
    return (((year % 4 == 0) &&
             (year % 100 != 0)) ||
            (year % 400 == 0));
}
// returns 1 if given date is valid.
int isvaliddate(Date *validDate)
{
    //check range of year,month and day
    if (validDate->yyyy > MAX_YR ||
            validDate->yyyy < MIN_YR)
        return 0;
    if (validDate->mm < 1 || validDate->mm > 12)
        return 0;
    if (validDate->dd < 1 || validDate->dd > 31)
        return 0;
    //Handle feb days in leap year
    if (validDate->mm == 2)
    {
        if (IsLeapYear(validDate->yyyy))
            return (validDate->dd <= 29);
        else
            return (validDate->dd <= 28);
    }
    //handle months which has only 30 days
    if (validDate->mm == 4 || validDate->mm == 6 ||
            validDate->mm == 9 || validDate->mm == 11)
        return (validDate->dd <= 30);
    return 1;
}

// Add medicine in database,we open in append mode

void addmedicineintodatabase()
{
  int days;
  medicineinfo addmedicineinfoindatabase = {0};
  FILE *fp = NULL;
  int status = 0;
  fp = fopen(FILE_NAME, "ab+");
  if (fp == NULL) {
    printf("file is not opened\n");
    exit(1);
  }
  headmessage("ADD NEW MEDICINE");

  printf("\n\n\t\t Enter Your Details Below:- ");
  printf("\n\t\t\t-------------------------------\n");
  printf("\n\t\t\t Medicine_id = ");
  fflush(stdin);
  scanf("%u", &addmedicineinfoindatabase.medicine_id);

  do {
    printf("\n\t\t\t medicine name= ");
    fflush(stdin);
    fgets(addmedicineinfoindatabase.medicinename, MAX_MEDICINE_NAME, stdin);

    status = isNameValid(addmedicineinfoindatabase.medicinename);
    if (!status) {
      printf("\n\t\t\t name contains invalid character, please enter again.");
    }

  } while (!status);



  do {
    // use of medicine from user
    printf("\n\t\t\t enter the use of this medicine :- ");
    fflush(stdin);
    fgets(addmedicineinfoindatabase.medicineuse, MAX_USE_OF_MEDICINE, stdin);

    status = isNameValid(addmedicineinfoindatabase.medicineuse);
    if (!status) {
      printf("\n\t\t\t contains invalid character enter again");
    }
  }
   while (!status);
do
    {
        //get date year,month and day from user
        printf("\n\t\t\tEnter date in format (day/month/year): ");
        scanf("%d/%d/%d",&addmedicineinfoindatabase.expirydate.dd,&addmedicineinfoindatabase.expirydate.mm,&addmedicineinfoindatabase.expirydate.yyyy);
        //check date validity
        status = isvaliddate(&addmedicineinfoindatabase.expirydate);
        if (!status)
        {
            printf("\n\t\t\tPlease enter a valid date.\n");
        }
    }
    while(!status);
  fwrite(&addmedicineinfoindatabase, sizeof(addmedicineinfoindatabase), 1, fp);
  fclose(fp);
}

// search books
void searchmedicine()
{
  int found = 0;
  char medicinename[MAX_MEDICINE_NAME] = {0};
  medicineinfo addmedicineinfoindatabase = {0};
  FILE *fp = NULL;
  int status = 0;
  fp = fopen(FILE_NAME, "rb");

  if (fp == NULL) {
    printf("\n\t\t\t file not opened");
    exit(1);
  }
  headmessage("search medicine");
  // use fseek control to a specific position
  if (fseek(fp,FILE_HEADER_SIZE, SEEK_SET) != 0) {
    fclose(fp);
    printf("\n\t\t\t facing issues while reading file \n");
    exit(1);
  }
  printf("\n\t\t\t Enter medicine name to search :- ");
  fflush(stdin);
  fgets(medicinename, MAX_MEDICINE_NAME, stdin);

  while (fread(&addmedicineinfoindatabase, sizeof(&addmedicineinfoindatabase), 1,
               fp)) {
    if (!strcmp(addmedicineinfoindatabase.medicinename, medicinename)) {
      found = 1;
      break;
    }
  }
  if (found) {
    printf("\n\t\t\tmedicine id=%u\n", addmedicineinfoindatabase.medicine_id);
    printf("\t\t\t medicine name=%s", addmedicineinfoindatabase.medicinename);
    printf("\t\t\t use of medicine=%s", addmedicineinfoindatabase.medicineuse);
    printf("\t\t\t expirydate(day/month/year)= (%d%d%d)",
           addmedicineinfoindatabase.expirydate.dd,
           addmedicineinfoindatabase.expirydate.mm,
           addmedicineinfoindatabase.expirydate.yyyy);
  } else {
        printf("\n\t\t\t No record");
  }
  fclose(fp);
  printf("\n\n\n\t\t\t press any key to go to main menu.............");
  getchar();
}

// view medicine function
void viewmedicine()
{
  int found = 0;
  char medicinename[MAX_MEDICINE_NAME] = {0};
  medicineinfo addmedicineinfoindatabase = {0};
  FILE *fp = NULL;
  int status = 0;
  unsigned int countmedicine = 1;

  headmessage("VIEW MEDICINE SECTION");
  fp = fopen(FILE_NAME, "rb");
  if (fp == NULL) {
    printf("FILE IS NOT OPENED\n");
    exit(1);
  }
  if (fseek(fp,FILE_HEADER_SIZE, SEEK_SET) != 0) {
    fclose(fp);
    printf("FACING SERIOUS ISSUES WHILE READING\n");
    exit(1);
  }
  while (
      fread(&addmedicineinfoindatabase, sizeof(addmedicineinfoindatabase), 1, fp)) {
    printf("\n\t\t\t count medicine=%d\n\n", countmedicine);
    printf("\t\t\t medicine id=%u", addmedicineinfoindatabase.medicine_id);
    printf("\n\t\t\t medicine name=%s", addmedicineinfoindatabase.medicinename);
    printf("\n\t\t\t use of medicine=%s", addmedicineinfoindatabase.medicineuse);
    printf("\t\t\t expiry date(day/month/year)=(%d%d%d)\n\n",
           addmedicineinfoindatabase.expirydate.dd,
           addmedicineinfoindatabase.expirydate.mm,
           addmedicineinfoindatabase.expirydate.yyyy);

    found = 1;
    ++countmedicine;
  }
  fclose(fp);

  if (!found) {
    printf("\n\t\t\t NO RECORD");
  }
  printf("\n\n\t\t\t press any key to go to main menu ");
  fflush(stdin);
  getchar();
}

// delete function
void deletemedicine()
 {
  int found = 0;
  int medicinedelete = 0;
  sFileHeader fileHeaderInfo = {0};
  char medicinename[MAX_MEDICINE_NAME] = {0};
  medicineinfo addmedicineinfoindatabase = {0};
  FILE *fp = NULL;
  FILE *tmpFp = NULL;
  int status = 0;

  headmessage("DELETE MEDICINE/n");
  fp = fopen(FILE_NAME, "rb");
  if (fp == NULL) {
    printf("File is not opened");
    exit(1);
  }
  tmpFp = fopen("tmp.bin", "wb");
  if (tmpFp == NULL)
    {
    fclose(fp);
    printf("FILE IS NOT OPENED\n");
    exit(1);
  }
  fread(&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
  fwrite(&fileHeaderInfo,FILE_HEADER_SIZE, 1, tmpFp);

  printf("\n\t\t\t Enter medcine id to delete: ");
  scanf("%d", &medicinedelete);

  while (
      fread(&addmedicineinfoindatabase, sizeof(addmedicineinfoindatabase), 1, fp))
       {
    if (addmedicineinfoindatabase.medicine_id != medicinedelete)
        {
      fwrite(&addmedicineinfoindatabase, sizeof(addmedicineinfoindatabase), 1,
             tmpFp);
    }
    else{
      found = 1;
  }

  if (found) {
    printf("\n\t\t\t CONGRATS BUDDY ! RECORD DELETED SUCESSFULLY");
  }
   else {
    printf("\n\t\t\t SORRY HAHAHA RECORD NOT FOUND!");
  }
  fclose;
  (fp);
  fclose(tmpFp);
  remove(FILE_NAME);
  rename("tmp.bin", FILE_NAME);
}
    }

    void updateCredential(void)
{
    sFileHeader fileHeaderInfo = {0};
    FILE *fp = NULL;
    unsigned char username[MAX_SIZE_USER_NAME] = {0};
    unsigned char password[MAX_SIZE_PASSWORD] = {0};
    headmessage("Update Credential");
    fp = fopen(FILE_NAME,"rb+");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    fread (&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
    if (fseek(fp,0,SEEK_SET) != 0)
    {
        fclose(fp);
        printf("\n\t\t\tFacing issue while updating password\n");
        exit(1);
    }
    printf("\n\n\t\t\tNew Username:");
    fflush(stdin);
    fgets(username,MAX_SIZE_USER_NAME,stdin);
    printf("\n\n\t\t\tNew Password:");
    fflush(stdin);
    fgets(password,MAX_SIZE_PASSWORD,stdin);
    strncpy(fileHeaderInfo.username,username,sizeof(username));
    strncpy(fileHeaderInfo.password,password,sizeof(password));
    fwrite(&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
    fclose(fp);
    printf("\n\t\t\tYour Password has been changed successfully");
    printf("\n\t\t\ttLogin Again:");
    fflush(stdin);
    getchar();
    exit(1);
}
// main menu using switch
void menu()
 {


  int choice = 0;
  do {
    headmessage("MAIN MENU");
    printf("\n\n\n\t\t\t 1. ADD MEDICINE: ");
    printf("\n\n\n\t\t\t 2. SEARCH MEDICINE: ");
    printf("\n\n\n\t\t\t 3. VIEW MEDICINE: ");
    printf("\n\n\n\t\t\t 4. DELETE MEDICINE: ");
    printf("\n\n\n\t\t\t 0. EXIT ");
    printf("\n\n\n\t\t\t ENTER CHOICE = ");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      addmedicineintodatabase();
      break;
    case 2:
      searchmedicine();
      break;
    case 3:
      viewmedicine();
      break;
    case 4:
      deletemedicine();
      break;
    case 0:
      printf("\n\n\n\t\t\t THANK YOU!!!! \n\n\n");
      exit(1);
      break;
    default:
      printf("\n\n\n\t\t\t INVALID...........");
    }
  }
    while (choice != 0);
}

void login()
{
    unsigned char username[MAX_SIZE_USER_NAME] = {0};
    unsigned char password[MAX_SIZE_PASSWORD] = {0};
    int L=0;
    sFileHeader fileHeaderInfo = {0};
    FILE *fp = NULL;
    headmessage("Login");
    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    fread (&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
    fclose(fp);
    do
    {
        printf("\n\n\n\t\t\t\tUsername:");
        fgets(username,MAX_SIZE_USER_NAME,stdin);
        printf("\n\t\t\t\tPassword:");
        fgets(password,MAX_SIZE_PASSWORD,stdin);
        if((!strcmp(username,fileHeaderInfo.username)) && (!strcmp(password,fileHeaderInfo.password)))
        {
            menu();
        }
        else
        {
            printf("\t\t\t\tLogin Failed Enter Again Username & Password\n\n");
            L++;
        }
    }
    while(L<=3);
    if(L>3)
    {
        headmessage("Login Failed");
        printf("\t\t\t\tSorry,Unknown User.");

        system("cls");
    }
}



//check if file exists
int isFileExist(const char* path)
{

    //try to open file
    FILE *fp=fopen(path,"rb");
    int status=0;
    //if file does not exist
    if(fp!=NULL)
    {
        status = 1;
        //if file exists hence close file
        fclose(fp);
    }
    return status;
}


//create file
void init()
{
    FILE *fp = NULL;
    int status = 0;
    const char defaultUsername[] ="bvm";
    const char defaultPassword[] ="bvm";
    sFileHeader fileHeaderInfo = {0};
    status = isFileExist(FILE_NAME);
    if(!status)
    {
        //create the binary file
        fp = fopen(FILE_NAME,"wb");
        if(fp != NULL)
        {
            //Copy default password
            strncpy(fileHeaderInfo.password,defaultPassword,sizeof(defaultPassword));
            strncpy(fileHeaderInfo.username,defaultUsername,sizeof(defaultUsername));
            fwrite(&fileHeaderInfo,FILE_HEADER_SIZE, 1, fp);
            fclose(fp);
        }
    }
}

void main()
{

    init();
    welcomemessage();
    login();
    menu();

  }

