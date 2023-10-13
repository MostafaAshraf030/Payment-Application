/*
Created on: Nov 18, 2022
Author: Mostafa Ashraf
*/

#include<stdio.h>
#include<string.h>
#include"../PaymentApplication/card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) /*Function to get the name of the card holder*/
{
	EN_cardError_t ErrorState = CARD_OK; /*Make the card ok by default*/

	uint8_t arrName[30];

	printf("Enter cardholder's name : ");

	gets(arrName);

	if ((arrName == NULL) || (strlen(arrName) < 20) || (strlen(arrName) > 24)) /*Check the text validity of the name that the user entered*/
	{
		ErrorState = WRONG_NAME;
	}
	else
	{
		for (uint32_t i = 0; i <= strlen(arrName); i++) /*Store the name in cardHolderName*/
		{
			cardData->cardHolderName[i] = arrName[i];
		}
	}
	return ErrorState;
}

void getCardHolderNameTest(void) /*Function to test getCardHolderName function*/
{
	ST_cardData_t cardData;

	printf("Tester Name: Mostafa Ashraf \n");
	printf("Function Name: getCardHolderName \n");

	for (uint32_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			printf("------------------------------------ \n");
			printf("Test Case 1: Name is NULL \n");
			printf("Input Data:   \n");
			printf("Expected Result: 1 :: WRONG_NAME \n");
			printf("Actual Result: %d \n", getCardHolderName(&cardData));
			break;
		case 1:
			printf("------------------------------------ \n");
			printf("Test Case 2: Less than 20 characters \n");
			printf("Input Data: Mostafa \n");
			printf("Expected Result: 1 :: WRONG_NAME \n");
			printf("Actual Result: %d \n", getCardHolderName(&cardData));
			break;
		case 2:
			printf("------------------------------------ \n");
			printf("Test Case 3: More than 24 characters \n");
			printf("Input Data: Mostafa Ashraf Hassan Mohamed \n");
			printf("Expected Result: 1 :: WRONG_NAME \n");
			printf("Actual Result: %d \n", getCardHolderName(&cardData));
			break;
		case 3:
			printf("------------------------------------ \n");
			printf("Test Case 4: Not more than 24 characters or less than 20 characters \n");
			printf("Input Data: Mostafa Ashraf Hassan \n");
			printf("Expected Result: 0 :: CARD_OK \n");
			printf("Actual Result: %d \n", getCardHolderName(&cardData));
			break;
		}
	}
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) /*Function to get the expiry date of the card holder*/
{
	EN_cardError_t ErrorStatus = CARD_OK; /*Make the card ok by default*/

	uint8_t arrExpDate[10];

	printf("Enter card expiry date : ");

	gets(arrExpDate);
	fflush(stdin);

	uint32_t month = (arrExpDate[0] - 48) * 10 + (arrExpDate[1] - 48); /*The correct format is: MM/YY*/

	if ((arrExpDate == NULL) || (strlen(arrExpDate) < 5) || (strlen(arrExpDate) > 5) /*Check the expiry date validity that the user entered*/
		|| month > 12 || month < 0 || (arrExpDate[2] != '/'))
	{
		ErrorStatus = WRONG_EXP_DATE;
	}
	else
	{
		for (uint32_t i = 0; i <= strlen(arrExpDate); i++) /*Store the expiry date in cardExpirationDate*/
		{
			cardData->cardExpirationDate[i] = arrExpDate[i];
		}
	}
	return ErrorStatus;
}

void getCardExpiryDateTest(void) /*Function to test getCardExpiryDate function*/
{
	ST_cardData_t cardData;

	printf("Tester Name: Mostafa Ashraf \n");
	printf("Function Name: getCardExpiryDate \n");

	for (uint32_t i = 0; i < 6; i++)
	{
		switch (i)
		{
		case 0:
			printf("------------------------------------ \n");
			printf("Test Case 1: Incorrect format YY/MM \n");
			printf("Input Data: 22/11 \n");
			printf("Expected Result: 2 :: WRONG_EXP_DATE \n");
			printf("Actual Result: %d \n", getCardExpiryDate(&cardData));
			break;
		case 1:
			printf("------------------------------------ \n");
			printf("Test Case 2: Correct format but month more than 12 \n");
			printf("Input Data: 15/22 \n");
			printf("Expected Result: 2 :: WRONG_EXP_DATE \n");
			printf("Actual Result: %d \n", getCardExpiryDate(&cardData));
			break;
		case 2:
			printf("------------------------------------ \n");
			printf("Test Case 3: String more than 5  \n");
			printf("Input Data: 11/2022 \n");
			printf("Expected Result: 2 :: WRONG_EXP_DATE \n");
			printf("Actual Result: %d \n", getCardExpiryDate(&cardData));
			break;
		case 3:
			printf("------------------------------------ \n");
			printf("Test Case 4: Non numric string \n");
			printf("Input Data: Mostafa \n");
			printf("Expected Result: 2 :: WRONG_EXP_DATE \n");
			printf("Actual Result: %d \n", getCardExpiryDate(&cardData));
			break;
		case 4:
			printf("------------------------------------ \n");
			printf("Test Case 5: Incorrect slash  \n");
			printf("Input Data: 11-22 \n");
			printf("Expected Result: 2 :: WRONG_EXP_DATE \n");
			printf("Actual Result: %d \n", getCardExpiryDate(&cardData));
			break;
		case 5:
			printf("------------------------------------ \n");
			printf("Test Case 6: Correct format and correct input \n");
			printf("Input Data: 11/22 \n");
			printf("Expected Result: 0 :: CARD_OK \n");
			printf("Actual Result: %d \n", getCardExpiryDate(&cardData));
			break;
		}
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) /*Function to get the PAN of the card holder*/
{

	EN_cardError_t ErrorState = CARD_OK; /*Make the card ok by default*/

	uint8_t arrPAN[25];

	printf("Enter card PAN : ");

	fflush(stdin);
	gets(arrPAN);

	if ((arrPAN == NULL) || strlen(arrPAN) < 16 || strlen(arrPAN) > 19) /*Check the PAN validity that the user entered*/
	{
		ErrorState = WRONG_PAN;
	}
	else
	{
		for (uint32_t i = 0; i < strlen(arrPAN); i++) /*Store the PAN in primaryAccountNumber*/
		{
			cardData->primaryAccountNumber[i] = arrPAN[i];
		}
		cardData->primaryAccountNumber[strlen(arrPAN)] = '\0';
	}
	return ErrorState;
}

void getCardPANTest(void) /*Function to test getCardPAN function*/
{
	ST_cardData_t cardData;

	printf("Tester Name: Mostafa Ashraf \n");
	printf("Function Name: getCardPAN \n");

	for (uint32_t i = 0; i < 4; i++) {
		switch (i)
		{
		case 0:
			printf("Test Case 1: PAN is NULL \n");
			printf("Input Data:  \n");
			printf("Expected Result: 3 :: WRONG_PAN \n");
			printf("Actual Result: %d \n", getCardPAN(&cardData));
			break;
		case 1:
			printf("Test Case 2: Less than 16 \n");
			printf("Input Data: 000011112222 \n");
			printf("Expected Result: 3 :: WRONG_PAN \n");
			printf("Actual Result: %d \n", getCardPAN(&cardData));
			break;
		case 2:
			printf("Test Case 3: More than 19 \n");
			printf("Input Data: 000011112222333344445555 \n");
			printf("Expected Result: 3 :: WRONG_PAN \n");
			printf("Actual Result: %d \n", getCardPAN(&cardData));
			break;
		case 3:
			printf("Test Case 4: Not more than 19 or less than 16 \n");
			printf("Input Data: 0000111122223333 \n");
			printf("Expected Result: 0 :: CARD_OK \n");
			printf("Actual Result: %d \n", getCardPAN(&cardData));
			break;
		}
	}
}

//int main()
//{
	//getCardHolderNameTest();
	//getCardExpiryDateTest();
	//getCardPANTest();
//}