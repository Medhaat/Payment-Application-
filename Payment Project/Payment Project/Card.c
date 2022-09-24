/************************************************* Card Source ******************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "Card.h"
#pragma warning(disable : 4996) // DISABLE WARNING WHILE USING SPECIFIC FUNCTIONS


/*************************** CARD HOLDER NAME FUNCTION ******************************/
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	char* UserInput = NULL; // USED TO VALIDATE USER INPUT

	printf("Please Enter Your Card Holder's Name: "); // ASKING FOR CARD HOLDER'S NAME
	fgets(cardData->cardHolderName,4*sizeof(cardData->cardHolderName), stdin); // STORE THE NAME INTO CARD DATA
	UserInput = cardData->cardHolderName;

	if ((strlen(UserInput) >= 25) || (strlen(UserInput) < 20)) // TESTING INPUT LENGTH
	{
		return WRONG_NAME;
	}
	else
	{
		return OK;
	}
}

/*************************** CARD EXPIRY DATE FUNCTION ******************************/
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	char* UserInput = NULL; int FormatTest = 0;
	int month_i = 0, year_i = 0; char MidChar_i = 0; // INPUT MONTH AND YEAR

	printf("Please Enter Card Expiry Date in ""MM/YY"" format: "); // ASKING FOR CARD EXPIRY DATE
	fgets(cardData->cardExpirationDate, 4*sizeof(cardData->cardExpirationDate), stdin); // STORE THE DATE INTO CARD DATA
	UserInput = cardData->cardExpirationDate;

	if (strlen(UserInput) != 6) // TESTING INPUT LENGHT
	{
		return WRONG_EXP_DATE;
	}

	FormatTest = sscanf(UserInput, "%d%c%d", &month_i, &MidChar_i, &year_i); // RETURNS 3 WHEN GETTING THE 3 VALUES

	if ((FormatTest == 3) && (MidChar_i == '/')) // TESTING EXPIRY DATE & INPUT FORMAT
	{
		return OK;
	}
	else
	{
		return WRONG_EXP_DATE;
	}
}

/*************************** CARD PAN NUMBER FUNCTION *******************************/
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	char* UserInput = NULL; int i = 0;

	for (int j = 0; j < sizeof(cardData->primaryAccountNumber); j++) // INITIALIZES PAN WITH '\0'
		cardData->primaryAccountNumber[j] = '\0';

	printf("Please Enter Your Card's Primary Account Number: "); // ASKING FOR CARD PRIMARY NUMBER
	fgets(cardData->primaryAccountNumber, 4 * sizeof(cardData->primaryAccountNumber), stdin); // STORE THE NUMBER INTO CARD DATA
	UserInput = cardData->primaryAccountNumber;

	if ((strlen(UserInput) > 20) || (strlen(UserInput) < 17)) // TESTING INPUT LENGTH
	{
		return WRONG_PAN;
	}
	else
	{
		while (*(UserInput+i) != '\n') // TESTING INPUT FORMAT (Alphanumeric Only)
		{
			if(isalpha(*(UserInput + i)))
				return WRONG_PAN;
			i++;
		}
		*(UserInput + i) = '\0'; // REMOVES '\n' IN THE END OF THE STRING
		return OK;
	}
}