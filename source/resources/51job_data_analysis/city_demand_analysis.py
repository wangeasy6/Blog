# coding:utf-8
# requirement: pyecharts 1.7.1

from spider_51job import *
from pyecharts import options as opts
from pyecharts.charts import Map

all_code = get_city_code()
city_code = {}
for i in all_code.keys():
    if "区" in i or "县" in i or \
        "镇" in i or "省" in i:
        continue
    if i == "国外":
        break
    city_code[i] = all_code[i]
print(city_code)

city_value = {}
for city in city_code.keys():
    cmd = "spider_51job.py 嵌入式 -c " + city
    args = convert_str2args(cmd)
    url = get_url(args)
    city_value[city] = get_number_of_pages(url)
    print("{} - {}".format(city, city_value[city]))

print(city_value)
citys = list(city_value.keys())
values = list(city_value.values())

map = Map(init_opts=opts.InitOpts(width="800px", height="600px"))
map.add(
    "嵌入式城市热力图",
    [list(z) for z in zip(citys, values)],
    "china-cities",
    is_map_symbol_show=False,  # 不显示小红点
    label_opts=opts.LabelOpts(is_show=False)  # 不显示城市名称Label
)
map.set_global_opts(
    title_opts=opts.TitleOpts(title="中国-城市地图"),
    visualmap_opts=opts.VisualMapOpts(
        min_=0, max_=70,
        range_color=["#EEEEEE", "#9ACD32", "#FF0000","#000000"]
    )
)
map.render("嵌入式城市热力图.html")