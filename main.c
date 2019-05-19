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

#define NO_VALID   	0	// No month
#define IS_VALID    1	// 
#define IN_VALID    2	// None valid data
#define CH_VALID	'Y'	// Valid char

#define MONTHS      12
#define START_YEARS 250
#define ADD_YEARS   50

#define MONTH_OFF	1 	// Array month offset		

char *months[] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
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

int main(int argc, char *argv[]) 
{
    int current_years = START_YEARS;
	
    // Buffers
    int yb;
    int mb;
    double rb;
    char vb;
	int d;
	bool first_run = true;

	size_t size = 0;	
	year_rainfull_t data[current_years];
	// Clear all values
	memset(data, 0, sizeof(data));

	// Eat first line
	while(mygetchar() != '\n');
	
    while(scanf("IDCJAC0001,%d,%d,%d,%lf,%c\n", &d, &yb, &mb, &rb, &vb) == 5)
    {
        // Special case, first run 
        if (first_run == true)
		{
			puts("first run");
			first_run = false;
			data[size].year = yb;
		}
		
		// Increment year
        if (data[size].year != yb)
        {
			puts("year increment");
            size++;
            data[size].year = yb;
        }
		puts("spam");
		printf("%d, %d \n", d, yb);
		// Input month data into that year
		data[size].month[mb- MONTH_OFF].rainfull = rb;
		if (vb == CH_VALID)
		{
			data[size].month[mb-MONTH_OFF].rainfull = IS_VALID;
		}
		else
		{
			data[size].month[mb-MONTH_OFF].rainfull = IN_VALID;
		}	
    }

	printf("%ld \n", size);
	
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%d ", data[i].year);
		int j;
		for (j = 0; j < MONTHS; j++)
		{
			if (data[i].month[j].valid != NO_VALID)
			{
				printf("%s", months[j+1]);
				
				if (data[i].month[j].valid != IS_VALID)
				{
					printf("* ");
				}
				else
				{
					printf(" ");
				}
			}
		}
		puts("");
	}
	return 0;
}


int mygetchar() 
{
	int c;
	while ((c=getchar())=='\r');
	return c;
}