- 每次我询问问题的时候你不要直接给出所有的代码
- 本次项目开发主要的是学习而不是呃完成整个项目为目的你不能因为达成某个目的然后强行筛入一些硬性编码或者硬编码，
- 以项目架构以及后续拓展性为主要核心 已学习c加加Qt以及tree-setter语法为出发点
- 每次回答 提供完整答案的一部分然后告诉我 要在哪里写 为什么这样写写 有什么用 对整体架构的好处
- 每次回答不用把整个链路全部写完整只需要给出一部分答案即可然后我说下一步的时候你再给出下下一部分
- 考虑 单例模式 工厂模式 pimpl模式 策划模式 命令模式 插件模式 等等优秀的设计模式
- 我说下一步或者任何表示我已经看完当前代码代码的意思表示的时候 你再进行下一次回答
- 在这过程中我会询问一些相关的语法问题你也做同样的解答但是如果我只要表达上一条意思的时候你就继续回答没有完成的任务

src/
├── core/
│   └── LosTreeSitter/
│       ├── LosTreeSitterDocument.h
│       ├── LosTreeSitterDocument.cpp
│       ├── LosTreeSitterLanguage.h
│       ├── LosTreeSitterLanguage.cpp
│       ├── LosTreeSitterNodeInfo.h
│       ├── LosTreeSitterFoldRange.h
│       ├── LosTreeSitterFoldingProvider.h
│       └── LosTreeSitterFoldingProvider.cpp
│
├── model/
│   └── LosCodeFolding/
│       ├── LosFoldRegion.h
│       ├── LosCodeFoldingModel.h
│       └── LosCodeFoldingModel.cpp
│
└── view/
    ├── LosEditorUi/
    │   ├── LosEditorUi.h
    │   └── LosEditorUi.cpp
    │
    └── LosLineNumberUi/
        ├── LosLineNumberUi.h
        └── LosLineNumberUi.cpp


QTextDocument
    │
    │ 文本变更
    ▼
LosTreeSitterDocument
    │
    │ 解析 AST
    ▼
LosTreeSitterFoldingProvider
    │
    │ 生成折叠范围
    ▼
LosCodeFoldingModel
    │
    │ 保存嵌套关系、折叠状态
    ▼
LosEditorUi
    │
    │ setVisible(false)
    ▼
QTextBlock
