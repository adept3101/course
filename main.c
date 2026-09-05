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
  struct memory chunk = {0};

  char from[4];
  char to[4];

  printf("From:");
  scanf("%s", from);

  printf("To:");
  scanf("%s", to);

  FILE *file = fopen("config.ini", "r");
  char api_url[256];

  fscanf(file, "%s", api_url);
  fclose(file);

  strcat(api_url, from);

  char *url = api_url;
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

  cJSON *json = cJSON_Parse(chunk.response);

  cJSON *rates = cJSON_GetObjectItem(json, "rates");
  cJSON *rub = cJSON_GetObjectItem(rates, to);
  printf("%f\n", rub->valuedouble);

  cJSON_Delete(json);
  free(chunk.response);
  curl_easy_cleanup(curl);
  return 0;
}
