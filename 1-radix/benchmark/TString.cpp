#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "TString.h"

TString::TString() : TString(NULL, 0) {
}

TString::TString(std::istream &is) {
    char c;
    size_t size=0;
//as possibility but has broblems with eof
//std::cout<<std::cin.getline(arr,MAXSIZE-1);
    c=is.get();//first(0) symbol
    while (c!=EOF && c!='\n' && size<MAXSIZE) {
        arr[size]=c;
        //because is>>c for ariphmetic types have to use .get
        c=is.get();
        size++;
    }
    for(;size<MAXSIZE;++size) {
        arr[size]='\0';
    }
}

TString::TString(char in[], size_t sizein) {
    size_t size=0;
    while(size<MAXSIZE && size<sizein) {
        arr[size]=in[size];
        size++;
    }
    for(;size<MAXSIZE;++size) {
            arr[size]='\0';
    }
}

void TString::Print() {
    size_t size=0;
    char out;
    while (size<MAXSIZE && arr[size]!='\0'){
        out=arr[size];
        std::cout<<out;
        size++;
    }
}

TString::~TString() {
}
