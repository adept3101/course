#include "cJSON/cJSON.h"
#include <curl/curl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
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
  cJSON_ArrayForEach(item, valute) {
    cJSON *charCode = cJSON_GetObjectItem(item, "CharCode");
    if (cJSON_IsString(charCode) && strcmp(charCode->valuestring, "USD") == 0) {
      cJSON *value = cJSON_GetObjectItem(item, "Value");
      if (cJSON_IsString(value)) {
        printf("USD value: %s\n", value->valuestring);
      }
      break;
    }
  }

  // printf("%.2f\n", usd->valuedouble);

  // if (cJSON_IsNumber(item)) {
  //   printf("%.2f\n", i->valuedouble);
  // } else {
  //   fprintf(stderr, "EROR: USD not found\n");
  // }

  curl_easy_cleanup(curl);
  return 0;
}
