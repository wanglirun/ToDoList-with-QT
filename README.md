# TodoList
* **一个简单的用QT开发的桌面待办小程序**
![image](https://github.com/wanglirun/ToDoList-with-QT/blob/main/sample.jpg)

* **备注**：为了方便调试，每当程序运行时，都会清空数据库中的表。

* **基本架构**
  
  * **三个用于选择时间的子界面**：初始化时，设置属性deleteOnClose，避免子窗口内存泄露。
    
    1. 新建task时，点击日历，弹出到期时间/提醒时间的子页面。（若没有选择一个日期，则<确定>按钮禁用）
    
    2. 设置task的到期日期时，弹出的子页面。
    
    3. 设置task的提醒时间时，弹出的子页面。
  
  * **分组**:
    
    * 当新建一个分组“new_table”时，数据库相应地建一个表"new_table"。
    
    * 同时，为了给该表唯一的标识，在数据库中创建一个记录分组的表myGroups，赋予每个表(分组)一个独一无二的id，在new QListWidgetItem时，将id存储在QListWidgetItem的userRole中。
    
    * 如此一来，当在不同分组间跳转时，根据userRole中存储的id，即可寻找到相应的表名，显示该表中的待办。
    
    * **编写辅助类Group**,用于创建新表（分组）、判断能否创建新表（新分组）（分组名不能相同，否则无法创建新表）、向myGroups中插入新表的条目、通过myGroups的id查询表名。
    
    * 初始化数据库时，首先创建<我的待办>表，该表的id一直为1。
    
    * 在MainWindow中设置一个全局变量currentGroupId，当点击不同的分组时，更新currentGroupId，指明当前应该用哪个表，调用group实例得到表名。
    
    * 刷新：在不同分组间跳转时，表也随之变换，编写一个refresh函数，根据得到的表名，重构显示待办的QTreeWidget。每次点击一个新分组，执行refresh一次。
  
  * **待办设置面板**：
    
    * 每一个待办在其所在的表中，都有一个独一无二的ID。当新建一个待办时，将该id存储在QTreeWidgetItem中的userRole里。点击某待办时，会清空task实例，并取出该待办的id，赋值给task的id。
  
  * **我的首页：**
    
    * **编写一个辅助类task**，主要用于
      
      1. 向表中插入待办条目；
      
      2. 打开数据库，更新待办的属性。
    
    *  task设置为public，无论是在mainWindow获得新待办名/新备注，还是在选择时间的界面获得新提醒时间/移除提醒时间，都能给task的成员变量赋值，随后调用task的set函数，来更新某某属性。
    
    * 比如，待办设置面板模块所述，当待办设置面板跳出时，task的id已经被设置为了相应的itemID,调用task的set系列函数，先通过groupID找到相应的表名，再根据itemID找到相应的待办条目，根据set的类型更新即可。
    
    * 刷新：和分组中刷新稍有不同，但也大同小异。

* **备注：** 时间原因，将已完成的待办unchecked后，该待办无法回到未完成的待办中。




































