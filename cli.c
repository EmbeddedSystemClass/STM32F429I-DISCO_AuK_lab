#include "cli.h"
#include "usart.h"
#include <string.h>
#include <ctype.h>


// head of CLI command list
static CLI_CommandItem *head = NULL;
static CLI_CommandItem *element =NULL;

// char buffer where command will be stored
static char commandBuffer[100];

/**
 * This function searches the CLI command list and tries to find a descriptor for the provided command.
 * The command format is case-insensitive.
 * Returns pointer to @ref CLI_MenuItem structure if given command was found in the CLI command list.
 *
 * @param command pointer to command (string)
 *
 * @retval pointer to @ref CLI_MenuItem structure desrcibing the command, if command was found
 * @retval NULL if the given command does not match any command regitstered in the CLI command list 
 */
static CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command);

/**
 * @bref This function is responsible for collecting the command reading in from USART.
 *
 * This function should check wether the USART interface has some new data. If it does
 * this function reads new characters and stores them in a command buffer. This function
 * is also responsible for providing echo of the input characters back to the buffer.
 *
 * The function exits when:
 * - no more characters are available to read from USART - the function returns false
 * - new line was detected - this function returns true
 *
 * @retval true if command was collected
 * @retval false if command is yet incomplete
 */
static bool CLI_StoreCommand(void);

/**
 * @brief This function converts string to a lowercase
 *
 * @param dst pointer where converted null terminated string will be stored
 * @param src pointer to string which will be converted
 */
static void CLI_StringToLower(char *dst, const char *src);
	uint32_t a;
	
	
void CLI_Proc(void){
	if(CLI_StoreCommand()){
			if(commandBuffer[0] == '?')		CLI_PrintAllCommands();
					
			CLI_CommandItem *ptr = CLI_GetMenuItemByCommandName(commandBuffer);
			
			if(ptr != NULL)	{
				if (commandBuffer[5]=='o' && commandBuffer[6]=='n') 	{ USART_WriteString("LED'S ON\n\r"); ptr->callback("on");commandBuffer[5]= NULL; commandBuffer[6]=NULL;} 
				else 
				if (commandBuffer[5]=='o' && commandBuffer[6]=='f'&& commandBuffer[7]=='f'){USART_WriteString("LED'S OFF\n\r");	ptr->callback("off");	commandBuffer[5]= NULL; commandBuffer[6]=NULL;commandBuffer[7]= NULL;	}	
				else
				ptr->callback(NULL);
			}
			else	USART_WriteString("Wrong command\n\r");
				
	}
}

bool CLI_AddCommand(CLI_CommandItem *item){
	if(item->callback ==NULL)
	{
		return false;
	}
	else
	{
		if(head == NULL)
		{
			head = item;
			return true;
		}
		else
		{		element=head;
				while(element->next != NULL)
				{
					element=element->next;
				}
				element->next = item;
				return true;
			}		
	}	
	return false;
}

void CLI_PrintAllCommands(void){
	
	CLI_CommandItem *temp = head;
	
		while(temp != NULL){
			USART_WriteString("nazwa polecenia: \n\r");
			USART_WriteString(temp->commandName);
			USART_WriteString("\n\r");
			USART_WriteString("opis polecenia: \n\r");
			USART_WriteString(temp->description);
			USART_WriteString("\n\r");
			temp = temp->next;
			
	}
}

CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command){
	
	CLI_CommandItem *ptr = head;	//Ustawienie wskaznika na poczatek listy
	while(1){
		if (memcmp(ptr->commandName,command,4) == 0) 
		{return ptr;			
		}
		else if(ptr->next == NULL){			
			return NULL;			//jezeli to koniec listy zwroc NULL
		}
		else{								
			ptr = ptr->next;		//inaczej podmien ptr
		}
	}
	
	return NULL;
};

void CLI_StringToLower(char *dst, const char *src){
		//todo proszę wykorzystać funkcje z biblioteki ctype.h
	*dst=tolower(*src);//zakladajac ze dst jest wyjsciowym
}
bool CLI_StoreCommand(){
	static int index;	//Przechowujemy ststyczny index listy
	char temp;			
	if(USART_GetChar(&temp)){ //Sprawdza czy dostalismy cos przez USART
		USART_PutChar(temp);	//echo
		CLI_StringToLower(&temp,&temp);
		commandBuffer[index] = temp; //Dodanie do bofora
		if((commandBuffer[index] == '\r') | (index == 99)){	//Sprawdza czy to juz koniec komendy
			USART_PutChar('\n');
			index = 0;
			return true;
		}
		index++;
	}
	
	return false;
}
