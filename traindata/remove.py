# coding: utf-8

import os

cnt = {}
os.chdir("movies")
for i in os.listdir("."):
    with open(i) as fp:
        for j in fp.xreadlines():
            name = ' '.join(j.strip().split()[:-1])
            if name in cnt:
                cnt[name] += 1
            else:
                cnt[name] = 1

for i in os.listdir("."):
    content = []
    with open(i) as fp:
        for j in fp.xreadlines():
            name = ' '.join(j.strip().split()[:-1])
            if cnt[name] > 1:
                content += [j]
    with open(i, "w") as fp:
        for j in content:
            fp.write(j)
