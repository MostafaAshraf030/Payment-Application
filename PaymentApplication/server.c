/*
Created on: Nov 22, 2022
Author: Mostafa Ashraf
*/

#include<stdio.h>
#include<string.h>
#include"server.h"

ST_accountsDB_t accountsDB[255] =
{
	{10000.0f, RUNNING, "3185151313543130"},
	{1200.0f, RUNNING, "6216421762123284"},
	{7600.0f, RUNNING, "2671983072642951"},
	{23000.0f, BLOCKED, "9713812542563250"},
	{500.0f, RUNNING, "9122841338005592"},
	{7500.0f, RUNNING, "1543125421235413"},
	{29000.0f, BLOCKED, "1535433987538120"},
	{16300.0f, BLOCKED, "3516843155484313"},
	{150.0f, RUNNING, "1638453184134460"},
	{1400.0f, RUNNING, "3218484103551640"},
	{100.5f, RUNNING, "6484168541631660"},
	{40000.0f,BLOCKED, "1684163416132050"},
	{4900.0f, RUNNING, "8746131231610623"},
	{69400.5f, BLOCKED, "1184854613049841"},
	{2100.5f, RUNNING, "5148541313163311"},
	{19000.7f, RUNNING, "3846854165465161"},
	{2630.3f, RUNNING, "8745464135153150"},
	{18600.9f,BLOCKED, "7416535465138521"},
	{350500.0f, BLOCKED, "3468541310006526"},
	{75.6f, RUNNING, "5848561016866516"},
};

ST_transaction_t transactionsDB[255] = { 0 };

uint32_t transactionIndex = 0;

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_transState_t transactionState = INTERNAL_SERVER_ERROR; /*Make the INTERNAL_SERVER_ERROR by default*/
	EN_serverError_t isValidAccount_State;
	EN_serverError_t isAmountAvailable_state;
	EN_serverError_t isBlockedAccount_state;

	ST_accountsDB_t* accountVerify; /*Pointer to account verification*/

	isValidAccount_State = isValidAccount(&(transData->cardHolderData), &accountVerify); /*Account verification*/

	if (isValidAccount_State == SERVER_OK)
	{
		isAmountAvailable_state = isAmountAvailable(&(transData->terminalData), accountVerify);
		isBlockedAccount_state = isBlockedAccount(accountVerify);

		if (isBlockedAccount_state == BLOCKED_ACCOUNT)
		{
			transactionState = DECLINED_STOLEN_CARD;
		}

		else if (isAmountAvailable_state == LOW_BALANCE)
		{
			transactionState = DECLINED_INSUFFECIENT_FUND;
		}

		else
		{
			transactionState = APPROVED;
			accountVerify->balance = accountVerify->balance - transData->terminalData.transAmount;
		}
	}
	else
	{
		transactionState = FRAUD_CARD;
	}

	transData->transState = transactionState;
	saveTransaction(transData);
	return transactionState;
}

void recieveTransactionDataTest(void) /*Function to test the recieveTransactionData*/
{
	ST_transaction_t transData;

	printf("Tester Name: Mostafa Ashraf \n");
	printf("Function Name: recieveTransactionData \n");

	for (uint32_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			printf("Test Case 1: Fraud account WRONG PAN \n");
			transData = (ST_transaction_t){ .cardHolderData = "Mostafa Ahmed Mahmoud","7264982173640917","11/23",.terminalData = {1000,2000,"14/12/2022"} };
			printf("Input Data: cardHolderData = {MostafaAshrafHassanMoha, 7264982173640917, 11/23},\nterminalData = {1000,2000,14/12/2022} \n");
			printf("Expected Result: 3 :: FRAUD_CARD \n");
			printf("Actual Result: %d \n", recieveTransactionData(&transData));
			break;
		case 1:
			printf("Test Case 2: Stolen card Account blocked \n");
			transData = (ST_transaction_t){ .cardHolderData = "Mahmoud Mohamed Ahmed","3468541310006526","11/23",.terminalData = {1000,2000,"14/12/2022"} };
			printf("Input Data: cardHolderData = {MostafaAshrafHassanMoha, 3468541310006526, 11/23},\nterminalData = {1000,2000,14/12/2022} \n");
			printf("Expected Result: 2 :: DECLINED_STOLEN_CARD \n");
			printf("Actual Result: %d \n", recieveTransactionData(&transData));
			break;
		case 2:
			printf("Test Case 3: Declined insufficiend fund \n");
			transData = (ST_transaction_t){ .cardHolderData = "Ahmed Mohamed Hassan","9122841338005592","11/23",.terminalData = {700,1400,"14/12/2022"} };
			printf("Input Data: cardHolderData = {MostafaAshrafHassanMoha, 9122841338005592, 11/23},\nterminalData = {700,1400,14/12/2022} \n");
			printf("Expected Result: 1 :: DECLINED_INSUFFECIENT_FUND \n");
			printf("Actual Result: %d \n", recieveTransactionData(&transData));
			break;
		case 3:
			printf("Test Case 4: Approved \n");
			transData = (ST_transaction_t){ .cardHolderData = "Mostafa Ashraf Hassan","3846854165465161","11/23",.terminalData = {150,2500,"17/12/2022"} };
			printf("Input Data: cardHolderData = {Mostafa Ashraf Hassan, 3846854165465161, 11/23},\nterminalData = {150,2500,14/12/2022} \n");
			printf("Expected Result: 0 :: APROVED \n");
			printf("Actual Result: %d \n", recieveTransactionData(&transData));
			break;
		}
	}
}

	EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t** accountRefrence) 
	{
		EN_serverError_t ErrorState = SERVER_OK; /*Make the SERVER_OK by default*/

		uint32_t flag = 0;

		for (uint32_t i = 0; i < 255; i++)
		{

			if (strcmp(accountsDB[i].primaryAccountNumber, cardData->primaryAccountNumber) == 0) /*Search for PAN in database*/
			{
				*accountRefrence = &accountsDB[i]; /*Store the account refrence in accounts database*/
				flag++;
				break;
			}
		}
		if (flag == 0)
		{
			*accountRefrence = NULL;
			ErrorState = ACCOUNT_NOT_FOUND;
		}
		return ErrorState;
	}

	void isValidAccountTest(void) /*Function to test the isValidAccount function*/
	{
		ST_cardData_t cardData;
		ST_accountsDB_t* accountRefrence;

		printf("Tester Name: Mostafa Ashraf \n");
		printf("Function Name: isValidAccount \n");

		for (uint32_t i = 0; i < 2; i++)
		{
			switch (i)
			{
			case 0:
				printf("Test Case 1: Account not found WRONG PAN \n");
				cardData = (ST_cardData_t){ .cardHolderName = "MostafaAshrafHassanMoha",.primaryAccountNumber = "1578917563074568",.cardExpirationDate = "11/23" };
				printf("Input Data: cardHolderName = MostafaAshrafHassanMoha, primaryAccountNumber = 1578917563074568,cardExpirationDate = 11/23\n");
				printf("Expected Result: 3 :: ACCOUNT_NOT_FOUND \n");
				printf("Actual Result: %d \n", isValidAccount(&cardData, &accountRefrence));
				break;
			case 1:
				printf("Test Case 2: Account found Correct PAN \n");
				cardData = (ST_cardData_t){ .cardHolderName = "MostafaAshrafHassanMoha",.primaryAccountNumber = "3846854165465161",.cardExpirationDate = "11/27" };
				printf("Input Data: cardHolder = Mostafa Ashraf Hassan, primaryAccountNumber = 3846854165465161, cardExpirationDate = 11/23\n");
				printf("Expected Result: 0 :: SERVER_OK \n");
				printf("Actual Result: %d \n", isValidAccount(&cardData, &accountRefrence));
				break;
			}
		}
	}

	EN_serverError_t isBlockedAccount(ST_accountsDB_t * accountRefrence)
	{
		EN_transState_t ErrorState = SERVER_OK; /*Make the SERVER_OK by default*/

		if (accountRefrence->state == BLOCKED)
		{
			ErrorState = BLOCKED_ACCOUNT;
		}
		return ErrorState;
	}

	void isBlockedAccountTest(void) /*Function to test isBlockedAccount function*/
	{
		ST_accountsDB_t* accountRefrence = NULL;

		printf("Tester Name: Mostafa Ashraf \n");
		printf("Function Name: isBlockedAccount \n");

		for (uint32_t i = 0; i < 2; i++)
		{
			switch (i)
			{
			case 0:
				printf("Test Case 1: Blocked account \n");
				accountRefrence = &accountsDB[2];
				printf("Input Data: &accountsDB[2]\n");
				printf("Expected Result: 5 :: BLOCKED_ACCOUNT \n");
				printf("Actual Result: %d \n", isBlockedAccount(accountRefrence));
				break;
			case 1:
				printf("Test Case 2: Not Blocked account \n");
				accountRefrence = &accountsDB[3];
				printf("Input Data: &accountsDB[3]\n");
				printf("Expected Result: 0 :: SERVER_OK \n");
				printf("Actual Result: %d \n", isBlockedAccount(accountRefrence));
				break;
			}
		}
	}

	EN_serverError_t isAmountAvailable(ST_terminalData_t * termData, ST_accountsDB_t * accountRefrence)
	{
		EN_serverError_t ErrorState = SERVER_OK; /*Make the SERVER_OK by default*/

		if (accountRefrence->balance < termData->transAmount) /*Chech if a balance less than trans amount*/
		{
			ErrorState = LOW_BALANCE;
		}
		return ErrorState;
	}

	void isAmountAvailableTest(void) /*Function to test isAmountAvailable function*/
	{
		ST_terminalData_t termData;
		ST_accountsDB_t* accountRefrence = NULL;

		printf("Tester Name: Mostafa Ashraf \n");
		printf("Function Name: isBlockedAccount \n");

		for (uint32_t i = 0; i < 2; i++)
		{
			switch (i)
			{
			case 0:
				printf("Test Case 1: account balance < transAmount \n");
				accountRefrence = &accountsDB[5];
				termData = (ST_terminalData_t){ .transAmount = 1000,.maxTransAmount = 1500,.transactionDate = "14/12/2022" };
				printf("Input Data: Account Balance = 1000.0, transAmount = 1500.0 \n");
				printf("Expected Result: 4 :: LOW_BALANCE \n");
				printf("Actual Result: %d \n", isAmountAvailable(&termData, accountRefrence));
				break;
			case 1:
				printf("Test Case 2: account balance > transAmount \n");
				accountRefrence = &accountsDB[0];
				termData = (ST_terminalData_t){ .transAmount = 1000,.maxTransAmount = 1500,.transactionDate = "14/12/2022" };
				printf("Input Data: Account Balance = 1500.0, transAmount = 1000.0 \n");
				printf("Expected Result: 0 :: SERVER_OK \n");
				printf("Actual Result: %d \n", isAmountAvailable(&termData, accountRefrence));
				break;
			}
		}
	}

	EN_serverError_t saveTransaction(ST_transaction_t * transData)
	{
		EN_transState_t ErrorState = SERVER_OK; /*Make the SERVER_OK by default*/

		if (&transactionsDB[transactionIndex] != NULL) /*Check if transaction index is null to store data*/
		{
			transactionsDB[transactionIndex].cardHolderData = transData->cardHolderData;
			transactionsDB[transactionIndex].terminalData = transData->terminalData;
			transactionsDB[transactionIndex].transactionSequenceNumber = transactionIndex;
			transData->transactionSequenceNumber = transactionIndex;
			transactionsDB[transactionIndex].transState = transData->transState;
			transactionIndex++;
		}
		else
		{
			ErrorState = SAVING_FAILED;
		}
		return ErrorState;
	}

	void saveTransactionTest(void)
	{
		ST_transaction_t transData;

		printf("Tester Name: Mostafa Ashraf \n");
		printf("Function Name: saveTransaction \n");

		for (uint32_t i = 0; i < 3; i++)
		{
			switch (i)
			{
			case 0:
				printf("----------------------------------------");
				printf("Test Case 1: Saving ok state APPROVED \n");
				transData = (ST_transaction_t)
				{ .cardHolderData = {.cardHolderName = "Mostafa Ashraf Hassan",.cardExpirationDate = "12/27",.primaryAccountNumber = "3846854165465161"}
				 ,.terminalData = {.maxTransAmount = 10000,.transactionDate = "14/12/2022",.transAmount = 3500}
				 ,.transState = APPROVED
				};
				printf("Input Data: cardHolderData = {cardHolderName = Mostafa Ashraf Hassan, cardExpirationDate = 12/27, primaryAccountNumber = 3846854165465161}\nterminalData = { maxTransAmount = 10000 transactionDate = 14/12/2022 transAmount = 3500 }\ntransactionSequenceNumber = 0, transState = APPROVED);\n");
				printf("Expected Result: 0 :: SERVER_OK \n");
				printf("Actual Result: %d \n", saveTransaction(&transData));
				break;
			case 1:
				printf("----------------------------------------");
				printf("Test Case 2: saving ok state DECLINED_INSUFFECIENT_FUND \n");
				transData = (ST_transaction_t)
				{ .cardHolderData = {.cardHolderName = "Mostafa Ahmed Mahmoud",.cardExpirationDate = "12/24",.primaryAccountNumber = "5848561016866516"}
				 ,.terminalData = {.maxTransAmount = 10000,.transactionDate = "25/01/2022",.transAmount = 1500}
				 ,.transState = APPROVED
				};
				printf("Input Data: cardHolderData = {cardHolderName = Mostafa Ahmed Mahmoud, cardExpirationDate = 12/24, primaryAccountNumber = 5848561016866516}\nterminalData = { maxTransAmount = 10000 transactionDate = 25/01/2022 transAmount = 1500 }\ntransactionSequenceNumber = 1, transState = DECLINED_INSUFFECIENT_FUND);\n");
				printf("Expected Result: 0 :: SERVER_OK \n");
				printf("Actual Result: %d \n", saveTransaction(&transData));
				break;
			case 2:
				printf("----------------------------------------");
				printf("Test Case 3: Saving ok state FRAUD_CARD \n");
				transData = (ST_transaction_t)
				{ .cardHolderData = {.cardHolderName = "Mahmoud Mohamed Ahmed",.cardExpirationDate = "12/23",.primaryAccountNumber = "9713812542563250"}
				 ,.terminalData = {.maxTransAmount = 10000,.transactionDate = "23/02/2022",.transAmount = 8000}
				 ,.transState = APPROVED
				};
				printf("Input Data: cardHolderData = {cardHolderName = Mahmoud Mohamed Ahmed, cardExpirationDate = 12/23, primaryAccountNumber = 9713812542563250}\nterminalData = { maxTransAmount = 10000 transactionDate = 23/02/2022 transAmount = 8000 }\ntransactionSequenceNumber = 2, .transState = FRAUD_CARD);\n");
				printf("Expected Result: 0 :: SERVER_OK \n");
				printf("Actual Result: %d \n", saveTransaction(&transData));
				break;
			}
		}
	}void listSavedTransactions(void)
	{
		if (transactionIndex == 0)
		{
			printf("There is no transactions \n");
		}
		else
		{
			for (uint32_t i = 0; i < transactionIndex; i++)
			{
				printf("----------------------------------------");
				printf("Transaction Sequence Number : %d \n", i);
				printf("Transaction Date: %s \n", transactionsDB[i].terminalData.transactionDate);
				printf("Transaction Amount: %.1f \n", transactionsDB[i].terminalData.transAmount);
				switch (transactionsDB[i].transState)
				{
				case APPROVED:
					printf("Transaction State : APPROVED \n");
					break;
				case DECLINED_INSUFFECIENT_FUND:
					printf("Transaction State : DECLINED_INSUFFECIENT_FUND \n");
					break;
				case DECLINED_STOLEN_CARD:
					printf("Transaction State : DECLINED_STOLEN_CARD \n");
					break;
				case FRAUD_CARD:
					printf("Transaction State : FRAUD_CARD \n");
					break;
				case INTERNAL_SERVER_ERROR:
					printf("Transaction State : INTERNAL_SERVER_ERROR \n");
					break;
				}
				printf("Terminal Max Amount : %.1f \n", transactionsDB[i].terminalData.maxTransAmount);
				printf("Cardholder Name: %s \n", transactionsDB[i].cardHolderData.cardHolderName);
				printf("PAN: %s \n", transactionsDB[i].cardHolderData.primaryAccountNumber);
				printf("Card Expiration Date: %s \n", transactionsDB[i].cardHolderData.cardExpirationDate);
			}
		}
	}

	void listSavedTransactionsTest(void) {
		listSavedTransactions();
	}

	//int main()
	//{
	//	recieveTransactionDataTest();
	//	isValidAccountTest();
	//	isBlockedAccountTest();
	//	isAmountAvailableTest();
	//	saveTransactionTest();
	//	listSavedTransactionsTest();
	//}
