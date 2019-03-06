/*-------------------------------------------------------*/
/* Ruthann Biel.                                         */
/* LAB 5, function that "returns" more than one value    */
/* Given the sides of a triangle, find the radius of     */
/* circles, one inscribed in a triangle, and the other   */
/* circumscribed about a triangle.                       */

#include "lab5.h"

int main(void)
{
    double a, b, c;		/* sides of the triangle */
    double radius_inside;	/* radius of the inside circle */	
    double radius_outside;	/* radius of the outside circle */

    FILE * data_in;      /* input file pointer */
    FILE * data_out;     /* output file pointer */


	/* Open the two required files */
    data_in = fopen(IN_FILE, "r");
    if (data_in == NULL)
    {
       printf("Error on fopen file %s \n", IN_FILE);
       exit(EXIT_FAILURE);
    }

    data_out = fopen(OUT_FILE, "w");
    if (data_out == NULL)
    {
       printf("Error on fopen file %s \n", OUT_FILE);
       exit(EXIT_FAILURE);
    }

    /* Print headers */
    fprintf(data_out, "\nYour name here.  Lab 5. \n\n");
    fprintf(data_out, "   Triangle Sides      Radius-Of-Circle    \n");
    fprintf(data_out, "   A     B     C       Inside   Outside    \n");
    fprintf(data_out, "--------------------   ------   -------    \n");

	/* Loop thru the values to compute the two radii  */
    while ((fscanf(data_in, "%lf%lf%lf", &a, &b, &c))== 3)
    {
	find_two_radii(a, b, c, &radius_inside, &radius_outside);

        fprintf(data_out,"%5.2f  %5.2f  %5.2lf %8.2f  %8.2f \n",
                a, b, c, radius_inside, radius_outside);
    }
    fprintf(data_out,"\n");
    fclose(data_in);
    fclose(data_out);
    return EXIT_SUCCESS;
}
/*-----------------------------------------------------------*/
