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

   Signed by: Flynn Harrison 992559
   Dated:     15/05/2019

*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define NO_VALID	0	// No data
#define IS_VALID	1	// 
#define IN_VALID	2	// None valid data
#define CH_VALID	'Y'	// Valid char

#define MONTHS		12
#define START_YEARS	250
#define ADD_YEARS	50

#define MONTH_OFF	1 	// Array month offset	

#define NO_DATA		"..."
#define IN_DATA		"*"

#define KEN_SUMI	2 	// starting point for sum
#define KEN_SUMJ	1
// Graph settings
#define Y_AXIS		24	
#define COL_WIDTH	4
#define YEAR_C_OFF	2

char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

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

typedef struct
{
	double rainfull;
	int months;
	int min_y;
	int max_y;
}month_avrage_t;

int mygetchar();
int delta_tau(double r_i, double r_j);
int get_data(year_rainfull_t *data, size_t *size, int *site);
int month_avrage(year_rainfull_t *data, size_t size, int m, double *avg_rain);
void store_month_avrage(year_rainfull_t *data, size_t size,
		 month_avrage_t *avrg_rainfull);
void yty_month_is_data(year_rainfull_t *data, size_t size,
		 int *min_y, int *max_y, int m);
void draw_graph(year_rainfull_t *data, size_t size, 
		month_avrage_t *avrg_rainfull, int scale_fac, 
		size_t year_pos, char *tile);
void s1_output(year_rainfull_t *data, int site, size_t size, int lines);
void s2_output(year_rainfull_t *data, size_t size,
		 month_avrage_t *avrg_rainfull);
void s3_output(year_rainfull_t *data, size_t size,
		 month_avrage_t *avrg_rainfull);
void s4_output(year_rainfull_t *data, size_t size,
		 month_avrage_t *avrg_rainfull, char *year);
double kendall_tau(year_rainfull_t *data, size_t size,
		 month_avrage_t *avrg_rainfull, int m);
size_t year_serch(year_rainfull_t *data, size_t size, int year);
double max_rainfull(year_rainfull_t *data, size_t size, size_t year_pos);


int main(int argc, char *argv[]) 
{
	int site	= 0;
	size_t size = 0;
	int lines 	= 0;	
	int i 		= 0;
	month_avrage_t avrg_rainfull[MONTHS];
	year_rainfull_t data[START_YEARS];
	// Clear all values
	memset(data, 0, sizeof(data));

	
	// S1
	lines = get_data(data, &size, &site);
	s1_output(data, site, size, lines);

	// S2
	store_month_avrage(data, size, avrg_rainfull);
	s2_output(data, size, avrg_rainfull);

	// S3
	s3_output(data, size, avrg_rainfull);

	// S4
	// itterate through the arguments 
	for(i = 1; i<argc; i++)
	{
		s4_output(data, size, avrg_rainfull, argv[i]);
	}

	// For fun (idk, if your curious these are part of VT100 control set)
	printf("\033[31mT\033[35ma \033[33md\033[32ma\033[36ma\033[34m!");
	return 1;
}


int mygetchar() 
{
	int c;
	while ((c=getchar())=='\r');
	return c;
}

/* Passes the contontent of stdin into year_rainfull_t array*/
int get_data(year_rainfull_t *data, size_t *size, int *site)
{
	int lines 		= 0;
	bool first_run 	= true;
	// Buffers
	int yb;
	int mb;
	int sb;
	char vb;
	double rb;
	
	



	// Eat first line
	while(mygetchar() != '\n');

	/* Should work without the \n at the end, but on debian (Linux mint 19.1)
	system it wont work without it, but works fine on Windows 10. 
	more testing is needed!*/
	while(scanf("IDCJAC0001,%d,%d,%d,%lf,%c\n", &sb, &yb, &mb, &rb, &vb) == 5)
	{
		lines++;
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

	*size +=1;
	// return lines read
	return lines;
}

/* Outputs S1 */ 
void s1_output(year_rainfull_t *data, int site, size_t size, int lines)
{
	printf("S1, site number %06d, %d datalines in input\n", site, lines);
	
	// Itterate though each year and print the months
	int i;
	for (i=0; i < size; i++)
	{
		printf("S1, %d:", data[i].year);

		int j;
		for (j = 0; j< MONTHS; j++)
		{
			// Check if what needs to be printed
			if (data[i].month[j].valid != NO_VALID)
			{
				printf("%4s", months[j]);
				if (data[i].month[j].valid == IN_VALID)
					printf("%s", IN_DATA);
				else
					putchar(' ');
				
			}
			else
			{
				printf(" %s ", NO_DATA);
			}
			
		}
		puts("");
	}
	puts("");
}

/* Finds min and max years for the month having data, regadless if valid */
void yty_month_is_data(year_rainfull_t *data, size_t size, 
			int *min_y, int *max_y, int m)
{
	// Get lowest month 
	int i;
	for (i = 0; i < size; i++)
	{
		if (data[i].month[m].valid != NO_VALID)
		{
			*min_y = data[i].year;
			break;
		}
	}

	// Check for max
	for (i = 0; i < size; i++)
	{
		if (data[i].month[m].valid != NO_VALID)
		{
			*max_y = data[i].year;
		}
	}
}

/* Finds the sum of the months rainfull, include invalid data.
returns number of months sumed */
int month_avrage(year_rainfull_t *data, size_t size, int m, double *avg_rain)
{
	int months = 0;
	
	int i;
	for (i = 0; i < size; i++)
	{
		// Check each month to see its validity first
		if(data[i].month[m].valid != NO_VALID)
		{
			months++;
			*avg_rain += data[i].month[m].rainfull;
		}
	}
	// Find the avrage 
	*avg_rain = *avg_rain/(double)months;
	return months;
}

/* Gets and stores the avrage rainfull for each month into a stuct */
void store_month_avrage(year_rainfull_t *data, size_t size,
			month_avrage_t *avrg_rainfull)
{
	int i;
	for(i = 0; i < MONTHS; i++)
	{
		// For each month fill month_avrage_t with rainfull related data
		avrg_rainfull[i].months = month_avrage(data, size, i, 
			&(avrg_rainfull[i].rainfull));
		// Get the years this data set is for
		yty_month_is_data(data, size, &(avrg_rainfull[i].min_y), 
			&(avrg_rainfull[i].max_y), i);
	}
}

/* Prints the S2 outputs */
void s2_output(year_rainfull_t *data, size_t size, 
			month_avrage_t *avrg_rainfull)
{
	int i;
	for (i = 0; i < MONTHS; i++)
	{
		// Check if at least one data point available before printing
		if (avrg_rainfull[i].months > 0)
		{
			printf("S2, %s, %2d values, %d-%d, mean of %5.1lfmm\n", 
			months[i], avrg_rainfull[i].months, avrg_rainfull[i].min_y,
			avrg_rainfull[i].max_y, avrg_rainfull[i].rainfull);
		}
		else 
		{
			printf("S2, %s, %2d values\n", 
			months[i], avrg_rainfull[i].months);
		}
	}
	puts("");
}

int delta_tau(double r_i, double r_j)
{
	if (r_i < r_j)
		return 1;
	if (r_i > r_j)
		return -1;
	if (r_i == r_j)
		return 0;
	// redundancy 
	return 0;
}

double kendall_tau(year_rainfull_t *data, size_t size,
			month_avrage_t *avrg_rainfull, int m)
{
    int sum = 0;
	// make an array of all exiting values for month
	// as it makes the math easier to think about
	double rain[avrg_rainfull[m].months];
	int j = 0;

	// Find r_i and r_j
	int i;
	for (i = 0; i <size; i++)
	{
		// Check each month to see its validity first
		if(data[i].month[m].valid != NO_VALID)
		{
			rain[j] = data[i].month[m].rainfull;
			j++;
		}
	}
	// Compute sum of delta_tau
	for (i = 0; i<=avrg_rainfull[m].months - KEN_SUMI; i++)
	{
		int n;
		for (n = i+1; n<=avrg_rainfull[m].months - KEN_SUMJ; n++)
		{
			sum += delta_tau(rain[i], rain[n]);
		}
	}

	return (2.0/((double)j*((double)j-1.0)))*sum;
} 

/* Prints the S3 outputs */
void s3_output(year_rainfull_t *data, size_t size,
			month_avrage_t *avrg_rainfull)
{
	int i;
	for (i = 0; i < MONTHS; i++)
	{
		// Check if at least two data point available before printing
		if (avrg_rainfull[i].months >= 2)
		{
			printf("S3, %s, %2d values, %d-%d, tau of %5.2lf\n", 
			months[i], avrg_rainfull[i].months, avrg_rainfull[i].min_y,
			avrg_rainfull[i].max_y, 
			kendall_tau(data, size, avrg_rainfull, i));
		}
		else 
		{
			printf("S3, %s, %2d values\n", 
			months[i], avrg_rainfull[i].months);
		}
	}
	puts("");
}

/* Prints graph of year */
void s4_output(year_rainfull_t *data, size_t size,
			month_avrage_t *avrg_rainfull, char *year)
{
	double max_rf 	= 0;
	size_t year_pos = 0;
	int scale_fac 	= 0;
	/* Create a tile from the last 2 digits of the year. i doubt this code
	will be used in a little under 8000 years*/
	char *graph_tile = year+2;
	// Find the largest value for 'year'
	year_pos = year_serch(data, size, atoi(year));
	max_rf = max_rainfull(data,size,year_pos);
	scale_fac = ceil(max_rf/Y_AXIS);

	printf("S4, %s max is %5.1lf, scale is %d\n", year, max_rf, scale_fac);

	// Draw graph
	draw_graph(data, size, avrg_rainfull, scale_fac, year_pos, graph_tile);
	puts("");
}

/* Returns the element that hold the year */
size_t year_serch(year_rainfull_t *data, size_t size, int year)
{
	// Take a guess on where the year is located
	int delta = year - data[0].year;
	while(delta >= 0 && delta <= size)
	{
		if(data[delta].year == year)
			return delta;
		if(data[delta].year > year)
			delta--;
		if(data[delta].year < year)
			delta++;
	}
	// If fails will return 0, might change to an assert 
	// or return -1 and have a check when calling
	return 0;
}
/* Finds the largest rainfull in the year */
double max_rainfull(year_rainfull_t *data, size_t size, size_t year_pos)
{
	double max = 0;

	// Cycle through months and only keep largest value
	int i;
	for(i = 0; i<MONTHS; i++)
	{
		if (data[year_pos].month[i].rainfull > max)
			max = data[year_pos].month[i].rainfull;
	}
	return max;
}

/* draws a graph of max hight Y_AXIS, with a 2 character string as a tile.
 Year is selected uing the year_pos (returned by year_serch())*/
void draw_graph(year_rainfull_t *data, size_t size,
			month_avrage_t *avrg_rainfull, int scale_fac,
			size_t year_pos, char *tile)
{
	// Draw graph from top to bottom
	double max = max_rainfull(data, size, year_pos);
	int i;
	for (i = Y_AXIS; i>0; i--)
	{
		// Remove empty rows from the top
		while(i > ceil(max/scale_fac) && i > 0)
		{
			i--;
		}
		// Draw y axis
		printf("%4d | ", i*scale_fac);
		int j;
		for (j = 0; j<MONTHS; j++)
		{
			// Check avrg first
			if (ceil(avrg_rainfull[j].rainfull/scale_fac) == i)
			{
				putchar('*');
				// Check if month needs a tile drawn
				if(ceil(data[year_pos].month[j].rainfull/scale_fac) >= i)
					printf("%s", tile);
				else
					printf("**");
				putchar('*');
			}
			else
			{
				if(ceil(data[year_pos].month[j].rainfull/scale_fac) >= i)
					printf(" %s ", tile);	
				else // Empty tile
					printf("    ");
			} 
			// Add extra space for the + colum
			putchar(' ');
		}
		puts("");
	}

	// Draw base
	printf("%4d +-",0);
	for(i = 0; i<MONTHS; i++)
	{
		printf("----+");
	}
	// \n followed by 7 spaces
	printf("\n       ");
	for (i = 0; i<MONTHS; i++)
	{
		printf("%s  ", months[i]);
	}
	puts("");
}

/* yet another fun assigment, i learnt from this one 
the power of pre planning my assigment, as it spead 
up development time very nicely :D*/