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
	
	
	
void CLI_Proc(void){
	CLI_StoreCommand(); //CYKLICZNIE
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
		element=head;
		USART_WriteString(element->commandName);		//if 
	do 
	{	element=element->next;	
		USART_WriteString(element->commandName);	
	}	
	while (element->next !=NULL);
}

CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command){
	
<<<<<<< HEAD
	CLI_CommandItem* xoxo;
	
	while (head!=NULL)
	{
		if (head->commandName ==command) 
		{xoxo->commandName=head->commandName;
			xoxo->callback=head->callback;
			xoxo->description=head->description;
			xoxo->next=head->next;			
		}return xoxo;	
	}
=======
>>>>>>> 9dd82ba2f879500a287fcdaf4b76b0320d40ae78
	
	return NULL;
};

void CLI_StringToLower(char *dst, const char *src){
	//todo proszę wykorzystać funkcje z biblioteki ctype.h
}
<<<<<<< HEAD


bool CLI_StoreCommand(){
	char sign=NULL;	//1znak
	int i=0;
	int length=0;
	
	while (sign!=13)
		{	
			if(USART_GetChar(&sign))
			{
				commandBuffer[i]=sign;
				length = USART_WriteString(&commandBuffer[i]);
				i++;
			}
			i=0;
		}		
	
		if (length ==i-1) { i=0; return true;}
		else i=0;
			return false;
	
	//else return false;
		
=======
int o=0;
bool CLI_StoreCommand(){
	char sign;	//1znak
	
	if (o>10)
		USART_WriteString(commandBuffer);
	if (USART_GetChar(&sign)) 
	{	commandBuffer[o]=sign;
		//USART_PutChar(sign);
	
		USART_WriteString(&commandBuffer[o]);
		o++;
		return true;
	}
	
	
	else return false;
>>>>>>> 9dd82ba2f879500a287fcdaf4b76b0320d40ae78
}
