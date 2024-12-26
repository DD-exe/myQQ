## myQQ
我选择了【项目四：聊天程序】作为开发项目，采用C++/winAPI为实现技术。项目可打包为Windows应用(setup.exe、.msi)，在包括Windows 11在内的Windows平台上运行。<br>
项目通过winsock库实现基于tcp协议的通讯，通过服务端中转，将文本消息传输给给定ip、端口的网络位置：如果该处恰好存在一个开启的客户端，后者将接收并显示消息。<br>
考虑到通讯双方隐私，服务端并未设定有显示消息内容的功能。<br>
项目采用了多线程设计，将客户端和服务端集成化。用户可仅通过打开不同窗口，建立任意多的客户端和服务端。<br>
项目框架如下：
```
myQQ
│  .gitignore
│  client.cpp					// 客户端、客户端设置callback函数
│  framework.h
│  listen.cpp					// 端口监听线程函数
│  myQQ.aps
│  myQQ.cpp					// 入口函数和消息循环
│  myQQ.h						
│  myQQ.rc					// 对话框控件设置文件
│  myQQ.sln
│  myQQ.vcxproj
│  myQQ.vcxproj.filters
│  myQQ.vcxproj.user
│  readme.md
│  record.txt
│  resource.h
│  server.cpp					// 服务端callback函数
│  targetver.h
│  tools.cpp					// 宽字符串处理和tools层实现函数
│
├─icon						// 图标文件和源文件
│      myQQ.ico
│      myQQ.ico.kra
│      small.ico
│      small.ico.kra
│
├─image						// 图片资源文件
│      touhou.bmp
│      touhou.ico
│      touhou.jpg
│      touhou_s.bmp
│
├─myOldQQ					// 老版本回滚预备
│      myMFCQQ.rar
│      myQQ.rar
│      myQQ1.0.rar
│      myQQ1.2.rar
│      myQQ1.21.rar
│ 
└─x64
    ├─Debug
    └─Release      
```