#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#define MAX_ARG 200
#define MAX_LEN 1000 

void print_Path(){
	char tmp[MAX_ARG];
	getcwd(tmp, sizeof(tmp));	//c de path alma
	printf("%s", tmp);
}

int get_Command(char * str)
{
	char dummy[MAX_ARG];
 	print_Path();
	printf(" >> ");
	fgets(dummy,sizeof(dummy),stdin);
	if (strlen(dummy) > 0) {
		dummy[strlen(dummy)-1]=0;	//sondaki new line'ı sil	
		strcpy(str,dummy);		
		return 1;
	} 
	return 0;
}

void bosluk_Sil(char* str, char** commands)
{
	int i=1;
	char* token;
	token=strtok(str," ");
	commands[0]=token;
	while(token!=NULL && (i < MAX_ARG)) {
		char* token2;		
		token2=strtok(NULL," ");
		commands[i]=token2;
		i++;
	}
	commands[i]=NULL;
}

void execute_Commands(char** commands)
{
	pid_t pid;
	int status;
	if ((pid = fork()) < 0) {
		printf("Error while forking child.\n");
		return;
	} else if (pid == 0) {
		if (execvp(commands[0], commands) < 0) {
			printf("Error occured while executing command.\n");
		}
		exit(0);
	} else {	
		while(wait(&status) != pid);
		return;
	}
}


void function_cd(char * str){
	int is_Error_Occured=chdir(str);
	if(is_Error_Occured == -1)
		printf("Error occured while changing directory.\n");	
}

void function_help(char ** commands){
	printf("\n\t\t  Shell Help\t\n");
	commands[0]="ls";
	commands[1]="/bin";

	execute_Commands(commands);
	printf("\n You can use commands above which are in /bin directory in my shell.And except those you can also use exit,cd,help.\n\n");

}

void function_l(char ** commands){
	commands[0]="ls";
	commands[1]="-CF";
	execute_Commands(commands);

}

int main(){
	char str[MAX_LEN],**commands,*name;
	int i,flag=1,flag2;
	name=getenv("USER");
	commands=(char **)calloc(MAX_ARG,sizeof(char *));
	if(commands == NULL){
		printf("Allocation problem , stopping program.");
		return -1;	
	}
	for(i=0;i<MAX_ARG;i++){
		commands[i]=(char *)calloc(MAX_LEN,sizeof(char));
		if(commands[i]==NULL){
			printf("Allocation problem , stopping program.");
			return -1;	
		}
	}
	printf("Welcome to my Shell %s. Please write exit if you want to exit.\n",name);	
	while(flag){
		flag2=get_Command(str);		
		if(flag2){
			bosluk_Sil(str,commands);
			if(!strcmp(commands[0],"exit"))
				exit(0);
			else if(!strcmp(commands[0],"clear")){
				execute_Commands(commands);				
				printf("Please write exit if you want to exit.\n");
			}
			else if(!strcmp(commands[0],"cd"))
				function_cd(commands[1]);
			else if(!strcmp(commands[0],"help")){			
				function_help(commands);
			}
			else if(!strcmp(commands[0],"l")){
				function_l(commands);
			}
			else			
				execute_Commands(commands);			
		}
	}
	printf("Good Bye.\n");
}
