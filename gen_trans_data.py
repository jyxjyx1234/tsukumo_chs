import json
from Lib import *
from HanziReplacer import *

trans = open_json('yiwen.json')
out = open('release/trans.dat','w',encoding="utf8")

nameset = []
replacementdict = {}

for dic in trans:
    ori = dic['pre_jp'].replace('〜','～')
    t = dic['pre_zh']
    name = dic['name']
    replacementdict[ori] = t
    if name not in nameset:
        nameset.append(name)

hanzireplacer = HanziReplacer()
hanzireplacer.ReadTransAndGetHanzidict([replacementdict,nameset])
hanzireplacer.ChangeFont('WenQuanYi.ttf','release\\tsukumo_font.ttf','tsukumo_font')

for dic in trans:
    ori = dic['pre_jp'].replace('〜','～')
    t = hanzireplacer.hanzitihuan(dic['pre_zh'])
    res = f'{ori}={t}\n'
    out.write(res)

for n in nameset:
    out.write(f'{n}={n}\n')