from Lib import *
import re
import json

yuanwen = open('yuanwen.txt','r',encoding='sjis',errors='ignore').readlines()
outfile = open('yuanwen.json','w',encoding='utf8')

messages = []#用于去重
out = []
dic = {}

n = 0#统计字数

for l in yuanwen:
    if l[0] == '@':
        dic['name'] = l[1:-1]
    else:
        if l not in messages:
            messages.append(l)
            dic['message'] = l[:-1]
            out.append(dic)
            dic = {}
            n+=len(l[:-1])
        else:
            dic = {}

json.dump(out,outfile,ensure_ascii=False,indent=2)
print(n)
