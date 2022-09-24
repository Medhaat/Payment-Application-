/************************************************* Server Source ******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include "Server.h"
#pragma warning(disable : 4996) // DISABLE WARNING WHILE USING SPECIFIC FUNCTIONS

ST_accountsDB_t* UserAccount = NULL;// GLOBAL POINTER USED TO FLAG A SPECIFIC ACCOUNT INSIIDE THE BankDatabase[255]

/************************ REVIEVE TRANSACTION FUNCTION ****************************/
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	if (isValidAccount(&(transData->cardHolderData)) == ACCOUNT_NOT_FOUND) // CHECK IF THE ACCOUNT IS BLOCKED
	{
		transData->transState = DECLINED_STOLEN_CARD; // CHANGE THE STATE
		if (saveTransaction(transData) == SAVING_FAILED) { return INTERNAL_SERVER_ERROR; } // IF SAVING FAILED 
		return DECLINED_STOLEN_CARD; // IF SAVED SUCCESSFULLY
	}

	else if (isAmountAvailable(&(transData->terminalData)) == LOW_BALANCE) // CHECK IF BALANCE IS INSUFFECIENT
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND; // CHANGE THE STATE
		if (saveTransaction(transData) == SAVING_FAILED) { return INTERNAL_SERVER_ERROR; } // IF SAVING FAILED
		return DECLINED_INSUFFECIENT_FUND; // IF SAVED SUCCESSFULLY
	}
	
	else
	{
		transData->transState = APPROVED; // IF ALL CHECKS ABOVED ARE PASSED THE TRANSACTION IS APPROVED
		UserAccount->balance = UserAccount->balance - transData->terminalData.transAmount; // UPDATING THE BALANCE
		if (saveTransaction(transData) == SAVING_FAILED) { return INTERNAL_SERVER_ERROR; } // IF SAVING FAILED
		return  APPROVED;
	}
}

/************************* IS VALID ACCOUNT FUNCTION ******************************/
EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	int i = 0;
	for (i = 0; i < 255; i++)
	{
		if (strcmp(cardData->primaryAccountNumber, BankDatabase[i].primaryAccountNumber) == 0)
		{
			UserAccount = &(BankDatabase[i]); // GLOBAL POINTER POINTS TO THE UserAccount
			return HEALTHY;
		}
	}
	return ACCOUNT_NOT_FOUND;
}

/*********************** IS AMOUNT AVAILABLE FUNCTION *****************************/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (termData->transAmount > UserAccount->balance)
	{
		return LOW_BALANCE;
	}
	else
	{
		return HEALTHY;
	}
}

/************************* SAVE TRANSACTION FUNCTION *****************************/
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	static uint32_t SeqNum = 1;

	if (SeqNum > 255)
	{
		return SAVING_FAILED;
	}

	transactionDatabase[SeqNum - 1].cardHolderData = transData->cardHolderData; // PASSING cardHolderData TO transactionDatabase 
	transactionDatabase[SeqNum - 1].terminalData = transData->terminalData; // PASSING terminalData TO transactionDatabase 
	transactionDatabase[SeqNum - 1].transactionSequenceNumber = SeqNum; // PASSING transactionNumber TO transactionDatabase 

	if (transData->transState == APPROVED)
	{
		transactionDatabase[SeqNum - 1].transState = APPROVED; // PASSING transState (APPROVED) TO transactionDatabase 
	}

	else if (transData->transState == DECLINED_INSUFFECIENT_FUND) // PASSING transState (ACCOUNT_NOT_FOUND) TO transactionDatabase
	{
		transactionDatabase[SeqNum - 1].transState = DECLINED_INSUFFECIENT_FUND;
	}

	else if (transData->transState == DECLINED_STOLEN_CARD)
	{
		transactionDatabase[SeqNum - 1].transState = DECLINED_STOLEN_CARD;
	}

	SeqNum++; // INCREMENT SeqNum FOR THE NEXT TRANSACTION
	return HEALTHY;
}
