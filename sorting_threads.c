#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

//global variables
int sizeA;
int array[1000];
int arrayCopy[1000];
int i=0, j=0, k=0;
int indicesOriginal[2]; //indicesOriginal[0] is left and indicesOriginal[1] is right

//declration of functions used
void fileReader(char *filename);
void fileWriter(char *filename);
void merge(int left, int right, int middle);
void mergeSort(int left, int right);
void *threadMerge(void *arg);

//main
int main()
{
char fileinput[255] ;
char *fileOutput = "outputSort.txt";

printf("Please enter the filename: ");
scanf("%s",fileinput);

//reading the unsorted array from input file
fileReader(fileinput);

//printing size of array after being read
printf("Array size: %d\n",sizeA);
//printing the array after being read
printf("Original array: ");
for(i=0;i<sizeA;i++)
printf("%d ",array[i]);
printf("\n");

//assigning the left and right indices
indicesOriginal[0] = 0;
indicesOriginal[1] = sizeA-1;

//creating a thread for the original array
pthread_t originalThread;
pthread_create(&originalThread, NULL, &threadMerge, &indicesOriginal);
pthread_join(originalThread, NULL);

//printing array after being sorted
printf("Sorted array: ");
for(i=0;i<sizeA;i++)
printf("%d ",array[i]);
printf("\n");

//writing the sorted array to output file
fileWriter(fileOutput);

return 0;
}

//reading the input file function
void fileReader(char *filename)
{
FILE * fpointer;

fpointer = fopen(filename, "r");

//ensures that the file exists
if(fpointer == NULL)
{
printf("File not found!!\n");
exit(1);
}

//reading the size of array from file
fscanf(fpointer, "%d", &sizeA);
//reading array elements from file
for(i=0;i<sizeA;i++)
{
fscanf(fpointer, "%d", &array[i]);
}

fclose(fpointer);
}

//writing to file function
void fileWriter(char *filename)
{
FILE* fp;
fp = fopen(filename,"w");

fputs("Sorted array\n",fp);
for(i=0;i<sizeA;i++)
fprintf(fp, "%d ",array[i]);
fprintf(fp, "\n");

fclose(fp);
}

//unthreaded merge sort as a reference but not used
void mergeSort(int left, int right)
{
     if (left < right)
     {
     int middle = (left + (right-1))/2;

     mergeSort(left, middle);
     mergeSort(middle + 1, right);
     merge(left, right, middle);
     }
}

//merging all the subarrays together
void merge(int left, int right, int middle)
{
        int size1 = (middle - left) +1;
        int size2 = right - middle;

        int array1[size1];
        int array2[size2];

        for(i=0;i<size1;i++)
        {
        array1[i] = array[left + i];
        }

        for(j=0;j<size2;j++)
        {
        array2[j] = array[middle + 1 +j];
        }

        i=0;
        j=0;
        k=left;

        while(i<size1 && j<size2)
        {
        if(array1[i] <= array2[j])
        {
        array[k] = array1[i];
        i++;
        }
        else {
        array[k] = array2[j];
        j++;
        }
        k++;
        }

        while(i<size1)
        {
        array[k] = array1[i];
        i++;
        k++;
        }

        while(j<size2)
        {
        array[k] = array2[j];
        j++;
        k++;
        }
}

//threaded merge sort function
void *threadMerge(void *arg)
{
      //extracting the values from arg
      int *argsIndices = arg;

      int left = argsIndices[0];
      int right = argsIndices[1];

      if(left<right)
      {
          //calculating middle index to divide the array
          int middle = (left + (right-1))/2;

          //creating two arrays for left and right indices of subarrays
          int indicesSub1[2];
          int indicesSub2[2];

          //assigning the left and right indices
          indicesSub1[0] = left;
          indicesSub1[1] = middle;

          indicesSub2[0] = middle + 1;
          indicesSub2[1] = right;

          //creating a thread for the first subarray
          pthread_t th1;
          pthread_create(&th1, NULL, &threadMerge, &indicesSub1);
          pthread_join(th1, NULL);

          //creating a thread for the second subarray
          pthread_t th2;
          pthread_create(&th2, NULL, &threadMerge, &indicesSub2);
          pthread_join(th2, NULL);

          //merging all the subarrays together
          merge(left, right, middle);
      }
}
