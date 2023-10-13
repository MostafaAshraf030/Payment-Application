/*
Created on: Nov 19, 2022
Author: Mostafa Ashraf
*/

#include"terminal.h"
//#include<stdlib.h>

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) /*Function to get the transation date from the system in the format DD / MM / YYYY*/
{
	time_t Time = time(NULL);
	struct tm currentTime = *localtime(&Time);

	uint8_t arrTime[11];
	arrTime[2] = arrTime[5] = '/';
	arrTime[0] = (currentTime.tm_mday / 10) + '0';
	arrTime[1] = (currentTime.tm_mday % 10) + '0';

	arrTime[3] = ((currentTime.tm_mon + 1) / 10) + '0';
	arrTime[4] = ((currentTime.tm_mon + 1) % 10) + '0';

	arrTime[6] = ((currentTime.tm_year + 1900) / 1000) + '0';
	arrTime[7] = ((currentTime.tm_year + 1900) / 100) % 10 + '0';
	arrTime[8] = ((currentTime.tm_year + 1900) / 10) % 10 + '0';
	arrTime[9] = (currentTime.tm_year + 1900) % 10 + '0';
	arrTime[10] = '\0';

	strncpy(termData->transactionDate, arrTime, 11); /*Store the date in transactionDate*/
	printf("%s\n", termData->transactionDate);

	return TERMINAL_OK;
}

void getTransactionDateTest(void) /*Function to test getTransactionDate function*/
{
	ST_terminalData_t termData;
	getTransactionDate(&termData);
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) /*Function to compare the transaction date with the expiration date*/
{
	uint8_t transDate[8]; /*Convert the transaction date in the same format as the card expiry date*/
	strncpy(transDate, &(termData->transactionDate[3]), 8);
	strncpy(&(transDate[3]), &(transDate[5]), 3);

	uint8_t cardDate[3], todayDate[3];
	strcpy(cardDate, &transDate[3]);
	strcpy(todayDate, &(cardData->cardExpirationDate[3]));

	uint8_t compResult; /*Comparing the years*/
	compResult = strncmp(cardDate, todayDate, 2);

	if (compResult == -1) /*If the expiry year is more than the current year*/
	{
		return TERMINAL_OK;
	}

	if (compResult == 1) /*If the current year is more than the expiry year*/
	{
		printf("\tDeclined expired card!\n");
		return EXPIRED_CARD;
	}

	compResult = strncmp(transDate, cardData->cardExpirationDate, 5); /*If the current year and the expiry year are same, compare the months*/
	if (compResult == 1)
	{
		printf("\tDeclined expired card!\n");
		return EXPIRED_CARD;
	}

	printf("Card is valid!\n");
	return TERMINAL_OK;
}

void isCardExpriedTest(void) /*Function to test the isCardExpried*/
{
	ST_cardData_t cardData1 = { "MostafaAshrafHassanMoha","1234567891234567","11/21" };
	ST_cardData_t cardData2 = { "MostafaAshrafHassanMoha","1234567891234567","11/27" };

	ST_terminalData_t termData;
	getTransactionDate(&termData);
	printf("%s\n\n", termData.transactionDate);

	printf("Tester Name: Mostafa Ashraf \n");
	printf("Function Name: isCardExpried \n");

	for (uint8_t i = 0; i < 2; i++)
	{

		switch (i)
		{
		case 0:
			printf("Test Case 1: Expired card \n");
			printf("Input Data: 11/21 \n");
			printf("Expected Result: 2 :: EXPIRED_CARD \n");
			printf("Actual Result: %d \n", isCardExpired(&cardData1, &termData));
			break;
		case 1:
			printf("Test Case 2: Valid card \n");
			printf("Input Data: 11/23 \n");
			printf("Expected Result: 0 :: TERMINAL_OK \n");
			printf("Actual Result: %d \n", isCardExpired(&cardData2, &termData));
			break;
		}
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) /*Function to get the amount of the transaction*/
{
	printf("\tPlease enter the amount of transaction: ");
	scanf(" %f", &(termData->transAmount));
	getchar();

	if ((termData->transAmount <= 0)) /*Check if the entered amount is less than 0*/
	{
		printf("\tInvalid transaction amount!\n");
		return INVALID_AMOUNT;
	}

	printf("The transcation amount is valid\n");
	return TERMINAL_OK;
}

void getTransactionAmountTest(void) /*Function to test the getTransactionAmount*/
{
	ST_terminalData_t termData;

	printf("Tester Name: Mostafa Ashraf \n");
	printf("Function Name: getTransactionAmount \n");

	for (uint8_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			printf("Test Case 1: Negative value \n");
			printf("Input Data: -10 \n");
			printf("Expected Result: 4 :: INVALID_AMOUNT \n");
			printf("Actual Result: %d \n", getTransactionAmount(&termData));
			break;
		case 1:
			printf("Test Case 2: Zero value \n");
			printf("Input Data: 0 \n");
			printf("Expected Result: 4 :: INVALID_AMOUNT \n");
			printf("Actual Result: %d \n", getTransactionAmount(&termData));
			break;
		case 2:
			printf("Test Case 3: Positve value \n");
			printf("Input Data: 10 \n");
			printf("Expected Result: 0 :: TEMINAL_OK \n");
			printf("Actual Result: %d \n", getTransactionAmount(&termData));
			break;
		case 3:
			printf("Test Case 4: Positve Float value \n");
			printf("Input Data: 155.3 \n");
			printf("Expected Result: 0 :: TEMINAL_OK \n");
			printf("Actual Result: %d \n", getTransactionAmount(&termData));
			break;
		}
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) /*Function to compare between the maximun and amount of transcation*/
{
	if ((termData->transAmount) > (termData->maxTransAmount))
	{
		printf("\tThe amount exceeds the maximum\n");
		return EXCEED_MAX_AMOUNT;
	}

	printf("The amount is below the maximum allowed amount\n");
	return TERMINAL_OK;
}

void isBelowMaxAmountTest(void) /*Function to test the isBelowMaxAmount*/
{
	ST_terminalData_t termData;
	setMaxAmount(&termData,10000);

	printf("Tester Name: Mostafa Ashraf \n");
	printf("Function Name: isBelowMaxAmount \n");

	for (uint8_t i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			printf("Test Case 1: More than max amount > 10000 \n");
			printf("Input Data: 15000 \n");
			printf("Expected Result: 5 :: EXCEED_MAX_AMOUNT \n");
			getTransactionAmount(&termData);
			printf("Actual Result: %d \n", isBelowMaxAmount(&termData));
			break;
		case 1:
			printf("Test Case 2: Less than max amount < 10000 \n");
			printf("Input Data: 5000 \n");
			printf("Expected Result: 0 :: TERMINAL_OK \n");
			getTransactionAmount(&termData);
			printf("Actual Result: %d \n", isBelowMaxAmount(&termData));
			break;
		case 2:
			printf("Test Case 3: Equal to max amount = 10000 \n");
			printf("Input Data: 10000 \n");
			printf("Expected Result: 0 :: TERMINAL_OK \n");
			getTransactionAmount(&termData);
			printf("Actual Result: %d \n", isBelowMaxAmount(&termData));
			break;
		}
	}
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount) /*Function to get the maximum available amount*/
{
	EN_terminalError_t maxAmountError = INVALID_MAX_AMOUNT; /* maximum amount error flag */
	if (maxAmount > 0)
	{
		termData->maxTransAmount = maxAmount;
		maxAmountError = TERMINAL_OK;
	}
}

void setMaxAmountTest(void) /*Function to test the setMaxAmount*/
{
	ST_terminalData_t termData;

	printf("Tester Name: Mostafa Ashraf \n");
	printf("Function Name: setMaxAmount \n");

	for (uint8_t i = 0; i < 4; i++)
	{

		switch (i)
		{
		case 0:
			printf("Test Case 1: Negative number\n");
			printf("Input Data: -100 \n");
			printf("Expected Result: 6 :: INVALID_MAX_AMOUNT \n");
			printf("Actual Result: %d \n", setMaxAmount(&termData,-100));
			break;
		case 1:
			printf("Test Case 2: zero \n");
			printf("Input Data: 0 \n");
			printf("Expected Result: 6 :: INVALID_MAX_AMOUNT \n");
			printf("Actual Result: %d \n", setMaxAmount(&termData,0));
			break;
		case 2:
			printf("Test Case 3: Positve number \n");
			printf("Input Data: 1000 \n");
			printf("Expected Result: 0 :: TERMINAL_OK \n");
			printf("Actual Result: %d \n", setMaxAmount(&termData,100));
			break;
		case 3:
			printf("Test Case 4: Float Positve number \n");
			printf("Input Data: 5000.5 \n");
			printf("Expected Result: 0 :: TERMINAL_OK \n");
			printf("Actual Result: %d \n", setMaxAmount(&termData,10.5));
			break;

		}
	}
}

//int main()
//{
	//getTransactionDateTest();
	//isCardExpriedTest();
	//getTransactionAmountTest();
	//isBelowMaxAmountTest();
	//setMaxAmountTest();
//}
