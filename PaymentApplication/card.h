/*
Created on: Nov 18, 2022
Author: Mostafa Ashraf
*/

#ifndef _CARD_H_
#define _CARD_H_

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

typedef struct ST_cardData_t /*Defining a structure to represent the data of the card*/
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}	ST_cardData_t;

typedef enum EN_cardError_t /*Defining the card error types*/
{
	CARD_OK,
	WRONG_NAME,
	WRONG_EXP_DATE,
	WRONG_PAN
}	EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData); /*Function to get the name of the card holder*/
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData); /*Function to get the expiration date of the card*/
EN_cardError_t getCardPAN(ST_cardData_t* cardData); /*Function to get the PAN of the card*/

void getCardHolderNameTest(void); /*Function to test the getCardHolderName*/
void getCardExpiryDateTest(void); /*Function to test the getCardExpiryDate*/
void getCardPANTest(void); /*Function to test the getCardPAN*/

#endif
