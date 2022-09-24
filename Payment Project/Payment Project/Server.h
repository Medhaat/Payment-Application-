/************************************************ Server Header *****************************************/

#ifndef DATA_TYPES
#define DATA_TYPES

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;

#endif 

#ifndef SERVER_HEADER
#define SERVER_HEADER

#ifndef CARD_DATA_ST
#define CARD_DATA_ST
typedef struct ST_cardData_t // CARD DATA STRUCTURE (HOLDER NAME, PAN, EXPIRY DATE)
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;
#endif

#ifndef TERMINAL_DATA_ST
#define TERMINAL_DATA_ST
typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[11];
}ST_terminalData_t;
#endif

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t; // APPROVED = 0, DECLINED_INSUFFECIENT_FUND = 1, DECLINED_STOLEN_CARD = 2, INTERNAL_SERVER_ERROR = 3

typedef enum EN_serverError_t
{
	HEALTHY, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t; // HEALTHY = 0, SAVING_FAILED = 1, TRANSACTION_NOT_FOUND = 2, ACCOUNT_NOT_FOUND = 3, LOW_BALANCE = 4

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

ST_accountsDB_t BankDatabase[255]; // GOLBAL VARIABLES ARE AUTOMATICALLY INITIALIZED WITH ZERO'S
ST_transaction_t transactionDatabase[255]; // GOLBAL VARIABLES ARE AUTOMATICALLY INITIALIZED WITH ZERO'S

EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
EN_transState_t recieveTransactionData(ST_transaction_t* transData);

#endif