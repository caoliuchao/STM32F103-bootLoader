#STM32F103-bootLoader  
============================================================================
__利用SD和fatfa文件系统，在线更新程序固件，之后跳到用户的APP执行程序__ 
 
__本系统使用的主控芯片为STM32F103ZET6，SD驱动移植ST官方的驱动，使用的是STM32的SDIO接口__  
__文件系统使用的是fatf，FatFs模块是用ANSI C编写的中间件，只要编译器遵循ANSI C，它都是平台无关的。__  
__只要编译器支持ANSI C即可。如果想要了解fafs的移植教程或者查看fatfs的函数使用方法，可以访问fatfs官方网址了解更多__  
__fatfs官方网站:__http://elm-chan.org/fsw/ff/00index_e.html   
__SD的在fatfs的根地址标签默认为为0：__  
__如果你的更新文件在System文件下，你可以这样输入路径可以这样输入："0:/System/Template.bin"__  
__注意：程序中需要你自己加入一旦更新失败的报警信息，默认是利用usart向上位机打印报警信息,__
__你也可以加入自己的报警信息__




