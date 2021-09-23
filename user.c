/*user program*/
/*Shelly Revivo 315661884 
 * Tomer Revivo 204470892 
 * */
 
 /*   Copyright :    https://stackoverflow.com/questions/3501338/c-read-file-line-by-line      */
/* The user program show us the file with the keys pressed - the file is on the /sys because we choose to use the kobject */
	
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

//colors on the printf 
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */



int main(){


  FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/sys/kernel/kobject_example/foo", "r");

    if (fp == NULL){
        exit(EXIT_FAILURE);
	}
       
	printf(YELLOW "********************************************************************* \n "RESET);
	printf(YELLOW "What would you like to do ?  \n 1) Show the file with the keys pressed ! \n 2) Show us the menual of this project ! : \n press on 1 or 2  : \n  "RESET);
	printf(YELLOW "********************************************************************* \n "RESET);
	char scan; 
	 scanf("%s" , &scan);  //read from user 
	
	
	//show the file on sysfs - with the keys pressed 
	if (scan == '1') 
	{
			printf(YELLOW "********************************************************************* \n "RESET);
			printf(YELLOW "The file on /sys/kernel/kobject_example/ that save all the keys pressed : \n "RESET);
			printf(YELLOW "********************************************************************* \n "RESET);
			
			  while ((read = getline(&line, &len, fp)) != -1) 
			  {
        
				printf("%s", line);
			}

		fclose(fp);
		if (line)
			free(line);
		exit(EXIT_SUCCESS);
	
	}
	
	//show the menual of the project ' how you need to run this ' all the commands line . 
	else if(scan =='2')
	{
		printf(RED "********************************************************************* \n "RESET);
		printf(RED "Manual : \n" RESET);
		printf(RED "********************************************************************* \n "RESET);
		printf( "run this commands one after one : \n" );
		printf( "--->   make \n" );
		printf( "--->  sudo insmod my_kobject.ko \n" );
		printf("--->   gcc user.c -o user_app \n");
		printf("--->   ./user_app\n");
		printf("--->   1\n");
		printf("now you watching on the file with all the key pressed . \n");
		
		
		printf("If you want to do this again you have to remove the module from lsmod by the cammand : \n -->  sudo rmmod my_project \nand after run all this command again \n  ");
		
		
	}
	else 
	{
	printf("error - pls choose 1 or 2 ");
	}
	


}




