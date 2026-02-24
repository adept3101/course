#include <stdio.h>
#include <curl/curl.h>

int main(){
  // char* url = "https://www.cbr.ru/scripts/XML_daily.asp";
  char* url = "http://localhost:8080";
  // snprintf(url, sizeof(url), , ...)
  CURL *curl = curl_easy_init();

  CURLcode res;

  if (curl == NULL){
    fprintf(stderr, "Error initialize curl\n");
    return -1;
  }
  
  curl_easy_setopt(curl, CURLOPT_URL, url);
  res = curl_easy_perform(curl);
  printf("%us\n", res);

  curl_easy_cleanup(curl);
  return 0;
}
