/************************************************* Card Header ******************************************/

#ifndef DATA_TYPES
#define DATA_TYPES

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;

#endif 

#ifndef CARD_HEADER
#define CARD_HEADER

typedef enum EN_cardError_t // ERROR STATE
{
	OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t; // OK = 0, WRONG_NAME = 1, WRONG_EXP_DATE = 2, WRONG_PAN = 3


#ifndef CARD_DATA_ST
#define CARD_DATA_ST
typedef struct ST_cardData_t // CARD DATA STRUCTURE (HOLDER NAME, PAN, EXPIRY DATE)
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;
#endif

EN_cardError_t getCardHolderName(ST_cardData_t* cardData); // GET CARD HOLDER'S NAME FUNCTION
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData); // GET CARD EXPIRY DATE FUNCTION
EN_cardError_t getCardPAN(ST_cardData_t* cardData); // GET CARD PAN FUNCTION

#endif