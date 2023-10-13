/*
Created on: Nov 22, 2022
Author: Mostafa Ashraf
*/

#ifndef _SERVER_H_
#define _SERVER_H_

#include"terminal.h"
#include"card.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

typedef enum EN_transState_t /*Defining the server transaction state*/
{
	APPROVED,
	DECLINED_INSUFFECIENT_FUND,
	DECLINED_STOLEN_CARD,
	FRAUD_CARD,
	INTERNAL_SERVER_ERROR
}	EN_transState_t;

typedef struct ST_transaction_t /*Defining a structure to represent the data of the transactions*/
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}	ST_transaction_t;

typedef enum EN_serverError_t /*Defining the server error types*/
{
	SERVER_OK,
	SAVING_FAILED,
	TRANSACTION_NOT_FOUND,
	ACCOUNT_NOT_FOUND,
	LOW_BALANCE,
	BLOCKED_ACCOUNT
}	EN_serverError_t;

typedef enum EN_accountState_t /*Defining the accounts state*/
{
	RUNNING,
	BLOCKED
}	EN_accountState_t;

typedef struct ST_accountsDB_t /*Defining a structure for datebase*/
{
	float balance;
	EN_accountState_t state;
	uint8_t primaryAccountNumber[20];
}	ST_accountsDB_t;

EN_transState_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t** accountRefrence);
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
void listSavedTransactions(void);

void recieveTransactionDataTest(void);
void isValidAccountTest(void);
void isBlockedAccountTest(void);
void isAmountAvailableTest(void);
void saveTransactionTest(void);
void listSavedTransactionsTest(void);

#endif