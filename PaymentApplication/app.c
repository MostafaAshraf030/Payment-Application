/*
Created on: Nov 23, 2022
Author: Mostafa Ashraf
*/

#include<stdio.h>
#include"../PaymentApplication/card.h"
#include"../PaymentApplication/terminal.h"
#include"../PaymentApplication/server.h"
#include"app.h"

void appStart(void)
{
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	ST_transaction_t transData;
	EN_transState_t transState;
	ST_accountsDB_t accountVerify;
	EN_cardError_t cardNameError = CARD_OK;

	while (getCardHolderName(&cardData) == WRONG_NAME) /*Get card data from user and verify*/
	{
		printf("Invalid name, Try again \n");
	}
	while (getCardExpiryDate(&cardData) == WRONG_EXP_DATE)
	{
		printf("Invalid expiry date, Try again \n");
	}
	while (getCardPAN(&cardData) == WRONG_PAN)
	{
		printf("Invalid PAN, Try again \n");
	}

	termData.maxTransAmount = 10000.0;

	getTransactionDate(&termData);
	transData.cardHolderData = cardData;

	if (isCardExpired(&cardData, &termData) == EXPIRED_CARD)
	{
		printf("Expired Card\n");
	}
	else
	{
		getTransactionAmount(&termData);

		if (isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT)
		{
			printf("Amount Excedding Limit\n");
		}
		else
		{

			transData.terminalData = termData; /*Store terminal data into transaction data*/

			switch (recieveTransactionData(&transData))
			{

			case APPROVED:
				printf("Transaction Approved\n");
				printf("The Account Balance is : %f \n", &(accountVerify.balance));
				break;
			case DECLINED_INSUFFECIENT_FUND:
				printf("Transaction DECLINED\tInsuffecient Fund\n");
				break;
			case DECLINED_STOLEN_CARD:
				printf("Transaction DECLINED\tStolent Card\n");
				break;
			case FRAUD_CARD:
				printf("Transaction DECLINED\tFraud Card\n");
				break;
			case INTERNAL_SERVER_ERROR:
				printf("Internal server error\n");
				break;
			}

		}

	}
}


