---
title: TTS (Tabletop Simulator) Lua脚本片段
date: 2026-02-10 12:27:00
toc: true
categories:
  - tech
tags:
  - iplay
---

桌游模拟器（Tabletop Simulator，简称 TTS）作为桌游的通用模拟器，不仅提供了基础的物理模拟，还提供了一个 Lua 脚本运行环境，其在基础的桌游基础上，提供了一些列的操作接口，使得可以完成线下无法做到的自动Setup、拿取/分发物品、终局结算等等电子游戏的功能，简化了玩家操作，让大家更加专注于策略层面。

📢 传统的桌游就是把一堆卡牌和Token拿来移去，好耗时好麻烦，而桌游模拟器就是在传统的桌游上加入脚本好好玩！！！

除了官方的API文档，和桌游模拟器脚本相关的中文资料十分稀少，所以在这里做一些总结，以供大家入门和参考。有任何一门编程语言的基础学起来应该都不难，对于会 Python 或者 JavaScript 等脚本语言的来说就更简单了。

适验版本：TTS v14.1.8，Lua 版本 v5.2。

<!--more-->

<br/>

## 简约开发流程

1. **创建本地游戏**：对于想要更改的游戏点击 `游戏` - `保存和载入` - `保存游戏`，输入名称。这一步是对于开发中的游戏建立一个本地存档，否则在更改脚本时会报错。

2. **修改游戏**：
   * 可以通过点击顶部栏的 `MOD` - `脚本` 来访问游戏内的Lua编辑器，或右键点击对象，选择脚本，然后从上下文菜单中选择 Lua编辑器。
   * 在Lua编辑器中，编辑器左侧的标签页允许你在全局脚本和对象脚本之间切换。
   * 将右侧的代码全部复制并保存为本地文件（*.lua），使用任意一款代码编辑工具编写代码。
   * 代码写好后，复制全部代码粘贴回TTS的代码编辑器，点击 `保存并播放` 按钮提交修改到存档文件，游戏会自动重新加载。 `保存并播放` 只会提交你的脚本更改，任何非脚本的更改都会丢失。
   * 非脚本的更改需要点击 `游戏` - `保存和载入`，对当前保存的游戏点击 `Option` - `覆盖` 进行保存。


3. **上传新游戏**：在修改完成的游戏中，点击 `MOD` - `上传创意工坊`。如果没有上传过就填写 `上传创意工坊` 表单，否则填写 `更新创意工坊` 表单。

<br/>

## Atom 环境搭建

使用简约开发流程时，需要不断地把代码复制进复制出，十分地麻烦，而搭建一个 Atom 的开发环境，不仅可以自行获取脚本，还有行号、语法高亮、自动补全和现代化的外观等诸多好处。

根据以下指引安装 Atom（试验版本：v1.60.0）和 插件：[Release 13.2.0 · Berserk-Games/atom-tabletopsimulator-lua · GitHub](https://github.com/Berserk-Games/atom-tabletopsimulator-lua/releases/tag/13.2.0)

安装后，在 Atom 中点击 `Packages` 如图所示：

![](/resources/tts-script/atom-lua-package-menu.png)

创建本地游戏和上传新游戏的流程不变，但简化了修改流程。

### 修改流程

1. 在 TTS 中，进入一个本地游戏。

2. 在 Atom 中，按快捷键 `Ctrl+Shift+L`，或者点击 `Packages` - `Tabletop Simlator` - `Get Lua Scrits`。

   会直接打开一个名为 `Tabletop Simulator Lua` 的工程，其中包含了当前游戏的所有脚本（.ttslua）和UI（.xml)。

3. 双击文件即可对脚本进行修改。

4. 修改完成后，按快捷键 `Ctrl+Shift+S`，即会触发游戏 `保存并播放`。

### 实现原理

编辑器与 TTS 之间的通信通过两个本地主机TCP连接进行：一个由TTS监听消息，另一个由编辑器监听消息。所有通信消息均为 JSON，通过 messageID 字段区分不同功能的消息。

Atom 监听端口 39998 的本地主机TCP连接。在 TTS 中 新建对象、新建对象脚本、加载新游戏、print 输出、Lua错误、调用sendExternalMessage、游戏保存 时，会发送消息给此端口。

TTS 监听端口 39999 的本地主机TCP连接。在 Atom 中执行 获取Lua脚本、保存并播放、发送自定义消息、执行Lua代码 时，会发送消息给此端口。

具体 JSON 定义参见：[External Editor API - Tabletop Simulator API](https://api.tabletopsimulator.com/externaleditorapi/)

<br/>

## Lua 基础

和其他语言相比，Lua 较为不同的点：

* **仅在函数内使用的变量，一定要加 local 声明**：函数中所有没有 local 修饰的变量都默认是全局变量，对于一些通用的变量名来说，会带来诸多非预期的改变。
* 在 Lua 索引值是以 **1** 为起始，但你也可以指定 0 开始。
* 要计算数组的长度（即获得数组元素的个数），在变量前加 `#` 号。

<br/>

## TTS Lua 基础

### Object

游戏场景中的所有物体都是一个对象（Object），如果一个物体被放在 袋子/牌堆 里，它将不存在于场景中。而一个牌堆被拿得只剩下一张牌，牌堆也会不存在。

### Player

每位玩家被分配一种颜色，观众为“灰色”，主持人为“黑色”。如果你针对某个玩家，需要根据他们所在座位的颜色来识别。

### Global Script

每个游戏/存档都有一个全局脚本，不会绑定在任何特定对象上。

全局脚本只有一个，而且它始终存在。

### Object Scripts

每个游戏内的对象也可能附带脚本。

默认情况下，新创建的对象不会附加脚本。不过，你可以创建/编辑某个对象的脚本：右键点击对象，选择` Lua编辑器`。

### GUID

在桌面模拟器中，GUID 是一个独特的 6 字符字符串，可用于识别游戏中的对象。对于场景中存在的对象，会自动分配 GUID。

如果一个对象被复制，有时它会在一帧内保持相同的 GUID，然后引擎才会为新对象分配新的 GUID。容器（袋子/牌堆等）中的物体不会自动分配新的GUID，只有当它们的内容物被移到场景中时才会有。

<br/>

## 代码片段

### 初始代码

```lua
--[[ Lua code. See documentation: https://api.tabletopsimulator.com/ --]]

--[[ The onLoad event is called after the game save finishes loading. --]]
function onLoad()
    --[[ print('onLoad!') --]]
end

--[[ The onUpdate event is called once per frame. --]]
function onUpdate()
    --[[ print('onUpdate loop!') --]]
end
```

默认的全局脚本，定义了两个事件函数：

* `onLoad()`：游戏加载完成后调用 onLoad。
* `onUpdate()`：每帧调用一次 onUpdate。

<br/>

### 消息输出

#### 在屏幕中央或游戏消息框显示

| 函数名                                                       | 描述                                                   | Return |
| :----------------------------------------------------------- | :----------------------------------------------------- | :----- |
| `broadcastToAll(string message, color message_tint)`         | 向所有玩家打印屏幕信息，以及他们的游戏内聊天。         | bool   |
| `broadcastToColor(string message, string player_color, color message_tint)` | 向指定的玩家打印屏幕上的信息，以及他们的游戏内聊天。   | bool   |
| `print(string message)`                                      | 它会在聊天中打印字符串，只有主机能看到。用于调试脚本。 | nil    |
| `printToAll(string message, color message_tint)`             | 把消息打印到所有连接玩家的聊天里。                     | bool   |
| `printToColor(string message, string player_color, color message_tint)` | 向特定玩家颜色打印消息。                               | bool   |

```lua
function msgTest()
  -- 向所有玩家的屏幕及其消息框输出消息，不指定消息颜色时默认为白色
  broadcastToAll("broadcastToAll")
    -- 向所有玩家的屏幕及其消息框输出消息，并指定消息颜色为红色
  broadcastToAll("broadcastToAll Red", {0.856, 0.1, 0.094})
    -- 向白色玩家的屏幕及其消息框输出消息，并指定消息颜色为白色
  broadcastToColor("broadcastToAll White Blue", "White", {0.118, 0.53, 1})

  -- 只会显示在房主的消息框，一般用于调试
  print("print")
    -- print 和 broadcast 的用法基本一直，只是 print 的消息只显示在消息框，而不会显示在屏幕中央
  printToAll("printToAll Yellow", "Yellow")
  printToColor("printToAll White Green", "White", "Green")
end
```

以上示例中使用的 color 数据均来自于 TTS 自定义的玩家颜色：[Colors - Tabletop Simulator API](https://api.tabletopsimulator.com/player/colors/)。对于 TTS 定义的 12 个颜色来说，可以直接使用它们的字符串作为 color 类型参数，TTS 会自动转换为对应的 color 值。

![msg-test.png](/resources/tts-script/msg-test.png)

<br/>

#### 在系统控制台显示

除了提供主机专用的消息显示通道，log 还提供了自动解析 table 为字符串显示的便利。

| 函数名                                                       | 描述                                       | Return |
| :----------------------------------------------------------- | :----------------------------------------- | :----- |
| `log(var value, string label, string tags)`                  | 向主机的系统控制台发送消息。（快捷键：~）  | bool   |
| `logString(var value, string label, string tags, bool concise, bool displayTag)` | 返回一个格式类似于log（...）输出的字符串。 | string |
| `logStyle(string tag, color tint, string prefix, string postfix)` | 为日志的指定标签类型设置样式选项。         | bool   |

```lua
function logTest()
  -- 以 error 的风格打印 string 类型的消息到系统控制台
  log("Something unexpected happened.", "Oh no!", "error")
  -- 以 info 的风格打印 table 类型的消息到系统控制台
  log(getSeatedPlayers(), "getSeatedPlayers", "info")
  -- 以 log 的形式解析成 string 后输出
  broadcastToAll(logString(getSeatedPlayers()))
  -- 自定义 seats 风格
  logStyle("seats", {0.5, 0.5, 0.5}, "Start List", "End List")
  -- 使用 seats 风格打印
  log(Player.getAvailableColors(), nil, "seats")
end
```

![log-test.png](/resources/tts-script/log-test.png)

其解析深度可以通过 `log_max_table_depth` 配置。

![log_max_table_depth.png](/resources/tts-script/log_max_table_depth.png)

<br/>

### 获取玩家人数

```lua
local seated_players_number = #getSeatedPlayers()
```

* **getSeatedPlayers()**：是一个[全局函数](https://api.tabletopsimulator.com/base/#global-functions)，返回一个包含已入座玩家颜色字符串的表格（Table）。

<br/>

### 获取玩家名称

```lua
-- 通过颜色获取玩家名称
function getPlayerName(color)
  if not Player[color].seated then
    return color
  end

  local player_info = getPlayer(color)
  if player_info then
    return player_info.steam_name
  else
    return color
  end
end
```

<br/>

### 创建按钮

```lua
params1 = {
  click_function = "buttonClicked",
  function_owner = self,     -- 函数所属对象（当前物件）
  label      = "点击按钮",    -- 按钮上显示的文字
  position   = {0, 0.3, 0},  -- 按钮相对于物件的位置 (X, Y, Z)
  rotation   = {0, 90, 0},   -- 旋转参数
  scale      = {1.5, 1, 1.5},-- 缩放参数
  width      = 1650,       -- 按钮宽度
  height     = 750,        -- 按钮高度
  font_size  = 340,        -- 字体大小
  color      = {0.5, 0.5, 0.5, 1},  -- 按钮背景颜色 (R, G, B, A)
  font_color = {1, 1, 1}   -- 字体颜色 (白色)
}

-- 新建 组件 - 卡牌 - 小丑
card_guid = 'b39034'

function onLoad()
    -- 在当前物件（承载此脚本的物件）的控制栏上创建按钮
  local card_bt = getObjectFromGUID(card_guid)
  card_bt.createButton(params1)
end

-- 定义按钮点击时触发的函数
function buttonClicked(obj, player_clicker_color, alt_click)
    print("【系统提示】按钮已被点击！")
    -- 你也可以使用 broadcastToAll 让所有玩家都看到更醒目的提示
    -- broadcastToAll("【系统提示】一个自定义按钮被按下了！")
end
```

按钮需要依附在物体的表面，所以先创建一张小丑牌（使用小丑牌，可以不用增加资产，厚度也比较合适），然后调整按钮的长宽等参数，覆盖下方的卡牌。

`createButton(parameters)`

`return bool`，创建一个脚本按钮连接到对象上。脚本按钮是指可以在游戏中点击，触发脚本中的某个函数。

**parameters**: 包含生成按钮所用信息的 Table。

-  **parameters.click_function**: 点击按钮时执行的函数名称字符串。
-  **parameters.function_owner**: 包含click_function函数的对象。可选，默认为全局。
-  **parameters.label**: 显示在按钮上的文本。可选，默认为空字符串。
-  **parameters.position**: 按钮相对于物体中心出现的位置。可选，默认为 {x=0， y=0， z=0}
-  **parameters.rotation**: 按钮相对于物体旋转的方向。可选，默认为 {x=0， y=0， z=0}
-  **parameters.scale**: 按钮相对于物体缩放的比例。可选，默认为 {x=1， y=1， z=1}
-  **parameters.width**: 按钮相对于物体的宽度。可选，默认为100。
-  **parameters.height**: 按钮相对于物体的高度。可选，默认为100。
-  **parameters.font_size**: 标签字体相对于对象的大小。可选，默认为100。
-  **parameters.color**: 用于可点击按钮的颜色。可选，默认为 {r=1， g=1， b=1}
-  **parameters.font_color**: 标签文字的颜色。可选，默认为 {r=0， g=0， b=0}
-  **parameters.hover_color**: 鼠标悬停时背景的颜色。可选。
-  **parameters.press_color**: 点击后背景颜色。可选。
-  **parameters.tooltip**: 文本弹窗，类似于鼠标悬停时显示对象名称的方式。可选，默认为空字符串。

`click_function(obj, player_clicker_color, alt_click)`

点击按钮会自动传递以下参数：

-  **obj**: 按钮附着的物体。
-  **player_clicker_color**: 按下按钮的[玩家颜色](https://api.tabletopsimulator.com/player/colors/)。判断玩家颜色可以针对特定玩家
-  **alt_click**: 如果用了非左键点击按钮，则为 true；左键点击按钮，则为 false。根据左右按键点击可以定义不同的行为。

<br/>

### 对象操作

```lua
role_card_guid = '752421'
role_card = getObjectFromGUID(role_card_guid)
print(role_card.getPosition())
role_card.setPosition({x,1.01,z})
role_card.setRotation({0,90,0})
```

<br/>

### 获取对象信息

```lua
card_guid = 'ecc9a9'
card = getObjectFromGUID(card_guid)
print(card.name, " | ", card.getName(), " | ",  card.getDescription())
-- Card | Field | Corn
```

* **name** 相当于对象类型，有 Card、Bag、PlayerPawn、Deck、Notecard、Custom_Board、Custom_Tile、Custom_Token、Custom_Model 等等。
* **getName()** 是可编辑的名称。
* **getDescription()** 是可编辑的描述。

<br/>

### 牌堆操作

```lua
-- 牌堆的 GUID
land_plate_guid = '81736e'
land_plate_pile = getObjectFromGUID(land_plate_guid)
land_plate_pile.getQuantity()
land_plate_pile.shuffle()  -- 洗牌

-- 从牌堆/袋子中拿出物品到指定位置
local params = {}
params.position = {-11.26, 1.1, 0}
local taken_object = land_plate_pile.takeObject(params)
-- 放入物品到牌堆
land_plate_pile.putObject(obj, 1)
```

<br/>

### 获取某一区域所有的物品

```lua
-- 一个简单的计算距离的函数
function distance(pos1, pos2)
    local dx = pos1[1] - pos2[1]
    -- local dy = pos1.y - pos2.y  -- 忽略上下的位置
    local dz = pos1[3] - pos2[3]
    return math.sqrt(dx*dx + dz*dz)
end

function findAllWaistPlate(wx, wz, radius)
  local allObjects = getAllObjects()
  local foundObjects = {}
  local d = 0

  for _, obj in ipairs(allObjects) do
    -- 检查物品类型
    -- local typeMatch = (objectType == nil or objectType == "" or obj.getType() == objectType)
    
    -- 计算物品与目标点的距离
    d = distance({wx,0,wz}, obj.getPosition())
    
    -- if typeMatch and d <= radius then
    if d <= radius then
      table.insert(foundObjects, obj)
      -- print('find d: ', d, '  ', obj.getGUID())
    end
  end

  return foundObjects
end

function onload()
  -- 废牌堆坐标
  local wx = -9.12
  local wz = 0.72
  local foundObjects = findAllWaistPlate(wx, wz, 1)
end
```

<br/>

### 从一堆物品中过滤某一类物品

```lua
-- 从一堆物品中过滤某一名称的物品
function filterObjectByName(allObjects, name)
  local foundObjects = {}

  for _, obj in ipairs(allObjects) do
    if obj.name == name then
      table.insert(foundObjects, obj)
    end
  end

  return foundObjects
end

-- 从一堆物品中过滤某一名称的物品
function filterObjectByGetName(allObjects, name)
  local foundObjects = {}

  for _, obj in ipairs(allObjects) do
    if obj.getName() == name then
      table.insert(foundObjects, obj)
    end
  end

  return foundObjects
end

-- 从一堆物品中过滤带有某一描述的物品
function filterObjectByDes(allObjects, name)
  local foundObjects = {}

  for _, obj in ipairs(allObjects) do
    if string.match(obj.getDescription(), name) then
      table.insert(foundObjects, obj)
    end
  end

  return foundObjects
end
```

<br/>

### 跨对象函数调用

#### 调用某个对象的函数

```lua
sub_object_guid = '9f9597'
local sub_object = getObjectFromGUID(sub_object_guid)
sub_object.call("subFuction", params)
```

在 Global 函数中，调用对象（9f9597）脚本中的函数。

* **params**：跨对象调用时，只允许传递一个参数，如果想要传递多个参数，需要先封装为一个 Table 类型的变量。

#### 对象脚本中调用全局函数

```lua
local workers_number = Global.call("getNumber", params)
```

在对象脚本中调用 Global 脚本中的 getNumber 函数。

<br/>

### 控制其它玩家转到房主当前视角

同步房主的视角给其它玩家（自动同步鼠标位置、旋转角度，暂时只支持固定的俯仰角、距离）：

```lua
function onLoad()
    addHotkey("Fllow Me", fllowMe, false)
end

function fllowMe(playerColor, hoveredObject, pointerPosition, isKeyUp)
  -- Only admin can do
  if Player[playerColor].admin then
    local parameters = {}
    parameters.position = pointerPosition
    parameters.pitch = 90       -- Fixed pitch
    parameters.distance = 20    -- Fixed distance
    -- Get rotation
    local y_pos = Player[playerColor].getPointerRotation()
    if y_pos < 180 then
      parameters.yaw = 180 + y_pos
    else
      parameters.yaw = y_pos - 180
    end

    for _,player in ipairs(getSeatedPlayers()) do
      if player != playerColor then
        Player[player].lookAt(parameters)
        broadcastToColor("同步了房主视角", player, playerColor)
      end
    end
    broadcastToColor("向其他玩家同步了视角", playerColor, playerColor)
  end
end
```

重新加载脚本后，点击`设置` - `游戏按键`，添加对应的快捷键：

![setting-keys.png](/resources/tts-script/setting-keys.png)

![bind-hotkey.png](/resources/tts-script/bind-hotkey.png)

设置完成后，按按键 K 即可同步当前视角给其它玩家。

<br/>

## 参考

* 官方 API：[Introduction - Tabletop Simulator API](https://api.tabletopsimulator.com/)
* TTS 基础知识：[Tabletop Simulator Knowledge Base](https://kb.tabletopsimulator.com/)
* Lua 官网：[Lua: getting started](https://www.lua.org/start.html)
* [Lua 基本语法 | 菜鸟教程](https://www.runoob.com/lua/lua-basic-syntax.html)

