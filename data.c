#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <mysql/mysql.h>

static char * host = "localhost";
static char * user = "root";
static char * pass = "kcci";
static char * dbname = "openWeather";



char device[] = "/dev/ttyUSB0";
int fd;
unsigned long baud = 9600;  


int main()
{
	MYSQL *conn;
	conn = mysql_init(NULL);

	int sql_index, flag = 0;
	char in_sql[300] = {0};

	if(!(mysql_real_connect(conn, host, user, pass, dbname, 0, NULL, 0)))
	{
		fprintf(stderr, "ERROR: %s[%d]\n",
		mysql_error(conn), mysql_errno(conn));
		exit(1);
	}
	else
		printf("Connection Successful!\n\n");
	char ser_buff[1000] = {0};
	int index = 0, temp, humi, str_len, res=0;

	//parsing value
	char *pToken;
	char *pArray[2] = {0};

	char* ptr ='\0';
	char realtemp[3];
	char realhumi[3];
	int  timetemp=0;
	int timehumi=0;

//	char* local = '\0';

	//start
	printf("Raspberry Startup\n");
	fflush(stdout);
	if((fd = serialOpen(device, baud)) < 0){
		fprintf(stderr, "Unable %s\n",strerror(errno));
		exit(1);
	}
	if(wiringPiSetup() == -1)
		return 1;
	while(1)
	{
		if(serialDataAvail(fd)) {

			flag = 1;
			ser_buff[index++] = serialGetchar(fd);

			if(ser_buff[index-1] == 'L')
			{
				ser_buff[index-1] = '\0';
				str_len = strlen(ser_buff);
				pToken = strtok(ser_buff, ":");
				int i = 0;
				while(pToken != NULL) {
					pArray[i] = pToken;
					if(++i>3)
						break;
					pToken = strtok(NULL, " ");
				}
				humi = atoi(pArray[0]);
				temp = atoi(pArray[1]);
				printf("temp = %d, humi = %d\n", temp, humi);
				for(int i = 0; i <= str_len; i++)
				{
					ser_buff[i]=0;
				}
				index = 0;
			}
			
            if(ser_buff[index-1] == 'Q')
            {
				ptr = strstr(ser_buff, "temp");
				//strncpy(realtemp,ptr+6,5);
				sprintf(realtemp,"%c%c",*(ptr+6), *(ptr+7));
				timetemp = atoi(realtemp);
				
            	ptr = strstr(ser_buff, "humidity");
				sprintf(realhumi,"%c%c",*(ptr+10), *(ptr+11));
				timehumi = atoi(realhumi);
				printf("timetemp = %d, timehumi = %d\n", timetemp, timehumi);
				index = 0;
           }
			
	
			
			if(temp <100 && humi < 100)
			{
				if(flag == 1)
				{
					sprintf(in_sql,"insert into SeoulData (id,date,time,SeoulTemp, SeoulHumi, IndoorTemp, IndoorHumi) values (null, curdate(), curtime(), %d, %d, %d, %d)",timetemp, timehumi, temp, humi);
					res = mysql_query(conn,in_sql);
					if(res)
					{
						fprintf(stderr, "error: %s[%d]\n",
						mysql_error(conn), mysql_errno(conn));
						exit(1);
					}
				}
				temp=255;
				humi=255;
			}
			flag = 0;
			
		}
		fflush(stdout);
	}

	return 0;
}
