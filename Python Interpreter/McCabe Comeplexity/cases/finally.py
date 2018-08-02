def f():
  for x in range(3):
    if x < 3:
      print x
    else:
      print x+1


def getnum(val):
    if val > 3:
        try:
            val=int(raw_input("Enter a number "))
        except Exception, e:
            print "Error -", e
        finally: 
            print "finally"
    else:
        try:
            for x in range(3):
             if x < 3:
                print x
             else:
                print x+1
        except Exception, e:
            print "Error -", e
        finally:
            print "finally"

f(2)

getnum(4)
