/********************************************** Application Source ***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "Card.h"
#include "Application.h"
#include "Terminal.h"
#include "Server.h"
#pragma warning(disable : 4996) // DISABLE WARNING WHILE USING SPECIFIC FUNCTIONS

int main()
{
	/****************** BANK DATABASE ARRAY INITIALIZATION ******************/
	puts("****************** Server Side Accounts Database ********************\n");

	int i = 0, j = 0; char Buffer[] = "\0";
	
	for (i = 0; i < 255; i++)
	{
		BankDatabase[i].balance = 1000 * i; // SETTING ACCOUNTS BALANCE
		for (j = 0; j < 16; j++) // GENERATING RANDOM PANS
		{
			itoa((rand()%10),Buffer, 10); // rand()%10 GENERATES RANDOM 0 ~ 9 NUMBERS
			if(j == 0)
				strcpy(BankDatabase[i].primaryAccountNumber, Buffer);
			else
				strcat(BankDatabase[i].primaryAccountNumber, Buffer);
		}
		printf("Account[%3d]: PAN: %s & Balance: %.3f EGP\n", i + 1, BankDatabase[i].primaryAccountNumber, BankDatabase[i].balance);
	}
	puts("----------------------------------------------------------------------\n");


	/************************* PAYMENT APPLICATION *************************/
	int attempts = 1;
	char command = 'A';
	char newline;
	while (command != 'E' && command != 'e')
	{
		printf("*********************** Attempt Number: %2d ************************\n\n",attempts);
		appStart();
		printf("Please type Y to try again or E to exit: "); // ASK USER IF HE/SHE WANTS TO START THE APPLICATION AGAIN
		command = getchar(); // USER TYPICAL ANSWER FORMAT = %C \n (Character + Enter)
		newline = getchar(); // TO ABSORB THE '\n' PRODUCED WHEN ASKING FOR COMMAND (Flush stdin)
		attempts++;
		puts("----------------------------------------------------------------------\n");
	} 

	puts("Payment application terminated here are all done transactions: "); // PRINTING DONE TRANSACTIONS
	for (i = 0; i < 255; i++)
	{
		char* state = NULL;

		if (transactionDatabase[i].transactionSequenceNumber == 0) break; // BREAKING THE LOOP WHEN DONE WITH RECORDED TRANSACTIONS

		switch (transactionDatabase[i].transState)
		{
		case 0:
			state = "Approved"; break;

		case 1:
			state = "Declined Insuffecient Fund"; break; 

		case 2:
			state = "Declined Stoled Card"; break;	

		default:
			break;
		}

		printf("Transaction no. %2d:\n\tCard Holder's Name: %s\tCard Expiry Date: %s\tCard PAN Number: %s\n\tTransaction Date: %s\n\tTransaction amount: %f\n\tTransaction State: ",transactionDatabase[i].transactionSequenceNumber,transactionDatabase[i].cardHolderData.cardHolderName,
			transactionDatabase[i].cardHolderData.cardExpirationDate, transactionDatabase[i].cardHolderData.primaryAccountNumber, transactionDatabase[i].terminalData.transactionDate, transactionDatabase[i].terminalData.transAmount); printf("%s\n", state);

		puts("----------------------------------------------------------------------\n");
	}
		puts("Updated Bank Database: ");

		for(i = 0; i < 255; i++)
			printf("Account[%3d]: PAN: %s & Balance: %.3f EGP\n", i + 1, BankDatabase[i].primaryAccountNumber, BankDatabase[i].balance);
	
		system("PAUSE");
	return 0;
}

/*************************** APPLICATION TRIGGER FUNCTION ******************************/
void appStart(void)
{
	ST_transaction_t* transaction = NULL;

	transaction = (ST_transaction_t*)malloc(sizeof(ST_transaction_t)); // MEMORY ALLOCATION FOR TRANSACTION DATA

	if (transaction == NULL)
		printf("The machine is currently out of service, Please try again later!"); // WHEN MEMORY ALLOCATION FAILED!
	else
	{
		if (setMaxAmount(&(transaction->terminalData))) // RETURNS INVALID_MAX_AMOUNT = 6 OR GOOD = 0
		{
			printf("Invalid Max Amount!\n");
			return;
		}
		else
			if (getCardHolderName(&(transaction->cardHolderData))) // RETURNS WRONG_NAME = 1 OR OK = 0
			{
				printf("Invaild Card Holder's Name!\n");
				return;
			}
			else
				if (getCardExpiryDate(&(transaction->cardHolderData))) // RETURNS WRONG_EXP_DATE = 2 OR OK = 0
				{
					printf("Invaild Expiry Date format!\n");
					return;
				}
				else
					if (getCardPAN(&(transaction->cardHolderData))) // RETURNS WRONG_PAN = 3 OR OK = 0
					{
						printf("Invaild Primary Account Number!\n");
						return;
					}
					else
					{
						printf("Wellcome to Terminal Machine!\nToday's date: ");
						getTransactionDate(&(transaction->terminalData)); // SETTING terminal->transactionDate MEMBER TO TODAY'S DATE
						printf("%s\nPlease wait we are checking your card data...\n", transaction->terminalData.transactionDate);

						if (isCardExpired(&(transaction->cardHolderData), &(transaction->terminalData))) // RETURNS EXPIRED_CARD = 2 OR GOOD = 0
							printf("Your Card is Expired!, Please Insert a valid card...\n");
						else
							if (getTransactionAmount(&(transaction->terminalData))) // RETURNS INVALID_AMOUNT = 4 OR GOOD = 0
							{
								printf("Invalid Amount!\n");
								return;
							}
							else
								if (isBelowMaxAmount(&(transaction->terminalData))) // RETURNS EXCEED_MAX_AMOUNT = 5 OR GOOD = 0
								{
									printf("Invalid Transaction!, Requested amount exceeds the max transaction amount\n");
									return;
								}
								else
								{
									printf("Please wait we are working on your request...\n");
									
									switch (recieveTransactionData(transaction)) // RETURNS APPROVED = 0, DECLINED_INSUFFECIENT_FUND = 1, DECLINED_STOLEN_CARD = 2, INTERNAL_SERVER_ERROR = 3
									{
									case APPROVED:
										printf("Transaction approved and has been saved successfully ^ ^\n"); break;

									case DECLINED_INSUFFECIENT_FUND:
										printf("Transaction declined!, No enough balance in your account.\n"); break;

									case DECLINED_STOLEN_CARD:
										printf("Invalid Account!, This Account has been blocked.\n"); break;

									case INTERNAL_SERVER_ERROR:
										printf("Failed to Save the transaction!\n"); break;

									}
								}
					}
	}
}
