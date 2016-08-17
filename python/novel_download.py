from urllib import request
import gzip
import re
import os
def getHtml(url1):
    data=request.urlopen(url1).read()
#    data=gzip.decompress(url)
    data=data.decode('utf-8')
    return data
def rex(Data):
    Re=re.compile('点此下载TXT</a></span>(.+?)<.+?\s<a href="(.+?)">下载1',re.S)
    list=Re.findall(Data)
    dirname=re.search('<title>(.+?)\|',Data).group(1)
    return (list,dirname)
def download(num):
    data=getHtml('http://xs.dmzj.com/%s/index.shtml'%num)
    list=rex(data)
    filename=list[1]
    illegal=(r'?',r'|',r'/','\\',r':',r'<',r'>')
    for y in illegal:
        filename=filename.replace(y,'')
    print(filename)
    os.mkdir('./%s'%filename)
    for x in list[0]:
        print(x[0])
        file=open('./%s/%s.txt'%(filename,x[0]),'wb')
        da=request.urlopen('http://xs.dmzj.com%s'%x[1]).read()
        file.write(da)
        file.close()
    print('下载完成！_(:3」∠)_')
    input()
print('Please type the num:',end='')
num=input()
download(num)


