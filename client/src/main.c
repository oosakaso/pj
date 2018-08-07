#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void command(char* cmd, char* data){
  /* Open the command for reading. */
  memset(data, 0, sizeof(data));
  FILE* fp = popen(cmd, "r");
  if (fp == NULL) {
    printf("popen Failed.\n" );
    exit;
  }
  char temp[2048] = "";
  /* Read the output a line at a time */
  while (fgets(temp, sizeof(temp), fp) != NULL) {
    strcat(data, temp);
  }
  if(pclose(fp) == -1){
    printf("pclose Failed.\n");
  }
}

int count_interface(char* data){
  //count the number of interface
  char *s = strdup(data);
  int i = 1;
  char* p = strsep(&s, "\n");
  while( p != NULL){
    if(!strcmp(p, "")){
      i++;
    }
    p = strsep(&s, "\n");
  }
  return (i - 2);
}

char* split_interface(int i, char* data, char* temp){
  memset(temp, 0, sizeof(temp));
  int current = 0;
  char *s = strdup(data);
  char *p = strsep(&s, "\n");
  while( p != NULL ){
    if(!strcmp(p, "")){
      current++;
    }
    if(current == i) {
      strcat(temp, p);
      strcat(temp, "\n");
    }
    p = strsep(&s, "\n");
  }
  return temp;
}

void get_interface_info(char* p, char* result){
  char* substring = strtok(p, ": \n");
  strcat(result, "{\"name\":\"");
  strcat(result, substring);
  strcat(result, "\"");
  while(substring != NULL){
    if(!strcmp(substring, "HWaddr")){
      substring = strtok(NULL, " \n");
      strcat(result, ",\"HWaddr\":\"");
      strcat(result, substring);
      strcat(result, "\"");
    }
    else if(!strcmp(substring, "inet")){
      substring = strtok(NULL, " :\n");
      if(!strcmp(substring, "addr")){
        substring = strtok(NULL, " \n");
      }
      strcat(result, ",\"inet-addr\":\"");
      strcat(result, substring);
      strcat(result, "\"");
    }
    substring = strtok(NULL, " :\n");
  }
  strcat(result, "}");
}

void get_process_info(char* p, char* result) {
  char* substring = strtok(p, " ");
  if(substring == NULL) return;
  strcat(result, "{\"UID\":\"");
  strcat(result, substring);
  strcat(result, "\",");
  substring = strtok(NULL, " ");
  strcat(result, "\"PID\":");
  strcat(result, substring);
  strcat(result, ",");
  substring = strtok(NULL, " ");
  strcat(result, "\"PPID\":");
  strcat(result, substring);
  strcat(result, ",");
  substring = strtok(NULL, " ");
  strcat(result, "\"C\":");
  strcat(result, substring);
  strcat(result, ",");
  substring = strtok(NULL, " ");
  strcat(result, "\"STIME\":\"");
  strcat(result, substring);
  strcat(result, "\",");
  substring = strtok(NULL, " ");
  strcat(result, "\"TTY\":\"");
  strcat(result, substring);
  strcat(result, "\",");
  substring = strtok(NULL, " ");
  strcat(result, "\"TIME\":\"");
  strcat(result, substring);
  strcat(result, "\",");
  substring = strtok(NULL, "\n");
  strcat(result, "\"CMD\":\"");
  strcat(result, substring);
  strcat(result, "\"}");
}

int connect_socket(char* addr, char* result){
  int sock = 0;
  struct sockaddr_in server;

  sock = socket(AF_INET , SOCK_STREAM , 0);
  if (sock == -1){
    printf("Failed in creating socket\n");
    close(sock);
    return -1;
  }
  printf("Socket created\n");

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(addr);
  server.sin_port = htons(8888);

  if (connect(sock, (struct sockaddr *)& server, sizeof(server)) < 0){
    printf("Connection Failed \n");
    close(sock);
    return -1;
  }
  printf("Socket connected\n");

  if(send(sock , result, strlen(result) , 0 ) < 0){
    close(sock);
    printf("%s\n", "Failed to send result");
    return -1;
  }
  printf("%s\n", "Result sent");
  close(sock);
  return 1;
}

int main( int argc, char *argv[] )
{

  char* p;
  char data[2048] = "";
  char result[2048] = "";
  char temp[2048] = "";

  command("ifconfig", data);
  int i = count_interface(data);
  strcat(result, "{\"interfaces\":[");

  //for each interface
  for(int j = 0; j < i; j++ ){
    p = split_interface(j, data, temp);
    get_interface_info(p, result);
    if(j != i-1){
      strcat(result, ",");
    }
  }
  strcat(result, "]");

  // command("ps -ef | grep ps | grep ef", data);
  // if(strcmp(data, "")){
  //   strcat(result, ",\"process\":");
  //   get_process_info(data, result);
  // }

  // command("cat <file name>", data);
  // add data to result

  //print result
  strcat(result, "}");
  printf("%s\n", result);

  // int fp2 = open("info.json", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO );
  // FILE *f = fopen("info.json", "w");
  // if(f == NULL){
  //   printf("error cannot create json\n");
  //   exit;
  // }
  // else{
  //   fprintf(f, "%s", result);
  // }
  // if(fclose(f) != 0){
  //   printf("fclose Failed.\n");
  //   exit;
  // }
  char addr[32] = "";
  int trial = 0;
  printf("%s\n", "Please enter server addr: ");
  scanf("%s", addr);
  while(trial < 10){
    if(connect_socket(addr, result) == -1){
      trial++;
      continue;
    }
    else {
      break;
    }
  }

  return 0;
}
