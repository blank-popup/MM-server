#!/home/nova/miniforge3/envs/mm/bin/python
#-*- coding: utf-8 -*-

import sys
import hid

for arg in sys.argv:
    print(f'$$$$$ ARG: {arg}')

print('$$$$ ############## Test PyRun_SimpleFile ====================')

dividezero = 1/ 0
print(f'{dividezero}\n')

print('sdfjdlskgjlsdfkjgfsldkjfsldkjflsdkjewijfrlksdjflksdfjlsdjflsdjfsdlk')

with open("/home/nova/foobar.txt", "w") as f:
    f.write("Life is too short, you need python !!!!!!!!!!!!!!!!!!!!!!!!")
