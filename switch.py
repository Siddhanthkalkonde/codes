from codeop import CommandCompiler
from pickle import TRUE
from pyfirmata import Arduino, util, pyfirmata
from dev import speak, takeCommand

 













port = 'COM3'

board = Arduino(port)

switch_1 = board.get_pin('d:7:o')
switch_2 = board.get_pin('d:6:o')
switch_3 = board.get_pin('d:5:o')
switch_4 = board.get_pin('d:4:o')
# trigpin = 8
# echopin = 9




# def mdistance() :
 
#     board.digital[trigpin].write(1)
#     time.sleep(0.00001)

#     board.digital[trigpin].write(0)
#     time.sleep(0.00001)


    
#     while board.digital[echopin].read() == 0:
#         pass
#     pulse_start = time.time() 

#     while board.digital[echopin].read == 1 :
#         pass
#     pulse_end = time.time()

#     duration = pulse_end - pulse_start
#     distance = duration * 0.034
#     distance = round(distance , 2)

#     return distance



# def ultrasonic() :
#     board.digital[trigpin].mode = pyfirmata.OUTPUT
#     board.digital[echopin].mode = pyfirmata.INPUT

#     it = util.Iterator(board)
#     it.start()

    

#     try :
#         while True:
#             distance  = mdistance()

#             print("Distance: {} cm ".format(distance))
#             time.sleep(1)


#     except KeyboardInterrupt:
        
#         board.exit()

    





            
        



def switches () :
    while True:


        # ultrasonic() 
       

      
       



        switchCommand = takeCommand().lower()
    




        if 'turn on' in switchCommand or ' switch on' in switchCommand or 'on' in switchCommand :
            
            if 'first' in switchCommand :
                
                speak("socket 1 : onn")
                switch_1.write(1)
                


            elif 'second' in switchCommand :
                speak("socket 2 : onn")
                switch_2.write(1)
                

            elif 'third' in switchCommand:
                speak("socket 3: onn")
                switch_3.write(1)
                

            elif 'fourth' in switchCommand or '4th' in switchCommand :
                speak("socket 4 : onn")
                switch_4.write(1)
                
            elif 'all' in switchCommand:
                speak("all sockets : onn")    
                switch_1.write(1)
                switch_2.write(1)
                switch_3.write(1)
                switch_4.write(1)


            elif '1 and 2' in switchCommand or 'one and two' in switchCommand:
                speak("socket 1 and 2: onn")
                switch_1.write(1)
                switch_2.write(1)


            elif '1 and 3' in switchCommand or 'one and three' in  switchCommand :
                speak("socket 1 and 3 : onn")
                switch_1.write(1)
                switch_3.write(1)

            elif '1 and 4' in switchCommand or 'one and four' in switchCommand:
                speak("socket 1 and 4 : onn")
                switch_1.write(1)
                switch_4.write(1)

            elif '2 and 1' in switchCommand or 'two and one' in switchCommand:
                speak("socket 2 and 1: onn")
                switch_2.write(1)
                switch_1.write(1)

            elif '2 and 3' in switchCommand or 'two and three' in switchCommand:
                speak("socket 2 and 3 : onn")
                switch_2.write(1)
                switch_3.write(1)

            elif '2 and 4' in switchCommand or 'two and four' in switchCommand:
                speak("socket 2 and 4 : onn")
                switch_2.write(1)
                switch_4.write(1)

            elif '3 and 1' in switchCommand or 'three and one' in switchCommand:
                speak("socket 3 and 1: onn")
                switch_3.write(1)
                switch_1.write(1)

            elif '3 and 2' in switchCommand or 'three and two' in switchCommand :
                speak("socket 3 and 2 : onn")
                switch_3.write(1)
                switch_2.write(1)

            elif '3 and 4' in switchCommand or 'three and four' in switchCommand:
                speak("socket 3 and 4: onn")
                switch_3.write(1)
                switch_4.write(1)

                


        elif 'turn off' in switchCommand or 'switch off' in switchCommand or ' of' in switchCommand:
            
            if 'first' in switchCommand :
                
                speak("socket 1 : off")
                switch_1.write(0)

            elif 'second' in switchCommand :
                speak("socket 2 : off")
                switch_2.write(0)

            elif 'third' in switchCommand:
                speak("socket 3: off")
                switch_3.write(0)

            elif 'fourth' in switchCommand or '4th' in switchCommand :
                speak("socket 4 : off")
                switch_4.write(0)

            elif 'all' in switchCommand :
                speak("all sockets : off")
                switch_1.write(0)
                switch_2.write(0)
                switch_3.write(0)
                switch_4.write(0)




        elif 'quit' in switchCommand or ' exit switch board' in switchCommand :
            speak("do you want me to off all sockets ?")
            binary = takeCommand().lower()

            if 'yes' in binary or 'please' in binary :

                speak("ohkay: all sockets: off")
                speak("shutting switch board")
                switch_1.write(0)
                switch_2.write(0)
                switch_3.write(0)
                switch_4.write(0)

                

            else :
                speak("ohkay , shutting switch board")

            
            break


            
        


        










    
        




        

