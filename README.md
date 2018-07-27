# EventCleaner
A tool mainly to erase specified records from Windows event logs, with additional functionalities.

**EventCleaner suspend**

suspends  logging threads of the windows event log service, after which no logs will be recorded   
暂停日志线程,停止日志记录

**EventCleaner normal**

resumes logging threads of the windows event log service   
恢复日志线程
  
**EventCleaner closehandle**
	
frees handle acquired exclusively by the windows event log service to security.evtx,   
so that we can manipulate the file   
解除 security.evtx的文件占坑

**EventCleaner 100**

remove the log with id 100   
删除 event record id 为 100 的日志


**How to use:**

 1. EventCleaner closehandle
 2. EventCleaner 100
 3. EventCleaner suspend
 4. do anything without worrying about logs
 5. EventCleaner normal
 6. delete EventCleaner

 
by pr0mise @ 360 A-TEAM
 
**Thanks for the help from darkray & Drknight**


*** 

**Screenshots**



![test](https://raw.githubusercontent.com/360-A-Team/EventCleaner/master/ScreenShots/2.png)

![test](https://raw.githubusercontent.com/360-A-Team/EventCleaner/master/ScreenShots/3.png)

![test](https://raw.githubusercontent.com/360-A-Team/EventCleaner/master/ScreenShots/4.png)

![test](https://raw.githubusercontent.com/360-A-Team/EventCleaner/master/ScreenShots/5.png)

![test](https://raw.githubusercontent.com/360-A-Team/EventCleaner/master/ScreenShots/6.png)

![test](https://raw.githubusercontent.com/360-A-Team/EventCleaner/master/ScreenShots/7.png)

![test](https://raw.githubusercontent.com/360-A-Team/EventCleaner/master/ScreenShots/8.png)






