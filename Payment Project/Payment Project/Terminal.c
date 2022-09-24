/************************************************* Terminal Source ******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include "Terminal.h"
#pragma warning(disable : 4996) // DISABLE WARNING WHILE USING SPECIFIC FUNCTIONS


/************************ GET TRANSACTION DATE FUNCTION ****************************/
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	int day = 0, month = 0, year = 0; // INTEGER FORMAT
	char day_s[3] = "0\0", month_s[3] = "0\0", year_s[5] = "0000"; // STRING FORMATE
	time_t t = time(NULL); struct tm* Current_Time = localtime(&t); //FETCHING DATE FROM HOST MACHINE


	/**************** GETTING DATE IN INTEGER FORMAT USING LOCAL TIME ***************/
	day = Current_Time->tm_mday; // TODAY
	month = (Current_Time->tm_mon) + 1; // CURRENT MONTH
	year = (Current_Time->tm_year) + 1900; // CURRENT YEAR



	/********* GETTING DATE IN STRING FORMAT USING itoa(NUMBER,STRING,BASE) *********/
	if (day > 9) // TODAY
	{
		itoa(day, day_s, 10);
	}
	else
	{
		itoa(day, &(day_s[1]), 10);
	}

	if (month > 9) // CURRENT MONTH
	{
		itoa(month, month_s, 10);
	}
	else
	{
		itoa(month, &(month_s[1]), 10);
	}

	itoa(year, year_s, 10); // CURRENT YEAR


	
/********************** GETTING DATE IN DD/MM/YYYY FORMAT ***************************/
	strcpy(termData->transactionDate, day_s); // Example : transactionDate = 14
	strcat(termData->transactionDate, "/"); // Example : transactionDate = 14/
	strcat(termData->transactionDate, month_s); // Example : transactionDate = 14/07
	strcat(termData->transactionDate, "/"); // Example : transactionDate = 14/07/
	strcat(termData->transactionDate, year_s); // Example : transactionDate = 14/07/2022

	return GOOD;
}

/*************************** IS CARD EXPIRED FUNCTION ******************************/
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
/************************** GETTING CARD EXPIRY DATE *******************************/
	int month_i = 0, year_i = 0;  // INPUT MONTH AND YEAR
	sscanf(cardData->cardExpirationDate, "%d/%d", &month_i, &year_i);
	year_i += 2000; // Example: CONVERTING 22 TO 2022

/************************** GETTING TRANSACTION DATE *******************************/
	int month_c = 0, year_c = 0;  // INPUT MONTH AND YEAR
	sscanf(termData->transactionDate, "%*d/%d/%d", &month_c, &year_c); // %*d USED TO SKIP THE DAY SCANING 

/************************** CHECKING CARD EXPIRY DATE ******************************/
	if (year_i >= year_c)
	{
		if (year_i == year_c)
		{
			if (month_i >= month_c)
			{
				return GOOD;
			}
			else
			{
				return EXPIRED_CARD;
			}
		}
		else
		{
			return GOOD;
		}
	}
	else
	{
		return EXPIRED_CARD;
	}

}

/************************ GET TRANSACTION AMOUNT FUNCTION **************************/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float UserInput = 0.0; char newline = 0;

	printf("Please Enter the transaction amount: "); // ASKING FOR TRANSACTION AMOUNT	
	scanf("%f",&(termData->transAmount)); // STORE THE TRANSACTION AMOUNT INTO TERMINAL DATA
	newline = getchar(); // ABSORBS '\n' CHARACTER LEFT FROM scanf()
	UserInput = termData->transAmount;

	if (UserInput <= 0) // TESTING INPUT VALIDATION
	{
		return INVALID_AMOUNT;
	}
	else
	{
		return GOOD;
	}
}

/***************************** SET MAX AMOUNT FUNCTION ******************************/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	float UserInput = 0.0; char newline = 0;

	printf("Please Enter the maximum allowed transaction amount for the terminal machine: "); // ASKING FOR MAX TRANSACTION AMOUNT	
	scanf("%f", &(termData->maxTransAmount)); // STORE THE MAX TRANSACTION AMOUNT INTO TERMINAL DATA
	newline = getchar(); // ABSORBS '\n' CHARACTER LEFT FROM scanf()
	UserInput = termData->maxTransAmount;

	if (UserInput <= 0) // TESTING INPUT VALIDATION
	{
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		printf("The maximum allowed transaction amount is: %.3f EGP\n", termData->maxTransAmount);
		puts("----------------------------------------------------------------------");
		return GOOD;
	}
}

/************************** IS BELOW MAX AMOUNT FUNCTION ***************************/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount) // TESTING TRANSACTION VALIDATION
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		return GOOD;
	}
}