---
title: CShape-WinForm界面布局
toc: true
categories:
  - null
tags:
  - null
---



<!--more-->

<br/>

#### 按顺序批量更改控件名

在放置控件时，VS会自送以控件类型+数字递增的方式给控件自动命名，如拖动一组CheckBox，会自动命名成：checkBox1、checkBox2、checkBox3、...

VS的设计编辑器只支持一个一个地重命名。

使用文本编辑器来打开这个Designer.cs文件，替换全部的checkBox为我们想要定义的变量名。

再回到界面编辑器，会提示重载，点击确认，可以看到此时变量名已经批量改变。

（此方法需要一批一批的改，如果有2组功能不同的CheckBox一起添加了，就不行了）

```C#
for(int i= 0;i<10;i++)
{
    Label lb=new Label();
    lb.Name="Label"+(i+1);
    this.Controls.Add(lb);
}
```

#### 按名称批量操作控件

```C#
this.Controls.Find("button" + i, true)[0].Text = string.Format("按钮{0:00}",i);

// 如果想使用控件的特定属性
for (int i = 1; i <= 16; i++)
{
    CheckBox _finded = (CheckBox)this.Controls.Find("checkBox" + i.ToString(), true)[0];
    if(_finded.Checked)
    {
        // Do something...
    }
}
```

如果所有的button都是放在Form上面的，其中this就是指form：

```C#
foreach (var item in this.Controls)
{
	if (item.GetType() == typeof(Button))
	{
		// Do something...
	}
}
```



#### CheckBox分小组

放到Panel中，设置无Border。

#### TextBox 更改大小

找到Multiline属性，我们可以发现它是处于“False”状态，点击方框右边更的向下小箭头改为“True”

然后我们再去改变TextBox的大小，就能改变大小了

#### Label 更改大小

更改AutoSize属性,该属性为true时,控件会自动根据其Text值自动设置宽度,用户不能自己修改。

设置为false才能修改其width值。