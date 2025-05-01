#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl/include/curl/curl.h"
//#include <curl/curl.h>
#include "cJSON/cJSON.h"

struct memory {
    char *response;
    size_t size;
};

size_t write_callback(char *data, size_t size, size_t nmemb, void *userp){
    size_t total_size = size * nmemb;
    struct memory *mem = (struct memory *)userp;

    char *ptr = realloc(mem->response, mem->size + total_size + 1);
    if(!ptr){
        return 0; /*out of memory*/
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, total_size);
    mem->size += total_size;
    mem->response[mem->size] = 0; 

    return total_size;
}

#define JSON_MAX_LEN 2048

int main(){
    struct memory chunk = {0};
    //struct memory chunk = { .response = NULL, .size = 0 };
    char city[32];
    printf("Введите город(по английски):");
    scanf("%s", &city);
    const char *KEY_API = "1ef837b74f994218910125613252904";
    char url[128];
    char response[JSON_MAX_LEN] = "\0";
    CURLcode res;
    snprintf(url,sizeof(url),"http://api.weatherapi.com/v1/current.json?key=%s&q=%s&aqi=no", KEY_API, city);
    
    CURL *curl = curl_easy_init();
    if(curl == NULL){
        fprintf(stderr, "ERROR: Unabtle to init curl\n");
        return 1;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK){
        fprintf(stderr, "ERROR: Unable to perform curl: %s\n", curl_easy_strerror(res));
        return 1;
    }

    printf("Respone:\n%s\n", chunk.response);

    cJSON *json = cJSON_Parse(chunk.response);
    cJSON *current = cJSON_GetObjectItem(json, "current");
    cJSON *temp_c = cJSON_GetObjectItem(current, "temp_c");

    if(cJSON_IsNumber(temp_c)){
        printf("Температура:%.1f\n", temp_c->valuedouble);
    }
    else{
        printf("Govno");
    }

    cJSON_Delete(json);
    free(chunk.response);
    curl_easy_cleanup(curl);

    return 0;
}
