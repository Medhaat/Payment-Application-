/*********************************************** Terminal Header ****************************************/

#ifndef DATA_TYPES
#define DATA_TYPES

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;

#endif 

#ifndef TERMINAL_HEADER
#define TERMINAL_HEADER

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

typedef enum EN_terminalError_t // ERROR STATE
{
	GOOD, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t; // GOOD = 0, WRONG_DATE = 1 (NOT USED), EXPIRED_CARD = 2, INVALID_CARD = 3, INVALID_AMOUNT = 4, EXCEED_MAX_AMOUNT = 5, INVALID_MAX_AMOUNT = 6 

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData); // GETTING TRANSACTION DATE AUTOMATICALLY FROM THE LOCAL MACHINE
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);

#endif