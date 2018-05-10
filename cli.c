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
	if(CLI_StoreCommand()){
			if(commandBuffer[0] == '?')		CLI_PrintAllCommands();
					
			CLI_CommandItem *ptr = CLI_GetMenuItemByCommandName(commandBuffer);
			
			if(ptr != NULL)	{
				if (commandBuffer[5]=='o' && commandBuffer[6]=='n') 	{ USART_WriteString("LED'S ON"); ptr->callback("on");} 
				else 
				if (commandBuffer[5]=='o' && commandBuffer[6]=='f'&& commandBuffer[7]=='f'){USART_WriteString("LED'S OFF");	ptr->callback("off");		}	
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
	//todo
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

//bool CLI_StoreCommand(){
//	char sign=NULL;	//1znak
//	int i=0;
//	int length=0;
//	
//	while (sign!=13)
//		{	
//			if(USART_GetChar(&sign))
//			{
//				commandBuffer[i]=sign;
// 				USART_PutChar(sign);
//				
//				i++;
//			}
//			i=0;
//		}
//		
//		
//		
//		USART_WriteString("\n\r");
//			return true;
//		if (length ==1) 
//			{ i=0; return true;}
//		else 
//			{i=0;
//			return false;}
	
	//else return false;
		
//}

//#include "cli.h"
//#include "usart.h"
//#include <string.h>
//#include <ctype.h>

//// head of CLI command list
//static CLI_CommandItem *head = NULL;

//// char buffer where command will be stored
//static char commandBuffer[100];

///**
// * This function searches the CLI command list and tries to find a descriptor for the provided command.
// * The command format is case-insensitive.
// * Returns pointer to @ref CLI_MenuItem structure if given command was found in the CLI command list.
// *
// * @param command pointer to command (string)
// *
// * @retval pointer to @ref CLI_MenuItem structure desrcibing the command, if command was found
// * @retval NULL if the given command does not match any command regitstered in the CLI command list 
// */
//static CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command);

///**
// * @bref This function is responsible for collecting the command reading in from USART.
// *
// * This function should check wether the USART interface has some new data. If it does
// * this function reads new characters and stores them in a command buffer. This function
// * is also responsible for providing echo of the input characters back to the buffer.
// *
// * The function exits when:
// * - no more characters are available to read from USART - the function returns false
// * - new line was detected - this function returns true
// *
// * @retval true if command was collected
// * @retval false if command is yet incomplete
// */
//static bool CLI_StoreCommand(void);

///**
// * @brief This function converts string to a lowercase
// *
// * @param dst pointer where converted null terminated string will be stored
// * @param src pointer to string which will be converted
// */
//static void CLI_StringToLower(char *dst, const char *src);
//	
//	
//void CLI_Proc(void){
//	if(CLI_StoreCommand()){
//		CLI_CommandItem *ptr = CLI_GetMenuItemByCommandName(commandBuffer);
//		if(ptr != NULL){
//			ptr->callback(NULL);
//		}
//		else{
//			USART_WriteString("Wrong command\n\r");

//		}
//	}
//}

//bool CLI_AddCommand(CLI_CommandItem *item){
//	CLI_CommandItem *ptr = head;
//	
//	if(((*item->callback) != NULL ) & (strlen(item->commandName) != 0)){
//		if(head == NULL){	// Jezeli jest to pierwsza komenda to dodaje jako head
//			head = item;
//			return true;
//		}
//		else{				// Inaczej musimy przeleciec na koniec listy i dodac element
//			while (ptr->next != NULL){	//Dopoki *next jest rozny od NULL to podmieniamy go na nastepny
//				ptr = ptr->next;
//			}
//			item->next = NULL;	//Reczne ustawienie wskaznika w dodawanym el. na NULL
//			ptr->next = item;	//Dodanie el. na koniec listy
//			return true;
//		}
//	}
//	return false;
//}

//void CLI_PrintAllCommands(void){
//	CLI_CommandItem *ptr = head;	//Ustawienie wskaznika na poczatek listy
//	while(1){
//		USART_WriteString(ptr->commandName);	//Wyslanie nazwy komendy przez USART
//		USART_WriteString("\r\n");	//Nowa linia
//		if(ptr->next == NULL)		//Sprawdzenie czy to juz koniec listy
//			return;
//		else
//			ptr = ptr->next;		//Podmiana el. na nastepny
//	}
//}

//CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command){
//	CLI_CommandItem *ptr = head;	//Ustawienie wskaznika na poczatek listy
//	while(1){
//		if(*(ptr->commandName) == *command){	
//			return ptr;				//jezeli sie zgadza to zwroc wskaznik
//		}
//		else if(ptr->next == NULL){			
//			return NULL;			//jezeli to koniec listy zwroc NULL
//		}
//		else{								
//			ptr = ptr->next;		//inaczej podmien ptr
//		}
//	}
//};

//void CLI_StringToLower(char *dst, const char *src){
//	//todo proszę wykorzystać funkcje z biblioteki ctype.h
//	*dst = tolower(*src);
//}

//bool CLI_StoreCommand(){
//	static int index;	//Przechowujemy ststyczny index listy
//	char temp;			
//	if(USART_GetChar(&temp)){ //Sprawdza czy dostalismy cos przez USART
//		USART_PutChar(temp);	//echo
//		commandBuffer[index] = temp; //Dodanie do bofora
//		if((commandBuffer[index] == '\r') | (index == 99)){	//Sprawdza czy to juz koniec komendy
//			USART_PutChar('\n');
//			index = 0;
//			return true;
//		}
//		index++;
//	}
//	
//	return false;
//}

