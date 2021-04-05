#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";
const char* db = "proj2";

int main(void) {
	FILE* fp = fopen("20161640.txt", "r");
	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	char query[1000] = "\0";
	char flat[20] = "Flat Envelope";
	char smallBox[20] = "Small Box";
	char largeBox[20] = "Large Box";
	char overnight[20] = "Overnight";
	char second[20] = "Second day";
	char moreday[20] = "More than two days";
	int fin = 0;
	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		
		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}
		if (fp) {
			while (fgets(query, sizeof(query), fp) == query) {
				mysql_query(connection, query);
			}
		}
		printf("***********************************************\n");
		printf("** Sogang Package Delivery Management System **\n");
		printf("***********************************************\n\n\n");
		while (1) {
			int type = 0;
			int type2 = 0;
			int year = 0;
			int trucknum = 0;
			char name[30] = { 0, };
			char year3[5] = { 0, };
			char month[3] = { 0, };
			char timestamp[10] = { 0, };
			int i = 0;
			printf("------- SELECT QUERY TYPES -------\n\n");
			printf("\t1. TYPE I\n");
			printf("\t2. TYPE II\n");
			printf("\t3. TYPE III\n");
			printf("\t4. TYPE IV\n");
			printf("\t5. TYPE V\n");
			printf("\t0. QUIT\n");
			printf("Which type of query? ");
			scanf("%d", &type);
			printf("\n");
			if (type == 1) {
				printf("---- TYPE I ----\n");
				while (1) {
					printf("Input the number of truck : ");
					scanf("%d", &trucknum);
					if (trucknum != 1721) {
						printf("Truck %d is not destroyed\n\n", trucknum);
						continue;
					}
					else {
						while (1) {
							printf("\n---- Subtypes in TYPE I ----\n");
							printf("\t1. TYPE I-1\n\t1. TYPE I-2\n\t1. TYPE I-3\n");
							printf("which type of query? ");
							scanf("%d", &type2);
							if (type2 == 1) {
								printf("\n---- TYPE I-1 ----\n");
								printf("** Find all customers who had a package on the truck at the time of the crash. **\n");
								sprintf(query, "select customer_name from customer, package, package_vehicle where vehicle_ID = 'TR-1721' and package.package_ID = package_vehicle.package_ID and customer.customer_ID = package.customer_ID");
								mysql_query(connection, query);
								sql_result = mysql_store_result(connection);
								printf("Customer Name : ");
								while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								{
									printf("%s. ", sql_row[0]);
								}
								mysql_free_result(sql_result);
								printf("\n");
							}
							else if (type2 == 2) {
								printf("\n---- TYPE I-2 ----\n");
								printf("** Find all recipients who had a package on the truck at the time of the crash. **\n");
								sprintf(query, "select recipient_name from customer, package, package_vehicle where vehicle_ID = 'TR-1721' and package.package_ID = package_vehicle.package_ID and customer.customer_ID = package.customer_ID");
								mysql_query(connection, query);
								sql_result = mysql_store_result(connection);
								printf("Recipient Name : ");
								while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								{
									printf("%s. ", sql_row[0]);
								}
								mysql_free_result(sql_result);
								printf("\n");
							}
							else if (type2 == 3) {
								printf("\n---- TYPE I-3 ----\n");
								printf("** Find the last successful delivery by that truck prior to the crash. **\n");
								sprintf(query, "select distinct location_name from package, package_vehicle, location where vehicle_ID = 'TR-1721' and package.package_ID = package_vehicle.package_ID and location.package_ID = package.package_ID and destroyed = '2'");
								mysql_query(connection, query);
								sql_result = mysql_store_result(connection);
								printf("Last successful delivery : ");
								while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								{
									printf("%s", sql_row[0]);
								}
								mysql_free_result(sql_result);
								printf("\n");
							}
							else if (type2 == 0) {
								fin = 1;
								break;
							}
							else
								continue;
						}
					}
					if (fin == 1) {
						fin = 0;
						printf("\n");
						break;
					}
				}
			}
			else if (type == 2) {
				printf("---- TYPE II ----\n");
				while (1) {
					printf("** Find the customer who has shipped the most packages in certain year **\n");
					printf("which year? ");
					scanf("%d", &year);
					if (year == 0)
						break;
					else {
						sprintf(query, "select customer.customer_name from(select max(P.dal) as del from(select count(package_ID) as dal from(select * from bill where bill.purchase_year = '%d') as N group by customer_ID) as P) as R, (select count(package_ID) as S, customer_ID from(select * from bill where bill.purchase_year = '%d') as M group by customer_ID) as B, customer where B.S = R.del and B.customer_ID = customer.customer_ID ", year, year);
						mysql_query(connection, query);
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("Customer Name : %s\n", sql_row[0]);
							i++;
						}
						if (i == 0) {
							printf("There are no purchase in year %d! Please enter year (2016~2018)\n", year);
						}
						i = 0;
						mysql_free_result(sql_result);
					}
				}
				printf("\n");
			}
			else if (type == 3) {
				printf("---- TYPE III ----\n");
				while (1) {
					printf("** Find the customer who has spent the most money on shipping in the past certain year **\n");
					printf("which year? ");
					scanf("%d", &year);
					if (year == 0)
						break;
					else {
						sprintf(query, "select customer_name from (select max(charge) as pal from bill, customer where customer.customer_ID = bill.customer_ID and bill.purchase_year = '%d') as S, bill, customer where S.pal = bill.charge and bill.customer_ID = customer.customer_ID", year);
						mysql_query(connection, query);
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("Customer Name : %s\n", sql_row[0]);
							i++;
						}
						if (i == 0) {
							printf("There are no purchase in year %d! Please enter year (2016~2018)\n", year);
						}
						i = 0;
						mysql_free_result(sql_result);
					}
					
				}
				printf("\n");
			}
			else if (type == 4) {
				printf("---- TYPE IV ----\n");
				printf("** Find those packages that were not delivered within the promised time **\n");
				sprintf(query, "select package_ID from tracking where track_status = 2");
				mysql_query(connection, query);
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					printf("%s, ", sql_row[0]);
				}
				mysql_free_result(sql_result);
				printf("\n");
				printf("\n");

			}
			else if (type == 5) {
				char bill[40] = "bill";
				char buf1[20] = { 0. };
				char buf2[20] = { 0, };
				printf("---- TYPE V ----\n");
				printf("** Generate the bill for each customer for the past certain month **\n");
				printf("Customer Name : ");
				scanf("%s", name);
				printf("Which month(YYYY-MM)? ");
				scanf("%s", timestamp);
				for (int i = 0; i < 4; i++)
					year3[i] = timestamp[i];
				for (int i = 0; i < 2; i++)
					month[i] = timestamp[i + 5];
				printf("Generating Bill..\n");
				sprintf(bill, "bill_%s%s_%s.txt", year3, month, name);
				FILE* fp2 = fopen(bill, "w");
				sprintf(query, "select customer_name, bill.address, sum(bill.charge) from bill, customer where customer_name = '%s' and bill.purchase_year = '%s' and bill.purchase_month = '%s'  and bill.customer_ID = customer.customer_ID ", name, year3, month);
				mysql_query(connection, query);
				sql_result = mysql_store_result(connection);
				fprintf(fp2, "Customer   Address    Amout\n");
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					fprintf(fp2, "%-11s %-11s $%-.4s\n", sql_row[0], sql_row[1], sql_row[2]);
				}
				fprintf(fp2, "----------------------------------\n");
				fprintf(fp2, "Itemized Billing List\n");
				fprintf(fp2, "==============================================================================\n");
				fprintf(fp2, "| Package Number | Amount | ServiceType | Weight | Timelines of Delivery\n");
				fprintf(fp2, "==============================================================================\n");
				sprintf(query, "select package_ID, bill.charge, package_type, weight, delivery_type from bill, customer where customer_name = '%s' and bill.purchase_year = '%s' and bill.purchase_month = '%s'  and bill.customer_ID = customer.customer_ID ", name, year3, month);
				mysql_query(connection, query);
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					if (strcmp(sql_row[2],"1")==0) 
						strcpy(buf1, flat);
					else if (strcmp(sql_row[2],"2")==0)
						strcpy(buf1, smallBox);
					else
						strcpy(buf1, largeBox);
					if (strcmp(sql_row[4], "1")==0)
						strcpy(buf2, overnight);
					else if (strcmp(sql_row[4],"2")==0)
						strcpy(buf2, second);
					else
						strcpy(buf2, moreday);
					fprintf(fp2, "%s | $%.4s | %s | %s | %s\n", sql_row[0], sql_row[1], buf1, sql_row[3], buf2);
				}
				fprintf(fp2, "-------------------------------------------------------------\n");
				mysql_free_result(sql_result);
				printf("Generation Completed\n\n");
				fclose(fp2);
			}
			else {
				fin = 0;
				printf("HAVE A GOOD DAY :)");
				break;
			}
		}
	}
	mysql_close(connection);
	fclose(fp);
	return 0;
}