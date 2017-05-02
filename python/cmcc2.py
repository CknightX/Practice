import re
import os
import requests
from bs4 import BeautifulSoup


class CMCC:
    def __init__(self, user, passwd):
        self.user = user
        self.passwd = passwd
        self.url = 'http://211.138.195.124/suiexingclient.jsp'
        self.test_url = 'http://s1.hdslb.com/'
        self.base_data = {
            'USER': self.user,
            'PWD': self.passwd,
            'pwdtype': '1',
            'forceflag': '0',
            'clienttype': 'PC,windows,2.5.0',
            'wlanacname': '',
            'wlanuserip': '',
            'actiontype': '',
            'wlanacssid': '',
            'languagetype': '',
            'logonsessid': ''
        }

    def __GetIp(self):  # get local host
        x = os.popen('ipconfig').read()
        return re.search(r'172.+', x).group(0)

    def __GetLoginData(self):
        self.base_data['actiontype'] = 'LOGIN'
        self.base_data['wlanuserip'] = self.__GetIp()
        return self.base_data

    def __GetLogoutData(self):
        self.base_data['actiontype'] = 'LOGOUT'
        self.base_data['wlanuserip'] = self.__GetIp()
        return self.base_data

    def GetCurrStatus(self):
        while(True):
            try:
                source = requests.get(self.test_url, timeout=2).text
            except:
                continue
            else:
                break
        self.bs = BeautifulSoup(source, 'html.parser')
        try:
            wlanacname = self.bs.find(
                attrs={'name': 'wlanacname'}).attrs['value']
            wlanuserip = self.bs.find(
                attrs={'name': 'wlanuserip'}).attrs['value']
        except:
            return None
        else:
            return wlanacname, wlanuserip

    def Login(self):
        requests.post(self.url, data=self.__GetLoginData())

    def Logout(self):
        requests.post(self.url, data=self.__GetLogoutData())

    def run(self):
        para = self.GetCurrStatus()
        if not os.path.exists('c:\\wlanacname.txt'):
            with open('c:\\wlanacname.txt', 'w') as f:
                print(
                    'you are first running,please keep outline to acquire the parameter')
                input()
                if not para:
                    print("online now,exit")
                    exit(0)
                else:
                    f.write(para[0])
        else:
            with open('c:\\wlanacname.txt', 'r') as f:
                acname = f.read()
                self.base_data['wlanacname'] = acname
        while True:
            if not para:
                print("online now,you can press enter to logout")
                input()
                self.Logout()
            else:
                print('outline now,you can press enter to login')
                input()
                self.Login()
            para = self.GetCurrStatus()


test = CMCC('', '')
test.run()
