【zmq介绍】
zmq是一个多对多的消息队列，一个节点可以被多个节点订阅，一个节点也可以订阅多个节点
本模块使用zmq传输Log消息

【配置】
每一个publish log的进程，配置好publish的地址，以及sub来自logviewer的命令的地址
LogViewer配置好pub命令的地址，及需要sub的logPublisher的地址
LogServer配置好保存Log文件的路径，及需要sub的logPublisher的地址

【功能】
LogViewer
每一个Category的Log消息，拥有单独的Log展示页面
LogViewer会Sub所有配置好的地址中publish的log消息
所有的Log（包含defaultCategory的Log）会显示到default页面
所有的Warn及以上的Log会显示到Warn页面
每一个Category的Log会显示到对应的页面
可以设置每一个Category的LogLevel
小于LogLevel的Log，不会被Format及Publish

LogServer
只允许开启一个实例
禁用关闭按钮（任务栏右键菜单关闭按钮，暂未找到合适方式禁用）
每一个Category的Log消息，会保存到不同的文件
LogViewer会Sub所有配置好的地址中publish的log消息
所有的Log（包含defaultCategory的Log）会保存到SilicoolLog.log
所有的Warn及以上的Log会保存到Warn.log
每一个Category的Log会保存到对应文件
可以配置是否输出Log到控制台

Logger
可以配置是否输出Log到控制台
本地调试时，可以启动一个LogServer查看Log消息，或者配置Logger输出到控制台

【使用】
为了避免主程序崩溃时，Sub不到关键的Log信息，因此将LogServer作为一个独立应用程序，建议主程序启动时，拉起LogServer
可以将LogViewer嵌入到主程序，用于查看Log
每一个需要Publish log消息的应用程序，需要在main函数开始处添加以下两行代码：
QObjectFactory::registerClass<LogCategoryConfig>();
Logger::getIns()->init();

每一个需要publish log消息的应用程序，可以拥有多个category，也建议每一个模块使用不同的category，方便查看Log
例如，AA1 定义LUT，SUT1, LensLoader，AACore1等category，AA2定义SUT2，SensorLoader，AACore2等category
----定义及使用category---
#include "Logger.h"
在全局区域定义要使用的category
SILICOOL_CREATE_LOGGING_CATEGORY(sutCate, "SutModule")
接下来就可以在函数中使用了
qcDebug(sutCate()) << "msg";    
qcInfo(sutCate()) << "msg";   
qcWarning(sutCate()) << "msg";   
qcCritical(sutCate()) << "msg";   
若要在其他文件中使用此category，若该文件不包含定义该category的头文件，则需在该文件的全局区域声明：
#include "Logger.h"
SILICOOL_DECLARE_LOGGING_CATEGORY（sutCate)

【Log规范】
辅助调试使用qcDebug
运行日志使用qcInfo
报警及错误必须使用qcWarning或qcCritical
若有严重错误导致程序无法执行，可以使用qFatal，qFatal将打印一条消息后调用terminal中止程序


