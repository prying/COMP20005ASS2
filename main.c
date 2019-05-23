/* Solution to comp20005 Assignment 2, 2019 semester 1.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: Flynn Harrison #add student number
   Dated:     15/05/2019

*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NO_VALID   	0	// No data
#define IS_VALID    1	// 
#define IN_VALID    2	// None valid data
#define CH_VALID	'Y'	// Valid char

#define MONTHS      12
#define START_YEARS 250
#define ADD_YEARS   50

#define MONTH_OFF	1 	// Array month offset	

#define STAGE_1 	'S1'
#define STAGE_2 	'S2'
#define STAGE_3 	'S3'

#define NO_DATA		"..."
#define IN_DATA		"*"

char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
// Could be done as defines 
enum months_e {na, Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep,
					Oct, Nov, Dec};

typedef struct
{
    __uint8_t valid;
    // Monthly Precipitation Total (millimetres)
    double rainfull;

}month_rainfull_t;

typedef struct
{
    int year;
    // needs better name
	month_rainfull_t month[MONTHS];

}year_rainfull_t;

int mygetchar();
int get_data(year_rainfull_t *data, int *site, size_t *size);
void s1_output(year_rainfull_t *data, int site, size_t size);

int main(int argc, char *argv[]) 
{
	int site = 0;
	size_t size = 0;	
	year_rainfull_t data[START_YEARS];
	// Clear all values
	memset(data, 0, sizeof(data));

	// S1
	get_data(data, &site, &size);
	s1_output(data, site, size);
}


int mygetchar() 
{
	int c;
	while ((c=getchar())=='\r');
	return c;
}

/* Passes the contontent of stdin into year_rainfull_t array*/
int get_data(year_rainfull_t *data, int *site, size_t *size)
{
	// Buffers
    int yb;
    int mb;
    double rb;
    char vb;
	int sb;

	bool first_run = true;
	*size = 0;

	// Eat first line
	while(mygetchar() != '\n');

	/* Should work without the \n at the end, but on debian (Linux mint 19.1)
	system it wont work without it, but works fine on Windows 10. 
	more testing is needed!*/
    while(scanf("IDCJAC0001,%d,%d,%d,%lf,%c\n", &sb, &yb, &mb, &rb, &vb) == 5)
    {
        // Special case, first run 
        if (first_run == true)
		{
			first_run = false;
			data[*size].year = yb;
			*site = sb;
		}
		
		// Increment year
        if (data[*size].year != yb)
        {
            *size += 1;
            data[*size].year = yb;

        }

		// Input month data into that year
		data[*size].month[mb- MONTH_OFF].rainfull = rb;
		if (vb == CH_VALID)
		{
			data[*size].month[mb-MONTH_OFF].valid = IS_VALID;
		}
		else
		{
			data[*size].month[mb-MONTH_OFF].valid = IN_VALID;
		}	
    }
	// Sucsess 
	return 1;
}

/* Outputs S1 */ 
/* STILL NEEDS FORMATTING!! */
void s1_output(year_rainfull_t *data, int site, size_t size)
{
	printf("S1, site number %d, %ld datalines in input\n", site, size);
	
	int i;
	for (i=0; i <= size; i++)
	{
		printf("S1, %d:", data[i].year);

		int j;
		for (j = 0; j< MONTHS; j++)
		{
			if (data[i].month[j].valid != NO_VALID)
			{
				printf("%5s", months[j]);
				if (data[i].month[j].valid == IN_VALID)
					printf("%s", IN_DATA);
			}
			else
			{
				printf(" %s", NO_DATA);
			}
			
		}
		puts("");
	}
}