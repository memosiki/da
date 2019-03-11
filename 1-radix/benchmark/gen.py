#!/usr/bin/python
from random import choice
from string import ascii_letters


import random

def get_number():
    number = random.randint(0,2**64-1)

    return str(number)

def get_data():
    size = random.randint(1, 64)
    return ''.join(choice(ascii_letters) for i in range(size))
    
size = input()
i = 0
while i < int(size):
        print(get_number() + "\t" + get_data())
        i += 1
