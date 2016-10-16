import urllib
import urllib.request
import re
import os
import time
import threading
MAX_TIME=2000
USER=''  #这里填写手机号
PASWD='' #这里填写密码
TIME=15	#每隔多少分钟断次线

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
	url='http://s1.hdslb.com/'
	data=urllib.request.urlopen(url).read().decode('utf-8')
	R=re.compile('cname\".+?\"(.+?)\".+?wlanuserip.+?=\"(.+?)\"',re.S) #re.S 使.支持换行符识别
	L=R.findall(data)
	if len(L)==0:
		return False
	else:
		return L[0][0],L[0][1]
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
		print('按回车下线,离自动下线还有：%d分钟'%(TIME))
		time.sleep(3)
		TIME=TIME-0.05
	WriteRemainTime(int(GetRemainTime())-int(_TIME-TIME))
	LogOut(DATA)
	os.system('cls')
	print('下线成功')
	
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
			
		IsQuit=False
		TIME=_TIME
		os.system('cls')
		print('离线状态，定时%d分钟后自动下线,本月还剩%s分钟'%(TIME,GetRemainTime()))
		input()
		DATA=inf
		Login(DATA)
		t1=threading.Thread(target=timer) #开一个线程计时
		t1.setDaemon(True) #设为守护进程
		t1.start()
		input()
		IsQuit=True
		t1.join()
		inf=False
InorOut()


