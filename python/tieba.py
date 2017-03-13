import urllib
import urllib.request
import re
import gzip
import time
import socket

#下载选项

ID='3282985385' #帖子ID
socket.setdefaulttimeout(5) #超时设置

class StrTools:
    removeImg=re.compile('<img.*?>')
    removeAddr=re.compile('<a.*?>|</a>')
    replaceBR=re.compile('<br>')
    replaceAt=re.compile(r'@.+?<')
    removeDiv=re.compile(r'<div.+?>|</div>')
    removeCc=re.compile(r'</cc>|<cc>')
    matchTitle=re.compile('(.+?)_')
    matchBar=re.compile('_(.+?)_')
    def replace(self,x):
        x=re.sub(self.removeImg,'',x)
        x=re.sub(self.removeAddr,'',x)
        x=re.sub(self.replaceBR,'\n',x)
        x=re.sub(self.replaceAt,'<',x)
        x=re.sub(self.removeCc,'',x)
        x=re.sub(self.removeDiv,'',x)
        return x.strip()
    def title_match(self,x):
        L=[]
        L.append(re.search(self.matchTitle,x).group(1))
        L.append(re.search(self.matchBar,x).group(1))
        return L
class Tieba:
    def __init__(self,ID,IS_ONLYLZ,IS_FLOOR):
        self.f=0
        self.StrTool=StrTools()
        self.curr_page=1
        if IS_ONLYLZ == True:
            see_lz='1'
        else:
            see_lz='0'
        self.is_floor=IS_FLOOR
        self.base_url='http://tieba.baidu.com/p/'+ID+'?see_lz='+see_lz+'&pn='
        self.first_page=self.__get_page_data(1)
        try:
            self.num=int(self.__get_num())
            l=self.StrTool.title_match(self.__get_title())
            self.title=l[0]
            self.bar=l[1]
        except:
            print('帖子不存在')
            input()
            exit()
        else:
            print('贴吧:%s'%self.bar)
            print('帖子标题:%s'%self.title)
            print('帖子页数:%d'%self.num)
    def download(self):
        self.file=open('./%s.txt'%self.title,'wb')
        #第一页已经获取，直接写入
        self.__write_list2file(self.__extract_content(self.first_page))
        print('第1页完成')
        for i in range(2,self.num+1):
            self.__write_list2file(self.__get_contents(i))
            print('第%d页完成'%i)
        self.file.close()
    def __get_num(self):
        match=re.search(r'<span class="red">(\d+)</span>',self.first_page,re.S)
        try:
            return match.group(1)
        except:
            raise
    def __get_page_data(self,page_num):
        try:
            url=self.base_url+str(page_num)
            data=urllib.request.urlopen(url).read().decode('utf-8')
        except:
            self.f=self.f+1
            if self.f>=10:
                print('打开失败，请检查网络。')
                self.file.close()
                input()
                exit()
            else:
                return self.__get_page_data(page_num)
        else:
            return data
            self.f=0
    def __get_title(self):
        match=re.search(r'<title>(.+?)</title>',self.first_page,re.S)
        try:
            return match.group(1)
        except:
            raise
    def __extract_content(self,data):
        contents=[]
        for floor in re.findall('<div id="post_content.+?>(.+?</div>.+?)</div>',data,re.S):
            tmp=self.StrTool.replace(floor)
            contents.append(tmp.encode('utf-8'))
        return contents
    def __get_contents(self,page):
        return self.__extract_content(self.__get_page_data(page))
    def __write_list2file(self,List):
        for L in List:
            if self.is_floor==True:
                self.file.write(('\n\n---------%d楼---------\n'%self.curr_page).encode('utf-8'))
                self.curr_page=self.curr_page+1
            else:
                self.file.write('\n\n'.encode('utf-8'))
            self.file.write(L)

tieba=Tieba(ID,True,True)
tieba.download()
print('下载完成！')
input()
