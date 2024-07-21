import json
from Lib import *
from HanziReplacer import *

trans = open_json('yiwen.json')
out = open('release/trans.dat','w',encoding="sjis")

nameset = []
replacementdict = {}

for dic in trans:
    ori = dic['pre_jp']
    t = dic['pre_zh']
    name = dic['name']
    replacementdict[ori] = t
    if name not in nameset:
        nameset.append(name)

hanzireplacer = HanziReplacer()
hanzireplacer.ReadTransAndGetHanzidict([replacementdict,nameset])
hanzireplacer.ChangeFont('WenQuanYi.ttf','tsukumo_font','release/tsukumo_font.ttf')

for dic in trans:
    ori = dic['pre_jp']
    t = hanzireplacer.hanzitihuan(dic['pre_zh'])
    res = f'{ori}={t}\n'
    out.write(res)