import urllib
import urllib.request
import re
import os
import time
import threading
MAX_TIME=2000
USER=''  #这里填写手机号
PASWD='' #这里填写密码
TIME=5    #每隔多少分钟断次线

_TIME=TIME
DATA=[]
IsQuit=False
def GetRemainTime():
    file=open('C:\\time.txt','r')
    Rtime=file.read()
    file.close()
    return Rtime
def WriteRemainTime(t):
    file=open('C:\\time.txt','w')
    file.write(str(t))
    file.close()
def GetIp():
    x=os.popen('ipconfig').read()
    return re.search(r'172.+',x).group(0)
def GetInf():
    #status=os.system('ping 8.8.8.8 -n 1')
    #if status==0:
        #return False
    url='http://s1.hdslb.com/'
    data=urllib.request.urlopen(url).read().decode('utf-8')
    R=re.search(r'"wlanacname" value="(.+?)".+?value="(.+?)"',data,re.S) #re.S 使.支持换行符识别
    if R:
        return R.group(1),R.group(2)
    else:
        return False
def Login(inf):
    url='http://211.138.195.124/suiexingclient.jsp'
    pd='USER=%s&PWD=%s&pwdtype=1&forceflag=0&clienttype=PC,windows,2.5.0&wlanacna\
me=%s&wlanuserip=%s&actiontype=LOGIN&wlanacssid=&languagetype=&logonsessid=\
'%(USER,PASWD,inf[0],inf[1])
    pd=pd.encode('utf-8')
    res=urllib.request.urlopen(url,pd)
def LogOut(inf):
    url='http://211.138.195.124/suiexingclient.jsp'
    pd='USER=%s&PWD=%s&pwdtype=1&forceflag=0&clienttype=PC,windows,2.5.0&wlanacna\
me=%s&wlanuserip=%s&actiontype=LOGOUT&wlanacssid=&languagetype=&logonsessid=\
'%(USER,PASWD,inf[0],inf[1])
    pd=pd.encode('utf-8')
    res=urllib.request.urlopen(url,pd)
    #print(res.read().decode('GBK'))
def timer():
    global IsQuit
    global TIME
    while TIME>0 and IsQuit==False:
        os.system('cls')
        print('离自动下线还有：%d分钟'%(TIME))
        time.sleep(3)
        TIME=TIME-0.05
    WriteRemainTime(int(GetRemainTime())-int(_TIME-TIME))
    LogOut(DATA)
    os.system('cls')
def InorOut():
    global DATA
    global TIME
    global IsQuit
    while True:
        inf=GetInf()
        if inf==False:
            print('在线状态，尝试下线')
            DATA=['0066.0025.250.00',GetIp()]
            LogOut(DATA)
            continue
        os.system('cls')
        print('离线状态，定时%d分钟后自动下线,本月还剩%s分钟'%(_TIME,GetRemainTime()))
        input()
        DATA=inf
        Login(DATA)
        TIME=_TIME
        timer()
InorOut()


