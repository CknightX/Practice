from urllib import request,error
import gzip
import re
import os
class Crawler(object):
    def __init__(self,url,rex):
        self._url=url
        self._rex=rex
    def get_all(self):
        self.__rex_data(1)
        return self._list
    def get_first(self):
        self.__rex_data(0)
        return self._list
    def reset(self,url,rex):
        self._url=url
        self._rex=rex
    def __get_html(self):
        try:
            self._data=request.urlopen(self._url).read()
#   data=gzip.decompress(url)
            self._data=self._data.decode('utf-8')
        except error.HTTPError:
            print("Can't open the website Type Enter to exit...")
            input()
            os._exit(0)
    def __rex_data(self,mode):
        self.__get_html()
        Re=re.compile(self._rex,re.S)
        self._list=[]
        if mode==0:
            reC=Re.search(self._data)
            i=1
            while True:
                try:
                    self._list.append(reC.group(i))                    
                    i=i+1
                except:
                    break
            if not self._list:
                self.__error()
        else:
            self._list=Re.findall(self._data)
            if not self._list:
                self.__error()
    def __error(self):
        print('No result! Type Enter to exit...')
        input()
        os._exit(0)
        
