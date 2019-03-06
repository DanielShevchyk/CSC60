/* Daniel Shevchyk                                 */
/* Lab 4                                           */
/* Figure the area of the top of a cylinder        */
/* and the volume of a cylinder                    */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#define IN  "lab4.dat"
//#define IN  "lab4sample.dat"
//#define OUT "lab4.out"

int main(void)
{
        char* inputFileName = "lab4.dat";
        char* outputFileName = "lab4.out";
        
        FILE* IN;
        FILE* OUT;

        double radius, height, toparea, volume;
        
        IN = fopen(inputFileName, "r");
                if(IN == NULL)
                {
                   printf("Error on opening the input file \n");
	           exit(EXIT_FAILURE);
                }
        OUT = fopen(outputFileName, "w");
                if(OUT == NULL)      
                {
                   printf("Error on opening the output file \n");
                   exit(EXIT_FAILURE);
                }
        fprintf(OUT, "\nDaniel Shevchyk. Lab4. \n");
        int count = 1;
        while(fscanf(IN, "%lf%lf", &radius, &height)==2)
        {
                toparea = M_PI * radius * radius;
                volume = M_PI * radius * radius * height;
                fprintf(OUT, "\nCylinder %1d", count);
	        fprintf(OUT, "\nThe radius is:    %9.3f", radius);
	        fprintf(OUT, "\nThe height is:    %9.3f", height);
	        fprintf(OUT, "\nThe top area is:  %9.3f", toparea);
	        fprintf(OUT, "\nThe volume is:    %9.3f\n", volume);
                count++;
        }
        fclose(IN);
        fclose(OUT);

        return EXIT_SUCCESS;

}
