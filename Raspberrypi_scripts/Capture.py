################################
#USB camera raspberry pi client#
#Author: Artur Cruz            #
#Date:18/11/2022               #
################################

##To do:
#

##Import library
import os

##Intruduction text
icon="          ___            \n"+"         / _ \           \n"+"        | / \ |          \n"+"        | \_/ |          \n"+"         \___/ ___       \n"+"         _|_|_/[_]\__==_ \n"+"        [---------------]\n"+"        | O   /---\     |\n"+"        |    |     |    |\n"+"        |     \___/     |\n"+"        [---------------]\n"+"              [___]      \n"
print(icon)
print("WELCOME TO THE IMAGE CAPTURE 9000!!!\n")
print("If no name is given img is adopted")
print("Type name then ENTER per image")

##Get all the files on the curret directory
curent=os.listdir()

##Match the name given (or not) by the user and set the number value
n=[]
name=input("Setname: ")
##If no name is provided it is set to 'img'
if name=='':
    name='img'
for x in curent:
    if x[:len(name)]==name:
        ##test images with the same starting name but longer names (img != img_ != img-)
        try:
            n.append(int(x[len(name):-4]))
        except:
            pass
##If not name match start counting at 1
if n==[]:
    number='1'
else:
    number=str(max(n)+1)

##Enter Photo loop while input empty (ENTER)
raw=''
while raw=='':
    ##Get all the files on the curret directory
    curent=os.listdir()
    ##File name configured
    print('file name: '+name+number+'.jpg')
    ##No Overwrite check
    if (name+number+'.jpg') in curent:
        print ("Cannnot overwrite image")
    else:
        ##Command to execute image capture
        os.system('fswebcam -r 1280x720 --no-banner '+name+number+'.jpg')
        ##Increment number at the end of the next img 
        number=str(int(number)+1)
    raw=input("Enter for next Picture or an alphanumeric and then Enter to close")
