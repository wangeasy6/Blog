import sys
import bluetooth

print("Scaning ...")
nearby_devices = bluetooth.discover_devices(lookup_names=True)
# print(nearby_devices)

num = len(nearby_devices)
print("Found {} devices:".format(num))

i = 0
for addr, name in nearby_devices:
    print("  {}  > {} - {}".format(i, addr, name))
    i += 1
print("  -1 > Rescan.")

print("Select device connect:")

bt_addr = input()

# 也可以直接输入蓝牙设备地址，如果输入序号则需要从列表中取
if len(bt_addr) < 4:
    index = int(bt_addr)
    if index < 0 or index >= num:
        print("index out of range.")
        sys.exit(0)
    bt_addr = nearby_devices[index][0]

print("Connect to {}".format(bt_addr))

sock = bluetooth.BluetoothSocket()
print(sock.fileno())
port = 1
sock.connect((bt_addr, port))

sock.send("get_wifi_list")

sock.close()

# for bdaddr in nearby_devices:
#     if target_name == bluetooth.lookup_name( bdaddr ):
#         target_address = bdaddr
#         break

# if target_address is not None:
#     print("found target bluetooth device with address ", target_address)
# else:
#     print("could not find target bluetooth device nearby")