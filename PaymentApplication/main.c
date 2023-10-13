/*
Created on: Nov 24, 2022
Author: Mostafa Ashraf
*/

#include<stdio.h>
#include"../PaymentApplication/app.h"

void main()
{
	static char choice[1] = "1";
	printf("W E L C O M E ...! \n");
	printf("To Make Transaction Press 1 \n");
	printf("TO View All Transactions Press 2 \n");

	while (1)
	{
		printf("choice: ");

		gets(choice);

		switch (choice[0])
		{
		case '1':
			appStart();
			break;
		case '2':
			listSavedTransactions();
			break;
		}
	}
}