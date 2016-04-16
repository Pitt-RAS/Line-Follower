import tkinter
import serial
import time
from sys import platform as _platform
if _platform == "linux" or _platform == "linux2":
    connection = "/dev/ttyUSB%s"
elif platform.system() == "Windows":
    connection = "COM%s"
else:
    connection =  "COM%s"
    
exist=0
root = tkinter.Tk()

# make it cover the entire screen
w, h = root.winfo_screenwidth(), root.winfo_screenheight()
root.overrideredirect(1)
root.geometry("%dx%d+0+0" % (w, h))

root.focus_set() # <-- move focus to this widget
root.bind("<Escape>", lambda e: e.widget.quit())

root.configure(background="blue")
canvas = tkinter.Canvas(root,width=500,height=250,bd=0,bg="black")
canvas.create_text(10,10,text="hello")
canvas.pack()

ports=[connection % (i+1) for i in range(256)]
for port in ports:
    try:    
        ser = serial.Serial(port,9600,timeout=0)
        exist=1
    except (OSError,serial.SerialException):
        pass
        print("No device connected")

class Output:
    def __init__(self,canvas):
        self.canvas=canvas
        self.id=canvas.create_text(10,10,text="hello")
    def draw(self,var):
        self.canvas.delete("all")
        temp=stringToTime(var,0)
        if(temp!="error in parsing"):
            var=temp
        self.canvas.create_text(250,125,text=var,font=("Helvetica","30","bold"),fill="green")
o=Output(canvas)

def stringToTime(s,checkFin):
    try:
        timeInt = int(s)
        temporary=timeInt%1000
        timeString=str(temporary)
        if(temporary<100):
            timeString="0"+timeString
            if(temporary<10):
                timeString="0"+timeString
        timeInt=int(timeInt/1000)
        timeString=":"+timeString
        count=2
        while(timeInt>0):
            temporary=timeInt%60
            if(temporary<10):
                timeString="0"+str(temporary)+timeString
            else:
                timeString=str(temporary)+timeString
            timeInt=int(timeInt/60)
            count=count-1
        while(count>0):
            timeString="00:"+timeString
            count=count-1
        if(checkFin==1):
            timeString="Finished\n"+timeString
        return timeString
    except ValueError:
        if("Finish" in s):
            return stringToTime(s.replace("Finish",""),1)
        return "error in parsing"

def clock():
    try:
        myStr=""
        while True:
            lineIn=ser.readline()
            print(lineIn)
            myStr=myStr+lineIn.decode('ascii')
            if(myStr[-1:]=="\n"):    
                root.update_idletasks()
                root.update()
                o.draw(myStr)
                break
    except ser.SerialTimeoutException:
        print("data could not be read kfdsjf;la;dkf;asdk")
    root.after(1,clock)
if exist==1:
    clock()
    root.mainloop()
    ser.close()
else:
    var=bytes("no device connected",'utf-8')
    o.draw(var)
    root.mainloop()
