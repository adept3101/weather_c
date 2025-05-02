#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl/include/curl/curl.h"
#include "cJSON/cJSON.h"

struct memory {
    char *response;
    size_t size;
};

typedef struct {
    const char *dir;
    const char *arrow;
} WindDirMap;

WindDirMap wind_map[] = {
    {"N",   "↑"},
    {"NNW", "↖"},
    {"NW",  "↖"},
    {"WNW", "↖"},
    {"W",   "←"},
    {"WSW", "↙"},
    {"SW",  "↙"},
    {"SSW", "↙"},
    {"S",   "↓"},
    {"SSE", "↘"},
    {"SE", "↘"},
    {"ESE", "↘"},
    {"E", "→"},
    {"ENE", "↗"},
    {"NE", "↗"},
    {"NNE", "↗"}
};

typedef struct {
    const int min;
    const int max;
    const char *stat;
    const char *pic;
} Cloudy;

Cloudy cloudy_stat[] = {
    {0, 12, "Sunny", "   \\   /     \n    .-.\n ― (   ) ―   \n    `-’      \n   /   \\      \n"},
    {13, 25, "Few clouds", "   \\  /\n__ /\"\".-.\n   \\_(   ).\n   /(__(__)\n"},
    {26, 37, "Wisps", "    .--.\n .-(    ).\n(__)  (__)\n"},
    {38, 50, "Scattered", "  \\  /\n_ /\"\".-.\n  \\_(  ).\n  / (__)\n"},
    {51, 62, "Mixed", "    .--.\n .-(    ).\n(___.__)__)\n   (   )\n"},
    {63, 75, "Partly cloudy", "  \\  /     .--.\n_ /\"\".-.  (    ).\n  \\_(   ).(__.__)\n  /(___)(__)\n"},
    {76, 87, "Broken to overcast", "     .--.\n  .-(    ).\n (___.__)__)\n (___(___))\n"},
    {88, 100, "Cloudy", "    .--.\n .-(    ).\n(___.__)__)\n"}
};
/*
            \   /     
             .-.      
          ― (   ) ―   
             `-’      
            /   \     
     
     */
/*
      .--.     
   .-(    ).   
  (___.__)__)  
               
*/

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

    //printf("Respone:\n%s\n", chunk.response);

    cJSON *json = cJSON_Parse(chunk.response);
    cJSON *current = cJSON_GetObjectItem(json, "current");
    cJSON *temp_c = cJSON_GetObjectItem(current, "temp_c");
    cJSON *wind_kph = cJSON_GetObjectItem(current, "wind_kph");
    cJSON *wind_dir = cJSON_GetObjectItem(current, "wind_dir");
    cJSON *vis_km = cJSON_GetObjectItem(current, "vis_km");
    cJSON *cloud = cJSON_GetObjectItem(current, "cloud");

    if(cJSON_IsNumber(temp_c)){
        printf("%.1f°C\n", temp_c->valuedouble);
    }
    else{
        fprintf(stderr, "ERROR: Unable get temperature\n");
    }

    if(cJSON_IsString(wind_dir)){
        const int map_size = sizeof(wind_map) / sizeof(wind_map[0]);
        for (int i = 0; i < map_size; i++) {
            if (strcmp(wind_dir->valuestring, wind_map[i].dir) == 0) {
                printf("%s ", wind_map[i].arrow);
            }
        }
    }
    else{
        fprintf(stderr, "ERROr: Unable to get wind_dir\n");
    }

    if(cJSON_IsNumber(wind_kph)){
        printf("%.1fm/s\n", (wind_kph->valuedouble * 1000) / 3600);
    }
    else{
        fprintf(stderr, "ERROR: Unable to get wind_kph\n");
        return 1; 
    }

    if(cJSON_IsNumber(vis_km)){
        printf("%.1fkm\n", vis_km->valuedouble);
    }
    else{
        fprintf(stderr, "ERROR: Unabgle to get vis\n");
    }

    if(cJSON_IsNumber(cloud)){
        const int size = sizeof(cloudy_stat) / sizeof(cloudy_stat[0]);
        for(int i = 0; i < size; i++){
            if(cloud->valueint >= cloudy_stat[i].min && cloud->valueint <= cloudy_stat[i].max){
                printf("%s\n", cloudy_stat[i].stat);
                printf("%s\n", cloudy_stat[i].pic);
            }
        }
    }
    else{
        fprintf(stderr, "ERROR: Unable to get cloud");
    }

    cJSON_Delete(json);
    free(chunk.response);
    curl_easy_cleanup(curl);

    return 0;
}
