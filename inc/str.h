#include<string>
#include<list>
#include<optional>
#include<malloc.h>
#include<string_view>
#include<iostream>
#include<vector>

#include "common.h"
using std::list;
using std::string;
using std::optional;
using std::vector;
using std::string_view;

vector<string_view> split(const string &content, string_view delim_str);