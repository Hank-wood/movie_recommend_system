#ifndef _MOVIE_HPP_
#define _MOVIE_HPP_

#include <cstddef>

#define NAMELEN 64

struct Movie {
    int id;
    char name[NAMELEN];
    int vote;
};

#endif
