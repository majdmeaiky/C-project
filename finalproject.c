#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
	int year;
	char month;
	char day;
}Date;

typedef struct
{
	long long Id;
	char* Name;
	char* Family;
	Date dateOfBirth;
	long long PartnerId;
	long long MotherId;
	long long FatherId;
	char NumOfChildren;
	long long* ChlidrenPtr;
}person;

typedef struct
{
	person* records;
	int recordsize;
	int citizensinrecords;
}db_mgr;

void print_person(const person* citizen);
void init_db(db_mgr* management);
char menu();
void add_person(db_mgr*citizen);
int digitsInId(long long id);
void orderId(db_mgr* citizen);
person* search_id(const db_mgr* citizen, const long long id);
void search_person(const db_mgr* citizen);
void search_parents(const db_mgr* citizen);
void delete_person(db_mgr* citizen);
void get_gen(const db_mgr* citizen);
void print_db(const db_mgr* citizen);
void search_by_name(const db_mgr* citizen);
void Quit(db_mgr* citizen);

void main()
{
	db_mgr citizen;// הגדרת מבנה מסוג בסיס נתונים
	printf("please enter number of citizens: ");
	scanf("%d", &citizen.recordsize); //הגדרת מספר תושבים בבסי הנתנונים
	citizen.citizensinrecords = 0;
	long long id;
	init_db(&citizen);//הקצאת מקום למערך התושבים
	char option; // משתנה לשמירת בחירת האפשרות מהתפריט

	do {
		printf("\n");
		option = menu();
		switch (option)
		{
		case '1':
			add_person(&citizen);
			break;
		case '2':
			search_person(&citizen);;
			break;
		case '3':
			search_parents(&citizen);
			break;
		case '4':
			delete_person(&citizen);
			break;
		case '5':
			get_gen(&citizen);
			break;
		case '6':
			print_db(&citizen);
			break;
		case '7':
			search_by_name(&citizen);
			break;
		case '8':
			Quit(&citizen);
			break;
		}
	} while (option != '8');
}

void print_person(const person* citizen)//function argument is pointer to person struct, the function prints all the relevant info about the person
{
	printf("id num is: %lli \n", citizen->Id); // הדפסת ת.ז
	printf("citizn first name is %s \n", citizen->Name); //הדפתסת שם פרטי
	printf("citizn last name is: %s \n", citizen->Family);//הדפתסת שם משפחה
	printf("citizen's date of birth is: %d-%d-%d \n", citizen->dateOfBirth.day, citizen->dateOfBirth.month, citizen->dateOfBirth.year);// הדפסת ת.לידיה
	if (citizen->PartnerId != 0)// הדפתסת ת.ז פרטנר אם קיים
	{
		printf("citizen partner Id is: %lli\n", citizen->PartnerId);
	}
	if (citizen->MotherId != 0)// הדפתסת ת.ז אמא אם קיים
	{
		printf("citizen mother Id is: %lli\n", citizen->MotherId);
	}
	if (citizen->FatherId != 0)// הדפתסת ת.ז אבא אם קיים
	{
		printf("citizen father Id is: %lli\n", citizen->FatherId);
	}
	printf("citizen number of children is: %d \n", citizen->NumOfChildren);
	int y = citizen->NumOfChildren + '0';// משתנה עזר להמרת מספר ילדים למשתנה מסוג מספר שלם
	if (y > 0)// הדפסת ת.ז ילדים אם קיים ע"י מעבר על מערך ילדים
	{
		for (int i = 0; i < citizen->NumOfChildren; i++)
		{
			printf("%d-citizen's children Id : %lli\t\n", (i + 1), citizen->ChlidrenPtr[i]);
		}
	}
}

void init_db(db_mgr* management)// function argument is pointer to db_mgr struct, the function checks the legality of the recors ssize allocates space in memory acording to record size
{

	if (management->recordsize > 0)//בדיקת תקינות לגודל המערך
	{
		management->records = (person*)malloc(management->recordsize * sizeof(person));//הקצאת זיכרון למערך התושבים
		if (management->records == 0)//בדיקת הצלחת הקצאה 
		{
			printf("allocation failed!");
		}
	}
}

char menu()//the function prints the menu of the possible function and return the choise (char)
{
	char tav; //משתנה מסוג תו לקליטת אפשרות הבחירה מהתפריט
	//הצגת תפריט ובקשה לבחירת אפשרות מהתפריט
	printf("Database System Menu:\n1. Add person\n2. Search a person\n3. Search Parents\n4. Delete a person\n");
	printf("5. Get generation\n6. Print database\n7. Search by name\n8. Quit\n");
	printf("please enter a choice from 1-8:\t");
	scanf(" %c", &tav);

	while (tav < '1' || tav > '8')//בדיקת תקינות הקלט וחזרה במידה ולא תקין
	{
		printf("\n***try again***\n\n");
		printf("Database System Menu:\n1. Add person\n2. Search a person\n3. Search Parents\n4. Delete a person\n");
		printf("5. Get generation\n6. Print database\n7. Search by name\n8. Quit");
		printf("please enter a choice from 1-8:\t");
		scanf(" %c", &tav);
	}
	return tav;// החזרת הערך הנבחר

}

void add_person(db_mgr* citizen)//the function arguments is pointer to db_mgr struct , the function reads info and checks its legality, and ads person to the records acording to the info 
{
	char nameTmp[100];//משתנה עזר לקליטת שם ושם משפחה

	//בדיקה האם יש מקו במערך להוספת תושב או שיש צורך להגדיל את גודל המערך
	if (citizen->citizensinrecords == citizen->recordsize)
	{
		person* toshav; // מערך עזר אליו נבצע העתקה
		toshav = (person*)malloc(citizen->recordsize * sizeof(person));//הקצאת זיכרון למערך עזר
		if (toshav == NULL)//בדיקת תקינות הקצאה
		{
			printf("allocation failed!");
			return NULL;
		}

		//העתקת מערך התושבים למערך העזר
		for (int i = 0; i < citizen->recordsize; i++)
		{
			toshav[i] = citizen->records[i];
		}
		citizen->recordsize++;//הגדלת גודל המערך +1
		init_db(citizen);//(הקצאת זיכרון חדשה למערך התושבים בגודל החדש (+1
		//העתקת מערך העזר חזרה למערך התושבים
		for (int j = 0; j < citizen->recordsize; j++)
		{
			citizen->records[j] = toshav[j];
		}
		free(toshav);//שחרור זכרון למערך העזר
	}
	//#הכנסת פרטי התושב החדש למקום האחרון במערך#
	printf("\nplease enter citizen Id (format of 9 numbers): ");
	scanf("%lli", &citizen->records[citizen->citizensinrecords].Id);//קליטת ת.ז
	while (digitsInId(citizen->records[citizen->citizensinrecords].Id) != 9)//בדיקת תקינות ת.ז אם היא באורך 9 ספרות
	{
		printf("ilegal Id try again!: ");
		scanf("%lli", &citizen->records[citizen->citizensinrecords].Id);
	}
	printf("\nplease enter citizen first name: ");
	scanf(" %s", &nameTmp); //קליטת שם פרטי למשתנה עזר מסוג סטרינג
	citizen->records[citizen->citizensinrecords].Name = (char*)calloc(strlen(nameTmp) + 1, sizeof(char));//הקצאת זיכרון לשם פרטי באורך השם +1
	if (citizen->records[citizen->citizensinrecords].Name == NULL)//בדיקת הקצאה
	{
		printf("allocation failed!");
		return NULL;
	}
	strcpy(citizen->records[citizen->citizensinrecords].Name, nameTmp);//העתקת מחרוזת ממשתנה העזר לשדה שם פרטי של התושב
	printf("\nplease enter citizen last name: ");
	scanf(" %s", &nameTmp);//קליטת שם משפחה למשתנה עזר מסוג סטרינג
	citizen->records[citizen->citizensinrecords].Family = (char*)calloc(strlen(nameTmp) + 1, sizeof(char));//הקצאת זיכרון לשם פרטי באורך השם +1
	if (citizen->records[citizen->citizensinrecords].Family == NULL)//בדיקת הקצאה
	{
		printf("allocation failed!");
		return NULL;
	}
	strcpy(citizen->records[citizen->citizensinrecords].Family, nameTmp);//העתקת מחרוזת ממשתנה העזר לשדה שם משפחה של התושב
	printf("\nplease enter citizen day of birth: ");
	int tempDate;//משתנה עזר לקליטת התאריך בצורה של מספר שלם
	scanf("%d", &tempDate);//קליטת יום כמספר שלם
	while (tempDate < 1 || tempDate> 30)//בדקת תקינות יום
	{
		printf("\nday is not legal try again!: ");
		scanf("%d", &tempDate);
	}
	citizen->records[citizen->citizensinrecords].dateOfBirth.day = tempDate;//השמת הקלט לתוך שדה יום בתאריך הלידה של התושב

	printf("\nplease enter citizen month of birth: ");
	scanf("%d", &tempDate);//קליטת חודש כמספר שלם 
	while (tempDate < 1 || tempDate > 12)//בדיקת תקינות חודש
	{
		printf("\nmonth is not legal try again!: ");
		scanf("%d", &tempDate);
	}
	citizen->records[citizen->citizensinrecords].dateOfBirth.month = tempDate;//השמת הקלט לתוך שדה חודש בתאריך הלידה של התושב

	printf("\nplease enter citizen year of birth: ");
	scanf(" %d", &citizen->records[citizen->citizensinrecords].dateOfBirth.year);//קליטת שנת לידה
	while (citizen->records[citizen->citizensinrecords].dateOfBirth.year > 2021 || citizen->records[citizen->citizensinrecords].dateOfBirth.year < 1921)//בדיקת תקינות שנת לידה
	{
		printf("\nyear is not legal try again!: ");
		scanf("%d", &citizen->records[citizen->citizensinrecords].dateOfBirth.year);
	}

	printf("\nplease enter partner Id if not exist enter 0 (format of 9 numbers): ");
	scanf("%lli", &citizen->records[citizen->citizensinrecords].PartnerId);//קליטת ת.ז פרטנר
	while (digitsInId(citizen->records[citizen->citizensinrecords].PartnerId) != 9 && citizen->records[citizen->citizensinrecords].PartnerId != 0)//בדקת תקינות ת.ז יש 9 ספרות או שווה ל0
	{
		printf("ilegal Id try again!: ");
		scanf("%lli", &citizen->records[citizen->citizensinrecords].PartnerId);
	}

	printf("\nplease enter mother Id if not exist enter 0 (format of 9 numbers): ");
	scanf("%lli", &citizen->records[citizen->citizensinrecords].MotherId); //קליטת ת.ז אמא
	while (digitsInId(citizen->records[citizen->citizensinrecords].MotherId) != 9 && citizen->records[citizen->citizensinrecords].MotherId != 0)//בדקת תקינות ת.ז יש 9 ספרות או שווה ל0
	{
		printf("ilegal Id try again!: ");
		scanf("%lli", &citizen->records[citizen->citizensinrecords].MotherId);
	}

	printf("\nplease enter father Id if not exist enter 0 (format of 9 numbers): ");
	scanf("%lli", &citizen->records[citizen->citizensinrecords].FatherId); //קליטת ת.ז אבא
	while (digitsInId(citizen->records[citizen->citizensinrecords].FatherId) != 9 && citizen->records[citizen->citizensinrecords].FatherId != 0)//בדקת תקינות ת.ז יש 9 ספרות או שווה ל0
	{
		printf("ilegal Id try again!: ");
		scanf("%lli", &citizen->records[citizen->citizensinrecords].FatherId);
	}

	printf("\nplease enter number of children: ");
	int tempChild; // משתנה עזר לקליטת מספר הילדים כמספר שלם
	scanf(" %d", &tempChild);//קליטת מספר הילדים למשתנה עזר
	citizen->records[citizen->citizensinrecords].NumOfChildren = tempChild;//השמת משתנה העזר לתוך שדה מספר הילדים אצל התושב
	if (tempChild > 0)//אם מספר הילדים גדול מ0 נעבור על מערך הילדים ונקלוט תז לכל אחד
	{
		citizen->records[citizen->citizensinrecords].ChlidrenPtr = (long long*)malloc((tempChild) * sizeof(long long));//הקצאת זיכרון למערך תעודות הזהות של הילדים של התושב
		if (citizen->records[citizen->citizensinrecords].ChlidrenPtr == NULL)//בדיקת הקצאה
		{
			printf("allocation failed!");
			return NULL;
		}
		for (int i = 0; i < tempChild; i++)//מעבר על מערך הילדים
		{
			printf("\nplease enter child %d  Id (format of 9 numbers): ", (i + 1));
			scanf("%lli", &citizen->records[citizen->citizensinrecords].ChlidrenPtr[i]);//קליטת ת.ז ילד
			while (digitsInId(citizen->records[citizen->citizensinrecords].ChlidrenPtr[i]) != 9)//בדקת תקינות ת.ז יש 9 ספרות 
			{
				printf("ilegal Id try again!: ");
				scanf("%lli", &citizen->records[citizen->citizensinrecords].ChlidrenPtr[i]);//הכנסת ת.ז למערך
			}
		}
	}
	orderId(citizen);//סידור מערך התושבים בבסיס הנתונים בסדר עולה
	citizen->citizensinrecords++;//מספר התושבים במערך +1 

}

int digitsInId(long long id)//function argument is long long type variable (id), the function returns the amount of digits in long long id
{
	int count = 0;//משתנה לסיפרת מספר הספרות-מונה
	while (id != 0)//כל עוד הת.ז לא שווה ל0 נוסיף נוסיף למונה 1 ונוריד ספרה מהת.ז 
	{
		count++;
		id = id / 10;
	}
	return count;//החזרת מספר הספרות בת.ז
}

void orderId(db_mgr* citizen)//the function arguments is pointer to db_mgr struct , the function search a place for the last added person to keep the records array in ascending order and reorder the persons in the records to place the last added person in the right place
{
	person tmp = citizen->records[citizen->citizensinrecords];//(משתנה עזר למבנה מסוג תושב מקבל את התושב האחרון במערך(האחרון שנוסף   
	for (int i = 0; i < citizen->citizensinrecords + 1; i++)
	{
		for (int j = i + 1; j < citizen->citizensinrecords + 1; j++)
		{
			if (citizen->records[i].Id > citizen->records[j].Id)
			{
				tmp = citizen->records[i];
				citizen->records[i] = citizen->records[j];
				citizen->records[j] = tmp;
			}
		}
	}
}

person* search_id(const db_mgr* citizen, const long long id)//the function arguments is pointer to db_mgr struct and long long type variable, the function will find person in the records with the same id and return pointer to person struct of the person how was found otherwise terurns null
{
	for (int i = 0; i < citizen->recordsize; i++)//מעבר על מערך התושבים
	{
		if (citizen->records[i].Id == id)//האם לתושב יש ת.ז זהה לזה שמבקשים
		{
			return (citizen->records + i);//החזרת מצביע לתושב
		}
	}
	return NULL;//אחרת מחזיר NULL
}

void search_person(const db_mgr* citizen)//the function argument is pointer to db_mgr struct, the function asks for id from the user and search for a person with the same id and prints his relevant info 
{
	long long id;//משתנה שבו נקלוט ת.ז
	person* personPtr;//משתנה עזר מסוג מצביע לתושב שלו נעשה השמה של המצביע לתושב במוחזר 
	printf("\nplease enter citizen Id (format of 9 numbers):\t");
	scanf("%lli", &id);//קליטת ת.ז
	personPtr = search_id(citizen, id);//השמת המציע לתושב שמוחזרת מהפונקציה למשתנה עזר
	if (personPtr == NULL) //אם הבן אדם לא נמצא נדפיס הודעת שגיאה
	{
		printf("\nperson not found!\n");
	}
	else//אחרת נדפיס את פרטי התושב
	{
		print_person(personPtr);
	}
}

void search_parents(const db_mgr* citizen) // the function arguments is pointer to db_mgr struct , the function will ask for id of person and will print the info about the perents of the person if there is any 
{
	long long id;//משתנה אליו נקלוט ת.ז של התושב
	person* personPtr;// משתנה עזר מסוג מצביע לתושב כדי לשמור את התושב
	person* motherPtr;//משתנה עזר מסוג מצביע לתושב לשמור את האמא 
	person* fatherPtr;//משתנה עזר מסוג מצביע לתושב לשמור את האבא 
	printf("\nplease enter citizen Id (format of 9 numbers): ");
	scanf("%lli", &id); // קליטת ת.ז
	personPtr = search_id(citizen, id);//השמת הערך המוחזר מהפונקציה למשתנה עזר תושב
	if (personPtr == NULL)//בדיקה האם התושב קיים בבסיס הנתונים
	{
		printf("\nperson not found!\n");
	}
	motherPtr = search_id(citizen, personPtr->MotherId);//השמת הערך המוחזר מהפונקציה למשתנה עזר אמא של התושב
	fatherPtr = search_id(citizen, personPtr->FatherId);//השמת הערך המוחזר מהפונקציה למשתנה עזר אבא של התושב
	if (motherPtr == NULL)//בדיקה האם האמא קיימת
	{
		printf("\nsorry may your mother RIP :( \n");//הצגת הודעה אם לא נמצאת
	}
	else
	{
		print_person(motherPtr);//הדפסת פרטי האבא אם נמצאא
	}
	if (fatherPtr == NULL)//בדיקה האם האבא קיים
	{
		printf("\nsorry may your father RIP :( \n");//הצגת הודעה אם לא נמצא
	}
	else
	{
		print_person(fatherPtr);//הדפסת פרטי האבא אם נמצא
	}
}

void delete_person(db_mgr* citizen)//the function arguments is pointer to db_mgr struct, the function asks for id of a person to delete and removes the person struct for the relevant id from the records array
{
	person* personPtr; //משתנה עזר מסוג מצביע לתןשב אותו נרצה למחוק
	person* partnerPtr;//משתנה עזר מסוג מצביע לתושב כדי לגשת לפרטנט של התושב אותו נרצה למחוק
	long long idToDelete; // משתנה לקליטת ת.ז של התושב אותו נרצה למחוק
	person* tmprecord;//מערך עזר 
	person* tmpmother;//משתנה עזר מסוג מצביע לתושב כדי לגשת לאמא של התושב אותו נרצה למחוק
	person* tmpfather;//משתנה עזר מסוג מצביע לתושב כדי לגשת לאבא של התושב אותו נרצה למחוק
	person* tmpchild;//משתנה עזר מסוג מצביע לתושב כדי לגשת לילד של התושב אותו נרצה למחוק

	printf("\nplease enter citizen id you want to delete:\t");
	scanf("%lli", &idToDelete);//קלית ת.ז למחיקה
	personPtr = search_id(citizen, idToDelete);
	if (personPtr == NULL)//מדיקה אם התושב נמצא 
	{
		printf("\nperson not found!");
		return;
	}
	if (personPtr->PartnerId != 0) //בדיקה האם קיים לתושב פרטנר
	{
		partnerPtr = search_id(citizen, personPtr->PartnerId);//השמת הערך המוחזר מחיפוש הפרטנר בבסיס למשתנה עזר
		partnerPtr->PartnerId = 0; // שינוי הערך ת.ז פרטנר אצל הפרטנר של התושב ל0 
	}
	if (personPtr->MotherId != 0)//בדיקה האם קיים לתושב אמא
	{
		tmpmother = search_id(citizen, personPtr->MotherId);//השמת הערך המוחזר מחיפוש ת.ז האמא בבסיס למשתנה עזר
		long long* tempChildArr;//הגדרת מערך עזר של תעודות זהות של הילדים
		tempChildArr = (long long*)malloc((tmpmother->NumOfChildren - 1) * sizeof(long long));//הקצאת מקום למערך
		if (tempChildArr == NULL)//בדיקת הקצאה
		{
			printf("alocation failed");
			return;
		}
		for (int i = 0, j = 0; i < tmpmother->NumOfChildren, j < tmpmother->NumOfChildren; i++, j++)//העתקת מערך הילדים של אמא של התושב אותו רוצים למחוק ללא התושב
		{
			if (tmpmother->ChlidrenPtr[i] == personPtr->Id)
			{
				j++;
			}
			tempChildArr[i] = tmpmother->ChlidrenPtr[j];
		}
		tmpmother->NumOfChildren--; // הקטנת מספר הילדים של אמא של התושב אותו רוצים למחוק
		tmpmother->ChlidrenPtr = (long long*)malloc((tmpmother->NumOfChildren) * sizeof(long long));//הקצאות מקום מחדש למערך הילדים של אמא של התושב בגודל -1
		if (tmpmother->ChlidrenPtr == NULL)//בדיקת הקצאה
		{
			printf("allocation failed!");
			return;
		}
		for (int i = 0; i < tmpmother->NumOfChildren; i++)//העתקה ממערך העזר חזרה למערך הילדים של האמא
		{
			tmpmother->ChlidrenPtr[i] = tempChildArr[i];
		}
		free(tempChildArr);//שחרור של מערך העזר
	}

	if (personPtr->FatherId != 0)//בדיקה האם קיים לתושב אבא
	{
		tmpfather = search_id(citizen, personPtr->FatherId);
		long long* tempChildArr;//הגדרת מערך עזר של תעודות זהות של הילדים
		tempChildArr = (long long*)malloc((tmpfather->NumOfChildren - 1) * sizeof(long long));//הקצאת מקום למערך
		if (tempChildArr == NULL)//בדיקת הקצאה
		{
			printf("alocation failed");
			return;
		}
		for (int i = 0, j = 0; i < tmpfather->NumOfChildren, j < tmpfather->NumOfChildren; i++, j++)//העתקת מערך הילדים של אבא של התושב אותו רוצים למחוק ללא התושב
		{
			if (tmpfather->ChlidrenPtr[i] == personPtr->Id)
			{
				j++;
			}
			tempChildArr[i] = tmpfather->ChlidrenPtr[j];
		}
		tmpfather->NumOfChildren--;//הקטנת מספר הילדים של אבא של התושב
		tmpfather->ChlidrenPtr = (long long*)malloc((tmpfather->NumOfChildren) * sizeof(long long));//הקצאת מקום מחדש למערך הילדים של אבא של התושב בגודל -1
		if (tmpfather->ChlidrenPtr == NULL)//בדיקת הקצאה
		{
			printf("allocation failed!");
			return;
		}
		for (int i = 0; i < tmpfather->NumOfChildren; i++)//העתקה ממערך העזר חזרה למערך הילדים של אבא של התושב
		{
			tmpfather->ChlidrenPtr[i] = tempChildArr[i];
		}
		free(tempChildArr);//שחרור של מערך העזר
	}

	if (personPtr->NumOfChildren != 0)//בדיקה האם לתושב יש ילדים
	{
		for (int i = 0; i < personPtr->NumOfChildren; i++)//מעבר על מערך הילדים של התושב
		{
			tmpchild = search_id(citizen, personPtr->ChlidrenPtr[i]);//השמת הערך החודר מהפונקציה למשתנה עזר ילד של התושב
			if (tmpchild->MotherId == idToDelete)//בדיקה אם תעודת הזהות אותה רוצים למחוק היא של האמא  
			{
				tmpchild->MotherId = 0;//איפוס ת.ז של האמא של הילד 
			}
			if (tmpchild->FatherId == idToDelete)//בדיקה אם תעודת הזהות אותה רוצים למחוק היא של האבא
			{
				tmpchild->FatherId = 0;//איפוס ת.ז של האבא של הילד 
			}
		}
	}
	tmprecord = (person*)malloc((citizen->recordsize) * sizeof(person));//הקצאת מקום למערך עזר מערך תושבים
	if (tmprecord == NULL)//בדיקת הקצאה
	{
		printf("allocation failed!");
		return;
	}
	for (int i = 0, j = 0; i < citizen->citizensinrecords, j < citizen->citizensinrecords; i++, j++)//העתקת התושבים למערך העזר ללא התושב אותו רוצים למחוק
	{
		if (citizen->records[j].Id == idToDelete)
		{
			j++;
		}
		tmprecord[i] = citizen->records[j];
	}
	citizen->recordsize--;//הקטנת גודל מערך התושבים
	citizen->citizensinrecords--;//הקטנת גודל מספר האנשים במערך התושבים
	citizen->records = (person*)malloc((citizen->recordsize) * sizeof(person));//הקצאת זיכרון חדשה למערך התושבים בגודל החדש 
	if (citizen->records == NULL)//בדיקת הקצאה
	{
		printf("allocation failed!");
		return;
	}
	for (int i = 0; i < citizen->citizensinrecords; i++)//העתקה חזרה ממערך העזר למערך התושבים
	{
		citizen->records[i] = tmprecord[i];
	}
	free(tmprecord);//שחרור מערך העזר
}

void print_db(const db_mgr* citizen)//the function arguments is pointer to db_mgr struct , the function will print the relevant info for all the person structs in the records
{

	for (int i = 0; i < citizen->citizensinrecords; i++)//מעבר על כל התושבים בבסיס הנתונים
	{
		printf("\n**person number %d** \n", (i + 1)); //הדפסת כותרת
		print_person(&(citizen->records[i]));//הדפסת פרטי התושב במקום הנוחכי במערך
		printf("\n");
	}
}

void search_by_name(const db_mgr* citizen)//the function arguments is pointer to db_mgr struct,the function will ask for first name and last name (string) to search and will print the info about all the peron structs in the records with the same first and last name
{
	int check = 0;//משתנה עזר לבדיקת מספר האנשים עם שם זהה
	char firstName[100];//משתנה עזר לקליטת שם פרטי
	char lastName[100];//משתנה עזר לקליטת שם משפחה
	printf("\nplease enter person first name:\t");
	scanf(" %s", &firstName);//קליטת שם פרטי
	printf("\nplease enter person last name:\t");
	scanf(" %s", &lastName);//קליטת שם משפחה
	printf("\npersons with the given name:\n\n");
	for (int i = 0; i < citizen->citizensinrecords; i++)
	{
		if ((strcmp(firstName, citizen->records[i].Name) == 0) && (strcmp(lastName, citizen->records[i].Family) == 0))//בדיקה אם התושב במקום הנוחכי הוא עם שם פרטי ושם משפחה לפי המבוקש
		{
			check++;//הגדלת מספר האנשים עם שם זהה ב1
			print_person(&(citizen->records[i]));//הדפסת פרטי התושב עם שם זהה
			printf("\n");
		}
	}
	if (check == 0)//בדיקה אם היו אנשים עם שם זהה לשם המבוקש
	{
		printf("\nthere isn't a person with this name\n");//הדפסת הודעה מתאימה
	}
}

void Quit(db_mgr* citizen)//the function arguments is pointer to db_mgr struct, the function will print finish massge and free all the allocated memory
{
	printf("\nthank you bye bye\n");//הדפסת הודעת סיום
	for (int i = 0; i < citizen->citizensinrecords; i++)//מעבר על עכל מערך התושבים
	{
		free(citizen->records[i].Name);//שחרור זכרון שהוקצא עבור שם פרטי
		free(citizen->records[i].Family);//שחרור זכרון שהוקצא עבור שם משפחה
		if (citizen->records[i].NumOfChildren > 0)//בדיקה האם קיממים ילדים
		{
			free(citizen->records[i].ChlidrenPtr);//אם כן שחרור מערך הילדים
		}
	}
	free(citizen->records);//שחרור זכרון מערך התושבים בבסיס הנתונים
}

void get_gen(const db_mgr* citizen)// the function arguments is pointer to db_mgr struct , the function will ask for id and will check if he have kids if his kids have kids and so forward and will calculate and print the persons generation
{
	long long id;//תעודת זהות של התושב עליו רוצים לחפש דור
	person* personPtr;//משתנה עזר מסוג מצביע לתושב 
	person* adults;//מערך עזר מסוג תושב- בוגרים
	person* kids;//מערך עזר מסוג תושב- ילדים
	int kidsCount = 0;//משתנה מונה ילדים
	int gen = 1;//משתנה לספירת הדור
	int tmpcount = 0;//משתנה עזר לספירה
	printf("\nplease enter citizen Id (format of 9 numbers):\t");
	scanf("%lli", &id);//קליטת תעודת זהות
	personPtr = search_id(citizen, id);//השמת הערך החוזר מהפונקציה למשתנה העזר
	if (personPtr == NULL)//בדיקה האם התושב נמצא בבסיס הנתונים
	{
		printf("\nperson not found!\n");//הדפסת פלט מתאים
		return;
	}

	if (personPtr->NumOfChildren == 0)//בדיקה האם לתושב אין ילדים
	{
		printf("\nperson number of generetion is 1\n");//הדפסת דור 1
		return;
	}

	adults = (person*)malloc(1 * sizeof(person));//הקצאת זיכרון למערך הבוגרים
	if (adults == NULL)//בדיקת הקצאה
	{
		printf("\nallocation failed!");
		return;
	}
	adults[0] = *personPtr;//השמת התושב למערך הבוגרים, הוא הבוגר הראשון

	kids = (person*)malloc(personPtr->NumOfChildren * sizeof(person));//הקצאת מקום למערך ילדים
	if (kids == NULL)//בדיקת הקצאה
	{
		printf("\nallocation failed!");
		return;
	}
	//השמת הילדים לתוך מערך הילדים, הם הילדים הראשונים
	for (int i = 0; i < personPtr->NumOfChildren; i++)
	{
		kids[i] = *(search_id(citizen, personPtr->ChlidrenPtr[i]));
		kidsCount++;
	}
	while (kidsCount != 0)//נעבור על הלולאה עד שלאף אחד ממערך הבוגרים אין ילדים זה יהיה הדור האחרון 
	{
		tmpcount = 0;
		// סופר את הילדים שיש להם ילדים , הם יהיו הבוגרים הבאיםTMPCOUNT 
		for (int i = 0; i < kidsCount; i++)
		{
			if (kids[i].NumOfChildren != 0)
			{
				tmpcount++;
			}
		}
		adults = (person*)malloc(tmpcount * sizeof(person));//הקצאת מקום מחדש למערך בוגרים בגודל מספר הילדים שיש להם ילדים
		if (adults == NULL)//בדיקת הקצאה
		{
			printf("\nallocation failed!");
			return;
		}
		//השמת הילדים שיש להם ילדים למערך בוגרים
		for (int i = 0, j = 0; i < kidsCount; i++)
		{
			if (kids[i].NumOfChildren != 0)
			{
				adults[j] = kids[i];
				j++;
			}
		}
		kidsCount = 0;//איפוס ספירת הילדים
		//ספירת סכום כל הילדים של מי שנמצא במערך הבוגרים
		for (int j = 0; j < tmpcount; j++)
		{
			kidsCount = kidsCount + adults[j].NumOfChildren;
		}
		kids = (person*)malloc(kidsCount * sizeof(person));//הקצאת זיכרון בגודל כל הילדים של מי שבמערך הבוגרים
		if (kids == NULL)//בדיקת הקצאה
		{
			printf("allocation failed!");
			return;
		}
		int idx = 0;// הגדרת אינדקס להשמה במערך הילדים
		//השמת כל הילדים של מי שבמערך הבוגרים למערך הילדים 
		for (int j = 0; j < tmpcount; j++)
		{
			for (int i = 0; i < adults[j].NumOfChildren; i++)
			{
				kids[idx] = *(search_id(citizen, adults[j].ChlidrenPtr[i]));
				idx++;
			}
		}
		gen++;//הגדלת הדור ב1
	}
	printf("\ngeneration is %d\n", gen);// הדפסת הדור
	free(adults);//שחרור מערך בוגרים
	free(kids);//שחרור מערך ילדים
}