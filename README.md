# MarkdownEditor

## 一、文件结构

> .
> ├── CMakeLists.txt
> ├── README.md
> ├── build
> ├── doc
> ├── include
> │   ├── common
> │   ├── edit
> │   ├── logging
> │   ├── markdown_editor.h
> │   ├── mdfile
> │   ├── sym
> │   └── utils
> ├── logs
> ├── src
> │   ├── edit
> │   ├── logging
> │   ├── main.cpp
> │   ├── markdown_editor.cpp
> │   ├── mdfile
> │   └── utils
> └── test



## 二、功能完成度

已完成lab1要求的全部功能。

- 命令：load、save、ws、switch、close、insert、append-head、append-tail、delete、undo、redo、list、list-tree、dir-tree
- 日志模块，history命令，日志文件目录为`logs/history.log`
- 统计模块，stats命令，日志文件目录为`logs/stats.log`



## 三、运行说明

**编译环境**

```
- cmake version 3.27.7
- clang version 14.0.3
```



**编译运行**

```shell
cd build
cmake .. && make
./md_editor
```

​	程序支持命令与`doc/实验要求.md`一致，新增`exit`命令，退出程序（不进行保存操作）



**测试**

```shell
cd test
bash test.sh
```

​	需要先成功编译，`build`保留了一份可执行程序；采用shell脚本进行测试，测试内容与`doc/testcase.md`一致，执行命令后分别对程序输出与新增的md文件进行比对操作，并打印测试结果。



**日志**

```shell
cd logs
ls
	history.log     stats.log
```



## 四、设计模式

### 1. 组合模式

​	对于Markdown文件本身的组织形式，可以采用组合模式，将对象组合成树形结构以表示"部分-整体"的层次结构。

​	具体代码中，类`MdText`和类`MdTitle`均继承自基类`MdElem`，类`MdText`表示文本项，类`MdTitle`表示标题项，这样设计使树枝和叶子实现统一接口，比如：

```c++
auto MdTitle::Display(int depth, int flag) -> void {
  if (level_ != 0) {
    PrintTreeElem(mdes_.content, depth, flag);
  }
  for (const auto& child : children_list_) {
    if (child == children_list_.back()) {
      child->Display(depth + 1, flag | (1 << depth));
      continue;
    }
    child->Display(depth + 1, flag);
  }
}
```

​	在进行树形展示时，只需要调用树枝节点的展示函数，即可利用相同的接口逐级向下遍历打印。



### 2. 命令模式

​	对于编辑类命令，由于要实现Undo、Redo操作，选择使用命令模式实现，使命令的调用者与实现者进行解耦，在类中管理命令操作的数据结构，从而实现Undo、Redo操作。

```
/include/edit
├── edit_cmd.h(基类)
├── appendhead_cmd.h
├── appendtail_cmd.h
├── delete_cmd.h
└── insert_cmd.h
```



### 3. 装饰器模式

​	ws命令展示时，不同文件具有不同的修饰符（*或<），使用装饰器模式进行修饰，在本次实验要求中，不使用也可，但装饰器模式可以提高展示形式的扩展性。



### 4. 观察者模式

​	要实现日志模块和统计模块与其他功能模块解耦合，选择使用观察者模式，在主功能模块组合一个被观察类`StateSubject`，而日志模块和统计模块均继承一个观察者基类，由类`StateSubject`进行管理。

```c++
class StateSubject {
 public:
  explicit StateSubject(std::vector<std::shared_ptr<MdFile>>& mdfiles);
  auto Add(const std::shared_ptr<Observer>& observer) -> void;
  auto Remove(const std::shared_ptr<Observer>& observer) -> void;
  auto NotifyObservers(const State& state) -> void;
  auto Change(const std::string& command) -> void;

 private:
  State state_;
  std::list<std::shared_ptr<Observer>> observers_;
};
```