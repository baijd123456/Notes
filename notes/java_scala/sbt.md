<!-- TOC -->

- [*sbt* 概述](#sbt-概述)
- [安装与配置](#安装与配置)
- [启动与使用](#启动与使用)
	- [使用 *Giter8* 模版](#使用-giter8-模版)
- [项目基本结构](#项目基本结构)
	- [默认路径](#默认路径)
- [构建配置](#构建配置)
	- [重新加载配置](#重新加载配置)
	- [访问构建信息](#访问构建信息)
- [依赖管理](#依赖管理)
	- [常用依赖](#常用依赖)
- [编译参数](#编译参数)
- [*Lightbend Activator* (已废弃)](#lightbend-activator-已废弃)
	- [安装与配置](#安装与配置-1)
	- [基本操作](#基本操作)
	- [后记](#后记)

<!-- /TOC -->



## *sbt* 概述
`sbt`全称`Simple Build Tool`，是`Scala`项目的标准构建工具，类似于`Java`下的`Maven`/`Groovy`中的`Gradle`。

与其它`Java`构建工具类似，`sbt`的核心功能如下：

- 项目的构建。
- 项目依赖自动化管理。
- 提供统一的工程结构。
- 提供交互式的`sbt shell`。



## 安装与配置
主流`Linux`发行版的仓库中，一般都包含`sbt`，可以使用发行版的包管理器安装`sbt`，以`ArchLinux`为例：

`# pacman -S sbt`

在Windows环境下，可以从官网`http://www.scala-sbt.org/download.html`中下载。  
下载完成之后解压并将目录下的`bin`目录加入`PATH`中。

如果已经安装了`Activator`，则无需再安装`sbt`，`Activator`中已经包含了`sbt`。



## 启动与使用
在任意目录下输入`sbt`指令，即可进入`sbt shell`，`sbt`会将当前路径作为`sbt`项目的**根目录**。

终端会输入如下信息：

```
[info] Set current project to sbt (in build file:/home/dainslef/Downloads/SBT/)
>
```

若路径为有效的`sbt`项目路径，则在`sbt shell`中输入`run`指令会尝试编译源码执行项目。

旧版的`sbt`中(`sbt 0.7.x`之前)，在`sbt shell`中输入`run`指令会在路径下生成完整的sbt项目结构，但新版的sbt已**不提供**此特性(可使用`sbt new`指令创建项目)。

sbt的常见指令有：

- `compile` 编译项目
- `update` 更新依赖
- `test` 运行测试用例
- `run` 运行项目
- `clean` 清理项目缓存
- `package` 将项目打包
- `console` 进入Scala REPL
- `reload` 重新加载项目的`build.sbt`构建配置

`sbt`指令在`sbt shell`内使用，部分指令也可作为参数跟在`sbt`指令之后直接在命令行中使用。

### 使用 *Giter8* 模版
`Giter8`是由`Nathan Hamblen`在`2010`年开始发起的模版项目，目前由`foundweekends`项目维护。  
在`sbt 0.13.13`版本后，可使用`sbt new`指令创建基于`Giter8`模版的项目：

```
$ sbt new [Giter8模版名称]
```

使用`sbt new`指令新建模板项目，根据模板类型，会交互式地要求输入一些项目信息。  
之后会在`当前目录/项目名称`路径下生成指定模板的目录结构。

`Lightbend`系列的各项技术均提供了基于`Giter8`的官方项目模版，常用模版名称如下：

- `scala/scala-seed.g8` 普通Scala项目模版
- `akka/akka-scala-seed.g8` Akka项目模版
- `playframework/play-scala-seed.g8` PlayFramework项目模版



## 项目基本结构
`sbt`项目结构与`Maven`项目类似。一个基本的sbt项目具有以下路径结构：

```
项目名称
├── build.sbt # 项目依赖关系(构建定义)
├── project
│    ├── plugins.sbt # 添加sbt插件
│    └── build.properties # 构建规则与参数
└── src # 源码目录
     ├── main
     │    ├── resources
     │    └── scala
     │         ├── Xxx.scala
     │         ├── Xxx.scala
     │         └── ...
     └── test
          ├── resources
          └── scala
               ├── Xxx.scala
               ├── Xxx.scala
               └── ...
```

新创建的项目没有`target`目录，但在`sbt shell`中执行了`run`之后还会生成`target`和`project/target`目录。  
`target`目录中包含的所有内容均由编译系统生成，将项目目录加入版本控制时需要忽略这些目录。

### 默认路径
在`sbt`项目中，直接使用**相对路径**访问目录/文件，则默认起始路径为项目的**根路径**(即与`build.sbt`文件在统一路径下)。

在项目**根目录**下创建文件`temp.txt`：

```scala
import scala.reflect.io.File

object Main extends App {
  // 文件生成在根目录下，路径为 [项目名称]/temp.txt
  File("temp.txt").createFile(false)
}
```

- 使用**项目相对路径**访问资源目录

	`sbt`项目中的`src/main`与`src/test`下都存在`resources`目录。  
	`resources`路径下的文件可以根据项目的相对路径来访问。  
	假设`src/main/resources`路径下存在文件`temp.txt`，打印文件内容：

	```scala
	import scala.reflect.io.File

	object Main extends App {
	  // 使用项目相对路径
	  File("src/main/resources/temp.txt").lines foreach println
	}
	```

- 访问输出路径下的资源文件

	在项目构建时，`resources`目录下的文件会被复制到输出路径下，并保留子目录结构。  
	从输出目录获取资源文件可以使用`Class`类型的`getResource()`方法：

	```java
	public java.net.URL getResource(String name);
	```

	`getResource()`方法返回的路径为`URL`类型，可以使用`getFile()`方法将其转换为文件路径字符串。  
	`getResource()`方法接收的路径参数可以为**相对路径**或**绝对路径**。

	当参数为**绝对路径**时，参数路径会以输出路径为起点。  
	假设`src/main/resources`路径下存在文件`temp.txt`，则打印该文件内容：

	```scala
	import scala.reflect.io.File

	object Main extends App {
	  File(getClass.getResource("/temp.txt").getFile).lines foreach println
	}
	```

	当参数为**相对路径**时，参数路径会以**当前类所属包**的输出路径为起点。

- `jar`包资源文件

	与项目构建时类似，将项目打包为`jar`包后，`resources`目录下的文件会被复制到`jar`包内部的**根目录**。  
	运行`jar`包时，起始相对目录的位置为`jar`所处的路径，**不能**通过`src/main/resources/***`相对路径访问资源目录下的文件。

	`jar`包内部文件应使用`getResource()`方法来获取路径，且应使用`URL`的形式表示，直接使用文本路径不能被正确识别。  
	`jar`包内部的`URL`路径格式为`jar:file:...`。



## 构建配置
`sbt`项目根目录下的`build.sbt`定义了项目的构建配置。  
`project`目录下也可以添加`*.scala`构建定义。

可以在`build.sbt`文件中设定项目的名称、版本信息、构建规则、依赖等配置。  
`build.sbt`文件遵循`Scala`语法。

一个简单的`build.sbt`文件内容如下所示：

```scala
name := "项目名称"
version := "项目版本号"
scalaVersion := "Scala编译器版本号"

libraryDependencies ++= Seq(
  "xx" % "xx" % "xx", //项目Java依赖
  ...
   "xx" % "xx" %% "xx", //项目Scala依赖
  ...
)

scalacOptions ++= Seq(
  "-xxx", //编译器选项
  ...
)

enablePlugins(Xxx) //启用插件
```

### 重新加载配置
`sbt shell`只在**启动时**读取一遍构建配置。  
若在`sbt shell`开启之后`build.sbt`文件发生了修改，则已经开启的`sbt shell`依旧使用之前的构建配置。  
若需要已开启的`sbt shell`使用新的构建配置，则应在`sbt shell`中使用`reload`指令重新加载构建配置。

### 访问构建信息
`sbt`没有提供访问`build.sbt`中项目构建信息的接口，使用`sbt`插件`sbt-buildinfo`可以让项目访问`sbt`的构建信息。  
在`sbt`项目中的`project/plugins.sbt`文件中引入该插件：

```scala
addSbtPlugin("com.eed3si9n" % "sbt-buildinfo" % "版本号")
```

在项目构建配置文件`build.sbt`中启用`sbt-buildinfo`插件：

```scala
enablePlugins(BuildInfoPlugin)
```

`sbt-buildinfo`插件的原理是利用`build.sbt`中的项目构建信息在项目构建时生成额外的源码，
并以**单例对象**的形式将构建信息提供给项目源码进行访问。

启用`sbt-buildinfo`插件后会增加插件相关的配置项。  
将`build.sbt`中的`name、version、scalaVersion、sbtVersion`等配置项传入`sbt-buildinfo`插件的`buildInfoKeys`配置项，
通过`buildInfoPackage`配置项设定生成单例的包路径。

在`build.sbt`文件中配置`sbt-buildinfo`插件，实例如下：

```scala
// sbt项目构建信息
name := "xxx"
version := "xxx"
scalaVersion := "2.12.3"
sbtVersion := "0.13.16"

// 启用 sbt-buildinfo 插件
enablePlugins(BuildInfoPlugin)

// 设定构建信息
buildInfoKeys := Seq(name, version, scalaVersion, sbtVersion)
buildInfoPackage := "xxx.yyy.zzz" //将构建信息生成到 xxx.yyy.zzz 包路径中
```

`sbt-buildinfo`插件生成的单例对象结构如下所示：

```scala
case object BuildInfo {
  /** The value is "xxx". */
  val name: String = "xxx"
  /** The value is "xxx". */
  val version: String = "xxx"
  /** The value is "2.12.2". */
  val scalaVersion: String = "2.12.2"
  /** The value is "0.13.15". */
  val sbtVersion: String = "0.13.15"
  override val toString: String = {
    "name: %s, version: %s, scalaVersion: %s, sbtVersion: %s" format (
      name, version, scalaVersion, sbtVersion
    )
  }
}
```



## 依赖管理
通过设定`build.sbt`文件中的`libraryDependencies`变量即可向项目中添加**托管依赖**。  
`libraryDependencies`配置项实际上是一个类型为`sbt.SettingKey[scala.Seq[sbt.ModuleID]]`的**变量**。  
每一项依赖由`sbt.ModuleID`类型定义，一个具体的依赖项格式如下所示：

```scala
// 普通依赖，通常适用于Java依赖包
groupID % artifactID % revision
// 在指定配置下的依赖
groupID % artifactID % revision % configuration
// 对于开放源码的库，可以指定在添加依赖时同时下载库源码和Java DOC
groupID % artifactID % revision % withSource() withJavadoc()
```

对于多数使用Scala开发的项目，项目的`artifactID`命名上通常会以使用Scala版本号作为结尾(`Scala`编译器相关模块除外，如`scala-reflect`、`scala-compiler`等)。

在添加`Scala`项目依赖时，使用`%%`操作符连接`groupID`和`artifactID`，则会将当前Scala版本号追加到`artifactID`上：

```scala
groupID %% artifactID % revision
```

等价于：

```scala
groupID % artifactID_[Scala版本] % revision
```

以`Scala 2.12`的`Akka 2.4.17`为例，依赖信息为：

```scala
"com.typesafe.akka" %% "akka-actor" % "2.4.17" //省略Scala版本信息
"com.typesafe.akka" % "akka-actor_2.12" % "2.4.17" //两种表示方式等价
```

`sbt.SettingKey`类型重载了`+=`和`++=`运算符：

- `+=`运算符用于添加单项依赖，如：

	```scala
	libraryDependencies += groupID % artifactID % revision
	```

- `++=`运算符用于添加多个依赖序列，如：

	```scala
	libraryDependencies ++= Seq(
	  groupID0 % artifactID0 % revision0,
	  groupID1 % artifactID1 % revision1,
	  ...
	)
	```

### 常用依赖
`sbt`依赖的描述信息与`Maven`相同，`sbt`允许直接添加`Maven`仓库的依赖，包的信息可以在**Maven中心仓库**搜索到，地址为`http://search.maven.org/`。

一些常用包的`GroupId`和`ArtifactId`信息如下：

| 包介绍 | GroupId | ArtifactId |
|:------|:--------|:-----------|
| MySQL JDBC Driver | mysql | mysql-connector-java |
| Scala Reflect | org.scala-lang | scala-reflect |
| Scala Swing | org.scala-lang.modules | scala-swing_[Scala版本号] |
| Scala Test | org.scalatest | scalatest_[Scala版本号] |
| ScalaFx | org.scalafx | scalafx_[Scala版本号]
| Slick | com.typesafe.slick | slick_[Scala版本号] |
| Akka | com.typesafe.akka | akka-actor_[Scala版本号] |



## 编译参数
通过设定`build.sbt`文件中的`scalacOptions`变量可以控制`scalac`编译器的编译参数。  
设定了编译参数后，执行`compile`指令时会采用设定的编译参数进行编译。

所有`scalac`支持的命令行参数都可添加，如下所示：

```scala
scalacOptions ++= Seq(
  "-feature",
  "-language:implicitConversions"
  "..."
)
```

`Scala`中的部分语言特性需要使用参数`-language:xxx`来开启：

- 使用`-language:help`参数显示所有可使用的语言特性参数。
- 使用`-language:_`参数可以开启所有的语言特性。



## *Lightbend Activator* (已废弃)
`Activator`提供了成套的`Scala`开发环境，相当于：

```
Scala编译器 + sbt + Play Framework + Akka + 项目模版 + 基于Play的WEB端项目管理
```

`Activator`内置了sbt，可以直接使用`Activator`管理、构建sbt项目。

### 安装与配置
多数Linux发行版**没有**将`Activator`添加到仓库中，因而无论是`Linux/Windows`环境下，都需要从官网下载`Activator`。

配置`Activator`方式与`sbt`类似。  
从`http://www.lightbend.com/activator/download`下载完整版的`Activator`，解压后将`bin`目录加入`PATH`环境变量中即可。

### 基本操作
在普通目录中输入`activator`指令会在浏览器中打开Activator的帮助页面。

在`sbt`项目目录中输入`activator`指令会进入`sbt shell`。

其它常见的指令：

- `$ activator ui` 进入WEB端的Activator界面
- `$ activator shell` 进入sbt交互shell
- `$ activator list-templates` 列出模版列表

`Activator`中带有大量的预定义项目模版，使用模版创建项目：

`$ activator new [项目名称] [模版名称]`

`Activator`同样支持与`sbt`相同的指令。

### 后记
在`2017-4-24`的`Lightbend`官方新闻(`https://www.lightbend.com/blog/introducing-a-new-way-to-get-started-with-lightbend-technologies-and-saying-goodbye-to-activator`)中，宣布在`2017-5-24`停止对`Activator`的支持。  
新闻中指出了`Activator`在以下的情形存在缺陷：

- 将`Activator`良好地运行在多样的机器类型上。
- 将`Activator`可靠地运行在多样的网络环境上。
- 处理双重要求：`保持更新`与`从不更新`。

鉴于`Activator`无法良好地实现以上要求，`Lightbend`停止了对`Activator`的支持并提供了以下替代方案：

- 基于`Web`技术的`Project Starter`：

	在页面`http://developer.lightbend.com/start/`中直接创建项目模版并将其下载到本地。

- 使用新增的`sbt new`指令：

	`Giter8`项目提供了`Lightbend`各类技术对应的项目模版，使用`sbt new [Giter8模版名称]`创建对应的项目模版。
