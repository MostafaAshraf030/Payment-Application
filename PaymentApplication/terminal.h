/*
Created on: Nov 19, 2022
Author: Mostafa Ashraf
*/

#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include<stdio.h>
#include<string.h>
#include<time.h>
#include"card.h"
#include<math.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

typedef struct ST_terminalData_t /*Defining a structure to represent the data of the terminal*/
{
    float transAmount;
    float maxTransAmount;
    uint8_t transactionDate[11];
}   ST_terminalData_t;

typedef enum EN_terminalError_t /*Defining the terminal error types*/
{
    TERMINAL_OK,
    WRONG_DATE,
    EXPIRED_CARD,
    INVALID_CARD,
    INVALID_AMOUNT,
    EXCEED_MAX_AMOUNT,
    INVALID_MAX_AMOUNT
}   EN_terminalError_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData); /*Function to get the transation date from the system*/
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData); /*Function to check if the card is expired*/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData); /*Function to get the amount of the transaction*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData); /*Function to compare between the maximun and amount of transcation*/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount); /*Function to get the maximum available amount*/

void getTransactionDateTest(void); /*Function to test the getTransactionDate*/
void isCardExpriedTest(void); /*Function to test the isCardExpried*/
void getTransactionAmountTest(void); /*Function to test the getTransactionAmount*/
void isBelowMaxAmountTest(void); /*Function to test the isBelowMaxAmount*/
void setMaxAmountTest(void); /*Function to test the setMaxAmount*/

#endif
