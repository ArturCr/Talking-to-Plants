import binascii
filename = 'leaf2.png'
with open(filename, 'rb') as f:
    content = f.read()

temp=binascii.hexlify(content)
print(temp)

a=open("leaf2.bin",'wb')
a.write(temp)
a.close()

a=open("leaf2.bin",'rb')
hexstr=a.read()
a.close()

temp=binascii.unhexlify(hexstr)
b=open("leaf3.png",'wb')
b.write(temp)
b.close()
