---
title: C# - Unit Test
toc: true
date: 2020-08-03 15:37:08
categories:
  - tech
---

在上线了几个产品之后，深刻的领悟到，完备的测试是程序稳定性的唯一保证。

之前一直使用的是Excel测试用例条目来记录测试方法以及测试结果，记录繁琐且不甚完全，尤其对于大量的输入输出，需要大量的记录和操作，面对一个几百行的表，任何一个人都会头大。

在一次搜索单元测试的时候，找到了C# Unit Test测试框架的Blog，用了之后发现真香。

测试输入和测试结果可以程序化比对，每一个输入和输出都清楚明了，且可以重复执行。有了完备的测试，还可以反向推进改进软件接口，提升软件的质量和稳定。

所以稍微研究了C#的单元测试框架，总结如下。

<!--more-->

<br/>

Demo基于VS Community 2017 + MSTest 2.1.2。

### 单元测试项目

1、 新建一个控制台应用。名称叫：UnitTestDemo

![UnitTestDemo.jpg](/images/CSharp-UnitTest/UnitTestDemo.jpg)

2、 添加一个待测试的方法。

```C#
public static int Add(int pNum1,int pNum2)
{
    {
        return pNum1 + pNum2;
    }
}
```

3、 新建一个单元测试项目。名称叫：UnitTestDemoTest

![UnitTestDemoTest.jpg](/images/CSharp-UnitTest/UnitTestDemoTest.jpg)

写代码。调用要测试的方法。

修改类名，及方法名。UnitTest1重命名为ProgramTest，方法名：TestMethod1改为：AddTest。

这里说明一下，方法面上面的特性：[TestMethod]这是必须的。告诉编译器这是一个测试法式。

```C#
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using UnitTestDemo;

namespace UnitTestDemoTest
{
    [TestClass]
    public class ProgramTest
    {
        [TestMethod]
        public void AddTest()
        {
            int a = 100;
            int b = 10;
            Assert.AreEqual(Program.Add(a, b), 110);
        }
    }
}
```

4、 打开“测试资源管理器”，在“测试”->窗口->测试资源管理器

![TestWindowOpen.jpg](/images/CSharp-UnitTest/TestWindowOpen.jpg)


![TestWindow.jpg](/images/CSharp-UnitTest/TestWindow.jpg)

在上面右击。选“运行选定测试”，

![TestRun.jpg](/images/CSharp-UnitTest/TestRun.jpg)

可以看到前面多了一个通过标记。表示测试通过。
![TestOk.jpg](/images/CSharp-UnitTest/TestOk.jpg)

如果未通过，是个红色的X。



![TestFailed.jpg](/images/CSharp-UnitTest/TestFailed.jpg)

5、 以上是手工建立测试项目的。还有一种快捷的方法建立测试项目。在要测试的方法行，右击。选择创建单元测试。可弹出建立单元测试对话框。

![TestUnitCreate.jpg](/images/CSharp-UnitTest/TestUnitCreate.jpg)

6、对于同一测试的不同输入输出，我们可以使用传值测试，用[DataTestMethod]和[DataRow]来标识数据集。

对于以下数据类：

```C#
public class StrIP
{
    private string ip;
    public string IP{
        get => ip;
        set
        {
            value = value.Trim();
            if (value == "")
            {
                throw new ArgumentException("IP为空");
            }

            string pattern = @"^((2[0-4]\d|25[0-5]|[01]?\d\d?)\.){3}(2[0-4]\d|25[0-5]|[01]?\d\d?)$";
            if (Regex.IsMatch(value, pattern))
            {
                ip = value;
            }
            else
            {
                throw new ArgumentException("IP非法");
            }
        }
    }
}
```

可以使用以下用例进行测试：

```c#
[DataTestMethod]
[DataRow("", "IP为空")]
[DataRow(" ", "IP为空")]
[DataRow("www.baidu.com", "IP非法")]
[DataRow("中文", "IP非法")]
[DataRow("192.168.0.256", "IP非法")]
public void IP_Test(string ip,string str_error)
{
    StrIP str_ip = new StrIP();
    try
    {
        str_ip.IP = ip;
    }
    catch (Exception ex)
    {
        Assert.AreEqual(ex.Message, str_error);
        return;
    }
    Assert.Fail();
}
```

运行测试用例：

![DataTest.jpg](/images/CSharp-UnitTest/DataTest.jpg)

7、在C#调试时，我们经常使用Console类来打印输出，程序调试时可以在“输出”栏看到程序输出。但在运行单元测试的时候，在“输出”栏中看不到输出，输出被移到了“测试资源管理器”中。

![TestOutput.jpg](/images/CSharp-UnitTest/TestOutput.jpg)

* [Microsoft Docs Example](https://docs.microsoft.com/en-us/visualstudio/test/walkthrough-creating-and-running-CSharp-UnitTests-for-managed-code?view=vs-2019)

<br/>

#### Assert类

可以看到，在Demo示例中，我们使用Assert.AreEqual来判断程序结果和预测之间是否相等。而在单元测试项目中，Assert类提供了许多方法来判定输出结果，如下表所示：

| 方法                       | 说明                                         |
| -------------------------- | -------------------------------------------- |
| AreEqual                   | 验证两个值相等，否则测试失败。               |
| AreNotEqual                | 验证两个值不相等，否则测试失败。             |
| AreSame                    | 验证两个内容引用的对象相同，否则测试失败。   |
| AreNotSame                 | 验证两个内容引用的对象不相同，否则测试失败。 |
| IsTrue                     | 验证条件为True，否则测试失败。               |
| IsFalse                    | 验证条件为False，否则测试失败。              |
| IsNull                     | 验证对象为null，否则测试失败。               |
| IsNotNull                  | 验证对象不为null，否则测试失败。             |
| Fail                       | 如果运行到此语句，则测试失败。               |
| IsInstanceofType           | 验证对象是某类型， 否则测试失败。            |
| IsNotInstanceofType        | 验证对象不是某类型， 否则测试失败。          |
| ThrowsException<Exception> | 验证操作会导致某类异常，否则测试失败。       |

* [Microsoft Docs Reference](https://docs.microsoft.com/en-us/dotnet/api/microsoft.visualstudio.testtools.unittesting.assert?view=mstest-net-1.2.0)

<br/>

#### String Assert类

对于字符串判定，使用StringAssert可能会更方便。

| 方法         | 说明                                                       |
| ------------ | ---------------------------------------------------------- |
| Contains     | 验证第一个字符串是否包含第二个字符串。此方法区分大小写。   |
| Matches      | 验证指定的字符串是否与正则表达式匹配。                     |
| DoesNotMatch | 验证指定的字符串是否与正则表达式不匹配。                   |
| StartsWith   | 验证第一个字符串是否以第二个字符串开头。此方法区分大小写。 |
| EndsWith     | 验证第一个字符串是否以第二个字符串结尾。此方法区分大小写。 |

* [Microsoft Docs Reference](https://docs.microsoft.com/zh-cn/previous-versions/ms245503(v=vs.120%29)

<br/>

#### Collection Assert类

在单元测试中验证与集合关联的命题。

| 方法                       | 说明                                             |
| -------------------------- | ------------------------------------------------ |
| AllItemsAreNotNull         | 验证指定集合中的所有项是否都不为 nullnull 引用。 |
| AllItemsAreInstancesOfType | 验证指定集合中的所有元素是否为指定类型的实例。   |
| AllItemsAreUnique          | 验证指定集合中的所有项是否都唯一。               |
| AreEqual                   | 验证两个指定的集合是否相等。                     |
| AreNotEqual                | 验证两个指定的集合是否不相等。                   |
| AreEquivalent              | 验证两个指定的集合是否等效。                     |
| AreNotEquivalent           | 验证两个指定的集合是否不等效。                   |
| Contains                   | 验证指定的集合是否包含指定的元素。               |
| DoesNotContain             | 验证指定的集合是否不包含指定的元素。             |
| IsSubsetOf                 | 验证第一个集合是否为第二个集合的子集。           |
| IsNotSubsetOf              | 验证第一个集合是否不是第二个集合的子集。         |

* [Microsoft Docs Reference](https://docs.microsoft.com/zh-cn/previous-versions/ms245294(v=vs.120%29)

<br/>

### 其它框架

VS在默认创建Unit Test工程时，默认使用的是MSTest。

除此之外，C#常用的框架还有NUnit、XUnit，相较于MSTest，它们拥有较好的弹性，在标注测试类和方法上略微不同，在MSTest无法满足应用的时候，可以考虑其它的框架。

* [NUnit](https://nunit.org/)
* [XUnit](https://xunit.net/)

<br/>

## 参考：

* https://blog.51cto.com/57388/2178761

* https://docs.python.org/zh-cn/3/library/unittest.html