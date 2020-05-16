---
title: 'C#代码小结'
toc: true
categories:
  - tech
date: 2020-04-11 23:38:01
---


![vs.jpg](/images/CSharp代码小结/vs.jfif)

用Visual Studio + C#写上位机，获得一份简单的快乐。

Visual Studio提供了非常易用的开发环境，对于Windows.Form来说，控件直接拖拽完成界面开发，点击对应的控件即自动添加click方法，每个控件都有一个事件列表，控件支持哪些事件一目了然，开发速度自然快了。

C#基于Windows .Net，对库的封装程度比较高，还有很多语法糖，VS还提供了自定添加引用，输入提示，类属性/方法提示等操作方法以简便代码编写。但是，使用C#开发的Windows.Form/WPF程序不是跨平台的，只能运行在Windows平台。

总体来说，比较傻瓜式。

<!--more-->

<br/>

### 基础操作

| 操作             | 代码                                   |
| ---------------- | -------------------------------------- |
| 控件使能         | bt.Enabled = true; #false              |
| 标签赋值         | label.Text = text;  text = label.Text; |
| 下拉框默认设置   | cmb.SelectedIndex = 0;                 |
| 下拉框添加项     | cmb.Items.Add(str);                    |
| 清空下拉框       | cmb.Items.Clear();                     |
| 设置窗体顶层显示 | this.TopMost = true;                   |
| 打印输出         | Console.WriteLine("");                 |
| 消息弹窗         | MessageBox.Show("提示消息");           |

<br/>

#### 捕捉异常

```c#
try
{
     n = Convert.ToInt32(textBox1.Text);
}
catch (Exception ex)
{
     Console.WriteLine(ex.Message);
}
```

<br/>

#### 关闭时关闭所有线程

绑定窗体的FormClosed事件：

```C#
private void FormClosed(object sender, FormClosingEventArgs e)
{
    System.Environment.Exit(0);
}
```

<br/>

#### 聚合类型范式

```C#
struct Person
{
    string id;
    public string Id {
        get => id;
        set{
            if (value.Count() > 32 || value.Count() < 1)
            {
                throw (new ArgumentOutOfRangeException("id", value,
                    "0 < id <= 32"));
            }
            else
            {
                id = value;
            }
        }
    }
}

// 使用
Person p1 = new Person
{
    Id = textBox1.Text		# 多个参数使用逗号分隔
};
```

```C#
public class Person
{
    private string id;

    public string ID
    {
        get => id;
        set{
            if (value.Count() > 32 || value.Count() < 1)
            {
                throw (new ArgumentOutOfRangeException("id", value,
                    "0 < id <= 32"));
            }
            else
            {
                id = value;
            }
        }
    }

    public Person(string id)
    {
        if (id.Count() > 32 || id.Count() < 1)
        {
            throw (new ArgumentOutOfRangeException("id", value,
                "0 < id <= 32"));
        }
        else
        {
            this.id = id;
        }
    }
}
```

使用Struct和Class的区别：

* struct是值类型，创建一个struct类型的实例被分配在栈上；class是引用类型，创建一个class类型实例被分配在托管堆上。
* struct实例的赋值，赋的是值；class实例的赋值，赋的是引用地址。
* struct有默认无参构造函数；class没有默认无参构造函数。
* struct不支持继承；class支持继承。
* struct类型的实例既可以通过new SomeStruct()来创建，也可以通过SomeStruct myStruct创建；class的实例只能通过new SomeClass()来创建。

总结来说，类偏向于"面向对象"，用于复杂、大型数据，struct偏向于"简单值"，结构简单。

<br/>

### 类型转换

**Covert**

[Convert](https://docs.microsoft.com/zh-cn/dotnet/api/system.convert?view=netframework-4.8) 类的静态方法主要用于支持与 .NET Framework 中的基本数据类型之间的转换。 支持的基类型为 Boolean、Char、SByte、Byte、Int16、Int32、Int64、UInt16、UInt32、UInt64、Single、Double、Decimal、DateTime 和 String。

```c#
int    iNumber = System.Convert.ToInt32(dNumber);
bool   bNumber = System.Convert.ToBoolean(dNumber);
byte   btNumber = System.Convert.ToByte(dNumber);
char chrNumber = System.Convert.ToChar(strNumber[0]);
```

**BitConverter**

[BitConverter](https://docs.microsoft.com/zh-cn/dotnet/api/system.bitconverter?view=netframework-4.8) 类用于整个数组类型的转换。BitConverter 类包括用于在每个基元类型与字节数组之间进行转换的静态方法。

```C#
byte[] btAryTranData = { 0xFA, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00, 0xE4, 0x0C, 0xFB };
Console.WriteLine(BitConverter.ToString(btAryTranData, 0));
```

**Marshal**

 [Marshal](https://docs.microsoft.com/zh-cn/dotnet/api/system.runtime.interopservices.marshal?view=netframework-4.7.2) 类的方法用于分配非托管内存、复制非托管内存块、将托管类型转换为非托管类型，此外还提供了在与非托管代码交互时使用的其他杂项方法。


| 常用函数                       | 说明                                       |
| ------------------------------ | ------------------------------------------ |
| AllocHGlobal                   | 从进程的非托管内存中分配内存               |
| FreeHGlobal(IntPtr)            | 释放以前从进程的非托管内存中分配的内存     |
| DestroyStructure(IntPtr, Type) | 释放指定的非托管内存块所指向的所有子结构。 |
| Copy                           |                                            |
| PtrToStringAnsi                | 将非托管 ANSI 字符串                       |
| PToStructure                   | 将数据从非托管内存块封送到托管对象         |
| ReadByte                       | 从非托管内存中读取单个字节                 |
| ReadInt16                      | 从非托管内存中读取单个 16 位带符号整数        |
| WriteByte                      | 将单字节写入非托管内存                     |
| WriteInt16                     | 将一个字符作为16位整数值写入非托管内存        |
| SizeOf                         | 返回对象的非托管大小（以字节为单位）          |

**ToString**

```c#
float f = 865.50f;
string strTemp = f.ToString("0.000");
```

<br/>

### 新建线程

```C#
Thread threadA = new Thread(loopSwitch);
threadA.Start();
static void loopSwitch(){}
```

<br/>

### 打开FileDialog选择文件

```C#
OpenFileDialog openFileDialog = new OpenFileDialog();
openFileDialog.Filter = "*.xlsx|*.xlsx";
if( openFileDialog.ShowDialog() == DialogResult.OK)
    {
        textBox_exportFile.Text = openFileDialog.FileName;
    }
```

<br/>

### 异步加载时的委托函数

```C#
// Windows.From
private delegate void RefreshViewDelegate(long currentSize, long allSize);
private void RefreshView(long currentSize, long allSize)
{
    if (this.InvokeRequired)
    {
        RefreshViewDelegate d = new RefreshViewDelegate(RefreshView);
        this.Invoke(d , new object[] { currentSize, allSize });
        return;
    }
    // Code
}
```

```c#
// WPF
this.label.Dispatcher.Invoke(new Action(() =>
    { label.Text = 'xx'; }));
```

<br/>

### DataGridView

#### 样式

每列Title直接在属性的Columns集合中添加就好了，每列的属性可以在弹出的编辑框中设置。

#### 添加数据

```C#
int index = dataGridView1.Rows.Add();
dataGridView1.Rows[index].Cells["Column1"].Value = text1;
// 下拉单元格，ColumnType选择DataGridViewComboBoxColumn
(dataGridView1.Rows[index].Cells["Column2"] as DataGridViewComboBoxCell).Value = text2;
```

#### 添加右键菜单

在设计界面中添加ContexMenuStrip，在其中的Items集合添加选项。

```c#
private void dataGridView1_CellMouseDown(object sender, DataGridViewCellMouseEventArgs e)
{
    if (e.Button == MouseButtons.Right)
    {
        if (e.RowIndex >= 0)
        {
            // 若行已是选中状态就不再进行设置
            if (dataGridView1.Rows[e.RowIndex].Selected == false)
            {
                dataGridView1.ClearSelection();
                dataGridView1.Rows[e.RowIndex].Selected = true;
            }

            // 弹出操作菜单
            selectedRow = e.RowIndex;
            contextMenuStrip1.Show(MousePosition.X, MousePosition.Y);
        }
    }
}
```

#### 右键删除行

向Items中添加选项，更改Text为”删除“，绑定Click时间函数为”deleteMenuItem_Click“。

```c#
private void deleteMenuItem_Click(object sender, EventArgs e)
{
    DialogResult ret = MessageBox.Show("确定删除数据?", "提示", MessageBoxButtons.OKCancel);
    if (ret == DialogResult.OK)
    {
        dataGridView1.Rows.Remove( dataGridView1.Rows[selectedRow] );
    }
}
```

<br/>

### 操作Excel表格

NPOI是一个开源的C#读写Excel、WORD等微软OLE2组件文档的项目，NPOI可以在没有安装Office的情况下对Word或Excel文档进行读写操作。

<br/>

#### 环境搭建

在 Visual Studio 中，项目/解决方案右键 > 管理 NuGet 程序包 > 在“浏览”中搜索“NPOI”，选择安装。

<br/>

#### 导入

```C#
IWorkbook book = new XSSFWorkbook("FilePath");
int sheetCount = book.NumberOfSheets;
if (sheetCount == 0)    return;

ISheet sheet = book.GetSheetAt(0);
if (sheet == null)    return;

for (int i = 0; i <= sheet.LastRowNum; i++)
{
    IRow row = sheet.GetRow(i);
    if (row == null) continue;

    int firstCellNum = row.FirstCellNum;
    int lastCellNum = row.LastCellNum;
    if (firstCellNum >= lastCellNum)    continue;

    for (int j = 0; j < lastCellNum; j++)
    {
        Console.WriteLine(row.GetCell(j).NumericCellValue);
        // Console.WriteLine(row.GetCell(j).StringCellValue);
        // Console.WriteLine(row.GetCell(j).DateCellValue);
    }
}
```

<br/>

#### 导出

```C#
IWorkbook workbook = new XSSFWorkbook();
ISheet sheet = workbook.CreateSheet("Sheet1");

//表头
IRow headrow = sheet.CreateRow(0);
ICell headcel0 = headrow.CreateCell(0);
headcel0.SetCellValue("序号");
sheet.SetColumnWidth(0, 1800);
for (int i = 1; i < dataGridView1.Columns.Count+1; i++)
{
    sheet.SetColumnWidth(i, 3200);
    ICell headcell = headrow.CreateCell(i);
    headcell.SetCellValue(dataGridView1.Columns[i-1].HeaderText);
}

//表内数据
for (int i = 0; i < dataGridView1.Rows.Count - 1; i++)
{
    IRow row = sheet.CreateRow(i + 1);
    ICell cell0 = row.CreateCell(0);
    cell0.SetCellValue((i+1).ToString());
    for (int j = 1; j < dataGridView1.Columns.Count+1; j++)
    {
        ICell cell = row.CreateCell(j);
        cell.SetCellValue(dataGridView1.Rows[i].Cells[j-1].Value.ToString());
    }
}

FileStream sw = File.Create("Out.xlsx");
workbook.Write(sw);
sw.Close();
Console.WriteLine("导出至 \"Out.xlsx\"");
```

<br/>

### 串口操作

```c#
	public partial class Form1 : Form
    {
        private static SerialPort iSerialPort;

        public Form1()
        {
            InitializeComponent();
            InitializeUI();
        }

        private void InitializeUI()
        {
            iSerialPort = new SerialPort();
            iSerialPort.DataReceived += new SerialDataReceivedEventHandler(ReceivedData);
        }

        public bool OpenSerial(string strPort, int nBaudrate, out string strException)
        {
            strException = string.Empty;

            if (iSerialPort.IsOpen)
            {
                iSerialPort.Close();
            }

            try
            {
                iSerialPort.PortName = strPort;
                iSerialPort.BaudRate = nBaudrate;
                iSerialPort.WriteTimeout = 200;
                iSerialPort.ReadTimeout = 200;
                iSerialPort.Open();
            }
            catch (System.Exception ex)
            {
                strException = ex.Message;
                return false;
            }
            return true;
        }

        public bool CloseSerial()
        {
            if (iSerialPort.IsOpen)
            {
                iSerialPort.Close();
            }

            return true;
        }

        public static int SendData(byte[] btArray)
        {
            if (!iSerialPort.IsOpen)
            {
                return -1;
            }

            try
            {
                iSerialPort.DiscardOutBuffer();
                iSerialPort.Write(btArray, 0, btArray.Length);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return -1;
            }

            return btArray.Length;
        }

        public byte[] ReceiveData()
        {
            iSerialPort.DiscardInBuffer();
            int nCount = 0, nRead = 0;
            int ReceiveTimeOut = 1000;  //ms

            byte[] btAryBuffer = new byte[1024];
            Stopwatch stopWatch = new Stopwatch();
            stopWatch.Start();
            try
            {
                while (stopWatch.ElapsedMilliseconds < ReceiveTimeOut)
                {
                    nRead = iSerialPort.BytesToRead;
                    if (nRead <= 0)
                    {
                        Thread.Sleep(5);
                        continue ;
                    }

                    iSerialPort.Read(btAryBuffer, nCount, nRead);
                    nCount += nRead;

                    // 自定义一帧得校验方法：CheckSum
                    if (btAryBuffer[nCount - 1] == CheckSum(btAryBuffer, 0, nCount - 1))
                    {
                        Console.WriteLine("ReceivComData " + nCount.ToString() + " " + BitConverter.ToString(btAryBuffer));
                        byte[] btAry = new byte[nCount];
                        Array.Copy(btAryBuffer, btAry, nCount);
                        stopWatch.Stop();
                        return btAry;
                    }
                }

            }
            catch (System.Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }

            stopWatch.Stop();
            return null;
        }

        private void ReceivedData(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                int nCount = iSerialPort.BytesToRead;
                if (nCount == 0)
                {
                    return;
                }

                byte[] btAryBuffer = new byte[nCount];
                iSerialPort.Read(btAryBuffer, 0, nCount);

                ReceivedDataAnalysis(btAryBuffer);	// 自定义，处理上报的数据流
            }
            catch (System.Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }

        private bool reading = false;                   // 用于开始和停止读取
        private void ReadLoopData()
        {
        	byte[] m_btReceivedDataBuffer = new byte[4096];
            nCount = 0;

            while (reading)
            {
                try
                {
                    // 使用ReadByte要比Read快
                    ReceivedBuffer[nCount] = (byte)serial_port.ReadByte();
                    nCount++;
                    // 处理上报的数据流，处理完重置数据和nCount
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }
        }
    }
```

<br/>

## WPF

| 操作         | 代码                     |
| ------------ | ------------------------ |
| 指定启动界面 | App.xaml中修改StartupUri |

<br/>

### 布局-基础属性

| 属性                | 说明                 | 参数                                                |
| ------------------- | -------------------- | --------------------------------------------------- |
| Width               | 控件宽               | 数字，默认单位px                                    |
| Height              | 控件高               | 数字，默认单位px                                    |
| Loaded              | 加载（仅用于Window） | 绑定函数                                            |
| Closed              | 关闭（仅用于Window） | 绑定函数                                            |
| x:Name              | 命名                 | String                                              |
| Padding             | 填充                 | "10"/"10 20"（左右 上下）/"10 20 30 40"（左上右下） |
| Margin              | 边白                 | 同上                                                |
| VerticalAlignment   | 垂直对齐             | Top/Bottom/Center/Stretch(默认：填充)               |
| HorizontalAlignment | 水平对齐             | Left/Center/Right/Stretch(默认：填充)               |
| Click               | 单击事件响应         | 绑定函数                                            |
| FontSize            | 字体大小             | 数值                                                |
| FontFamily          | 字体样式             |                                                     |
| Opacity             | 透明度               | 0-1.0                                               |

<br/>

### 布局-Grid

```xaml
<!-- *.xaml -->
<Grid>
    <Grid.ColumnDefinitions>
        <ColumnDefinition Width="*"/>
        <ColumnDefinition Width="*"/>
    </Grid.ColumnDefinitions>
    <Grid.RowDefinitions>
        <RowDefinition Height="60"></RowDefinition>
        <RowDefinition Height=""></RowDefinition>
    </Grid.RowDefinitions>

    <Label Grid.Row="0" Grid.Column="0">Select：</Label>
    <ComboBox Grid.Row="0" Grid.Column="1"></ComboBox>
    <TextBox Grid.Row="1" Grid.Column="0" Grid.ColumnSpan="2"></TextBox>
</Grid>
```

以上，使用Grid建立了一个2行2列的布局，第1行放置了两个控件，第2行放置了一个跨2列的控件。

<br/>

### 布局-数据管数字显示

使用DSEG7 Classic Mini字体，来显示数码管效果的数字。

在项目中新建/Resources/Font/目录，将[DSEG7字体文件](/images/CSharp代码小结/DSEG7 Classic Mini.ttf)添加到路径下，以4位数字为例：

```xaml
<TextBlock Text="{Binding Hour, FallbackValue=0000, StringFormat=00}"
    FontFamily="./Resources/Font/#DSEG7 Classic Mini" />
```

<br/>

### Button样式模板

以Button为例，如果多个控件的效果相同，则可以声明一个Style，对多个Button应用相同的样式。

```xaml
<!-- *.xaml -->
<!-- 在Window下声明 -->
<Window.Resources>
    <Style x:Key="bt_test" TargetType="Button">
        <Setter Property="OverridesDefaultStyle" Value="True" />
        <Setter Property="Cursor" Value="Hand" />
        <Setter Property="FontSize" Value="24"/>
        <Setter Property="Margin" Value="3,3" />
        <Setter Property="Template">
            <Setter.Value>
                <ControlTemplate TargetType="Button">
                    <Border Name="border" BorderThickness="0" BorderBrush="LimeGreen" >
                        <ContentPresenter HorizontalAlignment="Center" VerticalAlignment="Center" />
                    </Border>
                    <ControlTemplate.Triggers>
                        <Trigger Property="IsMouseOver" Value="True">
                            <Setter Property="Opacity" Value="0.9" />
                        </Trigger>
                    </ControlTemplate.Triggers>
                </ControlTemplate>
            </Setter.Value>
        </Setter>
    </Style>
</Window.Resources>

<!-- 使用 -->
<Button Style="{StaticResource bt_test}"/>
```

<br/>

### 显示表格

样式：

```xaml
<!-- *.xaml -->
<ListView x:Name="person_list" FontSize="15" Width="300">
    <ListView.View>
        <GridView>
            <GridViewColumn Header="ID" Width="90"  DisplayMemberBinding="{Binding ID, UpdateSourceTrigger=Explicit}" />
            <GridViewColumn Header="姓名" Width="30" DisplayMemberBinding="{Binding NAME, UpdateSourceTrigger=Explicit}"/>
            <GridViewColumn Header="性别" Width="30" DisplayMemberBinding="{Binding SEX, UpdateSourceTrigger=Explicit}"/>
            <GridViewColumn Header="电话" Width="75" DisplayMemberBinding="{Binding PHONE, UpdateSourceTrigger=Explicit}"/>
            <GridViewColumn Header="Email" Width="75" DisplayMemberBinding="{Binding EMAIL, UpdateSourceTrigger=Explicit}"/>
        </GridView>
    </ListView.View>
</ListView>
```

代码：

```C#
// 声明
class Person : INotifyPropertyChanged
{
    public event PropertyChangedEventHandler PropertyChanged;

    private string id;
    public string ID
    {
        get { return id; }
        set { id = value; OnPropertyChanged("ID"); }
    }

    private string name;
    public string NAME
    {
        get { return name; }
        set { name = value; OnPropertyChanged("NAME"); }
    }

    private string sex;
    public string SEX
    {
        get { return sex; }
        set { sex = value; OnPropertyChanged("SEX"); }
    }

    private string phone;
    public string PHONE
    {
        get { return phone; }
        set { phone = value; OnPropertyChanged("PHONE"); }
    }

    private string email;
    public string EMAIL
    {
        get { return email; }
        set { email = value; OnPropertyChanged("EMAIL"); }
    }

    private void OnPropertyChanged(string propertyName = "")
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}

// 绑定数据源
static BindingList<Person> _person_list = new BindingList<Person>();;
person_list.ItemsSource = _person_list;

// 添加数据
Person p = new Person();
p.ID = "007";
p.NAME = "邦德";
p.SEX = "男";
p.PHONE = "500238";
p.EMAIL = "500238@jqc.com";
this.person_list.Dispatcher.Invoke(new Action(() =>
{
    _person_list.Add(p);
}));
```

<br/>

### 状态栏显示操作结果

```xaml
<!-- *.xaml -->
<StatusBar DockPanel.Dock="Bottom">
    <Grid >
        <Grid.ColumnDefinitions>
            <ColumnDefinition Width="70"/>
            <ColumnDefinition Width="230"/>
            <ColumnDefinition Width="70"/>
            <ColumnDefinition Width="230"/>
            <ColumnDefinition Width="70"/>
            <ColumnDefinition Width="230"/>
        </Grid.ColumnDefinitions>
        <Label Grid.Column="0" FontSize="12">连接状态：</Label>
        <Label Grid.Column="1" x:Name="lbConStatus" FontSize="12" HorizontalAlignment="Left"></Label>
        <Label Grid.Column="2" FontSize="12">执行命令：</Label>
        <Label Grid.Column="3" x:Name="lbCmd" FontSize="12" HorizontalAlignment="Left"></Label>
        <Label Grid.Column="4" FontSize="12">执行结果：</Label>
        <Label Grid.Column="5" x:Name="lbCmdRet" FontSize="12" HorizontalAlignment="Left"></Label>
    </Grid>
</StatusBar>
```

```C#
private void statusUpdate(string cmd, string cmdRet)
{
    lbCmd.Dispatcher.Invoke(new Action(() =>
    { lbCmd.Content = cmd; }));
    lbCmdRet.Dispatcher.Invoke(new Action(() =>
    { lbCmdRet.Content = cmdRet; }));
}

private void statusUpdate(string conn, string cmd, string cmdRet)
{
    if (conn != null)
        lbConStatus.Content = conn;
    lbCmd.Content = cmd;
    lbCmdRet.Content = cmdRet;
}
```

<br/>

### 窗口跳转

```C#
private void jump_Click(object sender, RoutedEventArgs e)
{
    NewWindow newWindow = new NewWindow();  // NewWindow继承Window类
    newWindow.WindowStartupLocation = WindowStartupLocation.CenterScreen;
    newWindow.Show();  // 显示新窗口
    this.Close();      // 关闭当前窗口
}
```

<br/>

### 参考链接

* [C#语法中文教程](https://www.runoob.com/csharp/csharp-tutorial.html)
* [Microsoft文档搜索](https://docs.microsoft.com/zh-cn/search/)
* [Alignment、Margin 和 Padding 概述](https://docs.microsoft.com/zh-cn/dotnet/framework/wpf/advanced/alignment-margins-and-padding-overview)
* [C#：ListView控件如何实现点击列表头进行排序？](https://www.cnblogs.com/hongfei/p/3579373.html)