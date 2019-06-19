# -*- coding: utf-8 -*-
"""
Created on Thu Oct 25 11:02:10 2018

@author: sorry
"""
import numpy as np
import sys
N=256

thres=int(sys.argv[1])
#thres=12.0

delaysall=np.load('delaysall127.npy')
#print('size is ',delaysall.shape)
count=0
for a in range(-128,127):
    for i in range(-128,127):
        for j in range(-128,127):
            
            # print(delaysall[count]),
            if delaysall[count]>=thres:
                print(a),
                print(i),
                print(j),
                print(delaysall[count]),
                print('')
            count+=1
        
    


