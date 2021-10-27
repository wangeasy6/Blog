# encoding=utf-8

import json


def read():
    file_path ="./json_example.json"
    print("Read " + file_path)
    with open(file_path, encoding='utf-8') as file:
        file_content = file.read()
        print(file_content)

        result = json.loads(file_content)        # 将已编码的 JSON 字符串解码为 Python 对象
        print(type(result), result)
        print(type(result["name"]), result["name"])
        print(type(result["personnel"]), result["personnel"])

        for person in result["personnel"]:
            print(type(person["name"]), person["name"])
            print(type(person["height"]), person["height"])
            print(type(person["is_married"]), person["is_married"])
            print(type(person["advice"]), person["advice"])


def write():
    out_file_name = "output.json"
    obj = {}
    obj["name"] = "体检报告"

    personnel = []
    person = {}
    person["name"] = "张三"
    person["height"] = 175
    person["is_married"] = False
    person["advice"] = None
    personnel.append(person)

    person = {}
    person["name"] = "李四"
    person["height"] = 172.0
    person["is_married"] = True
    person["advice"] = "随访观察"
    personnel.append(person)

    obj["personnel"] = personnel

    content = json.dumps(obj, ensure_ascii=False, indent=4)  # 对象 转换为 str
    print("Write :\r\n" + content)
    with open(out_file_name, "w", encoding="utf-8") as file:
        file.write(content)
    print("to " + out_file_name)


if __name__ == '__main__':
    read()
    write()