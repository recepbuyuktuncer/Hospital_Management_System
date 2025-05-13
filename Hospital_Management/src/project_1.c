/*
 ============================================================================
 Name        		: hospital_automation.c
 Author      		: Recep Buyuktuncer
 Version     		:
 Code References   	: p1.state = 1 ---> Patient is being treated ;	d_ptr: Doctor Pointer	  ; p_ptr:Patient Pointer; dn: doctor number ;
 	 	 	 	 	  p1.state = 0 ---> Patient is waiting		 ;  e_ptr: Examination Pointer; 					   pl : policlinic number ;

 Description 		: Hospital Automation
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char id_number[20];		// It is better to take char if the numbers will not be processed.
	char name[20];
	char surname[20];
	char phone_number[20];
	char city[20];
	int  policlinic_number;
	int  doctor_number;
	int  state;
}patient;

typedef struct {
	char name[20];
	char surname[20];
	int  policlinic_number;
	int  doctor_number;
}doctor;

void patient_register()
{
	system("clear");
	patient p1;
	doctor d1;
	char file_name[20], file_pass[20];
	int pl, dn;
	int control = 0;

	printf("Patient Register Page\n");
	printf("%-20s: ", "ID Number");           scanf(" %[^\n]s", p1.id_number);
	printf("%-20s: ", "Name");                scanf(" %[^\n]s", p1.name);
	printf("%-20s: ", "Surname");             scanf(" %[^\n]s", p1.surname);
	printf("%-20s: ", "Phone Number");        scanf(" %[^\n]s", p1.phone_number);
	printf("%-20s: ", "City");                scanf(" %[^\n]s", p1.city);
	system("clear");

	printf("Policlinics:\n");
	printf("  1 - Otolaryngology\n");
	printf("  2 - Physical Medicine\n");
	printf("  3 - Pediatrics\n");
	printf("  4 - Neurology\n");
	printf("  5 - Internal Medicine\n");
	printf("%-20s: ", "Policlinic Number");   scanf("%d", &p1.policlinic_number);

    FILE *d_ptr = fopen("doctors.txt", "r");
    while (fscanf(d_ptr, "%s %s %d %d", file_name, file_pass, &pl, &dn) != EOF)
    {
    strcpy(d1.name, file_name);
    strcpy(d1.surname, file_pass);
    d1.policlinic_number = pl;
    d1.doctor_number = dn;

       if (p1.policlinic_number == d1.policlinic_number)
       {
    	   printf("Doctor %-3d\t%-15s %-15s\n", d1.doctor_number, d1.name, d1.surname);
    	   control = 1;
       }
    }
	fclose(d_ptr);
	if(control == 0)
	{
		printf("There is no doctor in this clinic!\n");
	}
	else
	{
		printf("Doctor Number: "); scanf("%d", &p1.doctor_number);
		p1.state = 0;
		FILE *p_ptr = fopen("patients.txt", "a");
		if (p_ptr == NULL)
		{
		    printf("File could not be created! \n");
		    return;
		}

	fprintf(p_ptr, "%s %s %s %s %s %d %d %d\n", p1.id_number, p1.name, p1.surname, p1.phone_number, p1.city, p1.policlinic_number, p1.doctor_number, p1.state);
	fclose(p_ptr);

	printf("%-20s\n", "Patient Register Successful");
	}
}

void examination(doctor d1)
{
	patient p1;
	int control = 0;
	char result[50];
	char file_name[6];
	file_name[0] = d1.name[0];
	file_name[1] = d1.surname[0];
	file_name[2] = '.';
	file_name[3] = 't';
    file_name[4] = 'x';
    file_name[5] = 't';

    FILE *e_ptr = fopen(file_name, "a");
    if (e_ptr == NULL)
    {
    	printf("Could not open examination result file.\n");
    	return;
    }

	printf("Waiting Patients \n\n");

	FILE *p_ptr = fopen("patients.txt", "r");
	if (p_ptr == NULL)
	{
		printf("No patients register found \n");
		return;
	}

	FILE *temp_ptr = fopen("temp.txt", "w");  // geçici dosya
	if (temp_ptr == NULL) {
		printf("Temporary file could not be created.\n");
		fclose(p_ptr);
		fclose(e_ptr);
		return;
	}

	while (fscanf(p_ptr, "%s %s %s %s %s %d %d %d", p1.id_number, p1.name, p1.surname,  p1.phone_number, p1.city, &p1.policlinic_number, &p1.doctor_number, &p1.state) == 8)
	{
		if (d1.doctor_number == p1.doctor_number && p1.state == 0)
		{
			control = 1;
			printf("%s-Examination Result :", p1.name); scanf(" %[^\n]s", result);
			p1.state=1; 	// Examination is over

			fprintf(e_ptr, "%s\n%s %s\nResult: %s\n\n", p1.id_number, p1.name, p1.surname, result);
			fprintf(temp_ptr, "%s %s %s %s %s %d %d %d\n",p1.id_number, p1.name, p1.surname, p1.phone_number, p1.city,p1.policlinic_number, p1.doctor_number, p1.state);

		}
	}
	fclose(p_ptr);
	fclose(temp_ptr);
	fclose(e_ptr);

	remove("patients.txt"); // The original file is deleted, the name of the temporary file is changed and the update process is completed.
	rename("temp.txt", "patients.txt");

	if (!control)
		printf("No Patients Waiting!\n");
	else
		examination(d1);
}

void waiting_patients(doctor d1)
{
		system("clear");
		patient p1;
	    int control = 0;

	    printf("Waiting Patients Page\n");

	    FILE *p_ptr = fopen("patients.txt", "r");
	    if (p_ptr == NULL)
	    {
	    printf("No patients register found \n");
	    return;
	    }
	    FILE *d_ptr = fopen("doctors.txt", "r");

	    while (fscanf(p_ptr, "%s %s %s %s %s %d %d %d", p1.id_number, p1.name, p1.surname,  p1.phone_number, p1.city, &p1.policlinic_number, &p1.doctor_number, &p1.state) == 8)
	    {
	    if (d1.doctor_number == p1.doctor_number && p1.state == 0)
	    	{
	    	control = 1;
	    	printf("%-15s %-15s %-15s \n", p1.id_number, p1.name, p1.surname);
	        }
	    }
	    fclose(p_ptr);
	    fclose(d_ptr);

	    if (!control)
	    	printf("No Patients Waiting! \n");
	    else
	    {
	    	examination(d1);
	    }
}

void doctor_login() {

	system("clear");
	doctor d1;
    char user_name[20], password[20];
    char file_name[20], file_pass[20];
    int pl, dn;
    int control = 0;

    printf("Doctor Login Page\n");
    printf("%-20s: ", "User Name");   scanf(" %[^\n]s", user_name);
    printf("%-20s: ", "Password");    scanf(" %[^\n]s", password);

    FILE *d_ptr = fopen("doctors.txt", "r");
    if (d_ptr == NULL)
    {
    printf("No doctor records found \n");
    return;
    }
    while (fscanf(d_ptr, "%s %s %d %d", file_name, file_pass, &pl, &dn) != EOF) { // fscanf returns the number of successful variable reads from the file. Actually, it may be more correct to use 4 instead of EOF
    if (strcmp(user_name, file_name) == 0 && strcmp(password, file_pass) == 0)
    	{
    	strcpy(d1.name, file_name);  // If you forget to fill in the values ​​after reading them from the file on the login page, you will have trouble while performing operations in other functions.
   	    strcpy(d1.surname, file_pass);
   	    d1.policlinic_number = pl;
   	    d1.doctor_number = dn;
    	printf("Doctor %-15s %-15s Welcome...\n", file_name, file_pass);
        control = 1;
        break;
        }
    }
    fclose(d_ptr);
    if (!control) // control the "control == 0", shortcut of "if (control = 0)"
    	printf("Incorrect User Name or Password!\n");
    else
    {
    	waiting_patients(d1);
    }
}

void doctor_register() {
    system("clear");
    doctor d1;
    printf("Doctor Register Page\n");
    printf("%-20s: ", "Name");                  scanf(" %[^\n]s", d1.name);
    printf("%-20s: ", "Surname");               scanf(" %[^\n]s", d1.surname);
    system("clear");
    printf("Policlinics:\n");
    printf("  1 - Otolaryngology\n");
    printf("  2 - Physical Medicine\n");
    printf("  3 - Pediatrics\n");
    printf("  4 - Neurology\n");
    printf("  5 - Internal Medicine\n");
    printf("%-20s: ", "Policlinic Number");     scanf("%d", &d1.policlinic_number);
    printf("%-20s: ", "Doctor Number");         scanf("%d", &d1.doctor_number);
    FILE *d_ptr = fopen("doctors.txt", "a"); // linux causes problems in binary file software, if linux is used, get it from the user and then write it to the file with fprintf
    if (d_ptr == NULL)
    {
    	printf("File could not be created! \n");
        return;
    }

    // fwrite(&d1, sizeof(doctor), 1,d_ptr) fwrite is used to print the complete contents of the address.
    fprintf(d_ptr, "%s %s %d %d\n", d1.name, d1.surname, d1.policlinic_number, d1.doctor_number);
    fclose(d_ptr);
    printf("%-20s\n", "Doctor Register Successful");
}

void list_doctors()
{
	system("clear");
	doctor d1;

	FILE *d_ptr = fopen("doctors.txt", "r");
	if (d_ptr == NULL) {
		printf("File could not be opened!\n");
		return;
	}

	printf("+-----------------+-----------------+-------------+------------+\n");
	printf("| %-15s | %-15s | %-11s | %-10s |\n", "Name", "Surname", "Clinic No", "Doctor No");
	printf("+-----------------+-----------------+-------------+------------+\n");

	while (fscanf(d_ptr, "%s %s %d %d", d1.name, d1.surname, &d1.policlinic_number, &d1.doctor_number) == 4) {
		printf("| %-15s | %-15s | %-11d | %-10d |\n",
			d1.name, d1.surname, d1.policlinic_number, d1.doctor_number);
	}

	printf("+-----------------+-----------------+-------------+------------+\n");
	fclose(d_ptr);
}



int menu () {
	int choice;
	printf("\nHOSPITAL AUTOMATION\n\n");
	printf("%-30s %d\n",   "For Patient Register",  1);
	printf("%-30s %d\n",   "For Doctor Login",      2);
	printf("%-30s %d\n",   "For Doctor Register",   3);
	printf("%-30s %d\n",   "For List Doctors",  	4);
	printf("%-30s %d\n\n", "For EXIT!",  			0);
	printf("%-20s", "Enter Your Choice");           scanf("%d", &choice);
	return choice;
}

int main() {
	int choice = menu();
	while(choice != 0)
	{
		switch(choice)
		{
		case 1: patient_register();   			break;
		case 2: doctor_login(); 				break;
		case 3: doctor_register();				break;
		case 4:	list_doctors();					break;
		default:printf("Invalid Request"); 		break;
		}
		choice = menu();
	}
	return 0;
}
