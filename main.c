#include "cJSON/cJSON.h"
#include <curl/curl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct memory {
  char *response;
  size_t size;
};

size_t write_callback(char *data, size_t size, size_t nmemb, void *userp) {
  size_t total_size = size * nmemb;
  struct memory *mem = (struct memory *)userp;

  char *ptr = realloc(mem->response, mem->size + total_size + 1);
  if (!ptr) {
    return 0; /*out of memory*/
  }

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, total_size);
  mem->size += total_size;
  mem->response[mem->size] = 0;

  return total_size;
}

int main(int argc, char *argv[]) {
  // char* url = "https://www.cbr.ru/scripts/XML_daily.asp";
  struct memory chunk = {0};
  char *url = "http://localhost:8080";
  // snprintf(url, sizeof(url), , ...)
  CURL *curl = curl_easy_init();

  CURLcode res;

  if (curl == NULL) {
    fprintf(stderr, "Error initialize curl\n");
    return -1;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  res = curl_easy_perform(curl);

  // printf("response:\n%s\n", chunk.response);
  // printf("%us\n", res);
  // printf("%u\n", res);

  cJSON *json = cJSON_Parse(chunk.response);

  cJSON *valute = cJSON_GetObjectItem(json, "Valute");
  // cJSON *char_code = cJSON_GetObjectItem(valute, "CharCode");
  // cJSON *usd = cJSON_GetObjectItem(char_code, "USD");

  cJSON *item = NULL;
  char code[3];
  char *chcode;

  printf("Введите код валюты:");
  scanf("%s", code);

  cJSON_ArrayForEach(item, valute) {
    cJSON *charCode = cJSON_GetObjectItem(item, "CharCode");
    if (cJSON_IsString(charCode) && strcmp(charCode->valuestring, code) == 0) {
      cJSON *value = cJSON_GetObjectItem(item, "Value");
      if (cJSON_IsString(value)) {
        // printf("%s: %s\n", code, value->valuestring);
        chcode = value->valuestring;

        for (int i = 0; chcode[i]; i++) {
          if (chcode[i] == ',') {
            chcode[i] = '.';
          }
        }

        double l_val = strtod(chcode, NULL);
        l_val = floor(l_val * 100) / 100;
        printf("%s:%.2f\n", code, l_val);
      }
      break;
    }
  }

  curl_easy_cleanup(curl);
  return 0;
}
