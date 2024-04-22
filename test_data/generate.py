import time
import numpy as np


sgn=['+','-','*','/']
sgn2=['+','-','*','/']
print('开始生成含两个运算符的四则运算式：')
for i in range(0,300):
    strq=str(np.random.randint(1,101))+sgn[np.random.randint(0,4)]+str(np.random.randint(1,101))+sgn[
        np.random.randint(0,4)]+str(np.random.randint(1,101)) #随机生成含两个运算符的四则运算式并保存在strq中
    if i%2==0:
        print(strq)
    else:
        print(strq)

time.sleep(10)
print("开始生成含两个运算符且结果在100以内的的四则运算式：")
num=0
while True:
    sgn_lindex=np.random.randint(0, 4)#第一个运算符下标
    sgn_rindex=np.random.randint(0,4)#第二个运算符下标
    a=np.random.randint(1,101)
    b=np.random.randint(1,101)
    c=np.random.randint(1,101)#三个运算项下标
    str1=str(a)+sgn2[sgn_lindex]+str(b)+sgn2[sgn_rindex]+str(c) #生成便于表达式计算的字符串
    str2=str(a)+sgn[sgn_lindex]+str(b)+sgn[sgn_rindex]+str(c)   #生成数学表达字符串
    res=eval(str1)#计算表达式
    if res<=100 and res>=0:#判断算式结果是否小于100大于等于0
        if num % 2 == 0:
            print(str2+'='+"%.2f" % res)
        else:
            print(str2+'='+"%.2f" % res)
        num+=1
    if num==300:
        break