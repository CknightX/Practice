#此脚本用来统计当前目录及子目录下所有C/C++程序的代码行数
import os
import re
sum=0
def walk(dir):
    global sum
    for root, dirs, files in os.walk(dir):
        for file in files:
            _path=os.path.join(root, file)
            s=re.search('(\.h|\.cpp|\.hpp|\.c)$',_path)
            if s:
                file=open(_path,'rb')
                d=len(file.readlines())
                sum=sum+d
                print('%s:%d行'%(_path,d))
                file.close()
        #for _dir in dirs: #子目录递归执行
            #walk(dir+ _dir)
walk('.\\')
print('总行数:%d'%sum)
input()
