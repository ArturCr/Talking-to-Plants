import os

#print(os.listdir())

icon="          ___            \n"+"         / _ \           \n"+"        | / \ |          \n"+"        | \_/ |          \n"+"         \___/ ___       \n"+"         _|_|_/[_]\__==_ \n"+"        [---------------]\n"+"        | O   /---\     |\n"+"        |    |     |    |\n"+"        |     \___/     |\n"+"        [---------------]\n"+"              [___]      \n"
print(icon)
print("WELCOME TO THE IMAGE CAPTURE 9000!!!")
print("Type name & number then ENTER per image")

name=input("Setname: ")
number=input("Number: ")
if name=='':
    name='img'
if number=='':
    number='0'

raw=input("Enter for next Picture")
while raw=='':
    raw=input("Enter for next Picture")
    curent=os.listdir()
    if (name+number+'.jpg') in curent:
        print ("Cannnot overwrite image")
    else:
        os.system('fswebcam -r 1280x720 --no-banner '+name+number+'.jpg')
        number=str(int(number)+1)
