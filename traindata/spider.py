# coding: utf-8

import sys
import re
from pyquery import PyQuery as pq

reload(sys)
sys.setdefaultencoding('utf-8')

def puts(s):
    sys.stdout.write(s + "\n")

def personal_data(url):
    votes = []
    doc = pq(url = url)
    div = doc('.item')
    for i in div:
        subdiv = pq(i).find('div')[1]
        ul = pq(subdiv)('ul')
        li = pq(ul)('li')
        a = pq(li)('a')
        em = pq(a)('em')
        s = em.html()
        name = re.findall(r"(.*?) */.*", s)[0] if '/' in s else s

        li = ul.find('li')[2]
        span = pq(li)('span')
        # no votes
        if span.attr('class') == "date":
            continue
        rating = span.attr('class')
        rating = re.findall(r"rating(.*)-t", rating)[0]
        votes += [(name, rating)]
        puts(name.encode('utf-8') + " " +  rating)

    return votes

cnt = 0
for start in range(0, 400, 20):
    tmp_url = "http://movie.douban.com/subject/1889243/comments?start=%d&limit=20&sort=new_score"
    doc = pq(url = tmp_url % start)
    data = doc('.comment-info')
    for i in data:
        a = pq(i)('a')
        movie = personal_data(a.attr('href') + "collect")
        with open("movies/" + a.html().encode('utf-8').replace('/', '-'), "w") as fp:
            for i in movie:
                fp.write(i[0] + " " + i[1] + "\n")
                cnt += 1
        print(str(cnt) + " completes")

