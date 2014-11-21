# coding: utf-8

import os
import sys

# get films' name
#os.chdir("movies")
#for i in os.listdir("."):
#    with open(i) as fp:
#        for j in fp.readlines():
#            film = ' '.join(j.strip().split()[:-1])
#            print(film)

os.chdir("movies")
for i in os.listdir("."):
    with open(i) as fp:
        for j in fp.readlines():
            uname = i
            fname = ' '.join(j.strip().split()[:-1])
            vote = j.strip().split()[-1]
            sys.stdout.write("((select user_id from users where user_name='%s'), (select movie_id from movies where movie_name='%s'), %s),\n" % (uname, fname, vote))
