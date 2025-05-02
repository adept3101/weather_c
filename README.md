# TUI погода
**Погода в терминале**
## Оглавление
* [Первый шаг](##Начало)
* [Билдинг](#building)
    * [Билдинг на Windows(cmake)](#windows(cmake))
* [Компиляция](#Compiling)
    * [Компиляция на Windows(gcc)](#window(gcc))
    * [Компиляция на Linux](#linux)
## Начало
1. Создать папку через терминал с помощью команды:
```
mkdir <название папки>
```
2. Перейти в созданную папку через терминал с помощью команды:

```
cd <название папки>
```
3. Клонировать репозиторий с Github с помощью команды:
``` 
git clone https://github.com/adept3101/weather_c.git
```
4. Перейдите в склонированный репозиторий:
```
cd weather_c
```
5. Запустите исполняемый файл:

```
./main
```
или
```
./main.exe
```
# Building
## windows(cmake)

1. На Windows для того чтобы сбилдить через ``cmake`` сначала нужно клонировать репозиторий ``cJSON``:
```
git clone https://github.com/DaveGamble/cJSON.git 
```
2. cJSON необходимо сбилдить:
```
cd cJSON
mkdir build
cd build
cmake ..
cmake --build .
```

3. Далее нужно сбилдить все вместе  и запустить исполняемый файл:
```
cd .. (для выхода из папки cJSON)
mkdir build
cd build
cmake -G "MinGW Makefiles" .. (Если используете не Visual Studio)
cmake .. (Для Visual Studio)
cmake --build .
./main
```

# Compiling
## windows(gcc)
1. На Windows для того чтобы скомпилировать исполняемый файл через ``gcc`` сначала нужно клонировать репозиторий ``cJSON``:
```
git clone https://github.com/DaveGamble/cJSON.git 
```
2. Подключить curl:
``#include "Path/to/curl.h" ``\
    * Обычно ``"curl/include/curl/curl.h"``
3. Компилириваот командой:\
```
gcc -g main.c cJSON/cJSON.c -Icurl/include -Lcurl/lib -lcurl -o main
```
## linux
1. Для начала нужно клоинровать cJSON:
```
git clone https://github.com/DaveGamble/cJSON.git --depth=1
```
2. На linux не нужно клонировать библиотеку ``curl`` и просто подключите ее так:\
``#include <curl/curl.h>``
3. Компилируйте командой:
```
gcc -g main.c cJSON/cJSON.c -o main -lcurl
```