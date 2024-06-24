---
title: C Json Example
date: 2023-09-06 20:34:00
toc: true
categories:
  - Technology
tags:
  - C
---

C 读写 [JSON](https://www.json.org/json-zh.html) 的代码示例，基于 [cJSON](https://github.com/DaveGamble/cJSON) 和 [yyjson](https://github.com/ibireme/yyjson)。

cJSON：[Release 1.7.16](https://github.com/DaveGamble/cJSON/releases/tag/v1.7.16)

yyjson：[Release 0.7.0](https://github.com/ibireme/yyjson/releases/tag/0.7.0)

<!--more-->

<br/>

## Test Json

```json
{
    "title":"体检报告",
    "personnel":[
        {
            "name":"张三",
            "height": 175,
            "is_married":false,
            "advice":null
        },
        {
            "name":"李四",
            "height":172.5,
            "is_married":true,
            "advice":"随访观察"
        }
    ]
}
```



## CJson

### 读

```c
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_PATH "test_code/read_demo.json"

int main()
{
    printf("cJSON Version: %s\n", cJSON_Version());

    // 读取json文件
    FILE *file = fopen(READ_PATH, "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 读取文件内容
    char *jsonData = (char *)malloc(fileSize + 1);
    fread(jsonData, 1, fileSize, file);
    jsonData[fileSize] = '\0';
    fclose(file);


    cJSON* cjson = NULL;
    cjson = cJSON_Parse(jsonData);
    if (cjson == NULL) {
        printf("Failed to parse json data.\n");
        return 1;
    }

    // 读取json中的数据
    cJSON *title = cJSON_GetObjectItem(cjson, "title");
    if (title != NULL && title->type == cJSON_String) {
        printf("title: %s\n", title->valuestring);
    }

    cJSON *personnel = cJSON_GetObjectItem(cjson, "personnel");
    int array_size = cJSON_GetArraySize(personnel);
    printf("personnel list len: %d\n", array_size);
    cJSON *p;
    cJSON_ArrayForEach(p, personnel)
    {
        cJSON* name = cJSON_GetObjectItem(p, "name");
        if (name != NULL && name->type == cJSON_String) {
            printf("name: %s\n", name->valuestring);
        }
        cJSON* height = cJSON_GetObjectItem(p, "height");
        if (height != NULL && height->type == cJSON_Number) {
            printf("height: %f\n", height->valuedouble);
        }
        cJSON* is_married = cJSON_GetObjectItem(p, "is_married");
        if (is_married != NULL && (is_married->type == cJSON_False || is_married->type == cJSON_True)) {
            printf("is_married: %d\n", is_married->valueint);
        }
        cJSON* advice = cJSON_GetObjectItem(p, "advice");
        if (advice != NULL && advice->type == cJSON_String) {
            printf("advice: %s\n", advice->valuestring);
        }
    }

    cJSON_Delete(cjson);
    free(jsonData);

    return 0;
}
```



### 写

```c
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WRITE_PATH "output.json"

int main()
{
    cJSON* root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "title", "体检报告");

    cJSON *personnel = cJSON_AddArrayToObject(root, "personnel");

    cJSON *item = cJSON_CreateObject();
    cJSON_AddStringToObject(item, "name", "张三");
    cJSON_AddNumberToObject(item, "height", 175);
    cJSON_AddFalseToObject(item, "is_married");
    cJSON_AddNullToObject(item, "advice");
    cJSON_AddItemToArray(personnel, item);

    item = cJSON_CreateObject();
    cJSON_AddStringToObject(item, "name", "李四");
    cJSON_AddNumberToObject(item, "height", 172.5);
    cJSON_AddTrueToObject(item, "is_married");
    cJSON_AddStringToObject(item, "advice", "随访观察");
    cJSON_AddItemToArray(personnel, item);

    FILE* file = fopen(WRITE_PATH, "w");
    if (file != NULL) {
        char* json_str = cJSON_Print(root);
        printf("%s\n", json_str);
        fputs(json_str, file);
        fclose(file);
        free(json_str);
    }

    cJSON_Delete(root);
    return 0;
}
```



## yyjson

### 读

```c
#include "yyjson.h"
#include <stdio.h>

#define READ_PATH "test_code/read_demo.json"

int main()
{
    yyjson_doc *doc = yyjson_read_file(READ_PATH, 0, NULL, NULL);
    yyjson_val *root = yyjson_doc_get_root(doc);

    printf("title: %s\n", yyjson_get_str(yyjson_obj_get(root, "title")));

    yyjson_val *personnel = yyjson_obj_get(root, "personnel");
    printf("personnel list len: %ld\n", yyjson_arr_size(personnel));

    for(size_t i = 0; i < yyjson_arr_size(personnel); i++)
    {
        yyjson_val *person = yyjson_arr_get(personnel, i);
        printf("[%ld] %s %ld %d %s\n", i, yyjson_get_str(yyjson_obj_get(person, "name")),
            yyjson_get_uint(yyjson_obj_get(person, "height")),
            yyjson_get_bool(yyjson_obj_get(person, "is_married")),
            yyjson_get_str(yyjson_obj_get(person, "advice")) );
    }

    yyjson_doc_free(doc);
    return 0;
}
```



### 写

```c
#include "yyjson.h"
#include <stdio.h>

#define WRITE_PATH "output.json"

int main()
{
    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    yyjson_mut_obj_add_str(doc, root, "title", "体检报告");

    yyjson_mut_val *personnel = yyjson_mut_arr(doc);

    yyjson_mut_val *p1 = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_str(doc, p1, "name", "张三");
    yyjson_mut_obj_add_uint(doc, p1, "height", 175);
    yyjson_mut_obj_add_bool(doc, p1, "is_married", false);
    yyjson_mut_obj_add_null(doc, p1, "advice");
    yyjson_mut_val *p2 = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_str(doc, p2, "name", "李四");
    yyjson_mut_obj_add_real(doc, p2, "height", 172.5);
    yyjson_mut_obj_add_bool(doc, p2, "is_married", true);
    yyjson_mut_obj_add_str(doc, p2, "advice", "随访观察");

    yyjson_mut_arr_add_val(personnel, p1);
    yyjson_mut_arr_add_val(personnel, p2);
    yyjson_mut_obj_add_val(doc, root, "personnel", personnel);

    // change value
    yyjson_mut_val *advice =  yyjson_mut_obj_get(p2, "advice");
    yyjson_mut_set_str(advice, "出院");

    size_t t;
    printf("%s\n", yyjson_mut_val_write(root, YYJSON_WRITE_NOFLAG, &t));
    printf("len:%ld\n", t);
    char *json_str = yyjson_mut_write(doc, YYJSON_WRITE_PRETTY, &t);
    printf("%.*s\n", (int)t, json_str);
    printf("len:%ld\n", t);

    yyjson_mut_val_write_file(WRITE_PATH, root, YYJSON_WRITE_PRETTY, NULL, NULL);
    printf("json writed.\n");

    yyjson_mut_doc_free(doc);
    return 0;
}
```



## 源码

```shell
wget https://www.easywang.site/resources/C-Json/c_json_example -O c_json_example.tar.gz
```

